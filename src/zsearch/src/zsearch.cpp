#include "zsearch.h"

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

		for (const std::string fileName : notesList) {
			// Busqueda en el nombre del archivo
			fileNameLowerCase = str_lowerCase (fileName);
			weight = searchInLine (inputStrList, fileNameLowerCase);
			bonusWords (weight, fileNameLowerCase);

			std::ifstream file (pathToNotes + fileName);
			if (file.is_open ()) {
				std::string line;

				std::regex headerPattern (R"(^(\#|\#\#|\#\#\#|\#\#\#\#|\#\#\#\#\#)\s.*)",
										  std::regex::ECMAScript);
				while (std::getline (file, line)) {
					std::string lineLowerCase = str_lowerCase (line);
					weight += searchInLine (inputStrList, lineLowerCase);

					if (std::regex_match (line, headerPattern)) {
						bonusWords (weight, lineLowerCase);
					}
				}

				// Búsqueda en las etiquetas del archivo

				// Búsqueda en el texto de la nota, de mayor a menor valor
				// - Encabezados de cualquier nivel
				// - Resto del texto
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

	int ZSearch::searchInLine (const std::vector<std::string>& inputStrList,
							   const std::string& line) {
		int output = 0;

		for (const std::string needle : inputStrList) {
			if (std::string::npos != line.find (needle)) {
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
}	// namespace jlu
