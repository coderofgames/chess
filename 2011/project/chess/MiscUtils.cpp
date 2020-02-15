#include "MiscUtils.h"


wxString GetSubstringInQuotes(string &inpt)
{
	int openQuotes = inpt.find('"');
	int closeQuotes = inpt.find_last_of('"');

	int length = inpt.length();
	int size = (length-openQuotes+1) - (length - closeQuotes);//.c_str()
	return wxString((const unsigned char *)inpt.substr(openQuotes+1, size-2).c_str());
}


wxString GetSubstringInQuotes(wxString &inpt)
{
	int openQuotes = inpt.find('"');
	int closeQuotes = inpt.find_last_of('"');

	int length = inpt.length();
	int size = (length-openQuotes+1) - (length - closeQuotes);//.c_str()
	return wxString((const unsigned char *)inpt.substr(openQuotes+1, size-2).c_str());
}


wxString SetSubstringInQuotes(wxString &inpt, string &set)
{
	int openQuotes = inpt.find('"');
	int closeQuotes = inpt.find_last_of('"');

	int length = inpt.length();
	int size = (length-openQuotes+1) - (length - closeQuotes);//.c_str()
	wxString::iterator start;
	wxString::iterator end;
	start = inpt.GetIterForNthChar(openQuotes+1);
	end = inpt.GetIterForNthChar(closeQuotes);

	inpt.erase(start, end);
	inpt.insert(openQuotes+1, set);

	return inpt;
}

wxString SetSubstringInQuotes(wxString &inpt, wxString &set)
{
	int openQuotes = inpt.find('"');
	int closeQuotes = inpt.find_last_of('"');

	int length = inpt.length();
	int size = (length-openQuotes+1) - (length - closeQuotes);//.c_str()
	wxString::iterator start;
	wxString::iterator end;
	start = inpt.GetIterForNthChar(openQuotes+1);
	end = inpt.GetIterForNthChar(closeQuotes);

	inpt.erase(start, end);
	inpt.insert(openQuotes+1, set);

	return inpt;
}



wxString GetSubstringInCurlyBraces(wxString& inpt)
{
	int openBraces = inpt.find('{');
	int closeBraces = inpt.find_last_of('}');

	int length = inpt.length();
	int size = (length-openBraces+1) - (length - closeBraces);//.c_str()
	return wxString((const unsigned char *)inpt.substr(openBraces+1, size-2).c_str());
}

wxString GetSubstringInCurlyBraces(string& inpt)
{
	int openBraces = inpt.find('{');
	int closeBraces = inpt.find_last_of('}');

	int length = inpt.length();
	int size = (length-openBraces+1) - (length - closeBraces);//.c_str()
	return wxString((const unsigned char *)inpt.substr(openBraces+1, size-2).c_str());
}




