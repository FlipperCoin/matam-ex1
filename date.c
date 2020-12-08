#include <stdlib.h>

#include "date.h"

#define DATE_LEAP_YEAR_JUMPS 4
#define DATE_DAYS_IN_FEBRUARY_NO_LEAP 28
#define DATE_DAYS_IN_FEBRUARY_LEAP 29

#define SIMPLE true
#define SIMPLE_MAX_DAY 30

typedef enum Month {
    JANUARY = 1,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
} Month_t;

struct Date_t {
    int day;
    int month;
    int year;
};

static int maxDayInMonth(int month, int year) {
    if (SIMPLE) {
        return SIMPLE_MAX_DAY;
    }

    int maxDay;
    switch (month) {
        case JANUARY:
        case MARCH:
        case MAY:
        case JULY:
        case AUGUST:
        case OCTOBER:
        case DECEMBER:
            maxDay = 31;
            break;
        case APRIL:
        case JUNE:
        case SEPTEMBER:
        case NOVEMBER:
            maxDay = 30;
            break;
        case FEBRUARY:
            // Checking if leap year
            if (year % DATE_LEAP_YEAR_JUMPS == 0)
                maxDay = DATE_DAYS_IN_FEBRUARY_LEAP;
            else
                maxDay = DATE_DAYS_IN_FEBRUARY_NO_LEAP;
            break;
    }

    return maxDay;
}

Date dateCreate(int day, int month, int year) {
    if (month < JANUARY || month > DECEMBER) return NULL;

    int max_day = maxDayInMonth(month, year);
    if (day < 1 || day > max_day) {
        return NULL;
    }

    Date date = (Date)malloc(sizeof(struct Date_t));
    if (date == NULL) {
        return NULL;
    }

    date->day=day;
    date->month=month;
    date->year=year;
    return date;
}

void dateDestroy(Date date) {
    free(date);
}

Date dateCopy(Date date) {
    if (date == NULL) {
        return NULL;
    }

    Date date_copy = dateCreate(date->day, date->month, date->year);
    return date_copy; // fine if NULL
}

bool dateGet(Date date, int* day, int* month, int* year) {
    if (date == NULL || day == NULL || month == NULL || year == NULL) {
        return false;
    }

    *day = date->day;
    *month = date->month;
    *year = date->year;

    return true;
}

int dateCompare(Date date1, Date date2) {
    if (date1 == NULL || date2 == NULL) {
        return 0;
    }

    int year_diff = date1->year - date2->year;
    if (year_diff != 0) return year_diff;

    int month_diff = date1->month - date2->month;
    if (month_diff != 0) return month_diff;

    int day_diff = date1->day - date2->day;
    return day_diff;
}

void dateTick(Date date) {
    if (date == NULL) {
        return;
    }

    if (date->day != maxDayInMonth(date->month, date->year)) {
        date->day++;
        return;
    }

    date->day = 1;
    if (date->month == DECEMBER) {
        date->month = JANUARY;
        date->year++;
    } else {
        date->month++;
    }
}