#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>

#include "event_manager.h"
#include "event.h"
#include "member.h"
#include "priority_queue.h"

#define NO_INDEX -1
#define NO_EVENT_AMOUNT -1

#define EVENTS_MAX_SIZE_INIT 25
#define MEMBERS_MAX_SIZE_INIT 25

#define ARRAY_REALLOC_AMOUNT 10

struct EventManager_t { 
    Event *events;
    Member *members;
    size_t events_max_size;
    size_t events_count;
    size_t members_max_size;
    size_t members_count;
    Date system_date;
};

EventManager createEventManager(Date date) {
    EventManager event_manager = (EventManager)malloc(sizeof(struct EventManager_t));
    if (event_manager == NULL) {
        return NULL;
    }   

    Date system_date = dateCopy(date);
    if (system_date == NULL) {
        return NULL;
    }

    event_manager->system_date = system_date;

    Event *events = (Event*)malloc(EVENTS_MAX_SIZE_INIT*sizeof(Event));
    Member *members = (Member*)malloc(MEMBERS_MAX_SIZE_INIT*sizeof(Member));
    if (events == NULL || members == NULL) {
        return NULL;
    }

    event_manager->events = events;
    event_manager->events_count = 0;
    event_manager->events_max_size = EVENTS_MAX_SIZE_INIT;

    event_manager->members = members;
    event_manager->members_count = 0;
    event_manager->members_max_size = MEMBERS_MAX_SIZE_INIT;

    return event_manager;
}

void destroyEventManager(EventManager em) {
    for (size_t i = 0; i < em->events_count; i++)
    {
        eventDestroy(em->events[i]);
    }
    
    for (size_t i = 0; i < em->members_count; i++)
    {
        memberDestroy(em->members[i]);
    }

    free(em->events);
    free(em->members);
    free(em->system_date);
    free(em);
}

static int emFindEventByNameAndDate(EventManager em, char const *event_name, Date date) {
    for (size_t i = 0; i < em->events_count; i++)
    {
        if (strcmp(eventGetName(em->events[i]), event_name) == 0) {
            if (dateCompare(eventGetDate(em->events[i]),date) == 0) {
                return i;
            }
        }
    }
    
    return NO_INDEX;
}

static int emFindEventById(EventManager em, int event_id) {
    for (size_t i = 0; i < em->events_count; i++)
    {
        if (eventGetId(em->events[i]) == event_id) {
            return i;
        }
    }
    
    return NO_INDEX;
}

static bool isEventDateValid(EventManager em, Date date) {
    return dateCompare(date,em->system_date) >= 0;
}

static bool isEventIdValid(int event_id) {
    return event_id >= 0;
}

EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id) {
    if (em == NULL || event_name == NULL || date == NULL) {
        return EM_NULL_ARGUMENT;
    }

    if (!isEventDateValid(em, date)) {
        return EM_INVALID_DATE;
    }

    if (!isEventIdValid(event_id)) {
        return EM_INVALID_EVENT_ID;
    }

    if (emFindEventByNameAndDate(em,event_name,date) >= 0) {
        return EM_EVENT_ALREADY_EXISTS;
    }

    if (emFindEventById(em,event_id) >= 0) {
        return EM_EVENT_ID_ALREADY_EXISTS;
    }

    Event event = eventCreate(event_name,date,event_id);
    if (event == NULL) {
        return EM_OUT_OF_MEMORY;
    }

    if (em->events_count == em->events_max_size) {
        Event *reallocated_members = reallocarray(em->events, em->events_max_size + ARRAY_REALLOC_AMOUNT, sizeof(Event));
        if (reallocated_members == NULL) {
            return EM_OUT_OF_MEMORY;
        }

        em->events = reallocated_members;
        em->events_max_size += ARRAY_REALLOC_AMOUNT;
    }

    em->events[em->events_count++] = event;

    return EM_SUCCESS;
}

EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id) {
    if (em == NULL || event_name == NULL) {
        return EM_NULL_ARGUMENT;
    }

    if (days < 0) {
        return EM_INVALID_DATE;
    }

    Date event_date = dateCopy(em->system_date);
    if (event_date == NULL) {
        return EM_OUT_OF_MEMORY;
    }

    for (size_t i = 0; i < days; i++)
    {
        dateTick(event_date);
    }

    EventManagerResult result = emAddEventByDate(em, event_name, event_date, event_id);
    dateDestroy(event_date);
    return result;
}

EventManagerResult emRemoveEvent(EventManager em, int event_id) {
    if (em == NULL) {
        return EM_NULL_ARGUMENT;
    }

    if (event_id < 0) {
        return EM_INVALID_EVENT_ID;
    }

    int index = emFindEventById(em, event_id);
    if (index == NO_INDEX) {
        return EM_EVENT_NOT_EXISTS;
    }

    eventDestroy(em->events[index]);

    for (size_t i = index; i < em->events_count; i++)
    {
        if (i == em->events_count) {
            em->events[i] = NULL;
            break;
        }

        em->events[i] = em->events[i+1];
    }

    em->events_count--;

    return EM_SUCCESS;
}

EventManagerResult emChangeEventDate(EventManager em, int event_id, Date new_date) {
    if (em == NULL || new_date == NULL) {
        return EM_NULL_ARGUMENT;
    }
    
    if (!isEventDateValid(em, new_date)) {
        return EM_INVALID_DATE;
    }

    if (!isEventIdValid(event_id)) {
        return EM_INVALID_EVENT_ID;
    }

    int index = emFindEventById(em, event_id);
    if (index == NO_INDEX) {
        return EM_EVENT_ID_NOT_EXISTS;
    }

    Event event = em->events[index];

    if (emFindEventByNameAndDate(em, eventGetName(event), new_date) >= 0) {
        return EM_EVENT_ALREADY_EXISTS;
    }

    EventResult result = eventSetDate(event, new_date);
    if (result == E_OUT_OF_MEMORY) {
        return EM_OUT_OF_MEMORY;
    }

    return EM_SUCCESS;
}

static bool isMemberIdValid(int member_id) {
    return member_id >= 0;
}

static int emFindMemberById(EventManager em, int member_id) {
    for (size_t i = 0; i < em->members_count; i++)
    {
        if(memberGetId(em->members[i]) == member_id) {
            return i;
        }
    }
    
    return NO_INDEX; 
}

EventManagerResult emAddMember(EventManager em, char* member_name, int member_id) {
    if (em == NULL || member_name == NULL) {
        return EM_NULL_ARGUMENT;
    }

    if (!isMemberIdValid(member_id)) {
        return EM_INVALID_MEMBER_ID;
    }

    if (emFindMemberById(em, member_id) >= 0) {
        return EM_MEMBER_ID_ALREADY_EXISTS;
    }

    Member member = memberCreate(member_id, member_name);
    if (member == NULL) {
        return EM_OUT_OF_MEMORY;
    }

    if (em->members_count == em->members_max_size) {
        Member *reallocated_members = reallocarray(em->members, em->members_max_size + ARRAY_REALLOC_AMOUNT, sizeof(Member));
        if (reallocated_members == NULL) {
            return EM_OUT_OF_MEMORY;
        }

        em->members = reallocated_members;
        em->members_max_size += ARRAY_REALLOC_AMOUNT;
    }

    em->members[em->members_count++] = member;

    return EM_SUCCESS;
}

EventManagerResult emAddMemberToEvent(EventManager em, int member_id, int event_id) {
    if (em == NULL) {
        return EM_NULL_ARGUMENT;
    }

    if (!isMemberIdValid(member_id)) {
        return EM_INVALID_MEMBER_ID;
    }

    if (!isEventIdValid(event_id)) {
        return EM_INVALID_EVENT_ID;
    }

    int member_index = emFindMemberById(em, member_id);
    if (member_index == NO_INDEX) {
        return EM_MEMBER_ID_NOT_EXISTS;
    }

    int event_index = emFindEventById(em, event_id);
    if (event_index == NO_INDEX) {
        return EM_EVENT_ID_NOT_EXISTS;
    }

    Event event = em->events[event_index];
    
    EventResult result = eventAddMember(event, member_id);
    if (result == E_MEMBER_ALREADY_EXISTS) {
        return EM_EVENT_AND_MEMBER_ALREADY_LINKED;
    } 
    else if (result == E_OUT_OF_MEMORY) {
        return EM_OUT_OF_MEMORY;
    }

    return EM_SUCCESS;
}

EventManagerResult emRemoveMemberFromEvent(EventManager em, int member_id, int event_id) {
    if (em == NULL) {
        return EM_NULL_ARGUMENT;
    }

    if (!isMemberIdValid(member_id)) {
        return EM_INVALID_MEMBER_ID;
    }

    if (!isEventIdValid(event_id)) {
        return EM_INVALID_EVENT_ID;
    }

    // need to fail if member id is not known, no real need for the member's index
    if (emFindMemberById(em, member_id) == NO_INDEX) {
        return EM_MEMBER_ID_NOT_EXISTS;
    }

    int event_index = emFindEventById(em, event_id);
    if (event_index == NO_INDEX) {
        return EM_EVENT_ID_NOT_EXISTS;
    }

    Event event = em->events[event_index];
    
    EventResult result = eventRemoveMember(event, member_id);
    if (result == E_MEMBER_NOT_EXISTS) {
        return EM_EVENT_AND_MEMBER_NOT_LINKED;
    }

    return EM_SUCCESS;
}

static void flushOldEvents(EventManager em) {
    size_t removed = 0;
    for (size_t i = 0; i < em->events_count; i++)
    {
        Date event_date = eventGetDate(em->events[i]);
        // Bad event, removing cause there's no use in keeping it
        if (event_date == NULL || dateCompare(event_date, em->system_date) < 0) {
            removed++;
            eventDestroy(em->events[i]);
            em->events[i] = NULL;
        }
    }

    size_t new_events_count = (em->events_count - removed);

    for (size_t i = 0; i < new_events_count; i++)
    {
        if (em->events[i] != NULL) {
            continue;
        }

        int j = i+1;
        while (em->events[j] == NULL)
        {
            j++;
        }
        
        em->events[i] = em->events[j];
        em->events[j] = NULL;    
    }
    
    em->events_count = new_events_count;
}

EventManagerResult emTick(EventManager em, int days) {
    if (em == NULL) {
        return EM_NULL_ARGUMENT;
    }

    if (days <= 0) {
        return EM_INVALID_DATE;
    }

    for (size_t i = 0; i < days; i++)
    {
        dateTick(em->system_date);
    }

    flushOldEvents(em);

    return EM_SUCCESS;
}

int emGetEventsAmount(EventManager em) {
    if (em == NULL) {
        return NO_EVENT_AMOUNT;
    }

    return em->events_count;
}

char* emGetNextEvent(EventManager em) {
    if (em == NULL) {
        return NULL;
    }

    Event next_event = NULL; // NULL is returned if no events are in em
    Date next_event_date = NULL;
    for (size_t i = 0; i < em->events_count; i++)
    {
        Event event = em->events[i];
        Date event_date = eventGetDate(event);
        if (event_date == NULL) {
            // can't use this event
            continue;
        }
        if (next_event == NULL) {
            next_event = event;
            next_event_date = event_date;
        } 
        else {
            // Relies on the fact that the order in the array is the order of insertion to the system.
            // Because of the requirement that if the events are on the same day, the event that should 
            // be returned is the event that got inserted first.
            if (dateCompare(next_event_date, event_date) > 0) {
                next_event = event;
                next_event_date = event_date;
            }
        }
    }
    
    if (next_event == NULL) {
        return NULL;
    }
    char const *event_name = eventGetName(next_event);
    return (char*)event_name;
}

static int intCompare(const void * a, const void * b) {
    return (*(int*)a - *(int*)b);
}

static void sortIntArray(int *array, size_t size) {
    qsort(array, size, sizeof(int), intCompare);
}

static char* getMembersString(EventManager em, Event event) {
    int const *members = eventGetMembers(event);
    if (members == NULL) {
        return NULL;
    }

    int members_count = eventGetMembersCount(event);
    if (members_count <= 0) {
        return NULL;
    }

    // members ordered by ID
    int members_temp[(members_count * sizeof(int))];
    memcpy(members_temp, members, members_count * sizeof(int));
    sortIntArray(members_temp, members_count);
    members = (int const *)members_temp;

    char const *members_names[members_count]; 
    char *members_delimeter = ",";
    int members_string_size = 1; // '\0' terminator in the end
    
    for (size_t i = 0; i < members_count; i++)
    {
        int member_index = emFindMemberById(em, members[i]);
        if (member_index == NO_INDEX) {
            return NULL;
        }
        Member member = em->members[member_index];
        char const *member_name = memberGetName(member);
        if (member_name == NULL) {
            return NULL;
        }

        members_names[i] = member_name;
        members_string_size += strlen(member_name);
        if (i < (members_count - 1)) {
            members_string_size += strlen(members_delimeter);
        }
    }
    
    char *members_string = (char*)malloc(members_string_size * sizeof(char));
    char *members_string_end = members_string;
    for (size_t i = 0; i < members_count; i++)
    {
        sprintf(members_string_end, "%s", members_names[i]);
        members_string_end += strlen(members_names[i]);
        if (i < (members_count - 1)) {
            sprintf(members_string_end, "%s", members_delimeter);
            members_string_end += strlen(members_delimeter);
        }
        
    }
    *members_string_end = '\0';    
    return members_string;
}

void emPrintAllEvents(EventManager em, const char* file_name) {
    if (em == NULL || file_name == NULL) {
        return;
    }
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        return;
    }

    for (size_t i = 0; i < em->events_count; i++)
    {
        Event event = em->events[i];
        
        Date event_date = eventGetDate(event);
        if (event_date == NULL) {
            break;
        }
        
        int day, month, year;
        if (dateGet(event_date, &day, &month, &year)) {
            break;
        }
        
        char const *event_name = eventGetName(event);
        if (event_name == NULL) {
            break;
        }

        char *members_string = getMembersString(em, event);
        if (members_string == NULL) {
            break;
        }
        
        fprintf(file, "%s,%d.%d.%d,%s", event_name, day, month, year, members_string);
        if (i < (em->events_count - 1)) {
            fprintf(file, "\n");
        }

        free(members_string);
    }
    fclose(file);
}

typedef struct MemberEvents {
    int member_id;
    char const *member_name;
    size_t events_count;
} MemberEvents_t;

PQElement memberEventsCopy(PQElement member_events) {
    // TODO: Check if member_name should be copied aswell to a different pointer.
    // Its a pointer to const so can't change data, but the pointer can be freed which is problematic
    // dependes on pq's insert function. 
    // if it uses copy and only inserts the copy then I should copy the name pointer aswell.
    MemberEvents_t *copy = (MemberEvents_t*)malloc(sizeof(MemberEvents_t));
    *copy = *(MemberEvents_t*)member_events;
    return copy;
}

void memberEventsFree(PQElement member_events) {
    // TODO: Check if to free the member_name (comment in events copy)
    free(member_events);
}

bool memberEventsEquals(PQElement element1, PQElement element2) {
    MemberEvents_t *member_events1 = (MemberEvents_t*)element1;
    MemberEvents_t *member_events2 = (MemberEvents_t*)element2;

    if (member_events1->member_id != member_events2->member_id || 
        member_events1->events_count != member_events2->events_count) {
            return false; 
    }

    return (strcmp(member_events1->member_name, member_events2->member_name) != 0);
}

PQElementPriority memberEventsCopyPriority(PQElementPriority member_events) {
    // TODO: check if copy name
    MemberEvents_t *copy = (MemberEvents_t*)malloc(sizeof(MemberEvents_t));
    *copy = *(MemberEvents_t*)member_events;
    return copy;
}

void memberEventsFreePriority(PQElementPriority member_events) {
    // TODO: check if free name
    free(member_events);
}

int memberEventsCompare(PQElementPriority element1, PQElementPriority element2) {
    MemberEvents_t *member_events1 = (MemberEvents_t*)element1;
    MemberEvents_t *member_events2 = (MemberEvents_t*)element2;
    
    int comparison = member_events1->events_count - member_events2->events_count;
    if (comparison == 0) {
        // element2 minus element1 because lower id has greater priority
        comparison = member_events2->member_id - member_events1->member_id;
    }

    return comparison;
}

void emPrintAllResponsibleMembers(EventManager em, const char* file_name) {
    if (em == NULL || file_name == NULL) {
        return;
    }

    PriorityQueue member_events_queue = pqCreate(memberEventsCopy, memberEventsFree, memberEventsEquals, memberEventsCopyPriority, memberEventsFreePriority, memberEventsCompare);
    if (member_events_queue == NULL) {
        return;
    }

    MemberEvents_t member_events[em->members_count];
    for (size_t i = 0; i < em->members_count; i++)
    {
        Member member = em->members[i];
        int member_id = memberGetId(member);
        char const *member_name = memberGetName(member);
        // Can set an invalid id/name here if get functions fail, is OK because handled later
        member_events[i] = (MemberEvents_t) { member_id, member_name, 0 };
    }
    
    for (size_t i = 0; i < em->events_count; i++)
    {
        int const *members = eventGetMembers(em->events[i]);
        if (members == NULL) {
            continue;
        }

        int membersCount = eventGetMembersCount(em->events[i]);
        if (membersCount <= 0) {
            continue;
        }

        for (size_t j = 0; j < membersCount; j++)
        {
            int member_index = emFindMemberById(em, members[j]);
            if (member_index == NO_INDEX) {
                continue;
            }

            member_events[member_index].events_count++;
        }
    }

    for (size_t i = 0; i < em->members_count; i++)
    {
        MemberEvents_t member = member_events[i];
        if (!isMemberIdValid(member.member_id) || member.member_name == NULL || member.events_count == 0) {
            continue;
        }
        pqInsert(member_events_queue, &member_events[i], &member_events[i]);
    }
    
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        return;
    }

    PQ_FOREACH(MemberEvents_t*, member_event, member_events_queue) {
        fprintf(file, "%s,%ld", member_event->member_name, member_event->events_count);
    }       
    
    pqDestroy(member_events_queue);
    fclose(file);
}