# None of the rules below will actually work right now.
# Just copied what they provided in the PDF (I kept the exact lines in the rule "csl3_build" below) and made some adjustments.
# I believe there are mistakes (like eventManagerTests*.c which does not much the file they provided).
# Will eventually check in forums/FAQ for any updates.
# ============================================================

event_manager: event_manager.o
	gcc -std=c99 -o event_manager -Wall -pedantic-errors -Werror -DNDEBUG event_manager.o tests/eventManagerTests*.c -L. -lpriority_queue

priority_queue: priority_queue.o
	gcc -std=c99 -o priority_queue -Wall -pedantic-errors -Werror -DNDEBUG priority_queue.o tests/pq_example_test.c mtm_pq/*.c 

event_manager.o: event_manager.c
	gcc -std=c99 -o event_manager.o -Wall -pedantic-errors -Werror -DNDEBUG *.c -L. -lpriority_queue

priority_queue.o: priority_queue.c
	gcc -std=c99 -o priority_queue.o -Wall -pedantic-errors -Werror -DNDEBUG mtm_pq/*.c 

# ============================================================

# These are copied from the exercise's PDF, they want us to make sure that these exact compile commands work on the server.
csl3_build:
	gcc -std=c99 -o event_manager -Wall -pedantic-errors -Werror -DNDEBUG *.c tests/eventManagerTests*.c -L. -lpriority_queue
	gcc -std=c99 -o priority_queue -Wall -pedantic-errors -Werror -DNDEBUG tests/pq_example_test.c mtm_pq/*.c 

# PHONY means that the rule name "clean" doesn't correspond to a file's name
.PHONY: clean

clean:
	rm *.o
	rm event_manager
	rm priority_queue