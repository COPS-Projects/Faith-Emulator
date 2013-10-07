/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgtalk.h"
#include "stringpacker.h"
#include "msgaction.h"
#include "msguserattrib.h"
#include "msgiteminfo.h"
#include "client.h"
#include "player.h"
#include <string.h>

MsgTalk :: MsgTalk(const char* aSpeaker, const char* aHearer, const char* aWords,
                   Channel aChannel, uint32_t aColor)
    : Msg(sizeof(MsgInfo) +
          (aSpeaker != nullptr ? strlen(aSpeaker) : 0)  + 1 +
          (aHearer != nullptr ? strlen(aHearer) : 0)  + 1 +
          /* (aEmotion != nullptr ? strlen(aEmotion) : 0) */ + 1 +
          (aWords != nullptr ? strlen(aWords) : 0) + 1),
      mInfo((MsgInfo*)mBuf)
{
    create(aSpeaker, aHearer, "", aWords, aChannel, aColor); // HACK !
}

MsgTalk :: MsgTalk(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen), mInfo((MsgInfo*)mBuf)
{
    ASSERT(aLen >= sizeof(MsgInfo));

    #if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
    #endif
}


MsgTalk :: ~MsgTalk()
{

}

void
MsgTalk :: create(const char* aSpeaker, const char* aHearer, const char* aEmotion,
                  const char* aWords, Channel aChannel, uint32_t aColor)
{
    ASSERT(aSpeaker != nullptr && aSpeaker[0] != '\0');
    ASSERT(aHearer != nullptr && aHearer[0] != '\0');
    ASSERT(aEmotion != nullptr);
    ASSERT(aWords != nullptr && aWords[0] != '\0');

    if (strlen(aSpeaker) < MAX_NAMESIZE &&
        strlen(aHearer) < MAX_NAMESIZE &&
        strlen(aEmotion) < MAX_NAMESIZE &&
        strlen(aWords) < MAX_WORDSSIZE)
    {
        mInfo->Header.Length = mLen;
        mInfo->Header.Type = MSG_TALK;

        mInfo->Color = aColor;
        mInfo->Channel = (uint16_t)aChannel;
        mInfo->Style = (int16_t)STYLE_NORMAL;
        mInfo->Timestamp = timeGetTime();

        StringPacker packer(mInfo->Buf);
        packer.addString(aSpeaker);
        packer.addString(aHearer);
        packer.addString(aEmotion);
        packer.addString(aWords);
    }
    else
    {
        LOG(ERROR, "Invalid length: hearer=%zu, speaker=%zu, emotion=%zu, words=%zu",
            strlen(aHearer), strlen(aSpeaker), strlen(aEmotion), strlen(aWords));
    }
}

void
MsgTalk :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);

    Client& client = *aClient;
    Player& player = *client.getPlayer();

    char speaker[MAX_NAMESIZE];
    char hearer[MAX_NAMESIZE];
    char words[MAX_WORDSSIZE];

    StringPacker packer(mInfo->Buf);
    packer.getString(speaker, sizeof(speaker), 0);
    packer.getString(hearer, sizeof(hearer), 1);
    packer.getString(words, sizeof(words), 3);

    // commands
    if (words[0] == '/')
    {
        if (true) // TODO: Real substring check
        {
            int mapId, x, y;
            int type;
            int param;
            if (sscanf(words, "/mm %d %d %d", &mapId, &x, &y) == 3)
            {
                player.setMapId(mapId);
                player.setPosition(x, y);

                MsgAction msg(&player, player.getMapId(), MsgAction::ACTION_ENTER_MAP);
                client.send(&msg);
            }
            else if (sscanf(words, "/action %d %d", &type, &param))
            {
                MsgAction msg(&player, param, (MsgAction::Action)type);
                client.send(&msg);
            }
            else if (sscanf(words, "/item %d %d", &type, &param))
            {
                int data[2];
                data[0] = type;
                data[1] = param;
                MsgItemInfo msg(data, MsgItemInfo::ACTION_ADD_ITEM);
                client.send(&msg);
            }
            else
            {
                player.sendSysMsg("Invalid syntax: /mm {map} {x} {y}");
            }
        }
        return;
    }

    // TODO...
    switch (mInfo->Channel)
    {
    default:
        fprintf(stdout, "%s said %s to %s\n", speaker, words, hearer);
        break;
    }
}

void
MsgTalk :: swap(uint8_t* aBuf)
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->Color = bswap32(info->Color);
    info->Channel = bswap16(info->Channel);
    info->Style = bswap16(info->Style);
    info->Timestamp = bswap32(info->Timestamp);
}
