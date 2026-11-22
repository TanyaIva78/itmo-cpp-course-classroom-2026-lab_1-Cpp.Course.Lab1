#include <catch2/catch_all.hpp>

#include "lab1/date.hpp"

using lab1::date::Date;
using namespace lab1::date;

TEST_CASE("Leap years follow the Gregorian rules", "[leap]")
{
    CHECK(is_leap(2000));
    CHECK(is_leap(2024));
    CHECK_FALSE(is_leap(1900));
    CHECK_FALSE(is_leap(2023));
}

TEST_CASE("Days in month", "[days_in_month]")
{
    CHECK(days_in_month(2024, 2) == 29);
    CHECK(days_in_month(2023, 2) == 28);
    CHECK(days_in_month(2020, 4) == 30);
    CHECK(days_in_month(2020, 12) == 31);
    CHECK(days_in_month(2020, 0) == 0);
    CHECK(days_in_month(2020, 13) == 0);
}

TEST_CASE("Date validation", "[is_valid]")
{
    CHECK(is_valid(Date{2024, 2, 29}));
    CHECK(is_valid(Date{0, 1, 1}));
    CHECK(is_valid(Date{9999, 12, 31}));

    CHECK_FALSE(is_valid(Date{2023, 2, 29}));
    CHECK_FALSE(is_valid(Date{2020, 4, 31}));
    CHECK_FALSE(is_valid(Date{2020, 13, 1}));
    CHECK_FALSE(is_valid(Date{2020, 0, 1}));
    CHECK_FALSE(is_valid(Date{2020, 1, 0}));
    CHECK_FALSE(is_valid(Date{-1, 1, 1}));
    CHECK_FALSE(is_valid(Date{10000, 1, 1}));
}

TEST_CASE("Day of week, Monday is 0", "[day_of_week]")
{
    CHECK(day_of_week(Date{1970, 1, 1}) == 3);
    CHECK(day_of_week(Date{2024, 2, 29}) == 3);
    CHECK(day_of_week(Date{2000, 1, 1}) == 5);
    CHECK(day_of_week(Date{2026, 11, 4}) == 2);
    CHECK(day_of_week(Date{2023, 2, 29}) == -1);
}

TEST_CASE("Days between dates", "[days_between]")
{
    CHECK(days_between(Date{2020, 1, 1}, Date{2020, 1, 31}) == 30);
    CHECK(days_between(Date{2020, 1, 31}, Date{2020, 1, 1}) == -30);
    CHECK(days_between(Date{2020, 2, 28}, Date{2020, 3, 1}) == 2);
    CHECK(days_between(Date{2021, 5, 5}, Date{2021, 5, 5}) == 0);
    CHECK(days_between(Date{2021, 5, 5}, Date{2023, 2, 29}) == 0);
}

TEST_CASE("Adding days crosses month and year boundaries", "[add_days]")
{
    CHECK(add_days(Date{2020, 3, 1}, -1) == Date{2020, 2, 29});
    CHECK(add_days(Date{2020, 12, 31}, 1) == Date{2021, 1, 1});
    CHECK(add_days(Date{2024, 2, 28}, 1) == Date{2024, 2, 29});
    CHECK(add_days(Date{2019, 12, 31}, 366) == Date{2020, 12, 31});
    CHECK(add_days(Date{1970, 1, 1}, 10000) == Date{1997, 5, 19});
    CHECK(add_days(Date{2021, 6, 15}, 0) == Date{2021, 6, 15});
}

TEST_CASE("Adding days out of range gives an invalid date", "[add_days]")
{
    CHECK_FALSE(is_valid(add_days(Date{0, 1, 1}, -1)));
    CHECK_FALSE(is_valid(add_days(Date{9999, 12, 31}, 1)));
    CHECK_FALSE(is_valid(add_days(Date{2023, 2, 29}, 1)));
}