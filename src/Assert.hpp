#ifndef ASSERT_HPP
#define ASSERT_HPP

#include <cassert>

#define ASSERT(true_cond, msg) assert(!(true_cond) && msg)

#endif // ASSERT_HPP
