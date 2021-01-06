#include "zsearch.h"

#include <iostream>

namespace jlu {
	void ZSearch::setPath (std::string newPath) {
		if (false == fs::exists (newPath)) {
			throw std::invalid_argument ("Path to notes not exists");
		}
		pathToNotes = newPath;
	}

	std::multimap<int, std::string> ZSearch::search (const std::string& toFindStr) {
		std::multimap<int, std::string> output;

		if (notesList.empty ()) {
			loadFiles ();
		}

		const std::string toFindStrLowerCase = str_lowerCase (toFindStr);
		std::vector<std::string> inputStrList = split (toFindStrLowerCase, " ");
		unsigned short weight = 0;
		std::string fileNameLowerCase;
		classifyWords (inputStrList);

		for (const std::string fileName : notesList) {
			// Busqueda en el nombre del archivo
			fileNameLowerCase = str_lowerCase (fileName);
			weight = searchInFileName (inputStrList, fileNameLowerCase);
			bonusWords (weight, fileNameLowerCase);
			std::ifstream file (pathToNotes + fileName);

			if (file.is_open ()) {
				std::string line;
				std::vector<std::string> tagsList;
				std::regex headerPattern (R"(^(\#|\#\#|\#\#\#|\#\#\#\#|\#\#\#\#\#)\s.*)",
										  std::regex::ECMAScript);
				std::regex tagsPattern (
					R"((^|\s)\#[a-zA-Z0-9\-áéíúóïüöäëÁÉÍÚÓÄËÜÏÖ]+(\.|\,|\s|\!|\?|\)|$))");
				unsigned short tmpWeight = 0;

				while (std::getline (file, line)) {
					std::string lineLowerCase = str_lowerCase (line);
					tmpWeight = searchInLine (inputStrList, lineLowerCase);

					if (std::regex_match (line, headerPattern) && (0 < tmpWeight)) {
						tmpWeight++;   // Palabra usada en una cabecera.
						bonusWords (tmpWeight, lineLowerCase);
					}

					if (std::regex_search (line, tagsPattern) && (false == tags.empty ())) {
						// search tags in line
						for (std::string tagItem : tags) {
							if (std::regex_search (lineLowerCase,
												   std::regex (tagItem, std::regex::icase))) {
								tmpWeight += 3;
							}
						}
					}

					weight += tmpWeight;
					tmpWeight = 0;
				}

			} else {
				throw "Fail to open file: " + fileName;
			}

			// Se añade al resultado
			if (0 < weight) {
				output.insert ({weight, fileName});
				weight = 0;
			}
		}

		return output;
	}

	// Private
	void ZSearch::loadFiles () {
		for (fs::directory_entry entry : fs::directory_iterator (pathToNotes)) {
			if (entry.is_regular_file ()) {
				if (isZettel (entry.path ().filename ())) {
					notesList.push_back (entry.path ().filename ());
				}
			}
		}
	}

	bool ZSearch::isZettel (const std::string& file) {
		bool output = false;
		std::regex pattern (R"(^\d{12}\-[a-zA-Z0-9\-\?çñ\¿áéíúóïüöäëÁÉÍÚÓÄËÜÏÖ]+\.md$)",
							std::regex::ECMAScript);
		if (std::regex_match (file, pattern)) {
			output = true;
		}

		return output;
	}

	std::vector<std::string> ZSearch::split (const std::string& str, const std::string& separator) {
		const std::regex pattern{"\\" + separator};
		std::sregex_token_iterator start{str.begin (), str.end (), pattern, -1}, end;
		return std::vector<std::string> (start, end);
	}

	std::string ZSearch::str_lowerCase (std::string str) {
		std::transform (str.begin (), str.end (), str.begin (),
						[] (unsigned char c) { return std::tolower (c); });
		return str;
	}

	// We must ignore articles and prepositions
	bool ZSearch::isWordsToIgnore (const std::string& str) {
		std::vector<std::string> toIgnore = {"un", "una",	 "unos", "unas",  "el",		 "los",
											 "la", "les",	 "lo",	 "a",	  "antes",	 "con",
											 "de", "dentro", "del",	 "desde", "después", "durante",
											 "en", "hasta",	 "por",	 "sobre", "tras"};
		return (toIgnore.end () != std::find (toIgnore.begin (), toIgnore.end (), str));
	}

	bool ZSearch::isTag (const std::string& word) {
		std::regex pattern (R"((^|\s)\#[a-zA-Z0-9\-\_ñáéíóúäëïöüÁÉÍÓÚ]+)");
		std::smatch matchStr;
		bool output = false;

		if (std::regex_search (word, matchStr, pattern)) {
			output = true;
		}
		return output;
	}

	void ZSearch::classifyWords (std::vector<std::string>& inputStrList) {
		tags.clear ();

		for (auto it = inputStrList.begin (); it != inputStrList.end ();) {
			if (isWordsToIgnore (*it)) {
				it = inputStrList.erase (it);
			} else if (isTag (*it)) {
				if (std::find (tags.begin (), tags.end (), *it) == tags.end ()) {
					tags.push_back (trim (*it));
				}

				it = inputStrList.erase (it);
			} else {
				++it;
			}
		}
	}

	int ZSearch::searchInFileName (const std::vector<std::string>& inputStrList,
								   const std::string& fileName) {
		int output = 0;

		for (const std::string needle : inputStrList) {
			if (std::string::npos != fileName.find (needle)) {
				output++;
			}
		}

		return output;
	}

	int ZSearch::searchInLine (const std::vector<std::string>& inputStrList,
							   const std::string& line) {
		int output = 0;

		for (const std::string needle : inputStrList) {
			std::regex pattern ("(^|\\s)" + needle + "(\\.|\\,|\\-|\\_|\\?|\\!|\\s|$)",
								std::regex::icase);
			if (std::regex_search (line, pattern)) {
				output++;
			}
		}

		return output;
	}

	// incrementa el peso si además encuentra palabras espceiales
	void ZSearch::bonusWords (unsigned short& weight, const std::string& line) {
		if (0 < weight) {
			std::vector<std::string> bonusWords = {"checklist", "principios", "regalo", "contactos",
												   "normas"};
			for (const std::string bonus : bonusWords) {
				if (std::string::npos != line.find (bonus)) {
					weight++;
				}
			}
		}
	}

	std::string ZSearch::trim (const std::string& str) {
		std::regex pattern (R"((^\s{1,}|\s{1,}$))");
		return std::regex_replace (str, pattern, "");
	}

}	// namespace jlu
