#include "pushover.h"

struct PMessage *Pushover_msg_create(
        char *user,
        char *token,
        char *body,
        char *title,
        char *url
)
{
	struct PMessage *msg = malloc(sizeof(struct PMessage));
	assert(msg != NULL);
	msg->title = msg-> url = 0;

	if (!user || strlen(user) != 30) {
		log_err("Invalid user string size (expected 30)");
		return NULL;
	}

	if (!token || strlen(token) != 30) {
		log_err("Invalid key/token size (expected 30)");
		return NULL;
	}

	if (!body || strlen(body) > 1024) {
		log_err("Invalid message or message too large (max 1024)");
		return NULL;
	}

	if (title && strlen(title) > 250) {
		log_err("Invalid message or message too large");
		return NULL;
	}

	msg->user  = strdup(user);
	msg->token = strdup(token);
	msg->body  = strdup(body);

	if (title)
		msg->title = strdup(title);

	if (url)
		msg->url = strdup(url);

	return msg;
}

void Pushover_msg_destroy(struct PMessage *msg)
{
	assert(msg != NULL);

	free(msg->title);
	free(msg->url);
	free(msg->token);
	free(msg->user);
	free(msg->body);

	free(msg);
}

int Pushover_msg_send(
        struct PMessage *msg,
        bool verbose
)
{
	CURL *curl;
	CURLcode res;
	int rval = 0;

	curl = curl_easy_init();
	curl_global_init(CURL_GLOBAL_DEFAULT);

	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;

	if (msg->title) {
		curl_formadd(&formpost,
		             &lastptr,
		             CURLFORM_COPYNAME, "title",
		             CURLFORM_COPYCONTENTS, msg->title,
		             CURLFORM_END);
	}

	if (msg->url) {
		curl_formadd(&formpost,
		             &lastptr,
		             CURLFORM_COPYNAME, "url",
		             CURLFORM_COPYCONTENTS, msg->url,
		             CURLFORM_END);
	}

	curl_formadd(&formpost,
	             &lastptr,
	             CURLFORM_COPYNAME, "message",
	             CURLFORM_COPYCONTENTS, msg->body,
	             CURLFORM_END);

	curl_formadd(&formpost,
	             &lastptr,
	             CURLFORM_COPYNAME, "token",
	             CURLFORM_COPYCONTENTS, msg->token,
	             CURLFORM_END);

	curl_formadd(&formpost,
	             &lastptr,
	             CURLFORM_COPYNAME, "user",
	             CURLFORM_COPYCONTENTS, msg->user,
	             CURLFORM_END);

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, PUSHOVER_URL);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.35.0");
		if (verbose)
			curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

		res = curl_easy_perform(curl);

		if(res != CURLE_OK) {
			fprintf(stderr, "Error sending notification: %s\n",
			        curl_easy_strerror(res));
			rval = -1;
		}

		curl_formfree(formpost);
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	return rval;
}
