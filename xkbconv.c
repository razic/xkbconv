#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <xkbcommon/xkbcommon.h>
#include <locale.h>
#include <ctype.h>

struct utf8_to_keycode_data {
	int c;
	int is_upper;
	xkb_keycode_t key;
};

char tolowerwithpunct(char c);
char toupperwithpunct(char c);
void utf8_to_keycode(struct xkb_keymap *keymap, xkb_keycode_t key, void *data);
void print_utf8_from_keycode(struct xkb_keymap *keymap, xkb_keycode_t key, void *data);

int main(int argc, char **argv)
{
	int opt;
	char *src_model;
	char *src_layout;
	char *src_variant;
	char *src_options;
	char *dst_model;
	char *dst_layout;
	char *dst_variant;
	char *dst_options;
	struct xkb_context *ctx;
	struct xkb_keymap *src_keymap;
	struct xkb_keymap *dst_keymap;

	if (setlocale(LC_ALL, "") == NULL) {
		exit(EXIT_FAILURE);
	}

	// option parsing
	static struct option long_options[] =
	{
		{"input",       required_argument, 0, 'i'},
		{"src-model",   required_argument, 0, 'M'},
		{"src-layout",  required_argument, 0, 'L'},
		{"src-variant", required_argument, 0, 'V'},
		{"src-options", required_argument, 0, 'O'},
		{"dst-model",   required_argument, 0, 'm'},
		{"dst-layout",  required_argument, 0, 'l'},
		{"dst-variant", required_argument, 0, 'v'},
		{"dst-options", required_argument, 0, 'o'},
		{0,0,0,0}
	};

	while (1)
	{
		opt = getopt_long(argc, argv, "m:l:v:o:M:L:V:O:", long_options, NULL);

		if (opt == -1)
			break;

		switch (opt)
		{
			case 'i':
				src_model = optarg;
				break;
			case 'm':
				src_model = optarg;
				break;
			case 'l':
				src_layout = optarg;
				break;
			case 'v':
				src_variant = optarg;
				break;
			case 'o':
				src_options = optarg;
				break;
			case 'M':
				dst_model = optarg;
				break;
			case 'L':
				dst_layout = optarg;
				break;
			case 'V':
				dst_variant = optarg;
				break;
			case 'O':
				dst_options = optarg;
				break;
		}
	}

	ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
	if (!ctx) return 1;

	struct xkb_rule_names src_names = {
		.rules = NULL,
		.model = src_model,
		.layout = src_layout,
		.variant = src_variant,
		.options = src_options
	};

	src_keymap = xkb_keymap_new_from_names(ctx, &src_names,
			XKB_KEYMAP_COMPILE_NO_FLAGS);
	if (!src_keymap) return 1;

	struct xkb_rule_names dst_names = {
		.rules = NULL,
		.model = dst_model,
		.layout = dst_layout,
		.variant = dst_variant,
		.options = dst_options
	};

	dst_keymap = xkb_keymap_new_from_names(ctx, &dst_names,
			XKB_KEYMAP_COMPILE_NO_FLAGS);
	if (!dst_keymap) return 1;

	int c;
	while((c = fgetc(stdin)) != EOF)
	{
		// just print the char to stdout if it's a whitespace char
		if (isspace(c)) {
			printf("%c", c);
			continue;
		}

		struct utf8_to_keycode_data data = {
			c: c,
			key: -1,
			is_upper: -1
		};

		// find the keycode for c in src keymap
		xkb_keymap_key_for_each(src_keymap, utf8_to_keycode, &data);
		// print ascii using keycode in dst keymap
		xkb_keymap_key_for_each(dst_keymap, print_utf8_from_keycode, &data);
	}

	return EXIT_SUCCESS;
}

void utf8_to_keycode(struct xkb_keymap *keymap, xkb_keycode_t key, void *data)
{
	// for some reason, some of the characters have more than one keycode,
	// err on the side of the first one found.
	if (((struct utf8_to_keycode_data *)data)->key != -1 ) return;

	struct xkb_state *state;
	state = xkb_state_new(keymap);
	if (!state) return;

	char *utf8key;
	int size;
	size = xkb_state_key_get_utf8(state, key, NULL, 0) + 1;
	if (size <= 1) return;
	utf8key = malloc(size);

	xkb_state_key_get_utf8(state, key, utf8key, size);

	int a = (int)*utf8key;
	int b = ((struct utf8_to_keycode_data *)data)->c;
	//printf("\ncomparing: %d / %d\n", a, b);
	if (a == b) {
		((struct utf8_to_keycode_data *)data)->key = key;
		//printf("\n%c: %d\n", b, key);
		return;
	} else if (a == tolowerwithpunct(b)) {
		((struct utf8_to_keycode_data *)data)->key = key;
		((struct utf8_to_keycode_data *)data)->is_upper = 1;
		//printf("\n%d\n ", key);
	}
}

void print_utf8_from_keycode(struct xkb_keymap *keymap, xkb_keycode_t key, void *data)
{
	// we're only interested in printing the given keycode
	if (key != ((struct utf8_to_keycode_data *)data)->key) return;


	struct xkb_state *state;
	state = xkb_state_new(keymap);
	if (!state) return;

	char *utf8key;
	int size;
	size = xkb_state_key_get_utf8(state, key, NULL, 0) + 1;
	if (size <= 1) return;
	utf8key = malloc(size);

	xkb_state_key_get_utf8(state, key, utf8key, size);

	if (((struct utf8_to_keycode_data *)data)->is_upper == 1) {
		printf("%c", toupperwithpunct((int)*utf8key));
	} else {
		printf("%s", utf8key);
	}
}

char tolowerwithpunct(char c) {
	if (isalpha(c) && isupper(c)) return tolower(c);
	if (ispunct(c) && c >= '{' && c <= '}') return c - 32;

	switch(c) {
		case '~': return '`';
		case '!': return '1';
		case '@': return '2';
		case '#': return '3';
		case '$': return '4';
		case '%': return '5';
		case '^': return '6';
		case '&': return '7';
		case '*': return '8';
		case '(': return '9';
		case ')': return '0';
		case '_': return '-';
		case '+': return '=';
		case '"': return '\'';
		case ':': return ';';
		case '?': return '/';
		case '>': return '.';
		case '<': return ',';
	}

	return c;
}

char toupperwithpunct(char c) {
	if (isalpha(c) && islower(c)) return toupper(c);
	if (ispunct(c) && c >= '[' && c <= ']') return c + 32;

	switch(c) {
		case '`': return '~';
		case '1': return '!';
		case '2': return '@';
		case '3': return '#';
		case '4': return '$';
		case '5': return '%';
		case '6': return '^';
		case '7': return '&';
		case '8': return '*';
		case '9': return '(';
		case '0': return ')';
		case '-': return '_';
		case '=': return '+';
		case '\'': return '"';
		case ';': return ':';
		case '/': return '?';
		case '.': return '>';
		case ',': return '<';
	}

	return c;
}
