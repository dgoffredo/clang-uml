/**
 * src/class_diagram/model/decorated_element.cc
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

#include "decorated_element.h"

namespace clanguml::class_diagram::model {

bool decorated_element::skip() const
{
    for (auto d : decorators_)
        if (std::dynamic_pointer_cast<decorators::skip>(d))
            return true;

    return false;
}

bool decorated_element::skip_relationship() const
{
    for (auto d : decorators_)
        if (std::dynamic_pointer_cast<decorators::skip_relationship>(d))
            return true;

    return false;
}

std::pair<relationship_t, std::string> decorated_element::relationship() const
{
    for (auto &d : decorators_)
        if (std::dynamic_pointer_cast<decorators::association>(d))
            return {relationship_t::kAssociation,
                std::dynamic_pointer_cast<decorators::relationship>(d)
                    ->multiplicity};
        else if (std::dynamic_pointer_cast<decorators::aggregation>(d))
            return {relationship_t::kAggregation,
                std::dynamic_pointer_cast<decorators::relationship>(d)
                    ->multiplicity};
        else if (std::dynamic_pointer_cast<decorators::composition>(d))
            return {relationship_t::kComposition,
                std::dynamic_pointer_cast<decorators::relationship>(d)
                    ->multiplicity};

    return {relationship_t::kNone, ""};
}

std::string decorated_element::style_spec()
{
    for (auto d : decorators_)
        if (std::dynamic_pointer_cast<decorators::style>(d))
            return std::dynamic_pointer_cast<decorators::style>(d)->spec;

    return "";
}

const std::vector<std::shared_ptr<decorators::decorator>> &
decorated_element::decorators() const
{
    return decorators_;
}

void decorated_element::add_decorators(
    const std::vector<std::shared_ptr<decorators::decorator>> &decorators)
{
    for (auto d : decorators) {
        decorators_.push_back(d);
    }
}
}
