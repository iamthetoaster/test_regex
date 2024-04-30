#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))
#define MIN(X, Y)       (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y)       (((X) > (Y)) ? (X) : (Y))

char const *const COLORS[] = { "\033[36m", "\033[31m", "\033[32m", "\033[33m",
    "\033[34m", "\033[35m" };

char const *const RESET = "\033[0m";
char const *const UNDERLINE = "\033[4m";
char const *const BOLD = "\033[1m";

size_t const N_COLORS = ARRAY_SIZE(COLORS);

#ifdef DEBUG_MODE
#define dbg_log(...)                                                           \
    __debug_logging_function(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

void __debug_logging_function(
    char const *file, size_t line, char const *func, char const *format, ...);

#else
#define dbg_log(...)
#endif

void display_match(regex_t regex, size_t nmatch, regmatch_t pmatch[], char *s);

int main(int argc_i, char **argv) {
    // TODO Proper erroring
    // Also tell users how to use the program
    size_t argc = argc_i;
    regex_t regex;
    if (regcomp(&regex, argv[1], REG_EXTENDED | REG_NEWLINE)) {
        return EXIT_FAILURE;
    }
    dbg_log("regex string: `%s`", argv[1]);

    size_t nmatch = 1;
    bool in_paren = false;
    char *end = strchr(argv[1], '\0');
    for (char *c = argv[1]; c < end; c++) {
        if (!in_paren && *c == '(') {
            in_paren = true;
            continue;
        }
        if (in_paren && *c == ')') {
            in_paren = false;
            nmatch++;
            continue;
        }
    }
    dbg_log("nmatch: %d\n", nmatch);
    regmatch_t *pmatch = calloc(sizeof(regmatch_t), nmatch);

    for (size_t i = 2; i < argc; i++) {
        display_match(regex, nmatch, pmatch, argv[i]);
    }

    regfree(&regex);
    free(pmatch);
    dbg_log("End of program!\n");
    return 0;
}

void display_match(regex_t regex, size_t nmatch, regmatch_t pmatch[], char *s) {
    if (regexec(&regex, s, nmatch, pmatch, 0)) {
        printf("%s%s%s NO MATCH%s\n", s, BOLD, COLORS[1], RESET);
        return;
    }
    regmatch_t entire = pmatch[0];
    pmatch[0].rm_so = entire.rm_eo;
    pmatch[0].rm_eo = entire.rm_so;
    printf("%s", RESET);
    printf("%.*s", (int) entire.rm_so, s);
    size_t curr_index = 0;
    // while (curr_index > 0 && pmatch[curr_index].rm_so == -1) {
    //     curr_index--;
    // }
    regmatch_t curr = pmatch[curr_index];
    for (size_t next_index = 1; next_index <= nmatch; next_index++) {
        int curr_len = MAX(0, (int) (curr.rm_eo - curr.rm_so));
        printf("%s%s%.*s", COLORS[curr_index % N_COLORS], UNDERLINE, curr_len,
            s + curr.rm_so);

        while (next_index <= nmatch && pmatch[next_index].rm_so == -1) {
            next_index++;
        }
        regmatch_t next = pmatch[next_index % nmatch];

        int between_len = MAX(0, (int) (next.rm_so - curr.rm_eo));
        printf("%s%s%.*s", RESET, UNDERLINE, between_len, s + curr.rm_eo);

        curr = next;
        curr_index = next_index;
    }
    printf("%s%s\n", RESET, s + entire.rm_eo);
}

#ifdef DEBUG_MODE
void __debug_logging_function(
    char const *file, size_t line, char const *func, char const *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "Logging from function `%s`, on line `%zu` of file `%s`:\n",
        func, line, file);
    vfprintf(stderr, format, args);
}

#endif
