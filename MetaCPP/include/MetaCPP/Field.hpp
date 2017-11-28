#ifndef METACPP_FIELD_HPP
#define METACPP_FIELD_HPP

#include <vector>
#include <string>

#include "IDs.hpp"
#include "Dumpeable.hpp"

namespace metacpp {
	class Field : public Dumpeable {
	public:
		Field(const FieldID id, const TypeID type, const TypeID owner, const std::string fullName, const std::string name);

		FieldID getID() const;
		TypeID getType() const;
		TypeID getOwnerType() const;
		std::string getName() const;
		std::string getFullName() const;

		void setType(const TypeID typeId);
		void setOwner(const TypeID ownerTypeId);
		void setOffset(const unsigned int bytes);

	private:
		const FieldID m_ID;
		TypeID m_Type;
		TypeID m_Owner;
		std::string m_FullName;
		std::string m_Name;
		
		unsigned int m_OffsetInBytes;

		void dump_obj(std::ostream& o) override;
	};
}

#endif