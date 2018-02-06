#ifndef METACPP_FIELD_HPP
#define METACPP_FIELD_HPP

#include <vector>
#include <string>

#include "TypeID.hpp"
#include "QualifiedName.hpp"
#include "QualifiedType.hpp"
#include "Exportable.hpp"

namespace metacpp {
	class Field : public Exportable {
	public:
		Field(QualifiedType* type, const QualifiedName& qName);

		QualifiedType* getType() const;
		TypeID getOwnerType() const;
		size_t getOffset() const;
		const QualifiedName& getQualifiedName() const;

		void setOwner(const TypeID ownerTypeId);
		void setOffset(const size_t bytes);

		template<typename T>
		T* get(const void* object) const;
		template<typename T>
		void set(const T& value, const void* object) const;

		mustache::data asMustache() const override;
	private:
		TypeID m_Owner;
		QualifiedType* m_Type;
		QualifiedName m_QualifiedName;

		size_t m_OffsetInBytes;
	};

	template<typename T>
	inline T* Field::get(const void* object) const
	{
		return reinterpret_cast<T*>((size_t)object + m_OffsetInBytes);
	}

	template<typename T>
	inline void Field::set(const T& value, const void* object) const
	{
		T& obj_value = *reinterpret_cast<T*>((size_t)object + m_OffsetInBytes);
		obj_value = value;
	}
}

#endif