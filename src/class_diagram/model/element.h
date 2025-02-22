/**
 * src/class_diagram/model/element.h
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

#include "class_relationship.h"
#include "decorated_element.h"

#include <atomic>
#include <string>
#include <vector>

namespace clanguml::class_diagram::model {

class element : public decorated_element {
public:
    element(const std::vector<std::string> &using_namespaces);

    std::string alias() const;

    void set_name(const std::string &name) { name_ = name; }

    std::string name() const { return name_; }

    void set_namespace(const std::vector<std::string> &ns) { namespace_ = ns; }

    std::vector<std::string> get_namespace() const { return namespace_; }

    virtual std::string full_name(bool relative) const { return name(); }

    void set_using_namespaces(const std::vector<std::string> &un);

    const std::vector<std::string> &using_namespaces() const;

    std::vector<class_relationship> &relationships();

    const std::vector<class_relationship> &relationships() const;

    void add_relationship(class_relationship &&cr);

protected:
    const uint64_t m_id{0};

private:
    std::string name_;
    std::vector<std::string> namespace_;
    std::vector<std::string> using_namespaces_;
    std::vector<class_relationship> relationships_;

    static std::atomic_uint64_t m_nextId;
};
}
