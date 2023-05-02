#include <string_view>
#include <algorithm>
#include <cctype>
#include <cstring>

using namespace std::literals;

#include <gtest/gtest.h>
#include <utils/log.h>

std::string_view
getHTTPStatusCode(const std::string_view &str) {
    const unsigned int STATUS_CODE_LEN = 3;
    #define SUBSTR_OF_BUF " >> " << (str.length() ? str.substr(0,128) : "No status line") << "..."
    if (str.length() < 9) { // the worst minimum length case: "HTTP/ " + "123" + ""
        LOG(ERROR)<<"There is no expected minimum length for the status-line." << SUBSTR_OF_BUF;
        return "";
    }

    if (str.compare(0, 5, "HTTP/") != 0) {
        LOG(ERROR)<<"There is no the expected HTTP/ prefix." << SUBSTR_OF_BUF;
        return "";
    }

    auto pos = str.find(' ', 5); // find the first space after "HTTP/"
    if (pos == std::string_view::npos || pos == str.length() - 1) { // no space or no status code
        LOG(ERROR)<<"There is no the expected space before status code." << SUBSTR_OF_BUF;
        return "";
    }

    std::string_view code = str.substr(pos + 1, STATUS_CODE_LEN); // status code is 3-digit long
    if (code.length() != STATUS_CODE_LEN) {
        LOG(ERROR)<<"The length of status code is equals to " << STATUS_CODE_LEN << "."<< SUBSTR_OF_BUF;
        return "";
    }
    auto has_2sp = false;
    auto sep_pos = pos + 1 + STATUS_CODE_LEN; // go to check the second seprate character which could be " \r\n\0" or nothing.
    if(sep_pos < str.length()) { // avoid accessing out of range elements, and we also accept the buffer ends with the status code.
        switch(str.at(sep_pos))
        {
        case ' ':
            has_2sp = true;
        case '\r':
        case '\n':
        case '\0':
            break;
        default:
            LOG(ERROR) << "There is no expected separator character for the status code." << SUBSTR_OF_BUF;
            return "";
        }
    }

    if (code.find_first_not_of("0123456789") != std::string_view::npos) { // not all digits
        LOG(ERROR)<<"The expected status code should be 3-digital number." << SUBSTR_OF_BUF;
        return "";
    }

    // We only help verify simple cases with additional checks to ensure that the format is not perfect,
    // we should take adding new templates seriously and not rely on the feedback from checks here.
    if (str.compare(5, 4, "1.1 ") != 0 && str.compare(5, 4, "1.0 ") != 0) {
         LOG(INFO)<<"Unexpected HTTP protocol version." << SUBSTR_OF_BUF;
    }
    if (!has_2sp) {
         LOG(INFO)<<"Unexpected status code is not between two space." << SUBSTR_OF_BUF;
    }
    return code;
}

std::string_view getHTTPStatusCode(const char *buf, int len)
{
    std::string_view s(buf, len);
    return getHTTPStatusCode(s);
}

TEST(getHTTPStatusCodeTest, NormalCase) {
  EXPECT_EQ(getHTTPStatusCode("HTTP/1.1 200 OK\r\n"sv), "200");
  EXPECT_EQ(getHTTPStatusCode("HTTP/1.0 200 OK\r\n"sv), "200");
}

TEST(getHTTPStatusCodeTest, AccpetableButNotGoodCase) {
  EXPECT_EQ(getHTTPStatusCode("HTTP/1. 200 OK\r\n"sv), "200");
  EXPECT_EQ(getHTTPStatusCode("HTTP/ 200 OK\r\n"sv), "200");
  EXPECT_EQ(getHTTPStatusCode("HTTP/x.y 200 OK\r\n"sv), "200");
}

TEST(getHTTPStatusCodeTest, MissingReasonSuffixButOK) {
  EXPECT_EQ(getHTTPStatusCode("HTTP/1.1 200\r\n"sv), "200");
  EXPECT_EQ(getHTTPStatusCode("HTTP/1.1 200\r"sv), "200");
  EXPECT_EQ(getHTTPStatusCode("HTTP/1.1 200\n"sv), "200");
  EXPECT_EQ(getHTTPStatusCode("HTTP/1.1 200"sv), "200");
}

TEST(getHTTPStatusCodeTest, CaseInsensitive) {
  EXPECT_EQ(getHTTPStatusCode("http/1.1 404 NOT FOUND\r\n"sv), "");
  EXPECT_EQ(getHTTPStatusCode("Http/1.0 301 MOVED PERMANENTLY\r\n"sv), "");
}

TEST(getHTTPStatusCodeTest, MissingHTTPPrefix) {
  EXPECT_EQ(getHTTPStatusCode("200 OK\r\n"sv), "");
  EXPECT_EQ(getHTTPStatusCode("HTTP1.0 200 OK\r\n"sv), "");
}

TEST(getHTTPStatusCodeTest, MissingStatusCode) {
  EXPECT_EQ(getHTTPStatusCode("HTTP/1.1 OK\r\n"sv), "");
  EXPECT_EQ(getHTTPStatusCode("HTTP/1.1"sv), "");
  EXPECT_EQ(getHTTPStatusCode("HTTP/1.1 "sv), "");
}

TEST(getHTTPStatusCodeTest, BadStatusCode) {
  EXPECT_EQ(getHTTPStatusCode("HTTP/1.1 1"sv), "");
  EXPECT_EQ(getHTTPStatusCode("HTTP/1.1 12"sv), "");
}

TEST(getHTTPStatusCodeTest, InvalidStatusCode) {
  EXPECT_EQ(getHTTPStatusCode("HTTP/1.1 2000 OK\r\n"sv), "");
  EXPECT_EQ(getHTTPStatusCode("HTTP/1.1 20 OK\r\n"sv), "");
  EXPECT_EQ(getHTTPStatusCode("HTTP/1.1 2a OK\r\n"sv), "");
}

TEST(getHTTPStatusCodeTest, EmptyInput) {
  EXPECT_EQ(getHTTPStatusCode(""sv), "");
  //EXPECT_EQ(getHTTPStatusCode(nullptr), ""); imposible for nullptr
}

int main(int argc, char *argv[])
{
    LOG(INFO)<<"start to test...";
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
