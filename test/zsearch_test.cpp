#include "zsearch.h"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include "gtest/gtest.h"

namespace fs = std::filesystem;
#define TEST_PATH_NOTES "/home/jlopez/Proyectos/SearchInZettelkastenFiles/test/asserts/"
#define LOG std::cout << "[          ] [INFO] "

class ZSearchTest : public testing::Test {
   public:
	jlu::ZSearch zs;
	void SetUp () {
		zs.setPath (TEST_PATH_NOTES);	// by default
	};

	void TearDown (){};
};

TEST_F (ZSearchTest, setPathFails) {
	ASSERT_THROW (zs.setPath ("inexistentPath"), std::invalid_argument);
}

TEST_F (ZSearchTest, setPathOk) {
	ASSERT_NO_THROW (zs.setPath (TEST_PATH_NOTES));
}

TEST_F (ZSearchTest, searchOneWord) {
	std::multimap<int, std::string> output = zs.search ("Mapa");
	ASSERT_EQ (1, output.size ());

	for (auto it = output.begin (); it != output.end (); it++) {
		ASSERT_EQ (1, it->first);
		LOG << "Weight: " << it->first << " file: " << it->second << std::endl;
	}

	output = zs.search ("Rampas");
	ASSERT_EQ (0, output.size ());
}

TEST_F (ZSearchTest, searchSomeWords) {
	std::multimap<int, std::string> output = zs.search ("Mapa mental");
	ASSERT_EQ (1, output.size ());

	for (auto it = output.begin (); it != output.end (); it++) {
		ASSERT_EQ (2, it->first);
		LOG << "Weight: " << it->first << " file: " << it->second << std::endl;
	}

	// Con caracteres especiales: çñáéíúóïüöäëÁÉÍÚÓÄËÜÏÖ
	output = zs.search ("Extrañas el texto");
	ASSERT_EQ (1, output.size ());

	for (auto it = output.begin (); it != output.end (); it++) {
		ASSERT_EQ (2, it->first);
		LOG << "Weight: " << it->first << " file: " << it->second << std::endl;
	}

	output = zs.search ("Caços en el raros área con índice");
	ASSERT_EQ (1, output.size ());

	for (auto it = output.begin (); it != output.end (); it++) {
		ASSERT_EQ (3, it->first);
		LOG << "Weight: " << it->first << " file: " << it->second << std::endl;
	}
}
