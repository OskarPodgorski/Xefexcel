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

			worksheet.cell(row, 6).value() =
				sanitizeForXml(invoice.info);

			//

			if (!invoice.netto.empty())
			{
				std::string nettoText = invoice.netto;

				std::replace(
					nettoText.begin(),
					nettoText.end(),
					',',
					'.'
				);

				worksheet.cell(row, 8).value() =
					std::stod(nettoText);
			}
			else
			{
				worksheet.cell(row, 8).value() = 0.0;
			}

			if (!invoice.vatPercent.empty())
			{
				worksheet.cell(row, 9).value() =
					std::stod(invoice.vatPercent) / 100.0;
			}
			else
			{
				worksheet.cell(row, 9).value() = 0.0;
			}

			if (!invoice.paymentDays.empty())
			{
				worksheet.cell(row, 12).value() =
					std::stoi(invoice.paymentDays);
			}
			else
			{
				worksheet.cell(row, 12).value() = 30;
			}

			//

			worksheet.cell(row, 15).value() =
				sanitizeForXml(invoice.additionalInfo);

			++row;
		}

		document.save();
		document.close();

		std::cout
			<< "[OK] Utworzono plik: "
			<< outputPath.string()
			<< '\n';

		std::system(("start \"\" \"" + outputPath.string() + "\"").c_str());

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

	return toUpperPolishUtf8(result);
}

std::string ExcelExporter::toUpperPolishUtf8(std::string text) const
{
	const std::pair<std::string, std::string> replacements[] =
	{
		{"ą", "Ą"},
		{"ć", "Ć"},
		{"ę", "Ę"},
		{"ł", "Ł"},
		{"ń", "Ń"},
		{"ó", "Ó"},
		{"ś", "Ś"},
		{"ź", "Ź"},
		{"ż", "Ż"}
	};

	for (char& c : text)
	{
		if (c >= 'a' && c <= 'z')
		{
			c = static_cast<char>(c - 'a' + 'A');
		}
	}

	for (const auto& [lower, upper] : replacements)
	{
		std::size_t pos = 0;

		while ((pos = text.find(lower, pos)) != std::string::npos)
		{
			text.replace(pos, lower.size(), upper);
			pos += upper.size();
		}
	}

	return text;
}