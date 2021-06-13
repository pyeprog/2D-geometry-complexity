#include <iostream>
#include <string>
#include "complexity.hpp"
#include "CLI/CLI.hpp"
#include "geos.h"


int main(int argc, char **argv) {
    CLI::App app{"calculate geometry complexity"};

    std::string wkt;
    app.add_option<std::string>("--wkt", wkt, "the wkt string of polygon or linestring");

    size_t j = 3;
    app.add_option("-j,--resolution", j, "the resolution", true);

    CLI11_PARSE(app, argc, argv);

    if (wkt.empty()) {
        std::cerr << "please specify wkt" << std::endl;
        return 1;
    }

    geos::io::WKTReader reader;
    auto pGeom = reader.read(wkt);

    geom_complex::Complexity complexity{std::move(pGeom), j};
    std::cout << complexity.calc() << std::endl;

    return 0;
}
