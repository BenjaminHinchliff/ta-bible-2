#pragma once

#include <fstream>
#include <unordered_map>
#include <vector>

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
                                    size_t depth = 0);
};

class BibleIndex {
public:
  BibleIndex(std::ifstream &bible);

  std::vector<FragmentNode> build_fragments(const std::string &target);

public:
  class BuildException : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

private:
  void index_bible(std::ifstream &bible);

  using vec_str_iter_t = std::vector<std::string>::iterator;

  // TODO: these types are... so ugly - maybe find a better way to type this?
  size_t fragment_length(const std::string &verse_num,
                         const std::vector<std::string> &target,
                         const vec_str_iter_t &start);

  std::vector<FragmentNode>
  build_fragments_impl(const std::vector<std::string> &target,
                       const vec_str_iter_t &start);

private:
  std::unordered_multimap<std::string, std::string> index;
  std::unordered_map<std::string, std::vector<std::string>> verses;
};

std::pair<size_t, std::vector<Fragment>>
max_children_score(std::vector<Fragment> prefix, size_t score,
                   const std::vector<FragmentNode> &tree);

void output_seq(std::ostream &out, const std::string &source,
                const std::vector<Fragment> &seq);

std::ifstream get_bible_handle();
