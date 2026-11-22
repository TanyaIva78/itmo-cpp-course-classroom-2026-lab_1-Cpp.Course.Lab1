#include "lab1/date.hpp"

namespace lab1::date
{

namespace
{

constexpr int min_year = 0;
constexpr int max_year = 9999;


long long days_from_civil(int year, int month, int day)
{
    const long long y = month <= 2 ? static_cast<long long>(year) - 1 : year;
    const long long era = (y >= 0 ? y : y - 399) / 400;
    const long long yoe = y - era * 400;
    const long long mp = month > 2 ? month - 3 : month + 9;
    const long long doy = (153 * mp + 2) / 5 + day - 1;
    const long long doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + doe - 719468;
}

Date civil_from_days(long long days)
{
    const long long z = days + 719468;
    const long long era = (z >= 0 ? z : z - 146096) / 146097;
    const long long doe = z - era * 146097;
    const long long yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    const long long doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    const long long mp = (5 * doy + 2) / 153;
    const long long day = doy - (153 * mp + 2) / 5 + 1;
    const long long month = mp < 10 ? mp + 3 : mp - 9;
    const long long year = yoe + era * 400 + (month <= 2 ? 1 : 0);
    return Date{static_cast<int>(year), static_cast<int>(month), static_cast<int>(day)};
}

} // namespace

bool is_leap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int days_in_month(int year, int month)
{
    static constexpr int lengths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month < 1 || month > 12)
    {
        return 0;
    }
    if (month == 2 && is_leap(year))
    {
        return 29;
    }
    return lengths[month - 1];
}

bool is_valid(Date date)
{
    if (date.year < min_year || date.year > max_year)
    {
        return false;
    }
    const int dim = days_in_month(date.year, date.month);
    return dim != 0 && date.day >= 1 && date.day <= dim;
}

int day_of_week(Date date)
{
    if (!is_valid(date))
    {
        return -1;
    }

    const long long days = days_from_civil(date.year, date.month, date.day);
    return static_cast<int>((days % 7 + 7 + 3) % 7);
}

int days_between(Date date1, Date date2)
{
    if (!is_valid(date1) || !is_valid(date2))
    {
        return 0;
    }
    return static_cast<int>(days_from_civil(date2.year, date2.month, date2.day) -
                            days_from_civil(date1.year, date1.month, date1.day));
}

Date add_days(Date date, int delta)
{
    const Date invalid{0, 0, 0};
    if (!is_valid(date))
    {
        return invalid;
    }
    const Date result = civil_from_days(days_from_civil(date.year, date.month, date.day) + delta);
    return is_valid(result) ? result : invalid;
}

}