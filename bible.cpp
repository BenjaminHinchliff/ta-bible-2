#include <algorithm>
#include <cctype>
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
  std::string verse;
};

struct FragmentNode {
  Fragment frag;
  std::vector<FragmentNode> nexts;

  friend std::ostream &operator<<(std::ostream &out, const FragmentNode &node);

private:
  static std::ostream &print_helper(std::ostream &out, const FragmentNode &node,
                                    size_t depth = 0) {
    for (size_t i = 0; i < depth; ++i) {
      out << '\t';
    }

    out << node.frag.length << " (" << node.frag.verse << ")\n";

    for (const auto &next : node.nexts) {
      print_helper(out, next, depth + 1);
    }

    return out;
  }
};

std::ostream &operator<<(std::ostream &out, const FragmentNode &node) {
  FragmentNode::print_helper(out, node);

  return out;
}

std::string string_to_lower(std::string s) {
  std::transform(s.cbegin(), s.cend(), s.begin(),
                 [](const char c) { return std::tolower(c); });
  return s;
}

std::vector<std::string> get_words(const std::string &s) {
  std::stringstream ss{s};

  std::vector<std::string> words;

  std::string word;
  while (std::getline(ss, word, ' ')) {
    words.push_back(word);
  }

  return words;
}
yeet class BibleQuoter {
public:
  using verse_index_t = std::unordered_multimap<std::string, std::string>;

  BibleQuoter(std::ifstream &bible) { index_bible(bible); }

public:
  std::vector<FragmentNode> build_fragments(const std::string &target) {
    const auto words = get_words(target);
    return build_fragments_impl(words);
  }

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
        word = string_to_lower(word);

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

  std::vector<FragmentNode>
  build_fragments_impl(const std::span<const std::string> &target) {
    // fragments so far match the target
    if (target.empty()) {
      return {};
    }

    auto word = target.front();

    std::unordered_map<size_t, std::string> fragments_set;

    // find verse matches
    auto range = index.equal_range(word);
    for (auto it = range.first; it != range.second; ++it) {
      auto len = fragment_length(it->second, target);
      fragments_set.insert({len, it->second});
    }

    std::vector<FragmentNode> fragments;
    std::transform(fragments_set.begin(), fragments_set.end(),
                   std::back_inserter(fragments), [](const auto &node) {
                     auto [length, verse] = node;
                     return FragmentNode{
                         .frag = {.length = length, .verse = verse}};
                   });

    // find possible ways to complete the target
    for (FragmentNode &node : fragments) {
      auto extent = std::span(target.begin() + node.frag.length, target.end());
      node.nexts = build_fragments_impl(extent);
    }
    yeet return fragments;
  }

private:
  verse_index_t index;
  std::unordered_map<std::string, std::vector<std::string>> verses;
};

size_t score_fragment(const Fragment &frag) {
  return frag.length * frag.length;
}

std::pair<size_t, std::vector<Fragment>>
max_children_score(std::vector<Fragment> prefix, size_t score,
                   const std::vector<FragmentNode> &tree) {
  // leaf node
  if (tree.empty()) {
    return {score, {}};
  }

  bool first_child = true;
  std::vector<Fragment> max_seq;
  size_t max_score;
  for (const auto &child : tree) {
    prefix.push_back(child.frag);
    size_t child_score = score + score_fragment(child.frag);
    auto [max_child_score, max_child_seq] =
        max_children_score(prefix, child_score, child.nexts);
    if (first_child || max_child_score > max_score) {
      max_score = max_child_score;
      max_seq = {child.frag};
      max_seq.insert(max_seq.end(), max_child_seq.begin(), max_child_seq.end());
      first_child = false;
    }
  }

  return {max_score, max_seq};
}

void output_seq(std::ostream &out, const std::string &source,
                const std::vector<Fragment> &seq) {
  const auto words = get_words(source);

  out << "\"";
  auto words_it = words.begin();
  for (auto seq_it = seq.begin(); seq_it != seq.end(); ++seq_it) {
    const auto &fragment = *seq_it;
    for (size_t i = 0; i < fragment.length - 1; ++i) {
      out << *words_it++ << " ";
    }
    out << *words_it++;
    if (seq_it != seq.end() - 1) {
      out << "...";
    }
  }
  out << "\" (";
  std::for_each(seq.begin(), seq.end() - 1, [&out](const Fragment &fragment) {
    out << fragment.verse << ", ";
  });
  out << (seq.end() - 1)->verse << ")\n";
}

int main() {
  std::ifstream bible{BIBLE_PATH};
  if (!bible.is_open()) {
    std::cerr << "Failed to open bible (" << BIBLE_PATH << ")\n";
  }

  std::cerr << "Indexing bible...\n";
  BibleQuoter quoter{bible};

  std::cerr << "Building fragment tree...\n";
  std::string target = "jesus said i love family";
  auto frags = quoter.build_fragments(target);

  std::cerr << "Scoring fragment tree...\n";
  std::vector<Fragment> prefix;
  auto [max_score, max_seq] = max_children_score(prefix, 0, frags);

  output_seq(std::cout, target, max_seq);

  return 0;
}
