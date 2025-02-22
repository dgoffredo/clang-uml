/**
 * src/cx/cursor.cc
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

#include "cx/cursor.h"

namespace clanguml::cx {

cursor::cursor()
    : m_cursor{clang_getNullCursor()}
{
}

cursor::cursor(CXCursor &&c)
    : m_cursor{std::move(c)}
{
}

cursor::cursor(const CXCursor &c)
    : m_cursor{c}
{
}

cursor::cursor(const cursor &c)
    : m_cursor{c.get()}
{
}

bool cursor::operator==(const cursor &b) const
{
    return clang_equalCursors(m_cursor, b.get());
}

cx::type cursor::type() const { return {clang_getCursorType(m_cursor)}; }

std::string cursor::display_name() const
{
    return to_string(clang_getCursorDisplayName(m_cursor));
}

std::string cursor::spelling() const
{
    return to_string(clang_getCursorSpelling(m_cursor));
}

bool cursor::is_void() const
{
    // Why do I have to do this like this?
    return spelling() == "void";
}

/**
 * @brief Return fully qualified cursor spelling
 *
 * This method generates a fully qualified name for the cursor by
 * traversing the namespaces upwards.
 *
 * TODO: Add caching of this value.
 *
 * @return Fully qualified cursor spelling
 */
std::string cursor::fully_qualified() const
{
    std::list<std::string> res;
    cursor iterator{m_cursor};
    if (iterator.spelling().empty())
        return {};

    int limit = 100;
    while (iterator.kind() != CXCursor_TranslationUnit) {
        auto name = iterator.spelling();
        if (!name.empty())
            res.push_front(iterator.spelling());
        iterator = iterator.semantic_parent();
        if (limit-- == 0)
            throw std::runtime_error(fmt::format(
                "Generating fully qualified name for '{}' failed at: '{}'",
                spelling(), fmt::join(res, "::")));
    }

    return fmt::format("{}", fmt::join(res, "::"));
}

cursor cursor::referenced() const
{
    return cx::cursor{clang_getCursorReferenced(m_cursor)};
}

cursor cursor::semantic_parent() const
{
    return {clang_getCursorSemanticParent(m_cursor)};
}

cursor cursor::lexical_parent() const
{
    return {clang_getCursorLexicalParent(m_cursor)};
}

CXCursorKind cursor::kind() const { return m_cursor.kind; }

std::string cursor::kind_spelling() const
{
    return to_string(clang_getCursorKindSpelling(m_cursor.kind));
}

cursor cursor::definition() const
{
    return clang_getCursorDefinition(m_cursor);
}

bool cursor::is_definition() const
{
    return clang_isCursorDefinition(m_cursor);
}

bool cursor::is_declaration() const { return clang_isDeclaration(kind()); }

bool cursor::is_forward_declaration() const
{
    auto definition = clang_getCursorDefinition(m_cursor);

    if (clang_equalCursors(definition, clang_getNullCursor()))
        return true;

    return !clang_equalCursors(m_cursor, definition);
}

bool cursor::is_invalid_declaration() const
{
    return clang_isInvalidDeclaration(m_cursor);
}

CXSourceLocation cursor::location() const
{
    return clang_getCursorLocation(m_cursor);
}

bool cursor::is_reference() const { return clang_isReference(kind()); }

bool cursor::is_expression() const { return clang_isExpression(kind()); }

bool cursor::is_statement() const { return clang_isStatement(kind()); }

bool cursor::is_namespace() const { return kind() == CXCursor_Namespace; }

bool cursor::is_attribute() const { return clang_isAttribute(kind()); }

bool cursor::has_attrs() const { return clang_Cursor_hasAttrs(m_cursor); }

bool cursor::is_invalid() const { return clang_isInvalid(kind()); }

bool cursor::is_translation_unit() const
{
    return clang_isTranslationUnit(kind());
}

bool cursor::is_preprocessing() const { return clang_isPreprocessing(kind()); }

bool cursor::is_method_virtual() const
{
    return clang_CXXMethod_isVirtual(m_cursor);
}

bool cursor::is_static() const
{
    return clang_Cursor_getStorageClass(m_cursor) == CX_SC_Static;
}

bool cursor::is_method_static() const
{
    return clang_CXXMethod_isStatic(m_cursor);
}

bool cursor::is_method_const() const
{
    return clang_CXXMethod_isConst(m_cursor);
}

bool cursor::is_method_pure_virtual() const
{
    return clang_CXXMethod_isPureVirtual(m_cursor);
}

bool cursor::is_method_defaulted() const
{
    return clang_CXXMethod_isDefaulted(m_cursor);
}

bool cursor::is_method_parameter() const { return kind() == CXCursor_ParmDecl; }

CXVisibilityKind cursor::visibitity() const
{
    return clang_getCursorVisibility(m_cursor);
}

CXAvailabilityKind cursor::availability() const
{
    return clang_getCursorAvailability(m_cursor);
}

CX_CXXAccessSpecifier cursor::cxxaccess_specifier() const
{
    return clang_getCXXAccessSpecifier(m_cursor);
}

cx::type cursor::underlying_type() const
{
    return clang_getTypedefDeclUnderlyingType(m_cursor);
}

int cursor::template_argument_count() const
{
    return clang_Cursor_getNumTemplateArguments(m_cursor);
}

CXTemplateArgumentKind cursor::template_argument_kind(unsigned i) const
{
    return clang_Cursor_getTemplateArgumentKind(m_cursor, i);
}

cx::type cursor::template_argument_type(unsigned i) const
{
    return clang_Cursor_getTemplateArgumentType(m_cursor, i);
}

long long cursor::template_argument_value(unsigned i) const
{
    return clang_Cursor_getTemplateArgumentValue(m_cursor, i);
}

cursor cursor::specialized_cursor_template() const
{
    return clang_getSpecializedCursorTemplate(m_cursor);
}

CXTranslationUnit cursor::translation_unit() const
{
    return clang_Cursor_getTranslationUnit(m_cursor);
}

bool cursor::is_template_parameter_variadic() const
{
    const auto &tokens = tokenize();
    return tokens.size() > 2 && tokens[1] == "...";
}

std::string cursor::usr() const
{
    return to_string(clang_getCursorUSR(m_cursor));
}

CXSourceRange cursor::extent() const { return clang_getCursorExtent(m_cursor); }

std::vector<std::string> cursor::tokenize() const
{
    auto range = extent();
    std::vector<std::string> res;
    CXToken *toks;
    unsigned toks_count{0};
    auto tu = translation_unit();

    clang_tokenize(tu, range, &toks, &toks_count);

    for (int i = 0; i < toks_count; i++) {
        res.push_back(to_string(clang_getTokenSpelling(tu, toks[i])));
    }

    return res;
}

std::string cursor::default_value() const
{
    assert(is_method_parameter());

    auto toks = tokenize();
    std::string res;
    auto it = std::find(toks.begin(), toks.end(), "=");
    if (it != toks.end()) {
        res = fmt::format("{}", fmt::join(it + 1, toks.end(), ""));
    }

    return res;
}

std::vector<std::string> cursor::tokenize_template_parameters() const
{
    auto toks = tokenize();
    std::vector<std::string> res;
    bool inside_template{false};
    bool is_namespace{false};

    for (int i = 0; i < toks.size(); i++) {
        // libclang returns ">..>>" in template as a single token...
        auto t = toks[i];
        if (std::all_of(
                t.begin(), t.end(), [](const char &c) { return c == '>'; })) {
            toks[i] = ">";
            for (int j = 0; j < t.size() - 1; j++)
                toks.insert(toks.begin() + i, ">");
        }
    }
    auto template_start = std::find(toks.begin(), toks.end(), "<");
    auto template_end = std::find(toks.rbegin(), toks.rend(), ">");

    decltype(res) template_contents(
        template_start + 1, template_end.base() - 1);

    return clanguml::util::split(
        fmt::format("{}", fmt::join(template_contents, "")), ",");
}

const CXCursor &cursor::get() const { return m_cursor; }

}
