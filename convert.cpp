#include "convert.hpp"

#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "MyPriorityQueue.hpp"

struct Node {
  unsigned lcdistance = 0;
  unsigned letterdiff = 0;
  std::vector<std::string> path;
  friend bool operator<(const Node& l, const Node& r) {
    if ((l.lcdistance + l.letterdiff) == (r.lcdistance + r.letterdiff)) {
      return l.lcdistance <= r.lcdistance;
    } else {
      return (l.lcdistance + l.letterdiff) < (r.lcdistance + r.letterdiff);
    }
  }

  friend bool operator<=(const Node& l, const Node& r) {
    if ((l.lcdistance + l.letterdiff) == (r.lcdistance + r.letterdiff)) {
      return l.lcdistance <= r.lcdistance;
    } else {
      return (l.lcdistance + l.letterdiff) < (r.lcdistance + r.letterdiff);
    }
  }
};


void loadWordsIntoTable(std::unordered_set<std::string>& words,
                        std::istream& in) {
  std::string line, word;
  std::stringstream ss;

  while (getline(in, line)) {
    ss.clear();
    ss << line;
    while (ss >> word) {
      words.insert(word);
    }
  }
}

unsigned letterDifference(const std::string& word, const std::string& s2) {
  // Calculate the difference in letters between a word and final string.
  unsigned diff = 0;
  for (int i = 0; i < s2.length(); i++) {
    if (word[i] != s2[i]) {
      diff += 1;
    }
  }
  return diff;
}

std::vector<std::string> convert(const std::string& s1, const std::string& s2,
                                 const std::unordered_set<std::string>& words) {
  if ((s1 == s2) || (s1.length() != s2.length())) {
    return {};
  }
  MyPriorityQueue<struct Node> pq;
  std::unordered_set<std::string> discovered{s1};
  struct Node start;
  start.path.push_back(s1);
  pq.insert(start);
  while (!pq.isEmpty()) {
    std::vector<std::string> path = pq.min().path;
    unsigned lcdistance = pq.min().lcdistance;
    pq.extractMin();
    std::string current = path.back();
    if (current == s2) {
      return path;
    }
    for (int i = 0; i < current.length(); i++) {
      std::string match = current;
      for (char j = 'a'; j <= 'z'; j++) {
        match[i] = j;
        if ((words.find(match) != words.end()) &&
            (discovered.find(match) == discovered.end())) {
          std::vector<std::string> new_path = path;
          new_path.push_back(match);
          struct Node new_node;
          new_node.lcdistance = lcdistance + 1;
          new_node.letterdiff = letterDifference(match, s2);
          new_node.path = new_path;
          discovered.insert(match);
          pq.insert(new_node);
        }
      }
    }
  }
  return {};
}
