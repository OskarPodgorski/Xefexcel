#include <vector>
#include <string>
#include "XMLData.h"

class ExcelExporter
{
public:

	void exportInvoices(const std::vector<XMLData>& invoices, const std::string& outputPath);
};