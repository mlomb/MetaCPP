#ifndef METACPP_JSON_SERIALIZER_HPP
#define METACPP_JSON_SERIALIZER_HPP

#include <string>

#include "TypeInfo.hpp"
#include "Storage.hpp"

namespace metacpp {
	class JsonSerializer {
	public:
		JsonSerializer(Storage* storage, bool use_references_table = false, int max_pointer_depth = 7);

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
		void* ptr = type->allocate();
		DeSerialize(type, json, ptr);
		return reinterpret_cast<T*>(ptr);
	}
}

#endif