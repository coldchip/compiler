#include "macro.h"

char *preprocess(char *dir, char *input) {
	StringBuilder *sb = sb_create();
	while(*input != '\0') {
		char include_tag[] = "#include";
		if(strncmp(input, include_tag, strlen(include_tag)) == 0) {
			input += strlen(include_tag);
			while(is_space(*input)) input++;
			if(*input == '"'){
				input++;
				assert_not_eof(input);
				StringBuilder *sb_name = sb_create();
				sb_appendf(sb_name, "%s/", dir);
				while(*input != '"') {
					sb_appendf(sb_name, "%c", *input);
					assert_not_eof(input);
					input++;
				}
				input++;

				char *name = sb_concat(sb_name);
				sb_free(sb_name);
				char *include_file = read_file_into_buffer(name);
				char *preprocessed_input = preprocess(dir, include_file);
				sb_appendf(sb, "%s", preprocessed_input);
				free(include_file);
				free(preprocessed_input);
				free(name);
			} else {
				c_error("Unable to preprocess");
			}
		}
		sb_appendf(sb, "%c", *input);
		input++;
	}
	char *output = sb_concat(sb);
	sb_free(sb);
	return output;
}