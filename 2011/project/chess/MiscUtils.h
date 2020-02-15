#ifndef MISC_UTILS_H
#define MISC_UTILS_H

#include "headers.h"
#include <iomanip>
#include <sstream>
#include <string>


wxString GetSubstringInQuotes(string &inpt);
wxString GetSubstringInQuotes(wxString &inpt);

wxString SetSubstringInQuotes(wxString &inpt, string &set);
wxString SetSubstringInQuotes(wxString &inpt, wxString &set);

wxString GetSubstringInCurlyBraces(wxString& inpt);
wxString GetSubstringInCurlyBraces(string& inpt);


template <class T>
inline std::string to_string (const T& t)
{
std::stringstream ss;
ss << t;
return ss.str();
}

template <class T>
inline std::string ZeroPadNumber(const T& num, int pad = 2)
{
    std::ostringstream ss;
    ss << std::setw(pad) << std::setfill('0') << num;
    std::string result = ss.str();

    return result;
}




#endif
