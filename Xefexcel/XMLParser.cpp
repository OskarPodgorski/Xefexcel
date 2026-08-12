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

	const pugi::xml_node fa =
		childByName(root, "Fa");

	const pugi::xml_node seller =
		childByName(root, "Podmiot1");

	const pugi::xml_node buyer =
		childByName(root, "Podmiot2");

	const pugi::xml_node sellerData =
		childByName(seller, "DaneIdentyfikacyjne");

	const pugi::xml_node buyerData =
		childByName(buyer, "DaneIdentyfikacyjne");

	data.invoiceNumber =
		childByName(fa, "P_2")
		.text()
		.as_string();

	data.date =
		childByName(fa, "P_1")
		.text()
		.as_string();

	data.sellerName =
		childByName(sellerData, "Nazwa")
		.text()
		.as_string();

	data.buyerName =
		childByName(buyerData, "Nazwa")
		.text()
		.as_string();

	int infoCount = 0;

	for (const pugi::xml_node& row : fa.children())
	{
		std::string rowName = row.name();

		const std::size_t colonPosition =
			rowName.find(':');

		if (colonPosition != std::string::npos)
		{
			rowName =
				rowName.substr(colonPosition + 1);
		}

		if (rowName != "FaWiersz")
		{
			continue;
		}

		const std::string product =
			childByName(row, "P_7")
			.text()
			.as_string();

		if (product.empty())
		{
			continue;
		}

		if (!data.info.empty())
		{
			data.info += ", ";
		}

		data.info += product;

		++infoCount;

		if (infoCount >= 3)
		{
			break;
		}
	}

	const pugi::xml_node footer =
		childByName(root, "Stopka");

	const pugi::xml_node footerInfo =
		childByName(footer, "Informacje");

	data.additionalInfo =
		childByName(footerInfo, "StopkaFaktury")
		.text()
		.as_string();

	for (const pugi::xml_node& row : fa.children())
	{
		std::string rowName = row.name();

		const std::size_t colonPosition =
			rowName.find(':');

		if (colonPosition != std::string::npos)
		{
			rowName =
				rowName.substr(colonPosition + 1);
		}

		if (rowName == "FaWiersz")
		{
			data.vatPercent =
				childByName(row, "P_12")
				.text()
				.as_string();

			break;
		}
	}

	if (!data.vatPercent.empty())
	{
		data.vatPercent += "%";
	}

	data.netto =
		childByName(fa, "P_13_1")
		.text()
		.as_string();

	if (data.netto.empty())
	{
		data.netto =
			childByName(fa, "P_13_2")
			.text()
			.as_string();
	}

	std::replace(
		data.netto.begin(),
		data.netto.end(),
		'.',
		',');

	if (!data.netto.empty())
	{
		data.netto.insert(0, "-");
	}

	const pugi::xml_node payment =
		childByName(fa, "Platnosc");

	data.paymentDays =
		childByName(payment, "TerminPlatnosci")
		.text()
		.as_string();

	if (data.paymentDays.empty())
	{
		data.paymentDays = "0";
	}

	return data;
}

pugi::xml_node XMLParser::childByName(const pugi::xml_node& parent, const std::string& name) const
{
	for (const pugi::xml_node& child : parent.children())
	{
		std::string childName = child.name();

		const std::size_t colonPosition = childName.find(':');

		if (colonPosition != std::string::npos)
		{
			childName = childName.substr(colonPosition + 1);
		}

		if (childName == name)
		{
			return child;
		}
	}

	return {};
}
