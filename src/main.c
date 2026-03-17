#include <stdio.h>
#include <string.h>
#include "data.h"
#include "args/builder.h"

int test(void)
{
    profile_t profile = lf_profile("testfiles/test.dmp");
    engine_spec_t engine = lf_engine_spec("testfiles/test.dme");
    config_t config = lf_config("testfiles/test.dconf");

    builder_t b;
    builder_init(&b);

    if (!builder_total(&b, &config, &profile, &engine))
    {
        printf("build failed\n");
        return 1;
    }

    for (size_t i = 0; i < b.entries; i++)
    {
        printf("[%zu] %s\n", i, b.argv[i]);
    }
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "test") == 0)
    {
        return test();
    }

    return 0;
}