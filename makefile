GCC_FLAGS = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG
CC = gcc
LINKAGE_OPTIONS = -L. -lpriority_queue

event_manager: tests/event_manager_example_tests.c event_manager.o event.o member.o date.o
	$(CC) $(GCC_FLAGS) $(LINKAGE_OPTIONS) -o $@ $^

priority_queue: tests/pq_example_tests.c priority_queue.o
	$(CC) $(GCC_FLAGS) -o $@ $^

%.o: %.c
	$(CC) $(GCC_FLAGS) -c -o $@ $<

clean:
	rm -f *.o
	rm -f event_manager
	rm -f priority_queue

clean_all: clean
	rm -rf test_out
	rm -f Submission.zip

submission:
	rm -f Submission.zip
	zip -j Submission.zip dry/dry.pdf event_manager.c date.c member.h member.c event.h event.c priority_queue.c makefile  

test: test_priority_queue test_event_manager 

test_priority_queue: priority_queue
	./priority_queue
	valgrind --track-origins=yes --leak-check=full ./priority_queue

test_event_manager: event_manager
	./event_manager
	valgrind --track-origins=yes --leak-check=full ./event_manager