#include "XMLParser.h"

std::vector<XMLData> XMLParser::parseFolder(const std::string& folderPath)
{
	std::system("cls");

	std::vector<XMLData> results;

	const fs::path path(folderPath);

	for (const fs::directory_entry& entry : fs::directory_iterator(path))
	{
		if (!entry.is_regular_file())
		{
			continue;
		}

		const fs::path extension = entry.path().extension();

		if (extension == ".xml")
		{
			pugi::xml_document doc;

			const pugi::xml_parse_result result =
				doc.load_file(entry.path().c_str());

			if (!result)
			{
				std::cout
					<< "[ERROR] Nie udalo sie odczytac XML\n";

				continue;
			}

			results.push_back(parseDocument(doc));
		}
		else if (extension == ".zip")
		{
			parseZIP(entry.path(), results);
		}
	}

	return results;
}

void XMLParser::parseZIP(const fs::path& zipPath, std::vector<XMLData>& results) const
{
	mz_zip_archive zip{};

	if (!mz_zip_reader_init_file(
		&zip,
		zipPath.string().c_str(),
		0))
	{
		std::cout
			<< "[ERROR] Nie udalo sie otworzyc ZIP: "
			<< zipPath.filename().string()
			<< '\n';

		return;
	}

	const mz_uint fileCount =
		mz_zip_reader_get_num_files(&zip);

	std::cout
		<< "[ZIP] "
		<< zipPath.filename().string()
		<< " | plikow: "
		<< fileCount
		<< '\n';

	for (mz_uint i = 0; i < fileCount; ++i)
	{
		mz_zip_archive_file_stat fileStat{};

		if (!mz_zip_reader_file_stat(
			&zip,
			i,
			&fileStat))
		{
			continue;
		}

		if (mz_zip_reader_is_file_a_directory(&zip, i))
		{
			continue;
		}

		const fs::path filePath{ fileStat.m_filename };

		if (filePath.extension() != ".xml")
		{
			continue;
		}

		std::cout
			<< "[ZIP XML] "
			<< fileStat.m_filename
			<< '\n';
	}

	mz_zip_reader_end(&zip);
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

	const pugi::xml_node paymentTerm =
		childByName(payment, "TerminPlatnosci");

	data.paymentDays =
		childByName(paymentTerm, "IloscDni")
		.text()
		.as_string();

	data.paymentDueDate =
		childByName(paymentTerm, "Termin")
		.text()
		.as_string();

	if (data.paymentDays.empty())
	{
		if (!data.paymentDueDate.empty())
		{
			data.paymentDays =
				calculatePaymentDays(
					data.date,
					data.paymentDueDate);
		}
		else
		{
			data.paymentDays = "0";
		}
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

std::string XMLParser::calculatePaymentDays(
	const std::string& invoiceDate,
	const std::string& paymentDate) const
{
	if (invoiceDate.empty() || paymentDate.empty())
	{
		return "0";
	}

	int invoiceYear;
	unsigned invoiceMonth;
	unsigned invoiceDay;

	int paymentYear;
	unsigned paymentMonth;
	unsigned paymentDay;

	char separator1;
	char separator2;

	std::istringstream invoiceStream(invoiceDate);

	invoiceStream
		>> invoiceYear
		>> separator1
		>> invoiceMonth
		>> separator2
		>> invoiceDay;

	std::istringstream paymentStream(paymentDate);

	paymentStream
		>> paymentYear
		>> separator1
		>> paymentMonth
		>> separator2
		>> paymentDay;

	const std::chrono::sys_days invoice{
		std::chrono::year{invoiceYear}
		/ std::chrono::month{invoiceMonth}
		/ std::chrono::day{invoiceDay}
	};

	const std::chrono::sys_days payment{
		std::chrono::year{paymentYear}
		/ std::chrono::month{paymentMonth}
		/ std::chrono::day{paymentDay}
	};

	const auto difference = payment - invoice;

	return std::to_string(difference.count());
}