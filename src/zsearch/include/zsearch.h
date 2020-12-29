#ifndef ZSEARCH_H
#define ZSEARCH_H

#include <exception>
#include <filesystem>
#include <regex>
#include <string>

namespace fs = std::filesystem;

namespace jlu {
	class ZSearch {
	   public:
		void setPath (std::string newPath);
		std::vector<std::string> search (const std::string& toFindStr);

	   private:
		void loadFiles ();
		bool isZettel (const std::string& file);
		std::vector<std::string> split (const std::string& str, const std::string& separator);
		std::string pathToNotes;
		std::vector<std::string> notesList;
	};
}	// namespace jlu

#endif	 // ZSEARCH_H
