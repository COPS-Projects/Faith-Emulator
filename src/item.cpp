/*
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "item.h"
#include "monster.h"
#include "basefunc.h"
#include "database.h"
#include <algorithm> // min, max

using namespace std;

/* static */
bool
Item :: createItem(Item** aOutItem, uint32_t aValue, Monster& aMonster, uint8_t aQuality /* = 0 */)
{
    uint8_t quality = 0;

    // item quality
    if (aQuality == 0)
    {
        // auto create this param
        int rand = random(0, 100);
        if (rand >= 0 && rand < 30)
            quality = 5;
        else if (rand >= 30 && rand < 70)
            quality = 4;
        else
            quality = 3;
    }
    else
        quality = aQuality;

    // item sort & item color
    uint16_t sort = 0;
    uint8_t color = 0;
    uint8_t lev = 0;

    int rand = random(0, 1200);
    if (rand >= 0 && rand < 20)
    {
        // shoes
        sort = 160;
        lev = aMonster.getDropShoes();
    }
    else if (rand >= 20 && rand < 50)
    {
        const uint16_t sorts[2] = { 120, 121 }; // taoist bag & necklace

        // necklace
        sort = sorts[random(0, 1)];
        lev = aMonster.getDropNecklace();
    }
    else if (rand >= 50 && rand < 100)
    {
        const uint16_t sorts[3] = { 150, 151, 152 };

        // ring
        sort = sorts[random(0, 2)];
        lev = aMonster.getDropRing();
    }
    else if (rand >= 100 && rand < 400)
    {
        const uint16_t sorts[6] = { 111, 112, 113, 114, 117, 118 };

        // armet
        sort = sorts[random(0, 5)];
        lev = aMonster.getDropArmet();

        if (sort <= 118) // have color
            color = random(0, 6) + 3; // 3 - 9
    }
    else if (rand >= 400 && rand < 700)
    {
        const uint16_t sorts[5] = { 130, 131, 132, 133, 134 };

         // armor
        sort = sorts[random(0, 4)];
        lev = aMonster.getDropArmor();
        color = random(0, 6) + 3; // 3 - 9
    }
    else
    {	// weapon & shield
        int rate = random(0, 100);
        if (rate >= 0 && rate < 20) // 20% for sword of Taoist
        {
            sort = 421;
        }
        else if (rate >= 20 && rate < 40) // 20% for archer
        {
            sort = 500;
        }
        else if (rate >= 40 && rate < 80) // 40% for single hand weapon
        {
            const uint16_t sorts[10] = { 410, 420, 421, 430, 440, 450, 460, 480, 481, 490 };

            sort = sorts[random(0, 9)];
        }
        else // 20% for two hand weapon
        {
            const uint16_t sorts[6] = { 510, 530, 560, 561, 580, 900 };

            sort = sorts[random(0, 5)];
        }

        if (sort == 900) // shield
        {
            color = random(0, 6) + 3; // 3 - 9
            lev = aMonster.getDropShield();
        }
        else // weapon
        {
            lev = aMonster.getDropWeapon();
        }
    }


    // item lev
    if (lev == 99)
        return false;

    {
        int rate = random(0, 100);
        if (rate < 50) // 50% down one lev
        {
            lev = random(0, lev / 2) + (lev / 3);

            if (lev >= 1)
                --lev;
        }
        else if (rate >= 80) // 20% up one lev
        {
            if ((sort >= 110 && sort <= 114) ||
                (sort >= 130 && sort <= 134) ||
                (sort >= 900 && sort <= 999))
            {
                // item with color
                lev = min(lev + 1, 9);
            }
            else
            {
                lev = min(lev + 1, 23);
            }
        }

        // 50% do nothing
    }


    // item type
    uint32_t itemtype = (sort * 1000) + (color * 100) + (lev * 10) + quality;

    static const Database& db = Database::getInstance(); // singleton
    const Item::Info* info = nullptr;

    if (!IS_SUCCESS(db.getItemInfo(&info, itemtype)))
        return false;

    // TODO : generate an unique ID
    Item* item = new Item(0, *info);

    item->mAmountLimit = info->AmountLimit * randomRate(0.3);
    if (item->mAmountLimit < 1)
        item->mAmountLimit = 1;

    if (quality > 5)
        item->mAmount = item->mAmountLimit * ((double)(50 + random(0, 50)) / 100.00);
    else
    {
        double price = info->Price;
        if (price <= 0)
            price = 1;

        item->mAmount = 3 * item->mAmountLimit * ((double)aValue / price);
        if (item->mAmount >= item->mAmountLimit)
            item->mAmount = item->mAmountLimit;

        if (item->mAmount < 1)
            item->mAmount = 1;
    }

    // gem hole
    if (itemtype >= 400000 && itemtype < 599999)
    {
        // is weapon
        int rate = random(0, 100);
        if (rate < 5) // 5% got 2 holes
        {
            item->mGem1 = 0xFF;
            item->mGem2 = 0xFF;
        }
        else if (rate < 20) // 15% got 1 hole
        {
            item->mGem1 = 0xFF;
        }

        // 80% do nothing
    }

    *aOutItem = item;
    item = nullptr;

    SAFE_DELETE(item);

    return true;
}

Item :: Item(uint32_t aUID, const Item::Info& aInfo)
    : mUID(aUID), mInfo(aInfo),
      mAmount(0), mAmountLimit(0),
      mIdent(0),
      mGem1(0), mGem2(0), mMagic1(0), mMagic2(0), mBless(0), mEnchant(0), mRestrain(0),
      mLocked(false), mSuspicious(false)
{

}

Item :: ~Item()
{

}

//int CItem::GetRecoverDurCost	(void)
//{
//    int nRealMaxDur = this->GetInt(ITEMDATA_AMOUNTLIMIT)*100/(100+this->GetGemDurEffect());
//    if (nRealMaxDur >= this->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL))
//        return 0;

//    int nRepairDur = this->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL)-nRealMaxDur;
//    int nCost = 0;
//    CHECKF(this->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL));
//    switch(this->GetQuality())
//    {
//    case 9:
//        nCost = ::MulDiv(5000000, nRepairDur, this->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL));
//        nCost = __max(nCost, 500000);
//        break;

//    case 8:
//        nCost = ::MulDiv(3500000, nRepairDur, this->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL));
//        nCost = __max(nCost, 350000);
//        break;

//    default:
//        nCost = ::MulDiv(1500000, nRepairDur, this->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL));
//        nCost = __max(nCost, 150000);
//        break;
//    }

//    return __max(1, nCost);
//}
