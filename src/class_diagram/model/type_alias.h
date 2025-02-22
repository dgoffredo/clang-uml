/**
 * src/class_diagram/model/type_alias.h
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

#include <string>

namespace clanguml::class_diagram::model {

class type_alias {
public:
    void set_alias(const std::string &alias);
    std::string alias() const;

    void set_underlying_type(const std::string &type);
    std::string underlying_type() const;

private:
    std::string alias_;
    std::string underlying_type_;
};

}
