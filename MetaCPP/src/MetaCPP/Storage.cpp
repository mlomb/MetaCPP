#include "MetaCPP/Storage.hpp"

#include <iostream>
#include <algorithm>

namespace metacpp {

	Storage::Storage() {

	}

	Storage::~Storage() {

	}

	TypeID Storage::GetTypeID(const std::string &name) const {
		auto it = m_IDs.find(name);
		return it != m_IDs.end() ? (*it).second : 0;
	}

	Type *Storage::GetType(const TypeID typeId) const {
		auto it = m_Types.find(typeId);
		return it != m_Types.end() ? (*it).second : nullptr;
	}

	Type *Storage::GetType(const std::string &name) const {
		return GetType(GetTypeID(name));
	}

	TypeID Storage::AssignTypeID(const std::string &name, const TypeID typeId) {
		TypeID _id = GetTypeID(name);
		if (_id != 0 && (typeId == 0 || _id == typeId))
			return _id;

		_id = typeId;
		if (typeId == 0)
			_id = m_NextID++;
		else
			m_NextID = std::max(m_NextID, _id) + 1;
		m_IDs.insert(std::make_pair(name, _id));
		return _id;
	}

	void Storage::AddType(Type *type) {
		m_Types.insert(std::make_pair(type->GetTypeID(), type));
	}

	void Storage::AddDynamicCast(TypeID base, TypeID derived, const DynamicCast &dc) {
		int index = (size_t) base << 32 | (unsigned int) derived;
		m_DynamicCasts.insert(std::make_pair(index, dc));
	}

	bool Storage::HasType(const TypeID typeId) const {
		return m_Types.count(typeId);
	}

	bool Storage::IsDerived(const TypeID derived, const TypeID base) const {
		Type *derivedType = GetType(derived);
		if (derivedType == 0)
			return false;

		auto &baseTypes = derivedType->GetBaseTypes();
		for (auto &baseType : baseTypes) {
			if (baseType.type.GetTypeID() == base)
				return true;
			if (IsDerived(baseType.type.GetTypeID(), base))
				return true;
		}
		return false;
	}

	std::vector<Field> Storage::GetAllFields(const Type *type) {
		std::vector<Field> fields;

		for (auto &baseType : type->GetBaseTypes()) {
			const std::vector<Field> &parentFields = GetAllFields(GetType(baseType.type.GetTypeID()));
			fields.reserve(fields.size() + parentFields.size());
			fields.insert(fields.end(), parentFields.begin(), parentFields.end());
		}

		const std::vector<Field> &typeFields = type->GetFields();

		fields.reserve(fields.size() + typeFields.size());
		fields.insert(fields.end(), typeFields.begin(), typeFields.end());

		return fields;
	}

	std::pair<const Type *, void *> Storage::ResolveDerivedType(const Type *baseClass, void *base_ptr) {
		for (const TypeID derived_id : baseClass->GetDerivedTypes()) {
			int index = (size_t) baseClass->GetTypeID() << 32 | (unsigned int) derived_id;
			auto it = m_DynamicCasts.find(index);
			if (it != m_DynamicCasts.end()) {
				const DynamicCast &dc = (*it).second;
				if (void *derived_ptr = dc(base_ptr)) {
					return ResolveDerivedType(GetType(derived_id), derived_ptr);
				}
			}
		}
		return std::make_pair(baseClass, base_ptr);
	}

	Type *Storage::FindDerivedTypeWithName(const Type *base, const std::string &derived_name) {
		for (const TypeID derived_id : base->GetDerivedTypes()) {
			Type *derived_type = GetType(derived_id);
			if (derived_type->GetQualifiedName().GetTemplatedName() == derived_name)
				return derived_type;
			FindDerivedTypeWithName(derived_type, derived_name);
		}
		return nullptr;
	}
}