/*****************************************************************************
 *
 * demo program - part of CLIPP (command line interfaces for modern C++)
 *
 * released under MIT license
 *
 * (c) 2017-2018 André Müller; foss@andremueller-online.de
 *
 *****************************************************************************/

#include "pch.h"
#include "example-def.h" // EXAMPLE_MAIN

#include <iostream>
#include "clipp.h"


int EXAMPLE_MAIN(int argc, char* argv[])
{
    using namespace clipp;
    using std::cout;

    bool rec = false, utf16 = false;
    std::string infile = "";
    std::string fmt = "csv";

    auto cli = (
        value("input file", infile),
        option("-r", "--recursive").set(rec).doc("convert files recursively"),
        option("-o") & value("output format", fmt),
        option("-utf16").set(utf16).doc("use UTF-16 encoding")
    );

    if(!parse(argc, argv, cli)) {
        cout << make_man_page(cli, "convert");
    }
    else {
        cout << "input file: " << infile << '\n'
             << "format:     " << fmt << '\n'
             << "recursive:  " << (rec ? "yes\n" : "no\n")
             << "UTF-16:     " << (utf16 ? "yes\n" : "no\n");
    }
    return 0;
}
