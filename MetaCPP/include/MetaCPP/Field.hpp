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
		const QualifiedName& getQualifiedName() const;

		void setOwner(const TypeID ownerTypeId);
		void setOffset(const unsigned int bytes);

		mustache::data Field::asMustache() const override;
	private:
		TypeID m_Owner;
		QualifiedType* m_Type;
		QualifiedName m_QualifiedName;

		unsigned int m_OffsetInBytes;
	};
}

#endif