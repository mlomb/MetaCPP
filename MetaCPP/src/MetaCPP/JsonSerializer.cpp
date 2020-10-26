#include "MetaCPP/JsonSerializer.hpp"

#include "MetaCPP/Type.hpp"

namespace metacpp {
	using namespace rapidjson;

	JsonSerializer::JsonSerializer(Storage* storage, bool use_references_table, int max_pointer_depth)
			: m_Storage(storage), m_UseReferencesTable(use_references_table), m_MaxPointerDepth(max_pointer_depth) {
	}

	static bool IsBasicType(const Type* type) {
		return type->IsPrimitive() || type->GetTypeID() == TypeInfo<std::string>::ID;
	}


	Value JsonSerializer::SerializeBasicType(const Type* type, void* ptr, SerializationContext& context) {
		Value value;
		value.SetNull();

		const TypeID id = type->GetTypeID();

		#define FIELD_SET(set_func, t) \
        else if (id == TypeInfo< t >::ID) \
			value.set_func(*reinterpret_cast< const t* >(ptr))

		if (false) {}
		FIELD_SET(SetBool, bool);
		FIELD_SET(SetInt, int);
		FIELD_SET(SetInt, char);
		FIELD_SET(SetInt64, long long);
		FIELD_SET(SetUint, unsigned int);
		FIELD_SET(SetUint64, unsigned long long);
		FIELD_SET(SetFloat, float);
		FIELD_SET(SetDouble, double);
		else if (id == TypeInfo<std::string>::ID) {
			const std::string* str = reinterpret_cast<const std::string*>(ptr);
			auto newValue = Value((*str).c_str(), context.document->GetAllocator());
			value.Swap(newValue);
		} else {
			// Unhandled basic type
		}

		return value;
	}

	Value JsonSerializer::SerializeType(const QualifiedType& qtype, void* ptr, const int pointer_recursion, SerializationContext& context) {
		// don't serialize const fields
		if (qtype.IsConst())
			return Value();

		const Type* type = context.serializer->GetStorage()->GetType(qtype.GetTypeID());
		if (type == 0)
			return Value();

		switch (qtype.GetQualifierOperator()) {
			case QualifierOperator::VALUE:
				assert(qtype.GetArraySize() == 1);
				if (type->GetArraySize() == 1) {
					if (IsBasicType(type)) {
						return SerializeBasicType(type, ptr, context);
					} else {
						return SerializeObject(type, ptr, false, pointer_recursion, context);
					}
				} else {
					Value array;
					array.SetArray();
					auto elemName = type->GetQualifiedName().ElementTypeQualified();
					auto elemType = m_Storage->GetType(elemName);
					QualifiedType elemQType = QualifiedType(elemType->GetTypeID(), QualifierOperator::VALUE, qtype.IsConst(), 1);
					for (size_t index = 0; index < type->GetArraySize(); ++index) {
						char* addr = (char*) ptr + elemType->GetSize() * index;
						array.PushBack(SerializeType(elemQType, addr, pointer_recursion, context), context.document->GetAllocator());
					}
					return array;
				}
			case QualifierOperator::POINTER: {
				assert(qtype.GetArraySize() == 1);
				assert(type->GetArraySize() == 1); // TODO: support array of pointers, maybe?
				void* pointee = *((void**) ptr);
				if (pointee != 0) {
					if (IsBasicType(type))
						return SerializeBasicType(type, pointee, context);
					else {
						auto derived = context.serializer->GetStorage()->ResolveDerivedType(type, pointee);
						return SerializeObject(derived.first, derived.second, true, pointer_recursion + 1, context);
					}
				}
				return Value();
			}
			case QualifierOperator::ARRAY: {
				assert(qtype.GetArraySize() > 1);
				Value array;
				array.SetArray();
				QualifiedType elemQType = QualifiedType(type->GetTypeID(), QualifierOperator::VALUE, qtype.IsConst(), 1);
				for (size_t index = 0; index < qtype.GetArraySize(); ++index) {
					char* addr = (char*) ptr + type->GetSize() * index;
					array.PushBack(SerializeType(elemQType, addr, pointer_recursion, context), context.document->GetAllocator());
				}
				return array;
			}
			default:
				// not supported
				return Value();
		}
	}

	Value JsonSerializer::SerializeObject(const Type* type, void* ptr, const bool is_reference, const int pointer_recursion, SerializationContext& context) {
		int index = -1;
		if (context.serializer->UsingReferencesTable()) {
			if (is_reference) {
				auto it = std::find(context.references.begin(), context.references.end(), ptr);
				if (it != context.references.end()) {
					return Value(it - context.references.begin());
				} else {
					context.document->PushBack(Value(), context.document->GetAllocator());
					context.references.push_back(ptr);
					index = (int) context.references.size() - 1;
				}
			}
		}
		if (pointer_recursion > context.serializer->MaxPointerDepth()) {
			// prevent infinite loops
			return Value();
		}

		Value object;
		object.SetNull();

		if (type->IsSequentialContainer()) {
			auto sc = static_cast<SequentialContainer*>(type->GetContainer());
			if (!sc)
				return object;

			size_t size = sc->Size(ptr);
			const QualifiedType item_qtype = sc->ValuesType();
			if (item_qtype.GetTypeID() == 0)
				return object;

			object.SetArray();
			for (int i = 0; i < size; i++) {
				void* item = sc->At(ptr, i);
				object.PushBack(SerializeType(item_qtype, item, pointer_recursion, context), context.document->GetAllocator());
			}

			return object;
		}
		if (type->IsSTL())
			return object;

		object.SetObject();

		if (type->IsPolymorphic()) {
			Value rc;
			rc.SetString(type->GetQualifiedName().GetTemplatedName(), context.document->GetAllocator());
			object.AddMember("reflection_class", rc, context.document->GetAllocator());
		}

		const std::vector<Field>& fields = context.serializer->GetStorage()->GetAllFields(type);

		for (const Field& field : fields) {
			const QualifiedType& field_qtype = field.GetType();

			void* field_ptr = field.Get<void>(ptr);
			Value field_value = SerializeType(field_qtype, field_ptr, pointer_recursion, context);

			if (!field_value.IsNull()) {
				Value field_name(field.GetQualifiedName().GetName().c_str(), context.document->GetAllocator());
				object.AddMember(field_name, field_value, context.document->GetAllocator());
			}
		}

		if (is_reference && context.serializer->UsingReferencesTable()) {
			context.document->GetArray()[index].Swap(object);
			return Value(index);
		}

		return object;
	}

	std::string JsonSerializer::Serialize(const Type* type, void* obj, const bool pretty) {
		Document document;

		SerializationContext context;
		context.serializer = this;
		context.document = &document;

		if (m_UseReferencesTable) {
			document.SetArray();
			SerializeObject(type, obj, true, 0, context);
		} else {
			auto serializedDocument = SerializeObject(type, obj, false, 0, context);
			document.Swap(serializedDocument);
		}

		StringBuffer sb;

		if (pretty) {
			PrettyWriter<StringBuffer> writer(sb);
			writer.SetFormatOptions(PrettyFormatOptions::kFormatSingleLineArray);
			document.Accept(writer);
		} else {
			Writer<StringBuffer> writer(sb);
			document.Accept(writer);
		}

		return sb.GetString();
	}

	void JsonSerializer::DeSerializeVoidPtr(void* ptr_data, void* ptr) {
		void** ptr_to_ptr = reinterpret_cast<void**>(ptr);
		*ptr_to_ptr = ptr_data;
	}

	void JsonSerializer::DeSerializeBasicType(const TypeID id, const Value& value, void* ptr) {
		#define FIELD_GET(is_func, t, get_func) \
        else if (value.is_func() && id == TypeInfo< t >::ID) \
			*reinterpret_cast<t*>(ptr) = value.get_func()

		if (false) {}
		FIELD_GET(IsBool, bool, GetBool);
		FIELD_GET(IsInt, int, GetInt);
		FIELD_GET(IsInt, char, GetInt);
		FIELD_GET(IsInt64, long long, GetInt64);
		FIELD_GET(IsUint, unsigned int, GetUint);
		FIELD_GET(IsUint64, unsigned long long, GetUint64);
		FIELD_GET(IsFloat, float, GetFloat);
		FIELD_GET(IsDouble, double, GetDouble);
		FIELD_GET(IsString, std::string, GetString);
		else {
			// not supported
		}
	}

	std::pair<const Type*, void*> JsonSerializer::Allocate(const Type* type, const Value& value, SerializationContext& context) {
		const Type* type_to_deserialize = type;

		if (value.IsObject()) {
			auto it = value.FindMember("reflection_class");
			if (it != value.MemberEnd()) {
				const Value& rc = it->value;
				if (rc.IsString()) {
					std::string derived_name = rc.GetString();
					Type* derived_type = context.serializer->GetStorage()->FindDerivedTypeWithName(type, derived_name);
					if (derived_type)
						type_to_deserialize = derived_type;
				}
			}
		}

		void* ptr = type_to_deserialize->Allocate();

		return std::make_pair(type_to_deserialize, ptr);
	}

	void JsonSerializer::DeSerializePointer(const Type* type, const Value& value, void* obj, SerializationContext& context) {
		if (value.IsInt() && context.serializer->UsingReferencesTable()) { // its a reference
			void* new_obj = context.references[value.GetInt()];
			if (new_obj == 0) {
				Value& ref_object = (*context.document)[value.GetInt()];

				auto instance = Allocate(type, ref_object, context);
				new_obj = instance.second;

				context.references[value.GetInt()] = new_obj;

				DeSerializeObject(instance.first, ref_object, new_obj, context);
			}
			DeSerializeVoidPtr(new_obj, obj);
		} else if (value.IsObject() && !context.serializer->UsingReferencesTable()) {
			auto instance = Allocate(type, value, context);
			DeSerializeVoidPtr(instance.second, obj);
			DeSerializeObject(instance.first, value, instance.second, context);
		}
	}


	void JsonSerializer::DeSerializeType(const QualifiedType& qtype, const Value& value, void* obj, SerializationContext& context) {
		if (value.IsNull())
			return;

		Type* type = context.serializer->GetStorage()->GetType(qtype.GetTypeID());
		TypeID id = type->GetTypeID();

		switch (qtype.GetQualifierOperator()) {
			case QualifierOperator::VALUE: {
				assert(qtype.GetArraySize() == 1);
				if (type->GetArraySize() == 1) {
					DeSerializeValue(value, obj, context, type, id);
				} else {
					auto elemName = type->GetQualifiedName().ElementTypeQualified();
					auto elemType = m_Storage->GetType(elemName);
					QualifiedType elemQType = QualifiedType(elemType->GetTypeID(), QualifierOperator::VALUE, qtype.IsConst(), 1);
					for (size_t index = 0; index < type->GetArraySize(); ++index) {
						char* addr = (char*) obj + elemType->GetSize() * index;
						DeSerializeType(elemQType, value[index], addr, context);
					}
				}
				break;
			}
			case QualifierOperator::POINTER:
				assert(qtype.GetArraySize() == 1);
				assert(type->GetArraySize() == 1); // TODO: support array of pointers, maybe?
				DeSerializePointer(type, value, obj, context);
				break;
			case QualifierOperator::ARRAY: {
				assert(qtype.GetArraySize() > 1);
				QualifiedType elemQType = QualifiedType(type->GetTypeID(), QualifierOperator::VALUE, qtype.IsConst(), 1);
				for (size_t index = 0; index < qtype.GetArraySize(); ++index) {
					char* addr = (char*) obj + type->GetSize() * index;
					DeSerializeType(elemQType, value[index], addr, context);
				}
				break;
			}
			default:
				// unsupported
				break;
		}
	}

	void JsonSerializer::DeSerializeValue(const Value& value, void* obj, SerializationContext& context, const Type* type, TypeID id) {
		if (IsBasicType(type)) {
			DeSerializeBasicType(id, value, obj);
		} else if (value.IsObject()) {
			DeSerializeObject(type, value, obj, context);
		} else if (type->IsSequentialContainer() && value.IsArray()) {
			auto sc = static_cast<SequentialContainer*>(type->GetContainer());
			if (sc) {
				const QualifiedType item_qtype = sc->ValuesType();
				if (item_qtype.GetTypeID() != 0) {
					Type* item_type = context.serializer->GetStorage()->GetType(item_qtype.GetTypeID());
					if (item_type) {
						DeSerializeArray(value, obj, context, sc, item_qtype, item_type);
					}
				}
			}
		}
	}

	void JsonSerializer::DeSerializeArray(const Value& value, void* obj, SerializationContext& context, const SequentialContainer* sc,
	                                      const QualifiedType& item_qtype, const Type* item_type) {
		for (const Value& item : value.GetArray()) {
			switch (item_qtype.GetQualifierOperator()) {
				case QualifierOperator::VALUE:
				case QualifierOperator::ARRAY: {
					void* temp_item_ptr = item_type->Allocate();

					DeSerializeType(item_qtype, item, temp_item_ptr, context);

					sc->PushBack(obj, temp_item_ptr);

					item_type->Delete(temp_item_ptr);
					break;
				}
				case QualifierOperator::POINTER: {
					void* holder = 0;
					DeSerializePointer(item_type, item, &holder, context);

					sc->PushBack(obj, &holder);
					break;
				}
				case QualifierOperator::REFERENCE: {
					assert(false);
					break;
				}
			}
		}
	}

	void JsonSerializer::DeSerializeObject(const Type* type, const Value& value, void* obj, SerializationContext& context) {
		const std::vector<Field>& fields = context.serializer->GetStorage()->GetAllFields(type);

		for (auto itr = value.MemberBegin(); itr != value.MemberEnd(); ++itr) {
			auto& member = *itr;

			for (const Field& field : fields) {
				std::string field_name = field.GetQualifiedName().GetName();
				if (field_name == member.name) {
					void* field_ptr = field.Get<void>(obj);
					DeSerializeType(field.GetType(), member.value, field_ptr, context);
					break;
				}
			}
		}
	}

	void JsonSerializer::DeSerialize(const Type* type, const std::string& json, void* obj) {
		Document document;
		document.Parse(json);

		SerializationContext context;
		context.serializer = this;
		context.document = &document;

		if (m_UseReferencesTable && document.IsArray()) {
			if (document.Size() > 0) {
				context.references.resize(document.Size());
				std::fill(context.references.begin(), context.references.end(), reinterpret_cast<void*>(0));

				DeSerializeObject(type, document[0], obj, context);
			}
		} else if (!m_UseReferencesTable && document.IsObject()) {
			DeSerializeObject(type, document, obj, context);
		}
	}

	Storage* JsonSerializer::GetStorage() {
		return m_Storage;
	}

	bool JsonSerializer::UsingReferencesTable() {
		return m_UseReferencesTable;
	}

	int JsonSerializer::MaxPointerDepth() {
		return m_MaxPointerDepth;
	}
}
