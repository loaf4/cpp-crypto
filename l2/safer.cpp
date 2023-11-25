#include <iostream>
#include <cmath>
#include <ctime>
#include "safer.h"

Safer::Safer(std::vector<uint8_t> data) {
    this->data = data;
    generate_keys();
}

std::vector<uint8_t> Safer::encrypt_block(std::vector<uint8_t> text) {
    std::vector<uint8_t>& cur_key {keys[0]};
    for (size_t i {0}; i < 8; ++i) {
        cur_key = keys[i * 2];
        for (size_t j {0}; j < 16; ++j) {
            text[j] = (groups[j] == 1) ? key_sum_mod2(cur_key[j], text[j])
                : key_sum_mod256(cur_key[j], text[j]);
        }
        for (size_t j {0}; j < 16; ++j) {
            text[j] = (groups[j] == 1) ? funcE(text[j]) 
                : funcL(text[j]);
        }
        cur_key = keys[i * 2 + 1];
        for (size_t j {0}; j < 16; ++j) {
            text[j] = (groups[j] == 1) ? key_sum_mod256(cur_key[j], text[j])
                : key_sum_mod2(cur_key[j], text[j]);
        }
        pht(text, 0);
    }
    cur_key = keys[16];
    for (size_t j {0}; j < 16; ++j) {
        text[j] = (groups[j] == 1) ? key_sum_mod2(cur_key[j], text[j]) 
            : key_sum_mod256(cur_key[j], text[j]);
    }
    return text;
}

std::vector<uint8_t> Safer::decrypt_block(std::vector<uint8_t> text) {
    std::vector<uint8_t>& cur_key {keys[16]};
    for (size_t j {0}; j < 16; ++j) {
        text[j] = (groups[j] == 1) ? key_sum_mod2(cur_key[j], text[j]) 
            : key_sub_mod256(cur_key[j], text[j]);
    }
    for (int i {7}; i >= 0; --i) {
        ipht(text, 0);
        cur_key = keys[i * 2 + 1];
        for (size_t j {0}; j < 16; ++j) {
            text[j] = (groups[j] == 1) ? key_sub_mod256(cur_key[j], text[j])
                : key_sum_mod2(cur_key[j], text[j]);
        }
        cur_key = keys[i * 2];
        for (size_t j {0}; j < 16; ++j) {
            text[j] = (groups[j] == 1) ? funcL(text[j]) 
                : funcE(text[j]);
        }
        for (size_t j {0}; j < 16; ++j) {
            text[j] = (groups[j] == 1) ? key_sum_mod2(cur_key[j], text[j])
                : key_sub_mod256(cur_key[j], text[j]);
        }
    }
    return text;
}

void Safer::encrypt() {
    for (size_t i {0}; i + 16 < data.size(); i += 16) {
        auto tmp = encrypt_block(std::vector<uint8_t> {data.begin() + i, data.begin() + i + 16});
        for (size_t j {0}; j < 16; ++j) {
            data[i + j] = tmp[j];
        }
    }
}

void Safer::decrypt() {
    for (size_t i {0}; i + 16 < data.size(); i += 16) {
        auto tmp = decrypt_block(std::vector<uint8_t> {data.begin() + i, data.begin() + i + 16});
        for (size_t j {0}; j < 16; ++j) {
            data[i + j] = tmp[j];
        }
    }
}

void Safer::encryptCFB() {
    generate_iv();
    std::vector<uint8_t> tmp;
    for (size_t i = 0; i + 16 < data.size(); i += 16) {
        tmp = encrypt_block(init_vector);
        for (size_t k = 0; k < 16; ++k) {
            data[i + k] = key_sum_mod2(data[i + k], tmp[k]);
        }
        for (size_t k = 0; k < 16; ++k) {
            init_vector[k] = data[i + k];
        }
    }
}

void Safer::decryptCFB() {
    std::vector<uint8_t> tmp;
    for (size_t i = 0; i + 16 < data.size(); i += 16) {
        tmp = encrypt_block(decrypt_init_vector);
        decrypt_init_vector = std::vector<uint8_t> {data.begin() + i, data.begin() + i + 16};
        for (size_t k = 0; k < 16; ++k) {
            data[i + k] = key_sum_mod2(tmp[k], data[i + k]);
        }
    }
}

void Safer::generate_keys() {
    srand(time(nullptr));
    for (size_t i = 0; i < 17; ++i) {
        keys[i].resize(16);
    }
    for (size_t i = 0; i < 16; ++i) {
        keys[0][i] = (rand() % 256);
    }
    std::vector<uint8_t> ex_key(17);
    for (size_t i = 0; i < 16; ++i) {
        ex_key[i] = keys[0][i];
    }
    uint8_t ex_byte {keys[0][0]};
    for (size_t i = 1; i < 16; ++i) {
        ex_byte = key_sum_mod2(ex_byte, ex_key[i]);
    }
    ex_key[16] = ex_byte;
    for (int i = 1; i < 17; ++i) {
        for (int j = 0; j < 17; ++j) {
            ex_key[j] <<= 3;
        }
        int j;
        for (j = i; j < 17; ++j) {
            keys[i][j - i] = (matrix_b[i - 1][j - 1] + ex_key[j]) % 256;
        }
        int k {j - i};
        for (j = 0; j < i - 1; ++j) {
            keys[i][k++] = (ex_key[j] + matrix_b[i - 1][j]) % 256;
        }
    }
}

void Safer::generate_iv() {
    init_vector.resize(16);
    for (size_t i = 0; i < 16; ++i) {
        init_vector[i] = (rand() % 256);
    }
    decrypt_init_vector = init_vector;
}

void Safer::pht(std::vector<uint8_t>& text, size_t offset) {
    std::vector<uint8_t> tmp;
    uint8_t item;
    for (size_t i = 0; i < 16; ++i) {
        item = 0;
        for (size_t j = 0; j < 16; ++j) {
            item = key_sum_mod256(encrypt_matrix_m[i][j] * text[j + offset], item);
        }
        tmp.push_back(item);
    }
    for (size_t i = 0; i < 16; ++i) {
        text[i + offset] = tmp[i];
    }
}

void Safer::ipht(std::vector<uint8_t>& text, size_t offset) {
    std::vector<uint8_t> tmp;
    uint8_t item;
    for (size_t i = 0; i < 16; ++i) {
        item = 0;
        for (size_t j = 0; j < 16; ++j) {
            item = key_sum_mod256(decrypt_matrix_m[i][j] * text[j + offset], item);
        }
        tmp.push_back(item);
    }
    for (size_t i = 0; i < 16; ++i) {
        text[i + offset] = tmp[i];
    }
}

int Safer::log45(int a) {
    return log2(a) / log2(45);
}

uint8_t Safer::key_sum_mod2(uint8_t key, uint8_t text) {
    return (key ^ text);
}

uint8_t Safer::key_sum_mod256(uint8_t key, uint8_t text) {
    return (key + text) % 256;
}

uint8_t Safer::key_sub_mod256(uint8_t key, uint8_t text) {
    return text - key % 256;
}

uint8_t Safer::funcE(uint8_t text) {
    return matrix_e[text];
}

uint8_t Safer::funcL(uint8_t text) {
    return matrix_l[text];
}
