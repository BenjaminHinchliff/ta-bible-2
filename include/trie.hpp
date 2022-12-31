#pragma once

#include <cassert>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

class VerseTrieNode {
public:
  static constexpr size_t ALPHA_SIZE = 'z' - 'a' + 1;

  using verses_t = std::unordered_set<std::string>;

  std::unique_ptr<VerseTrieNode> children[ALPHA_SIZE];
  std::optional<verses_t> verses;
  char letter;

  VerseTrieNode() : VerseTrieNode('\0') {}

  VerseTrieNode(char letter, std::optional<verses_t> verses = {});
};

class VerseTrie {
public:
  VerseTrieNode root;

  void insert(const std::string &key, std::string verse);

  using verses_it_t = VerseTrieNode::verses_t::const_iterator;

  std::optional<std::pair<verses_it_t, verses_it_t>>
  search(const std::string &key) const;
};
