#include <stdlib.h>
#include <string.h>

#include "event.h"

#define MEMBERS_MAX_SIZE_INIT 25

struct Event_t {
    int id;
    char *name;
    Date date;
    int *members;
    int members_count;
    int members_max_size;
};

Event eventCreate(char* event_name, Date date, int event_id) {
    Event event = (Event)malloc(sizeof(struct Event_t));
    if (event == NULL) {
        return NULL;
    }

    char* name = (char*)malloc(strlen(event_name)+1);
    if (name == NULL) {
        return NULL;
    }
    strcpy(name, event_name);
    event->name=name;

    Date event_date = dateCopy(date);
    if (event_date == NULL) {
        return NULL;
    }
    event->date=event_date;

    int *members = (int*)malloc(MEMBERS_MAX_SIZE_INIT * sizeof(int));
    if (members == NULL) {
        return NULL;
    }
    event->members = members;

    event->id = event_id;

    return event;
}

void eventDestroy(Event event) {
    free(event->name);
    dateDestroy(event->date);
    free(event->members);
    
    free(event);
}

int eventGetId(Event event) { return -1; };
EventResult eventSetId(Event event, int id) { return E_ERROR; };
char const* eventGetName(Event event) { return NULL; };
EventResult eventSetName(Event event, char const* name) { return E_ERROR; };
Date eventGetDate(Event event) { return NULL; };
EventResult eventSetDate(Event event, Date date) { return E_ERROR; };
int const* eventGetMembers(Event event) { return NULL; };
int eventGetMembersCount(Event event) { return -1; };
EventResult eventSetMembers(Event event, int const* members) { return E_ERROR; };

