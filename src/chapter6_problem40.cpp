
#ifdef __linux__
#include "gtest/gtest.h"

#include <print>
#include <string>

#include <dirent.h>
#include <sys/types.h>

namespace SCO {
TEST(ListDir, UNIX) {
  std::string rootDir = "../testing_dir";
  auto dirPointer = opendir(rootDir.c_str());
  while (auto dirEntry = readdir(dirPointer)) {
    struct stat stat;
    std::string filepath = rootDir + "/" + dirEntry->d_name;
    lstat(filepath.c_str(), &stat);
    std::print("[Dir entry] Name: '{}', Size: {} Bytes\n", dirEntry->d_name,
               stat.st_size);
  }
}
#endif
} // namespace SCO