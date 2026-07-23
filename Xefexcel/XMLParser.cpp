#include "XMLParser.h"

void XMLParser::insertFolderPath(const std::string& folderPath)
{
	this->folderPath = folderPath;
}

std::vector<XMLData> XMLParser::parseFolder()
{
	std::vector<XMLData> results;

	XMLData data;
	data.invoiceNumber = "FV/1/2026";
	data.nip = "1234567890";
	data.sellerName = "Firma";

	results.push_back(data);

	return results;
}

std::vector<XMLData> XMLParser::analyzeFolder()
{
	return std::vector<XMLData>();
}
