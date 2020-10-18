#include "MetaCPP/Field.hpp"

#include "MetaCPP/Storage.hpp"

namespace metacpp {
	Field::Field(const QualifiedType &type, const QualifiedName &qName)
			: m_Type(type),
			  m_QualifiedName(qName),
			  m_OffsetInBytes(0),
			  m_Owner(0) {
	}

	const QualifiedType &Field::GetType() const {
		return m_Type;
	}

	TypeID Field::GetOwnerType() const {
		return m_Owner;
	}

	size_t Field::GetOffset() const {
		return m_OffsetInBytes;
	}

	const QualifiedName &Field::GetQualifiedName() const {
		return m_QualifiedName;
	}

	void Field::SetOwner(const TypeID ownerTypeId) {
		m_Owner = ownerTypeId;
	}

	void Field::SetOffset(const size_t bytes) {
		m_OffsetInBytes = bytes;
	}
}
