#pragma once 

namespace lab1::date
{

struct Date
{

        int year;
        int month;
        int day;

        friend bool operator==(const Date&, const Date&) = default;

};

bool is_leap(int year);
int days_in_month(int year, int month);
bool is_valid(Date date);

int day_of_week(Date date);
int days_between(Date date1, Date date2);
Date add_days(Date date, int delta);

}
