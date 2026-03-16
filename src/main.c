#include <stdio.h>
#include <string.h>
#include "data.h"

int test(void) {
    profile_t profile = lf_profile("testfiles/profile1.dmp");
    if (profile.name == NULL) {
        return -1;
    }

    printf("name of profile: %s\n", profile.name);
    free_profile(&profile);
    return 0;
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        return test();
    }

    return 0;
}