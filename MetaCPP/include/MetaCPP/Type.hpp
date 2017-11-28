#ifndef METACPP_TYPE_HPP
#define METACPP_TYPE_HPP

#include <vector>

#include "IDs.hpp"
#include "Field.hpp"
//#include "Method.hpp"
#include "Dumpeable.hpp"

namespace metacpp {
	enum TypeKind {
		PRIMITIVE,
		CLASS,
		STRUCT
	};

	class Type : public Dumpeable {
	public:
		Type(const TypeID id, const std::string& fullName, const std::string& name);

		TypeID getID() const;
		std::string getName() const;
		std::string getFullName() const;
		TypeKind getKind() const;
		bool isPrimitive() const;
		bool isStruct() const;
		bool isClass() const;

		void setKind(const TypeKind kind);
		void setSize(const unsigned int bytes);

		void addField(const FieldID field);
		void addMethod(const MethodID method);

	private:
		const TypeID m_ID;
		const std::string m_Name, m_FullName;
		unsigned int m_SizeInBytes = -1;
		TypeKind m_Kind = PRIMITIVE;

		std::vector<FieldID> m_Fields;
		std::vector<MethodID> m_Methods;

		void dump_obj(std::ostream& o) override;
	};
}

#endif