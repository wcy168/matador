//
// Created by sascha on 15.01.20.
//

#include "JsonObjectMapperTest.hpp"
#include "../person.hpp"

#include "matador/object/json_object_mapper.hpp"
#include "../entities.hpp"
#include "../has_many_list.hpp"

using namespace matador;

JsonObjectMapperTest::JsonObjectMapperTest()
  : unit_test("json_object", "Json Object Mapper Test")
{
  add_test("simple", [this] { test_simple(); }, "test simple json object mapper");
  add_test("derived", [this] { test_derived(); }, "test derived json object mapper");
  add_test("has_many", [this] { test_has_many(); }, "test has many json object mapper");
  add_test("array", [this] { test_array(); }, "test array of objects json object mapper");
  add_test("to_json", [this] { test_to_json(); }, "test object to json");
  //add_test("to_string", [this] { test_to_string(); }, "test object to string");
}

void JsonObjectMapperTest::test_simple()
{
  json_object_mapper mapper;

  auto p = mapper.to_object<person>(R"(  { "id":  5, "name": "george", "height": 185 } )");

  UNIT_EXPECT_EQUAL(5UL, p->id());
  UNIT_EXPECT_EQUAL("george", p->name());
  UNIT_EXPECT_EQUAL(185U, p->height());
}

void JsonObjectMapperTest::test_derived()
{
  json_object_mapper mapper;

  auto p = mapper.to_object<citizen>(R"(  { "id":  5, "name": "george", "height": 185, "birthdate": "2001-11-27", "address": { "id": 4, "street": "east-street", "city": "east-city", "citizen": 5 } } )");

  date birthday(27, 11, 2001);
  UNIT_EXPECT_EQUAL(5UL, p->id());
  UNIT_EXPECT_EQUAL("george", p->name());
  UNIT_EXPECT_EQUAL(185U, p->height());
  UNIT_EXPECT_EQUAL(birthday, p->birthdate());

  UNIT_ASSERT_NOT_NULL(p->address_.get());

  object_ptr<address> addr = p->address_;

  UNIT_EXPECT_EQUAL(4UL, addr->id);
  UNIT_EXPECT_EQUAL("east-street", addr->street);
  UNIT_EXPECT_EQUAL("east-city", addr->city);
}

void JsonObjectMapperTest::test_has_many()
{
  json_object_mapper mapper;

  auto p = mapper.to_object<hasmanylist::owner>(R"(  {
"id":  5,
"name": "george",
"owner_item": [{"id": 13, "name": "strawberry"}, {"id": 17, "name": "banana"}   ]
} )");

  UNIT_EXPECT_EQUAL(5UL, p->id);
  UNIT_EXPECT_EQUAL("george", p->name);
  UNIT_ASSERT_EQUAL(2UL, p->items.size());

  const auto &i = p->items.front();
  UNIT_ASSERT_EQUAL(13UL, i->id);
  UNIT_ASSERT_EQUAL("strawberry", i->name);
}

void JsonObjectMapperTest::test_array()
{
  json_object_mapper mapper;

  auto array = mapper.to_objects<person>(R"(  [{ "id":  5, "name": "george", "height": 185 },{ "id":  6, "name": "jane", "height": 190 }] )");

  UNIT_ASSERT_EQUAL(2UL, array.size());
  auto it = array.begin();
  UNIT_ASSERT_EQUAL(5UL, (*it)->id());
  UNIT_ASSERT_EQUAL("george", (*it)->name());
  UNIT_ASSERT_EQUAL(185U, (*it)->height());
  ++it;
  UNIT_ASSERT_EQUAL(6UL, (*it)->id());
  UNIT_ASSERT_EQUAL("jane", (*it)->name());
  UNIT_ASSERT_EQUAL(190U, (*it)->height());

}

void JsonObjectMapperTest::test_to_json()
{
  object_store store;

  store.attach<person>("person");

  auto bd = date(13, 2, 1999);

  auto george = store.insert(new person(1, "george", bd, 185));

  json_object_mapper mapper;

  auto j = mapper.to_json(george);

  json result = {
    { "id", 1},
    { "name", "george"},
    { "birthdate", matador::to_string(bd) },
    { "height", 185 }
  };

  auto str = to_string(result);

  UNIT_ASSERT_EQUAL(result, j);
  UNIT_ASSERT_EQUAL(1, j["id"].as<long>());

  object_view<person> view(store);

  j = mapper.to_json(view);

  auto view_result = json::array();
  view_result.push_back(result);

  auto astr = to_string(view_result);

  UNIT_ASSERT_EQUAL(view_result, j);
}

void JsonObjectMapperTest::test_to_string()
{
  object_store store;
  store.attach<address>("address");
  store.attach_abstract<person>("person");
  store.attach<citizen, person>("citizen");

  auto bd = date(13, 2, 1999);

  auto home = store.insert(new address("mystreet 4", "hometown"));
  auto george = store.insert(new citizen("george", bd, 179));
  george.modify()->address_ = home;

  json_object_mapper mapper;

  auto str = mapper.to_string(home);

  UNIT_ASSERT_EQUAL("", str);
}
