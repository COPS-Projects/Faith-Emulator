/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _FAITH_EMULATOR_MAPMANAGER_H_
#define _FAITH_EMULATOR_MAPMANAGER_H_

#include "common.h"
#include "env.h"
#include "gamemap.h"
#include <map>

class MapData;

class MapManager : public Environment::Global
{
public:
    /**
     * Get the MapManager singleton. If the object does not exist yet,
     * it will be created.
     *
     * @returns A reference to the singleton
     */
    static MapManager& getInstance();

public:
    /* destructor */
    virtual ~MapManager();

    /**
     * Load all data from the data map files.
     *
     * @retval ERROR_SUCCESS on success
     * @returns Error code otherwise
     */
    err_t loadData();

    /**
     * Create a new game map with the specified information.
     *
     * @param[in]  aUID     the unique ID of the map
     * @param[in]  aInfo    a reference to the object containing the info
     *
     * @retval ERROR_SUCCESS on success
     * @returns Error code otherwise
     */
    err_t createMap(int32_t aUID, GameMap::Info** aInfo);

    /**
     * Create a dynamic map based on another map.
     *
     * @param[in]   aUID       the unique ID of the map
     * @param[in]   aRefUID    the unique ID of the reference map
     *
     * @retval ERROR_SUCCESS on success
     * @returns Error code otherwise
     */
    err_t linkMap(int32_t aUID, int32_t aRefUID);

public:
    /**
     * Get a game map based on its UID.
     *
     * @param[in]   aUID  the map UID
     *
     * @retval The game map object if found
     * @returns NULL otherwise
     */
    GameMap* getMap(int32_t aUID);

private:
    /* constructor */
    MapManager();

private:
    static MapManager* sInstance; //!< static instance of the singleton

private:
    std::map<int32_t, GameMap*> mGameMaps; //!< all game maps
    std::map<uint16_t, MapData*> mMaps; //!< all map data based on the UID
    std::map<std::string, MapData*> mData; //!< all map data based on the file
};

#endif // _FAITH_EMULATOR_MAPMANAGER_H_
