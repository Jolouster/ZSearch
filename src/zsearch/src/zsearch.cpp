#include "zsearch.h"

//@temporal:
#include <algorithm>
#include <iostream>
#include <string>

namespace jlu {
	void ZSearch::setPath (std::string newPath) {
		if (false == fs::exists (newPath)) {
			throw std::invalid_argument ("Path to notes not exists");
		}
		pathToNotes = newPath;
	}

	std::vector<std::string> ZSearch::search (const std::string& toFindStr) {
		std::vector<std::string> output;
		loadFiles ();
		const std::vector<std::string> inputStringList = split (toFindStr, " ");
		unsigned short weight = 0;

		for (const std::string file : notesList) {
			// Busqueda en el nombre del archivo
			// @todo: controlar el orden en el que se encuentran las palabras para aumentar el peso
			for (const std::string needle : inputStringList) {
				if (std::string::npos != file.find (needle)) {
					weight++;
				}
			}

			if (0 < weight) {
				output.push_back (file);
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
}	// namespace jlu
