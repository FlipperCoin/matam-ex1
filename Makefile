GCC_FLAGS = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG

event_manager: tests/event_manager_tests.c event_manager.o event.o member.o date.o
	gcc $(GCC_FLAGS) -o event_manager $^ -L. -lpriority_queue

priority_queue: priority_queue.o
	gcc $(GCC_FLAGS) -o priority_queue priority_queue.o tests/pq_example_test.c mtm_pq/*.c 

date: tests/date_tests.c date.o
	gcc $(GCC_FLAGS) -o date $^ -L.

%.o: %.c
	gcc $(GCC_FLAGS) -c -o $@ $< -L. -lpriority_queue

priority_queue.o: priority_queue.c
	gcc $(GCC_FLAGS) -c -o priority_queue.o mtm_pq/*.c 

# These are copied from the exercise's PDF, they want us to make sure that these exact compile commands work on the server.
csl3_build:
	gcc -std=c99 -o event_manager -Wall -pedantic-errors -Werror -DNDEBUG *.c tests/eventManagerTests*.c -L. -lpriority_queue
	gcc -std=c99 -o priority_queue -Wall -pedantic-errors -Werror -DNDEBUG tests/pq_example_test.c mtm_pq/*.c 

# PHONY means that these rule names don't correspond to a file's name
.PHONY: clean test test_priority_queue test_date test_event_manager

clean:
	rm *.o
	rm event_manager
	rm priority_queue

test: test_priority_queue test_date test_event_manager 

test_priority_queue: priority_queue
	./priority_queue
	valgrind --track-origins=yes --leak-check=full ./priority_queue

test_date: date
	./date
	valgrind --track-origins=yes --leak-check=full ./date

test_event_manager: event_manager
	./event_manager
	valgrind --track-origins=yes --leak-check=full ./event_manager