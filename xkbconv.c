#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <xkbcommon/xkbcommon.h>
#include <wchar.h>
#include <locale.h>



struct utf8_to_keycode_data {
	char ch;
};

void utf8_to_keycode(struct xkb_keymap *keymap, xkb_keycode_t key, void *data);

int main(int argc, char **argv)
{
	int opt;
	int option_index;
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

	setlocale(LC_ALL, "");

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

	wint_t wc;
	while((wc = fgetwc(stdin)) != WEOF)
	{
	}

	return EXIT_SUCCESS;
}

void utf8_to_keycode(struct xkb_keymap *keymap, xkb_keycode_t key, void *data)
{
}

//void keymap_iter(struct xkb_keymap *keymap, xkb_keycode_t key, void *data)
//{
//	char *buffer;
//	int size;
//	struct xkb_state *state;
//
//	state = xkb_state_new(keymap);
//	if (!state) return;
//
//	size = xkb_state_key_get_utf8(state,key,NULL,0) + 1;
//	buffer = malloc(size);
//	xkb_state_key_get_utf8(state, key, buffer, size);
//
//	printf("%s: %s\n", xkb_keymap_key_get_name(keymap, key), buffer);
//}
//