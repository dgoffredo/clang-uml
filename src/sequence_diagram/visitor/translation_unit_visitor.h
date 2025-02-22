/**
 * src/sequence_diagram/visitor/translation_unit_visitor.h
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

#include "config/config.h"
#include "cx/cursor.h"
#include "sequence_diagram/model/diagram.h"
#include "sequence_diagram/visitor/translation_unit_context.h"

#include <cppast/cpp_function.hpp>

namespace clanguml::sequence_diagram::visitor {

class translation_unit_visitor {
public:
    translation_unit_visitor(cppast::cpp_entity_index &idx,
        clanguml::sequence_diagram::model::diagram &diagram,
        const clanguml::config::sequence_diagram &config);

    void operator()(const cppast::cpp_entity &file);

private:
    void process_activities(const cppast::cpp_function &e);

    // ctx allows to track current visitor context, e.g. current namespace
    translation_unit_context ctx;
};
}
