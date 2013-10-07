/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _FAITH_EMULATOR_NETWORK_DEF_H_
#define _FAITH_EMULATOR_NETWORK_DEF_H_

#include "common.h"

const size_t MAX_NAMESIZE = 16;
const size_t MAX_NAMESIZE_CLI = 32;
const size_t MAX_WORDSSIZE = 255;
const size_t MAX_PARAMSIZE = 255;

const uint16_t MSG_NONE = 0;
const uint16_t MSG_GENERAL = 1000;

const uint16_t MSG_REGISTER         = MSG_GENERAL + 1;
const uint16_t MSG_LOGIN            = MSG_GENERAL + 2;
const uint16_t MSG_LOGOUT           = MSG_GENERAL + 3;
const uint16_t MSG_TALK             = MSG_GENERAL + 4;
const uint16_t MSG_WALK             = MSG_GENERAL + 5;
const uint16_t MSG_USERINFO         = MSG_GENERAL + 6;
const uint16_t MSG_ATTACK           = MSG_GENERAL + 7;
const uint16_t MSG_ITEMINFO         = MSG_GENERAL + 8;
const uint16_t MSG_ITEM             = MSG_GENERAL + 9;
const uint16_t MSG_ACTION           = MSG_GENERAL + 10;
const uint16_t MSG_ACCIDENT         = MSG_GENERAL + 11;
const uint16_t MSG_TICK             = MSG_GENERAL + 12;
const uint16_t MSG_ROOM             = MSG_GENERAL + 13;
const uint16_t MSG_PLAYER           = MSG_GENERAL + 14;
const uint16_t MSG_NAME             = MSG_GENERAL + 15;
const uint16_t MSG_WEATHER          = MSG_GENERAL + 16;
const uint16_t MSG_USERATTRIB       = MSG_GENERAL + 17;
const uint16_t MSG_ROLE             = MSG_GENERAL + 18;
const uint16_t MSG_FRIEND           = MSG_GENERAL + 19;
const uint16_t MSG_EFFECT           = MSG_GENERAL + 20;
const uint16_t MSG_QUIZGAME         = MSG_GENERAL + 21;
const uint16_t MSG_INTERACT         = MSG_GENERAL + 22;
const uint16_t MSG_TEAM             = MSG_GENERAL + 23;
const uint16_t MSG_ALLOT            = MSG_GENERAL + 24;
const uint16_t MSG_WEAPONSKILL      = MSG_GENERAL + 25;
const uint16_t MSG_TEAMMEMBER       = MSG_GENERAL + 26;
const uint16_t MSG_GEMEMBED         = MSG_GENERAL + 27;

const uint16_t MSG_ACCOUNT          = MSG_GENERAL + 51;
const uint16_t MSG_CONNECT          = MSG_GENERAL + 52;
const uint16_t MSG_TRADE            = MSG_GENERAL + 56;

const uint16_t MSG_BATTLESYSTEM     = MSG_GENERAL + 57;

const uint16_t MSG_LOGINREPLYEX     = MSG_GENERAL + 55;
const uint16_t MSG_LOGINACCOUNTEX   = MSG_GENERAL + 90;

const uint16_t MSG_MAPITEM          = MSG_GENERAL + 101;
const uint16_t MSG_PACKAGE          = MSG_GENERAL + 102;
const uint16_t MSG_MAGICINFO        = MSG_GENERAL + 103;
const uint16_t MSG_FLUSHEXP         = MSG_GENERAL + 104;
const uint16_t MSG_MAGICEFFECT      = MSG_GENERAL + 105;
const uint16_t MSG_SYNATTRINFO      = MSG_GENERAL + 106;
const uint16_t MSG_SYNDICATE        = MSG_GENERAL + 107;
const uint16_t MSG_ITEMINFOEX       = MSG_GENERAL + 108;
const uint16_t MSG_NPCINFOEX        = MSG_GENERAL + 109;
const uint16_t MSG_MAPINFO          = MSG_GENERAL + 110;
const uint16_t MSG_MESSAGEBOARD     = MSG_GENERAL + 111;
const uint16_t MSG_SYNMEMBERINFO    = MSG_GENERAL + 112;
const uint16_t MSG_DICE             = MSG_GENERAL + 113;
const uint16_t MSG_SYNINFO          = MSG_GENERAL + 114;

const uint16_t MSG_NPCINFO          = MSG_GENERAL + 1030;
const uint16_t MSG_NPC              = MSG_GENERAL + 1031;
const uint16_t MSG_DIALOG           = MSG_GENERAL + 1032;
const uint16_t MSG_FRIENDINFO       = MSG_GENERAL + 1033;
const uint16_t MSG_TIME             = MSG_GENERAL + 1034;
const uint16_t MSG_PETINFO          = MSG_GENERAL + 1035;
const uint16_t MSG_DATAARRAY        = MSG_GENERAL + 1036;
const uint16_t MSG_EUDEMONATTRIB    = MSG_GENERAL + 1037;
const uint16_t MSG_SCHOOLMEMBER     = MSG_GENERAL + 1038;
const uint16_t MSG_PLAYERTASK       = MSG_GENERAL + 1039;
const uint16_t MSG_TASK_LIST        = MSG_GENERAL + 1040;
const uint16_t MSG_ANNOUNCE_LIST    = MSG_GENERAL + 1041;
const uint16_t MSG_ANNOUNCE_INFO    = MSG_GENERAL + 1042;
const uint16_t MSG_AUCTION          = MSG_GENERAL + 1043;
const uint16_t MSG_CHATROOM         = MSG_GENERAL + 1044;
const uint16_t MSG_ITEMATTRIB       = MSG_GENERAL + 1045;

#endif // _FAITH_EMULATOR_NETWORK_DEF_H_
