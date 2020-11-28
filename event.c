#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "event.h"

#define MEMBERS_MAX_SIZE_INIT 25
#define MEMBERS_SIZE_INCREASE 10

struct Event_t {
    int id;
    char const *name;
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
    free((char*)event->name);
    dateDestroy(event->date);
    free(event->members);
    
    free(event);
}

int eventGetId(Event event) { 
    return event->id; 
}
EventResult eventSetId(Event event, int id) { 
    if (event == NULL) {
        return E_NULL_ARGUMENT;
    }
    if (id <= 0) {
        return E_INVALID_EVENT_ID;
    }
    event->id=id;
    return E_SUCCESS; 
}
char const* eventGetName(Event event) { 
    if (event == NULL) {
        return NULL;
    }
    return event->name; 
}
EventResult eventSetName(Event event, char const* name) { 
    if (event == NULL || name == NULL) {
        return E_NULL_ARGUMENT;
    }
    char const *name_copy = malloc(strlen(name));
    if (name_copy == NULL) {
        return E_OUT_OF_MEMORY;
    }
    free((char*)event->name);
    event->name = (char const *)name_copy;
    return E_SUCCESS; 
}
Date eventGetDate(Event event) {
    if (event == NULL) {
        return NULL;
    } 
    return event->date; 
}
EventResult eventSetDate(Event event, Date date) { 
    if (event == NULL || date == NULL) {
        return E_NULL_ARGUMENT;
    }
    Date date_copy = dateCopy(date);
    if (date_copy == NULL) {
        return E_ERROR;
    }
    dateDestroy(event->date);
    event->date = date_copy;
    return E_SUCCESS; 
}
int const* eventGetMembers(Event event) { 
    if (event == NULL) {
        return NULL;
    }
    return event->members; 
}
int eventGetMembersCount(Event event) { 
    if (event == NULL) {
        return -1;
    }

    return event->members_count;
}
EventResult eventSetMembers(Event event, int const* members, size_t length) { 
    if (event == NULL || members == NULL) {
        return E_NULL_ARGUMENT;
    }

    int *new_members = (int *)malloc(length * sizeof(int));
    if (new_members == NULL) {
        return E_OUT_OF_MEMORY;
    }
    memcpy(new_members, members, length * sizeof(int));
    free(event->members);
    event->members = new_members;
    event->members_count = length;
    event->members_max_size = length;
    return E_SUCCESS;
}
EventResult eventAddMember(Event event, int member_id) {
    if (event == NULL) {
        return E_NULL_ARGUMENT;
    }
    if (member_id <= 0) {
        return E_INVALID_MEMBER_ID;
    }
    if(event->members_count == event->members_max_size) {
        int *reallocated_members = reallocarray(event->members, event->members_count + MEMBERS_SIZE_INCREASE, sizeof(int));
        if (reallocated_members == NULL) {
            return E_OUT_OF_MEMORY;
        }
        event->members_max_size += MEMBERS_SIZE_INCREASE;
    }

    event->members[event->members_count++] = member_id;
    return E_SUCCESS;
}
EventResult eventRemoveMember(Event event, int member_id) {
    if (event == NULL) {
        return E_NULL_ARGUMENT;
    }
    for (size_t i = 0; i < event->members_count; i++)
    {
        if (event->members[i] == member_id) {
            for (size_t j = i; j < (event->members_count-1); j++)
            {
                event->members[j] = event->members[j+1];
            }
            // my way of "deleting" the last member
            // just a matter of safety, decreasing member_count is what matters
            event->members[event->members_count-1] = -1;
            event->members_count--;
            return E_SUCCESS;
        }
    }
    return E_SUCCESS;
}

