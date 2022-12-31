#pragma once

#include <cassert>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

class TrieNode {
public:
  static constexpr size_t ALPHA_SIZE = 'z' - 'a' + 1;

  using verses_t = std::unordered_set<std::string>;

  std::unique_ptr<TrieNode> children[ALPHA_SIZE];
  std::optional<verses_t> verses;
  char letter;

  TrieNode() : TrieNode('\0') {}

  TrieNode(char letter, std::optional<verses_t> verses = {});
};

class Trie {
public:
  TrieNode root;

  void insert(const std::string &key, std::string verse);

  using verses_it_t = TrieNode::verses_t::const_iterator;

  std::optional<std::pair<verses_it_t, verses_it_t>>
  search(const std::string &key) const;
};
