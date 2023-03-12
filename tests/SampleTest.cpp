
#include "gtest/gtest.h"
#include "MiniPL.h"


#include <filesystem>
namespace fs = std::filesystem;

class SampleTest :  
  public ::testing::Test
  // public testing::TestWithParam<std::tuple<Program, std::string, std::string>> 
{
  public:
    
  protected:
    void SetUp() override {
    }
    std::stringstream in, out;
    MiniPL miniPl{in, out};
    
};
TEST_F(SampleTest, samples) {

  std::string path = std::string(ROOT_DIR) + "/samples";
  // for (const auto & entry : fs::directory_iterator(path)){
  //   // std::cout << entry.path() << std::endl;
  //   EXPECT_NO_THROW(miniPl.runFile(entry.path()));
  // }
  miniPl.runFile(path+"/0.mpl");

}

