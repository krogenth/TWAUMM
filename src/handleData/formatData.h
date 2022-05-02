#pragma once
#include <locale>
#include <iomanip>
#include <string>
#include <sstream>
#include <iostream>

template<typename CharT>
struct Sep : public std::numpunct<CharT>
{
    virtual std::string do_grouping() const { return "\003"; }
};

template<class T>
std::string FormatWithCommas(T value)
{
    std::stringstream ss;
    ss.imbue(std::locale(std::cout.getloc(), new Sep<char>()));
    ss << std::fixed << value;
    return ss.str();
}