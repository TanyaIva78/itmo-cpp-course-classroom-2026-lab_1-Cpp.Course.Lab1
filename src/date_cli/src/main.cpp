#include "lab1/date.hpp"

#include <charconv>
#include <cstdio>
#include <optional>
#include <string_view>
#include <system_error>
#include <vector>

namespace
{

using lab1::date::Date;

constexpr const char* kWeekdays[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

constexpr int kExitInvalid = 1;
constexpr int kExitUsage = 2;

template <typename T> std::optional<T> parse_number(std::string_view text)
{
    T value{};
    const char* end = text.data() + text.size();
    const auto [ptr, ec] = std::from_chars(text.data(), end, value);
    if (ec != std::errc{} || ptr != end)
    {
        return std::nullopt;
    }
    return value;
}

std::optional<Date> parse_date(std::string_view text)
{
    if (text.size() != 10 || text[4] != '-' || text[7] != '-')
    {
        return std::nullopt;
    }
    const auto year = parse_number<int>(text.substr(0, 4));
    const auto month = parse_number<int>(text.substr(5, 2));
    const auto day = parse_number<int>(text.substr(8, 2));
    if (!year || !month || !day)
    {
        return std::nullopt;
    }
    return Date{*year, *month, *day};
}

std::optional<Date> parse_valid_date(std::string_view text)
{
    const auto date = parse_date(text);
    if (!date || !lab1::date::is_valid(*date))
    {
        return std::nullopt;
    }
    return date;
}

void print_date(Date date)
{
    std::printf("%04d-%02d-%02d\n", date.year, date.month, date.day);
}

void print_usage()
{
    std::fputs("usage:\n"
               "  date_cli validate YYYY-MM-DD\n"
               "  date_cli weekday  YYYY-MM-DD\n"
               "  date_cli add      YYYY-MM-DD N\n"
               "  date_cli diff     YYYY-MM-DD YYYY-MM-DD\n",
               stderr);
}

} // namespace

int main(int argc, char** argv)
{
    const std::vector<std::string_view> args(argv + 1, argv + argc);
    if (args.empty())
    {
        print_usage();
        return kExitUsage;
    }

    const std::string_view command = args[0];

    if (command == "validate" && args.size() == 2)
    {
        const bool ok = parse_valid_date(args[1]).has_value();
        std::puts(ok ? "OK" : "INVALID");
        return ok ? 0 : kExitInvalid;
    }

    if (command == "weekday" && args.size() == 2)
    {
        const auto date = parse_valid_date(args[1]);
        if (!date)
        {
            std::puts("INVALID");
            return kExitInvalid;
        }
        std::puts(kWeekdays[lab1::date::day_of_week(*date)]);
        return 0;
    }

    if (command == "add" && args.size() == 3)
    {
        const auto date = parse_valid_date(args[1]);
        if (!date)
        {
            std::puts("INVALID");
            return kExitInvalid;
        }
        const auto delta = parse_number<int>(args[2]);
        if (!delta)
        {
            std::fputs("error: N must be an integer\n", stderr);
            return kExitUsage;
        }
        const Date result = lab1::date::add_days(*date, *delta);
        if (!lab1::date::is_valid(result))
        {
            std::fputs("error: result is out of range 0000-01-01..9999-12-31\n", stderr);
            return kExitInvalid;
        }
        print_date(result);
        return 0;
    }

    if (command == "diff" && args.size() == 3)
    {
        const auto from = parse_valid_date(args[1]);
        const auto to = parse_valid_date(args[2]);
        if (!from || !to)
        {
            std::puts("INVALID");
            return kExitInvalid;
        }
        std::printf("%d\n", lab1::date::days_between(*from, *to));
        return 0;
    }

    print_usage();
    return kExitUsage;
}
