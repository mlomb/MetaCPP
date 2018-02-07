#ifndef METACPP_FIELD_HPP
#define METACPP_FIELD_HPP

#include <vector>
#include <string>

#include "TypeID.hpp"
#include "QualifiedName.hpp"
#include "QualifiedType.hpp"

namespace metacpp {
	class Field {
	public:
		Field(const QualifiedType& type, const QualifiedName& qName);

		const QualifiedType& GetType() const;
		TypeID GetOwnerType() const;
		size_t GetOffset() const;
		const QualifiedName& GetQualifiedName() const;

		void SetOwner(const TypeID ownerTypeId);
		void SetOffset(const size_t bytes);

		template<typename T>
		T* Get(const void* object) const;
		template<typename T>
		void Set(const T& value, const void* object) const;

	private:
		TypeID m_Owner;
		QualifiedType m_Type;
		QualifiedName m_QualifiedName;

		size_t m_OffsetInBytes;

		friend class MetaExporter;
	};

	template<typename T>
	inline T* Field::Get(const void* object) const
	{
		return reinterpret_cast<T*>((size_t)object + m_OffsetInBytes);
	}

	template<typename T>
	inline void Field::Set(const T& value, const void* object) const
	{
		T& obj_value = *reinterpret_cast<T*>((size_t)object + m_OffsetInBytes);
		obj_value = value;
	}
}

#endif