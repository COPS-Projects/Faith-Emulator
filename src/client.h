/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _FAITH_EMULATOR_CLIENT_H
#define _FAITH_EMULATOR_CLIENT_H

#include "common.h"
#include "tqcipher.h"
#include <string>

class Database;
class NetworkClient;
class Msg;
class Player;
class NpcTask;

class Client
{
    friend class Database; // the database can manipulate the data

    // !!! class has pointer data members !!!
    PROHIBIT_COPY(Client);

public:
    enum Status
    {
        /** The client is still not authenticated */
        NOT_AUTHENTICATED,
        /** The client is banned */
        BANNED,
        /** The client use an invalid username */
        INVALID_USERNAME,
        /** The client use an invalid password */
        INVALID_PASSWORD,
        /** The client use an invalid token */
        INVALID_KEY,
        /** The client is normal */
        NORMAL,
        /** The client is a new account */
        NEW_ACCOUNT
    };

public:
    Client(NetworkClient* aSocket);
    ~Client();

    void send(Msg* aMsg);
    void send(uint8_t* aBuf, size_t aLen);

    void disconnect() {  /* TODO... */ }

public:
    /** Get a reference to the client cipher */
    TqCipher& getCipher() { return mCipher; }

    /** Get the account name of the client. */
    const char* getAccount() const { return mAccount.c_str(); }
    /** Get the account UID of the client. */
    int32_t getAccountID() const { return mAccountID; }
    /** Get the status of the account */
    Status getStatus() const { return mStatus; }

    /** Get the player object linked to this client */
    Player* getPlayer() const { return mPlayer; }

    void setAccount(const std::string& aAccount) { mAccount = aAccount; }
    void setAccountID(int32_t aAccountID) { mAccountID = aAccountID; }
    void setStatus(Status aStatus) { mStatus = aStatus; }

    void setCurTask(const NpcTask& aTask) { mCurTask = &aTask; }

private:
    NetworkClient* mSocket; //!< the TCP/IP socket wrapper of the client
    TqCipher mCipher; //!< the cipher of the client

    Status mStatus; //!< the status of the account

    std::string mAccount; //!< the account name
    int32_t mAccountID; //!< the account ID
    int8_t mAccLvl; //!< the account permissions level
    int32_t mFlags; //!< the account flags
    std::string mCharacter; //!< the character name

    Player* mPlayer; //!< the character linked to this client

    const NpcTask* mCurTask; //!< the current NPC task
};

#endif // _FAITH_EMULATOR_CLIENT_H
