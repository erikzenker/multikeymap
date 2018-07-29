project(multikeymapUnitTest)

add_executable(multikeymap_unit_test main.cpp multikeymapTests.cpp)


###############################################################################
# Boost
###############################################################################
find_package(
        Boost 1.54.0
        COMPONENTS system filesystem unit_test_framework
        REQUIRED
)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

target_link_libraries(
  multikeymap_unit_test
    ${Boost_LIBRARIES}
)

add_test(NAME multikeymap_unit_test COMMAND multikeymap_unit_test)
