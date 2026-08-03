#include "XMLParser.h"

namespace fs = std::filesystem;

std::vector<XMLData> XMLParser::parseFolder(const std::string& folderPath)
{
	std::vector<XMLData> results;

	XMLData data;

	fs::path path(folderPath);
	pugi::xml_document doc;

	for (const fs::directory_entry& entry : fs::directory_iterator(path))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".xml")
		{
			pugi::xml_parse_result result = doc.load_file(entry.path().c_str());

			if (!result)
			{
				std::cout << "Nie udalo sie odczytac XML\n";
				return results;
			}

			std::cout << "test\n";
		}
	}

	results.push_back(data);

	return results;
}


