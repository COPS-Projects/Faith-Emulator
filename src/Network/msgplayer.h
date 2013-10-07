/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _FAITH_EMULATOR_MSG_PLAYER_H_
#define _FAITH_EMULATOR_MSG_PLAYER_H_

#include "common.h"
#include "msg.h"

class Player;
class Monster;

/**
 * Msg sent by the MsgServer to spawn a player or a monster.
 */
class MsgPlayer : public Msg
{
public:
    /** Legion ID mask on a 32-bit integer. */
    static const int32_t MASK_SYNID = INT32_C(0x00FFFFFF);
    /** Legion rank shift on a 32-bit integer. */
    static const int32_t MASK_RANK_SHIFT = INT32_C(24);

public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** The unique Id of the entity */
        uint32_t UniqId;
        /** The look of the entity */
        uint32_t Look;

        union
        {
            /** The status of the entity */
            uint64_t Status;
            struct
            {
                /** The life of the statuary */
                uint16_t StatuaryLife;
                /** The frame of the statuary */
                uint16_t StaturayFrame;
            };
        };

        union
        {
            /** Legion Id and rank of the player */
            int32_t SynId_Rank;
            /** Owner unique Id of the monster. */
            int32_t OwnerUID;
        };

        /** Right hand weapon Id. */
        int32_t WeaponRType;
        /** Left hand weapon Id. */
        int32_t WeaponLType;

        union
        {
            /** Mount Id. */
            int32_t MountType;
            struct
            {
                /** Current life of the monster. */
                uint16_t Life;
                /** Level of the monstre. */
                uint16_t Level;
            };
        };

        /** Mantle Id. */
        int16_t MantleType;
        /** The X coord of the entity. */
        uint16_t PosX;
        /** The Y coord of the entity. */
        uint16_t PosY;
        /** The hair of the player. */
        uint16_t Hair;
        /** The length of the entity (unused) */
        int8_t Length;
        /** The fat of the entity (unused) */
        int8_t Fat;
        /** The cardinal direction of the entity. */
        int8_t Direction;
        /**  The pose of the entity. */
        uint16_t Pose;
        /** The name of the entity */
        uint8_t Buf[1];
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgPlayer packet for the specified player.
     *
     * @param[in]   aPlayer     a reference to the player object
     */
    MsgPlayer(Player& aPlayer);

    /**
     * Create a new MsgPlayer packet for the specified monster.
     *
     * @param[in]   aMonster     a reference to the monster object
     */
    MsgPlayer(Monster& aMonster); //TODO...


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
    MsgPlayer(uint8_t** aBuf, size_t aLen);

    /* destructor */
    virtual ~MsgPlayer();

private:
    /* internal filling of the packet */
    void create(Player& aPlayer);
    void create(Monster& aMonster);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf);

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _FAITH_EMULATOR_MSG_PLAYER_H_
