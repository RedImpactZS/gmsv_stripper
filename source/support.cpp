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

#include <new>
#include <stdio.h>

#if defined WIN32
#include <windows.h>
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include <GarrysMod/Lua/Interface.h>

#include "parser.hpp"

#if defined WIN32
#define ALT_SEP_CHAR		'/'
#define PATH_SEP_CHAR		'\\'
#else
#define PATH_SEP_CHAR		'/'
#define ALT_SEP_CHAR		'\\'
#endif

#define GAME_PATH "garrysmod"
#define STRIPPER_PATH "addons/stripper"

namespace StripperSupport {

	namespace {
		size_t UTIL_FormatArgs(char *buffer, size_t maxlength, const char *fmt, va_list params)
		{
			size_t len = vsnprintf(buffer, maxlength, fmt, params);

			if (len >= maxlength)
			{
				len = maxlength - 1;
				buffer[len] = '\0';
			}

			return len;
		}

		size_t PathFormat(char *buffer, size_t len, const char *fmt, ...)
		{
			va_list ap;
			va_start(ap, fmt);
			size_t mylen = UTIL_FormatArgs(buffer, len, fmt, ap);
			va_end(ap);

			for (size_t i = 0; i < mylen; i++)
			{
				if (buffer[i] == ALT_SEP_CHAR)
				{
					buffer[i] = PATH_SEP_CHAR;
				}
			}

			return mylen;
		}

		static void
		path_format(char* buffer, size_t maxlength, const char* fmt, ...)
		{
			va_list ap;
			char new_buffer[1024];

			va_start(ap, fmt);
			vsnprintf(new_buffer, sizeof(new_buffer), fmt, ap);
			va_end(ap);

			new_buffer[sizeof(new_buffer) - 1] = '\0';

			PathFormat(buffer, maxlength, "%s", new_buffer);
		}
	}

	Stripper g_Stripper;

    const char* originalMapEntities;
    const char* newMapEntities;

	const char*
	parse_map(const char* map, const char* entities)
	{
		FILE* fp;
		char path[256];

		g_Stripper.SetEntityList(entities);

		path_format(path,
			sizeof(path),
			"%s/%s/global_filters.cfg",
			GAME_PATH,
			STRIPPER_PATH);
		fp = fopen(path, "rt");
		if (fp == NULL)
		{
			Msg("[gmsv_stripper] Could not find global filter file: %s\n", path);
		}
		else
		{
			fclose(fp);
			g_Stripper.ApplyFileFilter(path);
			Msg("[gmsv_stripper] Global filter rule applied: %s\n", path);
		}

		for (const char* pPart = map; *pPart++ != '\0';)
		{
			pPart = strchrnul(pPart, '_');
			path_format(path, sizeof(path), "%s/%s/maps/%.*s.cfg",
				GAME_PATH, STRIPPER_PATH, pPart - map, map);
			fp = fopen(path, "rt");

			if (fp)
			{
				fclose(fp);
				g_Stripper.ApplyFileFilter(path);
				Msg("[gmsv_stripper] Filter rule applied: %s\n", path);
			} else {
				Msg("[gmsv_stripper] Could not find filter file: %s\n", path);
			}
		}

		return g_Stripper.ToString();
	}

}