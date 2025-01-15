add_executable(IndexProgram IndexProgram.cpp)
add_executable(SearchProgram SearchProgram.cpp)

target_link_libraries(IndexProgram PRIVATE IndexLib)
target_link_libraries(SearchProgram PRIVATE SearchLib)
target_include_directories(IndexProgram PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(SearchProgram PUBLIC ${PROJECT_SOURCE_DIR})
