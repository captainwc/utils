add_executable(printTest printTest.cpp)
target_include_directories(printTest PUBLIC ${INCLUDE_DIR})

add_executable(graphicTest graphicTest.cpp)
target_include_directories(graphicTest PUBLIC ${INCLUDE_DIR})
target_link_libraries(graphicTest mygraphic)

add_executable(graphicTest2 graphicTest2.cpp)
target_include_directories(graphicTest2 PUBLIC ${INCLUDE_DIR})
target_link_libraries(graphicTest2 mygraphic)
