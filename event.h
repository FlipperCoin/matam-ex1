#ifndef EVENT_H
#define EVENT_H

#include "date.h"

typedef struct Event_t* Event;

typedef enum EventResult_t {
    E_SUCCESS,
    E_OUT_OF_MEMORY,
    E_NULL_ARGUMENT,
    E_INVALID_EVENT_ID,
    E_INVALID_MEMBER_ID,
    E_MEMBER_ALREADY_EXISTS,
    E_MEMBER_NOT_EXISTS,
    E_ERROR
} EventResult;

Event eventCreate(char* event_name, Date date, int event_id);
void eventDestroy(Event event);

int eventGetId(Event event);
EventResult eventSetId(Event event, int id);
char const* eventGetName(Event event);
EventResult eventSetName(Event event, char const* name);
Date eventGetDate(Event event);
EventResult eventSetDate(Event event, Date date);
int const* eventGetMembers(Event event);
EventResult eventSetMembers(Event event, int const* members, size_t length);
int eventGetMembersCount(Event event);
EventResult eventAddMember(Event event, int member_id);
EventResult eventRemoveMember(Event event, int member_id);
#endif // EVENT_H