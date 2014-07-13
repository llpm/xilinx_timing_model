#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include <xdlrc/parser/XDLRCParser.hpp>
#include <xdlrc/model/Device.hpp>

int main(int argc, const char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <xdlrc file>\n", argv[0]);
        return 1;
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

    return 0;
}
