/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgwalk.h"
#include "client.h"
#include "player.h"
#include "mapmanager.h"

MsgWalk :: MsgWalk(int32_t aUniqId, uint8_t aDirection, bool aIsRunning)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aUniqId, aDirection, aIsRunning);
}

MsgWalk :: MsgWalk(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen), mInfo((MsgInfo*)mBuf)
{
    ASSERT(aLen >= sizeof(MsgInfo));

    #if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
    #endif
}


MsgWalk :: ~MsgWalk()
{

}

void
MsgWalk :: create(int32_t aUniqId, uint8_t aDirection, bool aIsRunning)
{
    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_WALK;

    mInfo->UniqId = aUniqId;
    mInfo->Direction = aDirection;//(Byte)(((MyMath.Generate(100, 255) % 31) * 8) + Direction);
    mInfo->Mode = !aIsRunning ? 0 : 1;

    mInfo->Padding1 = 0;
}

void
MsgWalk :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);
    ASSERT(aClient->getPlayer() != nullptr);

    Client& client = *aClient;
    Player& player = *aClient->getPlayer();

    if (player.getUID() != mInfo->UniqId)
    {
        client.disconnect();
        return;
    }

    uint8_t dir = (uint8_t)(mInfo->Direction % 8);
    uint16_t newX = player.getPosX();
    uint16_t newY = player.getPosY();

    switch (dir)
    {
    case 0: { newY += 1; break; }
    case 1: { newX -= 1; newY += 1; break; }
    case 2: { newX -= 1; break; }
    case 3: { newX -= 1; newY -= 1; break; }
    case 4: { newY -= 1; break; }
    case 5: { newX += 1; newY -= 1; break; }
    case 6: { newX += 1; break; }
    case 7: { newX += 1; newY += 1; break; }
    }

    // TODO: Implement
    //if (!Player.IsAlive() && !Player.IsGhost())
    //{
    //    Player.SendSysMsg(Client.GetStr("STR_DIE"));
    //    return;
    //}

    MapManager& mgr = MapManager::getInstance();
    GameMap* map = mgr.getMap(player.getMapId());
    if (map != nullptr)
    {
        if (map->getFloorAccess(player.getPosX(), player.getPosY()))
        {
            player.sendSysMsg(STR_INVALID_COORDINATE);
            //player.kickBack(); // TODO
            return;
        }
    }

    player.move(newX, newY, dir);

    client.send(this);
    //  Player.Screen.Move(Buffer);
}

void
MsgWalk :: swap(uint8_t* aBuf)
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->UniqId = bswap32(info->UniqId);
}
