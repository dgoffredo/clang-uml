/**
 * tests/t20002/test_case.cc
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

TEST_CASE("t20002", "[test-case][sequence]")
{
    auto [config, db] = load_config("t20002");

    auto diagram = config.diagrams["t20002_sequence"];

    REQUIRE(diagram->include.namespaces.size() == 1);
    REQUIRE_THAT(diagram->include.namespaces,
        VectorContains(std::string{"clanguml::t20002"}));

    REQUIRE(diagram->name == "t20002_sequence");

    auto model = generate_sequence_diagram(db, diagram);

    REQUIRE(model.name == "t20002_sequence");

    auto puml = generate_sequence_puml(diagram, model);

    REQUIRE_THAT(puml, StartsWith("@startuml"));
    REQUIRE_THAT(puml, EndsWith("@enduml\n"));

    REQUIRE_THAT(puml, HasFunctionCall("m1", "m2"));
    REQUIRE_THAT(puml, HasFunctionCall("m2", "m3"));
    REQUIRE_THAT(puml, HasFunctionCall("m3", "m4"));

    save_puml(
        "./" + config.output_directory + "/" + diagram->name + ".puml", puml);
}
