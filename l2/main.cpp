#include <iostream>
#include "util_bmp.h"
#include "safer.h"

int main(int argc, char* argv[]) {

    util_bmp file1("test.bmp");

    Safer s {file1.get_data()};
    s.encryptCFB();
    file1.set_data(s.get_data());
    file1.write("qqq.bmp");

    s.decryptCFB();
    file1.set_data(s.get_data());
    file1.write("www.bmp");

    return 0;
}

