/**
 * src/cx/util.h
 *
 * Copyright (c) 2021 Bartek Kryza <bkryza@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <clang-c/CXCompilationDatabase.h>
#include <clang-c/Index.h>
#include <cppast/cpp_entity.hpp>
#include <cppast/cpp_type.hpp>

#include <string>

namespace clanguml {
namespace cx {
namespace util {

/**
 * @brief Convert CXString to std::string
 *
 * This function creates a new std::string from a CXString
 * and releases the CXString.
 *
 * @param cxs libclang string instance
 *
 * @return std::string instance
 */
std::string to_string(CXString &&cxs);

std::string full_name(
    const std::vector<std::string> &current_ns, const cppast::cpp_entity &e);

std::string full_name(const cppast::cpp_type &t,
    const cppast::cpp_entity_index &idx, bool inside_class);

std::string fully_prefixed(
    const std::vector<std::string> &current_ns, const cppast::cpp_entity &e);

const cppast::cpp_type &unreferenced(const cppast::cpp_type &t);

std::string ns(const cppast::cpp_entity &e);

std::string ns(const cppast::cpp_type &t, const cppast::cpp_entity_index &idx);

bool is_inside_class(const cppast::cpp_entity &e);

} // namespace util
} // namespace cx
} // namespace clanguml
