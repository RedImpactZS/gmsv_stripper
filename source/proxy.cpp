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

#include <detouring/classproxy.hpp>
#include <GarrysMod/Lua/Interface.h>

#include "eiface.h"

#include "support.hpp"
#include "main.hpp"

namespace StripperProxy {

    class IServerGameDLLProxy : public Detouring::ClassProxy<IServerGameDLL, IServerGameDLLProxy> {
    public:

        IServerGameDLLProxy(IServerGameDLL* server_gamedll) {
            Initialize(server_gamedll);

            if (!Hook(&IServerGameDLL::LevelInit, &IServerGameDLLProxy::LevelInit))
                throw std::runtime_error("[gmsv_stripper] failed to hook IServerGameDLL::LevelInit");
        }

        ~IServerGameDLLProxy() {
            UnHook(&IServerGameDLL::LevelInit);
        }

        // Called any time a new level is started (after GameInit() also on level transitions within a game)
        virtual bool LevelInit( char const *pMapName,
                                        char const *pMapEntities, char const *pOldLevel,
                                        char const *pLandmarkName, bool loadGame, bool background ) {

            StripperSupport::originalMapEntities = pMapEntities;

            const char *ents = StripperSupport::parse_map(pMapName, pMapEntities);

            StripperSupport::newMapEntities = ents;

            return Call(&IServerGameDLL::LevelInit,pMapName,ents,pOldLevel,pLandmarkName,loadGame,background);
        };

    };

    class IVEngineServerProxy : public Detouring::ClassProxy<IVEngineServer, IVEngineServerProxy> {
    public:

        IVEngineServerProxy(IVEngineServer* server_gamedll) {
            Initialize(server_gamedll);

            if (!Hook(&IVEngineServer::GetMapEntitiesString, &IVEngineServerProxy::GetMapEntitiesString))
                throw std::runtime_error("[gmsv_stripper] failed to hook IVEngineServer::GetMapEntitiesString");
        }

        ~IVEngineServerProxy() {
            UnHook(&IVEngineServer::GetMapEntitiesString);
        }

        // Called any time a new level is started (after GameInit() also on level transitions within a game)
        virtual const char* GetMapEntitiesString() {

            if (StripperSupport::newMapEntities != nullptr) {
                return StripperSupport::newMapEntities;
            }

            return Call(&IVEngineServer::GetMapEntitiesString);
        };

    };

    std::shared_ptr<StripperProxy::IServerGameDLLProxy> gamedll_proxy;
    std::shared_ptr<IVEngineServerProxy> engine_proxy;

    int Initialize(){

        if (engine_proxy == nullptr) {
            engine_proxy = std::make_shared<IVEngineServerProxy>(engine);
            Msg("[gmsv_stripper] Created proxy for IVEngineServer\n");
        } else {
            Msg("[gmsv_stripper] Already proxied IVEngineServer\n");
        }

        if (gamedll_proxy == nullptr) {
            gamedll_proxy = std::make_shared<IServerGameDLLProxy>(gamedll);
            Msg("[gmsv_stripper] Created proxy for IServerGameDLL\n");
        } else {
            Msg("[gmsv_stripper] Already proxied IServerGameDLL\n");
            return 1;
        }

        return 0;
    }

    void Uninitialize(){}
}