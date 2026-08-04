#include "XMLParser.h"

namespace fs = std::filesystem;

std::vector<XMLData> XMLParser::parseFolder(const std::string& folderPath)
{
	std::system("cls");

	std::vector<XMLData> results;

	XMLData data;

	fs::path path(folderPath);

	for (const fs::directory_entry& entry : fs::directory_iterator(path))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".xml")
		{
			pugi::xml_document doc;

			pugi::xml_parse_result result = doc.load_file(entry.path().c_str());

			if (!result)
			{
				std::cout << "Nie udalo sie odczytac XML\n";

				continue;
			}

			XMLData data = parseDocument(doc);

			results.push_back(data);

			std::cout
				<< "[OK] "
				<< data.invoiceNumber
				<< " | "
				<< data.sellerName
				<< '\n';
		}
	}
	return results;
}

void XMLParser::printNode(const pugi::xml_node& node, int depth) const
{
	const std::string indent(static_cast<std::size_t>(depth) * 4, ' ');

	std::cout << indent << node.name();

	const char* value = node.child_value();

	if (value[0] != '\0')
	{
		std::cout << ": " << value;
	}

	std::cout << '\n';

	for (const pugi::xml_node& child : node.children())
	{
		printNode(child, depth + 1);
	}
}

XMLData XMLParser::parseDocument(const pugi::xml_document& doc) const
{
	XMLData data;

	const pugi::xml_node root = doc.document_element();

	data.invoiceNumber =
		root.child("Fa")
		.child("P_2")
		.text()
		.as_string();

	data.date =
		root.child("Fa")
		.child("P_1")
		.text()
		.as_string();

	data.sellerName =
		root.child("Podmiot1")
		.child("DaneIdentyfikacyjne")
		.child("Nazwa")
		.text()
		.as_string();

	data.buyerName =
		root.child("Podmiot2")
		.child("DaneIdentyfikacyjne")
		.child("Nazwa")
		.text()
		.as_string();

	data.info =
		root.child("Stopka")
		.child("Informacje")
		.child("StopkaFaktury")
		.text()
		.as_string();

	data.vatPercent =
		root.child("Fa")
		.child("FaWiersz")
		.child("P_12")
		.text()
		.as_string();

	data.vatPercent += "%";

	data.netto =
		root.child("Fa")
		.child("P_13_1")
		.text()
		.as_string();

	std::replace(data.netto.begin(), data.netto.end(), '.', ',');

	data.netto.insert(0, "-");

	data.paymentDays =
		root.child("Fa")
		.child("Platnosc")
		.child("TerminPlatnosci")
		.text()
		.as_string();

	data.paymentDays =
		data.paymentDays.empty()
		? "0"
		: data.paymentDays;

	return data;
}
