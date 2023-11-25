#include <vector>
#include <cstdint>
#include <fstream>

#ifndef _UTIL_H_
#define _UTIL_H_

class util {
    private:
        std::vector<uint8_t> data;
    public:
        util(const char* fname) {
            read(fname);
        }
        void read(const char* fname);
        void write(const char* fname);
        std::vector<uint8_t> get_data() const { return data; }
        void set_data(std::vector<uint8_t> data) { this->data = data; }
};

#endif

