#include <iostream>
#include <fstream>
#include "util.h"

void util::read(const char* fname) {
    std::ifstream in{fname, std::ios::binary};
    if (in) {
        in.seekg(0, in.end);
        size_t size = in.tellg();
        in.seekg(0, in.beg);
        data.resize(size);
        in.read((char*)data.data(), size);
    } else {
        throw std::runtime_error("can't read file");
    }
}

void util::write(const char* fname) {
    std::ofstream out{fname, std::ios::binary};
    if (out) {
        out.write((const char*)data.data(), data.size());
    } else {
        throw std::runtime_error("can't write file");
    }
}
