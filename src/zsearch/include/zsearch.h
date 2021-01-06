#ifndef ZSEARCH_H
#define ZSEARCH_H

#include <cctype>
#include <exception>
#include <filesystem>
#include <fstream>
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
		bool isWordsToIgnore (const std::string& str);
		bool isTag (const std::string& word);
		void classifyWords (std::vector<std::string>& inputStrList);
		int searchInFileName (const std::vector<std::string>& inputStrList,
							  const std::string& line);
		int searchInLine (const std::vector<std::string>& inputStrList, const std::string& line);
		void bonusWords (unsigned short& weight, const std::string& line);
		std::string trim (const std::string& str);
		std::string pathToNotes;
		std::vector<std::string> notesList;
		std::vector<std::string> tags;
	};
}	// namespace jlu

#endif	 // ZSEARCH_H
