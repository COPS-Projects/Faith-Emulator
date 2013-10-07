/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _FAITH_EMULATOR_MSG_USERATTRIB_H_
#define _FAITH_EMULATOR_MSG_USERATTRIB_H_

#include "common.h"
#include "msg.h"

class Entity;

class MsgUserAttrib : public Msg
{
public:
    enum UserAttrType
    {
        USER_ATTRIB_LIFE = 0,
        USER_ATTRIB_MAX_LIFE = 1,
        USER_ATTRIB_MANA = 2,
        USER_ATTRIB_MAX_MANA = 3,
        USER_ATTRIB_MONEY = 4,
        USER_ATTRIB_EXP = 5,
        USER_ATTRIB_PK = 6,
        USER_ATTRIB_PROFESSION = 7,
        USER_ATTRIB_SIZE_ADD = 8,
        USER_ATTRIB_ENERGY = 9,
        USER_ATTRIB_MONEY_SAVED = 10,
        USER_ATTRIB_ADD_POINT = 11,
        USER_ATTRIB_LOOK_FACE = 12,
        USER_ATTRIB_LEV = 13,
        USER_ATTRIB_SOUL = 14,
        USER_ATTRIB_HEALTH = 15,
        USER_ATTRIB_FORCE = 16,
        USER_ATTRIB_SPEED = 17,

        USER_ATTRIB_HAIR = 27,
        USER_ATTRIB_XP = 28
    };

public:
    #pragma pack(push, 1)
    typedef struct
    {
        int32_t Type;
        int32_t Data;
    }UserAttrib;
    #pragma pack(pop)

public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        uint32_t UniqId;
        int32_t Amount;
        UserAttrib Attrib[1];
    }MsgInfo;
    #pragma pack(pop)

public:
    MsgUserAttrib(Entity* aEntity, int32_t aData, UserAttrType aType);

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
    MsgUserAttrib(uint8_t** aBuf, size_t aLen);

    /* destructor */
    virtual ~MsgUserAttrib();

private:
    /* internal filling of the packet */
    void create(Entity* aEntity, int32_t aData, UserAttrType aType);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf);

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _FAITH_EMULATOR_MSG_USERATTRIB_H_
