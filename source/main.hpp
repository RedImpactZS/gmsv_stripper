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

#ifndef _INCLUDE_STRIPPER_MAIN_H_
#define _INCLUDE_STRIPPER_MAIN_H_

class IServerGameDLL;
class IServer;
class IVEngineServer;

extern IServerGameDLL *gamedll;
extern IServer *server;
extern IVEngineServer *engine;

#endif /* _INCLUDE_STRIPPER_MAIN_H_ */