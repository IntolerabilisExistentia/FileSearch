#include <gtest/gtest.h>
#include <lib/IndexLib/PostingsFile.hpp>
#include <lib/SearchLib/FileSearch.hpp>
#include <string>

TEST(PostingsListTest, CreatePostingsListTest) {
  std::ofstream testFile("test.txt");
  testFile << "Hello World\nHello Labwork";
  testFile.close();
  
  auto postingsList = createPostingsList("test.txt");
  
  EXPECT_EQ(postingsList["hello"], std::vector<int>({0, 2}));
  EXPECT_EQ(postingsList["world"], std::vector<int>({1}));
  EXPECT_EQ(postingsList["labwork"], std::vector<int>({3}));

  std::remove("test.txt");
}

class FileReaderTest : public ::testing::Test {
protected:
  std::string testDir = "./testDir";
  
  void SetUp() override {
    std::filesystem::create_directory(testDir);
    std::ofstream(testDir + "/file1.txt") << "Hello world\n";
    std::filesystem::create_directory(testDir + "/subdir");
    std::ofstream(testDir + "/subdir/file2.txt") << "Labwork test\n";
  }

  void TearDown() override {
    std::filesystem::remove_all(testDir);
  }
};

TEST_F(FileReaderTest, GetFilesTest) {
    FileReader fileReader;
    char* argv[] = { "program", "test", "./testDir" };
    EXPECT_TRUE(fileReader.GetFiles(3, argv));
    
    auto filesIndex = fileReader.GetFilesIndex();
    ASSERT_EQ(filesIndex.size(), 1);
    EXPECT_EQ(filesIndex[0].first, "./testDir");
    EXPECT_EQ(filesIndex[0].second.size(), 2);
}

TEST_F(FileReaderTest, GetFilesNonExistentDirectoryTest) {
  FileReader fileReader;
  char* argv[] = { "program", "test", "./nonExistentDir" };
  EXPECT_FALSE(fileReader.GetFiles(3, argv));
  
  auto filesIndex = fileReader.GetFilesIndex();
  EXPECT_EQ(filesIndex.size(), 0);
}

TEST_F(FileReaderTest, WriteToFileTest) {
  FileReader fileReader;
  char* argv[] = { "program", "test", "./testDir" };
  fileReader.GetFiles(3, argv);

  std::ofstream outFile("output.txt");
  std::vector<uint64_t> wordsCount = { 2, 5, 3 };
  fileReader.WriteToFile(outFile, wordsCount);
  outFile.close();
  
  std::ifstream outputFile("output.txt");
  ASSERT_TRUE(outputFile.is_open());

  std::string line;
  std::getline(outputFile, line);
  EXPECT_EQ(line, "2 1");

  std::getline(outputFile, line);
  EXPECT_EQ(line, "./testDir 2");

  std::getline(outputFile, line);
  EXPECT_EQ(line, "/subdir/file2.txt 5");

  std::getline(outputFile, line);
  EXPECT_EQ(line, "/file1.txt 3");
  
  outputFile.close();
  std::remove("output.txt");
}

TEST_F(FileReaderTest, CreatePostingsForDirectoryTest) {
  FileReader fileReader;
  char* argv[] = { "program", "test", "./testDir" };
  fileReader.GetFiles(3, argv);
  
  createPostingsForDirectory(fileReader, "output.txt");
  
  std::ifstream outputFile("output.txt");
  ASSERT_TRUE(outputFile.is_open());

  std::string line;
  std::getline(outputFile, line);
  EXPECT_EQ(line, "File: ./testDir/subdir/file2.txt");

  std::getline(outputFile, line);
  EXPECT_EQ(line, "labwork: 0");

  std::getline(outputFile, line);
  EXPECT_EQ(line, "test: 1");

  std::getline(outputFile, line);
  EXPECT_EQ(line, "");

  std::getline(outputFile, line);
  EXPECT_EQ(line, "File: ./testDir/file1.txt");

  std::getline(outputFile, line);
  EXPECT_EQ(line, "hello: 0");

  std::getline(outputFile, line);
  EXPECT_EQ(line, "world: 1");

  outputFile.close();
  std::remove("output.txt");
}
