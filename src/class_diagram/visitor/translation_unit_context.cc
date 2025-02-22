/**
 * src/class_diagram/visitor/translation_unit_context.cc
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

#include "translation_unit_context.h"

#include "cx/util.h"

namespace clanguml::class_diagram::visitor {

translation_unit_context::translation_unit_context(
    cppast::cpp_entity_index &idx,
    clanguml::class_diagram::model::diagram &diagram,
    const clanguml::config::class_diagram &config)
    : entity_index_{idx}
    , diagram_{diagram}
    , config_{config}
{
}

bool translation_unit_context::has_type_alias(
    const std::string &full_name) const
{
    bool res = alias_index_.find(full_name) != alias_index_.end();

    LOG_DBG("Alias {} {} found in index", full_name, res ? "" : "not");

    return res;
}

void translation_unit_context::add_type_alias(const std::string &full_name,
    type_safe::object_ref<const cppast::cpp_type> &&ref)
{
    if (!has_type_alias(full_name)) {
        LOG_DBG("Stored type alias: {} -> {} ", full_name,
            cppast::to_string(ref.get()));

        alias_index_.emplace(full_name, std::move(ref));
    }
}

type_safe::object_ref<const cppast::cpp_type>
translation_unit_context::get_type_alias(const std::string &full_name) const
{
    assert(has_type_alias(full_name));

    return alias_index_.at(full_name);
}

type_safe::object_ref<const cppast::cpp_type>
translation_unit_context::get_type_alias_final(const cppast::cpp_type &t) const
{
    const auto type_full_name =
        cx::util::full_name(cppast::remove_cv(t), entity_index_, false);

    if (has_type_alias(type_full_name)) {
        return get_type_alias_final(alias_index_.at(type_full_name).get());
    }

    return type_safe::ref(t);
}

bool translation_unit_context::has_type_alias_template(
    const std::string &full_name) const
{
    bool res =
        alias_template_index_.find(full_name) != alias_template_index_.end();

    LOG_DBG("Alias template {} {} found in index", full_name, res ? "" : "not");

    return res;
}

void translation_unit_context::add_type_alias_template(
    const std::string &full_name,
    type_safe::object_ref<const cppast::cpp_type> &&ref)
{
    if (!has_type_alias_template(full_name)) {
        LOG_DBG("Stored type alias template for: {} ", full_name);

        alias_template_index_.emplace(full_name, std::move(ref));
    }
}

type_safe::object_ref<const cppast::cpp_type>
translation_unit_context::get_type_alias_template(
    const std::string &full_name) const
{
    assert(has_type_alias_template(full_name));

    return alias_template_index_.at(full_name);
}

void translation_unit_context::push_namespace(const std::string &ns)
{
    namespace_.push_back(ns);
}

void translation_unit_context::pop_namespace() { namespace_.pop_back(); }

const std::vector<std::string> &translation_unit_context::get_namespace() const
{
    return namespace_;
}

const cppast::cpp_entity_index &translation_unit_context::entity_index() const
{
    return entity_index_;
}

const clanguml::config::class_diagram &translation_unit_context::config() const
{
    return config_;
}

clanguml::class_diagram::model::diagram &translation_unit_context::diagram()
{
    return diagram_;
}

}
