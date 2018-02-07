#ifndef METACPP_QUALIFIEDTYPE_HPP
#define METACPP_QUALIFIEDTYPE_HPP

#include <string>

#include "TypeID.hpp"

namespace metacpp {
	class Storage;

	enum QualifierOperator {
		POINTER,
		REFERENCE,
		VALUE
	};

	class QualifiedType {
	public:
		QualifiedType();

		void SetTypeID(const TypeID typeID);
		void SetQualifierOperator(const QualifierOperator qualifierOperator);
		void SetConst(const bool is_const);

		TypeID GetTypeID() const;
		QualifierOperator GetQualifierOperator() const;
		bool IsConst() const;

		std::string GetQualifiedName(const Storage* storage) const;

	protected:
		QualifierOperator m_Operator;
		bool m_Const;
		TypeID m_Type;

		friend class MetaExporter;
	};
}

#endif