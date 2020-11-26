#include <stdlib.h>
#include <string.h>

#include "member.h"

struct Member_t {
    int id;
    char *name;
};

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

    char *name_copy = malloc(name_length*sizeof(char));
    if (name_copy == NULL) {
        return M_OUT_OF_MEMORY;
    }
    strcpy(name_copy, name);
    member->name = name_copy;

    return M_SUCCESS;
}