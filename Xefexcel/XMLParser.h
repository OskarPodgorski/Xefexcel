#include <string>
#include "XMLData.h"
#include <vector>
#include <pugixml.hpp>
#include <filesystem>
#include <iostream>

class XMLParser
{
public:

	std::vector<XMLData> parseFolder(const std::string& folderPath);
};