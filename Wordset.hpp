#ifndef __WORD_SET___HPP
#define __WORD_SET___HPP

#include <string>

unsigned polynomialHashFunction(const std::string& s, unsigned base,
                                unsigned mod);

class WordSet {
 public:
  explicit WordSet(unsigned initialCapacity,
                   unsigned evictionThreshold = DEFAULT_EVICT_THRESHOLD);
  ~WordSet();

  void insert(const std::string& s);

  bool contains(const std::string& s) const;

  // return how many distinct strings are in the set
  unsigned getCount() const;

  // return how large the underlying array is.
  unsigned getCapacity() const;

 private:
  static constexpr unsigned BASE_H1 = 37;
  static constexpr unsigned BASE_H2 = 41;
  unsigned capacity;
  std::string* T1;
  std::string* T2;
  unsigned eviction_threshold;
  void resize();  // When we resize the hashtable.
  static constexpr unsigned DEFAULT_EVICT_THRESHOLD = 5;
};

#endif
