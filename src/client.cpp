/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "client.h"
#include "networkclient.h"
#include "msg.h"
#include "player.h"
#include <stdlib.h>

Client :: Client(NetworkClient* aSocket)
{
    ASSERT(aSocket != nullptr);

    mSocket = aSocket;
    mCipher.generateIV(0x0705FD1F, 0x1B7A313F);

    mStatus = Client::NOT_AUTHENTICATED;

    mAccountID = -1;
    mAccLvl = 0;
    mFlags = 0;

    mPlayer = nullptr;
}

Client :: ~Client()
{
    mSocket = nullptr; // Qt handle the memory
    SAFE_DELETE(mPlayer);
}

void
Client :: send(Msg* aMsg)
{
    ASSERT(aMsg != nullptr);

    uint8_t* data = new uint8_t[aMsg->getLength()];
    memcpy(data, aMsg->getBuffer(), aMsg->getLength());

    mCipher.encrypt(data, aMsg->getLength());
    mSocket->send(data, aMsg->getLength());

    SAFE_DELETE_ARRAY(data);
}

void
Client :: send(uint8_t* aBuf, size_t aLen)
{
    ASSERT(aBuf != nullptr);

    uint8_t* data = new uint8_t[aLen];
    memcpy(data, aBuf, aLen);

    mCipher.encrypt(data, aLen);
    mSocket->send(data, aLen);

    SAFE_DELETE_ARRAY(data);
}
