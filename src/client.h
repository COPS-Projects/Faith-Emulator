/*
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _FAITH_EMULATOR_CLIENT_H_
#define _FAITH_EMULATOR_CLIENT_H_

#include "common.h"
#include "icipher.h"
#include <string>

class Database;
class NetworkClient;
class Msg;
class Player;
class NpcTask;

/**
 * Assossiated client to a NetworkClient.
 */
class Client
{
    friend class Database; // the database can manipulate the data

    // !!! class has pointer data members !!!
    PROHIBIT_COPY(Client);

public:
    /** The status of the client. */
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
    /* constructor */
    Client(NetworkClient* aSocket);

    /* destructor */
    ~Client();

    /**
     * Save the player.
     */
    void save();

    /**
     * Send a message to the client.
     *
     * @param[in]  aMsg     the message to send
     */
    void send(Msg* aMsg);

    /**
     * Send a message to the client.
     *
     * @param[in]   aBuf    the message to send
     * @param[in]   aLen    the length of the message
     */
    void send(uint8_t* aBuf, size_t aLen);

    /**
     * Disconnect the client from the server.
     */
    void disconnect();

public:
    /** Get a reference to the client cipher */
    ICipher& getCipher() const { return *mCipher; }

    /** Get the account name of the client. */
    const char* getAccount() const { return mAccount.c_str(); }
    /** Get the account UID of the client. */
    int32_t getAccountID() const { return mAccountID; }
    /** Get the status of the account */
    Status getStatus() const { return mStatus; }

    /** Get the player object linked to this client */
    Player* getPlayer() const { return mPlayer; }

    /** Set the account name of the client. */
    void setAccount(const std::string& aAccount) { mAccount = aAccount; }
    /** Set the account ID of the client. */
    void setAccountID(int32_t aAccountID) { mAccountID = aAccountID; }
    /** Set the client status. */
    void setStatus(Status aStatus) { mStatus = aStatus; }

    /** Set the current Npc task. */
    void setCurTask(const NpcTask& aTask) { mCurTask = &aTask; }

private:
    NetworkClient* mSocket; //!< the TCP/IP socket wrapper of the client
    ICipher* mCipher; //!< the cipher of the client

    Status mStatus; //!< the status of the account

    std::string mAccount; //!< the account name
    int32_t mAccountID; //!< the account ID
    int8_t mAccLvl; //!< the account permissions level
    int32_t mFlags; //!< the account flags
    std::string mCharacter; //!< the character name

    Player* mPlayer; //!< the character linked to this client

    const NpcTask* mCurTask; //!< the current NPC task
};

#endif // _FAITH_EMULATOR_CLIENT_H_
