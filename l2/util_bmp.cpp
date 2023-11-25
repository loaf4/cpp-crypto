#include <iostream>
#include <fstream>
#include "util_bmp.h"

void util_bmp::read(const char* fname) {
    std::ifstream in{fname, std::ios::binary};
    if (in) {
        in.seekg(0, in.end);
        size_t size = in.tellg();
        size -= 54;
        in.seekg(0, in.beg);
        header.resize(54);
        data.resize(size);
        in.read((char*)header.data(), 54);
        in.read((char*)data.data(), size);
        if (in) {
            std::cout << "all bytes read successfully" << std::endl;
        } else {
            std::cerr << "error: only " << in.gcount() << " could be read" << std::endl;
        }
    } else {
        throw std::runtime_error("can't read file");
    }
}

void util_bmp::write(const char* fname) {
    std::ofstream out{fname, std::ios::binary};
    if (out) {
        out.write((const char*)header.data(), header.size());
        out.write((const char*)data.data(), data.size());
    } else {
        throw std::runtime_error("can't write file");
    }
}

