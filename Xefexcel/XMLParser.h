#include <string>
#include "XMLData.h"
#include <vector>

class XMLParser
{
public:

	void insertFolderPath(const std::string folderPath);
	std::vector<XMLData> parseFolder();
	std::vector<XMLData> analyzeFolder();

private:

	std::string folderPath;
};