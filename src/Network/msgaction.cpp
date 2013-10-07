/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgaction.h"
#include "client.h"
#include "entity.h"
#include "player.h"
#include "msgplayer.h"

MsgAction :: MsgAction(Entity* aEntity, int32_t aData, Action aAction)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aEntity, aData, aAction);
}

MsgAction :: MsgAction(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen), mInfo((MsgInfo*)mBuf)
{
    ASSERT(aLen >= sizeof(MsgInfo));

    #if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
    #endif
}


MsgAction :: ~MsgAction()
{

}

void
MsgAction :: create(Entity* aEntity, int32_t aData, Action aAction)
{
    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_ACTION;

    mInfo->Timestamp = timeGetTime();
    if (aEntity != nullptr)
    {
        mInfo->UniqId = aEntity->getUID();
        mInfo->PosX = aEntity->getPosX();
        mInfo->PosY = aEntity->getPosY();
        mInfo->Direction = aEntity->getDirection();
    }
    else
    {
        mInfo->UniqId = 0;
        mInfo->PosX = 0;
        mInfo->PosY = 0;
        mInfo->Direction = 0;
    }

    mInfo->Data = aData;
    mInfo->Action = aAction;
}

void
MsgAction :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);
    ASSERT(aClient->getPlayer() != nullptr);

    Client& client = *aClient;
    Player& player = *aClient->getPlayer();

    switch (mInfo->Action)
    {
    case ACTION_CHG_DIR:
        {
            if (player.getUID() != mInfo->UniqId)
            {
                client.disconnect();
                return;
            }

            player.setDirection((uint8_t)mInfo->Direction);
            //BroadcastRoomMsg(false)
            break;
        }
    case ACTION_ENTER_MAP:
        {
            mInfo->PosX = player.getPosX();
            mInfo->PosY = player.getPosY();
            mInfo->Data = player.getMapId();
            mInfo->Direction = player.getDirection();
            client.send(this);

            player.enterMap();
            break;
        }
    case ACTION_GET_ITEM_SET:
        {
            // TODO: send item set

            client.send(this);
            break;
        }
    case ACTION_GET_GOOD_FRIEND:
        {
            // TODO send friends / enemies

            client.send(this);
            break;
        }
    case ACTION_GET_WEAPON_SKILL_SET:
        {
            // TODO send weapon skills

            client.send(this);
            break;
        }
    case ACTION_GET_MAGIC_SET:
        {
            // TODO send skills

            client.send(this);
            break;
        }
    case ACTION_SET_PKMODE:
        {
            const char* msg = nullptr;
            switch ((PkMode)mInfo->Data)
            {
            case PKMODE_FREE:
                {
                    msg = STR_FREE_PK_MODE;
                    break;
                }
            case PKMODE_SAFE:
                {
                    msg = STR_SAFE_PK_MODE;
                    break;
                }
            case PKMODE_TEAM:
                {
                    msg = STR_TEAM_PK_MODE;
                    break;
                }
            case PKMODE_ARRESTMENT:
                {
                    msg = STR_ARRESTMENT_PK_MODE;
                    break;
                }
            default:
                break; // TODO: Invalid mode
            }

            //role.pkmode = data;
            //role. isinbattle = false

            client.send(this);
            player.sendSysMsg(msg);
            break;
        }
    case ACTION_GET_SYN_ATTR:
        {
            // TODO send syn attributes

            client.send(this);
            break;
        }
    case ACTION_DESTROY_BOOTH:
        {
            // TODO: Implement booths
            break;
        }
    default:
        {
            fprintf(stdout, "Unknown action[%04u], data=[%d]\n",
                    mInfo->Action, mInfo->Data);
            break;
        }
    }
}

void
MsgAction :: swap(uint8_t* aBuf)
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->Timestamp = bswap32(info->Timestamp);
    info->UniqId = bswap32(info->UniqId);
    info->PosX = bswap16(info->PosX);
    info->PosY = bswap16(info->PosY);
    info->Direction = bswap32(info->Direction);
    info->Data = bswap32(info->Data);
    info->Action = bswap32(info->Action);
}
