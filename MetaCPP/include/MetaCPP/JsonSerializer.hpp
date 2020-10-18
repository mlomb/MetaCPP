#ifndef METACPP_JSON_SERIALIZER_HPP
#define METACPP_JSON_SERIALIZER_HPP

#include <string>

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include "MetaCPP/SequentialContainer.hpp"
#include "TypeInfo.hpp"
#include "Storage.hpp"

namespace metacpp {
	class JsonSerializer {
	public:
		JsonSerializer(Storage* storage, bool use_references_table = true, int max_pointer_depth = 7);

		template<typename T>
		std::string Serialize(T* obj, const bool pretty = false);
		template<typename T>
		T* DeSerialize(const std::string& json);

		std::string Serialize(const Type* type, void* obj, const bool pretty = false);
		void DeSerialize(const Type* type, const std::string& json, void* obj);

		Storage* GetStorage();
		bool UsingReferencesTable();
		int MaxPointerDepth();
	private:
		Storage* m_Storage;

		bool m_UseReferencesTable;
		int m_MaxPointerDepth;

		struct SerializationContext {
			JsonSerializer *serializer;

			rapidjson::Document *document;
			std::vector<void *> references;
		};

		std::pair<const Type *, void *> Allocate(const Type *type, const rapidjson::Value &value, SerializationContext &context);

		rapidjson::Value SerializeBasicType(const Type *type, void *ptr, SerializationContext &context);
		rapidjson::Value SerializeType(const QualifiedType &qtype, void *ptr, const int pointer_recursion, SerializationContext &context);
		rapidjson::Value SerializeObject(const Type *type, void *ptr, const bool is_reference, const int pointer_recursion, SerializationContext &context);

		void DeSerializeArray(const rapidjson::Value &value, void *obj, SerializationContext &context, const SequentialContainer *sc, const QualifiedType &item_qtype, const Type *item_type);
		void DeSerializeValue(const rapidjson::Value &value, void *obj, SerializationContext &context, const Type *type, TypeID id);
		void DeSerializeVoidPtr(void *ptr_data, void *ptr);
		void DeSerializePointer(const Type *type, const rapidjson::Value &value, void *obj, SerializationContext &context);
		void DeSerializeBasicType(const TypeID id, const rapidjson::Value &value, void *ptr);
		void DeSerializeType(const QualifiedType &qtype, const rapidjson::Value &value, void *obj, SerializationContext &context);
		void DeSerializeObject(const Type *type, const rapidjson::Value &value, void *obj, SerializationContext &context);

	};

	template<typename T>
	inline std::string JsonSerializer::Serialize(T* obj, const bool pretty)
	{
		Type* type = TypeInfo<T>::TYPE;
		if (type == nullptr) {
			return std::string("{}");
		}
		return Serialize(type, obj, pretty);
	}

	template<typename T>
	inline T* JsonSerializer::DeSerialize(const std::string& json)
	{
		Type* type = TypeInfo<T>::TYPE;
		if (type == nullptr) {
			return 0;
		}
		void* ptr = type->Allocate();
		DeSerialize(type, json, ptr);
		return reinterpret_cast<T*>(ptr);
	}
}

#endif