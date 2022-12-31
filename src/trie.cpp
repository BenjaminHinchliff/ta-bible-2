#include "trie.hpp"

VerseTrieNode::VerseTrieNode(char letter, std::optional<verses_t> verses) {
  this->letter = letter;
  this->verses = verses;
  for (size_t i = 0; i < VerseTrieNode::ALPHA_SIZE; i++) {
    children[i] = nullptr;
  }
}

void VerseTrie::insert(const std::string &key, std::string verse) {
  // don't want to take any ownership of the unique pointer
  VerseTrieNode *current = &root;
  for (char c : key) {
    size_t i = c - 'a';
    assert(i < ALPHA_SIZE && "trie index must be within alpha size");
    auto &child = current->children[i];
    if (!child) {
      child = std::make_unique<VerseTrieNode>(c);
    }
    current = current->children[i].get();
  }
  if (!current->verses.has_value()) {
    current->verses = std::make_optional<VerseTrieNode::verses_t>();
  }
  current->verses->insert(verse);
}

std::optional<std::pair<VerseTrie::verses_it_t, VerseTrie::verses_it_t>>
VerseTrie::search(const std::string &key) const {
  const VerseTrieNode *current = &root;
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
