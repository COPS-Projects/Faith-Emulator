/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _FAITH_EMULATOR_MSG_NPCINFO_H_
#define _FAITH_EMULATOR_MSG_NPCINFO_H_

#include "common.h"
#include "msg.h"

class Npc;

/**
 * Msg sent by the MsgServer to the client to spawn a Npc.
 */
class MsgNpcInfo : public Msg
{
public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** The unique Id of the NPC */
        uint32_t UniqId;
        /** The X coord of the NPC */
        uint16_t PosX;
        /** The Y coord of the NPC */
        uint16_t PosY;
        /** The look of the NPC or the direction for a statuary */
        uint16_t Look;
        /** The type of the NPC, or the frame for a statuary */
        int16_t Type;
        /** The sort of the NPC, or the pose for a statuary */
        int16_t Sort;
        /** The length of the NPC (unused) */
        int8_t Length;
        /** The fat of the NPC (unused) */
        int8_t Fat;
        /** Name of the NPC (not compulsary) */
        uint8_t Buf[1];
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgNpcInfo packet for the specified npc.
     *
     * @param[in]   aNpc     a reference to the npc object
     */
    MsgNpcInfo(const Npc& aNpc);

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
    MsgNpcInfo(uint8_t** aBuf, size_t aLen);

    /* destructor */
    virtual ~MsgNpcInfo();

private:
    /* internal filling of the packet */
    void create(const Npc& aNpc);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _FAITH_EMULATOR_MSG_NPCINFO_H_
