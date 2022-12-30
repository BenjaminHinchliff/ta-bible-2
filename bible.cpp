#include "bible.hpp"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>
#endif

const char *BIBLE_PATH = "kjv.txt";

std::ostream &FragmentNode::print_helper(std::ostream &out,
                                         const FragmentNode &node,
                                         size_t depth) {
  for (size_t i = 0; i < depth; ++i) {
    out << '\t';
  }

  out << node.frag.length << " (" << node.frag.verse << ")\n";

  for (const auto &next : node.nexts) {
    print_helper(out, next, depth + 1);
  }

  return out;
}

std::ostream &operator<<(std::ostream &out, const FragmentNode &node) {
  FragmentNode::print_helper(out, node);

  return out;
}

std::string string_to_lower(std::string s) {
  std::transform(s.cbegin(), s.cend(), s.begin(),
                 [](const unsigned char c) { return std::tolower(c); });
  return s;
}

// remove all characters save for alphanumeric ones
std::string string_clean(std::string s) {
  s.erase(
      std::remove_if(s.begin(), s.end(),
                     [](const unsigned char c) { return !std::isalnum(c); }),
      s.end());
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

BibleIndex::BibleIndex(std::ifstream &bible) { index_bible(bible); }

std::vector<FragmentNode>
BibleIndex::build_fragments(const std::string &target) {
  auto words = get_words(target);
  std::transform(
      words.begin(), words.end(), words.begin(),
      [](std::string word) { return string_clean(string_to_lower(word)); });
  return build_fragments_impl(words, words.begin());
}

size_t score_fragment(const Fragment &frag) {
  return frag.length * frag.length;
}

void BibleIndex::index_bible(std::ifstream &bible) {
  std::string line;
  while (std::getline(bible, line)) {
    std::stringstream line_ss{line};

    std::string verse;
    std::getline(line_ss, verse, ' ');

    std::vector<std::string> words;

    std::string word;
    while (std::getline(line_ss, word, ' ')) {
      word = string_clean(string_to_lower(word));

      index.insert({word, verse});
      words.push_back(word);
    }

    verses.insert({verse, words});
  }
}

size_t BibleIndex::fragment_length(const std::string &verse_num,
                                   const std::vector<std::string> &target,
                                   const vec_str_iter_t &start) {
  const auto verse = verses.at(verse_num);

  auto t_it = start;
  auto v_it = std::find(verse.begin(), verse.end(), *t_it);
  while (v_it != verse.end() && t_it != target.end() && *v_it == *t_it) {
    ++v_it;
    ++t_it;
  }

  return t_it - start;
}

std::vector<FragmentNode>
BibleIndex::build_fragments_impl(const std::vector<std::string> &target,
                                 const vec_str_iter_t &start) {
  // fragments so far match the target
  if (start >= target.end()) {
    return {};
  }

  auto word = *start;

  std::unordered_map<size_t, std::string> fragments_set;

  // find verse matches
  auto range = index.equal_range(word);
  // no verse matches
  if (range.first == range.second) {
    throw BuildException{std::string{"Failed to find word \""} + word +
                         "\" anywhere in the text."};
  }

  for (auto it = range.first; it != range.second; ++it) {
    auto len = fragment_length(it->second, target, start);
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
    node.nexts = build_fragments_impl(target, start + node.frag.length);
  }

  return fragments;
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

std::ifstream get_bible_handle() {
  std::ifstream handle{BIBLE_PATH};
  if (!handle.is_open()) {
    throw std::runtime_error("Failed to open bible.");
  }

  return handle;
}
