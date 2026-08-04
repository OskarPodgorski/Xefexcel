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
		worksheet.cell("E1").value() = "BUDOWA";
		worksheet.cell("F1").value() = "OPIS FAKTURY";
		worksheet.cell("G1").value() = "STAWKA";
		worksheet.cell("H1").value() = "NETTO";
		worksheet.cell("I1").value() = "VAT";
		worksheet.cell("J1").value() = "VAT KWOTA";
		worksheet.cell("K1").value() = "BRUTTO";
		worksheet.cell("L1").value() = "DNI";
		worksheet.cell("M1").value() = "TERMIN PŁATNOŚCI";
		worksheet.cell("O1").value() = "INNE";

		std::uint32_t row = 3;

		for (const XMLData& invoice : invoices)
		{
			worksheet.cell(row, 1).value() =
				sanitizeForXml(invoice.invoiceNumber);

			worksheet.cell(row, 2).value() =
				sanitizeForXml(invoice.date);

			worksheet.cell(row, 3).value() =
				sanitizeForXml(invoice.sellerName);

			worksheet.cell(row, 4).value() =
				sanitizeForXml(invoice.buyerName);

			worksheet.cell(row, 8).value() =
				sanitizeForXml(invoice.netto);

			worksheet.cell(row, 9).value() =
				sanitizeForXml(invoice.vatPercent);

			worksheet.cell(row, 12).value() =
				sanitizeForXml(invoice.paymentDays);



			worksheet.cell(row, 15).value() =
				sanitizeForXml(invoice.info);

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

std::string ExcelExporter::sanitizeForXml(const std::string& text) const
{
	std::string result;
	result.reserve(text.size());

	for (const unsigned char character : text)
	{
		if (character >= 0x20 ||
			character == '\t' ||
			character == '\n' ||
			character == '\r')
		{
			result.push_back(static_cast<char>(character));
		}
	}

	return result;
}