#ifndef METACPP_STORAGE_HPP
#define METACPP_STORAGE_HPP

#include <map>

#include "Type.hpp"
#include "TypeInfo.hpp"
#include "Exportable.hpp"

namespace metacpp {
	class Storage : public Exportable {
	public:
		Storage();

		TypeID assignTypeID(const std::string& name, const TypeID typeId = 0);
		void addType(Type* type);

		TypeID getTypeID(const std::string& name) const;
		Type* getType(const TypeID typeId) const;
		Type* getType(const std::string& name) const;
		template<typename T>
		Type* getType() const;

		bool hasType(const TypeID typeId) const;

		mustache::data asMustache() const override;
	private:
		std::string dumpTemplate() override;

		std::map<std::string, TypeID> m_IDs;
		std::map<TypeID, Type*> m_Types;

		TypeID m_NextID = 1;
	};

	template<typename T>
	inline Type* Storage::getType() const
	{
		return TypeInfo<T>::TYPE;
	}
}

#endif