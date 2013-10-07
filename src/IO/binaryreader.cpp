/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Taken from :
 * ****** BARLab - Open Source ******
 * Copyright (C) 2012 BARLab
 * Copyright (C) 2012 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "binaryreader.h"
#include "finder.h"
#include <stdio.h>

using namespace std;

BinaryReader :: BinaryReader(const char* aPath)
{
    mStream = nullptr;
    ASSERT(aPath != nullptr && aPath[0] != '\0');
    ASSERT(ERROR_SUCCESS == Finder::fileOpen(&mStream, aPath, "r"));
}

BinaryReader :: BinaryReader(const string& aPath)
{
    mStream = nullptr;
    ASSERT(&aPath != nullptr);
    ASSERT(!aPath.empty());
    ASSERT(ERROR_SUCCESS == Finder::fileOpen(&mStream, aPath.c_str(), "r"));
}

BinaryReader :: ~BinaryReader()
{
    close();
}

int64_t
BinaryReader :: tell()
{
    return Finder::fileTell(mStream);
}


err_t
BinaryReader :: seek(int64_t aOffset, int aWhence)
{
    return Finder::fileSeek(mStream, aOffset, aWhence);
}

void
BinaryReader :: close()
{
    if (mStream != nullptr)
    {
        fclose(mStream);
        mStream = nullptr;
    }
}

err_t
BinaryReader :: lock()
{
    ASSERT_ERR(mStream != nullptr, ERROR_BAD_STATE);

    return Finder::fileLock(mStream);
}

err_t
BinaryReader :: unlock()
{
    ASSERT_ERR(mStream != nullptr, ERROR_BAD_STATE);

    return Finder::fileUnlock(mStream);
}

err_t
BinaryReader :: readUInt8(uint8_t& aOutVal)
{
    int8_t* ptr = (int8_t*)(&aOutVal);
    return readInt8(*ptr);
}

err_t
BinaryReader :: readUInt16(uint16_t& aOutVal)
{
    int16_t* ptr = (int16_t*)(&aOutVal);
    return readInt16(*ptr);
}

err_t
BinaryReader :: readUInt32(uint32_t& aOutVal)
{
    int32_t* ptr = (int32_t*)(&aOutVal);
    return readInt32(*ptr);
}

err_t
BinaryReader :: readUInt64(uint64_t& aOutVal)
{
    int64_t* ptr = (int64_t*)(&aOutVal);
    return readInt64(*ptr);
}

template<class T>
err_t
BinaryReader :: read(T& aOutVal)
{
    err_t err = ERROR_SUCCESS;

    if (fread(&aOutVal, sizeof(aOutVal), 1, mStream) != 1)
    {
        err = ERROR_READ_FAULT;
    }

    return err;
}

template err_t BinaryReader :: read<int8_t>(int8_t& aOutVal);
template err_t BinaryReader :: read<int16_t>(int16_t& aOutVal);
template err_t BinaryReader :: read<int32_t>(int32_t& aOutVal);
template err_t BinaryReader :: read<int64_t>(int64_t& aOutVal);

err_t
BinaryReader :: read(void* aBuf, size_t aLen)
{
    ASSERT_ERR(aBuf != nullptr, ERROR_BAD_PARAMS);
    ASSERT_ERR(aLen > 0, ERROR_BAD_PARAMS);
    ASSERT_ERR(mStream != nullptr, ERROR_BAD_STATE);
    ASSERT_ERR(feof(mStream) == 0, ERROR_BAD_STATE);

    err_t err = ERROR_SUCCESS;

    if (fread(aBuf, sizeof(uint8_t), aLen, mStream) != aLen)
    {
        err = ERROR_READ_FAULT;
    }

    return err;
}

err_t
BinaryReader :: readBoolean(bool& aOutVal)
{
    ASSERT_ERR(&aOutVal != nullptr, ERROR_INVALID_REFERENCE);
    ASSERT_ERR(mStream != nullptr, ERROR_BAD_STATE);
    ASSERT_ERR(feof(mStream) == 0, ERROR_BAD_STATE);

    err_t err = ERROR_SUCCESS;

    int8_t val = 0;
    DOIF(err, read<int8_t>(val));
    aOutVal = val != 0 ? true : false;

    return err;
}

err_t
BinaryReader :: readInt8(int8_t& aOutVal)
{
    ASSERT_ERR(&aOutVal != nullptr, ERROR_INVALID_REFERENCE);
    ASSERT_ERR(mStream != nullptr, ERROR_BAD_STATE);
    ASSERT_ERR(feof(mStream) == 0, ERROR_BAD_STATE);

    err_t err = ERROR_SUCCESS;

    DOIF(err, read<int8_t>(aOutVal));

    return err;
}

err_t
BinaryReader :: readInt16(int16_t& aOutVal)
{
    ASSERT_ERR(&aOutVal != nullptr, ERROR_INVALID_REFERENCE);
    ASSERT_ERR(mStream != nullptr, ERROR_BAD_STATE);
    ASSERT_ERR(feof(mStream) == 0, ERROR_BAD_STATE);

    err_t err = ERROR_SUCCESS;

    DOIF(err, read<int16_t>(aOutVal));
    #if BYTE_ORDER == BIG_ENDIAN
    aOutVal = bswap16(aOutVal);
    #endif

    return err;
}

err_t
BinaryReader :: readInt32(int32_t& aOutVal)
{
    ASSERT_ERR(&aOutVal != nullptr, ERROR_INVALID_REFERENCE);
    ASSERT_ERR(mStream != nullptr, ERROR_BAD_STATE);
    ASSERT_ERR(feof(mStream) == 0, ERROR_BAD_STATE);

    err_t err = ERROR_SUCCESS;

    DOIF(err, read<int32_t>(aOutVal));
    #if BYTE_ORDER == BIG_ENDIAN
    aOutVal = bswap32(aOutVal);
    #endif

    return err;
}

err_t
BinaryReader :: readInt64(int64_t& aOutVal)
{
    ASSERT_ERR(&aOutVal != nullptr, ERROR_INVALID_REFERENCE);
    ASSERT_ERR(mStream != nullptr, ERROR_BAD_STATE);
    ASSERT_ERR(feof(mStream) == 0, ERROR_BAD_STATE);

    err_t err = ERROR_SUCCESS;

    DOIF(err, read<int64_t>(aOutVal));
    #if BYTE_ORDER == BIG_ENDIAN
    aOutVal = bswap64(aOutVal);
    #endif

    return err;
}
