#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>

class FileExplorer
{
public:

	void analyzeFolder() const;
	void insertFolderPath(const std::string& folderPath);
	const std::string& getFolderPath() const;

private:

	std::string folderPath;
};