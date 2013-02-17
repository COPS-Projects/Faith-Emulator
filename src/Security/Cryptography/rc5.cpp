/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "rc5.h"
#include <string.h> // memset
#include <algorithm> // max()

using namespace std;

/* static */
const size_t RC5::RC5_SUB;
const size_t RC5::RC5_KEY;

RC5 :: RC5()
{
    // security purpose only...
    memset(mKey, 0, sizeof(mKey));
    memset(mSub, 0, sizeof(mSub));
}

RC5 :: ~RC5()
{

}

void
RC5 :: generateKey(uint8_t aSeed[KEY_SIZE])
{
    ASSERT(aSeed != nullptr);

    uint32_t* seed = (uint32_t*)aSeed;
    for (size_t i = 0; i < RC5_KEY; ++i)
    {
        mKey[i] = seed[i];
    }

    mSub[0] = RC5_PW32;
    size_t i, j;
    for (i = 1; i < RC5_SUB; ++i)
    {
        mSub[i] = mSub[i - 1] - RC5_QW32;
    }

    uint32_t x, y;
    i = j = 0;
    x = y = 0;
    for (size_t k = 0, size = 3 * max(RC5_KEY, RC5_SUB); k < size; ++k)
    {
        mSub[i] = rotl((mSub[i] + x + y), 3);
        x = mSub[i];
        i = (i + 1) % RC5_SUB;
        mKey[j] = rotl((mKey[j] + x + y), (x + y));
        y = mKey[j];
        j = (j + 1) % RC5_KEY;
    }
}

void
RC5 :: encrypt(uint8_t* aBuf, size_t aLen)
{
    ASSERT(aBuf != nullptr);
    ASSERT(aLen > 0 && aLen % BLOCK_SIZE == 0);

    uint32_t* buf = (uint32_t*)aBuf;
    for (size_t i = 0, len = aLen / BLOCK_SIZE; i < len; ++i)
    {
        uint32_t a = buf[2 * i];
        uint32_t b = buf[2 * i + 1];

        uint32_t le = a + mSub[0];
        uint32_t re = b + mSub[1];
        for (size_t j = 1; j < ROUNDS; ++j)
        {
            le = rotl((le ^ re), re) + mSub[2 * j];
            re = rotl((re ^ le), le) + mSub[2 * j + 1];
        }

        buf[2 * i] = le;
        buf[2 * i + 1] = re;
    }
}

void
RC5 :: decrypt(uint8_t* aBuf, size_t aLen)
{
    ASSERT(aBuf != nullptr);
    ASSERT(aLen > 0 && aLen % BLOCK_SIZE == 0);

    uint32_t* buf = (uint32_t*)aBuf;
    for (size_t i = 0, len = aLen / BLOCK_SIZE; i < len; ++i)
    {
        uint32_t ld = buf[2 * i];
        uint32_t rd = buf[2 * i + 1];
        for (size_t j = ROUNDS; j >= 1; --j)
        {
            rd = rotr((rd - mSub[2 * j + 1]), ld) ^ ld;
            ld = rotr((ld - mSub[2 * j]), rd) ^ rd;
        }

        uint32_t a = rd - mSub[1];
        uint32_t b = ld - mSub[0];

        buf[2 * i] = b;
        buf[2 * i + 1] = a;
    }
}
