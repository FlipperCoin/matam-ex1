#include "test_utilities.h"
#include "../event_manager.h"
#include <stdlib.h>
#include <string.h>

#define NUMBER_TESTS 1

bool test() {
    bool result = true;
    Date date = dateCreate(1,12,2020);

    ASSERT_TEST(date != NULL, teardown);

teardown:
    dateDestroy(date);
    return result;

}

bool (*tests[]) (void) = {
        test
};

const char* testNames[] = {
        "test"
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
            RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
        fprintf(stdout, "Usage: event_manager_tests <test index>\n");
        return 0;
    }

    int test_idx = strtol(argv[1], NULL, 10);
    if (test_idx < 1 || test_idx > NUMBER_TESTS) {
        fprintf(stderr, "Invalid test index %d\n", test_idx);
        return 0;
    }

    RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1]);
    return 0;
}
