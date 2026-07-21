#include <string>
#include "XMLData.h"

class XMLParser
{
public:
	std::vector<XMLData> parseFolder(const std::string& folderPath);
};