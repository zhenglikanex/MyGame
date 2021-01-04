#pragma once

#include "nlohmann/json.hpp"
using nlohmann::json;

#if (defined(__cplusplus) && __cplusplus >= 201703L) || (defined(_HAS_CXX17) && _HAS_CXX17 == 1) // fix for issue #464
#define JSON_HAS_CPP_17
#define JSON_HAS_CPP_14
#elif (defined(__cplusplus) && __cplusplus >= 201402L) || (defined(_HAS_CXX14) && _HAS_CXX14 == 1)
#define JSON_HAS_CPP_14
#endif

#if defined(JSON_HAS_CPP_17)
#include <string_view>
#endif
