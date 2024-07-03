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

#pragma once

#ifndef _INCLUDE_STRIPPER_LUA_H_
#define _INCLUDE_STRIPPER_LUA_H_

namespace GarrysMod
{
	namespace Lua
	{
		class ILuaBase;
	}
}

namespace StripperLua {

    extern int Initialize(GarrysMod::Lua::ILuaBase* LUA);
    extern int Deinitialize(GarrysMod::Lua::ILuaBase* LUA);

};

#endif /* _INCLUDE_STRIPPER_LUA_H_ */