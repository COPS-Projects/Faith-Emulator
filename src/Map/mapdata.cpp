/**
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "log.h"
#include "mapdata.h"
#include "finder.h"
#include "binaryreader.h"

using namespace std;

struct Cell;

/* static */
err_t
MapData :: load(MapData** aOutData, const char* aPath)
{
    ASSERT_ERR(aOutData != nullptr && *aOutData == nullptr, ERROR_INVALID_PARAMETER);
    ASSERT_ERR(aPath != nullptr && aPath[0] != '\0', ERROR_INVALID_PARAMETER);

    err_t err = ERROR_SUCCESS;

    MapData* data = new MapData();

    if (Finder::fileExists(aPath))
    {
        BinaryReader reader(aPath);
        DOIF(err, reader.lock());

        DOIF(err, data->loadMapData(reader));

        reader.unlock();
        reader.close();
    }
    else
    {
        LOG(ERROR, "Can't load the DMap file at %s as the file doesn't exist.",
            aPath);
        err = ERROR_FILE_NOT_FOUND;
    }

    if (IS_SUCCESS(err))
    {
        *aOutData = data;
        data = nullptr;
    }
    SAFE_DELETE(data);

    return err;
}

MapData :: MapData()
    : mWidth(0), mHeight(0), mCells(nullptr)
{

}

MapData :: ~MapData()
{
    SAFE_DELETE_ARRAY(mCells);

    for (vector<Passage*>::iterator
            it = mPassages.begin(), end = mPassages.end();
         it != end; ++it)
    {
        Passage* passage = *it;
        SAFE_DELETE(passage);
    }
}


err_t
MapData :: loadMapData(BinaryReader& aReader)
{
    ASSERT_ERR(&aReader != nullptr, ERROR_INVALID_REFERENCE);

    err_t err = ERROR_SUCCESS;

    uint32_t version = 0, data = 0, width = 0, height = 0;
    DOIF(err, aReader.readUInt32(version));
    DOIF(err, aReader.readUInt32(data));

    // skipping puzzle file...
    DOIF(err, aReader.seek(sizeof(char) * MAX_PATH, SEEK_CUR));

    DOIF(err, aReader.readUInt32(width));
    DOIF(err, aReader.readUInt32(height));

    LOG(INFO, "TQ Digital DMap : v%u (data=%u) with [%u, %u] cells.",
        version, data, width, height);

    if (IS_SUCCESS(err))
    {
        if (width < UINT16_MAX && height < UINT16_MAX)
        {
            mWidth = (uint16_t)width;
            mHeight = (uint16_t)height;
        }
        else
        {
            LOG(ERROR, "Size overflow for the map.");
            err = ERROR_BAD_LENGTH;
        }
    }

    mCells = new Cell[mWidth * mHeight];
    for (uint16_t y = 0; ERROR_SUCCESS == err && y < mHeight; ++y)
    {
        uint32_t checksum = 0, tmp = 0;
        for (uint16_t x = 0; ERROR_SUCCESS == err && x < mWidth; ++x)
        {
            uint16_t mask = 0, terrain = 0;
            int16_t altitude = 0;

            DOIF(err, aReader.readUInt16(mask));
            DOIF(err, aReader.readUInt16(terrain));
            DOIF(err, aReader.readInt16(altitude));

            checksum += (uint32_t)((mask * (terrain + y + 1)) +
                                   ((altitude + 2) * (x + 1 + terrain)));

            Cell& cell = mCells[pos2idx(x, y)];
            cell.Accessible = mask != FALSE;
            cell.Altitude = altitude;
        }
        DOIF(err, aReader.readUInt32(tmp));

        if (checksum != tmp)
        {
            LOG(ERROR, "Invalid checksum for the block of cells for the position y=%u", y);
            err = ERROR_BAD_CHECKSUM;
        }
    }

    DOIF(err, loadPassageData(aReader));
    if (version == 1003)
        DOIF(err, loadRegionData(aReader));
    DOIF(err, loadLayerData(aReader));

    //The rest are LAYER_SCENE, but useless for a server. I'll not implement the rest as it would only
    //slow down the loading.

    return err;
}

err_t
MapData :: loadPassageData(BinaryReader& aReader)
{
    ASSERT_ERR(&aReader != nullptr, ERROR_INVALID_REFERENCE);

    err_t err = ERROR_SUCCESS;

    int32_t count = 0;
    DOIF(err, aReader.readInt32(count));
    mPassages.reserve(count);

    LOG(INFO, "Found %d passages for the map.", count);

    for (int32_t i = 0; ERROR_SUCCESS == err && i < count; ++i)
    {
        Passage* passage = new Passage();
        uint32_t posX = 0, posY = 0;

        DOIF(err, aReader.readUInt32(posX));
        DOIF(err, aReader.readUInt32(posY));
        DOIF(err, aReader.readInt32(passage->Index));

        if (IS_SUCCESS(err))
        {
            if (posX < UINT16_MAX && posX < UINT16_MAX)
            {
                passage->PosX = (uint16_t)posX;
                passage->PosY = (uint16_t)posY;
            }
            else
            {
                LOG(ERROR, "Size overflow for the position of the passage.");
                err = ERROR_BAD_LENGTH;
            }
        }

        if (IS_SUCCESS(err))
        {
            LOG(VRB, "Added passage %d at (%u, %u).",
                passage->Index, passage->PosX, passage->PosY);

            mPassages.push_back(passage);
            passage = nullptr;
        }
        SAFE_DELETE(passage);
    }

    return err;
}

err_t
MapData :: loadRegionData(BinaryReader& aReader)
{
    ASSERT_ERR(&aReader != nullptr, ERROR_INVALID_REFERENCE);

    err_t err = ERROR_SUCCESS;

    int32_t count = 0;
    DOIF(err, aReader.readInt32(count));

    LOG(INFO, "Found %d regions for the map.", count);

    if (IS_SUCCESS(err))
    {
        LOG(ERROR, "Regions are not supported yet.");
        err = ERROR_INVALID_FUNCTION;
    }

    return err;
}

err_t
MapData :: loadLayerData(BinaryReader& aReader)
{
    ASSERT_ERR(&aReader != nullptr, ERROR_INVALID_REFERENCE);

    err_t err = ERROR_SUCCESS;

    int32_t count = 0;
    DOIF(err, aReader.readInt32(count));

    LOG(INFO, "Found %d layers for the map.", count);

    for (int32_t i = 0; ERROR_SUCCESS == err && i < count; ++i)
    {
        int32_t type = MAP_NONE;
        DOIF(err, aReader.readInt32(type));

        switch (type)
        {
        case MAP_COVER: // 2DMapCoverObj
            {
                //Do nothing with it...
                LOG(VRB, "Found a 2D map cover object layer. Skipping.");
                DOIF(err, aReader.seek(416, SEEK_CUR));
                break;
            }
        case MAP_TERRAIN: // 2DMapTerrainObj
            {
                // A class could be good... But useless for a server implementation.

                char fileName[MAX_PATH];
                uint32_t startX = 0, startY = 0;
                int32_t count = 0;

                DOIF(err, aReader.read(fileName, sizeof(fileName)));
                DOIF(err, aReader.readUInt32(startX));
                DOIF(err, aReader.readUInt32(startY));

                LOG(VRB, "Found a 2D map terrain object at (%u, %u). Loading scene file '%s'",
                    startX, startY, fileName);

                // TODO: Normalize path to / instead of Windows...

                if (Finder::fileExists(fileName))
                {
                    BinaryReader reader(fileName);
                    DOIF(err, reader.lock());

                    DOIF(err, reader.readInt32(count));
                    LOG(VRB, "Found %d parts.", count);

                    // the server only need the new cells info, so it will be merged
                    // and the objects will be deleted...
                    // it is useless to parse most of the parts info...
                    for (int32_t i = 0; ERROR_SUCCESS == err && i < count; ++i) // all parts
                    {
                        uint32_t width = 0, height = 0;
                        uint32_t sceneOffsetX = 0, sceneOffsetY;
                        DOIF(err, reader.seek(256, SEEK_CUR)); // AniFile
                        DOIF(err, reader.seek(64, SEEK_CUR)); // AniTitle
                        DOIF(err, reader.seek(sizeof(uint32_t), SEEK_CUR)); // PosOffset(X)
                        DOIF(err, reader.seek(sizeof(uint32_t), SEEK_CUR)); // PosOffset(Y)
                        DOIF(err, reader.seek(sizeof(uint32_t), SEEK_CUR)); // AniInterval
                        DOIF(err, reader.readUInt32(width));
                        DOIF(err, reader.readUInt32(height));
                        DOIF(err, reader.seek(sizeof(int32_t), SEEK_CUR)); // Thick
                        DOIF(err, reader.readUInt32(sceneOffsetX));
                        DOIF(err, reader.readUInt32(sceneOffsetY));
                        DOIF(err, reader.seek(sizeof(int32_t), SEEK_CUR)); // Height

                        for (uint32_t y = 0; ERROR_SUCCESS == err && y < height; ++y)
                        {
                            for (uint32_t x = 0; ERROR_SUCCESS == err && x < width; ++x)
                            {
                                uint32_t mask = 0, terrain = 0;
                                int32_t altitude = 0;

                                DOIF(err, reader.readUInt32(mask));
                                DOIF(err, reader.readUInt32(terrain));
                                DOIF(err, reader.readInt32(altitude));

                                size_t posX = ((startX + sceneOffsetX) + x) - width;
                                size_t posY = ((startY + sceneOffsetY) + y) - height;
                                if (posX < UINT16_MAX && posX < UINT16_MAX)
                                {
                                    Cell& cell = mCells[pos2idx((uint16_t)posX, (uint16_t)posY)];
                                    cell.Accessible = mask != FALSE;
                                    cell.Altitude = altitude;
                                }
                                else
                                {
                                    LOG(ERROR, "Size overflow for the position of the scene part.");
                                    err = ERROR_BAD_LENGTH;
                                }
                            }
                        }
                    }

                    reader.unlock();
                    reader.close();
                }
                else
                {
                    LOG(ERROR, "Can't load the scene file at '%s'' as the file doesn't exist.",
                        fileName);
                    err = ERROR_FILE_NOT_FOUND;
                }

                break;
            }
        case MAP_SOUND: // MapSound
            {
                //Do nothing with it...
                LOG(VRB, "Found a map sound layer. Skipping.");
                DOIF(err, aReader.seek(276, SEEK_CUR));
                break;
            }
        case MAP_3DEFFECT: // 3DMapEffect
            {
                //Do nothing with it...
                LOG(VRB, "Found a 3D map effect layer. Skipping.");
                DOIF(err, aReader.seek(72, SEEK_CUR));
                break;
            }
        case MAP_3DEFFECTNEW: // 3DMapEffectNew
            {
                //Do nothing with it...
                LOG(VRB, "Found a 3D map effect (new) layer. Skipping.");
                DOIF(err, aReader.seek(96, SEEK_CUR));
                break;
            }
        default:
            {
                LOG(VRB, "Found a layer of type %d at offset %lld, which is unknown.",
                    type, aReader.tell());
                err = ERROR_BAD_FORMAT;
                break;
            }
        }
    }

    return err;
}
