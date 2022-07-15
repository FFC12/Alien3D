#ifndef BASE_HPP
#define BASE_HPP
#include <cassert>
#include <string>
#include <iostream>

#define ALIEN_ASSERT(x) assert(x)
#define ALIEN_ASSERT2(x) ALIEN_ERROR(x); abort()
#define ALIEN_INFO(x) std::cout << x << std::endl
#define ALIEN_WARN(x) std::cout << x << std::endl
#define ALIEN_ERROR(x) std::cerr << x << std::endl

// Type aliases
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t uchar;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float  f32;
typedef double f64;


#define RESOURCE_PATH(path) std::string("../res/") + std::string(path)

#endif