add_library(ZSearch STATIC
	src/zsearch.cpp
)

target_include_directories(ZSearch PUBLIC include)
target_link_libraries(ZSearch stdc++fs)
