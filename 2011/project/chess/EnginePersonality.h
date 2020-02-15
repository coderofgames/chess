#ifndef ENGINE_PERSONALITY_H
#define ENGINE_PERSONALITY_H

#include "headers.h"

//spin check combo button string
enum
{
	SPIN_TYPE,
	COMBO_TYPE,
	CHECK_TYPE,
	BUTTON_TYPE,
	STRING_TYPE,
};

class UCIOption
{
public:
	UCIOption(){
		name = "";

	}
	~UCIOption(){}

	void SetName(string nameToSet)
	{
		name = nameToSet;
	}

	string GetName()
	{
		return name;
	}

	int GetType()
	{
		return m_type;
	}

	string name;
	int m_type;
};

class SpinOption : public UCIOption
{
public:
	SpinOption(){
		m_type = SPIN_TYPE;
	}
	SpinOption(string nameToSet, int valueToSet){
		value = valueToSet;
		name = nameToSet;
		m_type = SPIN_TYPE;
	}
	~SpinOption(){}

	void SetValue(long valToSet)
	{
		value = valToSet;
	}

	long GetValue()
	{
		return value;
	}

	long value;
};

class CheckOption : public UCIOption
{
public:
	CheckOption(){
		m_type = CHECK_TYPE;
	}
	CheckOption(string nameToSet, bool val){
		state = val;
		name = nameToSet;
		m_type = CHECK_TYPE;
	}
	~CheckOption(){}

	void SetState(bool stateToSet)
	{
		state = stateToSet;
	}

	bool GetState()
	{
		return state;
	}

	bool state;
};

class ComboOption : public UCIOption
{
public:
	ComboOption(){
		m_type = COMBO_TYPE;
	}

	~ComboOption(){}

	void AddOption(string var)
	{
		options.push_back(var);
	}

	void SetSelectionInt( int sel)
	{
		selectedOption = sel;
	}

	void SetSelectonString( string selStr )
	{
		selectedStr = selStr;
		for( int i = 0; i < options.size(); i++ )
		{
			if( selectedStr == options[i] )
			{
				selectedOption = i;
				break;
			}
		}
	}

	string &GetSelectedString()
	{
		return selectedStr;
	}

	string selectedStr;
	int selectedOption;
	vector<string> options;
};

class ButtonOption : public UCIOption
{
public:
	ButtonOption(){
		m_type = BUTTON_TYPE;
	}
	ButtonOption(string nameToSet, int id){
		name = nameToSet;
		btnID = id;
		m_type = BUTTON_TYPE;
	}
	~ButtonOption(){}

	void SetBtnID(int idToSet)
	{
		btnID = idToSet;
	}

	int GetBtnID()
	{
		return btnID;
	}

	int btnID;
};

class StringOption : public UCIOption
{
public:
	StringOption(){
		m_type = STRING_TYPE;
	}
	StringOption(string nameToSet){
		name = nameToSet;
		m_type = STRING_TYPE;
	}
	StringOption(string nameToSet, string defString){
		name = nameToSet;
		value = defString;
		m_type = STRING_TYPE;
	}
	~StringOption(){}

	void SetString(string stringToSet)
	{
		value = stringToSet;
	}

	string GetString()
	{
		return value;
	}

	string value;
};


class EnginePersonalityMap
{
public:

	string m_engineName;
	string m_enginePersonalityFile;
};

class UCIEnginePersonality
{
public:

	UCIEnginePersonality(){}
	~UCIEnginePersonality(){
		for( int i = 0; i < optionsMap.size(); i++ )
		{
			if( optionsMap[i] )
			{
				delete optionsMap[i];
			}
		}
	}

	void Load(string name){}
	void Save(string name){}

	

	string m_engineName;
	vector< UCIOption* > optionsMap;

	int indexToFile;
};



#endif