#ifndef METACPP_STORAGE_HPP
#define METACPP_STORAGE_HPP

#include <vector>
#include <map>

#include "Type.hpp"
#include "Dumpeable.hpp"

namespace metacpp {
	class Storage : public Dumpeable {
	public:
		Storage();

		ID getID(const IDTypes idType, const std::string& name);
		TypeID getTypeID(const std::string& name);
		FieldID getFieldID(const std::string& name);

		void* get(const IDTypes idType, const ID id) const;
		Type* getType(const TypeID typeId) const;
		Field* getField(const FieldID fieldId) const;

		ID assignID(const IDTypes idType, const std::string& name);
		TypeID assignTypeID(const std::string& name);
		FieldID assignFieldID(const std::string& name);

		void add(const IDTypes idType, const ID id, void* ptr);
		void addType(Type* type);
		void addField(Field* field);

		bool has(const IDTypes idType, const ID id) const;
		bool hasType(const TypeID typeId) const;
		bool hasField(const FieldID fieldId) const;
		
	private:
		std::map<IDTypes, std::map<std::string, ID>>	m_IDs;
		std::map<IDTypes, std::map<ID,			void*>> m_Objects;

		ID m_NextID = 1;

		void dump_obj(std::ostream& o) override;
	};
}

#endif