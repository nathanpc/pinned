// Credits: http://stackoverflow.com/a/6782480/126353

#ifndef BASE64_H_
#define BASE64_H_

class Base64 {
	public:
		static char *encode(const char *data, size_t input_length);
		static char *decode(const char *data, size_t input_length, size_t *output_length);

		static void build_decoding_table();
		static void cleanup();
};

#endif
