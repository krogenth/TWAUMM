#pragma once
#include <locale>
#include <iomanip>
#include <string>
#include <sstream>

template<class T>
std::string FormatWithCommas(T value)
{
    std::stringstream ss;
    ss.imbue(std::locale(""));
    ss << std::fixed << value;
    return ss.str();
}