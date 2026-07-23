#include <string>
#include "XMLData.h"
#include <vector>

class XMLParser
{
public:

	std::vector<XMLData> parseFolder(const std::string& folderPath);
};