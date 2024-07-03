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

#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/Lua/Types.h>
#include <stdio.h>

#include "main.hpp"
#include "support.hpp"

using namespace GarrysMod::Lua;

namespace StripperLua {

    static const char *tablename = "stripper";


    LUA_FUNCTION_STATIC( GetOriginalMapEntities ) {

        if (StripperSupport::originalMapEntities != nullptr) {
            LUA->PushString( StripperSupport::originalMapEntities );
        } else {
            LUA->PushNil();
        }

        return 1;
    }

    LUA_FUNCTION_STATIC( GetNewMapEntities ) {

        if (StripperSupport::newMapEntities != nullptr) {
            LUA->PushString( StripperSupport::newMapEntities );
        } else {
            LUA->PushNil();
        }

        return 1;
    }

    void Initialize(ILuaBase* LUA){

        LUA->CreateTable( );

        LUA->PushString( "gmsv_stripper: 0.0.1" );
        LUA->SetField( -2, "Version" );

        // version num follows LuaJIT style, xx.yy.zz
        LUA->PushNumber( 2 );
        LUA->SetField( -2, "VersionNum" );

        LUA->PushCFunction( GetOriginalMapEntities );
        LUA->SetField( -2, "GetOriginalMapEntities" );

        LUA->PushCFunction( GetNewMapEntities );
        LUA->SetField( -2, "GetNewMapEntities" );

        LUA->SetField( GarrysMod::Lua::INDEX_GLOBAL, tablename );


    }

    void Uninitialize(ILuaBase* LUA){} 
}