#pragma once

// STL
#include <map>                /* std::map */

// HANA
#include <boost/hana.hpp>

namespace hana = boost::hana;

namespace etl {

    namespace detail {

        /**
         * @brief Type definition for map of maps.
         *
         */
        template<typename... T_Tail>
        struct MapOfMaps;

        template<typename T_Key, typename T_Value>
        struct MapOfMaps<T_Key, T_Value> {
            using type = std::map<T_Key, T_Value>;
        };

        template<typename T_Key, typename... T_Tail>
        struct MapOfMaps<T_Key, T_Tail...> {
            using type = typename std::map<T_Key, typename MapOfMaps<T_Tail...>::type>;
        };


        /**
         * @brief Collects all values of a map subtree into the values container and its
         *        corresponding keys into the key container
         *
         */
        template<int KeysLeft>
        struct SubTreeValues {

            template<typename T_Map, typename T_ValuesCT, typename T_KeysCT, typename T_Tuple>
            void operator()(T_Map &map, T_ValuesCT &values, T_KeysCT &keys, T_Tuple &tuple) {
                auto begin = map.begin();
                auto end = map.end();
                for (auto it = begin; it != end; ++it) {
                    auto nextTuple = hana::concat(tuple, hana::make_tuple(it->first));
                    SubTreeValues<KeysLeft - 1>()(it->second, values, keys, nextTuple);
                }

            }

            template<typename T_Map, typename T_ValuesCT, typename T_Keys>
            void operator()(T_Map &map, T_ValuesCT &values, T_Keys &keys) {
                auto begin = map.begin();
                auto end = map.end();
                for (auto it = begin; it != end; ++it) {
                    auto t = hana::make_tuple(it->first);
                    SubTreeValues<KeysLeft - 1>()(it->second, values, keys, t);
                }

            }

        };

        template<>
        struct SubTreeValues<0> {

            template<typename T_Value, typename T_ValuesCT, typename T_KeysCT, typename T_Tuple>
            void operator()(T_Value &value, T_ValuesCT &values, T_KeysCT &keys, T_Tuple &tuple) {
                keys.push_back(tuple);
                values.push_back(value);
            }

        };


        /**
         * @brief Traverse through a map of map of ... and
         *        return the value of the last key.
         *
         */
        auto at(
            [](auto &map, const auto &key)
                -> auto & {
                return map[key];
            }
        );


        template<typename T_Map, typename T_Keys>
        auto &traverse(T_Map &map, T_Keys const &keys) {
            return hana::fold_left(keys, map, at);
        }


    } // namespace detail

    /**
     * @brief A map with multiple keys, implemented by cascading several
     *        std::maps.
     *
     * The MultiKeyMap has the goal to provide the look and feel of
     * a usual std::map by the at and () methods. Furthermore, it
     * provides the possibility to retrieve the values of subtrees
     * by specifying not all keys by the values method.
     *
     */
    template<typename T_Value, typename... T_Keys>
    class MultiKeyMap {

        using Value = T_Value;
        using LastKey = decltype(hana::back(hana::tuple<T_Keys ...>()));

    public:

        MultiKeyMap() {

        }


        /**********************************************************************
         * Iterator
         **********************************************************************/
        class iterator {
            typename std::map<LastKey, Value>::iterator current;
        };

        void begin(){
        }

        void end(){
        }

        /**********************************************************************
         * Element accesses
         **********************************************************************/
        T_Value &operator()(const T_Keys... keys) {
            return detail::traverse(multiKeyMap, hana::make_tuple(keys...));

        }

        T_Value& at(const T_Keys... keys) {
            return atImpl(hana::make_tuple(keys...));
        }


        /**********************************************************************
         * Modifiers
         **********************************************************************/
        bool erase(const T_Keys ...keys) {
            return eraseImpl(hana::make_tuple(keys...));
        }


        /**********************************************************************
         * Operations
         **********************************************************************/
        bool test(const T_Keys ...keys) {
            return testImpl(hana::make_tuple(keys...));
        }

        template<typename T_ValuesCT, typename T_KeysCT>
        void values(T_Value &values, T_KeysCT &keys) {
            valuesImpl(values, keys);
        }

        template<typename T_ValuesCT, typename T_KeysCT, typename ...T_Sub_Keys>
        void values(T_ValuesCT &values, T_KeysCT &keys, const T_Sub_Keys... subKeys) {
            valuesImpl(values, keys, subKeys...);
        }

    private:

        typename detail::MapOfMaps<T_Keys..., T_Value>::type multiKeyMap;


        /***************************************************************************
         * at
         ***************************************************************************/
        template<typename T_KeysTpl>
        T_Value &atImpl(const T_KeysTpl keysTuple) {
            auto firstKeysSize = hana::int_c<hana::minus(hana::int_c < hana::size(keysTuple) > , 1)>;
            auto firstKeys = hana::take_front_c<firstKeysSize>(keysTuple);
            auto lastKey = hana::back(keysTuple);

            return detail::traverse(multiKeyMap, firstKeys).at(lastKey);
        }


        /***************************************************************************
         * test
         ***************************************************************************/
        template<typename T_KeysTpl>
        bool testImpl(const T_KeysTpl keysTuple) {
            auto firstKeysSize = hana::int_c<hana::minus(hana::int_c < hana::size(keysTuple) > , 1)>;
            auto firstKeys = hana::take_front_c<firstKeysSize>(keysTuple);
            auto lastKey = hana::back(keysTuple);

            auto &subMap = detail::traverse(multiKeyMap, firstKeys);
            auto it = subMap.find(lastKey);
            auto end = subMap.end();

            return (it == end) ? false : true;

        }


        /***************************************************************************
         * erase
         ***************************************************************************/
        template<typename T_KeysTpl>
        bool eraseImpl(const T_KeysTpl keysTuple) {
            auto firstKeysSize = hana::int_c<hana::minus(hana::int_c < hana::size(keysTuple) > , 1)>;
            auto firstKeys = hana::take_front_c<firstKeysSize>(keysTuple);
            auto lastKey = hana::back(keysTuple);


            auto &lastMap = detail::traverse(multiKeyMap, firstKeys);
            auto it = lastMap.find(lastKey);
            auto end = lastMap.end();

            if (it == end) return false;
            else {
                lastMap.erase(it);
                return true;
            }

        }


        /***************************************************************************
         * values
         ***************************************************************************/
        template<typename T_ValuesCT, typename T_KeysCT>
        void valuesImpl(T_Value &values, T_KeysCT &keys) {
            constexpr size_t keysTupleSize = std::tuple_size<std::tuple<T_Keys...>>::value;
            detail::SubTreeValues<keysTupleSize>()(multiKeyMap, values, keys);
        }

        template<typename T_ValuesCT, typename T_KeysCT, typename ...T_Sub_Keys>
        void valuesImpl(T_ValuesCT &values, T_KeysCT &keys, const T_Sub_Keys... subKeys) {
            constexpr size_t subKeysTupleSize = std::tuple_size<std::tuple<T_Sub_Keys...>>::value;
            constexpr size_t keysTupleSize = std::tuple_size<std::tuple<T_Keys...>>::value;
            constexpr size_t subTreeSize = keysTupleSize - subKeysTupleSize;
            auto tuple = hana::make_tuple(subKeys...);
            detail::SubTreeValues<subTreeSize>()(detail::traverse(multiKeyMap, tuple), values, keys, tuple);

        }

    };

} // namespace etl