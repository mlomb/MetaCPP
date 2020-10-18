#ifndef METACPP_METHOD_HPP
#define METACPP_METHOD_HPP

#include <vector>
#include <string>

#include "TypeID.hpp"
#include "QualifiedName.hpp"
#include "QualifiedType.hpp"

namespace metacpp {
	class MethodParameter {
	public:
		MethodParameter(const std::string &name, const QualifiedType &type);

		const std::string &GetName() const;
		const QualifiedType &GetType() const;

	private:
		std::string m_Name;
		QualifiedType m_Type;
	};

	class Method {
	public:
		Method(const QualifiedName &qName);

		TypeID GetOwnerType() const;
		const QualifiedName &GetQualifiedName() const;

		void SetOwner(const TypeID ownerTypeId);

		void AddParameter(const MethodParameter &parameter);

	private:
		TypeID m_Owner;
		QualifiedName m_QualifiedName;

		bool m_Static, m_Virtual;
		std::vector<MethodParameter> m_Parameters;

		friend class MetaExporter;
	};
}

#endif