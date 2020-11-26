#ifndef MEMBER_H
#define MEMBER_H

#define BAD_ID -1

typedef struct Member_t* Member;

typedef enum MemberResult_t {
    M_SUCCESS,
    M_OUT_OF_MEMORY,
    M_NULL_ARGUMENT,
    M_INVALID_MEMBER_ID,
    M_INVALID_MEMBER_NAME,
    M_ERROR
} MemberResult;

Member memberCreate(int member_id, char* member_name);
void memberDestroy(Member member);

int memberGetId(Member member);
MemberResult memberSetId(Member member, int id);
char const* memberGetName(Member member);
MemberResult memberSetName(Member member, char const* name);
#endif // MEMBER_H