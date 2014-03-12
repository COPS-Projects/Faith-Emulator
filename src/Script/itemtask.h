/*
 * ****** Faith Emulator - Closed Source ******
 * Copyright (C) 2012 - 2013 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _FAITH_EMULATOR_ITEM_TASK_H_
#define _FAITH_EMULATOR_ITEM_TASK_H_

#include "common.h"
#include "script.h"
#include <string>

class Client;
struct lua_State;

/**
 * Process a Lua script and execute the processAction when executed.
 * It is used by item's actions.
 */
class ItemTask : public Script
{
public:
    /**
     * Register the specific functions of the item's actions.
     *
     * @retval ERROR_SUCCESS on success
     * @returns Error code otherwise
     */
    static err_t registerFunctions();

public:
    /**
     * Create a new task to be used by an item.
     *
     * @param[in]  aUID    the unique ID of the script
     * @param[in]  aPath   the path of the Lua script
     */
    ItemTask(uint32_t aUID, const char* aPath);

    /* destructor */
    virtual ~ItemTask();

    /**
     * Execute the script for the specified client and parameter.
     *
     * @param[in]  aClient   the caller
     * @param[in]  aParam    the parameter
     *
     * @retval ERROR_SUCCESS on success
     * @retval ERROR_EXEC_FAILED if the Lua script failed
     * @returns Error code otherwise
     */
    virtual err_t execute(Client& aClient, int32_t aParam) const;

private:
    // MsgItem Lua methods

private:
    std::string mFct; //!< the function's name to call
};

#endif // _FAITH_EMULATOR_ITEM_TASK_H_
