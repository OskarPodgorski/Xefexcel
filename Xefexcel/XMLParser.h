#include <string>
#include "XMLData.h"
#include <vector>
#include <pugixml.hpp>
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <sstream>

class XMLParser
{
public:

	std::vector<XMLData> parseFolder(const std::string& folderPath);

private:

	void printNode(const pugi::xml_node& node, int depth) const;

	XMLData parseDocument(const pugi::xml_document& doc) const;

	pugi::xml_node childByName(const pugi::xml_node& parent, const std::string& name) const;

	std::string calculatePaymentDays(const std::string& invoiceDate, const std::string& paymentDate) const;
};