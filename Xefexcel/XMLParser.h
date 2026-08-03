#include <string>
#include "XMLData.h"
#include <vector>
#include <pugixml.hpp>

class XMLParser
{
public:

	std::vector<XMLData> parseFolder(const std::string& folderPath);
};