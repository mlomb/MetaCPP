#ifndef METACPP_METHOD_HPP
#define METACPP_METHOD_HPP

#include <vector>
#include <string>

#include "TypeID.hpp"
#include "QualifiedName.hpp"
#include "QualifiedType.hpp"
#include "Exportable.hpp"

namespace metacpp {
	class MethodParameter {
	public:
		MethodParameter(const std::string& name, const QualifiedType& type);

	private:
		std::string m_Name;
		QualifiedType m_Type;
	};

	class Method : public Exportable {
	public:
		Method(const QualifiedName& qName);

		TypeID getOwnerType() const;
		const QualifiedName& getQualifiedName() const;

		void setOwner(const TypeID ownerTypeId);

		void addParameter(const MethodParameter& parameter);

	private:
		TypeID m_Owner;
		QualifiedName m_QualifiedName;

		bool m_Constructor, m_Destructor, m_Static, m_Virtual;
		std::vector<MethodParameter> m_Parameters;
	};
}

#endif