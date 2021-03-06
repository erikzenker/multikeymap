// STL
#include <iostream>
#include <string>
#include <vector>

// MultiKeyMap
#include <multikeymap.hpp>

// HANA
#include <boost/hana.hpp>
namespace hana = boost::hana;

int main()
{

    // Set types for keys and value
    using Country = std::string;
    using City = std::string;
    using Street = std::string;
    using Number = int;
    using Inhabitant = std::string;

    // Create multi key map
    MultiKeyMap<Country, City, Street, Number, Inhabitant> inhabitants;

    // Set inhabitants
    inhabitants("Germany", "Hamburg", "Goetheweg", 5) = "Karl Koch";
    inhabitants("Germany", "Berlin", "Schillerweg", 10) = "Hans Meier";

    // Print inhabitant of Goetheweg 5
    std::cout << inhabitants.at("Germany", "Hamburg", "Goetheweg", 5) << std::endl;

    // Check for inhabitants in Schillerweg 10
    if (inhabitants.test("Germany", "Berlin", "Schillerweg", 10)) {
        std::cout << inhabitants.at("Germany", "Berlin", "Schillerweg", 10) << std::endl;
    }

    // Print all inhabitants of Germany
    std::vector<Inhabitant> inhabitantsOfGermany;
    std::vector<hana::tuple<Country, City, Street, Number>> keys;

    inhabitants.values(inhabitantsOfGermany, keys, "Germany");

    for (std::size_t i = 0; i < inhabitantsOfGermany.size(); ++i) {
      auto country = hana::at(keys[i], hana::size_c<0>);
      auto city = hana::at(keys[i], hana::size_c<1>);
      auto street = hana::at(keys[i], hana::size_c<2>);
      auto number = hana::at(keys[i], hana::size_c<3>);
      std::cout << country << " " << city << " " << street << " " << number << " "
                << inhabitantsOfGermany[i] << std::endl;
    }

    return 0;
}
