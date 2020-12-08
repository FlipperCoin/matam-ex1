#!   /usr/bin/env bash

# These are copied from the exercise's PDF, they want us to make sure that these exact compile commands work on the server.
gcc -std=c99 -o event_manager -Wall -pedantic-errors -Werror -DNDEBUG *.c tests/eventManagerTests*.c -L. -lpriority_queue
gcc -std=c99 -o priority_queue -Wall -pedantic-errors -Werror -DNDEBUG tests/pq_example_test.c mtm_pq/*.c 