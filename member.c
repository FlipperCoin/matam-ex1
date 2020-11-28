#include <stdlib.h>
#include <string.h>

#include "member.h"

struct Member_t{
    int id;
    char const *name;
};

Member memberCreate(int member_id, char* member_name) {
    if (member_name == NULL) {
        return NULL;
    }

    Member member = (Member)malloc(sizeof(struct Member_t));
    if (member == NULL) return NULL;

    member->id = member_id;
    char *member_name_copy = (char*)malloc(strlen(member_name)+1);
    strcpy(member_name_copy, member_name);
    member->name = (char const *)member_name_copy;
    return member;
}

void memberDestroy(Member member) {
    free((char*)member->name);
    free(member);
}

int memberGetId(Member member) {
    if (member == NULL) {
        return BAD_ID;
    }

    return member->id;
}

MemberResult memberSetId(Member member, int id) {
    if (member == NULL) {
        return M_NULL_ARGUMENT;
    }

    if (id < 0) {
        return M_INVALID_MEMBER_ID;
    }

    member->id = id;
    return M_SUCCESS;
}

char const* memberGetName(Member member) {
    if (member == NULL) {
        return NULL;
    }

    return member->name;
}

MemberResult memberSetName(Member member, char const* name) {
    if (member == NULL || name == NULL) {
        return M_NULL_ARGUMENT;
    }

    size_t name_length = strlen(name);
    if (name_length == 0) {
        return M_INVALID_MEMBER_NAME;
    }

    char *name_copy = malloc((name_length+1)*sizeof(char));
    if (name_copy == NULL) {
        return M_OUT_OF_MEMORY;
    }
    strcpy(name_copy, name);
    free((char*)member->name);
    member->name = (char const *)name_copy;

    return M_SUCCESS;
}