#ifndef __PUSHOVER_H__
#define __PUSHOVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <assert.h>
#include "dbg.h"

#define PUSHOVER_URL "https://api.pushover.net/1/messages"

struct PMessage {
	char *user;
	char *token;
	char *body;
	char *title;    /* optional */
	char *url;      /* optional */
};

int Pushover_msg_send(
        struct PMessage *msg,
        bool verbose
);

void Pushover_msg_destroy(struct PMessage *msg);
struct PMessage *Pushover_msg_create(
        char *user,
        char *token,
        char *body,
        char *title,
        char *url
);

#endif /*__PUSHOVER_H__*/
