/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _FAITH_EMULATOR_MSG_REGISTER_H_
#define _FAITH_EMULATOR_MSG_REGISTER_H_

#include "common.h"
#include "msg.h"

class MsgRegister : public Msg
{
public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** The account name */
        char Account[MAX_NAMESIZE];
        /** The player's name */
        char Name[MAX_NAMESIZE];
        /** The un-encrypted password */
        char Password[MAX_NAMESIZE];
        /** The player's look */
        uint16_t Look;
        /** The player's profession */
        uint16_t Profession;
        /** The account UID */
        int32_t AccountUID;
        /** The player's force */
        uint8_t Force;
        /** The player's health */
        uint8_t Health;
        /** The player's speed */
        uint8_t Speed;
        /** The player's soul */
        uint8_t Soul;
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a message object from the specified buffer.
     * The buffer will be took by the object and the memory
     * freed when the object will be destroyed.
     *
     * If the server is on a Be architecture, all the integers
     * are swapped.
     *
     * @param[in,out] aBuf        a pointer to the buffer to take
     *                            the pointer will be set to null
     * @param[in]     aLen        the length in bytes of the buffer
     */
    MsgRegister(uint8_t** aBuf, size_t aLen);

    /* destructor */
    virtual ~MsgRegister();

    /**
     * Process the message received from the client.
     *
     * @param[in]     aClient      a pointer to the client which
     *                             has sent the message
     */
    virtual void process(Client* aClient);

private:
    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf);


private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _FAITH_EMULATOR_MSG_REGISTER_H_
