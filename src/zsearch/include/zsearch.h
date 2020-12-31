#ifndef ZSEARCH_H
#define ZSEARCH_H

#include <cctype>
#include <exception>
#include <filesystem>
#include <iterator>
#include <map>
#include <regex>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace jlu {
	class ZSearch {
	   public:
		void setPath (std::string newPath);
		std::multimap<int, std::string> search (const std::string& toFindStr);

	   private:
		void loadFiles ();
		bool isZettel (const std::string& file);
		std::vector<std::string> split (const std::string& str, const std::string& separator);
		std::string str_lowerCase (std::string str);
		bool isArticleOrPreposition (const std::string& str);
		void clearList (std::vector<std::string>& inputStrList);
		std::string pathToNotes;
		std::vector<std::string> notesList;
	};
}	// namespace jlu

#endif	 // ZSEARCH_H
