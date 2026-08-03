#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <optional>
#include <cstdlib>

class FileExplorer
{
public:

	std::optional<size_t> analyzeFolder() const;

	void insertFolderPath(const std::string& folderPath);

	const std::string& getFolderPath() const;

private:

	std::string folderPath;
};