#include "XMLParser.h"

namespace fs = std::filesystem;

std::vector<XMLData> XMLParser::parseFolder(const std::string& folderPath)
{
	std::vector<XMLData> results;

	XMLData data;

	fs::path path(folderPath);

	for (const fs::directory_entry& entry : fs::directory_iterator(path))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".xml")
		{
			pugi::xml_document doc;

			pugi::xml_parse_result result = doc.load_file(entry.path().c_str());

			if (!result)
			{
				std::cout << "Nie udalo sie odczytac XML\n";

				continue;
			}

			printNode(doc.document_element(), 0);
		}
	}

	results.push_back(data);

	return results;
}

void XMLParser::printNode(const pugi::xml_node& node, int depth) const
{
	const std::string indent(static_cast<std::size_t>(depth) * 4, ' ');

	std::cout << indent << node.name();

	const char* value = node.child_value();

	if (value[0] != '\0')
	{
		std::cout << ": " << value;
	}

	std::cout << '\n';

	for (const pugi::xml_node& child : node.children())
	{
		printNode(child, depth + 1);
	}
}


