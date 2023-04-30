#include "Wordset.hpp"

#include <cmath>
#include <string>

// returns s, as a number in the given base, mod the given modulus
void WordSet::resize() {
  unsigned old_capacity = capacity;
  capacity = 2 * capacity;
  bool isprime = false;
  while (!isprime) {
    isprime = true;
    for (int i = 2; i <= (capacity / 2); i++) {
      if (capacity % i == 0) {
        capacity += 1;
        isprime = false;
        break;
      }
    }
  }
  std::string* old_T1 = T1;
  std::string* old_T2 = T2;
  T1 = new std::string[capacity];
  T2 = new std::string[capacity];
  for (int i = 0; i < old_capacity; i++) {
    if (old_T1[i] != "") {
      insert(old_T1[i]);
    }
    if (old_T2[i] != "") {
      insert(old_T2[i]);
    }
  }
  delete[] old_T1;
  delete[] old_T2;
}

unsigned polynomialHashFunction(const std::string& s, unsigned base,
                                unsigned mod) {
  unsigned p = 0, coeff = 0;
  long long aggregate = 0;
  for (int i = 0; i < s.length(); i++) {
    p = s.length() - i - 1;  // Power of each term of polynomial
    coeff = s[i] - 'a' + 1;  // Coefficient for each term.
    long long term = (coeff % mod);
    while (p > 0) {
      term *= (base % mod);
      term = term % mod;
      p -= 1;
    }
    aggregate += term;
  }
  unsigned result = (long long)aggregate % mod;
  return result;
}

WordSet::WordSet(unsigned initialCapacity, unsigned evictionThreshold) {
  eviction_threshold = evictionThreshold;
  capacity = initialCapacity;
  T1 = new std::string[capacity];
  T2 = new std::string[capacity];
}

WordSet::~WordSet() {
  delete[] T1;
  delete[] T2;
}

void WordSet::insert(const std::string& s) {
  unsigned evictions = 0;
  unsigned index = polynomialHashFunction(s, BASE_H1, capacity);
  if (T1[index] == "")
    T1[index] = s;
  else {
    std::string evicted = T1[index];
    T1[index] = s;
    evictions += 1;
    unsigned counter =
        2;  // Counter for deciding which table to try to insert in.
    while (evicted != "") {
      if (evictions > eviction_threshold) {
        resize();
        evictions = 0;
      }

      if (counter == 2) {
        index = polynomialHashFunction(evicted, BASE_H2, capacity);
        std::string new_evicted = T2[index];
        T2[index] = evicted;
        evicted = new_evicted;
        counter = 1;
      }

      else if (counter == 1) {
        index = polynomialHashFunction(evicted, BASE_H1, capacity);
        std::string new_evicted = T1[index];
        T1[index] = evicted;
        evicted = new_evicted;
        counter = 2;
      }
      evictions++;
    }
  }
}

bool WordSet::contains(const std::string& s) const {
  unsigned index = polynomialHashFunction(s, BASE_H1, capacity);
  if (T1[index] == s) return true;
  index = polynomialHashFunction(s, BASE_H2, capacity);
  if (T2[index] == s) return true;
  return false;
}

// return how many distinct strings are in the set
unsigned WordSet::getCount() const {
  unsigned count = 0;
  for (int i = 0; i < capacity; i++) {
    if (T1[i] != "") count += 1;
    if (T2[i] != "") count += 1;
  }
  return count;
}

// return how large the underlying array is.
unsigned WordSet::getCapacity() const { return capacity; }
