cmake_minimum_required(VERSION 3.18.0)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)
set(SOURCES ${TEST_SOURCES})

add_executable(appTest ${TEST_SOURCES})

target_include_directories (
	appTest
	PUBLIC
	${PROJECT_SOURCE_DIR}/src/zsearch
)

# target_link_libraries (
# 	app
# 	PUBLIC
# 	ManageTask
# )

target_link_libraries (
	appTest
	PUBLIC
	gtest_main
	ZSearch
)

add_test(
	NAME ZSearch
	COMMAND appTest
)
