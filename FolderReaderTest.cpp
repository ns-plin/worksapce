#include <iostream>
#include <filesystem>
#include <experimental/filesystem>
#include <fstream>
#include <dirent.h>
#include <string_view>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "utils/FolderReader.h"

using namespace std::literals;

class TestReader : public IReader {
public:
    virtual int walkReadFile(const std::string& folder_name, const std::string& filename, const std::string_view& sv) {
        std::cout << "Reading file " << filename << " in folder " << folder_name << std::endl;
        return 0;
    }
};

TEST(FolderReaderTest, BasicTest) {
    FolderReader reader;
    TestReader test_reader;
    std::string folder = "./test_folder";
    int result = reader.runFolderFileData(folder, test_reader, true, 0);
    EXPECT_EQ(result, 0);
}

class MockReader : public IReader {
public:
    MOCK_METHOD(int, walkReadFile, (const std::string&, const std::string&, const std::string_view&), (override));
};

TEST(FolderReaderTest, TestRunWithFilesAndContent) {
    // create test folder and files
    std::string test_dir = "test_dir";
    std::filesystem::create_directory(test_dir);
    std::ofstream file1(test_dir + "/file1.txt");
    file1 << "Hello, World!";
    file1.close();
    std::ofstream file2(test_dir + "/file2.txt");
    file2 << "Testing FolderReader";
    file2.close();

    MockReader mock_reader;

    // expect to call walkReadFile for twice
    EXPECT_CALL(mock_reader, walkReadFile(test_dir, "file1.txt", "Hello, World!"sv)).WillOnce(testing::Return(0));
    EXPECT_CALL(mock_reader, walkReadFile(test_dir, "file2.txt", "Testing FolderReader"sv)).WillOnce(testing::Return(0));

    // 執行 FolderReader::runFolderFileData
    FolderReader folder_reader;
    folder_reader.runFolderFileData(test_dir, mock_reader, false, 0);
    std::filesystem::remove_all(test_dir);
}

TEST(FolderReaderTest, TestRunWithFilesAndCounts) {
    // create test folder and files
    std::string test_dir = "test_dir";
    std::filesystem::create_directory(test_dir);
    std::ofstream file1(test_dir + "/file1.txt");
    file1 << "Hello, World!";
    file1.close();
    std::ofstream file2(test_dir + "/file2.txt");
    file2 << "Testing FolderReader";
    file2.close();

    MockReader mock_reader;

    // expect to call walkReadFile for twice
    EXPECT_CALL(mock_reader, walkReadFile(test_dir, testing::_, testing::_))
        .Times(testing::AtLeast(2))
        .WillRepeatedly(testing::Return(0));


    // 執行 FolderReader::runFolderFileData
    FolderReader folder_reader;
    folder_reader.runFolderFileData(test_dir, mock_reader, false, 0);
    std::filesystem::remove_all(test_dir);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
