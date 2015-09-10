#define log(...) \
        {FILE *f = fopen("/data/local/tmp/log", "a+");\
        fprintf(f, __VA_ARGS__);\
        fflush(f); fclose(f); }

struct hook_t {
	unsigned int jump[3];
	unsigned int store[3];
	unsigned int orig;
	unsigned int patch;
	void *data;
};

void hook_precall(struct hook_t *h);
void hook_postcall(struct hook_t *h);
