/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "env.h"
#include "common.h"
#include <stdlib.h>
#include <algorithm>

using namespace std;

/* static */
Environment* Environment::sInstance = nullptr;

/* static */
Environment&
Environment :: getInstance()
{
    // TODO? Thread-safe
    if (sInstance == nullptr)
    {
        sInstance = new Environment();
        ASSERT(sInstance != nullptr);
    }
    return *sInstance;
}

/* static */
void
Environment :: destroy()
{
    SAFE_DELETE(sInstance);
}

Environment :: Environment()
{
    ASSERT(sInstance == nullptr);
    mIsDestroying = false;

    atexit(Environment::destroy);
}

Environment :: ~Environment()
{
    ASSERT(this == sInstance);
    mIsDestroying = true;

    for (vector<Global*>::iterator
            it = mObjects.begin(), end = mObjects.end();
         it != end; ++it)
    {
        Global* obj = *it;
        SAFE_DELETE(obj);
    }
    mObjects.clear();

    sInstance = nullptr;
}

void
Environment :: registerObj(Global* aObj)
{
    ASSERT(aObj != nullptr);
    mObjects.push_back(aObj);
}

void
Environment :: unregisterObj(Global* aObj)
{
    ASSERT(aObj != nullptr);
    if (!mIsDestroying)
    {
        vector<Global*>::iterator it =
                find(mObjects.begin(), mObjects.end(), aObj);

        if (mObjects.end() != it)
        {
            mObjects.erase(it);
        }
    }
}

