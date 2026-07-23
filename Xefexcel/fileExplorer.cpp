#include "fileExplorer.h"

const std::string& FileExplorer::getFolderPath() const { return folderPath; }

void FileExplorer::insertFolderPath(const std::string& folderPath)
{
	this->folderPath = folderPath;
}

void FileExplorer::analyzeFolder(const std::string& folderPath)
{
	
}
