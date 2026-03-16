#include <stdio.h>
#include <string.h>
#include "data.h"

int test(void) {
    profile_t profile = lf_profile("testfiles/test.dmp");
    engine_spec_t engine_spec = lf_engine_spec("testfiles/test.dme");

    if (profile.name == NULL || engine_spec.name == NULL) {
        return -1;
    }

    printf("name of profile: %s\n", profile.name);
    printf("name of engine: %s\n", engine_spec.name);

    free_profile(&profile);
    free_engine_spec(&engine_spec);
    return 0;
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        return test();
    }

    return 0;
}