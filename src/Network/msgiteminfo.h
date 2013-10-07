/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _FAITH_EMULATOR_MSG_ITEMINFO_H_
#define _FAITH_EMULATOR_MSG_ITEMINFO_H_

#include "common.h"
#include "msg.h"

class MsgItemInfo : public Msg
{
public:
    enum Action
    {
        ACTION_NONE = 0,
        ACTION_ADD_ITEM = 1,
        ACTION_TRADE = 2,
        ACTION_UPDATE = 3,
        ACTION_OTHER_PLAYER_EQUIP = 4, // uid is user uid
        ACTION_AUCTION = 5
    };

public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        uint32_t UniqId;
        int32_t Type;
        uint16_t Amount;
        uint16_t AmountLimit;
        uint8_t Action;
        uint8_t Ident;
        uint8_t Position;
        uint8_t Gem1;
        uint8_t Gem2;
        uint8_t Magic1; // Attr
        uint8_t Magic2; // Unknown
        uint8_t Magic3; // Plus
    }MsgInfo;
    #pragma pack(pop)

public:
    MsgItemInfo(void* aItem, Action aAction); // FIXME!

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
    MsgItemInfo(uint8_t** aBuf, size_t aLen);

    /* destructor */
    virtual ~MsgItemInfo();

private:
    /* internal filling of the packet */
    void create(void* aItem, Action aAction); // FIXME!

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf);

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _FAITH_EMULATOR_MSG_ITEMINFO_H_
