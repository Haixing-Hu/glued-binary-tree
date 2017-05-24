#include <iostream>
#include <string>
#include <cstdlib>
#include <random>
#include <cassert>
using namespace std;

#ifdef DEBUG
  #define PRINT_NODE(v, p, l, r)        \
    cout << "DEBUG: node = " << v       \
         << " parent = " << p           \
         << " left child = " << l       \
         << " right child = " << r << endl
#else
  #define PRINT_NODE(v, p, l, r)        (void)0
#endif

template <class Generator>
inline
size_t random_choose(Generator& random, size_t x, size_t y)
{
  auto r = random();
  return (r % 2 == 0) ? x : y;
}

template <class Generator>
inline
size_t random_choose(Generator& random, size_t x, size_t y, size_t z)
{
  auto r = random();
  switch (r % 3) {
    case 0:
      return x;
    case 1:
      return y;
    default:
      return z;
  }
}

template <class Generator>
void random_walk(Generator& random, int level)
{
  assert(level >= 1);
  size_t const n = 2 * ((1 << (level + 1)) - 1);
  size_t const dest = n - 1;
  size_t const left_start = (1 << level) - 1;
  size_t const right_start = (1 << (level + 1)) - 1;
  size_t const leaves = (1 << level);
  size_t const left_end = left_start + leaves - 1;
  size_t const right_end = right_start + leaves - 1;
  size_t steps = 0;
  size_t current = 0;
  while (current != dest) {
    size_t next;
    if (current == 0) {     // the start vertex
      next = random_choose(random, 1, 2);
    } else if (current < left_start) {   // the middle vertex in the left tree
      size_t const parent = (current - 1) / 2;
      size_t const left_child = 2 * current + 1;
      size_t const right_child = left_child + 1;
      next = random_choose(random, parent, left_child, right_child);
      PRINT_NODE(current, parent, left_child, right_child);
    } else if (current > right_start) {  // the middle vertex in the right tree
      size_t const parent = dest - (dest - current - 1) / 2;
      size_t const left_child = 2 * current - dest - 2;
      size_t const right_child = left_child + 1;
      next = random_choose(random, parent, left_child, right_child);
      PRINT_NODE(current, parent, left_child, right_child);
    } else if ((current >= left_start) && (current <= left_end)) {  // the left leaves
      size_t const parent = (current - 1) / 2;
      size_t const left_child = right_start + (current - left_start);
      size_t const right_child = right_start + (current - left_start + 1) % leaves;
      next = random_choose(random, parent, left_child, right_child);
      PRINT_NODE(current, parent, left_child, right_child);
    } else { // the right leaves
      assert((current >= right_start) && (current <= right_end));
      size_t const parent = dest - (dest - current - 1) / 2;
      size_t const left_child = left_start + (current - right_start - 1 + leaves) % leaves;
      size_t const right_child = left_start + (current - right_start);
      next = random_choose(random, parent, left_child, right_child);
      PRINT_NODE(current, parent, left_child, right_child);
    }
    current = next;
    ++steps;
  }
  cout << "Level of binary trees: " << level << endl;
  cout << "Total number of nodes: " << n << endl;
  cout << "Total number of steps: " << steps << endl;
}


int main(int argc, char* argv[])
{
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " level" << endl;
    return -1;
  }
  int level = atoi(argv[1]);
  if (level < 1) {
    cerr << "The level must be greater than 1." << endl;
    return -1;
  }
  random_device rd;
  seed_seq seeds{rd(), rd(), rd(), rd(), rd(), rd()};
  mt19937 random(seeds);

  random_walk(random, level);
  return 0;
}