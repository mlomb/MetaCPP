#ifndef METACPP_QUALIFIEDTYPE_HPP
#define METACPP_QUALIFIEDTYPE_HPP

#include <string>
#include <iostream>
#include <variant>

#include "TypeID.hpp"

namespace metacpp {
	class Storage;

	enum class QualifierOperator {
		POINTER,
		REFERENCE,
		VALUE,
		ARRAY
	};

	class QualifiedType {
	public:
		QualifiedType();
		QualifiedType(TypeID typeID, QualifierOperator qualifierOperator, bool is_const, size_t arraySize);

		void SetTypeID(const TypeID typeID);
		void SetQualifierOperator(const QualifierOperator qualifierOperator);
		void SetConst(const bool is_const);
		void SetArraySize(std::size_t arraySize);

		TypeID GetTypeID() const;
		QualifierOperator GetQualifierOperator() const;
		bool IsConst() const;
		std::size_t GetArraySize() const;

		std::string GetQualifiedName(const Storage* storage) const;

	protected:
		QualifierOperator m_Operator;
		bool m_Const;
		TypeID m_Type;
		std::size_t m_ArraySize;

		friend class MetaExporter;
	};

	using TemplateArgument = std::variant<QualifiedType, unsigned long long>;
}

#endif