#include "MetaCPP/JsonSerializer.hpp"

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include "MetaCPP/Type.hpp"
#include "MetaCPP/SequentialContainer.hpp"

namespace metacpp {
	using namespace rapidjson;

	JsonSerializer::JsonSerializer(Storage* storage, bool use_references_table, int max_pointer_depth)
		: m_Storage(storage), m_UseReferencesTable(use_references_table), m_MaxPointerDepth(max_pointer_depth)
	{
	}

	struct SerializationContext {
		JsonSerializer* serializer;

		Document* document;
		std::vector<void*> references;
	};

	static bool IsBasicType(const Type* type) {
		return type->isPrimitive() || type->getID() == TypeInfo<std::string>::ID;
	}

	Value SerializeBasicType(const Type* type, void* ptr, SerializationContext& context);
	Value SerializeType(const QualifiedType* qtype, void* ptr, const int pointer_recursion, SerializationContext& context);
	Value SerializeObject(const Type* type, void* ptr, const bool is_reference, const int pointer_recursion, SerializationContext& context);

	Value SerializeBasicType(const Type* type, void* ptr, SerializationContext& context) {
		Value value;
		value.SetNull();

		const TypeID id = type->getID();

		#define FIELD_SET(set_func, t) \
		else if (id == TypeInfo< t >::ID) { value.set_func(*reinterpret_cast< const t* >(ptr)); }

		if (false) {}
		FIELD_SET(SetBool, bool)
		FIELD_SET(SetInt, int)
		FIELD_SET(SetInt, char)
		FIELD_SET(SetInt64, long long)
		FIELD_SET(SetUint, unsigned int)
		FIELD_SET(SetUint64, unsigned long long)
		FIELD_SET(SetFloat, float)
		FIELD_SET(SetDouble, double)
		else if(id == TypeInfo<std::string>::ID) {
			const std::string* str = reinterpret_cast<const std::string*>(ptr);
			value.Swap(Value((*str).c_str(), context.document->GetAllocator()));
		}
		else {
			// Unhandled basic type
		}

		return value;
	}

	Value SerializeType(const QualifiedType* qtype, void* ptr, const int pointer_recursion, SerializationContext& context) {
		// don't serialize const fields
		if (qtype->isConst())
			return Value();

		const Type* type = context.serializer->GetStorage()->getType(qtype->getTypeID());

		if(type == 0)
			return Value();

		switch (qtype->getQualifierOperator())
		{
		case QualifierOperator::VALUE:
			if (IsBasicType(type)) {
				return SerializeBasicType(type, ptr, context);
			}
			else {
				return SerializeObject(type, ptr, false, pointer_recursion, context);
			}
		case QualifierOperator::POINTER:
		{
			void* pointee = *((void**)ptr);
			if (pointee != 0) {
				if(IsBasicType(type))
					return SerializeBasicType(type, pointee, context);
				else {
					auto derived = context.serializer->GetStorage()->ResolveDerivedType(type, pointee);
					return SerializeObject(derived.first, derived.second, true, pointer_recursion + 1, context);
				}
			}
		}
		default:
			// not supported
			return Value();
		}
	}

	Value SerializeObject(const Type* type, void* ptr, const bool is_reference, const int pointer_recursion, SerializationContext& context) {
		int index = -1;
		if (context.serializer->UsingReferencesTable()) {
			if (is_reference) {
				auto it = std::find(context.references.begin(), context.references.end(), ptr);
				if (it != context.references.end()) {
					return Value(it - context.references.begin());
				} else {
					context.document->PushBack(Value(), context.document->GetAllocator());
					context.references.push_back(ptr);
					index = (int)context.references.size() - 1;
				}
			}
		}
		if (pointer_recursion > context.serializer->MaxPointerDepth()) {
			// prevent infinite loops
			return Value();
		}

		Value object;
		object.SetNull();
		
		if (type->isSequentialContainer()) {
			auto sc = static_cast<SequentialContainer*>(type->getContainer());
			if (!sc)
				return object;

			size_t size = sc->Size(ptr);
			QualifiedType* item_qtype = sc->ValuesType();
			if (!item_qtype)
				return object;

			object.SetArray();
			for (int i = 0; i < size; i++) {
				void* item = sc->At(ptr, i);
				object.PushBack(SerializeType(item_qtype, item, pointer_recursion, context), context.document->GetAllocator());
			}

			return object;
		}
		if (type->isSTL())
			return object;

		object.SetObject();

		if (type->isPolymorphic()) {
			Value rc;
			rc.SetString(type->getQualifiedName().getTemplatedName(), context.document->GetAllocator());
			object.AddMember("reflection_class", rc, context.document->GetAllocator());
		}

		std::vector<Field*> fields = context.serializer->GetStorage()->getAllFields(type);

		for (Field* field : fields) {
			const QualifiedType* field_qtype = field->getType();

			void* field_ptr = field->get<void*>(ptr);
			Value field_value = SerializeType(field_qtype, field_ptr, pointer_recursion, context);

			if (field_value.IsNull())
				continue;

			Value field_name(field->getQualifiedName().getName().c_str(), context.document->GetAllocator());

			object.AddMember(field_name, field_value, context.document->GetAllocator());
		}
		
		if (is_reference && context.serializer->UsingReferencesTable()) {
			context.document->GetArray()[index].Swap(object);
			return Value(index);
		}

		return object;
	}
	
	std::string JsonSerializer::Serialize(const Type* type, void* obj, const bool pretty)
	{
		Document document;

		SerializationContext context;
		context.serializer = this;
		context.document = &document;

		if (m_UseReferencesTable) {
			document.SetArray();
			SerializeObject(type, obj, true, 0, context);
		}
		else {
			document.Swap(SerializeObject(type, obj, false, 0, context));
		}

		StringBuffer sb;
		
		if (pretty) {
			PrettyWriter<StringBuffer> writer(sb);
			writer.SetFormatOptions(PrettyFormatOptions::kFormatSingleLineArray);
			document.Accept(writer);
		}
		else {
			Writer<StringBuffer> writer(sb);
			document.Accept(writer);
		}

		return sb.GetString();
	}

	void DeSerializeVoidPtr(void* ptr_data, void* ptr);
	void DeSerializeBasicType(const TypeID id, const Value& value, void* ptr);
	void DeSerializeType(const QualifiedType* qtype, const Value& value, void* obj, SerializationContext& context);
	void DeSerializeObject(const Type* type, const Value& value, void* obj, SerializationContext& context);

	void DeSerializeVoidPtr(void* ptr_data, void* ptr)
	{
		void** ptr_to_ptr = reinterpret_cast<void**>(ptr);
		*ptr_to_ptr = ptr_data;
	}

	void DeSerializeBasicType(const TypeID id, const Value& value, void* ptr) {
		#define FIELD_GET(is_func, t, get_func) \
		else if (value.is_func() && id == TypeInfo< t >::ID) { \
			t& obj_value = *reinterpret_cast<t*>(ptr); \
			obj_value = value.get_func(); \
		} \

		if (false) {}
		FIELD_GET(IsBool, bool, GetBool)
		FIELD_GET(IsInt, int, GetInt)
		FIELD_GET(IsInt, char, GetInt)
		FIELD_GET(IsInt64, long long, GetInt64)
		FIELD_GET(IsUint, unsigned int, GetUint)
		FIELD_GET(IsUint64, unsigned long long, GetUint64)
		FIELD_GET(IsFloat, float, GetFloat)
		FIELD_GET(IsDouble, double, GetDouble)
		FIELD_GET(IsString, std::string, GetString)
		else {
			// not supported
		}
	}

	std::pair<const Type*, void*> Allocate(const Type* type, const Value& value, SerializationContext& context) {
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

		void* ptr = type_to_deserialize->allocate();

		return std::make_pair(type_to_deserialize, ptr);
	}

	void DeSerializePointer(const Type* type, const Value& value, void* obj, SerializationContext& context) {
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
		}
		else if (value.IsObject() && !context.serializer->UsingReferencesTable()) {
			auto instance = Allocate(type, value, context);
			DeSerializeVoidPtr(instance.second, obj);
			DeSerializeObject(instance.first, value, instance.second, context);
		}
	}

	void DeSerializeType(const QualifiedType* qtype, const Value& value, void* obj, SerializationContext& context) {
		if (value.IsNull())
			return;

		Type* type = context.serializer->GetStorage()->getType(qtype->getTypeID());
		TypeID id = type->getID();

		switch (qtype->getQualifierOperator()) {
		case QualifierOperator::VALUE:
			if (IsBasicType(type)) {
				DeSerializeBasicType(id, value, obj);
			}
			else if (value.IsObject()) {
				DeSerializeObject(type, value, obj, context);
			}
			else if (type->isSequentialContainer() && value.IsArray()) {
				auto sc = static_cast<SequentialContainer*>(type->getContainer());
				if (!sc) return;

				QualifiedType* item_qtype = sc->ValuesType();
				if (!item_qtype) return;
				Type* item_type = context.serializer->GetStorage()->getType(item_qtype->getTypeID());
				if (!item_type) return;

				for (const Value& item : value.GetArray()) {
					switch (item_qtype->getQualifierOperator()) {
					case QualifierOperator::VALUE:
					{
						void* temp_item_ptr = item_type->allocate();

						DeSerializeType(item_qtype, item, temp_item_ptr, context);

						sc->PushBack(obj, temp_item_ptr);

						delete temp_item_ptr;
						break;
					}
					case QualifierOperator::POINTER:
						void* holder = 0;
						DeSerializePointer(item_type, item, &holder, context);

						sc->PushBack(obj, &holder);
						break;
					}
				}
			}
			break;
		case QualifierOperator::POINTER:
			DeSerializePointer(type, value, obj, context);
			break;
		default:
			// unsupported
			break;
		}
	}

	void DeSerializeObject(const Type* type, const Value& value, void* obj, SerializationContext& context) {
		const std::vector<Field*>& fields = type->getFields();

		for (auto itr = value.MemberBegin(); itr != value.MemberEnd(); ++itr) {
			auto& member = *itr;

			for (Field* field : fields) {
				std::string field_name = field->getQualifiedName().getName();
				if (field_name == member.name) {
					void* field_ptr = field->get<void*>(obj);
					DeSerializeType(field->getType(), member.value, field_ptr, context);
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
		}
		else if (!m_UseReferencesTable && document.IsObject()) {
			DeSerializeObject(type, document, obj, context);
		}
	}
	
	Storage* JsonSerializer::GetStorage()
	{
		return m_Storage;
	}

	bool JsonSerializer::UsingReferencesTable()
	{
		return m_UseReferencesTable;
	}

	int JsonSerializer::MaxPointerDepth()
	{
		return m_MaxPointerDepth;
	}
}
