/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "log.h"
#include "mapmanager.h"
#include "gamemap.h"
#include "mapdata.h"
#include "finder.h"
#include <stdio.h>
#include <QSettings>
#include <QStringList>

using namespace std;

/* static */
MapManager* MapManager::sInstance = nullptr;

/* static */
MapManager&
MapManager :: getInstance()
{
    // TODO? Thread-safe
    if (sInstance == nullptr)
    {
        sInstance = new MapManager();
    }
    return *sInstance;
}

MapManager :: MapManager()
{

}

MapManager :: ~MapManager()
{
    for (map<int32_t, GameMap*>::iterator
            it = mGameMaps.begin(), end = mGameMaps.end();
         it != end; ++it)
    {
        GameMap* map = it->second;
        SAFE_DELETE(map);
    }
    mGameMaps.clear();


    for (map<string, MapData*>::iterator
            it = mData.begin(), end = mData.end();
         it != end; ++it)
    {
        MapData* data = it->second;
        SAFE_DELETE(data);
    }
    mMaps.clear(); // refs to data objects...
    mData.clear();
}

err_t
MapManager :: loadData()
{
    ASSERT_ERR(mGameMaps.empty() && mMaps.empty() && mData.empty(), ERROR_INVALID_STATE);

    err_t err = ERROR_SUCCESS;

    const char* path = "./GameMap.ini";
    if (Finder::fileExists(path))
    {
        QSettings gamemap(path, QSettings::IniFormat);
        QStringList groups = gamemap.childGroups();

        for (QStringList::const_iterator
                it = groups.begin(), end = groups.end();
             ERROR_SUCCESS == err && it != end; ++it)
        {
            const QString& group = *it;
            unsigned int mapId = 0;

            if (sscanf(qPrintable(group), "Map%u", &mapId) == 1)
            {
                if (mMaps.find((uint16_t)mapId) == mMaps.end())
                {
                    string dataPath = gamemap.value(group + "/File", "N/A").toString().toStdString();
                    if (dataPath != "N/A")
                    {
                        map<string, MapData*>::const_iterator data_it;
                        if ((data_it = mData.find(dataPath)) == mData.end())
                        {
                            MapData* data = nullptr;
                            err = MapData::load(&data, dataPath.c_str());

                            if (IS_SUCCESS(err))
                            {
                                ASSERT_ERR(data != nullptr, ERROR_INVALID_POINTER);

                                LOG(INFO, "Loaded map data at '%s' for id=%u.",
                                    dataPath.c_str(), mapId);

                                mData[dataPath] = data;
                                mMaps[(uint16_t)mapId] = data;
                                data = nullptr;
                            }
                            else if (ERROR_FILE_NOT_FOUND == err)
                            {
                                LOG(ERROR, "Could not find all files for loading the map data file '%s'. Ignoring error.",
                                    dataPath.c_str());
                                err = ERROR_SUCCESS;
                            }
                            SAFE_DELETE(data);
                        }
                        else
                        {
                            LOG(INFO, "Found already loaded map data for id=%u.",
                                mapId);
                            mMaps[(uint16_t)mapId] = data_it->second;
                        }
                    }
                    else
                    {
                        LOG(WARN, "Could not find the key 'File' under the group '%s'. Skipping",
                            qPrintable(group));
                    }
                }
                else
                {
                    LOG(ERROR, "Duplicated entry for map %u.", mapId);
                    err = ERROR_CANNOT_CREATE;
                }
            }
            else
            {
                LOG(WARN, "Found an invalid group (%s) at root. Skipping.",
                    qPrintable(group));
            }
        }
    }
    else
    {
        LOG(ERROR, "Could not find the '%s' file for loading maps.", path);
        err = ERROR_FILE_NOT_FOUND;
    }

    return err;
}

err_t
MapManager :: createMap(int32_t aUID, GameMap::Info** aInfo)
{
    ASSERT_ERR(aInfo != nullptr && *aInfo != nullptr, ERROR_INVALID_POINTER);

    err_t err = ERROR_SUCCESS;

    if (mGameMaps.find(aUID) == mGameMaps.end())
    {
        map<uint16_t, MapData*>::const_iterator it;
        if ((it = mMaps.find((*aInfo)->DocID)) != mMaps.end())
        {
            GameMap* gameMap = new GameMap(aUID, aInfo, *it->second);
            ASSERT(gameMap != nullptr);

            mGameMaps[aUID] = gameMap;

            LOG(INFO, "Created game map %d with data of %u.",
                gameMap->getUID(), gameMap->getDocID());
        }
        else
        {
            LOG(ERROR, "Missing map data for doc ID %u. The map %d won't be created.",
                (*aInfo)->DocID, aUID);
            err = ERROR_NOT_FOUND;
        }
    }
    else
    {
        LOG(ERROR, "Duplicated map %d.", aUID);
        err = ERROR_CANNOT_CREATE;
    }

    return err;
}

GameMap*
MapManager :: getMap(int32_t aUID)
{
    GameMap* gameMap = nullptr;

    map<int32_t, GameMap*>::iterator it;
    if ((it = mGameMaps.find(aUID)) == mGameMaps.end())
    {
        gameMap = it->second;
    }

    return gameMap;
}
