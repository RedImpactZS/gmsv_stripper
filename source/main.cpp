#include <GarrysMod/InterfacePointers.hpp>
#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/Lua/Types.h>

#include "eiface.h"

#include <detouring/classproxy.hpp>

#include "pcre.h"

#include "support.h"
#include "iserver.h"

#if defined WIN32
#define ALT_SEP_CHAR		'/'
#define PATH_SEP_CHAR		'\\'
#else
#define PATH_SEP_CHAR		'/'
#define ALT_SEP_CHAR		'\\'
#endif

IServerGameDLL *gamedll = nullptr;
IServer *server = nullptr;

static std::string g_mapname;
static stripper_core_t stripper_core;

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

const char*
get_map_name()
{
    return server->GetMapName();
}

stripper_game_t stripper_game =
{
    NULL,
    NULL,
    NULL,
    NULL,
    path_format,
    get_map_name,
};

char*
UTIL_ToLowerCase(const char *str)
{
	size_t len = strlen(str);
	char *buffer = new char[len + 1];
	for (size_t i = 0; i < len; i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			buffer[i] = str[i] - ('A' - 'a');
		else
			buffer[i] = str[i];
	}
	buffer[len] = '\0';
	return buffer;
}

const char*
GetMapEntitiesString_handler()
{
    return stripper_core.ent_string();
}

class IServerGameDLLProxy : public Detouring::ClassProxy<IServerGameDLL, IServerGameDLLProxy> {
public:

    IServerGameDLLProxy(IServerGameDLL* server_gamedll) {
        Initialize(server_gamedll);

        if (!Hook(&IServerGameDLL::LevelInit, &IServerGameDLLProxy::LevelInit))
            throw std::runtime_error("failed to hook IServerGameDLL::LevelInit");
    }

    ~IServerGameDLLProxy() {
        UnHook(&IServerGameDLL::LevelInit);
    }

	// Called any time a new level is started (after GameInit() also on level transitions within a game)
	virtual bool LevelInit( char const *pMapName, 
									char const *pMapEntities, char const *pOldLevel, 
									char const *pLandmarkName, bool loadGame, bool background ) {

        g_mapname.assign(pMapName);

        const char *ents = stripper_core.parse_map(g_mapname.c_str(), pMapEntities);

        return Call(&IServerGameDLL::LevelInit,pMapName,ents,pOldLevel,pLandmarkName,loadGame,background);                              
    };

};

std::shared_ptr<IServerGameDLLProxy> gamedll_detour;

GMOD_MODULE_OPEN( )
{

    stripper_game.game_path = "garrysmod";
    stripper_game.stripper_path = "addons/stripper";
    stripper_game.stripper_cfg_path = stripper_game.stripper_path;

    LoadStripper(&stripper_game, &stripper_core);

    gamedll = InterfacePointers::ServerGameDLL();
    if (gamedll == nullptr) {
      throw std::runtime_error(
          "failed to load required IServerGameDLL interface");
    }

    server = InterfacePointers::Server();
    if (gamedll == nullptr) {
      throw std::runtime_error(
          "failed to load required IServerGameDLL interface");
    }

    if (gamedll_detour == nullptr) {
        gamedll_detour = std::make_shared<IServerGameDLLProxy>(gamedll);
        Msg("gmsv_stripper: Loaded. PCRE version: %s\n", pcre_version());
    } else {
        Msg("gmsv_stripper: Skipping load already hooked\n");
    }
    
    return 0;
}

GMOD_MODULE_CLOSE( )
{
    //gamedll_detour.reset();
    Msg("gmsv_stripper: Kept loaded\n");
    return 0;
}
