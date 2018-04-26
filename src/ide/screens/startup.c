#include <rsge-ide/screens/startup.h>
#include <string.h>

RSGE_IDE_SCREEN_CREATE_PROTO(startup) {
    memset(screen,0,sizeof(rsge_ide_screen_t));
    return RSGE_IDE_ERROR_NONE;
}