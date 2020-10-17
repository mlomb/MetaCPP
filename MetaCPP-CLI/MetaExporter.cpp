#include "MetaExporter.hpp"

#include <iostream>
#include <fstream>

#include "Templates.hpp"
#include "MetaCPP/Storage.hpp"

namespace metacpp {
	MetaExporter::MetaExporter(const Storage* storage)
		: m_Storage(storage)
	{
	}

	void MetaExporter::Export(const std::string& inputSource, const std::string& outputHeader, const std::string& outputSource)
	{
		std::ofstream out_header(outputHeader);
		std::ofstream out_source(outputSource);

		mustache::data storageData = ExportStorage();

		out_source << GenerateSourceFile({ outputHeader }, storageData);
		out_header << GenerateHeaderFile({ inputSource }, storageData);
	}

	std::string MetaExporter::GenerateSourceFile(const std::vector<std::string>& includes, mustache::data& storageData)
	{
		mustache::mustache source_template(templates::source);
		source_template.set_custom_escape([](const std::string& str) { return str; });

		SetIncludes(includes, storageData);

		return source_template.render(storageData);
	}

	std::string MetaExporter::GenerateHeaderFile(const std::vector<std::string>& includes, mustache::data& storageData)
	{
		mustache::mustache header_template(templates::header);
		header_template.set_custom_escape([](const std::string& str) { return str; });

		SetIncludes(includes, storageData);

		return header_template.render(storageData);
	}

	void MetaExporter::SetIncludes(const std::vector<std::string>& includes, mustache::data& data)
	{
		mustache::data inclds{ mustache::data::type::list };
		for (const std::string& file : includes)
			inclds << mustache::data{ "file", file };
		data["includes"] = mustache::data{ inclds };
	}

	mustache::data MetaExporter::ExportStorage()
	{
		mustache::data data;

		// IDs
		mustache::data ids{ mustache::data::type::list };
		for (const auto& kv : m_Storage->m_IDs) {
			mustache::data entry;
			entry["id"] = std::to_string(kv.second);
			entry["qualifiedName"] = kv.first;
			ids << entry;
		}
		data["ids"] = mustache::data{ ids };

		// Types
		mustache::data types{ mustache::data::type::list };
		for (const auto& kv : m_Storage->m_Types)
			types << ExportType(kv.second);
		data["types"] = mustache::data{ types };

		return data;
	}

	mustache::data MetaExporter::ExportType(const Type* type)
	{
		mustache::data data;
		data["id"] = std::to_string(type->m_ID);
		data["qualifiedName"] = type->m_QualifiedName.FullQualified();
		data["name"] = type->m_QualifiedName.GetName();
		data["size"] = std::to_string(type->m_SizeInBytes);
		data["kind"] = std::to_string(type->m_Kind);
		data["access"] = std::to_string(type->m_Access);
		data["valid"] = std::to_string(type->IsValid());
		data["polymorphic"] = std::to_string(type->m_Polymorphic);
        data["hasDefaultConstructor"] = std::to_string(type->m_HasDefaultConstructor);
        data["hasDefaultDestructor"] = std::to_string(type->m_HasDefaultDestructor);

		bool seqContainer = type->IsSequentialContainer();
		bool assocContainer = type->IsAssociativeContainer();

		if (type->m_TemplateArguments.size() > 0) {
			auto t = type->m_TemplateArguments[0];
			data["containerValueQualifiedName"] = t.GetQualifiedName(m_Storage);
		}

		data["isSequentialContainer"] = std::to_string(seqContainer);
		data["isAssociativeContainer"] = std::to_string(assocContainer);
		data["isContainer"] = std::to_string(seqContainer || assocContainer);

		// Base Types
		mustache::data baseTypes{ mustache::data::type::list };
		for (const BaseType& base : type->m_BaseTypes) {
			mustache::data baseType;
			baseType["qualifiedType"] = ExportQualifiedType(base.type);
			baseType["access"] = std::to_string(base.access);
			baseTypes << baseType;
		}

		// Derived Types
		mustache::data derivedTypes{ mustache::data::type::list };
		for (const TypeID derived_typeid : type->m_DerivedTypes) {
			Type* derived_type = m_Storage->GetType(derived_typeid);
			if (!derived_type->IsValid())
				continue;

			mustache::data data;
			data["derivedTypeId"] = std::to_string(derived_typeid);
			data["derivedQualifiedName"] = derived_type->GetQualifiedName().FullQualified();
			derivedTypes << data;
		}

		// Template Arguments
		mustache::data templateArguments{ mustache::data::type::list };
		for (const QualifiedType& arg : type->m_TemplateArguments) {
			if (arg.GetTypeID() != 0)
				templateArguments << ExportQualifiedType(arg);
			else
				templateArguments << mustache::data{ "null", false };
		}

		// Fields
		mustache::data fields{ mustache::data::type::list };
		for (const Field& field : type->m_Fields)
			fields << ExportField(field);


		data["fields"] = mustache::data{ fields };
		data["baseTypes"] = mustache::data{ baseTypes };
		data["derivedTypes"] = mustache::data{ derivedTypes };
		data["templateArguments"] = mustache::data{ templateArguments };

		return data;
	}

	mustache::data MetaExporter::ExportQualifiedType(const QualifiedType& qtype)
	{
		mustache::data data;
		data["typeID"] = std::to_string(qtype.m_Type);
		data["const"] = std::to_string(qtype.m_Const);
		data["operator"] = std::to_string(qtype.m_Operator);
		return data;
	}

	mustache::data MetaExporter::ExportField(const Field& field)
	{
		mustache::data data;
		data["ownerId"] = std::to_string(field.m_Owner);
		data["qualifiedType"] = ExportQualifiedType(field.m_Type);
		data["qualifiedName"] = field.m_QualifiedName.FullQualified();
		data["name"] = field.m_QualifiedName.GetName();
		data["offset"] = std::to_string(field.m_OffsetInBytes);
		return data;
	}
}
