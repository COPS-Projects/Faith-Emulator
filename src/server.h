/*
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _FAITH_EMULATOR_SERVER_H_
#define _FAITH_EMULATOR_SERVER_H_

#include "common.h"
#include "env.h"
#include "tcpserver.h"
#include <string>

class NetworkClient;

/**
 * Global server object with the AccServer and the MsgServer.
 * It is a singleton and will be created when getting the instance.
 */
class Server : public Environment::Global
{
    // !!! class is a singleton !!!
    PROHIBIT_COPY(Server);

public:
    /** The account server port. */
    static const uint16_t ACCSERVER_PORT = 9958;
    /** The game server port. */
    static const uint16_t MSGSERVER_PORT = 5816;

public:
    /**
     * Get the Server singleton. If the object does not exist yet,
     * it will be created.
     *
     * @returns A reference to the singleton
     */
    static Server& getInstance();

    /** Get the IPv4 address of the MsgServer. */
    static const char* getServerIP() { return getInstance().mServerIP.c_str(); }
    /** Get the name of the MsgServer. */
    static const char* getServerName() { return getInstance().mServerName.c_str(); }

public:
    /* destructor */
    ~Server();

private:
    /**
     * Function callback to handle new connections to the server.
     *
     * @param aClient[in]   The new network client
     */
    static void connectionHandler(NetworkClient* aClient);

    /**
     * Function callback to handle data received by the server.
     *
     * @param aClient[in]   The client that sent the data
     * @param aBuf[in]      A pointer to the buffer containing the data
     * @param aLen[in]      The length in bytes of the buffer
     */
    static void receiveHandler(NetworkClient* aClient, uint8_t* aBuf, size_t aLen);

    /**
     * Function callback to handle disconnections of a client.
     *
     * @param aClient[in]   The client that will be disconnected
     */
    static void disconnectionHandler(NetworkClient* aClient);

private:
    /* constructor */
    Server();

private:
    static Server* sInstance; //!< static instance of the singleton

private:
    TcpServer mAccServer; //!< TCP/IP server for the AccServer
    TcpServer mMsgServer; //!< TCP/IP server for the MsgServer

    std::string mServerName; //!< name of the MsgServer
    std::string mServerIP; //!< IPv4 address for the MsgServer
};

#endif // _FAITH_EMULATOR_SERVER_H_
