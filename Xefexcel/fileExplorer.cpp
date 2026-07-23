#include "fileExplorer.h"

namespace fs = std::filesystem;

const std::string& FileExplorer::getFolderPath() const { return folderPath; }

void FileExplorer::insertFolderPath(const std::string& folderPath)
{
	this->folderPath = folderPath;
}

void FileExplorer::analyzeFolder() const
{
	fs::path path(folderPath);

    if (!fs::exists(path))
    {
        std::cout << "[ERROR] Folder nie istnieje: "
            << folderPath << '\n';
        return;
    }

    if (!fs::is_directory(path))
    {
        std::cout << "[ERROR] Podana sciezka nie jest folderem: "
            << folderPath << '\n';
        return;
    }

    for (const fs::directory_entry& entry : fs::directory_iterator(path))
    {
        if (entry.is_regular_file())
        {
            std::cout << "[FILE] "
                << entry.path().filename().string()
                << '\n';
        }
    }
}
