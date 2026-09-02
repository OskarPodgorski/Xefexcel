#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "XMLData.h"

class ExcelExporter
{

public:

	bool exportInvoices(const std::vector<XMLData>& invoices, const std::string& outputFolder) const;

private:

	std::string sanitizeForXml(const std::string& text) const;

	std::string toUpperPolishUtf8(std::string text) const;
};