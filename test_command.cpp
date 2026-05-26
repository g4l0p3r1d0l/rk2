#include <gtest/gtest.h>
#include <sstream>
#include "webpage_button.cpp"  // Подключаем весь код

TEST(CommandPatternTest, ChangeWebPageCommandExecutesChangeWebPage) {
    // Перехватываем stdout
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    WebPageManager manager;
    ChangeWebPageCommand cmd(manager, "http://example.com");
    cmd.Execute();

    std::cout.rdbuf(old);
    std::string output = buffer.str();
    EXPECT_NE(output.find("Change web page to http://example.com."), std::string::npos);
}
