#include "ExcelExporter.h"

#include <filesystem>
#include <iostream>

#include <OpenXLSX.hpp>

namespace fs = std::filesystem;

bool ExcelExporter::exportInvoices(const std::vector<XMLData>& invoices, const std::string& outputFolder) const
{
	try
	{
		const fs::path outputPath =
			fs::path{ outputFolder } / "_XEFEXEL.xlsx";

		OpenXLSX::XLDocument document;

		document.create(outputPath.string());

		OpenXLSX::XLWorksheet worksheet =
			document.workbook().worksheet("Sheet1");

		worksheet.cell("A1").value() = "NR FV";
		worksheet.cell("B1").value() = "DATA";
		worksheet.cell("C1").value() = "SPRZEDAWCA";
		worksheet.cell("D1").value() = "NABYWCA";
		worksheet.cell("E1").value() = "OPIS FAKTURY";
		worksheet.cell("F1").value() = "NETTO";
		worksheet.cell("G1").value() = "VAT";
		worksheet.cell("H1").value() = "TERMIN P£ATNOŒCI";

		std::uint32_t row = 2;

		for (const XMLData& invoice : invoices)
		{
			worksheet.cell(row, 1).value() = invoice.invoiceNumber;
			worksheet.cell(row, 2).value() = invoice.date;
			worksheet.cell(row, 3).value() = invoice.sellerName;
			worksheet.cell(row, 4).value() = invoice.buyerName;
			worksheet.cell(row, 5).value() = invoice.info;
			worksheet.cell(row, 6).value() = invoice.netto;
			worksheet.cell(row, 7).value() = invoice.vatPercent;
			worksheet.cell(row, 8).value() = invoice.paymentDays;

			++row;
		}

		document.save();
		document.close();

		std::cout
			<< "[OK] Utworzono plik: "
			<< outputPath.string()
			<< '\n';

		return true;
	}
	catch (const std::exception& error)
	{
		std::cout
			<< "[ERROR] Nie udalo sie utworzyc XLSX: "
			<< error.what()
			<< '\n';

		return false;
	}
}