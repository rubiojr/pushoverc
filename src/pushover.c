#include "pushover.h"

int pushover_send(
        const char *user,
        const char *token,
        const char *message,
        const char *title,
        const char *url,
        bool verbose
)
{
	CURL *curl;
	CURLcode res;

	if (!user || strlen(user) != 30) {
		log_err("Invalid user string size (expected 30)");
		return -1;
	}

	if (!token || strlen(token) != 30) {
		log_err("Invalid key/token size (expected 30)");
		return -1;
	}

	if (!message || strlen(message) > 1024) {
		log_err("Invalid message or message too large (max 1024)");
		return -1;
	}

	if (title && strlen(title) > 250) {
		log_err("Invalid message or message too large");
		return -1;
	}

	curl = curl_easy_init();
	curl_global_init(CURL_GLOBAL_ALL);

	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;

	if (title) {
		curl_formadd(&formpost,
		             &lastptr,
		             CURLFORM_COPYNAME, "title",
		             CURLFORM_COPYCONTENTS, title,
		             CURLFORM_END);
	}

	if (url) {
		curl_formadd(&formpost,
		             &lastptr,
		             CURLFORM_COPYNAME, "url",
		             CURLFORM_COPYCONTENTS, url,
		             CURLFORM_END);
	}

	curl_formadd(&formpost,
	             &lastptr,
	             CURLFORM_COPYNAME, "message",
	             CURLFORM_COPYCONTENTS, message,
	             CURLFORM_END);

	curl_formadd(&formpost,
	             &lastptr,
	             CURLFORM_COPYNAME, "token",
	             CURLFORM_COPYCONTENTS, token,
	             CURLFORM_END);

	curl_formadd(&formpost,
	             &lastptr,
	             CURLFORM_COPYNAME, "user",
	             CURLFORM_COPYCONTENTS, user,
	             CURLFORM_END);

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, PUSHOVER_URL);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.35.0");
		if (verbose)
			curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

		res = curl_easy_perform(curl);

		if(res != CURLE_OK)
			fprintf(stderr, "Error sending notification: %s\n",
			        curl_easy_strerror(res));

		curl_formfree(formpost);
		curl_easy_cleanup(curl);
	}

	return 0;
}
