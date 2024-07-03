/** vim: set ts=4 sw=4 et tw=99:
 *
 * === Stripper for Metamod:Source ===
 * Copyright (C) 2005-2009 David "BAILOPAN" Anderson
 * No warranties of any kind.
 * Based on the original concept of Stripper2 by botman
 *
 * License: see LICENSE.TXT
 * ===================================
 */

#include <memory>

#include <GarrysMod/InterfacePointers.hpp>
#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/Lua/Types.h>

#include "proxy.hpp"
#include "support.hpp"
#include "lua.hpp"

IServerGameDLL *gamedll = nullptr;
IServer *server = nullptr;
IVEngineServer *engine = nullptr;

GMOD_MODULE_OPEN()
{

    gamedll = InterfacePointers::ServerGameDLL();
    if (gamedll == nullptr) {
      throw std::runtime_error(
          "[gmsv_stripper] Failed to load required IServerGameDLL interface");
    }

    server = InterfacePointers::Server();
    if (gamedll == nullptr) {
      throw std::runtime_error(
          "[gmsv_stripper] Failed to load required IServer interface");
    }

    engine = InterfacePointers::VEngineServer();
    if (engine == nullptr) {
      throw std::runtime_error(
          "[gmsv_stripper] Failed to load required IVEngineServer interface");
    }

    StripperLua::Initialize(LUA);

    if (StripperProxy::Initialize()) {
        Msg("[gmsv_stripper] Successfully loaded. Will activate on next map\n");
    } else {
        Msg("[gmsv_stripper] Already loaded, skipping...\n");
    }

    return 0;
}

GMOD_MODULE_CLOSE( )
{

    StripperProxy::Uninitialize();
    StripperLua::Uninitialize(LUA);

    Msg("[gmsv_stripper] Module close acknowledged\n");
    return 0;
}
