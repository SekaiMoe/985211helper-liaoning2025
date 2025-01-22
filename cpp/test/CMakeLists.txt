project(search_test)

add_executable(${PROJECT_NAME} main.cpp ${CMAKE_SOURCE_DIR}/src/search.cpp ${CMAKE_SOURCE_DIR}/src/json.cpp ${CMAKE_SOURCE_DIR}/src/data.cpp)
add_executable(cxx17test cpp17.cpp)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
