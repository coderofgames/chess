#ifndef POLYGLOT_BOOK_MANAGER_H
#define POLYGLOT_BOOK_MANAGER_H

#include "headers.h"
#include "PGLookup.h"

class PolyglotBook
{
public:
	PolyglotBook()
	{
		name = "";
		filePtr = NULL;
	}
	PolyglotBook( wxString &Name )
	{
		path = Name;
		name = Name.substr(Name.find_last_of("\\") +1, Name.size());
		fopen(Name.c_str().AsChar(), "r+b");
	}
	~PolyglotBook()
	{
		if( filePtr )
		{
			fclose(filePtr);
		}
	}

	wxString path;
	wxString name;
	FILE* filePtr;
};

class PolyglotBookManager
{
public:

	PolyglotBookManager(){}
	~PolyglotBookManager()
	{
		m_booksLoaded.clear();
	}

	void LoadBook(char * filename)
	{
		m_booksLoaded.push_back(new PolyglotBook(filename));
	}
	void UnloadBook(char *name)
	{
		for( int i = 0; i < m_booksLoaded.size(); i++ )
		{
			if( m_booksLoaded[i] && m_booksLoaded[i]->name == name )
			{
				delete m_booksLoaded[i];
				m_booksLoaded[i] = NULL;
			}
		}
	}

	void ClearAll()
	{
		for( int i = 0; i < m_booksLoaded.size(); i++ )
		{
			delete m_booksLoaded[i];
			m_booksLoaded[i] = NULL;
			
		}
	}

	void AddBook(char *filename)
	{
		m_booksLoaded.push_back(new PolyglotBook(filename));
	}

	entry_t EntryFrom

	vector<PolyglotBook*> m_booksLoaded;
	FILE* filePtr;
};

#endif