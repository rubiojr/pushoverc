#include <argp.h>
#include "pushover.h"

const char *argp_program_version = "pushoverc 1.0";
const char *argp_program_bug_address = "<rubiojr@frameos.org>";

static char doc[] = "pushoverc -- tiny Pushover client";

static char args_doc[] = "message";

/* The options we understand. */
static struct argp_option options[] = {
	{"verbose",  'v', 0,       0,  "Produce verbose output" },
	{"key",      'k', "KEY",   0,  "Auth key" },
	{"title",    't', "TITLE", 0,  "Message title" },
	{"user",     'u', "USER",  0,  "Pushover user ID" },
	{"url",      'U', "URL",   0,  "URL to send with the message" },
	{ 0 }
};

struct arguments {
	char *args[1];
	int verbose;
	char *key;
	char *user;
	char *title;     /* optional */
	char *url;       /* optional */
};

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;

	switch (key) {
	case 'v':
		arguments->verbose = true;
		break;
	case 'k':
		if (!arguments->key)
			arguments->key = arg;
		break;
	case 'u':
		if (!arguments->user)
			arguments->user = arg;
		break;
	case 't':
		arguments->title = arg;
		break;
	case 'U':
		arguments->url = arg;
		break;
	case ARGP_KEY_ARG:
		if (state->arg_num >= 2)
			argp_usage (state);
		arguments->args[state->arg_num] = arg;
		break;
	case ARGP_KEY_END:
		if (state->arg_num < 1)
			argp_usage (state);
		if (!arguments->user) {
			fprintf(stderr,
			        "PUSHOVER_USER environment variable or -u <user> missing\n\n");
			argp_usage (state);
		}
		if (!arguments->key) {
			fprintf(stderr,
			        "PUSHOVER_KEY environment variable or -k <key> missing\n\n");
			argp_usage (state);
		}

		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int main (int argc, char **argv)
{
	struct arguments arguments;
	const char *env_token;
	const char *env_user;

	arguments.verbose = 0;
	arguments.user = NULL;
	arguments.key = NULL;
	arguments.title = NULL;

	if ( (env_token = getenv("PUSHOVER_KEY")) != NULL )
		arguments.key = (char *)env_token;

	if ((env_user  = getenv("PUSHOVER_USER")) != NULL )
		arguments.user = (char *)env_user;

	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	pushover_send(
	        arguments.user,
	        arguments.key,
	        arguments.args[0],
	        arguments.title,
	        arguments.url,
	        arguments.verbose
	);

	return 0;
}
