#include <string>
#include <iostream>
#include <cstdlib>


std::string f(std::string x) {
  if (x=="") {
    return "";
  }
  char first = x[0];
  std::string rest = x.substr(1);
  std::cout << "first = " << first << " and rest = " << rest << std::endl;
  std::string ans = f(rest);
  ans.push_back(first);
  std::cout << "ans = " << ans << std::endl;
  return ans;
}

int main(void) {
  std::string start("Apple");
  std::string ans = f(start);
  std::cout << "f(" << start << ") = " << ans << std::endl;
  return EXIT_SUCCESS;
}
