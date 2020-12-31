#include "zsearch.h"

//@temporal:
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
		clearList (inputStrList);

		for (const std::string file : notesList) {
			// Busqueda en el nombre del archivo
			// @todo: controlar el orden en el que se encuentran las palabras para aumentar el peso
			fileNameLowerCase = str_lowerCase (file);

			for (const std::string needle : inputStrList) {
				/* @todo: ignorar los artículos indefinidos y definidos (un, una, unos, unas, el,
				 * los, la, las, lo) */
				if (std::string::npos != fileNameLowerCase.find (needle)) {
					weight++;
				}
			}

			// Búsqueda en las etiquetas del archivo

			// Búsqueda en el texto de la nota

			// Se añade al resultado
			if (0 < weight) {
				output.insert ({weight, file});
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
	bool ZSearch::isArticleOrPreposition (const std::string& str) {
		std::vector<std::string> toIgnore = {
			"un",	   "una",	  "unos", "unas",  "el",  "los",	"la",
			"les",	   "lo",	  "a",	  "antes", "de",  "dentro", "desde",
			"después", "durante", "en",	  "hasta", "por", "sobre",	"tras"};
		return (toIgnore.end () != std::find (toIgnore.begin (), toIgnore.end (), str));
	}

	void ZSearch::clearList (std::vector<std::string>& inputStrList) {
		for (auto it = inputStrList.begin (); it != inputStrList.end ();) {
			if (isArticleOrPreposition (*it)) {
				it = inputStrList.erase (it);
			} else {
				++it;
			}
		}
	}
}	// namespace jlu
