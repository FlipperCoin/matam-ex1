#include "test_utilities.h"
#include "../event_manager.h"
#include <stdlib.h>
#include <string.h>

bool callAllFunctionsWithErrors_returnNullOrError() {
	bool result = true;
	Date date = dateCreate(6,6,2020);
	Date date2 = dateCreate(7,6,2020);
	Date invalid_date = dateCreate(5,6,2020);
	EventManager em = createEventManager(date);
	EventManager em_with_event = createEventManager(date);
	EventManager em_with_member = createEventManager(date);
	EventManager em_with_member_and_event = createEventManager(date);
	
	ASSERT_TEST(EM_SUCCESS == emAddEventByDate(em_with_event, "a", date, 1), teardown);
	ASSERT_TEST(EM_SUCCESS == emAddMember(em_with_member, "m", 1), teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDate(em_with_member_and_event, "a", date, 1), teardown);
	ASSERT_TEST(EM_SUCCESS == emAddMember(em_with_member_and_event, "m", 1), teardown);

	ASSERT_TEST(NULL == createEventManager(NULL),teardown);
	
	ASSERT_TEST(EM_NULL_ARGUMENT == emAddEventByDate(NULL, "a", date, 1),teardown);
	ASSERT_TEST(EM_NULL_ARGUMENT == emAddEventByDate(em, NULL, date, 1),teardown);
	ASSERT_TEST(EM_NULL_ARGUMENT == emAddEventByDate(em, "a", NULL, 1),teardown);
	ASSERT_TEST(EM_INVALID_EVENT_ID == emAddEventByDate(em, "a", date, -1),teardown);
	ASSERT_TEST(EM_INVALID_DATE == emAddEventByDate(em, "a", invalid_date, 1),teardown);
	ASSERT_TEST(EM_INVALID_DATE == emAddEventByDate(em, "a", invalid_date, -1),teardown);
	ASSERT_TEST(EM_INVALID_DATE == emAddEventByDate(em_with_event, "a", invalid_date, 1),teardown);
	ASSERT_TEST(EM_EVENT_ALREADY_EXISTS == emAddEventByDate(em_with_event, "a", date, 2),teardown);
	ASSERT_TEST(EM_EVENT_ID_ALREADY_EXISTS == emAddEventByDate(em_with_event, "a", date2, 1),teardown);
	ASSERT_TEST(EM_EVENT_ID_ALREADY_EXISTS == emAddEventByDate(em_with_event, "b", date, 1),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDate(em_with_event, "b", date2, 2),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDate(em_with_event, "b", date, 3),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDate(em_with_event, "a", date2, 4),teardown);
	ASSERT_TEST(EM_SUCCESS == emRemoveEvent(em_with_event, 2),teardown); // teardown
	ASSERT_TEST(EM_SUCCESS == emRemoveEvent(em_with_event, 3),teardown); // teardown
	ASSERT_TEST(EM_SUCCESS == emRemoveEvent(em_with_event, 4),teardown); // teardown
	
	ASSERT_TEST(EM_NULL_ARGUMENT == emAddEventByDiff(NULL, "a", 1, 1),teardown);
	ASSERT_TEST(EM_NULL_ARGUMENT == emAddEventByDiff(em, NULL, 1, 1),teardown);
	ASSERT_TEST(EM_INVALID_DATE == emAddEventByDiff(em, "a", -1, 1),teardown);
	ASSERT_TEST(EM_INVALID_EVENT_ID == emAddEventByDiff(em, "a", 1, -1),teardown);
	ASSERT_TEST(EM_EVENT_ALREADY_EXISTS == emAddEventByDiff(em_with_event, "a", 0, 1),teardown);
	ASSERT_TEST(EM_EVENT_ID_ALREADY_EXISTS == emAddEventByDiff(em_with_event, "a", 1, 1),teardown);
	ASSERT_TEST(EM_EVENT_ID_ALREADY_EXISTS == emAddEventByDiff(em_with_event, "b", 0, 1),teardown);
	ASSERT_TEST(EM_EVENT_ALREADY_EXISTS == emAddEventByDiff(em_with_event, "a", 0, 2),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDate(em_with_event, "b", date2, 2),teardown); // setup
	ASSERT_TEST(EM_EVENT_ALREADY_EXISTS == emAddEventByDiff(em_with_event, "a", 0, 3),teardown);
	ASSERT_TEST(EM_EVENT_ALREADY_EXISTS == emAddEventByDiff(em_with_event, "b", 1, 3),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em_with_event, "a", 1, 3),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em_with_event, "b", 0, 4),teardown);
	ASSERT_TEST(EM_SUCCESS == emRemoveEvent(em_with_event, 2),teardown); // teardown
	ASSERT_TEST(EM_SUCCESS == emRemoveEvent(em_with_event, 3),teardown); // teardown
	ASSERT_TEST(EM_SUCCESS == emRemoveEvent(em_with_event, 4),teardown); // teardown
	
	ASSERT_TEST(EM_NULL_ARGUMENT == emRemoveEvent(NULL, 1),teardown);
	ASSERT_TEST(EM_INVALID_EVENT_ID == emRemoveEvent(em, -1),teardown);
	ASSERT_TEST(EM_EVENT_NOT_EXISTS == emRemoveEvent(em, 2), teardown);	
	ASSERT_TEST(EM_EVENT_NOT_EXISTS == emRemoveEvent(em_with_event, 2), teardown);
	ASSERT_TEST(EM_SUCCESS == emRemoveEvent(em_with_event, 1), teardown);	
	ASSERT_TEST(EM_SUCCESS == emAddEventByDate(em_with_event, "a", date, 1), teardown); // teardown
	
	ASSERT_TEST(EM_NULL_ARGUMENT == emChangeEventDate(NULL, 1, date),teardown);
	ASSERT_TEST(EM_INVALID_EVENT_ID == emChangeEventDate(em, -1, date),teardown);
	ASSERT_TEST(EM_NULL_ARGUMENT == emChangeEventDate(em, 1, NULL),teardown);
	ASSERT_TEST(EM_EVENT_ID_NOT_EXISTS == emChangeEventDate(em, 1, date),teardown);
	ASSERT_TEST(EM_INVALID_DATE == emChangeEventDate(em_with_event, 1, invalid_date),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDate(em_with_event, "a", date2, 2),teardown); // setup
	ASSERT_TEST(EM_EVENT_ALREADY_EXISTS == emChangeEventDate(em_with_event, 1, date2),teardown);
	ASSERT_TEST(EM_SUCCESS == emRemoveEvent(em_with_event, 2), teardown); // teardown
	
	ASSERT_TEST(EM_NULL_ARGUMENT == emAddMember(NULL, "m", 0),teardown);
	ASSERT_TEST(EM_NULL_ARGUMENT == emAddMember(em, NULL, 0),teardown);
	ASSERT_TEST(EM_INVALID_MEMBER_ID == emAddMember(em, "m", -1),teardown);
	ASSERT_TEST(EM_MEMBER_ID_ALREADY_EXISTS == emAddMember(em_with_member, "m", 1),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddMember(em, "m", 0),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddMember(em_with_member, "m", 0),teardown);
	destroyEventManager(em);
	em = createEventManager(date); // teardown
	destroyEventManager(em_with_member);
	em_with_member = createEventManager(date); // teardown
	ASSERT_TEST(EM_SUCCESS == emAddMember(em_with_member, "m", 1), teardown); // teardown

	ASSERT_TEST(EM_NULL_ARGUMENT == emAddMemberToEvent(NULL, 1, 1),teardown);
	ASSERT_TEST(EM_INVALID_MEMBER_ID == emAddMemberToEvent(em, -1, 1),teardown);
	ASSERT_TEST(EM_INVALID_EVENT_ID == emAddMemberToEvent(em, 1, -1),teardown);
	ASSERT_TEST(EM_EVENT_ID_NOT_EXISTS == emAddMemberToEvent(em, 0, 1),teardown);
	ASSERT_TEST(EM_EVENT_ID_NOT_EXISTS == emAddMemberToEvent(em_with_member, 0, 1),teardown);
	ASSERT_TEST(EM_EVENT_ID_NOT_EXISTS == emAddMemberToEvent(em_with_member, 1, 0),teardown);
	ASSERT_TEST(EM_EVENT_ID_NOT_EXISTS == emAddMemberToEvent(em_with_member_and_event, 1, 0),teardown);
	ASSERT_TEST(EM_MEMBER_ID_NOT_EXISTS == emAddMemberToEvent(em_with_member_and_event, 0, 1),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddMemberToEvent(em_with_member_and_event, 1, 1),teardown); // setup
	ASSERT_TEST(EM_EVENT_AND_MEMBER_ALREADY_LINKED == emAddMemberToEvent(em_with_member_and_event, 1, 1),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em_with_member_and_event, "b", 0, 2),teardown); // setup
	ASSERT_TEST(EM_SUCCESS == emAddMemberToEvent(em_with_member_and_event, 1, 2),teardown);
	ASSERT_TEST(EM_EVENT_AND_MEMBER_ALREADY_LINKED == emAddMemberToEvent(em_with_member_and_event, 1, 2),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddMember(em_with_member_and_event, "m2", 2),teardown); // setup
	ASSERT_TEST(EM_SUCCESS == emAddMemberToEvent(em_with_member_and_event, 2, 1),teardown);
	ASSERT_TEST(EM_EVENT_AND_MEMBER_ALREADY_LINKED == emAddMemberToEvent(em_with_member_and_event, 2, 1),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddMemberToEvent(em_with_member_and_event, 2, 2),teardown);
	ASSERT_TEST(EM_EVENT_AND_MEMBER_ALREADY_LINKED == emAddMemberToEvent(em_with_member_and_event, 2, 2),teardown);
	destroyEventManager(em_with_member_and_event);
	em_with_member_and_event = createEventManager(date); // teardown	
	ASSERT_TEST(EM_SUCCESS == emAddEventByDate(em_with_member_and_event, "a", date, 1), teardown); // teardown
	ASSERT_TEST(EM_SUCCESS == emAddMember(em_with_member_and_event, "m", 1), teardown); // teardown

	ASSERT_TEST(EM_NULL_ARGUMENT == emRemoveMemberFromEvent(NULL, 1, 1),teardown);
	ASSERT_TEST(EM_INVALID_MEMBER_ID == emRemoveMemberFromEvent(em, -1, 1),teardown);
	ASSERT_TEST(EM_INVALID_EVENT_ID == emRemoveMemberFromEvent(em, 1, -1),teardown);
	ASSERT_TEST(EM_EVENT_ID_NOT_EXISTS == emRemoveMemberFromEvent(em, 1, 1),teardown);
	ASSERT_TEST(EM_EVENT_ID_NOT_EXISTS == emRemoveMemberFromEvent(em_with_member, 0, 1),teardown);
	ASSERT_TEST(EM_EVENT_ID_NOT_EXISTS == emRemoveMemberFromEvent(em_with_member_and_event, 1, 0),teardown);
	ASSERT_TEST(EM_MEMBER_ID_NOT_EXISTS == emRemoveMemberFromEvent(em_with_member_and_event, 0, 1),teardown);
	ASSERT_TEST(EM_EVENT_AND_MEMBER_NOT_LINKED == emRemoveMemberFromEvent(em_with_member_and_event, 1, 1),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddMemberToEvent(em_with_member_and_event, 1, 1),teardown); // setup
	ASSERT_TEST(EM_SUCCESS == emRemoveMemberFromEvent(em_with_member_and_event, 1, 1),teardown);
	ASSERT_TEST(EM_EVENT_AND_MEMBER_NOT_LINKED == emRemoveMemberFromEvent(em_with_member_and_event, 1, 1),teardown);

	ASSERT_TEST(EM_NULL_ARGUMENT == emTick(NULL, 1),teardown);
	ASSERT_TEST(EM_INVALID_DATE == emTick(em, 0),teardown);
	ASSERT_TEST(EM_INVALID_DATE == emTick(em, -1),teardown);
	ASSERT_TEST(EM_SUCCESS == emTick(em, 1),teardown);
	ASSERT_TEST(1 == emGetEventsAmount(em_with_event),teardown);
	ASSERT_TEST(EM_SUCCESS == emTick(em_with_event, 1),teardown);
	ASSERT_TEST(0 == emGetEventsAmount(em),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDate(em_with_event, "a", date2, 1), teardown);
	destroyEventManager(em_with_event);
	em_with_event = createEventManager(date); // teardown
	ASSERT_TEST(EM_SUCCESS == emAddEventByDate(em_with_event, "a", date, 1), teardown);

	ASSERT_TEST(-1 == emGetEventsAmount(NULL),teardown);
	ASSERT_TEST(0 == emGetEventsAmount(em),teardown);
	ASSERT_TEST(1 == emGetEventsAmount(em_with_event),teardown);
	ASSERT_TEST(1 == emGetEventsAmount(em_with_member_and_event),teardown);
	
	ASSERT_TEST(NULL == emGetNextEvent(NULL),teardown);
	ASSERT_TEST(NULL == emGetNextEvent(em),teardown);
	ASSERT_TEST(strcmp("a",emGetNextEvent(em_with_event)) == 0,teardown);
	
	// fail silently, no mem leaks (check with valgrind)
	emPrintAllEvents(NULL, "a");
	emPrintAllEvents(em, NULL);
	emPrintAllEvents(em, "");
	emPrintAllResponsibleMembers(NULL, "a");
	emPrintAllResponsibleMembers(em, NULL);
	emPrintAllResponsibleMembers(em, "");

teardown:
	dateDestroy(date);
	dateDestroy(date2);
	dateDestroy(invalid_date);
	destroyEventManager(em);
	destroyEventManager(em_with_event);
	destroyEventManager(em_with_member);
	destroyEventManager(em_with_member_and_event);
	return result;
}

bool create_dateGood_success() {
	bool result = true;
	Date date = dateCreate(1, 11, 2020);
	EventManager em = createEventManager(date);

	ASSERT_TEST(em != NULL,teardown);

teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}

bool create_dateBad_fail() {
	bool result = true;
	EventManager em = createEventManager(NULL);

	ASSERT_TEST(em == NULL,teardown);

teardown:
	destroyEventManager(em);
	return result;
}

// runs for valgrind
bool destroy_containsStuff_removesAll() {
	bool result = true;
	Date date = dateCreate(1, 11, 2020);
	Date date2 = dateCreate(11, 11, 2020);
	EventManager em = createEventManager(date);
	EventManagerResult res = emAddEventByDate(em, "hello", date, 1);
	res = (res != EM_SUCCESS ? res : emAddEventByDate(em, "bye", date2, 0));
	res = (res != EM_SUCCESS ? res : emAddMember(em, "Itai", 0));
	res = (res != EM_SUCCESS ? res : emAddEventByDiff(em, "hello", 3, 3));
	res = (res != EM_SUCCESS ? res : emAddMember(em, "Zoe", 1));
	res = (res != EM_SUCCESS ? res : emAddMember(em, "Gershon", 2));
	res = (res != EM_SUCCESS ? res : emAddMemberToEvent(em, 0, 0));
	res = (res != EM_SUCCESS ? res : emAddMemberToEvent(em, 1, 0));
	res = (res != EM_SUCCESS ? res : emAddMemberToEvent(em, 1, 1));
	res = (res != EM_SUCCESS ? res : emAddMemberToEvent(em, 1, 3));
	res = (res != EM_SUCCESS ? res : emAddEventByDiff(em, "hello again", 2, 4));
	res = (res != EM_SUCCESS ? res : emAddMemberToEvent(em, 0, 4));
	for (size_t i = 0; i < 25; i++)
	{
		res = (res != EM_SUCCESS ? res : emAddEventByDiff(em, "a"+i, i, 5+i));
	}
	for (size_t i = 0; i < 25; i++)
	{
		res = (res != EM_SUCCESS ? res : emAddMember(em, "a"+i, 5+i));
	}

	ASSERT_TEST(res == EM_SUCCESS,teardown);

teardown:
	dateDestroy(date);
	dateDestroy(date2);
	destroyEventManager(em);
	return result;
}

// bool addEventByDate_valid_success() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addEventByDate_nameNotGood_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addEventByDate_dateOld_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addEventByDate_idNotValid_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addEventByDate_existsWithSameEverything_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addEventByDate_existsWithSameId_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

bool addEventByDate_existsWithSameNameAndDate_fail() {
	bool result = true;
	Date date = dateCreate(6,6,2020);
	EventManager em = createEventManager(date);

	EventManagerResult res = emAddEventByDate(em, "a", date, 0);
	ASSERT_TEST(res == EM_SUCCESS, teardown);
	res = emAddEventByDate(em, "a", date, 1);
	ASSERT_TEST(res == EM_EVENT_ALREADY_EXISTS,teardown);

teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}

// bool addEventByDate_existsWithSameNameDifferentDate_success() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addEventByDate_existsWithDifferentNameSameDate_success() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addEventByDate_alreadyAdded25Events_success() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addEventByDate_printEvents_there() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addEventByDiff_daysNegative_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addEventByDiff_tickLess_stillExists() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addEventByDiff_tickExact_doesntExist() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addEventByDiff_tickMore_doesntExist() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addEventByDiff_printEvents_there() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool removeEvent_idExists_removeAndSucceed() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool removeEvent_idDoesntExist_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool removeEvent_printEvents_notThere() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool changeEventDate_dateOld_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool changeEventDate_idNotValid_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool changeEventDate_doesntExist_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool changeEventDate_existWithSameDate_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool changeEventDate_existAndUniqueDate_success() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool changeEventDate_tickDoesntRemoveOnOldDate_removeOnNewDate() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool changeEventDate_printEvents_printedNewDate() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addMember_idNotValid_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addMember_idExists_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addMember_valid_success() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addMember_already25MembersExist_success() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addMemberToEvent_memberIdNotValid_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addMemberToEvent_eventIdNotValid_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addMemberToEvent_memberDoesntExist_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addMemberToEvent_eventDoesntExist_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addMemberToEvent_allGood_success() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool addMemberToEvent_printEvents_memberInEvent() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool removeMemberFromEvent() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool removeMemberFromEvent_memberIdNotValid_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool removeMemberFromEvent_eventIdNotValid_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool removeMemberFromEvent_memberDoesntExist_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool removeMemberFromEvent_eventDoesntExist_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool removeMemberFromEvent_membersNotLinkedToEvent_fail() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool removeMemberFromEvent_allGood_success() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

// bool removeMemberFromEvent_printEvents_memberNotInEvent() {
// 	bool result = true;


// 	//ASSERT_TEST(,teardown);

// teardown:

// 	return result;
// }

bool tick_removeExactOnEachTick_rightAmountEachTimeUpdated() {
	bool result = true;
	Date date = dateCreate(6,6,2020);
	EventManager em = createEventManager(date);

	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, "h", 0, 0),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, "a", 0, 7),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, "b", 1, 1),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, "c", 2, 2),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, "d", 2, 3),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, "e", 2, 4),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, "f", 5, 5),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, "f1", 5, 8),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, "f2", 5, 9),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, "g", 8, 6),teardown);
	ASSERT_TEST(10 == emGetEventsAmount(em),teardown);
	ASSERT_TEST(EM_SUCCESS == emTick(em, 1),teardown);
	ASSERT_TEST(8 == emGetEventsAmount(em),teardown);
	ASSERT_TEST(EM_SUCCESS == emTick(em, 2),teardown);
	ASSERT_TEST(4 == emGetEventsAmount(em),teardown);
	ASSERT_TEST(EM_SUCCESS == emTick(em, 4),teardown);
	ASSERT_TEST(1 == emGetEventsAmount(em),teardown);
	ASSERT_TEST(EM_SUCCESS == emTick(em, 2),teardown);
	ASSERT_TEST(0 == emGetEventsAmount(em),teardown);

teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}

bool getEventsAmount_noEvents_returnAmount() {
	bool result = true;
	Date date = dateCreate(6,6,2020);
	EventManager em = createEventManager(date);

	ASSERT_TEST(0 == emGetEventsAmount(em),teardown);

teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}
bool getEventsAmount_severalEvents_returnAmount() {
	bool result = true;
	Date date = dateCreate(6,6,2020);
	EventManager em = createEventManager(date);

	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, "a", 2, 0),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, "c", 2, 300),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, "d", 3, 2),teardown);
	ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, "f", 100, 15),teardown);
	ASSERT_TEST(4 == emGetEventsAmount(em),teardown);

teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}
bool getEventsAmount_lotsOfEvents_returnAmount() {
	bool result = true;
	Date date = dateCreate(6,6,2020);
	EventManager em = createEventManager(date);

	for (size_t i = 0; i < 200; i+=3)
	{
		char name[5];
		sprintf(name,"%zu",i+1);
		ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, name, i%20, i),teardown);
	}
	
	ASSERT_TEST(67 == emGetEventsAmount(em),teardown);

teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}
bool getEventsAmount_lotsOfEventsAddedAndRemoved_returnAmount() {
	bool result = true;
	Date date = dateCreate(6,6,2020);
	EventManager em = createEventManager(date);

	for (size_t i = 0; i < 50; i++)
	{
		char name[5];
		sprintf(name,"%zu",i+1);
		ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, name, i%20, i),teardown);
	}
	for (size_t i = 0; i < 10; i++)
	{
		ASSERT_TEST(EM_SUCCESS == emRemoveEvent(em, i*2),teardown);
	}
	for (size_t i = 0; i < 30; i++)
	{
		char name[5];
		sprintf(name,"%zu",i+50+1);
		ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, name, i%20, 50+i),teardown);
	}
	for (size_t i = 0; i < 15; i++)
	{
		ASSERT_TEST(EM_SUCCESS == emRemoveEvent(em, 20+i*2),teardown);
	}
	for (size_t i = 0; i < 40; i++)
	{
		char name[5];
		sprintf(name,"%zu",i+80+1);
		ASSERT_TEST(EM_SUCCESS == emAddEventByDiff(em, name, i%20, 80+i),teardown);
	}
	
	ASSERT_TEST(95 == emGetEventsAmount(em),teardown);

teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}

bool getNextEvent_noEvents_null() {
	bool result = true;
	Date date = dateCreate(1,11,2020);
	EventManager em = createEventManager(date);

	ASSERT_TEST(NULL == emGetNextEvent(em),teardown);

teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}

bool getNextEvent_eventsAddedAtRandom1_returnClosestEvent() {
	bool result = true;
	Date date = dateCreate(1,11,2020);
	EventManager em = createEventManager(date);
	char *expected_name = "a";

	emAddEventByDate(em, "a", date, 0);
	emAddEventByDate(em, "b", date, 1);
	emAddEventByDiff(em, "c", 1, 2);
	emAddEventByDiff(em, "d", 3, 12);
	emAddEventByDiff(em, "e", 5, 18);
	emAddEventByDiff(em, "f", 1, 17);
	emAddEventByDiff(em, "g", 9, 15);
	emAddEventByDiff(em, "h", 2, 14);
	emAddEventByDiff(em, "j", 10, 13);
	emAddEventByDiff(em, "k", 10, 11);
	char *actual_name = emGetNextEvent(em);

	ASSERT_TEST(strcmp(actual_name,expected_name) == 0,teardown);

teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}

bool getNextEvent_eventsAddedAtRandom2_returnClosestEvent() {
	bool result = true;
	Date date = dateCreate(1,11,2020);
	EventManager em = createEventManager(date);
	char *expected_name = "b";

	emAddEventByDate(em, "b", date, 1);
	emAddEventByDate(em, "a", date, 0);
	emAddEventByDiff(em, "c", 1, 2);
	emAddEventByDiff(em, "d", 3, 12);
	emAddEventByDiff(em, "e", 5, 18);
	emAddEventByDiff(em, "f", 1, 17);
	emAddEventByDiff(em, "g", 9, 15);
	emAddEventByDiff(em, "h", 2, 14);
	emAddEventByDiff(em, "j", 10, 13);
	emAddEventByDiff(em, "k", 10, 11);
	char *actual_name = emGetNextEvent(em);

	ASSERT_TEST(strcmp(actual_name,expected_name) == 0,teardown);

teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}

bool getNextEvent_eventsAddedAtRandom3_returnClosestEvent() {
	bool result = true;
	Date date = dateCreate(1,11,2020);
	Date date2 = dateCreate(1,12,2020);
	EventManager em = createEventManager(date);
	char *expected_name = "d";

	emAddEventByDate(em, "a", date2, 0);
	emAddEventByDiff(em, "c", 6, 2);
	emAddEventByDiff(em, "e", 5, 18);
	emAddEventByDiff(em, "f", 4, 17);
	emAddEventByDiff(em, "d", 3, 12);
	emAddEventByDiff(em, "g", 9, 15);
	emAddEventByDate(em, "c", date2, 100);
	emAddEventByDiff(em, "h", 12, 14);
	emAddEventByDiff(em, "e", 3, 1);
	emAddEventByDiff(em, "j", 10, 13);
	emAddEventByDiff(em, "k", 10, 11);
	char *actual_name = emGetNextEvent(em);

	ASSERT_TEST(strcmp(actual_name,expected_name) == 0,teardown);

teardown:
	dateDestroy(date);
	dateDestroy(date2);
	destroyEventManager(em);
	return result;
}

bool getNextEvent_eventsRemovedAndGetNextEventAgain_returnClosestEvent() {
	bool result = true;
	Date date = dateCreate(1,11,2020);
	Date date2 = dateCreate(1,12,2020);
	EventManager em = createEventManager(date);
	char *expected_name1 = "d";
	char *expected_name2 = "g";

	emAddEventByDate(em, "a", date2, 0);
	emAddEventByDiff(em, "c", 6, 2);
	emAddEventByDiff(em, "e", 5, 18);
	emAddEventByDiff(em, "f", 4, 17);
	emAddEventByDiff(em, "d", 3, 12);
	emAddEventByDiff(em, "g", 9, 15);
	emAddEventByDate(em, "c", date2, 100);
	emAddEventByDiff(em, "h", 12, 14);
	emAddEventByDiff(em, "e", 3, 1);
	emAddEventByDiff(em, "j", 10, 13);
	emAddEventByDiff(em, "k", 10, 11);
	char *actual_name = emGetNextEvent(em);
	ASSERT_TEST(strcmp(actual_name,expected_name1) == 0,teardown);

	emTick(em, 7);
	actual_name = emGetNextEvent(em);

	ASSERT_TEST(strcmp(actual_name,expected_name2) == 0,teardown);


teardown:
	dateDestroy(date);
	dateDestroy(date2);
	destroyEventManager(em);
	return result;
}

bool getNextEvent_2EventsOnTheSameDate1_returnTheOneInsertedFirst() {
	bool result = true;
	Date date = dateCreate(1,11,2020);
	EventManager em = createEventManager(date);
	char *expected_name = "a";

	emAddEventByDate(em, "a", date, 0);
	emAddEventByDate(em, "b", date, 1);
	char *actual_name = emGetNextEvent(em);

	ASSERT_TEST(strcmp(actual_name,expected_name) == 0,teardown);

teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}

bool getNextEvent_2EventsOnTheSameDate2_returnTheOneInsertedFirst() {
	bool result = true;
	Date date = dateCreate(1,11,2020);
	EventManager em = createEventManager(date);
	char *expected_name = "b";

	emAddEventByDate(em, "b", date, 1);
	emAddEventByDate(em, "a", date, 0);
	char *actual_name = emGetNextEvent(em);

	ASSERT_TEST(strcmp(actual_name,expected_name) == 0,teardown);

teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}

bool printAllEvents_severalMembers1_printMembersInEachEventInAscendingIdOrder() {
	bool result = true;
	Date date = dateCreate(1,11,2020);
	EventManager em = createEventManager(date);
	char *file_name = "printAllEvents_severalMembers1_printMembersInEachEventInAscendingIdOrder.txt";
	char *expected = "a,1.11.2020,Itai,Zoe";

	emAddMember(em, "Zoe", 1);
	emAddMember(em, "Itai", 0);
	emAddEventByDate(em, "a", date, 0);
	emAddMemberToEvent(em, 1, 0);
	emAddMemberToEvent(em, 0, 0);
	emPrintAllEvents(em, file_name);

	FILE *f = fopen(file_name,"r");
	ASSERT_TEST(f != NULL,teardown);
	char buf[100];
	while (fgets(buf,100,f));
	ASSERT_TEST(strcmp(buf,expected) == 0,teardown_all);

teardown_all:
	fclose(f);
teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}

bool printAllEvents_severalMembers2_printMembersInEachEventInAscendingIdOrder() {
	bool result = true;
	Date date = dateCreate(1,11,2020);
	EventManager em = createEventManager(date);
	char *file_name = "printAllEvents_severalMembers2_printMembersInEachEventInAscendingIdOrder.txt";
	char *expected = "a,1.11.2020,Itai,Zoe";

	emAddMember(em, "Zoe", 1);
	emAddMember(em, "Itai", 0);
	emAddEventByDate(em, "a", date, 0);
	emAddMemberToEvent(em, 0, 0);
	emAddMemberToEvent(em, 1, 0);
	emPrintAllEvents(em, file_name);

	FILE *f = fopen(file_name,"r");
	ASSERT_TEST(f != NULL,teardown);
	char buf[100];
	while (fgets(buf,100,f));
	ASSERT_TEST(strcmp(buf,expected) == 0,teardown_all);

teardown_all:
	fclose(f);
teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}

bool printAllEvents_severalEvents_printAscendingDateOrder() {
	bool result = true;
	Date date = dateCreate(1,10,2020);
	Date date1 = dateCreate(5,11,2020);
	Date date2 = dateCreate(2,12,2020);
	Date date3 = dateCreate(1,1,2021);
	EventManager em = createEventManager(date);
	char *file_name = "printAllEvents_severalEvents_printAscendingDateOrder.txt";
	char expected[] = "a,5.11.2020,Itai,Zoe\nb,2.12.2020,Itai,Zoe,Gershon\nc,1.1.2021,Gershon";

	emAddMember(em, "Zoe", 1);
	emAddMember(em, "Itai", 0);
	emAddMember(em, "Gershon", 2);
	emAddEventByDate(em, "c", date3, 2);
	emAddEventByDate(em, "b", date2, 0);
	emAddEventByDate(em, "a", date1, 1);
	emAddMemberToEvent(em, 0, 1);
	emAddMemberToEvent(em, 1, 1);
	emAddMemberToEvent(em, 0, 0);
	emAddMemberToEvent(em, 2, 0);
	emAddMemberToEvent(em, 1, 0);
	emAddMemberToEvent(em, 2, 2);
	emPrintAllEvents(em, file_name);

	FILE *f = fopen(file_name,"r");
	ASSERT_TEST(f != NULL,teardown);
	fseek(f, 0, SEEK_END); 
	long size = ftell(f);
	fseek(f, 0, SEEK_SET); 
	char *buf = (char*)malloc(size+1);
	fread(buf,1,size,f);
	buf[size]='\0';
	ASSERT_TEST(strcmp(buf,expected) == 0,teardown_all);

teardown_all:
	fclose(f);
	free(buf);
teardown:
	dateDestroy(date);
	dateDestroy(date1);
	dateDestroy(date2);
	dateDestroy(date3);
	destroyEventManager(em);
	return result;
}

bool printAllEvents_moreThan25Events_printCorrectly() {
	bool result = true;
	Date date = dateCreate(1,10,2020);
	EventManager em = createEventManager(date);
	char *file_name = "printAllEvents_moreThan25Events_printCorrectly.txt";
	char expected[] = "a,1.10.2020,Itai\n\
a,2.10.2020,Itai\n\
a,3.10.2020,Itai\n\
a,4.10.2020,Itai\n\
a,5.10.2020,Itai\n\
a,6.10.2020,Itai\n\
a,7.10.2020,Itai\n\
a,8.10.2020,Itai\n\
a,9.10.2020,Itai\n\
a,10.10.2020,Itai\n\
a,11.10.2020,Itai\n\
a,12.10.2020,Itai\n\
a,13.10.2020,Itai\n\
a,14.10.2020,Itai\n\
a,15.10.2020,Itai\n\
a,16.10.2020,Itai\n\
a,17.10.2020,Itai\n\
a,18.10.2020,Itai\n\
a,19.10.2020,Itai\n\
a,20.10.2020,Itai\n\
a,21.10.2020,Itai\n\
a,22.10.2020,Itai\n\
a,23.10.2020,Itai\n\
a,24.10.2020,Itai\n\
a,25.10.2020,Itai\n\
a,26.10.2020,Itai\n\
a,27.10.2020,Itai\n\
a,28.10.2020,Itai\n\
a,29.10.2020,Itai\n\
a,30.10.2020,Itai";

	emAddMember(em, "Itai", 1);
	for (size_t i = 0; i < 30; i++)
	{
		emAddEventByDiff(em, "a", i, i);
		emAddMemberToEvent(em, 1, i);
	}
	emPrintAllEvents(em, file_name);

	FILE *f = fopen(file_name,"r");
	ASSERT_TEST(f != NULL,teardown);
	fseek(f, 0, SEEK_END); 
	long size = ftell(f);
	fseek(f, 0, SEEK_SET); 
	char *buf = (char*)malloc(size+1);
	fread(buf,1,size,f);
	buf[size]='\0';
	ASSERT_TEST(strcmp(buf,expected) == 0,teardown_all);

teardown_all:
	fclose(f);
	free(buf);
teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}


bool printAllEvents_moreThan40Events_printCorrectly() {
	bool result = true;
	Date date = dateCreate(1,10,2020);
	EventManager em = createEventManager(date);
	char *file_name = "printAllEvents_moreThan40Events_printCorrectly.txt";
	char expected[] = "a,1.10.2020,Itai\n\
a,2.10.2020,Itai\n\
a,3.10.2020,Itai\n\
a,4.10.2020,Itai\n\
a,5.10.2020,Itai\n\
a,6.10.2020,Itai\n\
a,7.10.2020,Itai\n\
a,8.10.2020,Itai\n\
a,9.10.2020,Itai\n\
a,10.10.2020,Itai\n\
a,11.10.2020,Itai\n\
a,12.10.2020,Itai\n\
a,13.10.2020,Itai\n\
a,14.10.2020,Itai\n\
a,15.10.2020,Itai\n\
a,16.10.2020,Itai\n\
a,17.10.2020,Itai\n\
a,18.10.2020,Itai\n\
a,19.10.2020,Itai\n\
a,20.10.2020,Itai\n\
a,21.10.2020,Itai\n\
a,22.10.2020,Itai\n\
a,23.10.2020,Itai\n\
a,24.10.2020,Itai\n\
a,25.10.2020,Itai\n\
a,26.10.2020,Itai\n\
a,27.10.2020,Itai\n\
a,28.10.2020,Itai\n\
a,29.10.2020,Itai\n\
a,30.10.2020,Itai\n\
a,31.10.2020,Itai\n\
a,1.11.2020,Itai\n\
a,2.11.2020,Itai\n\
a,3.11.2020,Itai\n\
a,4.11.2020,Itai\n\
a,5.11.2020,Itai\n\
a,6.11.2020,Itai\n\
a,7.11.2020,Itai\n\
a,8.11.2020,Itai\n\
a,9.11.2020,Itai\n\
a,10.11.2020,Itai\n\
a,11.11.2020,Itai\n\
a,12.11.2020,Itai\n\
a,13.11.2020,Itai\n\
a,14.11.2020,Itai";

	emAddMember(em, "Itai", 1);
	for (size_t i = 0; i < 45; i++)
	{
		emAddEventByDiff(em, "a", i, i);
		emAddMemberToEvent(em, 1, i);
	}
	emPrintAllEvents(em, file_name);

	FILE *f = fopen(file_name,"r");
	ASSERT_TEST(f != NULL,teardown);
	fseek(f, 0, SEEK_END); 
	long size = ftell(f);
	fseek(f, 0, SEEK_SET); 
	char *buf = (char*)malloc(size+1);
	fread(buf,1,size,f);
	buf[size]='\0';
	ASSERT_TEST(strcmp(buf,expected) == 0,teardown_all);

teardown_all:
	fclose(f);
	free(buf);
teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}

bool printAllEvents_moreThan25Members_printCorrectly() {
	bool result = true;
	Date date = dateCreate(1,10,2020);
	EventManager em = createEventManager(date);
	char *file_name = "printAllEvents_moreThan25Members_printCorrectly.txt";
	char expected[] = "a,1.10.2020,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30";

	emAddEventByDiff(em, "a", 0, 0);
	for (size_t i = 0; i < 30; i++)
	{
		char name[5];
		sprintf(name,"%zu",i+1);
		emAddMember(em, name, i);
		emAddMemberToEvent(em, i, 0);
	}
	emPrintAllEvents(em, file_name);

	FILE *f = fopen(file_name,"r");
	ASSERT_TEST(f != NULL,teardown);
	fseek(f, 0, SEEK_END); 
	long size = ftell(f);
	fseek(f, 0, SEEK_SET); 
	char *buf = (char*)malloc(size+1);
	fread(buf,1,size,f);
	buf[size]='\0';
	ASSERT_TEST(strcmp(buf,expected) == 0,teardown_all);

teardown_all:
	fclose(f);
	free(buf);
teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}

bool printAllEvents_moreThan40Members_printCorrectly() {
	bool result = true;
	Date date = dateCreate(1,10,2020);
	EventManager em = createEventManager(date);
	char *file_name = "printAllEvents_moreThan40Members_printCorrectly.txt";
	char expected[] = "a,1.10.2020,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45";

	emAddEventByDiff(em, "a", 0, 0);
	for (size_t i = 0; i < 45; i++)
	{
		char name[5];
		sprintf(name,"%zu",i+1);
		emAddMember(em, name, i);
		emAddMemberToEvent(em, i, 0);
	}
	emPrintAllEvents(em, file_name);

	FILE *f = fopen(file_name,"r");
	ASSERT_TEST(f != NULL,teardown);
	fseek(f, 0, SEEK_END); 
	long size = ftell(f);
	fseek(f, 0, SEEK_SET); 
	char *buf = (char*)malloc(size+1);
	fread(buf,1,size,f);
	buf[size]='\0';
	ASSERT_TEST(strcmp(buf,expected) == 0,teardown_all);

teardown_all:
	fclose(f);
	free(buf);
teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}

// Event with no members - not printed
// Event with several members - ordered by id
// Event on same date with other event - inserted first
bool printAllEvents_randomAllOverThePlace_printCorrectly() {
	bool result = true;
	Date date = dateCreate(1,10,2020);
	Date date1 = dateCreate(5,11,2020);
	Date date2 = dateCreate(2,12,2020);
	Date date3 = dateCreate(1,1,2021);
	Date date4 = dateCreate(22,11,2020);
	EventManager em = createEventManager(date);
	char *file_name = "printAllEvents_randomAllOverThePlace_printCorrectly.txt";
	char expected[] = "a,5.11.2020,Itai1,Itai,Gershon\n\
c,5.11.2020,Zoe,Itai,Gershon1\n\
a,22.11.2020,Itai1,Itai,Gershon,Gershon1\n\
b,2.12.2020,Itai1,Zoe,Gershon\n\
a,2.12.2020,Gershon,Gershon1\n\
c,1.1.2021,Zoe,Itai";

	emAddMember(em, "Zoe", 1);
	emAddMember(em, "Itai", 2);
	emAddMember(em, "Gershon", 5);
	emAddMember(em, "Itai1", 0);
	emAddMember(em, "Gershon1", 6);
	emAddEventByDate(em, "c", date3, 2);
	emAddEventByDate(em, "b", date2, 0);
	emAddEventByDate(em, "a", date1, 1); // same date several members
	emAddEventByDate(em, "a", date4, 4); // several members
	emAddEventByDate(em, "c", date1, 6); // same date several members
	emAddEventByDate(em, "b", date1, 7); // same date no members
	emAddEventByDate(em, "a", date2, 8);
	emAddMemberToEvent(em, 2, 2);
	emAddMemberToEvent(em, 1, 2);
	emAddMemberToEvent(em, 1, 2);
	emAddMemberToEvent(em, 2, 2);
	emAddMemberToEvent(em, 6, 8);
	emAddMemberToEvent(em, 5, 8);
	emAddMemberToEvent(em, 0, 0);
	emAddMemberToEvent(em, 5, 0);
	emAddMemberToEvent(em, 1, 0);
	emAddMemberToEvent(em, 1, 6);
	emAddMemberToEvent(em, 1, 6);
	emAddMemberToEvent(em, 6, 6);
	emAddMemberToEvent(em, 2, 6);
	emAddMemberToEvent(em, 0, 1);
	emAddMemberToEvent(em, 5, 1);
	emAddMemberToEvent(em, 2, 1);
	emAddMemberToEvent(em, 0, 4);
	emAddMemberToEvent(em, 2, 4);
	emAddMemberToEvent(em, 6, 4);
	emAddMemberToEvent(em, 5, 4);
	emPrintAllEvents(em, file_name);

	FILE *f = fopen(file_name,"r");
	ASSERT_TEST(f != NULL,teardown);
	fseek(f, 0, SEEK_END); 
	long size = ftell(f);
	fseek(f, 0, SEEK_SET); 
	char *buf = (char*)malloc(size+1);
	fread(buf,1,size,f);
	buf[size]='\0';
	ASSERT_TEST(strcmp(buf,expected) == 0,teardown_all);

teardown_all:
	fclose(f);
	free(buf);
teardown:
	dateDestroy(date);
	dateDestroy(date1);
	dateDestroy(date2);
	dateDestroy(date3);
	dateDestroy(date4);
	destroyEventManager(em);
	return result;
}

bool printMembers_severalMembersWithSeveralEvents_printDescendingOrderOnAmountOfEvents() {
	bool result = true;
	Date date = dateCreate(1,11,2020);
	EventManager em = createEventManager(date);
	char *file_name = "printMembers_severalMembersWithSeveralEvents_printDescendingOrderOnAmountOfEvents.txt";
	char *expected = "Zoe,6\nRandom,6\nItai,5\nGershon,3";

	emAddMember(em, "Zoe", 1);
	emAddMember(em, "Itai", 0);
	emAddMember(em, "Gershon", 4);
	emAddMember(em, "Random", 7);
	emAddMember(em, "Nobody", 5);
	emAddEventByDiff(em, "a", 1, 0);
	emAddEventByDiff(em, "b", 1, 1);
	emAddEventByDiff(em, "c", 2, 2);
	emAddEventByDiff(em, "d", 3, 3);
	emAddEventByDiff(em, "e", 3, 4);
	emAddEventByDiff(em, "e", 4, 5);
	emAddEventByDiff(em, "e", 5, 6);
	emAddMemberToEvent(em, 0, 0);
	emAddMemberToEvent(em, 1, 3);
	emAddMemberToEvent(em, 0, 2);
	emAddMemberToEvent(em, 0, 1);
	emAddMemberToEvent(em, 1, 1);
	emAddMemberToEvent(em, 4, 0);
	emAddMemberToEvent(em, 7, 3);
	emAddMemberToEvent(em, 7, 2);
	emAddMemberToEvent(em, 7, 1);
	emAddMemberToEvent(em, 4, 4);
	emAddMemberToEvent(em, 0, 5);
	emAddMemberToEvent(em, 1, 5);
	emAddMemberToEvent(em, 0, 6);
	emAddMemberToEvent(em, 1, 4);
	emAddMemberToEvent(em, 1, 6);
	emAddMemberToEvent(em, 4, 6);
	emAddMemberToEvent(em, 7, 6);
	emAddMemberToEvent(em, 7, 5);
	emAddMemberToEvent(em, 7, 4);
	emAddMemberToEvent(em, 1, 0);
	emPrintAllResponsibleMembers(em, file_name);

	FILE *f = fopen(file_name,"r");
	ASSERT_TEST(f != NULL,teardown);
	fseek(f, 0, SEEK_END); 
	long size = ftell(f);
	fseek(f, 0, SEEK_SET); 
	char *buf = (char*)malloc(size+1);
	fread(buf,1,size,f);
	buf[size]='\0';
	ASSERT_TEST(strcmp(buf,expected) == 0,teardown_all);

teardown_all:
	free(buf);
	fclose(f);
teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}

bool printMembers_2MembersWithTheSameAmountOfEvents_printAscendingId() {
	bool result = true;
	Date date = dateCreate(1,11,2020);
	EventManager em = createEventManager(date);
	char *file_name = "printMembers_2MembersWithTheSameAmountOfEvents_printAscendingId.txt";
	char *expected = "Itai,3\nZoe,3";

	emAddMember(em, "Zoe", 1);
	emAddMember(em, "Itai", 0);
	emAddEventByDiff(em, "a", 1, 0);
	emAddEventByDiff(em, "b", 1, 1);
	emAddEventByDiff(em, "c", 2, 2);
	emAddEventByDiff(em, "d", 3, 3);
	emAddEventByDiff(em, "e", 3, 4);
	emAddMemberToEvent(em, 0, 0);
	emAddMemberToEvent(em, 1, 3);
	emAddMemberToEvent(em, 0, 2);
	emAddMemberToEvent(em, 0, 1);
	emAddMemberToEvent(em, 1, 4);
	emAddMemberToEvent(em, 1, 1);
	emPrintAllResponsibleMembers(em, file_name);

	FILE *f = fopen(file_name,"r");
	ASSERT_TEST(f != NULL,teardown);
	fseek(f, 0, SEEK_END); 
	long size = ftell(f);
	fseek(f, 0, SEEK_SET); 
	char *buf = (char*)malloc(size+1);
	fread(buf,1,size,f);
	buf[size]='\0';
	ASSERT_TEST(strcmp(buf,expected) == 0,teardown_all);

teardown_all:
	free(buf);
	fclose(f);
teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}

bool printMembers_memberWith0Events_notPrinted() {
	bool result = true;
	Date date = dateCreate(1,11,2020);
	EventManager em = createEventManager(date);
	char *file_name = "printMembers_memberWith0Events_notPrinted.txt";
	char *expected = "Zoe,1";

	emAddMember(em, "Itai", 0);
	emAddMember(em, "Zoe", 1);
	emAddEventByDiff(em, "a", 1, 0);
	emAddMemberToEvent(em, 1, 0);
	emPrintAllResponsibleMembers(em, file_name);

	FILE *f = fopen(file_name,"r");
	ASSERT_TEST(f != NULL,teardown);
	fseek(f, 0, SEEK_END); 
	long size = ftell(f);
	fseek(f, 0, SEEK_SET); 
	char *buf = (char*)malloc(size+1);
	fread(buf,1,size,f);
	buf[size]='\0';
	ASSERT_TEST(strcmp(buf,expected) == 0,teardown_all);

teardown_all:
	free(buf);
	fclose(f);
teardown:
	dateDestroy(date);
	destroyEventManager(em);
	return result;
}



bool testEventManagerCreateDestroy() {
    bool result = true;
    Date start_date = dateCreate(1,12,2020);
    EventManager em = createEventManager(start_date);

    ASSERT_TEST(em != NULL, destroyEventManagerCreateDestroy);
    ASSERT_TEST(emGetEventsAmount(em) == 0, destroyEventManagerCreateDestroy);
    ASSERT_TEST(emGetNextEvent(em) == NULL, destroyEventManagerCreateDestroy);

destroyEventManagerCreateDestroy:
    destroyEventManager(em);
    dateDestroy(start_date);
    return result;

}

bool testAddEventByDiffAndSize() {
    bool result = true;

    Date start_date = dateCreate(1,12,2020);
    EventManager em = createEventManager(start_date);

    char* event_name = "event1";
    ASSERT_TEST(emAddEventByDiff(em, event_name, 2, 1) == EM_SUCCESS, destroyAddEventByDiffAndSize);
    ASSERT_TEST(emGetEventsAmount(em) == 1, destroyAddEventByDiffAndSize);
    ASSERT_TEST(strcmp(event_name, emGetNextEvent(em)) == 0, destroyAddEventByDiffAndSize);

destroyAddEventByDiffAndSize:
    dateDestroy(start_date);
    destroyEventManager(em);
    return result;
}

bool testEMTick() {
	bool result = true;

    Date start_date = dateCreate(1,12,2020);
    EventManager em = createEventManager(start_date);

    char* event_name = "event1";
    ASSERT_TEST(emAddEventByDiff(em, event_name, 1, 1) == EM_SUCCESS, destroyEMTick);

    ASSERT_TEST(emGetEventsAmount(em) == 1, destroyEMTick);
    ASSERT_TEST(emTick(em, 2) == EM_SUCCESS, destroyEMTick);
    ASSERT_TEST(emGetEventsAmount(em) == 0, destroyEMTick);
destroyEMTick:
    dateDestroy(start_date);
    destroyEventManager(em);
    return result;
}

bool (*tests[]) (void) = {
        testEventManagerCreateDestroy,
        testAddEventByDiffAndSize,
        testEMTick,
		create_dateGood_success,
		destroy_containsStuff_removesAll,
		printMembers_memberWith0Events_notPrinted,
		printMembers_2MembersWithTheSameAmountOfEvents_printAscendingId,
		printMembers_severalMembersWithSeveralEvents_printDescendingOrderOnAmountOfEvents,
		getNextEvent_2EventsOnTheSameDate1_returnTheOneInsertedFirst,
		getNextEvent_2EventsOnTheSameDate2_returnTheOneInsertedFirst,
		printAllEvents_severalMembers1_printMembersInEachEventInAscendingIdOrder,
		printAllEvents_severalMembers2_printMembersInEachEventInAscendingIdOrder,
		printAllEvents_severalEvents_printAscendingDateOrder,
		printAllEvents_randomAllOverThePlace_printCorrectly,
		printAllEvents_moreThan25Members_printCorrectly,
		printAllEvents_moreThan40Members_printCorrectly,
		printAllEvents_moreThan25Events_printCorrectly,
		printAllEvents_moreThan40Events_printCorrectly,
		addEventByDate_existsWithSameNameAndDate_fail,
		callAllFunctionsWithErrors_returnNullOrError,
		tick_removeExactOnEachTick_rightAmountEachTimeUpdated,
		getEventsAmount_noEvents_returnAmount,
		getEventsAmount_severalEvents_returnAmount,
		getEventsAmount_lotsOfEvents_returnAmount,
		getEventsAmount_lotsOfEventsAddedAndRemoved_returnAmount,
		getNextEvent_eventsAddedAtRandom1_returnClosestEvent,
		getNextEvent_eventsAddedAtRandom2_returnClosestEvent,
		getNextEvent_eventsAddedAtRandom3_returnClosestEvent,
		getNextEvent_eventsRemovedAndGetNextEventAgain_returnClosestEvent,
};

const char* testNames[] = {
        "testEventManagerCreateDestroy",
        "testAddEventByDiffAndSize",
        "testEMTick",
		"create_dateGood_success",
		"destroy_containsStuff_removesAll",
		"printMembers_memberWith0Events_notPrinted",
		"printMembers_2MembersWithTheSameAmountOfEvents_printAscendingId",
		"printMembers_severalMembersWithSeveralEvents_printDescendingOrderOnAmountOfEvents",
		"getNextEvent_2EventsOnTheSameDate1_returnTheOneInsertedFirst",
		"getNextEvent_2EventsOnTheSameDate2_returnTheOneInsertedFirst",
		"printAllEvents_severalMembers1_printMembersInEachEventInAscendingIdOrder",
		"printAllEvents_severalMembers2_printMembersInEachEventInAscendingIdOrder",
		"printAllEvents_severalEvents_printAscendingDateOrder",
		"printAllEvents_randomAllOverThePlace_printCorrectly",
		"printAllEvents_moreThan25Members_printCorrectly",
		"printAllEvents_moreThan40Members_printCorrectly",
		"printAllEvents_moreThan25Events_printCorrectly",
		"printAllEvents_moreThan40Events_printCorrectly",
		"addEventByDate_existsWithSameNameAndDate_fail",
		"callAllFunctionsWithErrors_returnNullOrError",
		"tick_removeExactOnEachTick_rightAmountEachTimeUpdated",
		"getEventsAmount_noEvents_returnAmount",
		"getEventsAmount_severalEvents_returnAmount",
		"getEventsAmount_lotsOfEvents_returnAmount",
		"getEventsAmount_lotsOfEventsAddedAndRemoved_returnAmount",
		"getNextEvent_eventsAddedAtRandom1_returnClosestEvent",
		"getNextEvent_eventsAddedAtRandom2_returnClosestEvent",
		"getNextEvent_eventsAddedAtRandom3_returnClosestEvent",
		"getNextEvent_eventsRemovedAndGetNextEventAgain_returnClosestEvent",
		NULL
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; testNames[test_idx]!=NULL; test_idx++) {
            RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    // if (argc != 2) {
    //     fprintf(stdout, "Usage: event_manager_tests <test index>\n");
    //     return 0;
    // }

    // int test_idx = strtol(argv[1], NULL, 10);
    // if (test_idx < 1 || test_idx > NUMBER_TESTS) {
    //     fprintf(stderr, "Invalid test index %d\n", test_idx);
    //     return 0;
    // }

    // RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1]);
    return 0;
}
