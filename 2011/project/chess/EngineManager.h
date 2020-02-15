#ifndef ENGINE_MANAGER_H
#define ENGINE_MANAGER_H

#include "Headers.h"

enum
{
	UCI,
	WINBOARD
};

class EnginePersonality
{
public:
	EnginePersonality()
	{
		name = "Default";
		configFile = "";
	}
	~EnginePersonality()
	{
	}

	wxString name;
	wxString configFile;
};

class Engine
{
public:
	Engine()
	{
		type = UCI;
		name = "";
		path = "";
		currentPersonality = 0;
		personalities.push_back(new EnginePersonality());
		bVisible = false;
		bActive = false;
	}
	Engine(wxString &Name, wxString &Path, int type)
	{
		type = type;
		name = Name;
		path = Path;
		currentPersonality = 0;
		personalities.push_back(new EnginePersonality());
		bVisible = false;
		bActive = false;
	}
	~Engine()
	{
		personalities.clear();
	}

	wxString& GetName()
	{
		return name;
	}

	void SetName(wxString &Name)
	{
		name = Name;
	}

	wxString& GetPath()
	{
		return path;
	}

	void SetPath(wxString &Path)
	{
		path = Path;
	}

	wxString name;
	wxString path;
	int currentPersonality;
	vector< EnginePersonality* > personalities;
	bool bVisible;
	bool bActive;
	int type;
};


class EngineList
{
public:

	EngineList(){}
	~EngineList()
	{
		engines.clear();
	}

	inline void AddEngine( Engine* pEngine )
	{
		engines.push_back(pEngine);
	}

	inline Engine* GetEngine( int indx )
	{
		return engines[indx];
	}

	inline int Size()
	{
		return engines.size();
	}

	inline Engine* operator [] (int indx )
	{
		return engines[indx];
	}



	vector<Engine*> engines;
};

class EngineManager : public EngineList
{
public:
	EngineManager():EngineList()
	{
		LoadEngineList();
	}
	~EngineManager()
	{
	}

	void LoadEngineList()
	{
		ifstream myfile ("settings/engine_list.txt");
		while ( myfile.good() )
		{
			string line;
			getline (myfile,line);
			if( (line.size() != 0) && (line[0] != ' '))
			{
				wxString name;
				wxString path;
				int path_pos = line.find_last_of("\\");
				if( path_pos > 0 )
				{
					path = line.substr(0, path_pos);
					name = line.substr(path_pos+1, line.size());
				}
				else
				{
					path = "";
					name = line;
				}
				//name = "";
				//name = line;
				AddEngine(name, path, 0);

			}
		}
	}

	void SaveEngineList()
	{
		ofstream myfile ("settings/engine_list.txt");
	
		for( int i = 0; i < Size(); i++ )
		{
			//wxString str = lc->GetTextValue (i, 1);
			//if( str.Length() != 0 )
			{		
				myfile << engines[i]->name.c_str().AsChar();
				myfile << "\n";
			}
		}
	}

	void AddEngine( wxString &name, wxString &path, int type)
	{
		Engine *engine = new Engine(name, path, type);
		EngineList::AddEngine(engine);
	}

	void RemoveEngine(wxString &name)
	{
		for( int i = 0; i < Size(); i++ )
		{
			if( engines[i]->GetName() == name )
			{
			//	engines.erase(i, 1);
			}
		}
	}

	wxString& GetPath( int indx )
	{
		return engines[indx]->GetPath();
	}

	wxString& GetName( int indx )
	{
		return engines[indx]->GetName();
	}

	wxString& GetDir(int indx)
	{
		return engines[indx]->GetPath() + engines[indx]->GetName();
	}

};


#endif