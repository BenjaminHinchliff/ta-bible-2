#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <span>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

const char *BIBLE_PATH = "kjv.txt";

struct Fragment {
  size_t length;
  std::string source;
  std::vector<Fragment> nexts;
};

class BibleQuoter {
public:
  using verse_index_t = std::unordered_multimap<std::string, std::string>;

  BibleQuoter(std::ifstream &bible) { index_bible(bible); }

private:
  void index_bible(std::ifstream &bible) {
    std::string line;
    while (std::getline(bible, line)) {
      std::stringstream line_ss{line};

      std::string verse;
      std::getline(line_ss, verse, ' ');

      std::vector<std::string> words;

      std::string word;
      while (std::getline(line_ss, word, ' ')) {
        index.insert({word, verse});
        words.push_back(word);
      }

      verses.insert({verse, words});
    }
  }

  size_t fragment_length(std::string verse_num,
                         const std::span<const std::string> &target) {
    const auto verse = verses.at(verse_num);

    auto t_it = target.begin();
    auto v_it = std::find(verse.begin(), verse.end(), *t_it);
    while (v_it != verse.end() && t_it != target.end() && *v_it == *t_it) {
      ++v_it;
      ++t_it;
    }

    return t_it - target.begin();
  }

public:
  static std::vector<std::string> get_words(const std::string &s) {
    std::stringstream ss{s};

    std::vector<std::string> words;

    std::string word;
    while (std::getline(ss, word, ' ')) {
      words.push_back(word);
    }

    return words;
  }

  std::vector<Fragment> find_fragments(const std::string &prefixes,
                                       const std::vector<std::string> &target) {
    auto word = target.front();

    std::cout << word << ":\n";

    std::unordered_map<size_t, std::string> fragments_set;

    // find verse matches
    auto range = index.equal_range(word);
    for (auto it = range.first; it != range.second; ++it) {
      std::cout << '\t' << it->second << ": ";

      auto len = fragment_length(it->second, target);
      fragments_set.insert({len, it->second});
      // for (const auto word : fragment) {
      //   std::cout << word << ' ';
      // }

      std::cout << ";\n";
    }

    std::vector<Fragment> fragments;
    std::transform(fragments_set.begin(), fragments_set.end(),
                   std::back_inserter(fragments), [](auto frag) {
                     return Fragment{.length = std::get<0>(frag),
                                     .source = std::get<1>(frag)};
                   });

    return fragments;
  }

private:
  verse_index_t index;
  std::unordered_map<std::string, std::vector<std::string>> verses;
};

int main() {
  using namespace std::chrono_literals;
  std::ifstream bible{BIBLE_PATH};
  if (!bible.is_open()) {
    std::cerr << "Failed to open bible (" << BIBLE_PATH << ")\n";
  }

  std::cerr << "Indexing bible...\n";
  BibleQuoter quoter{bible};

  auto target = BibleQuoter::get_words("the beginning");
  quoter.find_fragments({}, target);

  return 0;
}
