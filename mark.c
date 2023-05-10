#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

char *codecs[] = {
    "ldac",
    "aac",
    "aptx-hd",
    "aptx",
    "sbc",
};

#define SNK_FMT "./debian-img/snk/home/user/bttest_files/%s-dec.bin"
#define SRC_FMT "./debian-img/src/home/user/bttest_files/%s-enc.bin"

#define PASS "\033[32mPASS\033[39m\n"
#define FAIL "\033[31mFAIL\033[39m"

#define MAX_FRAMELEN 4096

typedef struct {
    char v[256];
} String;


typedef struct {
    size_t len;
    uint8_t v[MAX_FRAMELEN];
} Array;


bool arrays_equal(Array *a, Array *b) {
    if (a->len != b->len) return false;

    for (int i = 0; i < a->len; i++) {
        if (a->v[i] != b->v[i]) return false;
    }

    return true;
}

int main(void) {
    String snk_name = {};
    String src_name = {};
    Array sent = {};
    Array received = {};

    for(int i = 0; i < sizeof(codecs)/sizeof(codecs[0]); i++) {
        size_t total = 0;
        size_t good = 0;
        snprintf(snk_name.v, sizeof(snk_name), SNK_FMT, codecs[i]);
        snprintf(src_name.v, sizeof(src_name), SRC_FMT, codecs[i]);

        FILE *snk = fopen(snk_name.v, "rb");
        if (snk == NULL) {
            fprintf(stderr, "Could not open file %s: %s\n", snk_name.v, strerror(errno));
            continue;
        }

        FILE *src = fopen(src_name.v, "rb");
        if (src == NULL) {
            fprintf(stderr, "Could not open file %s: %s\n", src_name.v, strerror(errno));
            continue;
        }

        while (true) {
            size_t src_size = 0;
            size_t snk_size = 0;
            if (fread(&src_size, sizeof(src_size), 1, src) < 1) break;
            if (fread(sent.v, sizeof(sent.v[0]), src_size, src) < src_size) break;
            if (fread(&snk_size, sizeof(snk_size), 1, snk) < 1) break;
            if (fread(received.v, sizeof(received.v[0]), snk_size, snk) < snk_size) break;
            total++;
            while (snk_size != src_size) {
                if (fread(&src_size, sizeof(src_size), 1, src) < 1) break;
                if (fread(sent.v, sizeof(sent.v[0]), src_size, src) < snk_size) break;
                total++;
            }
            sent.len = src_size;
            received.len = snk_size;
            if (arrays_equal(&sent, &received)) good++;
        }

        double mark = good * 100. / (double) total;

        bool pass = mark >= 90;


        printf("Codec %s: %s", codecs[i], pass ? PASS : FAIL);

        if (!pass) {
            printf(". Details: total %zu, received %zu, lost %lf%%\n", total, good, 100 - mark);
        }

        fclose(snk);
        fclose(src);

    }

    return 0;
}
