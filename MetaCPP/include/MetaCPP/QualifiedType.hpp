#ifndef METACPP_QUALIFIEDTYPE_HPP
#define METACPP_QUALIFIEDTYPE_HPP

#include <string>

#include "TypeID.hpp"
#include "Exportable.hpp"

namespace metacpp {
	class Storage;

	enum QualifierOperator {
		POINTER,
		REFERENCE,
		VALUE
	};

	class QualifiedType : public Exportable {
	public:
		QualifiedType();

		void setTypeID(const TypeID typeID);
		void setQualifierOperator(const QualifierOperator qualifierOperator);
		void setConst(const bool is_const);

		TypeID getTypeID() const;
		QualifierOperator getQualifierOperator() const;
		bool isConst() const;

		std::string getQualifiedName(const Storage* storage) const;

		mustache::data asMustache() const override;

	private:
		std::string dumpTemplate() override;
	protected:
		QualifierOperator m_Operator;
		bool m_Const;
		TypeID m_Type;
	};
}

#endif