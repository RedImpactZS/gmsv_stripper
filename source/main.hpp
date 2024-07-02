#include "eiface.h"

IServerGameDLL *gamedll;
IServer *server;

// parent project will call into these functions for results.
const char *GetMapEntitiesString_handler();
void Stripper_SetCommandClient(int client);
// ----------

char *UTIL_ToLowerCase(const char *str);