#include <multikeymap.hpp>

#include <boost/filesystem/fstream.hpp>
#include <boost/test/unit_test.hpp>

struct MultiKeyMapOneKeyTests {
  using Key = int;
  using Value = int;

  etl::MultiKeyMap<Value, Key> multiKeyMap;
};

BOOST_FIXTURE_TEST_CASE(should_compile, MultiKeyMapOneKeyTests) {}

BOOST_FIXTURE_TEST_CASE(should_throw, MultiKeyMapOneKeyTests) {
  BOOST_REQUIRE_THROW(multiKeyMap.at(0), std::out_of_range);
}

BOOST_FIXTURE_TEST_CASE(should_get_and_set_value_with_one_key,
                        MultiKeyMapOneKeyTests) {
  multiKeyMap(1) = 1;
  BOOST_REQUIRE_EQUAL(multiKeyMap.at(1), 1);
}

BOOST_FIXTURE_TEST_CASE(should_test_negative, MultiKeyMapOneKeyTests) {
  BOOST_REQUIRE_EQUAL(multiKeyMap.test(1), false);
}

BOOST_FIXTURE_TEST_CASE(should_test_positive, MultiKeyMapOneKeyTests) {
  multiKeyMap(1) = 1;
  BOOST_REQUIRE_EQUAL(multiKeyMap.test(1), true);
}

BOOST_FIXTURE_TEST_CASE(should_return_all_values, MultiKeyMapOneKeyTests) {
  std::vector<Value> values;
  std::vector<hana::tuple<Key>> keys;

  multiKeyMap(1) = 1;
  multiKeyMap.values(values, keys);

  BOOST_REQUIRE_EQUAL(values.at(0), 1);
}

BOOST_FIXTURE_TEST_CASE(should_return_all_values_of_subtree,
                        MultiKeyMapOneKeyTests) {
  std::vector<Value> values;
  std::vector<hana::tuple<Key>> keys;

  multiKeyMap(1) = 1;
  multiKeyMap.values(values, keys, 1);

  BOOST_REQUIRE_EQUAL(values.at(0), 1);
  BOOST_REQUIRE_EQUAL(1, keys.size());
}

struct MultiKeyMapTwoKeyTests {
  using Key1 = int;
  using Key2 = int;
  using Value = int;

  etl::MultiKeyMap<Value, Key1, Key2> multiKeyMap;
};

BOOST_FIXTURE_TEST_CASE(should_return_all_values_of_two_keys_map,
                        MultiKeyMapTwoKeyTests) {
  std::vector<Value> values;
  std::vector<hana::tuple<Key1, Key2>> keys;

  multiKeyMap(0, 0) = 0;
  multiKeyMap(0, 1) = 1;
  multiKeyMap(1, 2) = 2;
  multiKeyMap.values(values, keys);

  BOOST_REQUIRE_EQUAL(values.at(0), 0);
  BOOST_REQUIRE_EQUAL(values.at(1), 1);
  BOOST_REQUIRE_EQUAL(values.at(2), 2);
  BOOST_REQUIRE_EQUAL(3, keys.size());
}

BOOST_FIXTURE_TEST_CASE(should_return_all_values_of_subtree_of_two_keys_map,
                        MultiKeyMapTwoKeyTests) {
  std::vector<Value> values;
  std::vector<hana::tuple<Key1, Key2>> keys;

  multiKeyMap(0, 0) = 0;
  multiKeyMap(0, 1) = 1;
  multiKeyMap(1, 2) = 2;
  multiKeyMap.values(values, keys, 0);

  BOOST_REQUIRE_EQUAL(values.at(0), 0);
  BOOST_REQUIRE_EQUAL(values.at(1), 1);
  BOOST_REQUIRE_EQUAL(2, keys.size());
}