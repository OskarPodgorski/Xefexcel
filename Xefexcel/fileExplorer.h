#include <string>
#include <vector>

class FileExplorer
{
public:

	void analyzeFolder(const std::string& folderPath);
	void insertFolderPath(const std::string& folderPath);
	const std::string& getFolderPath() const;

private:

	std::string folderPath;
};