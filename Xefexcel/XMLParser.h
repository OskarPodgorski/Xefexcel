#include <string>
#include "XMLData.h"
#include <vector>
#include <pugixml.hpp>
#include <filesystem>
#include <iostream>
#include <cstdlib>

class XMLParser
{
public:

	std::vector<XMLData> parseFolder(const std::string& folderPath);

private:

	void printNode(const pugi::xml_node& node, int depth) const;

	XMLData parseDocument(const pugi::xml_document& doc) const;
};