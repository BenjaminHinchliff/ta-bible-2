#pragma once

#include <cassert>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

constexpr size_t ALPHA_SIZE = 'z' - 'a' + 1;

class TrieNode {
public:
  using verses_t = std::unordered_set<std::string>;

  std::unique_ptr<TrieNode> children[ALPHA_SIZE];
  std::optional<verses_t> verses;
  char letter;

  TrieNode() : TrieNode('\0') {}

  TrieNode(char letter, std::optional<verses_t> verses = {}) {
    this->letter = letter;
    this->verses = verses;
    for (size_t i = 0; i < ALPHA_SIZE; i++) {
      children[i] = nullptr;
    }
  }
};

class Trie {
public:
  TrieNode root;

  void insert(const std::string &key, std::string verse) {
    // don't want to take any ownership of the unique pointer
    TrieNode *current = &root;
    for (char c : key) {
      size_t i = c - 'a';
      assert(i < ALPHA_SIZE && "trie index must be within alpha size");
      auto &child = current->children[i];
      if (!child) {
        child = std::make_unique<TrieNode>(c);
      }
      current = current->children[i].get();
    }
    if (!current->verses.has_value()) {
      current->verses = std::make_optional<TrieNode::verses_t>();
    }
    current->verses->insert(verse);
  }

  using verses_it_t = TrieNode::verses_t::const_iterator;

  std::optional<std::pair<verses_it_t, verses_it_t>>
  search(const std::string &key) const {
    const TrieNode *current = &root;
    for (char c : key) {
      size_t i = c - 'a';
      assert(i < ALPHA_SIZE && "trie index must be within alpha size");
      const auto &child = current->children[i];
      if (!child) {
        return {};
      }
      current = child.get();
    }
    const auto &verse = current->verses;
    if (!verse.has_value()) {
      return {};
    }
    return {{verse->begin(), verse->end()}};
  }
};
