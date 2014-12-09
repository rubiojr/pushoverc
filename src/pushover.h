#ifndef _db_h
#define _db_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>
#include "dbg.h"

#define PUSHOVER_URL "https://api.pushover.net/1/messages"

int pushover_send(
        const char *user,
        const char *token,
        const char *message,
        const char *title,
        const char *url,
        bool verbose
);

#endif
