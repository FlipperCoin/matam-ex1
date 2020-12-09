#!   /usr/bin/env bash

# These are copied from the exercise's PDF, they want us to make sure that these exact compile commands work on the server.
gcc -std=c99 -o priority_queue -Wall -pedantic-errors -Werror -DNDEBUG tests/priority_queue_tests.c <your priority queue .o files>
gcc -std=c99 -o event_manager -Wall -pedantic-errors -Werror -DNDEBUG tests/event_manager_tests.c -L. -lpriority_queue <your event manager .o files>