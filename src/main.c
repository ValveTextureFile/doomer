#include <stdio.h>
#include <string.h>
#include "data.h"
#include "args/builder.h"

int test(void)
{
    builder_t b;
    builder_init(&b);

    builder_push(&b, "gzdoom");
    builder_push2(&b, "-iwad", "doom2.wad");
    builder_push(&b, "-file");
    builder_push(&b, "sunlust.wad");
    builder_push2(&b, "+map", "MAP01");
    builder_finalize(&b);

    for (size_t i=0;i<b.entries;i++)
        printf("[%zu] %s\n", i, b.argv[i]);

    builder_free(&b);
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "test") == 0)
    {
        return test();
    }

    return 0;
}