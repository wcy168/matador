#include "IdentifierSerializerTest.h"

#include "matador/sql/connection.hpp"
#include "matador/sql/query.hpp"

using namespace matador;

template < class IdentifierType, long Size = -1 >
struct identifier_entity
{
  IdentifierType id{};
  std::string name;

  template<class Operator>
  void serialize(Operator &op)
  {
    op.on_primary_key("id", id, Size);
    op.on_attribute("name", name, 255);
  }

};

template<typename IdType, typename SecondIdType, typename... OtherIdTypes>
void IdentifierSerializerTest::test_identifier_result(matador::connection &conn) {
  test_identifier_result<IdType>(conn);
  test_identifier_result<SecondIdType, OtherIdTypes...>(conn);
}

template<typename IdType>
void IdentifierSerializerTest::test_identifier_result(matador::connection &conn) {
  query<identifier_entity<IdType>> q("id_type");

  auto res = q.create().execute(conn);

  identifier_entity<IdType> id { 1, "george" };

  res = q.insert(id).execute(conn);
  res = q.select().execute(conn);

  for (const auto &i : res) {
    UNIT_EXPECT_EQUAL(i->id, 1);
    UNIT_EXPECT_EQUAL(i->name, "george");
  }

  res = q.drop().execute(conn);
}

template<>
void IdentifierSerializerTest::test_identifier_result<std::string>(matador::connection &conn) {
  query<identifier_entity<std::string, 255>> q("id_type");

  auto res = q.create().execute(conn);

  identifier_entity<std::string, 255> id { "id", "george" };

  res = q.insert(id).execute(conn);
  res = q.select().execute(conn);

  for (const auto &i : res) {
    UNIT_EXPECT_EQUAL(i->id, "id");
    UNIT_EXPECT_EQUAL(i->name, "george");
  }

  res = q.drop().execute(conn);
}

template<typename IdType, typename SecondIdType, typename... OtherIdTypes>
void IdentifierSerializerTest::test_identifier_row_result(connection &conn) {
  test_identifier_row_result<IdType>(conn);
  test_identifier_row_result<SecondIdType, OtherIdTypes...>(conn);
}

template<typename IdType>
void IdentifierSerializerTest::test_identifier_row_result(connection &conn) {
  auto q = query<>("id_row_type");

  auto res = q.create({
                      make_typed_id_column<IdType>("id"),
                      make_typed_varchar_column("name", 255)
                      }).execute(conn);

  res = q.insert({"id", "name"}).values({1, "george"}).execute(conn);

  res = q.select({"id", "name"}).from("id_row_type").execute(conn);

  for (const auto &i : res) {
    UNIT_EXPECT_EQUAL(i->template at<IdType>("id"), 1);
    UNIT_EXPECT_EQUAL(i->template at<std::string>("name"), "george");
  }

  res = q.drop().execute(conn);
}

template<>
void IdentifierSerializerTest::test_identifier_row_result<std::string>(connection &conn) {
  auto q = query<>("id_row_type");

  auto res = q.create({
                      make_typed_id_column<std::string>("id"),
                      make_typed_varchar_column("name", 255)
                      }).execute(conn);

  res = q.insert({"id", "name"}).values({"id", "george"}).execute(conn);

  res = q.select({"id", "name"}).from("id_row_type").execute(conn);

  for (const auto &i : res) {
    UNIT_EXPECT_EQUAL(i->at<std::string>("id"), "id");
    UNIT_EXPECT_EQUAL(i->at<std::string>("name"), "george");
  }

  res = q.drop().execute(conn);
}

IdentifierSerializerTest::IdentifierSerializerTest(const std::string &prefix, std::string dns)
: unit_test(prefix + "_identifier_serializer", "identifier serializer test")
, dns_(std::move(dns))
{
  add_test("result", [this] { test_identifier_result_test(); }, "test identifier result binding");
//  add_test("row_result", [this] { test_identifier_row_result_test(); }, "test identifier row result binding");
}

void IdentifierSerializerTest::test_identifier_result_test()
{
  connection conn(dns_);

  conn.connect();

  UNIT_ASSERT_TRUE(conn.is_connected());

  test_identifier_result<short,
                         int,
                         long,
                         long long,
                         unsigned short,
                         unsigned int,
                         unsigned long,
                         unsigned long long,
                         std::string>(conn);

  conn.disconnect();

  UNIT_ASSERT_FALSE(conn.is_connected());
}

void IdentifierSerializerTest::test_identifier_row_result_test()
{
  connection conn(dns_);

  conn.connect();

  UNIT_ASSERT_TRUE(conn.is_connected());

  test_identifier_row_result<short,
                             int,
                             long,
                             long long,
                             unsigned short,
                             unsigned int,
                             unsigned long,
                             unsigned long long,
                             std::string>(conn);

  conn.disconnect();

  UNIT_ASSERT_FALSE(conn.is_connected());
}
