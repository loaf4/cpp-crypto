#include <cstdint>
#include <vector>
#include <fstream>

#ifndef _UTIL_BMP_H_
#define _UTIL_BMP_H_

class util_bmp {
    private:
        std::vector<uint8_t> header;
        std::vector<uint8_t> data;

    public:
        util_bmp(const char* fname) {
            read(fname);
        }
        void read(const char* fname);
        void write(const char* fname);
        std::vector<uint8_t> get_data() const { return data; }
        void set_data(std::vector<uint8_t> data) { 
            this->data = data;
        }
};

#endif

