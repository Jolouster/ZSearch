#include "zsearch.h"
#include <filesystem>
#include "gtest/gtest.h"

namespace fs = std::filesystem;
#define TEST_PATH_NOTES "/home/jlopez/Proyectos/SearchInZettelkastenFiles/test/asserts/"

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
	std::vector<std::string> output = zs.search ("Mapa");
	ASSERT_EQ (1, output.size ());
	output = zs.search ("Rampas");
	ASSERT_EQ (0, output.size ());
}

TEST_F (ZSearchTest, searchSomeWords) {
	std::vector<std::string> output = zs.search ("Mapa mental");
	ASSERT_EQ (1, output.size ());
}
