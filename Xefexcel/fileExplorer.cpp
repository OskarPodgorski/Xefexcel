#include "fileExplorer.h"

namespace fs = std::filesystem;

const std::string& FileExplorer::getFolderPath() const { return folderPath; }

void FileExplorer::insertFolderPath(const std::string& folderPath)
{
	this->folderPath = folderPath;
}

std::optional<size_t> FileExplorer::analyzeFolder() const
{
	std::system("cls");

	fs::path path(folderPath);

	if (!fs::exists(path))
	{
		std::cout << "[ERROR] Folder nie istnieje: "
			<< folderPath << '\n';
		return std::nullopt;
	}

	if (!fs::is_directory(path))
	{
		std::cout << "[ERROR] Podana sciezka nie jest folderem: "
			<< folderPath << '\n';
		return std::nullopt;
	}

	int filesCount = 0;

	for (const fs::directory_entry& entry : fs::directory_iterator(path))
	{
		if (entry.is_regular_file())
		{
			std::cout << "[FILE] "
				<< entry.path().filename().string()
				<< '\n';

			std::filesystem::path extension = entry.path().extension();

			if (extension == ".xml" || extension == ".zip")
			{
				++filesCount;
			}
		}
	}

	return filesCount;
}
