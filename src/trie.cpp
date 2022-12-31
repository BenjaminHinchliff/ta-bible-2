#include "trie.hpp"

TrieNode::TrieNode(char letter, std::optional<verses_t> verses) {
  this->letter = letter;
  this->verses = verses;
  for (size_t i = 0; i < TrieNode::ALPHA_SIZE; i++) {
    children[i] = nullptr;
  }
}

void Trie::insert(const std::string &key, std::string verse) {
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

std::optional<std::pair<Trie::verses_it_t, Trie::verses_it_t>>
Trie::search(const std::string &key) const {
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
