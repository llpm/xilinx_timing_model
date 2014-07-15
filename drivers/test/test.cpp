#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <set>

#include <boost/foreach.hpp>

using namespace std;

#include <xdlrc/parser/XDLRCParser.hpp>
#include <xdlrc/model/Device.hpp>
#include <xdlrc/model/Element.hpp>

using namespace bil;

int main(int argc, const char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <xdlrc file>\n", argv[0]);
        return 1;
    }

    set<string> prims;
    for (int i=2; i<argc; i++) {
        prims.insert(argv[i]);
    }

    string xdlrcFileName = argv[1];
    printf("Attempting to parse: %s...\n", xdlrcFileName.c_str());

    ifstream xdlrcFile(xdlrcFileName);
    bil::XDLRCParser parser;
    bil::Device device;

    printf("\tFirst pass...\n");
    parser.parseFirstPass(xdlrcFile, device);
    printf("\tSecond pass...\n");
    xdlrcFile.clear();
    xdlrcFile.seekg(0);
    parser.parseSecondPass();
    printf("Done.\n");

    printf("Read in device information for: %s\n", device.name().c_str());

    printf("Primitive defs: \n");
    auto types = device.primitiveTypes();
    BOOST_FOREACH(auto type, types) {
        printf("\t%s\n", type.name().c_str());
        if (prims.count(type.name()) > 0) {
            const Elements& es = type.elements();
            BOOST_FOREACH(auto& e, es) {
                printf("\t\t%s\n", e.name().c_str());
                const auto& pins = e.pins();
                BOOST_FOREACH(const auto& p, pins) {
                    if (p.isInput())
                        printf("\t\t\t%s input\n", p.externalName().c_str());
                    else
                        printf("\t\t\t%s output\n", p.externalName().c_str());
                }
                const auto& conns = e.conns();
                BOOST_FOREACH(const auto& c, conns) {
                    printf("\t\t\t%s %s.%s\n", c.pin().c_str(),
                           c.dstElement().c_str(), c.dstPin().c_str());
                }
            }
        }
    }

    return 0;
}
