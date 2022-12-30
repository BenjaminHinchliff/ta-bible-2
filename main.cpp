#include "bible.hpp"

#include <iostream>

std::string parse_target(int argc, char *argv[]) {
  using namespace std::string_literals;
  if (argc <= 1) {
    throw std::runtime_error("No target string passed.");
  }

  std::string target = argv[1];
  for (int i = 2; i < argc; ++i) {
    target += " "s + argv[i];
  }

  return target;
}

// file containing main method, not used for emscripten build
int main(int argc, char *argv[]) {
  std::string target;
  try {
    target = parse_target(argc, argv);

    std::ifstream bible = get_bible_handle();

    std::cerr << "Indexing bible...\n";
    BibleIndex index{bible};

    std::cerr << "Building fragment tree...\n";

    std::vector<FragmentNode> frags = index.build_fragments(target);

    std::cerr << "Scoring fragment tree...\n";
    std::vector<Fragment> prefix;
    auto [max_score, max_seq] = max_children_score(prefix, 0, frags);

    output_seq(std::cout, target, max_seq);
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << " Stopping.\n";
    return 1;
  }

  return 0;
}
