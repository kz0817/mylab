#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include "md5.h"
using namespace std;

const size_t MD5_CALC_NUM_BLOCK = 4;
const size_t MD5_PAD_HEADER_LENGTH = 1;
const size_t MD5_PAD_SIZE_LENGTH = 8;

static const size_t s[MD5_CALC_BLOCK_SIZE] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,
};

static const uint32_t K[MD5_CALC_BLOCK_SIZE] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

static const uint32_t initialValues[MD5_CALC_NUM_BLOCK] = {
    0x67452301,
    0xefcdab89,
    0x98badcfe,
    0x10325476,
};

void dump(const void *buf, const size_t len)
{
    const size_t NUM_VAL_PER_LINE = 16;
    const size_t numLoop = len / NUM_VAL_PER_LINE;
    const size_t numLastData = len % NUM_VAL_PER_LINE;
    const uint8_t *data = (const uint8_t *)buf;
    for (size_t i = 0; i < numLoop; i++) {
        for (size_t j = 0; j < NUM_VAL_PER_LINE; j++, data++)
            printf("%02x ", *data);
        printf("\n");
    }
    if (numLastData) {
        for (size_t j = 0; j < numLastData; j++, data++)
            printf("%02x ", *data);
        printf("\n");
    }
}

static void initialize(MD5Context *ctx)
{
    ctx->A = initialValues[0];
    ctx->B = initialValues[1];
    ctx->C = initialValues[2];
    ctx->D = initialValues[3];
}

struct CalcLoopInfo {
    size_t numLoops;
    bool paddingHeaderInPreLast;
    size_t zeroPadLenInPreLast;
    size_t lastDataLen;
};

static void add(MD5Context *ctx)
{
    ctx->A += ctx->a;
    ctx->B += ctx->b;
    ctx->C += ctx->c;
    ctx->D += ctx->d;
}

static void load(MD5Context *ctx)
{
    ctx->a = ctx->A;
    ctx->b = ctx->B;
    ctx->c = ctx->C;
    ctx->d = ctx->D;
}

static uint32_t leftrotate(uint32_t x, size_t c)
{
    return (x << c) | (x >> (32-c));
}

static uint32_t revertEndian(const uint32_t n)
{
    uint8_t v[4];
    const uint8_t *m = (const uint8_t *)&n;
    v[0] = m[3];
    v[1] = m[2];
    v[2] = m[1];
    v[3] = m[0];
    return *((uint32_t *)v);
}

static void updateWorkVars(
  MD5Context *ctx, const uint32_t K, const size_t s,
  const uint32_t M, const uint32_t W)
{
    const uint32_t prevD = ctx->d;
    ctx->d = ctx->c;
    ctx->c = ctx->b;
    ctx->b += leftrotate(ctx->a + W + K + M, s);
    ctx->a = prevD;
}

static uint32_t F(const uint32_t B, const uint32_t C, const uint32_t D)
{
    return (B & C) | ((~B) & D);
}

static uint32_t G(const uint32_t B, const uint32_t C, const uint32_t D)
{
    return (D & B) | ((~D) & C);
}

static uint32_t H(const uint32_t B, const uint32_t C, const uint32_t D)
{
    return B ^ C ^ D;
}

static uint32_t I(const uint32_t B, const uint32_t C, const uint32_t D)
{
    return C ^ (B | (~D));
}

static void round(
  MD5Context *ctx,
  uint32_t (*func)(const uint32_t, const uint32_t, const uint32_t),
  const uint32_t K, const size_t s, const uint32_t M)
{
    updateWorkVars(ctx, K, s, M, func(ctx->b, ctx->c, ctx->d));
}

static void doRound(MD5Context *ctx, const uint32_t *M)
{
    load(ctx);

    round(ctx, F, K[0], s[0], M[0]);
    round(ctx, F, K[1], s[1], M[1]);
    round(ctx, F, K[2], s[2], M[2]);
    round(ctx, F, K[3], s[3], M[3]);
    round(ctx, F, K[4], s[4], M[4]);
    round(ctx, F, K[5], s[5], M[5]);
    round(ctx, F, K[6], s[6], M[6]);
    round(ctx, F, K[7], s[7], M[7]);
    round(ctx, F, K[8], s[8], M[8]);
    round(ctx, F, K[9], s[9], M[9]);
    round(ctx, F, K[10], s[10], M[10]);
    round(ctx, F, K[11], s[11], M[11]);
    round(ctx, F, K[12], s[12], M[12]);
    round(ctx, F, K[13], s[13], M[13]);
    round(ctx, F, K[14], s[14], M[14]);
    round(ctx, F, K[15], s[15], M[15]);

    round(ctx, G, K[16], s[16], M[1]);
    round(ctx, G, K[17], s[17], M[6]);
    round(ctx, G, K[18], s[18], M[11]);
    round(ctx, G, K[19], s[19], M[0]);
    round(ctx, G, K[20], s[20], M[5]);
    round(ctx, G, K[21], s[21], M[10]);
    round(ctx, G, K[22], s[22], M[15]);
    round(ctx, G, K[23], s[23], M[4]);
    round(ctx, G, K[24], s[24], M[9]);
    round(ctx, G, K[25], s[25], M[14]);
    round(ctx, G, K[26], s[26], M[3]);
    round(ctx, G, K[27], s[27], M[8]);
    round(ctx, G, K[28], s[28], M[13]);
    round(ctx, G, K[29], s[29], M[2]);
    round(ctx, G, K[30], s[30], M[7]);
    round(ctx, G, K[31], s[31], M[12]);

    round(ctx, H, K[32], s[32], M[5]);
    round(ctx, H, K[33], s[33], M[8]);
    round(ctx, H, K[34], s[34], M[11]);
    round(ctx, H, K[35], s[35], M[14]);
    round(ctx, H, K[36], s[36], M[1]);
    round(ctx, H, K[37], s[37], M[4]);
    round(ctx, H, K[38], s[38], M[7]);
    round(ctx, H, K[39], s[39], M[10]);
    round(ctx, H, K[40], s[40], M[13]);
    round(ctx, H, K[41], s[41], M[0]);
    round(ctx, H, K[42], s[42], M[3]);
    round(ctx, H, K[43], s[43], M[6]);
    round(ctx, H, K[44], s[44], M[9]);
    round(ctx, H, K[45], s[45], M[12]);
    round(ctx, H, K[46], s[46], M[15]);
    round(ctx, H, K[47], s[47], M[2]);

    round(ctx, I, K[48], s[48], M[0]);
    round(ctx, I, K[49], s[49], M[7]);
    round(ctx, I, K[50], s[50], M[14]);
    round(ctx, I, K[51], s[51], M[5]);
    round(ctx, I, K[52], s[52], M[12]);
    round(ctx, I, K[53], s[53], M[3]);
    round(ctx, I, K[54], s[54], M[10]);
    round(ctx, I, K[55], s[55], M[1]);
    round(ctx, I, K[56], s[56], M[8]);
    round(ctx, I, K[57], s[57], M[15]);
    round(ctx, I, K[58], s[58], M[6]);
    round(ctx, I, K[59], s[59], M[13]);
    round(ctx, I, K[60], s[60], M[4]);
    round(ctx, I, K[61], s[61], M[11]);
    round(ctx, I, K[62], s[62], M[2]);
    round(ctx, I, K[63], s[63], M[9]);

    add(ctx);
}

static void setLengthAsBits(uint8_t *buf, const uint64_t msgBytes)
{
    uint64_t msgBits = 8 * msgBytes;
    memcpy(buf, &msgBits, sizeof(msgBytes));
}

static size_t copyDataAndPadHeader(
  uint8_t *buf, const uint8_t *data, const CalcLoopInfo *info)
{
    memcpy(buf, data, info->lastDataLen);
    buf += info->lastDataLen;

    *buf = 0x80;

    return info->lastDataLen + MD5_PAD_HEADER_LENGTH;
}

void createPreLastBlock(
  uint8_t *buf, const uint8_t *data, const CalcLoopInfo *info)
{
    buf += copyDataAndPadHeader(buf, data, info);
    bzero(buf, info->zeroPadLenInPreLast);
}

void createLastBlock(
  uint8_t *buf, const uint8_t *data, const CalcLoopInfo *info, size_t totalSize)
{
    size_t zeroPadLen = MD5_CALC_BLOCK_SIZE - MD5_PAD_SIZE_LENGTH;
    if (!info->paddingHeaderInPreLast) {
        const size_t copyLen = copyDataAndPadHeader(buf, data, info);
        buf += copyLen;
        zeroPadLen -= copyLen;
    }

    bzero(buf, zeroPadLen);
    buf += zeroPadLen;

    setLengthAsBits(buf, totalSize);
}

static uint8_t *currTailBufferPtr(MD5Context *ctx)
{
    return &ctx->tailBuffer[ctx->tailBufferIdx];
}

void MD5init(MD5Context *ctx)
{
    initialize(ctx);
    ctx->totalLength = 0;
    ctx->tailBufferIdx = 0;
}

void MD5update(const void *_data, size_t len, MD5Context *ctx)
{
    const size_t numLoops = (ctx->tailBufferIdx + len) / MD5_CALC_BLOCK_SIZE;
    const size_t remaining = (ctx->tailBufferIdx + len) % MD5_CALC_BLOCK_SIZE;

    uint8_t *dataPtr = (uint8_t *)_data;
    for (size_t i = 0; i < numLoops; i++) {
        if (i == 0 && ctx->tailBufferIdx) {
            const size_t copyLen = MD5_CALC_BLOCK_SIZE - ctx->tailBufferIdx;
            memcpy(currTailBufferPtr(ctx), dataPtr, copyLen);
            doRound(ctx, (const uint32_t *)ctx->tailBuffer);

            ctx->tailBufferIdx = 0;
            dataPtr += copyLen;
            continue;
        }

        doRound(ctx, (const uint32_t *)dataPtr);
        dataPtr += MD5_CALC_BLOCK_SIZE;
    }

    if (remaining) {
        memcpy(ctx->tailBuffer, dataPtr, remaining);
        ctx->tailBufferIdx += remaining;
    }

    ctx->totalLength += len;
}

static void setupLastBlock(MD5Context *ctx, const size_t space)
{
    const size_t zeroLen = space - MD5_PAD_SIZE_LENGTH;
    bzero(currTailBufferPtr(ctx), zeroLen);
    ctx->tailBufferIdx += zeroLen;
    setLengthAsBits(currTailBufferPtr(ctx), ctx->totalLength);
}

void MD5finish(MD5Context *ctx)
{
    ctx->tailBuffer[ctx->tailBufferIdx] = 0x80;
    ctx->tailBufferIdx++;

    bool lastBlock = false;
    const size_t space = MD5_CALC_BLOCK_SIZE - ctx->tailBufferIdx;
    if (space >= MD5_PAD_SIZE_LENGTH) {
        setupLastBlock(ctx, space);
        lastBlock = true;
    } else if (space) {
        bzero(currTailBufferPtr(ctx), space);
    }
    ctx->tailBufferIdx = 0;

    doRound(ctx, (const uint32_t *)ctx->tailBuffer);
    if (lastBlock)
        return;

    setupLastBlock(ctx, MD5_CALC_BLOCK_SIZE);
    doRound(ctx, (const uint32_t *)ctx->tailBuffer);
}

string getDigest(const MD5Context *ctx)
{
    stringstream ss;
    ss << hex << setfill('0')
       << setw(8) << revertEndian(ctx->A)
       << setw(8) << revertEndian(ctx->B)
       << setw(8) << revertEndian(ctx->C)
       << setw(8) << revertEndian(ctx->D);
    return ss.str();
}
