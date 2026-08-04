#pragma once

#include <string>
#include <vector>

#include "XMLData.h"

class ExcelExporter
{

public:

	bool exportInvoices(const std::vector<XMLData>& invoices, const std::string& outputFolder) const;

private:

	std::string sanitizeForXml(const std::string& text) const;

};