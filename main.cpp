#include "bible.hpp"

#include <iostream>

std::string parse_target(int argc, char *argv[]) {
  using namespace std::string_literals;

  std::string target;
  if (argc > 1) {
    target = argv[1];
    for (int i = 2; i < argc; ++i) {
      target += " "s + argv[i];
    }
  }

  return target;
}

// file containing main method, not used for emscripten build
int main(int argc, char *argv[]) {
  std::string target;
  try {
    target = parse_target(argc, argv);

    std::cerr << "Indexing bible...\n";
    BibleIndex index{};

    std::cerr << "Building quote...\n";
    std::cout << build_quote(index, target) << '\n';

  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << " Stopping.\n";
    return 1;
  }

  return 0;
}
