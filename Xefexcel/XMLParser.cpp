#include "XMLParser.h"

std::vector<XMLData> XMLParser::parseFolder(
    const std::string& folderPath)
{
    std::vector<XMLData> results;

    XMLData data;
    data.invoiceNumber = "FV/1/2026";
    data.nip = "1234567890";
    data.sellerName = "Firma";

    results.push_back(data);

    return results;
}