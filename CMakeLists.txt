cmake_minimum_required(VERSION 3.10)
project(CommandPattern VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Исполняемый файл демонстрации (из оригинального main)
add_executable(command_demo webpage_button.cpp)
target_include_directories(command_demo PRIVATE ${CMAKE_SOURCE_DIR})

# Установка: программа и заголовок AccessKey.h (хотя он не используется в коде, но по условию он дан)
install(TARGETS command_demo RUNTIME DESTINATION bin)
install(FILES AccessKey.h DESTINATION include)

# CPack для deb-пакета
set(CPACK_PACKAGE_NAME "command-pattern-demo")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION "Command pattern example from original repo (webpage_button.cpp + AccessKey.h)")
set(CPACK_PACKAGE_CONTACT "student@example.com")
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Student <student@example.com>")
include(CPack)

# Тесты
enable_testing()
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(command_tests tests/test_command.cpp)
target_link_libraries(command_tests PRIVATE GTest::gtest_main)
add_test(NAME CommandTests COMMAND command_tests)

# Если файл tests/test_command.cpp не найден, используем альтернативный тест
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/tests/test_command.cpp)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/fallback_test.cpp "
#include <gtest/gtest.h>
#include <sstream>
#include \"../webpage_button.cpp\"

TEST(CommandPatternTest, ChangeWebPageCommandExecutesChangeWebPage) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    WebPageManager manager;
    ChangeWebPageCommand cmd(manager, \"http://example.com\");
    cmd.Execute();
    std::cout.rdbuf(old);
    std::string output = buffer.str();
    EXPECT_NE(output.find(\"Change web page to http://example.com.\"), std::string::npos);
}
")
    target_sources(command_tests PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/fallback_test.cpp)
else()
    target_sources(command_tests PRIVATE tests/test_command.cpp)
endif()
