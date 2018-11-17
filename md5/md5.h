#pragma once
#include <string>

static const size_t MD5_CALC_BLOCK_SIZE = 64;

struct MD5Context {
    // updated every 512 bits of data is processed
    uint32_t A;
    uint32_t B;
    uint32_t C;
    uint32_t D;

    // updated every round
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;

    uint64_t totalLength;
    uint8_t tailBuffer[MD5_CALC_BLOCK_SIZE];
    size_t  tailBufferIdx;

    void (*verboseMessage)(const std::string &msg);
};

void MD5init(MD5Context *ctx);
void MD5update(const void *_data, size_t len, MD5Context *ctx);
void MD5finish(MD5Context *ctx);
std::string getDigest(const MD5Context *ctx);
