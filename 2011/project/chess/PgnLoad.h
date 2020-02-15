#ifndef PGN_LOAD_H
#define PGN_LOAD_H

#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include "ChessSquare.h"
#include "PGKey.h"
//#include <ofstream>
#include "HashTable.h"

#include "sqlite3.h"

#include "ColorPallete.h"

#include "ExpandableArray.h"

#include <string>
using namespace std;

static bool variation_auto_fold = false;

enum FORMAT_ERROR_CODES
{
	INCORRECT_LENGTH,
	FORMAT_ERROR,
	PASSED_TEST
};

#ifndef WHITE
#define WHITE 0
#define BLACK 1
#endif

//namespace MY_GLOBAL_HASH_SPACE {
extern std::unordered_map<uint64, HashEntry> hash_table;

class HeaderElemC 
{
public:

	string data;
};

class CompressedHeader
{
public:

	CompressedHeader()
	{
		wname = 0;
		bname = 0;
		eventname = 0;
		sitename = 0;
		date = 0;
		result = 0;
		whiteElo = 0;
		blackElo = 0;
		eco=0;
	}

	int wname;
	int bname;
	int eventname;
	int sitename;
	int date;
	int whiteElo;
	int blackElo;
	int eco;
	unsigned char result;

	int Save(FILE *fp)
	{
		fwrite((void*)&wname, sizeof(int), 1, fp); 
		fwrite((void*)&bname, sizeof(int), 1, fp); 
		fwrite((void*)&eventname, sizeof(int), 1, fp); 
		fwrite((void*)&sitename, sizeof(int), 1, fp); 
		fwrite((void*)&date, sizeof(int), 1, fp); 
		// #2
		fwrite((void*)&whiteElo, sizeof(int), 1, fp); 
		fwrite((void*)&blackElo, sizeof(int), 1, fp);

		fwrite((void*)&eco, sizeof(int), 1, fp);
		fwrite((void*)&result, sizeof(unsigned char), 1, fp); 
		
		return sizeof(int)*8 + sizeof(unsigned char);
	}

	int Read(FILE* fp, int &offset)
	{
		offset += fread((void*)&wname, sizeof(int), 1, fp); 
		offset += fread((void*)&bname, sizeof(int), 1, fp); 
		offset += fread((void*)&eventname, sizeof(int), 1, fp); 
		offset += fread((void*)&sitename, sizeof(int), 1, fp); 
		offset += fread((void*)&date, sizeof(int), 1, fp);

		offset += fread((void*)&whiteElo, sizeof(int), 1, fp); 
		offset += fread((void*)&blackElo, sizeof(int), 1, fp); 

		offset += fread((void*)&eco, sizeof(int), 1, fp); 
		offset += fread((void*)&result, sizeof(unsigned char), 1, fp); 

		return offset;
	}
};

class CompressedMoveList
{
public:

	CompressedMoveList()
	{
		plyCount = 0 ;
		moves = 0;
	}

	void Allocate(int numMoves)
	{
		plyCount = numMoves;
		moves = new unsigned char[numMoves];
	}

	void DeAllocate()
	{
		delete [] moves;
	}

	bool SetMove( int indx, unsigned char mv )
	{
		if( indx < plyCount )
		{
			moves[indx] = mv;
			return true;
		}
		else return false;
	}

	unsigned char GetMove( int indx )
	{
		if( indx < plyCount )
		{
			return moves[indx];
		}
		else return 255;
	}

	int Save(FILE *fp)
	{
		fwrite((void*)&plyCount, sizeof(int), 1, fp); 
		fwrite((void*)moves, sizeof(unsigned char), plyCount, fp); 
		return sizeof(int) + sizeof(unsigned char)*plyCount;
	}

	void Read(FILE* fp, int &offset )
	{
		offset += fread((void*)&plyCount, sizeof(int), 1, fp);
		moves = new unsigned char[plyCount];
		offset += fread((void*)moves, sizeof(unsigned char), plyCount, fp);

	}
 
	int plyCount;
	unsigned char* moves;
};


class GameIndex
{
public:
	GameIndex()
	{
		header_start = -1;
		game_end= 0;
		pNext = NULL;
	}
	~GameIndex(){
		/*if( pNext )
		{
			delete pNext;
			pNext = NULL;
		}*/
		//elems.clear();
	}

	void Save(FILE *fp)
	{
		hdr.Save(fp);
		moves.Save(fp);
	}

	void Save(FILE *fp, int &offset)
	{
		offset += hdr.Save(fp);
		offset += moves.Save(fp);
	}
	uint64 header_start;
	uint64 header_end;
	uint64 game_start;
	uint64 game_end;
	GameIndex *pNext;
	//vector<HeaderElemC*> elems;
	CompressedHeader hdr;
	CompressedMoveList moves;
};

class GameIndex32
{
public:
	GameIndex32()
	{
		header_start = 0;
		game_end= 0;
		pNext = NULL;
	}
	~GameIndex32(){
		/*if( pNext )
		{
			delete pNext;
			pNext = NULL;
		}*/
		//elems.clear();
	}

	void Save(FILE *fp)
	{
		hdr.Save(fp);
		moves.Save(fp);
	}

	void Save(FILE *fp, int &offset)
	{
		offset += hdr.Save(fp);
		offset += moves.Save(fp);
	}

	unsigned int header_start;
	unsigned int header_end;
	unsigned int game_start;
	unsigned int game_end;

	GameIndex32 *pNext;
	//vector<HeaderElemC*> elems;
	CompressedHeader hdr;
	CompressedMoveList moves;

};


class GameIndex32Moves
{
public:
	GameIndex32Moves()
	{
		header_start = 0;
		game_end= 0;
		pNext = NULL;
		moves=0;
		//moves.resize(1024);
	}
	~GameIndex32Moves(){
		/*if( pNext )
		{
			delete pNext;
			pNext = NULL;
		}*/
		//elems.clear();
		if( moves )
		{
			delete [] moves;
		}
	}

	void Save(FILE *fp)
	{
		hdr.Save(fp);
		//moves.Save(fp);
	}

	void Save(FILE *fp, int &offset)
	{
		offset += hdr.Save(fp);
		//offset += moves.Save(fp);
	}

	unsigned int header_start;
	unsigned int header_end;
	unsigned int game_start;
	unsigned int game_end;

	GameIndex32Moves *pNext;
	//vector<HeaderElemC*> elems;
	CompressedHeader hdr;
	//CompressedMoveList moves;
	unsigned short int nummoves;
	unsigned short int* moves;
};

#define SORTABLE_GAME_INDEX_MODE_RAM 0
#define SORTABLE_GAME_INDEX_MODE_VM 1

class SortableGameIndex
{
public:
	SortableGameIndex()
	{
		original_loc =0;
		m_Gi = NULL;
		DiskLocGiStart=0;
		
		//DiskSizeGi=0;
	}
	SortableGameIndex( int discLoc )
	{
		DiskLocGiStart = discLoc;
		m_Gi = NULL;
		
	}
	SortableGameIndex( GameIndex32Moves *gi32m, int loc)
	{
		m_Gi = gi32m;
		original_loc = loc;
	}
	~SortableGameIndex()
	{
		if( m_Gi )
		{
			delete m_Gi;
		}
	}

	int GetHdrWhiteNameIDX();
	int GetHdrBlackNameIDX();
	int GetHdrWhiteEloIDX();
	int GetHdrBlackEloIDX();
	int GetHdrEventIDX();
	int GetHdrSiteIDX();
	int GetHdrDateIDX();
	int GetHdrResult();

	
	int DiskLocGiStart;
	//unsigned short int DiskSizeGi;
	GameIndex32Moves *m_Gi;
	int original_loc;
};


class CompressedGameIndex
{
public:

	uint64 start;
	int end;
	CompressedHeader hdr;
	CompressedMoveList moves;
	CompressedGameIndex* pNext;
};

class VM_GameIndex
{
public:
	unsigned int header_start;
	unsigned int header_end;
	unsigned int game_start;
	unsigned int game_end;

};

class NameSet
{
public:

	int AddName( string &newName )
	{
		for( unsigned int i = 0; i < names.size(); i++ )
		{
			if( newName == names[i] )
			{
				return i;
			}
		}

		names.push_back(newName);
		
		return names.size() -1;
	}

	int Find( string &name )
	{
		for( unsigned int i = 0; i < names.size(); i++ )
		{
			if( name == names[i] )
			{
				return i;
			}
		}

		return -1;
	}

	string GetName( int idx )
	{
		string noEvent = "noEvent";
		if( idx < names.size() )
		{
			return names[idx];
		}
		else return noEvent;
	}

	bool WriteToFile(string &fileName )
	{
		string tempPlayerNames = fileName;

		FILE* tempFile;
		tempFile = fopen(tempPlayerNames.c_str(), "wb" );
		for( int i = 0; i < names.size(); i++ )
		{
			//names[i] += '\n';
			fwrite((void*)names[i].c_str(), sizeof(char),names[i].size(), tempFile); 
		}
		fclose(tempFile);

		return true;
	}

	void ReadFromFile(string &fileName)
	{
		FILE * inpt = fopen(fileName.c_str(), "rb");
		char c;
		if (inpt==NULL) return ;//perror ("Error opening file");
		else
		{
			string name = "";
			do {
				c = fgetc (inpt);
				
				if( c == '\n' )
				{
					names.push_back(name);
					name = "";
				}
				else
				{
					name += c;
				}
			} while (c != EOF);
    
			fclose (inpt);
		}
	}
	string column_type;
	vector< string > names;
};

class SortableStringNode
{
public:

	string str;
	int id;
};

class ArraySet
{
public:

	string& GetString(int p)
	{
		if( p < elems.size() )
		{
			return elems[p].str;
		}
		else return string("");
	}


	void Append(string &s)
	{
		SortableStringNode e;
		e.str = s;
		e.id = 0;
		elems.push_back(e);
	}

	
	vector<SortableStringNode> elems;
};

class TreeNode
{
public:

	TreeNode()
	{
		parent = NULL;
		left = right = NULL;
		data = -1;
		searchVisited = false;
	}

	TreeNode(TreeNode* pParent)
	{
		parent = pParent;
		left = right = NULL;
		data = -1;
		searchVisited = false;
	}

	~TreeNode()
	{
		if( left )
		{
			delete left;
		}

		if( right )
		{
			delete right;
		}
	}

	int data;
	bool searchVisited ;

	int Insert(string &s2, ArraySet &set)
	{
		string s1 = set.GetString(data);
		
		if( s1 == s2 )
		{
			return data;
		}
		else
		{
			if( s1 < s2 )
			{
				if( right )
				{
					return right->Insert(s2, set);
				}
				else
				{
					right = new TreeNode(this);
					set.Append(s2);
					right->data = set.elems.size()-1;
					return right->data;
				}
			}
			else if( s1 > s2 )
			{
				if( left )
				{
					return left->Insert(s2, set);
				}
				else
				{
					left = new TreeNode(this);
					set.Append(s2);
					left->data = set.elems.size()-1;
					return left->data;
				}
			}
		}
	}

	void Visit(vector<int> &rmp, ArraySet& mylife)
	{
		if( left )
		{
			left->Visit(rmp, mylife);
		}

		rmp.push_back(data);
		mylife.elems[this->data].id = rmp.size()-1;

		if( right )
		{
			right->Visit(rmp, mylife);
		}
	}

	TreeNode* parent;
	TreeNode* left;
	TreeNode *right;
};

	class FindNameInfo
	{
	public:
		string str;
		int id;
	};

class TreeIndexedSet
{
public:

	TreeIndexedSet()
	{
		root = NULL;
	}

	~TreeIndexedSet()
	{
		delete root;
	}

	void DeleteTreePart()
	{
		if( root )
		{
			delete root;
			root = NULL;
		}
	}

	int AddName(string &str)
	{
		if( root )
		{
			return root->Insert(str, m_set);
		}
		else
		{
			root = new TreeNode;
			m_set.Append(str);
			root->data = m_set.elems.size()-1;

			return root->data;
		}
	}

	string& GetName(int p)
	{
		return m_set.GetString(p);
	}

	void SortRemap()
	{
		if( m_set.elems.size() > 10000000 )
		{
			SortRemapIterate();
		}
		else
		{
			root->Visit(sortedRemapping, m_set);
		}
	}

	void SortRemapIterate()
	{
        TreeNode *currNode = root;

        while (true)
        {

            if (currNode == NULL)
                break;

			if (currNode->left != NULL && currNode->left->searchVisited != true)  
			{
					currNode = currNode->left;
			}
            else if (currNode->right != NULL && currNode->right->searchVisited != true)
            {
				currNode = currNode->right;
			}
            else if (currNode->searchVisited == false)
            {
				sortedRemapping.push_back(currNode->data);
                currNode->searchVisited = true;
            }
			else
            {
				currNode = currNode->parent;
			}

        }
	}

	bool WriteToFile(string &fileName )
	{
		string tempPlayerNames = fileName;

		FILE* tempFile;
		tempFile = fopen(tempPlayerNames.c_str(), "wb" );
		for( int i = 0; i < m_set.elems.size(); i++ )
		{
			//m_set.elems[i].str += '\n';
			string strToWrite = m_set.elems[i].str + '\n';
			fwrite((void*)strToWrite.c_str(), sizeof(char),strToWrite.length(), tempFile); 
		}
		fclose(tempFile);

		return true;
	}

	void ReadFromFile(string &fileName)
	{
		FILE * inpt = fopen(fileName.c_str(), "rb");
		char c;
		if (inpt==NULL) return ;//perror ("Error opening file");
		else
		{
			string name = "";
			do {
				c = fgetc (inpt);
				
				if( c == '\n' )
				{
					this->AddName(name);
					//names.push_back(name);
					name = "";
				}
				else
				{
					name += c;
				}
			} while (c != EOF);
    
			fclose (inpt);
		}
	}

	void Clear()
	{
		this->DeleteTreePart();
		if( sortedRemapping.size() > 0 )
		{
			sortedRemapping.clear();
		}
		if( m_set.elems.size() > 0 )
		{
			m_set.elems.clear();
		}
	}

	bool FindExactString( string &toMatch, int &index )
	{
		if( toMatch.length() == 0 )
			return false;

		for( int i = 0; i < m_set.elems.size(); i++ )
		{
			if( m_set.elems[i].str == toMatch )
			{
				index = i;
				return true;
			}
		}

		return false;
	}

	bool FindSubstring( string &toMatch, int &index )
	{
		if( toMatch.length() == 0 )
			return false;

		for( int i = 0; i < m_set.elems.size(); i++ )
		{
			if( m_set.elems[i].str.find(toMatch)!= -1 )
			{
				index = i;
				return true;
			}
		}

		return false;
	}

	

	// binary search
	void FindSimilarStrings(vector<FindNameInfo> &vecFound, string &toFind)
	{
		if( toFind.length() == 0 )
			return;

		

		for( int i = 0; i < m_set.elems.size(); i++ )
		{
			if( (m_set.elems[i].str.length() > 0) && (m_set.elems[i].str[0] == toFind[0]) )
			{
				if( (toFind.length() > 1) && (m_set.elems[i].str.length() > 1) )
				{
					if( toFind[1] == m_set.elems[i].str[1] )
					{
						if( (toFind.length() > 2) && (m_set.elems[i].str.length() > 2) )
						{
							if( toFind[2] == m_set.elems[i].str[2] )
							{
								if( (toFind.length() > 3) && (m_set.elems[i].str.length() > 3) )
								{
									if( toFind[4] == m_set.elems[i].str[4] )
									{
										FindNameInfo fni;
										fni.str = m_set.elems[i].str;
										fni.id = i;
										vecFound.push_back(fni);
									}
								}
								else // e.g the name Tal is only 3 letters.
								{
									FindNameInfo fni;
									fni.str = m_set.elems[i].str;
									fni.id = i;
									vecFound.push_back(fni);
								}
							}
						}
						else // the case of a name like Ra
						{
							FindNameInfo fni;
							fni.str = m_set.elems[i].str;
							fni.id = i;
							vecFound.push_back(fni);
						}
					}
				}
			}
		}
	}

	vector< int > sortedRemapping;
	TreeNode* root;
	ArraySet m_set;
};


class IntegerSet
{
public:

	int AddInt( int a )
	{
		for( unsigned int i = 0; i < integers.size(); i++ )
		{
			if( a == integers[i] )
			{
				return i;
			}
		}

		integers.push_back(a);
		
		return integers.size() -1;
	}

	int Find( int a )
	{
		for( unsigned int i = 0; i < integers.size(); i++ )
		{
			if( a == integers[i] )
			{
				return i;
			}
		}

		return -1;
	}

	int GetInteger( int idx )
	{
		
		if( idx < integers.size() )
		{
			return integers[idx];
		}
		else return 0;
	}



	bool WriteToFile(string &fileName )
	{
		string tempPlayerNames = fileName;

		FILE* tempFile;
		tempFile = fopen(tempPlayerNames.c_str(), "wb" );
		int numInts = integers.size();
		fwrite((void*)&numInts, sizeof(int), 1, tempFile);
		for( int i = 0; i < integers.size(); i++ )
		{
			//names[i] += '\n';
			fwrite((void*)&integers[i], sizeof(int),1, tempFile); 
		}
		fclose(tempFile);

		return true;
	}

	void ReadFromFile(string &fileName)
	{
		FILE * inpt = fopen(fileName.c_str(), "rb");
		char c;
		if (inpt==NULL) return ;//perror ("Error opening file");
		else
		{
			int numInts = 0;
			fread((void*)&numInts, sizeof(int), 1, inpt);
			for( int i = 0; i < numInts; i++ )
			{
				int newInt = 0;
				fread((void*)&newInt, sizeof(int),1,inpt);
				integers.push_back(newInt);
			}
			fclose(inpt);
		}
	}

	void Clear()
	{
		if( integers.size() > 0 )
		{
			integers.clear();
		}
	}

	vector< int > integers;
};

class HeaderListData
{
public:

/*	NameSet playerNames;
	NameSet eventNames;
	NameSet dateNames;
	NameSet siteNames;
	*/
	TreeIndexedSet playerNames;
	TreeIndexedSet eventNames;
	TreeIndexedSet dateNames;
	TreeIndexedSet siteNames;
	TreeIndexedSet ecoNames;

	IntegerSet playerElos;

	void DeleteTrees()
	{
		playerNames.DeleteTreePart();
		eventNames.DeleteTreePart();
		dateNames.DeleteTreePart();
		siteNames.DeleteTreePart();
		ecoNames.DeleteTreePart();

	}

	void Clear()
	{
		playerNames.Clear();
		eventNames.Clear();
		dateNames.Clear();
		siteNames.Clear();
		ecoNames.Clear();

		playerElos.Clear();
	}

	void PerformSort()
	{
		if( playerNames.m_set.elems.size() > 0 )
		{
			playerNames.sortedRemapping.clear();
			playerNames.SortRemap();
		}
		if( eventNames.m_set.elems.size() > 0 )
		{
			eventNames.sortedRemapping.clear();
			eventNames.SortRemap();
		}
		if( dateNames.m_set.elems.size() > 0 )
		{
			dateNames.sortedRemapping.clear();
			dateNames.SortRemap();
		}
		if( siteNames.m_set.elems.size() > 0 )
		{
			siteNames.sortedRemapping.clear();
			siteNames.SortRemap();
		}
		if( ecoNames.m_set.elems.size() > 0 )
		{
			ecoNames.sortedRemapping.clear();
			ecoNames.SortRemap();
		}
	}

	int AddPlayerName( string &name )
	{
		return playerNames.AddName(name);
	}

	int AddEventName( string &name )
	{
		return eventNames.AddName(name);
	}

	int AddDateName( string &name )
	{
		return dateNames.AddName(name);
	}

	int AddSiteName( string &name )
	{
		return siteNames.AddName(name);
	}

	int AddEco( string &name )
	{
		return ecoNames.AddName(name);
	}

	int AddELO( int elo )
	{
		return playerElos.AddInt(elo);
	}

	string LookupPlayerName( int idx )
	{
		return playerNames.GetName(idx);
	}

	string LookupEventName( int idx )
	{
		return eventNames.GetName(idx);
	}

	string LookupDateName( int idx )
	{
		return dateNames.GetName(idx);
	}

	string LookupSiteName( int idx )
	{
		return siteNames.GetName( idx );
	}

	string LookupEcoName( int idx )
	{
		return ecoNames.GetName( idx );
	}



	int LookupElo( int idx )
	{
		return playerElos.GetInteger(idx);
	}
	

	bool WriteLists(string &outputName)
	{
		/* #3
			write file...
			"num columns (integer)"
			"Players string, Events string, Dates string, Sites string"
			"index files"
			"PlayersFileString Players, EventsString Events, DatesString Dates, SitesString Sites"
		*/
		playerNames.WriteToFile(outputName + "_players.idx");
		eventNames.WriteToFile(outputName + "_events.idx");
		dateNames.WriteToFile(outputName + "_dates.idx");
		siteNames.WriteToFile(outputName + "_sites.idx");
		ecoNames.WriteToFile(outputName + "_ecos.idx");
		playerElos.WriteToFile(outputName + "_elos.idx");
		return true;
	}

	void ReadLists( string &inputName )
	{
		/* #3
			read file ...
			"num Columns"
			"read string"
			"tokenize with comma"
			"for each"
			"find one of type, "string, int, int64 etc"
			"get token up to first space, this is the name"
			"is the name one of the recognized names {event, site, date, white, black, whiteElo, blackElo} etc"
			"if yes, then use normal routine for creating column for them"
			"make sure the type is the same"
			"else create a new column based on the type"
			"read the files names in the same method, and assign them to the container"
			"load the files with the ReadFromFile command"
			"list types can be polymorphic, so all list types are stored in a vector"
			"the vector is accessed by name, so find("Player", index) calls search on the list
			 of columns and then returns, so the columns should be accessable by index too"
			" int GetColumnIndex("Player") returns the int, so a class can always retrieve data with
			  find(playerListIndex, individualPlayerIndex)"

		*/
		playerNames.ReadFromFile(inputName + "_players.idx");
		eventNames.ReadFromFile(inputName + "_events.idx");
		dateNames.ReadFromFile(inputName + "_dates.idx");
		siteNames.ReadFromFile(inputName + "_sites.idx");
		ecoNames.ReadFromFile(inputName + "_ecos.idx");
		playerElos.ReadFromFile(inputName + "_elos.idx");
	}
};

extern vector<string> output;


extern const char* memFileData;
extern GameIndex *start;

extern GameIndex **arrayGi;

extern bool pgnMode;// = true;

class MemPool;
class CFileMemPool;
class ChessBoard;
class Line; // from notation canvas

#include "MemFile.h"

bool CopyMemFile(const char* source, const char* dest, bool overwrite, ChessBoard *board);
bool WriteVM_PGNIndexFile(string &outputfileName, GameIndex* pStart);
const char* IndexVM_PGN(const char* sourceName, const char* dest, bool overwrite, GameIndex *start_game, GameIndex **arrayGii, InputMemoryFile *source_mf_p, int &game_count, MemPool **mempool);
string GetHeaderData( string &matchName, GameIndex *temp, char* data );
void LoadCompressedLists(string source, ChessBoard *board);
bool NewLine(const char* data, int i);
bool NextLinePattern(const char* data, int i);


class VarMoveNode
{
public:

	VarMoveNode()
	{
		whitesMove = false;
		fen = "";
		pgnMove = "";
	}

	VarMoveNode(string &moveStr, MyChessMove& mv, string& Fen, int white_black, string movNum )
	{
		if( white_black == 1 )
			whitesMove = true;
		else whitesMove = false;
		//whitesMove = false;
		fen = Fen;
		pgnMove = moveStr;
		move = mv;
		info = movNum;
		startX = 0;
		startY = 0;
		width = 0;
		height = 0;
		selected = false;
	}

	~VarMoveNode()
	{

	}

	int startX;
	int startY;
	int width;
	int height;

	string info;
	string pgnMove;
	MyChessMove move;
	string fen;
	bool whitesMove;
	bool selected;

};


namespace PGN {

template<typename Operator>
void tokenize(Operator& op, const char* input, const char* delimiters) {
  const char* s = input;
  const char* e = s;
  while (*e != 0) {
    e = s;
    while (*e != 0 && strchr(delimiters, *e) == 0) ++e;
    if (e - s > 0) {
      op(s, e - s);
    }
    s = e + 1;
  }
}


template<typename Operator>
void tokenize2(Operator& op, const char* input, const char* delim)
{
  const char* s = input;
  const char* e = s;
  while (*e != 0) {
    e = s;
    while (*e != 0 && strchr(delim, *e) == 0) 
	{
		++e;
	}
	if( *e == '(' || *e == ')' || *e == '{' || *e == '}')
	{
		if( e-s > 0)
		{
			op(s, e - s);
			op(e, 1);
			
		}
		else
			op(e, 1);

		s = e + 1;
	}
	else
	{
		if (e - s > 0) {
			op(s, e - s);
		}
		s = e + 1;
	}
  }
}

//using namespace MY_GLOBAL_HASH_SPACE;

class Appender : public std::vector<std::string> {
public:
  void operator() (const char* s, unsigned length) { 
    this->push_back(std::string(s,length));
  }
};

class HeaderElem
{
public:
	string data;

};

class Move;

class Variation
{
public:
	Variation(){
		after_whites_move = false;

	}

	~Variation()
	{
		vecMoves.clear();
	}
	bool after_whites_move;
	vector<Move> vecMoves;
};

class Comment
{
public:
	Comment()
	{
		after_whites_move = false;
	}
	~Comment()
	{
		text.clear();
	}

	void operator=(Comment &rhs)
	{
		after_whites_move = rhs.after_whites_move;
		for( unsigned int i = 0; i < rhs.text.size(); i++ )
			text.push_back(rhs.text[i]);
	}
	bool after_whites_move;
	//string text;
	vector<string> text;
};
//ofstream of;


class MoveInserter
{
public:

	MoveInserter( string pgn_string, int white_black, string fen, MyChessMove* epn=0,
		string epn_string="")
		
	{
		m_pgn_string = pgn_string;
		if(( white_black == 0 ) || (white_black == 2))
		{
			m_white_black = 0;
		}
		else 
		{
			m_white_black = 1;
		}
		m_fen = fen;
		if( epn )
			m_epn = *epn;

		vector<string> vec_fen;

		/*istringstream iss(fen);
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
					back_inserter<vector<string> >(vec_fen));
					*/
		int last_space = fen.find_last_of(" ");
		//int
			moveNum = fen.substr(last_space+1, fen.length() - last_space+1).c_str();
		//int movNum = atoi(vec_fen[5].c_str());
		//movNum--;
		//char buff[4];
		//_itoa(movNum, buff, 10);
		
		//	moveNum = buff;
			//moveNum = vec_fen[5];

	}

	string m_fen;
	MyChessMove m_epn;
	string m_epn_string;
	string m_pgn_string;
	string moveNum;
	int m_white_black;
};



#define PIECE_FLAG 0x00000001
#define FILE_FLAG 0x00000010
#define RANK_FLAG 0x00000100
#define CAPTURE_FLAG 0x00001000
#define FILE_DEST_FLAG 0x00010000
#define RANK_DEST_FLAG 0x00100000
#define PROMOTE_FLAG 0x01000000
#define GLYPHS 0x10000000

#define INVALID_1 0x01000001
#define INVALID_2 0x00111110
#define INVALID_3 0x00000001

	
#define TEST_FLAG(X) (patternFlag & X)
#define SET_FLAG(X) patternFlag |= X
#define UNSET_FLAG(X) patternFlag &= ~X
#define RESET() patternFlag=0x00000000
#define PATTERN_ZERO() (patternFlag == 0x00000000)



#define MOVE_FLAG_VISITED  0x01
#define MOVE_FLAG_DISPLAY_VARIATIONS  0x02
#define MOVE_FLAG_AT_END_WHITE  0x04
#define MOVE_FLAG_AT_END_BLACK  0x08
#define MOVE_FLAG_HAS_FEN_WHITE  0x10
#define MOVE_FLAG_HAS_FEN_BLACK  0x20
#define MOVE_FLAG_X 0x40
#define MOVE_FLAG_Y  0x80




class Move
{
public:
	//friend class Game;
	Move()
	{
		//variation_count_white = 0;
		//variation_count_black = 0;
//		x = y = 0;
		pNext = NULL;
		pPrev = NULL;
		//hasFenWhite = false;
		//hasFenBlack = false;
		white ="";
		//white.rea
		black ="";
		pVarWhite = NULL;
		pVarBlack = NULL;
		pUp = NULL;
		/*for( int i = 0; i < 64; i++ )
		{
			pVariationBlack[i] = NULL;
			pVariationWhite[i] = NULL;
		}*/

//		atEndWhite = false;
//		atEndBlack = false;

		//displayVariations = true;
		//SetRenderColor(0,0,0);
		//SetRenderColorWhite(0,0,0);
		//SetRenderColorBlack(0,0,0);
		rm = 0;
		rmw = 0;
		rmb = 0;
		//visited = false;
		move_cp_white = 0;
		move_cp_black = 0;
		depthWhite = 0;
		depthBlack = 0;

		this->ResetMoveFlag();
		if( !variation_auto_fold )
			this->SetMoveFlag( MOVE_FLAG_DISPLAY_VARIATIONS );
	}

	~Move()
	{

		
		if( pNext )
		{
			delete pNext;
			pNext = NULL;
		}

		if( pVarWhite )
		{			
			delete pVarWhite;
			pVarWhite = NULL;
		}

		if( pVarBlack )
		{
			delete pVarBlack;
			pVarBlack = NULL;
		}
	}

	void SetLine( Line* pLine )
	{
		m_line = pLine;
	}


	Move* GetVariationRoot()
	{
		Move* temp = this;
		while( temp->pUp == NULL )
		{
			if( temp->pPrev )
				temp = temp->pPrev;
			else break;
		}
		return temp->pUp;
	}

	bool IsThisBranchOfMove(Move* pMv)
	{
		if( !pMv )
			return false;

		Move* temp = GetVariationRoot();
		
		while( (temp!=NULL) && 
			   (pMv != temp) && 
			   (temp->pPrev != NULL) )
		{
			temp = temp->GetVariationRoot();
		}
		
		if( temp == pMv )
			return true;
		else
			return false;
	}


	bool IsThisMainLine()
	{
		return (this->GetVariationRoot()==NULL);
	}

	string GetCurrentVariationAsString()
	{
		Move* temp = this;
		while( temp->pUp == NULL )
		{
			temp = temp->pPrev;
		}

		ostringstream of;
		temp->SaveLineToStream(of);

		return of.str();

	}

	int GetMoveCount(int &num)
	{
		Move* temp = this;

		while( temp )
		{
			if( temp->pVarWhite )
			{
				Move* temp2 = temp->pVarWhite;
				while( temp2 )
				{
					num = temp2->GetMoveCount(num);
					temp2 = temp2->pVarWhite;
				}
			}
			if( temp->pVarBlack )
			{
				Move* temp2 = temp->pVarBlack;
				while( temp2 )
				{
					num = temp2->GetMoveCount(num);
					temp2 = temp2->pVarBlack;
				}
			}
			num++;
			temp = temp->pNext;
		}

		return num;
	}

	/*int GetVariationMoveCount(int &inpt)
	{
		if( 
	}*/

	int CountMovesToStart(int black_white, int &maxValue)
	{
		int retVal = 0;
		Move* temp = this;
		

		if( black_white != 1 )
		{
			maxValue = abs(this->move_cp_black);	
			retVal++;

			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					int absUpWhites = abs(temp->move_cp_white);
					maxValue = maxValue > absUpWhites ? maxValue : absUpWhites;
					retVal++;

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return retVal;		
			}
			else
			{
				int absWhite = abs(temp->move_cp_white);
				maxValue = maxValue > absWhite ? maxValue : absWhite;
				
				retVal++;

				if( temp->pUp != NULL )
					temp = temp->pUp;
			}
		}
		else
		{
			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					int absWhite = abs(temp->move_cp_white);
					maxValue = maxValue > absWhite ? maxValue : absWhite;
					retVal++;
					
					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return retVal;
				
			}
			else
			{
				int absWhite = abs(temp->move_cp_white);
				maxValue = maxValue > absWhite ? maxValue : absWhite;
				retVal++;

				if( temp->pUp != NULL )
					temp = temp->pUp;
				//retVal.insert(0, movWhite );
			}
		}

		temp = temp->pPrev;
//		retVal++;
		if( temp->pPrev == 0 )
			return retVal;


		while( temp->pPrev != NULL)
		{
			int absBlack = abs(temp->move_cp_black);
			maxValue = maxValue > absBlack ? maxValue : absBlack;
			retVal++;

			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					int absWhite = abs(temp->move_cp_white);
					maxValue = maxValue > absWhite ? maxValue : absWhite;
					retVal++;

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return retVal;
			}
			else
			{
				int absWhite = abs(temp->move_cp_white);
				maxValue = maxValue > absWhite ? maxValue : absWhite;
				retVal++;

				if( temp->pUp != NULL )
					temp = temp->pUp;

			}
			if( temp->pPrev )
				temp = temp->pPrev;
			else break;
		}

		return retVal;
	}



	string GetSanStringsFromStartWithSpace(int black_white)
	{
		string retVal = "";
		Move* temp = this;
		
		if( black_white != 1 )
		{
			string movBlack = " ";
			movBlack += temp->black;//temp->blackMove.toString();
			retVal += movBlack;   // = " e7e5\n";


			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = " ";
					movWhite += temp->white;//Move.toString();
					
					retVal.insert(0, movWhite );

					string inf_m = " ";
					inf_m += temp->info + ".";
					retVal.insert(0, inf_m );

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return "error";
				
			}
			else
			{
				string movWhite = " ";
				movWhite += temp->white;//Move.toString();

				retVal.insert(0, movWhite );

				string inf_m = " ";
				inf_m += temp->info + ".";
				retVal.insert(0, inf_m );

				if( temp->pUp != NULL )
					temp = temp->pUp;
			}
		}
		else
		{
			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = " ";
					movWhite += temp->white;//.toString();

					retVal.insert(0, movWhite );
					
					string inf_m = " ";
					inf_m += temp->info + ".";
					retVal.insert(0, inf_m );

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return "error";
				
			}
			else
			{
				string movWhite = " ";
				movWhite += temp->white;//Move.toString();
				retVal += movWhite;

				string inf_m = " ";
				inf_m += temp->info + ".";
				retVal.insert(0, inf_m );

				if( temp->pUp != NULL )
					temp = temp->pUp;
				//retVal.insert(0, movWhite );
			}
		}

		temp = temp->pPrev;
		if( !temp || temp->pPrev == NULL )
			return retVal;

		while( temp->pPrev != NULL)
		{

			string movBlack = " ";
			movBlack += temp->black;//Move.toString();
			retVal.insert(0, movBlack );

			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = " ";
					movWhite += temp->white;//Move.toString();

					retVal.insert(0, movWhite );

					string inf_m = " ";
					inf_m += temp->info + ".";
					retVal.insert(0, inf_m );

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return "error";
			}
			else
			{
				string movWhite = " ";
				movWhite += temp->white;//Move.toString();

				retVal.insert(0, movWhite );

				string inf_m = " ";
				inf_m += temp->info + ".";
				retVal.insert(0, inf_m );

				if( temp->pUp != NULL )
					temp = temp->pUp;

			}
			if( temp->pPrev )
				temp = temp->pPrev;
			else break;
		}

		return retVal;
		

	}

	string GetSanStringsFromStart(int black_white)
	{
		string retVal = "";
		Move* temp = this;
		
		if( black_white != 1 )
		{
			string movBlack = " ";
			movBlack += temp->black;//temp->blackMove.toString();
			retVal += movBlack;   // = " e7e5\n";


			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = "";
					movWhite += temp->white;//Move.toString();
					
					retVal.insert(0, movWhite );

					string inf_m = " ";
					inf_m += temp->info + ".";
					retVal.insert(0, inf_m );

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return "error";
				
			}
			else
			{
				string movWhite = "";
				movWhite += temp->white;//Move.toString();

				retVal.insert(0, movWhite );

				string inf_m = " ";
				inf_m += temp->info + ".";
				retVal.insert(0, inf_m );

				if( temp->pUp != NULL )
					temp = temp->pUp;
			}
		}
		else
		{
			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = "";
					movWhite += temp->white;//.toString();

					retVal.insert(0, movWhite );
					
					string inf_m = " ";
					inf_m += temp->info + ".";
					retVal.insert(0, inf_m );

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return "error";
				
			}
			else
			{
				string movWhite = "";
				movWhite += temp->white;//Move.toString();
				retVal += movWhite;

				string inf_m = " ";
				inf_m += temp->info + ".";
				retVal.insert(0, inf_m );

				if( temp->pUp != NULL )
					temp = temp->pUp;
				//retVal.insert(0, movWhite );
			}
		}

		temp = temp->pPrev;
		if( !temp || temp->pPrev == NULL )
			return retVal;

		while( temp->pPrev != NULL)
		{

			string movBlack = " ";
			movBlack += temp->black;//Move.toString();
			retVal.insert(0, movBlack );

			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = "";
					movWhite += temp->white;//Move.toString();

					retVal.insert(0, movWhite );

					string inf_m = " ";
					inf_m += temp->info + ".";
					retVal.insert(0, inf_m );

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return "error";
			}
			else
			{
				string movWhite = "";
				movWhite += temp->white;//Move.toString();

				retVal.insert(0, movWhite );

				string inf_m = " ";
				inf_m += temp->info + ".";
				retVal.insert(0, inf_m );

				if( temp->pUp != NULL )
					temp = temp->pUp;

			}
			if( temp->pPrev )
				temp = temp->pPrev;
			else break;
		}

		return retVal;
		

	}

	void GetCompressedMoveIntegersFromStart(int black_white, vector<unsigned short int> *c_mvs)
	{
		Move* temp = this;
		
		if( black_white != 1 )
		{
			temp->blackMove.Compress();
			c_mvs->push_back(temp->blackMove.packed);

			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;

					temp->whiteMove.Compress();
					c_mvs->push_back(temp->whiteMove.packed);

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return;// "error";
				
			}
			else
			{
				temp->whiteMove.Compress();
				c_mvs->push_back(temp->whiteMove.packed);
				
				if( temp->pUp != NULL )
					temp = temp->pUp;
			}
		}
		else
		{
			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;

					temp->whiteMove.Compress();
					c_mvs->push_back(temp->whiteMove.packed);

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return;// "error";
				
			}
			else
			{
				temp->whiteMove.Compress();
				c_mvs->push_back(temp->whiteMove.packed);
				
				if( temp->pUp != NULL )
					temp = temp->pUp;
				//retVal.insert(0, movWhite );
			}
		}

		temp = temp->pPrev;
		if( !temp || temp->pPrev == NULL )
			return;// retVal;

		while( temp->pPrev != NULL)
		{
			temp->blackMove.Compress();
			c_mvs->push_back(temp->blackMove.packed);

			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
	
					temp->whiteMove.Compress();
					c_mvs->push_back(temp->whiteMove.packed);

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return;// "error";
			}
			else
			{
				temp->whiteMove.Compress();
				c_mvs->push_back(temp->whiteMove.packed);
				
				if( temp->pUp != NULL )
					temp = temp->pUp;

			}
			if( temp->pPrev )
				temp = temp->pPrev;
			else break;
		}
		return;// retVal;
	}

	// gets the move strings from the start in inverse order
	string GetMoveStringsFromStart(int black_white)
	{
		string retVal = "";
		Move* temp = this;
		
		if( black_white != 1 )
		{
			string movBlack = " ";
			movBlack += temp->blackMove.toString();
			retVal += movBlack;   // = " e7e5\n";


			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = " ";
					movWhite += temp->whiteMove.toString();

					retVal.insert(0, movWhite );

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return "error";
				
			}
			else
			{
				string movWhite = " ";
				movWhite += temp->whiteMove.toString();

				retVal.insert(0, movWhite );

				if( temp->pUp != NULL )
					temp = temp->pUp;
			}
		}
		else
		{
			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = " ";
					movWhite += temp->whiteMove.toString();

					retVal.insert(0, movWhite );
					
					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return "error";
				
			}
			else
			{
				string movWhite = " ";
				movWhite += temp->whiteMove.toString();
				retVal += movWhite;

				if( temp->pUp != NULL )
					temp = temp->pUp;
				//retVal.insert(0, movWhite );
			}
		}

		temp = temp->pPrev;
		if( !temp || temp->pPrev == NULL )
			return retVal;

		while( temp->pPrev != NULL)
		{

			string movBlack = " ";
			movBlack += temp->blackMove.toString();
			retVal.insert(0, movBlack );

			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = " ";
					movWhite += temp->whiteMove.toString();

					retVal.insert(0, movWhite );

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return "error";
			}
			else
			{
				string movWhite = " ";
				movWhite += temp->whiteMove.toString();

				retVal.insert(0, movWhite );

				if( temp->pUp != NULL )
					temp = temp->pUp;

			}
			if( temp->pPrev )
				temp = temp->pPrev;
			else break;
		}

		return retVal;
		

	}

	Move* GetMoveChainFromHere(int color)
	{
		Move* temp = this;
		Move* newMove = new Move();
		newMove->CopyNode(temp);

		if( color == WHITE )
		{
			newMove->UnsetMoveFlag( MOVE_FLAG_Y );
			newMove->black = "";
			//newMove->blackMove 
		}
		else
		{
			newMove->SetMoveFlag( MOVE_FLAG_X );
			newMove->SetMoveFlag( MOVE_FLAG_Y );
		}

		while( temp->pPrev )
		{
			temp = temp->pPrev;

			Move* newMoveV = new Move();
			newMoveV->CopyNode(temp);
			newMoveV->pNext = newMove;
			newMove->pPrev = newMoveV;
			newMove = newMoveV;
			
		}

		return newMove;
	}

	Move* GetMoveChainFromRootToVariation()
	{
		Move* temp = this;
		while( temp->pNext )
		{
			temp = temp->pNext;
		}
		Move* newMove = new Move();
		newMove->CopyNode(temp);

		while( temp->pPrev )
		{
			temp = temp->pPrev;

			Move* newMoveV = new Move();
			newMoveV->CopyNode(temp);
			newMoveV->pNext = newMove;
			newMove->pPrev = newMoveV;
			newMove = newMoveV;
			
		}



		return newMove;
	}


// gets the move strings from the start in inverse order
	string GetMoveStringsFromFenSetupPosition(int black_white)
	{
		string retVal = "";
		Move* temp = this;
		
		if( black_white != 1 )
		{
			string movBlack = " ";
			movBlack += temp->blackMove.toString();
			retVal += movBlack;   // = " e7e5\n";


			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = " ";
					string movWhiteStr = temp->whiteMove.toString();
					if( movWhiteStr != "" )
					{	
						movWhite += movWhiteStr;
						retVal.insert(0, movWhite );
					}

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return retVal;
				
			}
			else
			{
				string movWhite = " ";
				movWhite += temp->whiteMove.toString();

				retVal.insert(0, movWhite );

				if( temp->pUp != NULL )
					temp = temp->pUp;
			}
		}
		else
		{
			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = " ";
					movWhite += temp->whiteMove.toString();

					retVal.insert(0, movWhite );
					
					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return "";
				
			}
			else
			{
				string movWhite = " ";
				movWhite += temp->whiteMove.toString();
				retVal += movWhite;

				if( temp->pUp != NULL )
					temp = temp->pUp;
				//retVal.insert(0, movWhite );
			}
		}

		temp = temp->pPrev;
		if( temp->pPrev == 0 )
			return retVal;

		while( temp->pPrev != NULL)
		{

			string movBlack = " ";
			movBlack += temp->blackMove.toString();
			retVal.insert(0, movBlack );

			if( temp->white == "" )
			{
				if( temp->pUp != NULL )
				{
					temp = temp->pUp;
					string movWhite = " ";
					string tempMovWhiteStr = temp->whiteMove.toString();
					if( (tempMovWhiteStr != "") )
					{
						movWhite += temp->whiteMove.toString();
						retVal.insert(0, movWhite );
					}

					if( temp->pUp != NULL )
						temp = temp->pUp;
				}
				else return retVal;
			}
			else
			{
				string movWhite = " ";
				movWhite += temp->whiteMove.toString();

				retVal.insert(0, movWhite );

				if( temp->pUp != NULL )
					temp = temp->pUp;

			}
			if( temp->pPrev )
				temp = temp->pPrev;
			else break;
		}

		return retVal;
		

	}



	int GetVariationRootColor()
	{
		Move* temp = this;
		while( temp->pUp == NULL )
		{
			if( temp->pPrev )
				temp = temp->pPrev;
			else break;
		}
		
		if( temp->pUp )
		{
			Move* tpUp = temp->pUp;
			if( tpUp->pVarWhite )
			{
				while( tpUp && (tpUp != temp) )
				{
					tpUp = tpUp->pVarWhite;
				}
				if( tpUp )
				{
					return 0;
				}
			}
			else return 1;
		}

		return 2; // unreachable
	}


	bool MoveIsVariationRoot(Move* pMoveInVar)
	{
		Move* pVarRoot = GetVariationRoot();
		if( pMoveInVar == pVarRoot )
			return true;
		else return false;
	}

	

	int InsertHashWhite(Move* pMov )
	{
		HashEntry hEntry;
		hEntry.hits = 1;
		hEntry.key = pMov->zhashKeyWhite;
		std::unordered_map<uint64, HashEntry>::iterator i1;
		i1 = hash_table.find(pMov->zhashKeyWhite);
		if( i1 != hash_table.end() && i1->second.hits >= 1 )
		{
			i1->second.hits++;
			return i1->second.hits;
		}
		/*if( hash_table[pMov->zhashKeyWhite].hits > 0 )
		{
			//hash_table[pMov->zhashKeyWhite].hits++;
			return hash_table[pMov->zhashKeyWhite].hits++;
		}*/
		else
		{
			hash_table.insert(std::make_pair(pMov->zhashKeyWhite, hEntry));
		}
		return hEntry.hits;
	}


	int InsertHashBlack(Move* pMov )
	{
		HashEntry hEntry;
		hEntry.hits = 1;
		hEntry.key = pMov->zhashKeyBlack;
		std::unordered_map<uint64, HashEntry>::iterator i1;
		i1 = hash_table.find(pMov->zhashKeyBlack);
		if( i1 != hash_table.end() && i1._Ptr && i1->second.hits >= 1 )
		{
			i1->second.hits++;
			return i1->second.hits;
		}
		/*if( hash_table[pMov->zhashKeyBlack].hits > 0 )
		{
			//hash_table[pMov->zhashKeyWhite].hits++;
			return hash_table[pMov->zhashKeyBlack].hits++;
		}
		else*/
		{
			hash_table.insert(std::make_pair(pMov->zhashKeyBlack, hEntry));
		}
		return hEntry.hits;
	}


	//	1. e4 e5
	//  insert variation of e4 ... move d4
	//  x=1 y=1
	//  d4!=e4 add move
	// insert variation of black into d4 ... move d5
	//	x=1 y=0
	//  d4 d5
	// insert variation into d5 ... move c4
	// x=1 y=1
	// 
	/*
#define MOVE_FLAG_VISITED 1
#define MOVE_FLAG_DISPLAY_VARIATIONS 2
#define MOVE_FLAG_AT_END_WHITE 4
#define MOVE_FLAG_AT_END_BLACK 8
#define MOVE_FLAG_HAS_FEN_WHITE 16
#define MOVE_FLAG_HAS_FEN_BLACK 32
#define MOVE_FLAG_X 64
#define MOVE_FLAG_Y 128	
	*/
	Move* InsertMove(MoveInserter *movPgn, int* hits = 0)
	{
		if( movPgn->m_white_black == 0 )
		{
			if( /*x!= 0*/ this->MoveFlagIsSet(MOVE_FLAG_X) )
			{
				if( this->MoveFlagIsSet(MOVE_FLAG_Y) )
				{
					if( pNext == NULL  )
					{
						if( movPgn->moveNum == this->info )
						{
							if( white == movPgn->m_pgn_string )
							{
								return this;
							}
							else
							{
								Move* temp = pVarWhite;
								while( temp )
								{
									if( temp->white == movPgn->m_pgn_string )
									{
										return temp;
									}
									temp = temp->pVarWhite;
								}
								Move* pMov = getWhiteVariationForParse();
								pMov->white = movPgn->m_pgn_string;
						
								//if( this->atEndBlack )
								//	this->atEndBlack = false;
								//pMov->atEndWhite = true;
								//if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_BLACK ))
								//	this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );						
								pMov->SetMoveFlag(MOVE_FLAG_AT_END_WHITE);

								//pMov->x = 1;
								pMov->SetMoveFlag(MOVE_FLAG_X);
								char buf[10];
								_itoa(atoi(info.c_str()), buf, 10);
							
								pMov->info = movPgn->moveNum;
								pMov->m_fenWhite = movPgn->m_fen;
								pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
						
								*hits = InsertHashWhite(pMov);

								pMov->whiteMove = movPgn->m_epn;
							//his-> = pMov;
								return pMov;
							}
						}
						else
						{
							Move* pMov = new Move;
							pMov->pPrev = this;
						
							//if( this->atEndBlack )
							//	this->atEndBlack = false;
							//pMov->atEndWhite = true;
							
							if( this->MoveFlagIsSet(MOVE_FLAG_AT_END_BLACK) )
								this->UnsetMoveFlag(MOVE_FLAG_AT_END_BLACK);
							pMov->SetMoveFlag(MOVE_FLAG_AT_END_WHITE);
							
							
							this->pNext = pMov;
							pMov->white = movPgn->m_pgn_string;
							//pMov->x = 1;
							pMov->SetMoveFlag(MOVE_FLAG_X);
							//if(  != "" )
							char buf[10];
							_itoa(atoi(info.c_str())+1, buf, 10);
							pMov->info = movPgn->moveNum;
							pMov->m_fenWhite = movPgn->m_fen;
							pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
						
							*hits = InsertHashWhite(pMov);
							//Game::hash_table.
							pMov->whiteMove = movPgn->m_epn;
							return pMov;
						}
					}
					else if( this->pNext->white == "" && (movPgn->moveNum != this->info) )
					{
						delete this->pNext;
						Move* pMov = new Move;
						pMov->pPrev = this;
						
						//if( this->atEndBlack )
						//	this->atEndBlack = false;
						//pMov->atEndWhite = true;

						if( this->MoveFlagIsSet(MOVE_FLAG_AT_END_BLACK) )
							this->UnsetMoveFlag(MOVE_FLAG_AT_END_BLACK);
						
						pMov->SetMoveFlag(MOVE_FLAG_AT_END_WHITE);


						this->pNext = pMov;
						pMov->white = movPgn->m_pgn_string;
						
						//pMov->x = 1;
						pMov->SetMoveFlag(MOVE_FLAG_X);
						//if(  != "" )
						char buf[10];
						_itoa(atoi(info.c_str())+1, buf, 10);
						pMov->info = movPgn->moveNum;
						pMov->m_fenWhite = movPgn->m_fen;
						pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
						
						*hits = InsertHashWhite(pMov);						
						
						pMov->whiteMove = movPgn->m_epn;
						return pMov;
					}
					else if( movPgn->m_pgn_string != this->pNext->white &&(movPgn->moveNum != this->info))
					{
						Move* temp = pNext->pVarWhite;
						while( temp )
						{
							if( temp->white == movPgn->m_pgn_string )
							{
								return temp;
							}
							temp = temp->pVarWhite;
						}
						Move* pMov = pNext->getWhiteVariationForParse();
						pMov->white = movPgn->m_pgn_string;
						
						//if( this->atEndBlack )
						//	this->atEndBlack = false;
						//pMov->atEndWhite = true;

						if( this->MoveFlagIsSet(MOVE_FLAG_AT_END_BLACK) )
							this->UnsetMoveFlag(MOVE_FLAG_AT_END_BLACK);
						pMov->SetMoveFlag(MOVE_FLAG_AT_END_WHITE);
						
						//pMov->x = 1;
						pMov->SetMoveFlag(MOVE_FLAG_X);
						char buf[10];
						_itoa(atoi(info.c_str()), buf, 10);
							
						pMov->info = movPgn->moveNum;
						pMov->m_fenWhite = movPgn->m_fen;
						pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
						
						*hits = InsertHashWhite(pMov);

						pMov->whiteMove = movPgn->m_epn;
					//his-> = pMov;
						return pMov;
					}
					else if( (movPgn->moveNum == this->info) &&(white!= movPgn->m_pgn_string))
					{
						Move* temp = pVarWhite;
						while( temp )
						{
							if( temp->white == movPgn->m_pgn_string )
							{
								return temp;
							}
							temp = temp->pVarWhite;
						}
						Move* pMov = getWhiteVariationForParse();
						pMov->white = movPgn->m_pgn_string;
						
						//if( this->atEndBlack )
						//	this->atEndBlack = false;
						//pMov->atEndWhite = true;

						pMov->SetMoveFlag(MOVE_FLAG_AT_END_WHITE);

						//pMov->x = 1;
						pMov->SetMoveFlag(MOVE_FLAG_X);

						char buf[10];
						_itoa(atoi(info.c_str()), buf, 10);
							
						pMov->info = movPgn->moveNum;
						pMov->m_fenWhite = movPgn->m_fen;
						pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
						
						*hits = InsertHashWhite(pMov);

						pMov->whiteMove = movPgn->m_epn;
					//his-> = pMov;
						return pMov;
					}
					else if( movPgn->m_pgn_string == this->pNext->white )
					{
						return this->pNext;
					}
					//pMov->info = string((atoi(this->info.c_str())+1));
				}
				
				else if( movPgn->m_pgn_string != this->white )
				{
					Move* temp = pVarWhite;
					while( temp )
					{
						if( movPgn->m_pgn_string == this->white )
						{
							return temp;
						}
						temp = temp->pVarWhite;
					}
					Move* pMov = getWhiteVariationForParse();
					pMov->white = movPgn->m_pgn_string;
					//if( this->atEndBlack )
					//	this->atEndBlack = false;
					//pMov->atEndWhite = true;

					if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_BLACK ) )
						this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					pMov->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
					
					//pMov->x = 1;
					pMov->SetMoveFlag(MOVE_FLAG_X);

					pMov->info = this->info;
					pMov->m_fenWhite = movPgn->m_fen;
					pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
					
					*hits = InsertHashWhite(pMov);

					pMov->whiteMove = movPgn->m_epn;
					return pMov;
				}
				else return this;
			}
			else
			{
				//x = 1;
				
				SetMoveFlag(MOVE_FLAG_X);

				white = movPgn->m_pgn_string;
				//this->atEndWhite = true;

				this->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
				
				//if( this->pPrev && this->pPrev->pPrev == NULL )
				//	info = "1";
				info = movPgn->moveNum;
				m_fenWhite = movPgn->m_fen;
				zhashKeyWhite = hash_from_fen((char*)m_fenWhite.c_str());
				
				*hits = InsertHashWhite(this);
				
				whiteMove = movPgn->m_epn;
				//pMove->info = this->info;
			}
			return this;
		}
		else
		{
			if( this->MoveFlagIsSet(MOVE_FLAG_Y) )
			{
				if( movPgn->m_pgn_string != this->black )
				{
					Move* temp = pVarBlack;
					while( temp )
					{
						if( temp->black == movPgn->m_pgn_string )
						{
							return temp;
						}
						temp = temp->pVarBlack;
					}
					Move* pMov = getBlackVariationForParse();
					pMov->black = movPgn->m_pgn_string;
					//if( this->atEndWhite )
					//	this->atEndWhite = false;
					//pMov->atEndBlack = true;

					if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_WHITE ) )
						this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
					pMov->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );

					//pMov->x = 1;
					//pMov->y = 1;
					
					pMov->SetMoveFlag(MOVE_FLAG_X);
					pMov->SetMoveFlag(MOVE_FLAG_Y);

					char buf[10];
					_itoa(atoi(info.c_str()), buf, 10);
					pMov->info = buf;//this->info;//movPgn->moveNum;
					//info = 
					pMov->m_fenBlack = movPgn->m_fen;
					pMov->zhashKeyBlack = hash_from_fen((char*)pMov->m_fenBlack.c_str());
					
					*hits = InsertHashBlack(pMov);
					
					pMov->blackMove = movPgn->m_epn;
					return pMov;
				}
			}
			else
			{
				black = movPgn->m_pgn_string;
				/*if( this->atEndWhite )
					this->atEndWhite = false;
				this->atEndBlack = true;
				*/
				if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_WHITE ))
					this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
				SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
			//	if( this->MoveFlagIsSet(
				//x = 1;
				//y = 1;
				SetMoveFlag(MOVE_FLAG_X);
				SetMoveFlag(MOVE_FLAG_Y);
				m_fenBlack = movPgn->m_fen;
				zhashKeyBlack = hash_from_fen((char*)m_fenBlack.c_str());
				
				*hits = InsertHashBlack(this);
				
				blackMove = movPgn->m_epn;
			}
			return this;
		}
		return this;
	}

	//	1. e4 e5
	//  insert variation of e4 ... move d4
	//  x=1 y=1
	//  d4!=e4 add move
	// insert variation of black into d4 ... move d5
	//	x=1 y=0
	//  d4 d5
	// insert variation into d5 ... move c4
	// x=1 y=1
	// 
	Move* InsertMoveToVariation(MoveInserter *movPgn, int *hits = 0)
	{
		if( movPgn->m_white_black == 0 )
		{

			if( /*x!= 0*/this->MoveFlagIsSet(MOVE_FLAG_X) )
			{
				if( this->MoveFlagIsSet(MOVE_FLAG_Y) )
				{
					if( pNext == NULL  )
					{
						if( movPgn->moveNum == this->info )
						{
							if( white == movPgn->m_pgn_string )
							{
								return this;
							}
							else
							{
								Move* temp = pVarWhite;
								while( temp )
								{
									if( temp->white == movPgn->m_pgn_string )
									{
										return temp;
									}
									temp = temp->pVarWhite;
								}
								Move* pMov = getWhiteVariationForParse();
								pMov->white = movPgn->m_pgn_string;
						
								//if( this->atEndBlack )
								//	this->atEndBlack = false;
								//pMov->atEndWhite = true;
								
								if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_BLACK ))
									this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
								pMov->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						
								pMov->SetMoveFlag(MOVE_FLAG_X);
								//pMov->x = 1;
								char buf[10];
								_itoa(atoi(info.c_str())+1, buf, 10);
							
								pMov->info = buf;//movPgn->moveNum;
								pMov->m_fenWhite = movPgn->m_fen;
								pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
						
								if( *hits != -1 )
									*hits = InsertHashWhite(pMov);
						
								pMov->whiteMove = movPgn->m_epn;
							//his-> = pMov;
								return pMov;
							}
						}
						else
						{
							Move* pMov = new Move;
							pMov->pPrev = this;
						
							//if( this->atEndBlack )
							//	this->atEndBlack = false;
							//pMov->atEndWhite = true;

							if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_BLACK ))
								this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
							pMov->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );

							this->pNext = pMov;
							pMov->white = movPgn->m_pgn_string;
							
							//pMov->x = 1;
							pMov->SetMoveFlag( MOVE_FLAG_X );
							//if(  != "" )
							char buf[10];
							_itoa(atoi(info.c_str())+1, buf, 10);
							pMov->info = buf;//movPgn->moveNum;
							pMov->m_fenWhite = movPgn->m_fen;
							pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
						
							if( *hits != -1 )
								*hits = InsertHashWhite(pMov);
						
							pMov->whiteMove = movPgn->m_epn;
							return pMov;
						}
					}
					else if( this->pNext->white == "" && (movPgn->moveNum != this->info))
					{
						delete this->pNext;
						Move* pMov = new Move;
						pMov->pPrev = this;
						
						//if( this->atEndBlack )
						//	this->atEndBlack = false;
						//pMov->atEndWhite = true;

						if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_BLACK ))
							this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						pMov->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );

						this->pNext = pMov;
						pMov->white = movPgn->m_pgn_string;
						//pMov->x = 1;

						pMov->SetMoveFlag( MOVE_FLAG_X );
						//if(  != "" )
						char buf[10];
						_itoa(atoi(info.c_str())+1, buf, 10);
						pMov->info = buf;//movPgn->moveNum;
						pMov->m_fenWhite = movPgn->m_fen;
						pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
						
						if( *hits != -1 )
							*hits = InsertHashWhite(pMov);
						
						pMov->whiteMove = movPgn->m_epn;
						return pMov;
					}
					else if( (movPgn->m_pgn_string != this->pNext->white)&& (movPgn->moveNum != this->info) )
					{
						Move* temp = pNext->pVarWhite;
						while( temp )
						{
							if( temp->white == movPgn->m_pgn_string )
							{
								return temp;
							}
							temp = temp->pVarWhite;
						}
						Move* pMov = pNext->getWhiteVariationForParse();
						pMov->white = movPgn->m_pgn_string;
						
						//if( this->atEndBlack )
						//	this->atEndBlack = false;
						//pMov->atEndWhite = true;

						if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_BLACK ))
							this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						pMov->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						
						pMov->SetMoveFlag( MOVE_FLAG_X );

						//pMov->x = 1;
						char buf[10];
						_itoa(atoi(info.c_str())+1, buf, 10);
							
						pMov->info = buf;//movPgn->moveNum;
						pMov->m_fenWhite = movPgn->m_fen;
						pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
						
						if( *hits != -1 )
							*hits = InsertHashWhite(pMov);
						
						pMov->whiteMove = movPgn->m_epn;
					//his-> = pMov;
						return pMov;
					}
					else if( (movPgn->moveNum == this->info) &&(white!= movPgn->m_pgn_string))
					{
						Move* temp = pVarWhite;
						while( temp )
						{
							if( temp->white == movPgn->m_pgn_string )
							{
								return temp;
							}
							temp = temp->pVarWhite;
						}
						Move* pMov = getWhiteVariationForParse();
						pMov->white = movPgn->m_pgn_string;
						
						//if( this->atEndBlack )
						//	this->atEndBlack = false;
						//pMov->atEndWhite = true;
						
						//if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_BLACK ))
						//	this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						pMov->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						
						pMov->SetMoveFlag( MOVE_FLAG_X );
						
						//pMov->x = 1;
						char buf[10];
						_itoa(atoi(info.c_str()), buf, 10);
							
						pMov->info = movPgn->moveNum;
						pMov->m_fenWhite = movPgn->m_fen;
						pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
						if( *hits != -1 )
							*hits = InsertHashWhite(pMov);

						pMov->whiteMove = movPgn->m_epn;
					//his-> = pMov;
						return pMov;
					}
					else if( movPgn->m_pgn_string == this->pNext->white )
					{
						return this->pNext;
					}
					//pMov->info = string((atoi(this->info.c_str())+1));
				}
				
				else if( movPgn->m_pgn_string != this->white )
				{
					Move* temp = pVarWhite;
					while( temp )
					{
						if( temp->white == movPgn->m_pgn_string )
						{
							return temp;
						}
						temp = temp->pVarWhite;
					}
					Move* pMov = getWhiteVariationForParse();
					pMov->white = movPgn->m_pgn_string;
					//if( this->atEndBlack )
					//	this->atEndBlack = false;
					//pMov->atEndWhite = true;

					if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_BLACK ))
						this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					pMov->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						
					pMov->SetMoveFlag( MOVE_FLAG_X );
					
					//pMov->x = 1;
					pMov->info = this->info;
					pMov->m_fenWhite = movPgn->m_fen;
					pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
					
					if( *hits != -1 )
						*hits = InsertHashWhite(pMov);
					
					pMov->whiteMove = movPgn->m_epn;
					return pMov;
				}
				else return this;
			}
			else
			{
				//x = 1;
				SetMoveFlag( MOVE_FLAG_X );

				white = movPgn->m_pgn_string;
				//this->atEndWhite = true;

				SetMoveFlag( MOVE_FLAG_AT_END_WHITE );

				char buf[10];
				_itoa(atoi(pPrev->info.c_str())+1, buf, 10);
				//if( this->pPrev && this->pPrev->pPrev == NULL )
				//	info = "1";
				info = buf;// movPgn->moveNum;
				m_fenWhite = movPgn->m_fen;
				zhashKeyWhite = hash_from_fen((char*)m_fenWhite.c_str());
			
				if( *hits != -1 )
					*hits = InsertHashWhite(this);
				
				whiteMove = movPgn->m_epn;
				//pMove->info = this->info;
			}
			return this;
		}
		else
		{
			if( /*y!= 0*/ this->MoveFlagIsSet( MOVE_FLAG_Y) )
			{
				if( movPgn->m_pgn_string != this->black )
				{
					Move* temp = pVarBlack;
					while( temp )
					{
						if( temp->black == movPgn->m_pgn_string )
						{
							return temp;
						}
						temp = temp->pVarBlack;
					}
					Move* pMov = getBlackVariationForParse();
					pMov->black = movPgn->m_pgn_string;
					//if( this->atEndWhite )
					//	this->atEndWhite = false;
					//pMov->atEndBlack = true;

					if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_WHITE ))
						this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
					pMov->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						
					pMov->SetMoveFlag( MOVE_FLAG_X );
					pMov->SetMoveFlag( MOVE_FLAG_Y );
					
					//pMov->x = 1;
					//pMov->y = 1;
					pMov->info = movPgn->moveNum;
					//info = 
					pMov->m_fenBlack = movPgn->m_fen;
					pMov->zhashKeyBlack = hash_from_fen((char*)pMov->m_fenBlack.c_str());
					
					if( *hits != -1 )
						*hits = InsertHashBlack(pMov);
					
					pMov->blackMove = movPgn->m_epn;
					return pMov;
				}
			}
			else
			{
				black = movPgn->m_pgn_string;
				//if( this->atEndWhite )
				//	this->atEndWhite = false;
				//this->atEndBlack = true;
				//x = 1;
				//y = 1;

					if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_WHITE ))
						this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
					SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						
					SetMoveFlag( MOVE_FLAG_X );
					SetMoveFlag( MOVE_FLAG_Y );

				m_fenBlack = movPgn->m_fen;
				zhashKeyBlack = hash_from_fen((char*)m_fenBlack.c_str());

				if( *hits != -1 )
					*hits = InsertHashBlack(this);

				blackMove = movPgn->m_epn;
			}
			return this;
		}
		return this;
	}

	Move* InsertVariation(MoveInserter *movPgn, int *hits=0)
	{
		if( movPgn->m_white_black == 0 )
		{
			if( /*x!= 0*/ this->MoveFlagIsSet( MOVE_FLAG_X ) && pNext != NULL)
			{
				if( movPgn->moveNum == this->info )
				{
					Move* pMov = getWhiteVariationForParse();
					pMov->white = movPgn->m_pgn_string;
						
					//if( this->atEndBlack )
					//	this->atEndBlack = false;
					//pMov->atEndWhite = true;
					
					pMov->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
					pMov->SetMoveFlag( MOVE_FLAG_X );

					//pMov->x = 1;
					char buf[10];
					_itoa(atoi(info.c_str())+1, buf, 10);
							
					pMov->info = movPgn->moveNum;
					pMov->m_fenWhite = movPgn->m_fen;
					pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
				
					if( *hits != -1 )
						*hits = InsertHashWhite(pMov);

					pMov->whiteMove = movPgn->m_epn;
				//his-> = pMov;
				
					return pMov;
				}
				else
				{
					Move* pMov = pNext->getWhiteVariationForParse();
					pMov->white = movPgn->m_pgn_string;
						
					//if( this->atEndBlack )
					//	this->atEndBlack = false;
					//pMov->atEndWhite = true;
						
					//pMov->x = 1;

					if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_BLACK ))
						this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					pMov->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						
					pMov->SetMoveFlag( MOVE_FLAG_X );

					char buf[10];
					_itoa(atoi(info.c_str())+1, buf, 10);
							
					pMov->info = pNext->info;//movPgn->moveNum;
					pMov->m_fenWhite = movPgn->m_fen;
					pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
				
					if( *hits != -1 )
						*hits = InsertHashWhite(pMov);

					pMov->whiteMove = movPgn->m_epn;
				//his-> = pMov;
				
					return pMov;
				}
			}
			else if( pNext == NULL && /*x!=0*/ this->MoveFlagIsSet( MOVE_FLAG_X ))
			{
				pNext = new Move();
				Move* pMov = pNext;
				pMov->pPrev = this;
				pMov->white = movPgn->m_pgn_string;
						
				//if( this->atEndBlack )
				//	this->atEndBlack = false;
				//pMov->atEndWhite = true;
						
				//pMov->x = 1;

					if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_BLACK ))
						this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					pMov->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						
					pMov->SetMoveFlag( MOVE_FLAG_X );

				char buf[10];
				_itoa(atoi(info.c_str())+1, buf, 10);
							
				pMov->info = movPgn->moveNum;
				pMov->m_fenWhite = movPgn->m_fen;
				pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
				
				if( *hits != -1 )
					*hits = InsertHashWhite(pMov);

				pMov->whiteMove = movPgn->m_epn;
			//his-> = pMov;
				
				return pMov;
			}
			else if( /*x==0*/ !this->MoveFlagIsSet( MOVE_FLAG_X) )
			{
				if( pPrev == NULL )
				{
					if( pNext == NULL )
					{
						pNext = new Move();
						Move* pMov = pNext;
						pMov->pPrev = this;
						pMov->white = movPgn->m_pgn_string;
						
						//if( this->atEndBlack )
						//	this->atEndBlack = false;
						//pMov->atEndWhite = true;
						
						//pMov->x = 1;

						if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_BLACK ))
							this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						pMov->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						
						pMov->SetMoveFlag( MOVE_FLAG_X );

						char buf[10];
						_itoa(atoi(info.c_str())+1, buf, 10);
							
						pMov->info = movPgn->moveNum;
						pMov->m_fenWhite = movPgn->m_fen;
						pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
				
						if( *hits != -1 )
							*hits = InsertHashWhite(pMov);

						pMov->whiteMove = movPgn->m_epn;
					//his-> = pMov;
				
						return pMov;
					}
					else
					{
						Move* pMov = pNext->getWhiteVariationForParse();
						pMov->white = movPgn->m_pgn_string;
						
						//if( this->atEndBlack )
					//		this->atEndBlack = false;
					//	pMov->atEndWhite = true;
						
					//	pMov->x = 1;

						if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_BLACK ))
							this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						pMov->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
							
						pMov->SetMoveFlag( MOVE_FLAG_X );

						char buf[10];
						_itoa(atoi(info.c_str())+1, buf, 10);
							
						pMov->info = pNext->info;//movPgn->moveNum;
						pMov->m_fenWhite = movPgn->m_fen;
						pMov->zhashKeyWhite = hash_from_fen((char*)pMov->m_fenWhite.c_str());
				
						if( *hits != -1 )
							*hits = InsertHashWhite(pMov);

						pMov->whiteMove = movPgn->m_epn;
					//his-> = pMov;
				
						return pMov;
					}
				}
				else
				{
					SetMoveFlag( MOVE_FLAG_X );
					SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
					//x = 1;
					white = movPgn->m_pgn_string;
					
					//this->atEndWhite = true;
					//if( this->pPrev && this->pPrev->pPrev == NULL )
					//	info = "1";
					info = movPgn->moveNum;
				
					m_fenWhite = movPgn->m_fen;
					zhashKeyWhite = hash_from_fen((char*)m_fenWhite.c_str());

					if( *hits != -1 )
						*hits = InsertHashWhite(this);

					whiteMove = movPgn->m_epn;
					return this;
				}

			}

		}
		else
		{
			if( /*y!= 0*/ MoveFlagIsSet( MOVE_FLAG_Y ) )
			{	
				Move* pMov = getBlackVariationForParse();
				pMov->black = movPgn->m_pgn_string;
				//if( this->atEndWhite )
			//		this->atEndWhite = false;
			//	pMov->atEndBlack = true;
			//	pMov->x = 1;
			//	pMov->y = 1;

				if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_WHITE ))
					this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
				pMov->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						
				pMov->SetMoveFlag( MOVE_FLAG_X );
				pMov->SetMoveFlag( MOVE_FLAG_Y );

				pMov->info = this->info;
				pMov->m_fenBlack = movPgn->m_fen;
				pMov->blackMove = movPgn->m_epn;
				pMov->zhashKeyBlack = hash_from_fen((char*)pMov->m_fenBlack.c_str());

				if( *hits != -1 )
					*hits = InsertHashBlack(this);
				return pMov;
			}
			else
			{
				black = movPgn->m_pgn_string;
				//if( this->atEndWhite )
				//	this->atEndWhite = false;
				//this->atEndBlack = true;
				//x = 1;
				//y = 1;

				if( this->MoveFlagIsSet( MOVE_FLAG_AT_END_WHITE ))
					this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
				SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						
				SetMoveFlag( MOVE_FLAG_X );
				SetMoveFlag( MOVE_FLAG_Y );

				m_fenBlack = movPgn->m_fen;
				blackMove = movPgn->m_epn;
				zhashKeyBlack = hash_from_fen((char*)m_fenBlack.c_str());
				
				if( *hits != -1 )
					*hits = InsertHashBlack(this);


				return this;
			}
		}
		return this;
	}

	Move* RemoveNode()
	{
		Move* prev = pPrev;
		pPrev->pNext = NULL;
		return pPrev;
	}
	
	Move* PromoteVariation(Move* sel)
	{       
		Move *temp = sel;
		if( temp->pUp == NULL )
			while( temp->pUp == NULL && temp->pPrev)
				temp = temp->pPrev;

		if( temp->pUp == NULL )
			return temp;
		
		Move* copy = temp;
		Move* Up = copy->pUp;
		Move* UpPrev = Up->pPrev;
		Move* UpNext = Up->pNext;
		Move* UpDown;

		if( temp->white != "" ) {
			UpDown = Up->pVarWhite;
			Move* tempDown= temp->pVarWhite;
			Move* insert = UpDown;
			Move* uppUp = Up->pUp;
			
			if( insert == temp )
			{
				//insert->pVarWhite = Up;
				temp->pVarWhite = Up;
			}
			else
			{
				temp->pVarWhite = UpDown;
				while(  insert->pVarWhite && insert->pVarWhite != temp )
					insert = insert->pVarWhite;

				insert->pVarWhite = Up;
			}


			/*
			Move* UppVarBlack = Up->pVarBlack;
			Move* temppVarBlack = temp->pVarBlack;

				Up->pVarBlack = temppVarBlack;
			temp->pVarBlack = UppVarBlack;
			*/
			// swap
			Up->pUp = temp;
			temp->pUp = uppUp;
			
			Up->pVarWhite = tempDown;
			temp->pPrev = UpPrev;
			if( uppUp == NULL )
				UpPrev->pNext = temp;
			Up->pPrev = temp;

			

			insert = temp->pVarWhite;
			while( insert )//&& insert->pVarWhite != Up )
			{
				insert->pUp = temp;
				insert->pPrev = temp;
				insert = insert->pVarWhite;
			}

			/*insert = temp->pVarBlack;
			while( insert )//&& insert->pVarWhite != Up )
			{
				insert->pUp = temp;
				insert->pPrev = temp;
				insert = insert->pVarBlack;
			}*/

		} else {
			UpDown = Up->pVarBlack;
			Move* tempDown= temp->pVarBlack;
			Move* insert = UpDown;
			Move* uppUp = Up->pUp;

			if( insert == temp )
			{
				//insert->pVarWhite = Up;
				temp->pVarBlack = Up;
			//	tempDown = temp;
			}
			else
			{
				temp->pVarBlack = UpDown;
				while(  insert->pVarBlack && insert->pVarBlack != temp )
					insert = insert->pVarBlack;

				insert->pVarBlack = Up;
			}

			temp->white = Up->white;
		//	MyChessMove mvTemp = temp->whiteMove;
			temp->whiteMove = Up->whiteMove;
			Up->white = "";

			Move* UppVarWhite = Up->pVarWhite;
			Move* temppVarWhite = temp->pVarWhite;

			string fen_up = Up->m_fenWhite;
			string fen_temp = temp->m_fenWhite;
			uint64 zhashUpWhite = Up->zhashKeyWhite;
			uint64 zhashTempWhite = temp->zhashKeyWhite;
			unsigned char tempFlags = temp->flags;
			unsigned char upFlags = Up->flags;

			Up->m_fenWhite = fen_temp;
			Up->zhashKeyWhite = zhashTempWhite;
			temp->m_fenWhite = fen_up;
			temp->zhashKeyWhite = zhashUpWhite;
			Up->pVarWhite = temppVarWhite;
			//Up->whiteMove = mvTemp;
			temp->pVarWhite = UppVarWhite;

			//Up->flags = tempFlags;
			//temp->flags = upFlags;
			

			// swap
			Up->pUp = temp;
			temp->pUp = uppUp;
			if( uppUp != NULL )
			{
				Move* findUpUppVarWhite = uppUp;
				while( findUpUppVarWhite->pVarWhite != Up )
					findUpUppVarWhite = findUpUppVarWhite->pVarWhite;

				findUpUppVarWhite->pVarWhite = temp;
			}
			Up->pVarBlack = tempDown;
			temp->pPrev = UpPrev;
			if( uppUp == NULL )
				UpPrev->pNext = temp;
			Up->pPrev = temp;

			insert = temp->pVarBlack;
			while( insert )//&& insert->pVarWhite != Up )
			{
				insert->pUp = temp;
				insert->pPrev = temp;
				insert = insert->pVarBlack;
			}
			insert = temp->pVarWhite;
			while( insert )//&& insert->pVarWhite != Up )
			{
				insert->pUp = temp;
				insert->pPrev = temp;
				insert = insert->pVarWhite;
			}
		}
		return Up;
	}

	bool IsThisFirstMoveInVariationWithoutNext(int white_black)
	{
		if( this->pUp != NULL && this->pNext == NULL )
		{
			if( white_black == 1 && this->white == "" && pNext == NULL)
			{
				return true;
			}
			else if( !(white_black == 1) && !this->MoveFlagIsSet(MOVE_FLAG_Y) && pNext == NULL )
			{
				return true;
			}
		}

		return false;
	}

	Move* DeleteRemainingMoves(int black_white)
	{
		if( black_white == 1 )
		{
			//this->y = 0;
			this->UnsetMoveFlag( MOVE_FLAG_Y );

			this->m_fenBlack = "";
			this->black = "";
			if( pVarBlack )
			{
				delete pVarBlack;
				pVarBlack = NULL;
			}
//			this->atEndWhite = true;
			this->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
		}
		if( this->pNext )
		{
			delete pNext;
			pNext = NULL;

		}

		return this;
	}

	Move* GetLastMove()
	{
		Move* temp = this;
		while( temp->pNext && temp->pNext->white.length() > 0 )
		{
			temp = temp->pNext;
		}

		return temp;
	}

	int Count(int &numMoves)
	{
		numMoves++;
		Move* temp = pVarWhite;
		while( temp )
		{
			temp->Count(numMoves);
			temp = temp->pVarWhite;
		}

		temp = pVarBlack;
		while( temp )
		{
			temp->Count(numMoves);
			temp = temp->pVarBlack;
		}

		if( pNext )
			pNext->Count(numMoves);

		return numMoves;
	}

	Move* getWhiteVariationForParse()
	{
		/*pVariationWhite[variation_count_white] = new Move();
		pVariationWhite[variation_count_white]->pPrev = this;

		return pVariationWhite[variation_count_white++];*/
	//	variation_count_white++;
		if( pVarWhite == NULL )
		{
			pVarWhite = new Move;
			pVarWhite->pPrev = this;
			pVarWhite->pUp = this;
			return pVarWhite;
		}
		else
		{
			Move* temp = pVarWhite;
			while( temp->pVarWhite )
			{
				temp = temp->pVarWhite;
			}
			temp->pVarWhite = new Move;
			temp->pVarWhite->pPrev = this;
			temp->pVarWhite->pUp = this;
			temp = temp->pVarWhite;
			return temp;
		}
	}

	Move* getBlackVariationForParse()
	{
		//variation_count_black++;
		if( pVarBlack == NULL )
		{
			pVarBlack = new Move;
			pVarBlack->pPrev = this;
			pVarBlack->pUp = this;
		//	pVarBlack->variation_count_black = variation_count_black;
			return pVarBlack;
		}
		else
		{
			Move* temp = pVarBlack;
			while( temp->pVarBlack )
			{
				temp = temp->pVarBlack;
			}
			temp->pVarBlack = new Move;
			temp->pVarBlack->pPrev = this;
			temp->pVarBlack->pUp = this;
		//	temp->pVarBlack->variation_count_black = variation_count_black;
			temp = temp->pVarBlack;
			return temp;
		}
	}

	void Print()
	{
		/*if( annotation.after_whites_move == false )
		{
			cout<< "printing"<<endl;
			cout<< info << " w: "
				<< white <<" b: "
				<< black <<" c: ";

				for( int aa = 0; aa < annotation.text.size(); aa++)
					cout<< annotation.text[aa];
				cout << endl;
		}
		else
		{
			cout<< info << " w: "
				<< white <<" c: ";
			for( int aa = 0; aa < annotation.text.size(); aa++)
				cout<< annotation.text[aa];
					

			cout <<" b: " << black << endl;
		}*/


		cout<< info << " w: "
			<< white <<" ";
	//	of<< info << " w: "
	//		<< white <<" ";
		for( int aa = 0; aa < annotation_white.text.size(); aa++)
		{
				cout<< annotation_white.text[aa];
		//		of <<annotation_white.text[aa];
		
		}
		cout << " b: "	<< black << " ";
		//of << " b: "	<< black << " ";
		for( int aa = 0; aa < annotation_black.text.size(); aa++)
		{
			cout<< annotation_black.text[aa];
			//of <<annotation_black.text[aa];
		}

		cout<<endl;
		//of <<endl;

		if( pVarWhite )
		{
				cout<< "    [variation white]"<<endl;
			//	of<< "    [variation white]"<<endl;
				pVarWhite->Print();
			//	pVarWhite = pVarWhite->pVarWhite;
				cout<<"    [end variation white]"<<endl;
			//	of<<"    [end variation white]"<<endl;
			
		}
		if( pVarBlack )
		{
				cout<< "    [variation black]"<<endl;
		//		of<< "    [variation black]"<<endl;
				pVarBlack->Print();
				cout<<"    [end variation black]"<<endl;
		//		of<<"    [end variation black]"<<endl;
			//	pVarWhite = pVarWhite->pVarWhite;
			
		}



		if( pNext )
			pNext->Print();
	}

	void SaveNode(ofstream &of)
	{
		if( white == "" )
		{
			of<< info <<"... ";
		}
		else
		{
			of<< info <<". "<< white <<" ";
		}

		if( annotation_white.text.size() > 0 )
			of << "{ ";
		
		for( int aa = 0; aa < annotation_white.text.size(); aa++)
		{
			of<< annotation_white.text[aa] <<" ";
		}
		
		if( annotation_white.text.size() > 0 )
			of << " }" <<endl;;
		


		of << black << " ";
		
		if( annotation_black.text.size() > 0 )
			of << "{ "; 
		
		for( int aa = 0; aa < annotation_black.text.size(); aa++)
		{
			of<< annotation_black.text[aa] << " ";
		}
		
		if( annotation_black.text.size() > 0 )
			of << " }" <<endl;
	}


	void SaveToFile(ofstream &of)
	{
		if( info == "" )
		{
			//if( pPrev && pPrev->result != "" )
			//	of << pPrev->result;
			return;
		}

		if( white == "" )
		{
			of<< info <<"... ";
		}
		else
		{
			of<< info <<". "<< white <<" ";
		}

		
		for( int aa = 0; aa < annotation_white.text.size(); aa++)
		{
			of<< annotation_white.text[aa] <<" ";
		}
		
		if( pVarWhite )
		{
			
			Move* tempVar = pVarWhite;
			while(tempVar)
			{
				of << "( ";
				Move* tempNext = tempVar;
				while( tempNext )
				{
					if( tempNext->info == "" )
					{
						tempNext = tempNext->pNext;
						continue;
					}

					if( (tempNext->white == "") && (tempNext->black == ""))
					{
						/*if(tempNext->result != "")
						{
							of<< result << " ";
						}*/
						tempNext = tempNext->pNext;
							continue;
					}


					//if( tempNext != pVarWhite )
					{
						if( tempNext->white == "" )
						{
							of<< tempNext->info <<"... ";
						}
						else
						{
							of<< tempNext->info <<". "<< tempNext->white <<" ";
						}
		
						for( int aa = 0; aa < tempNext->annotation_white.text.size(); aa++)
						{
							of<< tempNext->annotation_white.text[aa] <<" ";
						}
					}

					if((tempNext!=tempVar) && (tempNext->pVarWhite))
					{
						of << "( ";
						tempNext->pVarWhite->SaveToFile(of);
						of << ") ";	
					}

					if( tempNext->black == "" )
					{
						tempNext = tempNext->pNext;
						continue;
					}

					if( (tempNext!=tempVar) && (tempNext->pVarWhite))
						of << tempNext->info <<"... "<<tempNext->black << " ";
					else
						of << tempNext->black << " ";
				
		
					for( int aa = 0; aa < tempNext->annotation_black.text.size(); aa++)
					{
						of<< tempNext->annotation_black.text[aa] << " ";
					}
		

					if( tempNext->pVarBlack )
					{
						of << "( ";
						tempNext->pVarBlack->SaveToFile(of);
						of << ") ";		
					}
					tempNext = tempNext->pNext;
				}
				tempVar = tempVar->pVarWhite;
				of << ") ";	
			}
			
		}

		if( black == "" )
		{
			//if( result != "" )
			//	of << " " <<result;
			return;
		}



		if( pVarWhite )
			of << info << "... " << black <<" ";
		else 
			of << black << " ";
		
		
		for( int aa = 0; aa < annotation_black.text.size(); aa++)
		{
			of<< annotation_black.text[aa] << " ";
		}
		

		if( pVarBlack )
		{
			
			Move* tempVar = pVarBlack;
			while(tempVar)
			{
				of << "( ";
				Move* tempNext = tempVar;
				while( tempNext )
				{
					if( tempNext->info == "" )
					{
						tempNext = tempNext->pNext;
						continue;
					}

					if( (tempNext->white == "") && (tempNext->black == ""))
					{
						/*if(tempNext->result != "")
						{
							of<< result << " ";
						}*/
						tempNext = tempNext->pNext;
							continue;
					}

					if( tempNext->white != "" )
					{
						of<< tempNext->info <<". "<< tempNext->white <<" ";
					}

					//if( tempNext != tempVar )
					{

						for( int aa = 0; aa < tempNext->annotation_white.text.size(); aa++)
						{
							of<< tempNext->annotation_white.text[aa] <<" ";
						}
					}

					if( (tempNext->white == "") )
					{
						of<< tempNext->info <<"... ";
					}

					if(tempNext!=tempVar && tempNext->pVarWhite)
					{
						of << "( ";
						tempNext->pVarWhite->SaveToFile(of);
						of << ") ";	
					}

					if( tempNext->black == "" )
					{
						tempNext = tempNext->pNext;
						continue;
					}

					if( (tempNext != tempVar) && (tempNext->pVarWhite) )
						of << tempNext->info <<"... "<<tempNext->black << " ";
					else
						of << tempNext->black << " ";

		
					for( int aa = 0; aa < tempNext->annotation_black.text.size(); aa++)
					{
						of<< tempNext->annotation_black.text[aa] << " ";
					}
		

					if( tempNext!=tempVar && tempNext->pVarBlack )
					{
						of << "( ";
						tempNext->pVarBlack->SaveToFile(of);
						of << ") ";		
					}
					tempNext = tempNext->pNext;
				}
				tempVar = tempVar->pVarBlack;
				of << ") ";
			}
				
		}

		if( pNext )
			pNext->SaveToFile(of);
	}

	void SaveToStream(ostringstream &of)
	{
		if( info == "" )
		{
		//	if( pPrev && pPrev->result != "" )
		//		of << pPrev->result;
			return;
		}

		if( white == "" )
		{
			of<< info <<"... ";
		}
		else
		{
			of<< info <<". "<< white <<" ";
		}

		
		for( int aa = 0; aa < annotation_white.text.size(); aa++)
		{
			of<< annotation_white.text[aa] <<" ";
		}
		
		if( pVarWhite )
		{
			
			Move* tempVar = pVarWhite;
			while(tempVar)
			{
				of << "( ";
				Move* tempNext = tempVar;
				while( tempNext )
				{
					if( tempNext->info == "" )
					{
						tempNext = tempNext->pNext;
						continue;
					}

					if( (tempNext->white == "") && (tempNext->black == ""))
					{
						/*if(tempNext->result != "")
						{
							of<< result << " ";
						}*/
						tempNext = tempNext->pNext;
							continue;
					}


					//if( tempNext != pVarWhite )
					{
						if( tempNext->white == "" )
						{
							of<< tempNext->info <<"... ";
						}
						else
						{
							of<< tempNext->info <<". "<< tempNext->white <<" ";
						}
		
						for( int aa = 0; aa < tempNext->annotation_white.text.size(); aa++)
						{
							of<< tempNext->annotation_white.text[aa] <<" ";
						}
					}

					if((tempNext!=tempVar) && (tempNext->pVarWhite))
					{
						of << "( ";
						tempNext->pVarWhite->SaveToStream(of);
						of << ") ";	
					}

					if( tempNext->black == "" )
					{
						tempNext = tempNext->pNext;
						continue;
					}

					if( (tempNext!=tempVar) && (tempNext->pVarWhite))
						of << tempNext->info <<"... "<<tempNext->black << " ";
					else
						of << tempNext->black << " ";
				
		
					for( int aa = 0; aa < tempNext->annotation_black.text.size(); aa++)
					{
						of<< tempNext->annotation_black.text[aa] << " ";
					}
		

					if( tempNext->pVarBlack )
					{
						of << "( ";
						tempNext->pVarBlack->SaveToStream(of);
						of << ") ";		
					}
					tempNext = tempNext->pNext;
				}
				tempVar = tempVar->pVarWhite;
				of << ") ";	
			}
			
		}

		if( black == "" )
		{
			//if( result != "" )
			//	of << " " <<result;
			return;
		}



		if( pVarWhite )
			of << info << "... " << black <<" ";
		else 
			of << black << " ";
		
		
		for( int aa = 0; aa < annotation_black.text.size(); aa++)
		{
			of<< annotation_black.text[aa] << " ";
		}
		

		if( pVarBlack )
		{
			
			Move* tempVar = pVarBlack;
			while(tempVar)
			{
				of << "( ";
				Move* tempNext = tempVar;
				while( tempNext )
				{
					if( tempNext->info == "" )
					{
						tempNext = tempNext->pNext;
						continue;
					}

					if( (tempNext->white == "") && (tempNext->black == ""))
					{
						/*if(tempNext->result != "")
						{
							of<< result << " ";
						}*/
						tempNext = tempNext->pNext;
							continue;
					}

					if( tempNext->white != "" )
					{
						of<< tempNext->info <<". "<< tempNext->white <<" ";
					}

					//if( tempNext != tempVar )
					{

						for( int aa = 0; aa < tempNext->annotation_white.text.size(); aa++)
						{
							of<< tempNext->annotation_white.text[aa] <<" ";
						}
					}

					if( (tempNext->white == "") )
					{
						of<< tempNext->info <<"... ";
					}

					if(tempNext!=tempVar && tempNext->pVarWhite)
					{
						of << "( ";
						tempNext->pVarWhite->SaveToStream(of);
						of << ") ";	
					}

					if( tempNext->black == "" )
					{
						tempNext = tempNext->pNext;
						continue;
					}

					if( (tempNext != tempVar) && (tempNext->pVarWhite) )
						of << tempNext->info <<"... "<<tempNext->black << " ";
					else
						of << tempNext->black << " ";

		
					for( int aa = 0; aa < tempNext->annotation_black.text.size(); aa++)
					{
						of<< tempNext->annotation_black.text[aa] << " ";
					}
		

					if( tempNext!=tempVar && tempNext->pVarBlack )
					{
						of << "( ";
						tempNext->pVarBlack->SaveToStream(of);
						of << ") ";		
					}
					tempNext = tempNext->pNext;
				}
				tempVar = tempVar->pVarBlack;
				of << ") ";
			}
				
		}

		if( pNext )
			pNext->SaveToStream(of);
	}


	void SaveLineToStream(ostringstream &of)
	{
		if( info == "" )
		{
		//	if( pPrev && pPrev->result != "" )
		//		of << pPrev->result;
			return;
		}

		if( white == "" )
		{
			of<< info <<"... ";
		}
		else
		{
			of<< info <<". "<< white <<" ";
		}

		
		for( int aa = 0; aa < annotation_white.text.size(); aa++)
		{
			of<< annotation_white.text[aa] <<" ";
		}
		
		if( black == "" )
		{
			//if( result != "" )
			//	of << " " <<result;
			return;
		}



		of << black << " ";
		
		
		for( int aa = 0; aa < annotation_black.text.size(); aa++)
		{
			of<< annotation_black.text[aa] << " ";
		}
		







		if( pNext )
			pNext->SaveToStream(of);
	}

	void SetFenWhite( string fen )
	{
		m_fenWhite = fen;
		zhashKeyWhite = hash_from_fen((char*)fen.c_str());

		//int hits = InsertHashWhite(this);

		//hasFenWhite = true;
		this->SetMoveFlag( MOVE_FLAG_HAS_FEN_WHITE );
	}

	string getFenWhite()
	{
		return m_fenWhite;
	}

	void SetFenBlack( string fen )
	{
		m_fenBlack = fen;
		zhashKeyBlack = hash_from_fen((char*)fen.c_str());
		
		//int hits = InsertHashBlack(this);
		//hasFenBlack = true;
		this->SetMoveFlag( MOVE_FLAG_HAS_FEN_BLACK );

	}

	string getFenBlack()
	{
		return m_fenBlack;
	}

	bool IsThisTheEnd(){
		return ((this->MoveFlagIsSet( MOVE_FLAG_AT_END_WHITE )) || (this->MoveFlagIsSet( MOVE_FLAG_AT_END_BLACK ))); 
	}

	bool EndWhite()
	{
		return MoveFlagIsSet( MOVE_FLAG_AT_END_WHITE );
	}

	bool EndBlack()
	{
		return MoveFlagIsSet( MOVE_FLAG_AT_END_BLACK );
	}

	void SetWhiteMoveSquares(MyChessMove &src)
	{
		//whiteMove.from.file = src.from.file;
		//whiteMove.from.rank = src.from.rank;
		//whiteMove.to.file = src.to.file;
		//whiteMove.to.rank = src.to.rank;
		whiteMove = src;
	}

	void SetBlackMoveSquares(MyChessMove &src)
	{
		//blackMove.from.file = src.from.file;
		//blackMove.from.rank = src.from.rank;
		//blackMove.to.file = src.to.file;
		//blackMove.to.rank = src.to.rank;
		blackMove = src;
	}

	void CopyFast( Move *temp )
	{
		//x = temp->x;
		//y = temp->y;
		flags = temp->flags;
		white = temp->white;
		black = temp->black;
		info = temp->info;

	//	atEndWhite = temp->atEndWhite;
	//	atEndBlack = temp->atEndBlack;

		whiteMove = temp->whiteMove;
		blackMove = temp->blackMove;
	}

	void CopyNode( Move* temp )
	{
		//x = temp->x;
		//y = temp->y;
		flags = temp->flags;
		
		white = temp->white;
		black = temp->black;
//		number = temp->number;

		//annotations = temp->annotations;
		annotation_white = temp->annotation_white;
		annotation_black = temp->annotation_black;
//		capture = temp->capture;
//		promote = temp->promote;
		info = temp->info;
		result = temp->result;
		m_fenWhite = temp->m_fenWhite;
		m_fenBlack = temp->m_fenBlack;
	//	hasFenWhite = temp->hasFenWhite;
	//	hasFenBlack = temp->hasFenBlack;
		zhashKeyWhite = temp->zhashKeyWhite;
		zhashKeyBlack = temp->zhashKeyBlack;
	//	atEndWhite = temp->atEndWhite;
	//	atEndBlack = temp->atEndBlack;
		whiteMove = temp->whiteMove;
		blackMove = temp->blackMove;
		//variation_count_white = temp->variation_count_white;
		//variation_count_black = temp->variation_count_black;
	}

	void Copy( Move* pMove )
	{
		this->CopyNode(pMove);

		Move* temp = pMove->pVarWhite;
		Move* tVarW = this->pVarWhite;
		while( temp )
		{
			tVarW = new Move();//this->GetWhiteVariationForParse();
			tVarW->pUp = this;
			tVarW->pPrev = this;

			tVarW->Copy(temp);

			tVarW = tVarW->pVarWhite;
			
			temp = temp->pVarWhite;
		}

		temp = pMove->pVarBlack;
		Move* tVarB = this->pVarBlack;
		while( temp )
		{
			tVarB = new Move();
			tVarB->pUp = this;
			tVarB->pPrev = this;

			tVarB->Copy(temp);

			tVarB = tVarB->pVarBlack;

			temp = temp->pVarBlack;
		}

		if( pMove->pNext )
		{
			Move* next = new Move();
			next->pPrev = this;
			this->pNext = next;

			next->Copy(pMove->pNext);
		}
	}

	


	void PrepareAnnotations()
	{
		/*for( int aa = 0; aa < annotation_white.text.size(); aa++ )
		{
			int find_curly = -1;
			find_curly = annotation_white.text[aa].find("}");
			if( find_curly != -1 )
			{
				int find_smooth = -1;
				find_smooth = annotation_white.text[aa].find(")");
				if( find_smooth > find_curly )
				{
					annotation_white.text[aa].erase(find_smooth, 1);
				}
			}
			text_white += " "+annotation_white.text[aa];
		}
		for( int aa = 0; aa < annotation_black.text.size(); aa++ )
		{
			int find_curly = -1;
			find_curly = annotation_black.text[aa].find("}");
			if( find_curly != -1 )
			{
				int find_smooth = -1;
				find_smooth = annotation_black.text[aa].find(")");
				if( find_smooth > find_curly )
				{
					annotation_black.text[aa].erase(find_smooth, 1);
				}
			}
			text_black += " "+annotation_black.text[aa];
		}*/
	}

	// BROKEN
	/*void EnumerateVariations()
	{
		char varIds[15]="ABCDEFGHIJKLMN";
		char varIdsBlack[15]="abcdefghijklmn";
		if( variation_count_white > 0 )
		{
			if( variation_count_white > 1 )
			{
				Move* pVarIter = pVarWhite;
				pVarIter->variation_id += varIds[0];
				pVarIter->variation_id += "\0";
				int count = 1;
				pVarIter = pVarIter->pVarWhite;
				while( pVarIter && count < 14)
				{
					pVarIter->variation_id += varIds[count++];
					pVarIter->variation_id += "\0";
					Move* pVarNextIter = pVarIter->pNext;
					while( pVarNextIter )
					{
						pVarNextIter->EnumerateVariations();
						pVarNextIter = pVarNextIter->pNext;
					}
					pVarIter = pVarIter->pVarWhite;
				}
			}
		}
		if( variation_count_black > 0 )
		{
			if( variation_count_black > 1 )
			{
				Move* pVarIter = pVarBlack;
				pVarIter->variation_id += varIdsBlack[0];
				pVarIter->variation_id += "\0";
				int count = 1;
				pVarIter = pVarIter->pVarBlack;
				while( pVarIter && count < 14)
				{
					pVarIter->variation_id += varIdsBlack[count++];
					pVarIter->variation_id += "\0";
					Move* pVarNextIter = pVarIter->pNext;
					while( pVarNextIter )
					{
						pVarNextIter->EnumerateVariations();
						pVarNextIter = pVarNextIter->pNext;
					}
					pVarIter = pVarIter->pVarBlack;
				}
			}
		}
		if( pNext )
			pNext->EnumerateVariations();
	}*/

	bool IsTrueDisplayVariations()
	{
		return this->MoveFlagIsSet( MOVE_FLAG_DISPLAY_VARIATIONS );
		//return displayVariations;
	}

	void SetDisplayVariations(bool set)
	{
		//displayVariations = set;
		if( set )
		{
			this->SetMoveFlag( MOVE_FLAG_DISPLAY_VARIATIONS );
		}
		else
		{
			this->UnsetMoveFlag( MOVE_FLAG_DISPLAY_VARIATIONS );
		}
	}

	void SetRenderColor( int r, int g, int b, ColorPallete &pallete )
	{
		rm = pallete.AddColor(r,g,b);

	}

	inline RenderMoveColor* GetRenderColor(ColorPallete &pallete)
	{
		return &pallete.GetColor(rm);
	}

	void SetRenderColorWhite(int r, int g, int b, ColorPallete &pallete)
	{
		rmw = pallete.AddColor(r,g,b);
		
		/*renderMoveColorWhite.r = r;
		renderMoveColorWhite.g = g;
		renderMoveColorWhite.b = b;
		*/
	}

	void SetRenderColorBlack(int r, int g, int b, ColorPallete &pallete)
	{
		rmb = pallete.AddColor(r,g,b);
		/*renderMoveColorBlack.r = r;
		renderMoveColorBlack.g = g;
		renderMoveColorBlack.b = b;*/
	}

	inline RenderMoveColor* GetRenderColorWhite(ColorPallete &pallete)
	{
		return &pallete.GetColor(rmw);
	}

	inline RenderMoveColor* GetRenderColorBlack(ColorPallete &pallete)
	{
		return &pallete.GetColor(rmb);
	}


	void SetVariationRenderColor(int r, int g, int b, ColorPallete &pallete)
	{
		Move* temp = this;
		while( temp->pUp == NULL )
		{
			if( temp->pPrev )
				temp = temp->pPrev;
			else break;
		}
		
		while(temp)
		{
			temp->SetRenderColor(r,g,b,pallete);
			temp->SetRenderColorWhite(r,g,b,pallete);
			temp->SetRenderColorBlack(r,g,b,pallete);
			temp = temp->pNext;
		}
	}


	inline MyChessMove* GetWhiteMove()
	{
		return &whiteMove;
	}

	inline MyChessMove* GetBlackMove()
	{
		return &blackMove;
	}

	void Visit()
	{
		//visited = true;
		this->SetMoveFlag( MOVE_FLAG_VISITED );
	}

	bool HasBeenVisited()
	{
		return this->MoveFlagIsSet(MOVE_FLAG_VISITED);
		//return visited;
	}

	void PrepareForVisitor()
	{
		//visited = false;
		
		this->UnsetMoveFlag( MOVE_FLAG_VISITED );
		Move* temp = this->pVarWhite;
		while( temp )
		{
			temp->PrepareForVisitor();
			temp = temp->pVarWhite;
		}

		temp = this->pVarBlack;

		while( temp )
		{
			temp->PrepareForVisitor();
			temp = temp->pVarBlack;
		}

		if( pNext )
			pNext->PrepareForVisitor();
	}

	void RemoveResultString()
	{
		Move* temp = this;
		while( temp->pNext && temp->result == "")
		{
			temp = temp->pNext;
		}

		temp->result = "";

		if( temp->white == "" )
		{
			Move* removedFrom = temp->pPrev;
			removedFrom->pNext = NULL;
			delete temp;
			temp = NULL;
			
		}
		
	}

	inline string GetAnnotationWhite()
	{
		string retVal = "";
		for( int i = 0; i < this->annotation_white.text.size(); i++ )
		{
			if( (this->annotation_white.text[i].find("{") == -1) &&
				(this->annotation_white.text[i].find("}") == -1) )
			{
				retVal += this->annotation_white.text[i];
				//retVal += " ";
			}
		}
		return retVal;
	}

	inline string GetAnnotationBlack()
	{
		string retVal = "";
		for( int i = 0; i < this->annotation_black.text.size(); i++ )
		{
			if( (this->annotation_black.text[i].find("{") == -1) &&
				(this->annotation_black.text[i].find("}") == -1) )
			{
				retVal += this->annotation_black.text[i];
				//retVal += " ";
			}
		}
		return retVal;
	}

	bool MoveIsEqual(Move* move)
	{
		if( move->white == white )
		{
			if( move->black == black )
			{
				if( move->info == info )
				{
					return true;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}

	bool MoveIsEqualWhite(Move* move)
	{
		if( move->white == white &&
			move->info == info )
		{
			return true;
		}
		else return false;
	}

	bool MoveIsEqualBlack(Move* move)
	{
		if( move->black == black &&
			move->info == info )
		{
			return true;
		}
		else return false;
	}

	string Compare(Move* move)
	{
		if( MoveIsEqualWhite(move) )
		{
			if( MoveIsEqualBlack(move) )
			{
				return "";
			}
			else return GetAnnotationWhite();
		}
		else return "";
	}

	// not working, the idea is to push move up to the move called toRoot, then 
	// to see what the ECO code is
	string PushMove2Root(Move* move, Move* toRoot)
	{
		if( this == toRoot )
		{
			if( this->black == toRoot->black )
			{
				return GetAnnotationBlack();
			}
			else if( this->white == toRoot->white )
			{
				return GetAnnotationWhite();
			}
			else {
				return pPrev->GetAnnotationBlack();
			}
		}
		if( move->white == white )
		{
			if( move->black == black )
			{
				if( pNext )
				{
					if( move->pNext )
					{
						if( pNext->white != "" )
						{
							return pNext->PushMove2(move->pNext);
							//return;
						}
						else
						{
							return this->GetAnnotationBlack();
							// there is nothing in whites move
							// return this->annotationblack

						}
						
					}
					else 
					{
						return GetAnnotationBlack();
					}
				}
				else
				{
					return this->GetAnnotationBlack();
					// there is nothing else, black and white
					// are equal, return this->annotation black

				}
				return this->GetAnnotationBlack();
			}
			else
			{
				if( black == "" )
				{
					// blacks move is empty, return annotation white
					return this->GetAnnotationWhite();
				}
				else 
				{
					if( move->black != "" )
					{
						if( pVarBlack )
						{
							Move* temp = pVarBlack;
							bool match = false;
							while( temp )
							{
								if( temp->black == move->black )
								{
									if( temp->pNext )
									{
										if( move->pNext )
										{
											return temp->pNext->PushMove2(move->pNext);
											
										}
									}
									else //if( move->pNext )
									{
										// move is inserted into the 
										// tree as next, return temp annotation black
										return temp->GetAnnotationBlack();
									}
								}

								temp = temp->pVarBlack;
							}
						}
						
						// move is inserted as a variation of blacks move, 
						// return annotation white
						return this->GetAnnotationWhite();
					}
					else 
					{
						return this->GetAnnotationWhite();
					}
				}
			}
		}
		else
		{
			if( white == "" )
			{
				return pPrev->GetAnnotationBlack();
			}
			else if( pVarWhite )
			{
				// check white variations
				
				
				return pVarWhite->PushMove2(move);
					
				
				
			}
			else 
			{
				return pPrev->GetAnnotationBlack();
			}
		}
		
		return "";
	}

	// working stable
	string PushMove2(Move* move)
	{
		if( move->white == white )
		{
			if( move->black == black )
			{
				if( pNext )
				{
					if( move->pNext )
					{
						if( pNext->white != "" )
						{
							return pNext->PushMove2(move->pNext);
							//return;
						}
						else
						{
							return this->GetAnnotationBlack();
							// there is nothing in whites move
							// return this->annotationblack

						}
						
					}
					else 
					{
						return GetAnnotationBlack();
					}
				}
				else
				{
					return this->GetAnnotationBlack();
					// there is nothing else, black and white
					// are equal, return this->annotation black

				}
				return this->GetAnnotationBlack();
			}
			else
			{
				if( black == "" )
				{
					// blacks move is empty, return annotation white
					return this->GetAnnotationWhite();
				}
				else 
				{
					if( move->black != "" )
					{
						if( pVarBlack )
						{
							Move* temp = pVarBlack;
							bool match = false;
							while( temp )
							{
								if( temp->black == move->black )
								{
									if( move->pNext )
									{
										if( temp->pNext )
										{
											return temp->pNext->PushMove2(move->pNext);
											
										}
									}
									else //if( move->pNext )
									{
										// move is inserted into the 
										// tree as next, return temp annotation black
										return temp->GetAnnotationBlack();
									}
								}

								temp = temp->pVarBlack;
							}
						}
						
						// move is inserted as a variation of blacks move, 
						// return annotation white
						return this->GetAnnotationWhite();
					}
					else 
					{
						return this->GetAnnotationWhite();
					}
				}
			}
		}
		else
		{
			if( white == "" )
			{
				return pPrev->GetAnnotationBlack();
			}
			else if( pVarWhite )
			{
				// check white variations
				
				
				return pVarWhite->PushMove2(move);
					
				
				
			}
			else 
			{
				return pPrev->GetAnnotationBlack();
			}
		}
		
		return "";
	}

	// function for trees, get the annotation // pushmove2 is working
	string PushMove(Move* move)
	{
		if( move->info != info )
			return "";

		if( move->white == white )
		{
			if( move->black == black )
			{
				if( pNext && move->pNext)
				{
					return PushMove(move->pNext);
				}
				else
					return GetAnnotationBlack();
			}
			else 
			{
				if( pVarBlack && move->pNext)
				{
					Move* temp = pVarBlack;
					while( temp )
					{
						
						string ret = temp->PushMove(move);
						if( ret != "" )
							return ret;
						temp = temp->pVarBlack;
					}
				}
				return GetAnnotationWhite();
			}
		}
		else
		{
			if( white == "" && pUp )
			{
				if( move->black == black )
				{
					if( pNext && move->pNext )
						return PushMove(move->pNext);
					else return "";
				}
				else return "";
			}
			else if( pVarWhite  )
			{
				//if( move->pNext )
				{
					Move* temp = pVarWhite;
					while( temp )
					{
						if( temp->white == move->white )
						{
							if( temp->black == move->black )
							{
								if( temp->pNext && move->pNext)
								{
									return temp->pNext->PushMove(move->pNext);
								}
								else return temp->GetAnnotationBlack();
							}
							else if( pVarBlack && move->pNext)
							{
								return pVarBlack->PushMove(move->pNext);
							
							}
							
						}
						//string ret = temp->PushMove(move);
						
						temp = temp->pVarWhite;
					}
				}
				
			}
			if( pPrev )
			{

				return pPrev->GetAnnotationBlack();
			}
			else return "";
		}
	}

	void CopyAnnotationWhiteFromBtoA(Move* A, Move* B)
	{
		for( int i = 0; i< B->annotation_white.text.size(); i++ )
		{
			A->annotation_white.text.push_back( B->annotation_white.text[i] );
		}
	}

	void CopyAnnotationBlackFromBtoA(Move* A, Move* B)
	{
		for( int i = 0; i< B->annotation_black.text.size(); i++ )
		{
			A->annotation_black.text.push_back( B->annotation_black.text[i] );
		}
	}

	// working
void Merge(Move* move)
	{
		if( move->white == white )
		{
			if( this->annotation_white.text.size() == 0 )
			{
				this->annotation_white = move->annotation_white;
			}
			if( move->black == black )
			{
				if( this->annotation_black.text.size() == 0 )
				{
					this->annotation_black = move->annotation_black;
				}
				if( pNext )
				{
					if( move->pNext )
					{
						if( pNext->white != "" )
						{
							pNext->Merge(move->pNext);
							return;
						}
						else
						{
							result = "";

							pNext = move->pNext;
							pNext->pPrev = this;
							


							move->pNext = NULL;
							
							this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
							this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
							//this->atEndBlack = false;
							//this->atEndWhite = false;
						}
						return;
					}
				}
				else
				{
					if( move->pNext )
					{
						result = "";

						pNext = move->pNext;
						pNext->pPrev = this;

						move->pNext = NULL;


						this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						
						//this->atEndBlack = false;
						//this->atEndWhite = false;
						
						return;
					}
				}
			}
			else
			{
				if( black == "" )
				{
					if( move->black != "" )
					{
						black = move->black;
						m_fenBlack = move->m_fenBlack;
						this->CopyAnnotationBlackFromBtoA(this, move);
						if( move->pNext )
						{
							result = "";

							pNext = move->pNext;
							pNext->pPrev = this;

							move->pNext = NULL;
						
							this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
							this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
							
							//this->atEndBlack = false;
							//this->atEndWhite = false;
							
							return;
						}
						//this->atEndBlack = true;
						//this->atEndWhite = false;
						
						this->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
					}
					else
					{
						if( pNext )
						{
							delete pNext;
							pNext = NULL;
						}
						//this->atEndBlack = false;
						//this->atEndWhite = true;

						this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						this->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						
						return;
					}
				}
				else 
				{
					if( move->black != "" )
					{
						if( pVarBlack )
						{
							Move* temp = pVarBlack;
							bool match = false;
							while( temp )
							{
								if( temp->black == move->black )
								{
									if( temp->annotation_black.text.size() == 0 )
									{
										temp->annotation_black = move->annotation_black;
									}
									if( temp->pNext )
									{
										if( move->pNext )
										{
											temp->pNext->Merge(move->pNext);
											return;
										}
									}
									else if( move->pNext )
									{
										temp->pNext = move->pNext;
										if(move->pNext)
											move->pNext->pPrev = temp;
										move->pUp = this->pUp;
										//temp->atEndBlack = false;
										//temp->atEndWhite = false;

										temp->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
										temp->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
										return;
									}
								}

								temp = temp->pVarBlack;
							}
						}
						
						Move* blackVar = this->getBlackVariationForParse();

						blackVar->CopyNode(move);
						blackVar->white = "";
						blackVar->pNext = move->pNext;
						if( move->pNext )
							move->pNext->pPrev = blackVar;

						move->pNext = NULL;
						blackVar->pUp = this;
						blackVar->pPrev = this;
						return;
					}
				}
			}
		}
		else
		{
			if( white == "" )
			{
				if( move->white != "" )
				{
					result = "";

					CopyNode(move);

					//this->atEndBlack = false;
					//this->atEndWhite = false;

					this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );

					if( black == "" )
					{
						//this->atEndBlack = true;
						this->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						
					}

					pNext = move->pNext;
					move->pNext = NULL;
					pVarWhite = move->pVarWhite;
					pVarBlack = move->pVarBlack;
					
					if( pNext )
					{
						pNext->pPrev = this;
						pNext->pUp = pUp;
					}

					return;
				}
				else 
				{
					if( pNext )
					{
						delete pNext;
						pNext = NULL;
					}
				}return;
			}
			
			if( move->white != "" )
			{
				// check white variations
				if( pVarWhite )
				{
					pVarWhite->Merge(move);
					return;
				}

				Move* whiteVar;
				if( pUp )
				{
					Move* temp2 = pUp;
					
					whiteVar = pUp->getWhiteVariationForParse();	
				}
				else
				{
					whiteVar = getWhiteVariationForParse();
				}

				whiteVar->CopyNode(move);

				whiteVar->pNext = move->pNext;
			
				move->pNext = NULL;

				if( whiteVar->pNext )
				{
					whiteVar->pNext->pPrev = whiteVar;
				}
			}
			return;
		}
	}

	// not working?
	void MergeNonBook(Move* move)
	{
		if( move->white == white )
		{

			if( move->black == black )
			{

				if( pNext )
				{
					if( move->pNext )
					{
						if( pNext->white != "" )
						{
							pNext->Merge(move->pNext);
							return;
						}
						else
						{
							result = "";

							pNext = move->pNext;
							pNext->pPrev = this;
							


							move->pNext = NULL;
							
							this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
							this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );

							//this->atEndBlack = false;
							//this->atEndWhite = false;
						}
						return;
					}
				}
				else
				{
					if( move->pNext )
					{
						result = "";

						pNext = move->pNext;
						pNext->pPrev = this;

						move->pNext = NULL;
				
						this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );

						//this->atEndBlack = false;
						//this->atEndWhite = false;
						
						return;
					}
				}
			}
			else
			{
				if( black == "" )
				{
					if( move->black != "" )
					{
						black = move->black;
						m_fenBlack = move->m_fenBlack;

						if( move->pNext )
						{
							result = "";

							pNext = move->pNext;
							pNext->pPrev = this;

							move->pNext = NULL;

							this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
							this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
							
							//this->atEndBlack = false;
							//this->atEndWhite = false;
							
							return;
						}

						this->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						
						//this->atEndBlack = true;
						//this->atEndWhite = false;
						
					}
					else
					{
						if( pNext )
						{
							delete pNext;
							pNext = NULL;
						}
						//this->atEndBlack = false;
						//this->atEndWhite = true;

						this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						this->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						
						return;
					}
				}
				else 
				{
					if( move->black != "" )
					{
						if( pVarBlack )
						{
							Move* temp = pVarBlack;
							bool match = false;
							while( temp )
							{
								if( temp->black == move->black )
								{
									if( temp->annotation_black.text.size() == 0 )
									{
										temp->annotation_black = move->annotation_black;
									}
									if( temp->pNext )
									{
										if( move->pNext )
										{
											temp->pNext->Merge(move->pNext);
											return;
										}
									}
									else if( move->pNext )
									{
										temp->pNext = move->pNext;
										if(move->pNext)
											move->pNext->pPrev = temp;
										move->pUp = this->pUp;
							//			temp->atEndBlack = false;
								//		temp->atEndWhite = false;

										temp->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
										temp->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
										return;
									}
								}

								temp = temp->pVarBlack;
							}
						}
						
						Move* blackVar = this->getBlackVariationForParse();

						blackVar->CopyFast(move);
						blackVar->white = "";
						blackVar->pNext = move->pNext;
						if( move->pNext )
							move->pNext->pPrev = blackVar;

						move->pNext = NULL;
						blackVar->pUp = this;
						blackVar->pPrev = this;
						return;
					}
				}
			}
		}
		else
		{
			if( white == "" )
			{
				if( move->white != "" )
				{
					result = "";

					CopyFast(move);

					//this->atEndBlack = false;
					//this->atEndWhite = false;

					this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );

					if( black == "" )
					{
						//this->atEndBlack = true;
						this->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						
					}

					pNext = move->pNext;
					move->pNext = NULL;
					pVarWhite = move->pVarWhite;
					pVarBlack = move->pVarBlack;
					
					if( pNext )
					{
						pNext->pPrev = this;
						pNext->pUp = pUp;
					}

					return;
				}
				else 
				{
					if( pNext )
					{
						delete pNext;
						pNext = NULL;
					}
				}return;
			}
			
			if( move->white != "" )
			{
				// check white variations
				if( pVarWhite )
				{
					pVarWhite->Merge(move);
					return;
				}

				Move* whiteVar;
				if( pUp )
				{
					Move* temp2 = pUp;
					
					whiteVar = pUp->getWhiteVariationForParse();	
				}
				else
				{
					whiteVar = getWhiteVariationForParse();
				}

				whiteVar->CopyFast(move);

				whiteVar->pNext = move->pNext;
			
				move->pNext = NULL;

				if( whiteVar->pNext )
				{
					whiteVar->pNext->pPrev = whiteVar;
				}
			}
			return;
		}
	}

	void AddResultToString(PGN::Comment &annotation, int resultCode )
	{
		if( annotation.text.size() == 0 )
		{
			if( resultCode == 0 )
			{
				istringstream iss;
				//iss << 
			}
			else if( resultCode == 1 )
			{
			}
			else if( resultCode == 2 )
			{
			}
		}
	}

	void InsertAnnotationWhite(string &inpt)
	{
		annotation_white.text.push_back("{");
		istringstream iss(inpt);
		copy(istream_iterator<string>(iss),
			istream_iterator<string>(),
				back_inserter<vector<string> >(annotation_white.text));
			
		annotation_white.text.push_back("}");
	}

	void InsertAnnotationBlack(string &inpt)
	{
		annotation_black.text.push_back("{");
		istringstream iss(inpt);
		copy(istream_iterator<string>(iss),
			istream_iterator<string>(),
				back_inserter<vector<string> >(annotation_black.text));
			
		annotation_black.text.push_back("}");
	}
	void CopyStoredScoreToAnnotation()
	{
		char buff[20];
		//__itoa(this->move_cp_white, buff, 10);
		sprintf( buff, "[%3.2f,", float(this->move_cp_white)/100.0f);
		if( this->annotation_white.text.size() > 0 )
		{
			this->annotation_white.text.clear();
		}
		this->annotation_white.text.push_back("{");
		this->annotation_white.text.push_back(string(buff));
		sprintf(buff, "D=%d]",(int)(this->depthWhite));
		this->annotation_white.text.push_back(string(buff));
		this->annotation_white.text.push_back("}");

		//__itoa(this->move_cp_black, buff, 10);
		sprintf( buff, "[%3.2f,", float(this->move_cp_black)/100.0f);
		if( this->annotation_black.text.size() > 0 )
		{
			this->annotation_black.text.clear();
		}
		this->annotation_black.text.push_back("{");
		this->annotation_black.text.push_back(string(buff));
		sprintf(buff, "D=%d]",(int)(this->depthBlack));
		this->annotation_black.text.push_back(string(buff));
		this->annotation_black.text.push_back("}");

	}


	void RemoveAllAnnotationsFromHere()
	{
		if( this->annotation_white.text.size() > 0 )
		{
			this->annotation_white.text.clear();
		}
		if( this->annotation_black.text.size() > 0 )
		{
			this->annotation_black.text.clear();
		}

		if( pVarWhite )
		{
			pVarWhite->RemoveAllAnnotationsFromHere();
		}

		if( pVarBlack )
		{
			pVarBlack->RemoveAllAnnotationsFromHere();
		}

		if( pNext )
		{
			pNext->RemoveAllAnnotationsFromHere();
		}
	}


	void RemoveAnnotations()
	{
		if( this->annotation_white.text.size() > 0 )
		{
			this->annotation_white.text.clear();
		}
		if( this->annotation_black.text.size() > 0 )
		{
			this->annotation_black.text.clear();
		}
	}

	int ParseAnnotationWhiteForDepth()
	{
		int retVal = 0;
		if( this->annotation_white.text.size() > 0 )
		{
			if( this->annotation_white.text[0] != "{" )
			{
				this->annotation_white.text.clear();
				return retVal;
			}
			else
			{

				string whiteDepth;
				int equalsPoint = this->annotation_white.text[2].find("=");
				
				if( equalsPoint != -1 )
				{
					whiteDepth = this->annotation_white.text[2].substr(equalsPoint+1, this->annotation_white.text[2].length() - equalsPoint-1);
				}
				else
				{
					return 0;
				}
				//string whiteScore = ;
				//std::string test("1234.56");
				std::istringstream inpStream(whiteDepth);
				//double inpValue = 0.0;
				if (inpStream >> retVal)
				{
					// ... Success!!  test is a number.
				}
				else
				{
					retVal = 0;
					// ... Failure!!  test is not a number.
				}
				return retVal;
			}
			//return 0;

		}
		return 0;
	}

	int ParseAnnotationBlackForDepth()
	{


		int retVal = 0;
		if( this->annotation_black.text.size() > 0 )
		{
			if( this->annotation_black.text[0] != "{" )
			{
				this->annotation_black.text.clear();
				return retVal;
			}
			else
			{

				string blackDepth;
				int equalsPoint = this->annotation_black.text[2].find("=");
				
				if( equalsPoint != -1 )
				{
					blackDepth = this->annotation_black.text[2].substr(equalsPoint+1, this->annotation_black.text[2].length() - equalsPoint-1);
				}
				else
				{
					return 0;
				}
				//string whiteScore = ;
				//std::string test("1234.56");
				std::istringstream inpStream(blackDepth);
				//double inpValue = 0.0;
				if (inpStream >> retVal)
				{
					// ... Success!!  test is a number.
				}
				else
				{
					retVal = 0;
					// ... Failure!!  test is not a number.
				}
				return retVal;
			}
			//return 0;

		}
		return 0;
	}

	void ParseAnnotationForScore()
	{
		if( this->annotation_white.text.size() > 0 )
		{
			if( this->annotation_white.text[0] != "{" )
			{
				this->annotation_white.text.clear();
			}
			else
			{
				int commaPoint = this->annotation_white.text[1].find(",");
				string whiteScore ;
				if(  commaPoint != -1 )
				{
					whiteScore = this->annotation_white.text[1].substr(0, commaPoint);
				}
				else
				{
					whiteScore = this->annotation_white.text[1];
				}
				//std::string test("1234.56");
				std::istringstream inpStream(whiteScore);
				//double inpValue = 0.0;
				if (inpStream >> this->move_cp_white)
				{
					// ... Success!!  test is a number.
				}
				else
				{
					this->move_cp_white = 0;
					// ... Failure!!  test is not a number.
				}
			}

		}

		if( this->annotation_black.text.size() > 0 )
		{
			if( this->annotation_black.text[0] != "{" )
			{
				this->annotation_black.text.clear();
			}
			else
			{
				int commaPoint = this->annotation_black.text[1].find(",");
				string blackScore ;
				if(  commaPoint != -1 )
				{
					blackScore = this->annotation_black.text[1].substr(0, commaPoint);
				}
				else
				{
					blackScore = this->annotation_black.text[1];
				}

				//string blackScore = this->annotation_black.text[1];
				//std::string test("1234.56");
				std::istringstream inpStream(blackScore);
				//double inpValue = 0.0;
				if (inpStream >> this->move_cp_black)
				{
					// ... Success!!  test is a number.
				}
				else
				{
					this->move_cp_black = 0;
					// ... Failure!!  test is not a number.
				}
			}

		}
	}

	void WriteScoresToAnnotation(string &str, int whoseMov)
	{
		istringstream iss(str);
		if( whoseMov == 1 )
		{
			if( this->annotation_white.text.size() > 0 )
				this->annotation_white.text.clear();

			this->annotation_white.text.push_back("{");
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
				back_inserter<vector<string> >(annotation_white.text));
			this->annotation_white.text.push_back("}");
		}
		else
		{
			if( this->annotation_black.text.size() > 0 )
				this->annotation_black.text.clear();

			this->annotation_black.text.push_back("{");
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
				back_inserter<vector<string> >(annotation_black.text));
			this->annotation_black.text.push_back("}");
		}
	}


	void PushMoveBlack(Move* move)
	{
		if( move->black == "")
		{
			return;
		}

		if( black == "" )
		{
			if( move->black != "" )
			{
				// copy results
				black = move->black;
				
				if( move->pNext )
				{
					pNext = move->pNext;
					pNext->pPrev = this;

					move->pNext = NULL;

					//this->atEndBlack = false;
					//this->atEndWhite = false;

					this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
				}
				return;
			}
		}
		if( black == move->black )
		{
			// add results
			if( pNext )
			{
				if( move->pNext )
				{
					pNext->PushMoveWhite(move->pNext);
				}
				else return;
			}
			else
			{
				if( move->pNext )
				{
					pNext = move->pNext;
					move->pNext->pPrev = this;

					move->pNext = NULL;

					//this->atEndBlack = false;
					//this->atEndWhite = false;

					this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
				}
				else return;
			}
		}
		else
		{
			Move * temp = pVarBlack;

			while( temp )
			{
				if( temp->black == move->black )
				{
					// add results
					if( temp->pNext )
					{
						if( move->pNext )
						{
							temp->pNext->PushMoveWhite(move->pNext);
							
						}
						return;
					}
					else
					{
						if( move->pNext )
						{
							temp->pNext = move->pNext;
							move->pNext->pPrev = temp;
							move->pNext = NULL;
							
					//		temp->atEndBlack = false;
					//		temp->atEndWhite = false;		

							temp->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
							temp->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						}
						return;
					}
				}
				temp = temp->pVarBlack;
			}

			Move* newNode = getBlackVariationForParse();

			newNode->black = move->black;//CopyNode(move);
			newNode->info = move->info;
			// add results
			if( move->pNext )
			{
				newNode->pNext = move->pNext;
				newNode->pNext->pPrev = newNode;

				move->pNext = NULL;
				//newNode->atEndBlack = false;
				//newNode->atEndWhite = false;

				newNode->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
				newNode->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
			}
			return;
		}
	}

	void PushMoveWhite(Move* move)
	{
		if( move->white == "")
		{
			return;
		}

		if( white == "" )
		{
			if( move->white != "" )
			{
				CopyNode(move);
				pNext = move->pNext;
				if( pNext )
				{
					pNext->pPrev = this;
					//this->atEndBlack = false;
					//this->atEndWhite = false;

					this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );

					move->pNext = NULL;
				}				
				
			}

			return;
		}

		if( white == move->white )
		{
			// add results
			PushMoveBlack(move);

			return;
		}
		else
		{
			Move* temp = pVarWhite;

			while( temp )
			{
				if( temp->white == move->white)
				{
					// add results
					temp->PushMoveBlack(move);
					return;
				}
				temp = temp->pVarWhite;
			}

			Move *newMove = getWhiteVariationForParse();

			newMove->CopyNode(move);
			newMove->pNext = move->pNext;
			
			if( move->pNext )
			{
				move->pNext->pPrev = newMove;
				//newMove->atEndBlack = false;
				//newMove->atEndWhite = false;

				newMove->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
				newMove->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
				move->pNext = NULL;
			}

			return;
		}

	}

	void MergeAccumulatingResults(Move* move, int resultCode)
	{
		if( move->white == white )
		{
			this->annotation_white = move->annotation_white;
			if( move->black == black )
			{
				this->annotation_black = move->annotation_black;
				if( pNext )
				{
					if( move->pNext )
					{
						if( pNext->white != "" )
						{
							pNext->Merge(move->pNext);
							return;
						}
						else
						{
							result = "";

							pNext = move->pNext;
							pNext->pPrev = this;
							


							move->pNext = NULL;
							
							
							this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
							this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
							
							//this->atEndBlack = false;
							//this->atEndWhite = false;
						}
						return;
					}
				}
				else
				{
					if( move->pNext )
					{
						result = "";

						pNext = move->pNext;
						pNext->pPrev = this;

						move->pNext = NULL;

						//this->atEndBlack = false;
						//this->atEndWhite = false;

						this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						
						return;
					}
				}
			}
			else
			{
				if( black == "" )
				{
					if( move->black != "" )
					{
						black = move->black;
						m_fenBlack = move->m_fenBlack;
						this->CopyAnnotationBlackFromBtoA(this, move);
						if( move->pNext )
						{
							result = "";

							pNext = move->pNext;
							pNext->pPrev = this;

							move->pNext = NULL;
							
							//this->atEndBlack = false;
							//this->atEndWhite = false;

							this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
							this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
							
							return;
						}
						//this->atEndBlack = true;
						//this->atEndWhite = false;

						this->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						
					}
					else
					{
						if( pNext )
						{
							delete pNext;
							pNext = NULL;
						}
						//this->atEndBlack = false;
						//this->atEndWhite = true;

						this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						this->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						
						return;
					}
				}
				else 
				{
					if( move->black != "" )
					{
						if( pVarBlack )
						{
							Move* temp = pVarBlack;
							bool match = false;
							while( temp )
							{
								if( temp->black == move->black )
								{
									temp->annotation_black = move->annotation_black;
									if( temp->pNext )
									{
										if( move->pNext )
										{
											temp->pNext->Merge(move->pNext);
											return;
										}
									}
									else if( move->pNext )
									{
										temp->pNext = move->pNext;
										if(move->pNext)
											move->pNext->pPrev = temp;
										move->pUp = this->pUp;
							//			temp->atEndBlack = false;
								//		temp->atEndWhite = false;

										temp->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
										temp->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
										
										return;
									}
								}

								temp = temp->pVarBlack;
							}
						}
						
						Move* blackVar = this->getBlackVariationForParse();

						blackVar->CopyNode(move);
						blackVar->white = "";
						blackVar->pNext = move->pNext;
						if( move->pNext )
							move->pNext->pPrev = blackVar;

						move->pNext = NULL;
						blackVar->pUp = this;
						blackVar->pPrev = this;
						return;
					}
				}
			}
		}
		else
		{
			if( white == "" )
			{
				if( move->white != "" )
				{
					result = "";

					CopyNode(move);

//					this->atEndBlack = false;
	//				this->atEndWhite = false;

					this->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					this->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );

					if( black == "" )
					{
		//				this->atEndBlack = true;
						this->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					}

					pNext = move->pNext;
					move->pNext = NULL;
					pVarWhite = move->pVarWhite;
					pVarBlack = move->pVarBlack;
					
					if( pNext )
					{
						pNext->pPrev = this;
						pNext->pUp = pUp;
					}

					return;
				}
				else 
				{
					if( pNext )
					{
						delete pNext;
						pNext = NULL;
					}
				}return;
			}
			
			if( move->white != "" )
			{
				// check white variations
				if( pVarWhite )
				{
					pVarWhite->Merge(move);
					return;
				}

				Move* whiteVar;
				if( pUp )
				{
					Move* temp2 = pUp;
					
					whiteVar = pUp->getWhiteVariationForParse();	
				}
				else
				{
					whiteVar = getWhiteVariationForParse();
				}

				whiteVar->CopyNode(move);

				whiteVar->pNext = move->pNext;
			
				move->pNext = NULL;

				if( whiteVar->pNext )
				{
					whiteVar->pNext->pPrev = whiteVar;
				}
			}
			return;
		}
	}


	int SetMove(string &str, int color)
	{
		int patternFlag;
		/*if( color == WHITE )
		{
			white = str;
		}
		else
		{
			black = str;

		}
		return PASSED_TEST;*/
		RESET();

		int length = str.length();

		if( length < 2 ) return INCORRECT_LENGTH;
		if( length > 9 ) return INCORRECT_LENGTH;

		int pos = 0;
		bool pieceMove = false;

		

		// {Piece} {file|rank} {captures} [file][rank] {pawn:promote}{glyphs&check} | castles
		if( str[pos] == 'O' )
		{
			if( str.find("O-O") != -1)
			{
				pos=3;
				if( length >= 5 && str[4]=='O')
				{
					pos = 5;
				}

				SET_FLAG(PIECE_FLAG);
				SET_FLAG(FILE_DEST_FLAG);
				SET_FLAG(RANK_DEST_FLAG);

			}	
		}

		if (pos < length && 
			(str[pos] == 'B' || str[pos] == 'N' ||
			 str[pos] == 'R' || str[pos] == 'Q' ||
			 str[pos] == 'K')) 
		{
			pieceMove = true;
			pos++;
			SET_FLAG(PIECE_FLAG);
		}		

		if( pos < length && (str[pos] >= 'a' && str[pos] <= 'h'))
		{
			pos++;
			if( length == pos) return INCORRECT_LENGTH;
			SET_FLAG(FILE_FLAG);
		}
		

		if( pos < length && (str[pos] >= '1' && str[pos] <= '8'))
		{
			if( pos == 0 ) return FORMAT_ERROR;
			pos++;
			SET_FLAG(RANK_FLAG);
			if( (TEST_FLAG( FILE_FLAG) && !TEST_FLAG( PIECE_FLAG))|| 
				(patternFlag == 0x00000111 ))
			{
				UNSET_FLAG( FILE_FLAG);
				UNSET_FLAG( RANK_FLAG);
				SET_FLAG(FILE_DEST_FLAG);
				SET_FLAG(RANK_DEST_FLAG);
			}
		}

		if( pos < length && str[pos] == 'x' )
		{
			if( pos == 0 ) return FORMAT_ERROR;
			pos++;
			if( pos == length-1) return INCORRECT_LENGTH;
			
			SET_FLAG(CAPTURE_FLAG);
		}

		if (pos < length && (str[pos] >= 'a' && str[pos] <= 'h'))
		{
			pos++;
			if( pos == length) return INCORRECT_LENGTH;
			SET_FLAG(FILE_DEST_FLAG);
			
		}

		if (pos < length && str[pos] >= '1' && str[pos] <= '8')
		{
			pos++;
			if( !TEST_FLAG(PIECE_FLAG))
			{
				if( !TEST_FLAG(FILE_FLAG) )
				{
					return FORMAT_ERROR;
				}
			}
			SET_FLAG(RANK_DEST_FLAG);
		}

		if( pos < length && str[pos] == '=' )
		{
			pos++;
			
			SET_FLAG(PROMOTE_FLAG);
			
			if(TEST_FLAG(PIECE_FLAG))
				return FORMAT_ERROR;

			if( pos < length && 
				(str[pos] == 'Q' ||
				 str[pos] == 'R' ||
				 str[pos] == 'N' ||
				 str[pos] == 'B'))
			{
				pos++;
			}
			else return FORMAT_ERROR;
		}

		
		if( pos < length && (str[pos] == '+' || str[pos] == '#'))
		{
			pos++;
		}

		if( pos < length && (str[pos] == '!' || str[pos] == '?'))
		{
			pos++;
			SET_FLAG(GLYPHS);
		}
		if( pos < length && (str[pos] == '!' || str[pos] == '?'))
		{
			pos++;
			SET_FLAG(GLYPHS);
		}

		if( pos == 0 )
			return FORMAT_ERROR;

		if( PATTERN_ZERO() )
			return FORMAT_ERROR;

		
		if ( !TEST_FLAG(FILE_DEST_FLAG) || 
			 !TEST_FLAG(RANK_DEST_FLAG))
			 return FORMAT_ERROR;

		if( !TEST_FLAG(PIECE_FLAG) )
		{
			if( TEST_FLAG(FILE_FLAG) &&
				TEST_FLAG(RANK_FLAG) &&
				TEST_FLAG(CAPTURE_FLAG))
				return FORMAT_ERROR;

			if( TEST_FLAG(FILE_FLAG) &&
				!TEST_FLAG(CAPTURE_FLAG))
				return FORMAT_ERROR;

			if( TEST_FLAG(RANK_FLAG) &&
				!TEST_FLAG(CAPTURE_FLAG))
				return FORMAT_ERROR;

			if( color == WHITE )
			{
				white = str;
			}
			else
			{
				black = str;
			}
			return PASSED_TEST;
		}
		
		if( TEST_FLAG(PIECE_FLAG))
		{
			if( TEST_FLAG(PROMOTE_FLAG))
				return FORMAT_ERROR;
			
			if( color == WHITE )
			{
				white = str;
			}
			else
			{
				black = str;
			}
			return PASSED_TEST;
		}

		return FORMAT_ERROR;
	}

	int SetInfo(string& inf)
	{
		if( inf.length() == 0 )
			return INCORRECT_LENGTH;

		if( !(inf[0] > '0' && inf[0] <='9') )
		{
			return FORMAT_ERROR;
		}

		for(int i=0; i < inf.length(); i++ )
			if( !(inf[0] > '0' && inf[0] <='9') )
				return FORMAT_ERROR;

		int moveToNum = atoi(inf.c_str());
		
		if( pPrev )
		{
			if( pPrev->info == "" )
			{
				this->info = inf;
				return PASSED_TEST;
			}
			else
			{
				int prevMoveToNum = atoi(pPrev->info.c_str());

				if( pUp )
				{
					if( prevMoveToNum != moveToNum )
					{
						return FORMAT_ERROR;
					}
				}
				else if( prevMoveToNum != moveToNum-1 )
				{
					return FORMAT_ERROR;
				}
			
				this->info = inf;

				return PASSED_TEST;
			}
		}
		else
		{
			return FORMAT_ERROR;
		}
		return FORMAT_ERROR;
	}


	inline bool MoveFlagIsSet( unsigned char X )
	{
		return (flags & X);
	}

	inline void SetMoveFlag( unsigned char X )
	{
		flags |= X;
	}

	inline void UnsetMoveFlag( unsigned char X)
	{
		flags &= ~X;
	}

	inline void ResetMoveFlag()
	{
		flags = 0;
	}

	inline bool MoveFlagIsZero()
	{
		return flags==0;
	}
	//typedef enum varIds{ A, B, C, D, E, F, G, H };

	// flags?

	string white;
	string black;
	//int number;

	//string annotations;
	//string text_white;
	//string text_black;
	Comment annotation_white;
	Comment annotation_black;
	//int capture;	// used?
	//int promote;	// used?
	string info;
	string result;


	Line* m_line;
	

	string m_fenWhite;	
	string m_fenBlack;


	
	uint64 zhashKeyWhite;
	uint64 zhashKeyBlack;
	//int end_white_black;

	MyChessMove whiteMove;
	MyChessMove blackMove;

	Move* pNext;
	Move* pPrev;
	Move* pVarWhite;
	Move* pVarBlack;
	Move* pUp;

	//int variation_count_white;
	//int variation_count_black;

	//string variation_id;
	
	// meta data
	// can this all be indexed?
	// can the colors be integers indexing a set of selected colors?
	// INDEXED. SOLVED
	unsigned char rm;
	unsigned char rmw;
	unsigned char rmb;
	/*RenderMoveColor renderMoveColor;
	RenderMoveColor renderMoveColorWhite;
	RenderMoveColor renderMoveColorBlack;*/
	int move_cp_white;
	int move_cp_black;
	//string depthWhite;			// strings or numbers
	//string depthBlack;
	unsigned char depthWhite;
	unsigned char depthBlack;
	
	//string analysis_engine;		// unsigned char unneeded
	//int depth_analyzed;			// unsigned char

	unsigned char flags;
	
	// flags ... total = 8 bits atm
	//bool visited;
	//bool displayVariations;
	// flags?
	//bool atEndWhite;
	//bool atEndBlack;

	//bool hasFenWhite;
	//bool hasFenBlack;

	//int x;
	//int y;

//	unsigned char lm_white; // compressed values for moves.
//	unsigned char lm_black;
};



class Game
{
public:
//	friend class Move;
	Move* pCurrMove;
	Move* pRoot;
	Move* pBase; // the base from which the tree groweth
	int selectedMove;
	string whiteName;
	string blackName;
	string eventName;
	string resultName;
	string dateName;
	bool bHasGeneratedFens;

	int sourceGameIndex;
	string SourceFileName;
	bool isMemberOfPGN;

	int mainlinePlyCount;
	

	
	Game()
	{
		move_counter = -1;
		white_black = 0;
		game_curr_move = 0;
		//pCurrMove = NULL;
		pRoot = new Move();
		pBase = new Move();
		pBase->pNext = pRoot;
		pRoot->pPrev = pBase;
		
		pBase->SetFenBlack(string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
		pBase->SetFenWhite(string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));

		//pRoot->info = "1"; // default
		pCurrMove = pRoot;
		selectedMove = 0;
		end_white_black = -1;
		bHasGeneratedFens = false;
		gameBeginsAtStartPos = true;
		parse_error = "";
		game_text = "";
		vec_moves = NULL;
		result = "*";

		SourceFileName = "";
		isMemberOfPGN = false;

		sourceGameIndex = 0;
		SourceFileName = "";
		isMemberOfPGN = false;

		mainlinePlyCount = 0;
	}

	~Game()
	{
	//	Reset();
		// clear vectors
		if( pBase )
		{
			delete pBase; // and all the pNext's are deleted recursively
			pBase = NULL;
		}
	}


	
	void ToggleAllVariationFolds()
	{
		Move* pVarRoot = pCurrMove->GetVariationRoot();
		if( pVarRoot != NULL )
		{
			pCurrMove = pVarRoot;
		}

		Move* temp = pRoot;

		while( temp )
		{
			if( temp->IsTrueDisplayVariations() )
			{
				temp->SetDisplayVariations(false);
			}
			temp = temp->pNext;
		}
	}

	void CopyStoredScoresToAnnotationForMainLine()
	{
		Move* temp = pRoot;
		
		while( temp )
		{
			temp->CopyStoredScoreToAnnotation();
			temp = temp->pNext;
		}
	}

	void CopyStoredScoresToAnnotationForVariation()
	{
		Move* temp = pCurrMove;

		while( temp->pUp == NULL )
		{
			temp = temp->pPrev;
		}
		
		while( temp )
		{
			temp->CopyStoredScoreToAnnotation();
			temp = temp->pNext;
		}
	}



	void CountFullAnalysisMatches(int &numt1w, int& numt2w, int &numt3w, int &numt4w, int &numMovesW,
								 int &numt1b, int& numt2b, int &numt3b, int &numt4b, int &numMovesB)
	{
		Move* temp = pRoot;
		
		while( temp )
		{
			if( temp->pVarWhite )
			{
				numMovesW++;

				Move* tempVar = temp->pVarWhite;
				int count = 0;
				while( tempVar )
				{
					if( tempVar->white == temp->white )
					{
						
						if( count == 0 )
						{
							numt1w++;
						}
						else if( count == 1 )
						{
							numt2w++;
						}
						else if( count == 2 )
						{
							numt3w++;
						}
						else if( count == 3 )
						{
							numt4w++;
						}
					}
					count++;
					tempVar = tempVar->pVarWhite;
				}
			}

			if( temp->pVarBlack )
			{
				numMovesB++;

				Move* tempVar = temp->pVarBlack;
				int count = 0;
				while( tempVar )
				{
					if( tempVar->black == temp->black )
					{
						if( count == 0 )
						{
							numt1b++;
						}
						else if( count == 1 )
						{
							numt2b++;
						}
						else if( count == 2 )
						{
							numt3b++;
						}
						else if( count == 3 )
						{
							numt4b++;
						}
					}
					count++;
					tempVar = tempVar->pVarBlack;
				}
			}
			
			temp = temp->pNext;
		}
	}

	void ColorCodeFullAnalysisMatchesInVariation(ColorPallete &pallete)
	{

		Move* temp = pCurrMove;
		while( temp->pUp == NULL )
		{
			temp = temp->pPrev;
		}
		
		while( temp )
		{
			if( temp->pVarWhite )
			{
				Move* tempVar = temp->pVarWhite;
				int count = 0;
				while( tempVar )
				{
					if( tempVar->white == temp->white )
					{
						
						if( count == 0 )
						{
							tempVar->SetVariationRenderColor(238,113,25, pallete);
							temp->SetRenderColorWhite(238,113,25,pallete);
						}
						else if( count == 1 )
						{
							tempVar->SetVariationRenderColor(19,116,50,pallete);
							temp->SetRenderColorWhite(19,116,50,pallete);
						}
						else if( count == 2 )
						{
							tempVar->SetVariationRenderColor(4,148,87,pallete);
							temp->SetRenderColorWhite(4,148,87,pallete);
						}
						else if( count == 3 )
						{
							tempVar->SetVariationRenderColor(46,20,141,pallete);
							temp->SetRenderColorWhite(46,20,141,pallete);
						}
					}
					count++;
					tempVar = tempVar->pVarWhite;
				}
			}

			if( temp->pVarBlack )
			{
				Move* tempVar = temp->pVarBlack;
				int count = 0;
				while( tempVar )
				{
					if( tempVar->black == temp->black )
					{
						if( count == 0 )
						{
							tempVar->SetVariationRenderColor(238,113,25,pallete);
							temp->SetRenderColorBlack(238,113,25,pallete);
						}
						else if( count == 1 )
						{
							tempVar->SetVariationRenderColor(19,116,50,pallete);
							temp->SetRenderColorBlack(19,116,50,pallete);
						}
						else if( count == 2 )
						{
							tempVar->SetVariationRenderColor(4,148,87,pallete);
							temp->SetRenderColorBlack(4,148,87,pallete);
						}
						else if( count == 3 )
						{
							tempVar->SetVariationRenderColor(46,20,141,pallete);
							temp->SetRenderColorBlack(46,20,141,pallete);
						}
					}
					count++;
					tempVar = tempVar->pVarBlack;
				}
			}
			temp = temp->pNext;
		}
	}

	void ColorCodeFullAnalysisMatches(ColorPallete &pallete)
	{
		Move* temp = pRoot;
		
		while( temp )
		{
			if( temp->pVarWhite )
			{
				Move* tempVar = temp->pVarWhite;
				int count = 0;
				while( tempVar )
				{
					if( tempVar->white == temp->white )
					{
						
						if( count == 0 )
						{
							tempVar->SetVariationRenderColor(238,113,25,pallete);
							temp->SetRenderColorWhite(238,113,25,pallete);
						}
						else if( count == 1 )
						{
							tempVar->SetVariationRenderColor(19,116,50,pallete);
							temp->SetRenderColorWhite(19,116,50,pallete);
						}
						else if( count == 2 )
						{
							tempVar->SetVariationRenderColor(4,148,87,pallete);
							temp->SetRenderColorWhite(4,148,87,pallete);
						}
						else if( count == 3 )
						{
							tempVar->SetVariationRenderColor(46,20,141,pallete);
							temp->SetRenderColorWhite(46,20,141,pallete);
						}
					}
					count++;
					tempVar = tempVar->pVarWhite;
				}
			}

			if( temp->pVarBlack )
			{
				Move* tempVar = temp->pVarBlack;
				int count = 0;
				while( tempVar )
				{
					if( tempVar->black == temp->black )
					{
						if( count == 0 )
						{
							tempVar->SetVariationRenderColor(238,113,25,pallete);
							temp->SetRenderColorBlack(238,113,25,pallete);
						}
						else if( count == 1 )
						{
							tempVar->SetVariationRenderColor(19,116,50,pallete);
							temp->SetRenderColorBlack(19,116,50,pallete);
						}
						else if( count == 2 )
						{
							tempVar->SetVariationRenderColor(4,148,87,pallete);
							temp->SetRenderColorBlack(4,148,87,pallete);
						}
						else if( count == 3 )
						{
							tempVar->SetVariationRenderColor(46,20,141,pallete);
							temp->SetRenderColorBlack(46,20,141,pallete);
						}
					}
					count++;
					tempVar = tempVar->pVarBlack;
				}
			}
			temp = temp->pNext;
		}
	}

	Move* TraverseToLastMove()
	{
		Move* temp = this->pRoot;
		while( temp->pNext )
		{
			temp = temp->pNext;
		}

		return temp;
	}

	void PrepareForVisitor()
	{
		pBase->PrepareForVisitor();
	}


	void SetHasGeneratedFens()
	{
		bHasGeneratedFens = true;
	}

	bool HasGeneratedFens()
	{
		return bHasGeneratedFens;
	}

	void SaveGame(char* filename)
	{
		ofstream of;
		of.open(filename); 
		for( int i = 0; i < vec_header.size(); i++ )
			of << vec_header[i].data.c_str() << endl;
		
		/*if( pBase->m_fenWhite != "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
		{
			of << "[FEN \"";
			
			of << pBase->m_fenWhite;

			of<< "\"]";

			of << endl;
		}*/

		of<<endl;

		pRoot->SaveToFile(of);

		of << " "<<this->result;
		
		of<<endl;
	}

	ostringstream& GameToBuffer(ostringstream& of)
	{

		//of.open(filename); 
		for( int i = 0; i < vec_header.size(); i++ )
			of << vec_header[i].data.c_str() << endl;
		
		
		/*if( pBase->m_fenWhite != "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
		{
			bool foundFen = false;

			if( !foundFen )
			{
				of << "[FEN \"";
			
				of << pBase->m_fenWhite;

				of<< "\"]";

				of << endl;
			}
		}*/

		of<<endl;

		pRoot->SaveToStream(of);
		
		of<<endl;

		return of;
	}

	void DeleteAllVariationsOfCurrentMove()
	{
		Move* temp = pCurrMove;
		
		if( temp->pVarWhite )
		{
			delete temp->pVarWhite;
			temp->pVarWhite = NULL;
		}

		if( temp->pVarBlack )
		{
			delete temp->pVarBlack;
			temp->pVarBlack = NULL;
		}
	}


	void DeleteVariation()
	{
		// if the node has an up pointer
		if( pCurrMove->pUp == NULL )
		{
			Move* temp = pCurrMove;
			while( temp->pUp == NULL )
			{
				temp = temp->pPrev;
			}
			pCurrMove = temp;
		}
		
		if( pCurrMove->white == "" )
		{
			Move* Up = pCurrMove->pUp;
			Move* temp = pCurrMove->pUp->pVarBlack;
			if( temp == pCurrMove )
			{
				Up->pVarBlack = pCurrMove->pVarBlack;
				pCurrMove->pVarBlack = NULL;
				delete pCurrMove;
				pCurrMove = Up;
				return;
			}
			else
			{
				while( temp->pVarBlack != pCurrMove )
				{
					temp = temp->pVarBlack;
				}
				temp->pVarBlack = pCurrMove->pVarBlack;
				pCurrMove->pVarBlack = NULL;
				delete pCurrMove;
				pCurrMove = Up;
				return;

			}
		}
		else
		{
			Move* Up = pCurrMove->pUp;
			Move* temp = pCurrMove->pUp->pVarWhite;
			if( temp == pCurrMove )
			{					
				Up->pVarWhite = pCurrMove->pVarWhite;
				pCurrMove->pVarWhite = NULL;
				delete pCurrMove;
				pCurrMove = Up;
				return;
			}
			else
			{
				while( temp->pVarWhite != pCurrMove )
				{
					temp = temp->pVarWhite;
				}
				temp->pVarWhite = pCurrMove->pVarWhite;
				pCurrMove->pVarWhite = NULL;
				delete pCurrMove;
				pCurrMove = Up;
				return;
			}
		}

	}

	void RemoveResultNode()
	{
		Move* temp = pRoot;
		while( temp->pNext )
		{

			temp = temp->pNext;
		}

		if( temp->black == "" && temp->white != "")
		{
			temp->result = "";
			return;
		}
		else if( temp->black == "" && temp->white == "")
		{
			delete temp;
			temp->pPrev->pNext = NULL;
		}
	}

	void AddResultNode(string &result)
	{
		Move* temp = pRoot;
		while( temp->pNext )
		{

			temp = temp->pNext;
		}

		if( temp->black == "")
		{
			temp->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
			//temp->atEndBlack = true;
			temp->result = result;
			return;
		}
		else
		{
			if( temp->pNext == NULL )
			{
				temp->pNext = new Move();
				//temp->pNext->x = 0;
				//temp->pNext->y = 0;
				temp->UnsetMoveFlag( MOVE_FLAG_X );
				temp->UnsetMoveFlag( MOVE_FLAG_Y );
				temp->pNext->pPrev = temp;
				temp->pNext->result = result;
			}
			else
			{
				if( temp->white != "" )
				{
					// error ...
					temp->result = result;
				}
				else
				{
					temp->result = result;
				}
			}
		}


	}

	void CopyHeader(Game* gm)
	{
		this->vec_header.clear();
		for( int i = 0; i < gm->vec_header.size(); i++ )
		{
			HeaderElem hd;
			hd.data = gm->vec_header[i].data;

			vec_header.push_back(hd);
		}
	}

	void CreateEmptyHeader()
	{
/*
1) Event (the name of the tournament or match event)

2) Site (the location of the event)

3) Date (the starting date of the game)

4) Round (the playing round ordinal of the game)

5) White (the player of the white pieces)

6) Black (the player of the black pieces)

7) Result (the result of the game)
*/

		HeaderElem eventElem;
		eventElem.data += "[Event \"?\"]";
		
		HeaderElem siteElem;
		siteElem.data += "[Site \"?\"]";

		HeaderElem dateElem;
		dateElem.data += "[Date \"????.??.??\"]";

		HeaderElem whiteElem;
		whiteElem.data += "[White \"?\"]";

		HeaderElem blackElem;
		blackElem.data += "[Black \"?\"]";

		HeaderElem resultElem;
		resultElem.data += "[Result \"?\"]";

		vec_header.push_back(eventElem);
		vec_header.push_back(siteElem);
		vec_header.push_back(dateElem);
		vec_header.push_back(whiteElem);
		vec_header.push_back(blackElem);
		vec_header.push_back(resultElem);
	}

	void SortHeader()
	{	
		vector<HeaderElem> temp;

		string names[] = {"White", "Black", "Event", "Date", "Result", "Termination"};

		for( unsigned int j = 0; j < 5; j++ )
		{
			bool found = false;
			for( unsigned int i = 0; i < vec_header.size(); i++ )
			{
				if( vec_header[i].data.substr(1, names[j].length()) == names[j] )
				{
					temp.push_back(vec_header[i]);
					found = true; 
					break;
				}
			}

			if( !found )
			{
				HeaderElem tempElem;
				tempElem.data += "[" + names[j] +"\"?\"]";
			
				temp.push_back(tempElem);
			}
		}

		vec_header.clear();
		for( unsigned int i = 0; i < temp.size(); i++ )
		{
			vec_header.push_back(temp[i]);
		}
	}

	string GetHeaderElem(string name)
	{
		for( unsigned int i = 0; i < vec_header.size(); i++ )
		{
			if( vec_header[i].data.find( name )!=-1 )
			{
				return GetSubstringInQuotes(vec_header[i].data);
			}
		}
		return "*";
	}

	string GetSubstringInQuotes(string &inpt)
	{
		int openQuotes = inpt.find('"');
		int closeQuotes = inpt.find_last_of('"');

		int length = inpt.length();
		int size = (length-openQuotes+1) - (length - closeQuotes);//.c_str()
		return inpt.substr(openQuotes+1, size-2);
	}

	string SetSubstringInQuotes(string &inpt, string &set)
	{
		int openQuotes = inpt.find('"');
		int closeQuotes = inpt.find_last_of('"');

		int length = inpt.length();
		int size = (length-openQuotes+1) - (length - closeQuotes);//.c_str()
		string::iterator start;
		string::iterator end;
		start = inpt.begin() + (openQuotes+1);
		end = inpt.begin() + (closeQuotes);

		inpt.erase(start, end);
		inpt.insert(openQuotes+1, set);

		return inpt;
	}

	void WriteTerminationIntoHeader(string &termination)
	{
		bool foundTermination = false;
		for( int j = 0; j < vec_header.size(); j++ )
		{
			string currString = vec_header[j].data;
						
			int findTermination = -1; 
			findTermination = currString.find("Termination ");
			int findTerminationLc = -1; 
			findTerminationLc = currString.find("termination ");


			if( (findTermination != -1) )
			{			 
				SetSubstringInQuotes(vec_header[j].data,termination);
				foundTermination = true;
				break;
			}
			if( (findTerminationLc != -1) )
			{
				SetSubstringInQuotes(vec_header[j].data,termination);
				foundTermination = true;
				break;
			}
		}

		if( !foundTermination )
		{
			HeaderElem terminationElem;
			terminationElem.data += "[Termination \"?\"]";
			SetSubstringInQuotes(terminationElem.data,termination);
			vec_header.push_back(terminationElem);
		}
	}

	void WriteEventNameIntoHeader(string &eventName)
	{
		bool foundEvent = false;
		for( int j = 0; j < vec_header.size(); j++ )
		{
			string currString = vec_header[j].data;
						
			int findEvent = -1; 
			findEvent = currString.find("Event ");
			int findEventLc = -1; 
			findEventLc = currString.find("event ");


			if( (findEvent != -1) )
			{			 
				SetSubstringInQuotes(vec_header[j].data,eventName);
				foundEvent = true;
				break;
			}
			if( (findEventLc != -1) )
			{
				SetSubstringInQuotes(vec_header[j].data,eventName);
				foundEvent = true;
				break;
			}
		}

		if( !foundEvent )
		{
			HeaderElem EventElem;
			EventElem.data += "[Event \"?\"]";
			SetSubstringInQuotes(EventElem.data,eventName);
			vec_header.push_back(EventElem);
		}
	}

	void WriteECONameIntoHeader(string &ECOName)
	{
		bool foundECO = false;
		for( int j = 0; j < vec_header.size(); j++ )
		{
			string currString = vec_header[j].data;
						
			int findECO = -1; 
			findECO = currString.find("ECO ");
			int findECOLc = -1; 
			findECOLc = currString.find("Eco ");


			if( (findECO != -1) )
			{			 
				SetSubstringInQuotes(vec_header[j].data,ECOName);
				foundECO = true;
				break;
			}
			if( (findECOLc != -1) )
			{
				SetSubstringInQuotes(vec_header[j].data,ECOName);
				foundECO = true;
				break;
			}
		}

		if( !foundECO )
		{
			HeaderElem ECOElem;
			ECOElem.data += "[ECO \"?\"]";
			SetSubstringInQuotes(ECOElem.data,ECOName);
			vec_header.push_back(ECOElem);
		}
	}
	

	void WriteResultIntoHeader()
	{
		if( this->result == "")
			return;

		bool foundResult = false;
		for( int j = 0; j < vec_header.size(); j++ )
		{
			string currString = vec_header[j].data;
						
			int findResult = -1; 
			findResult = currString.find("Result ");
			int findResultLc = -1; 
			findResultLc = currString.find("result ");


			if( (findResult != -1) )
			{			 
				SetSubstringInQuotes(vec_header[j].data,this->result);
				foundResult = true;
			}
			if( (findResultLc != -1) )
			{
				SetSubstringInQuotes(vec_header[j].data,this->result);
				foundResult = true;
			}
		}

		if( !foundResult )
		{
			HeaderElem resultElem;
			resultElem.data += "[Result \"?\"]";
			SetSubstringInQuotes(resultElem.data,this->result);
			vec_header.push_back(resultElem);
		}
	}

	void SetHeaderConvenienceFunction(string whiteName, string blackName, string eventName)
	{


		for( int j = 0; j < vec_header.size(); j++ )
		{
			string currString = vec_header[j].data;
						
			int findWhite = -1; 
			findWhite = currString.find("White ");
			int findWhiteLc = -1; 
			findWhiteLc = currString.find("white ");
			int findWhiteElo = -1;
			findWhiteElo = currString.find("WhiteElo ");
			int findWhiteEloLc = -1;
			findWhiteEloLc = currString.find("whiteelo ");
			if( (findWhite != -1) && (findWhiteElo != findWhite))
			{			 
				SetSubstringInQuotes(vec_header[j].data,whiteName);
			}
			if( (findWhiteLc != -1) && (findWhiteEloLc != findWhiteLc))
			{
				SetSubstringInQuotes(vec_header[j].data,whiteName);
			}


			int findBlack = -1; 
			findBlack = currString.find("Black ");
			int findBlackLc = -1;
			findBlackLc = currString.find("black ");
			int findBlackElo = -1;
			findBlackElo = currString.find("BlackElo ");
			int findBlackEloLc = -1;
			findBlackEloLc = currString.find("blackelo ");
			if( (findBlack != -1) && (findBlackElo != findBlack) )
			{
				SetSubstringInQuotes(vec_header[j].data,blackName);
			}
			if( (findBlackLc != -1) && (findBlackEloLc != findBlackLc) )
			{
				SetSubstringInQuotes(vec_header[j].data,blackName);
			}


			int findEvent = -1;
			findEvent = currString.find("Event ");
			int findEventLc = -1;
			findEventLc = currString.find("event ");
			if( findEvent!= -1 )
			{

				SetSubstringInQuotes(vec_header[j].data,eventName);
			}
			if( findEventLc!= -1 )
			{
				SetSubstringInQuotes(vec_header[j].data,eventName);
			}
		}
	}

	void ParseHeader()
	{
		pBase->SetFenBlack(string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
		pBase->SetFenWhite(string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));

		if( vec_header.size() > 0 )
		{
			string strEvent;
			string strDate;
			string strWhite;
			string strBlack;
			string strResult;

			for( int j = 0; j < vec_header.size(); j++ )
			{
				string currString = vec_header[j].data;
						
				int findEvent = -1;
				findEvent = currString.find("Event ");
				int findEventLc = -1;
				findEventLc = currString.find("event ");
				if( findEvent!= -1 )
				{

					eventName =  GetSubstringInQuotes(currString);
				}
				if( findEventLc!= -1 )
				{
					eventName =  GetSubstringInQuotes(currString);
				}

				int findDate = -1;
				findDate = currString.find("Date ");
				int findDateLc = -1;
				findDateLc = currString.find("date ");
				if( findDate != -1 )
				{
					dateName =  GetSubstringInQuotes(currString);
				}
				if( findDateLc != -1 )
				{
					dateName =  GetSubstringInQuotes(currString);
				}

				int findWhite = -1; 
				findWhite = currString.find("White ");
				int findWhiteLc = -1; 
				findWhiteLc = currString.find("white ");
				int findWhiteElo = -1;
				findWhiteElo = currString.find("WhiteElo ");
				int findWhiteEloLc = -1;
				findWhiteEloLc = currString.find("whiteelo ");
				if( (findWhite != -1) && (findWhiteElo != findWhite))
				{
							 
					whiteName =  GetSubstringInQuotes(currString);
				}
				if( (findWhiteLc != -1) && (findWhiteEloLc != findWhiteLc))
				{
					whiteName =  GetSubstringInQuotes(currString);
				}

				int findBlack = -1; 
				findBlack = currString.find("Black ");
				int findBlackLc = -1;
				findBlackLc = currString.find("black ");
				int findBlackElo = -1;
				findBlackElo = currString.find("BlackElo ");
				int findBlackEloLc = -1;
				findBlackEloLc = currString.find("blackelo ");
				if( (findBlack != -1) && (findBlackElo != findBlack) )
				{
					blackName =  GetSubstringInQuotes(currString);
				}
				if( (findBlackLc != -1) && (findBlackEloLc != findBlackLc) )
				{
					blackName =  GetSubstringInQuotes(currString);
				}

				int findResult = -1; 
				findResult = currString.find("Result ");
				int findResultLc = -1; 
				findResultLc = currString.find("result ");
				if( findResult != -1 )
				{
					result =  GetSubstringInQuotes(currString);
				}	
				if( findResultLc != -1 )
				{
					result =  GetSubstringInQuotes(currString);
				}	

				int findFen = -1;
				findFen = currString.find("FEN ");
				if( findFen != -1 )
				{
					if(pRoot->white == "" && /*pRoot->x == 1*/ pRoot->MoveFlagIsSet(MOVE_FLAG_X) )
					{
						pRoot->m_fenWhite = GetSubstringInQuotes(currString);
						pRoot->zhashKeyWhite = hash_from_fen((char*)pRoot->m_fenWhite.c_str());
						
						pRoot->InsertHashWhite(pRoot);
						
						pBase->m_fenBlack = GetSubstringInQuotes(currString);
						pBase->zhashKeyBlack = hash_from_fen((char*)pBase->m_fenBlack.c_str());
						
						pBase->InsertHashBlack(pBase);

						pBase->m_fenWhite = GetSubstringInQuotes(currString);
						pBase->zhashKeyWhite = hash_from_fen((char*)pBase->m_fenWhite.c_str());
						
						pBase->InsertHashWhite(pBase);

						gameBeginsAtStartPos = false;
					}
					else {
						pBase->m_fenBlack = GetSubstringInQuotes(currString);
						pBase->zhashKeyBlack = hash_from_fen((char*)pBase->m_fenBlack.c_str());
						pBase->InsertHashBlack(pBase);
						pBase->m_fenWhite = GetSubstringInQuotes(currString);
						pBase->zhashKeyWhite = hash_from_fen((char*)pBase->m_fenWhite.c_str());
						pBase->InsertHashWhite(pBase);
						gameBeginsAtStartPos = false;
					}
				}
			}
		}

		//SortHeader();
	}

	void CopyGame(Game *pGame)
	{
		this->CopyHeader(pGame);

		pRoot->Copy(pGame->pRoot);
	}

	bool HasVariations()
	{
		Move* temp = pBase;
		while( temp )
		{
			if( temp->pVarWhite || temp->pVarBlack )
			{
				return true;
			}
			else
			{
				temp = temp->pNext;
			}
		}
		return false;
	}

	/*void RemoveNags()
	{

	}*/

	Move* GetLastMove()
	{
		Move* temp = this->pRoot;
		while( temp->pNext )
		{
			temp = temp->pNext;
		}
		return temp;
	}

	void TraverseVariation(Move* pVarMove, vector< string > &vec_game_strings, vector< string > &last_move_annotations)
	{
		Move* temp = pVarMove;

		while( temp )
		{
			if( temp->MoveFlagIsSet(MOVE_FLAG_AT_END_WHITE) )
			{
				vec_game_strings.push_back(temp->GetSanStringsFromStart(1));
				last_move_annotations.push_back(temp->GetAnnotationWhite());
				break;
			}
			if(	temp->MoveFlagIsSet(MOVE_FLAG_AT_END_BLACK))
			{
				vec_game_strings.push_back(temp->GetSanStringsFromStart(0));
				last_move_annotations.push_back(temp->GetAnnotationBlack());
				break;
			}

			if( temp->pVarWhite )
			{
				Move* tempVar = temp;
				while( tempVar->pVarWhite )
				{
					TraverseVariation( tempVar->pVarWhite, vec_game_strings, last_move_annotations );
					tempVar = tempVar->pVarWhite;
				}
					
			}

			if( temp->pVarBlack )
			{
				Move* tempVar = temp;
				while( tempVar->pVarBlack )
				{
					TraverseVariation( tempVar->pVarBlack, vec_game_strings, last_move_annotations );
					tempVar = tempVar->pVarBlack;
				}					
			}

			temp->Visit();

			temp = temp->pNext;
		}
	}

	bool MergeGame(Game* pGame)
	{
		if( pGame->HasVariations() )
		{
			vector< string > vec_game_strings;
			vector< string > last_move_annotations;
			pGame->PrepareForVisitor();

			Move* temp = pGame->pBase;

			while( temp )
			{
				if( temp->MoveFlagIsSet(MOVE_FLAG_AT_END_WHITE) )
				{
					vec_game_strings.push_back(temp->GetSanStringsFromStart(1));
					last_move_annotations.push_back(temp->GetAnnotationWhite());

					break;
				}
				if(	temp->MoveFlagIsSet(MOVE_FLAG_AT_END_BLACK))
				{
					vec_game_strings.push_back(temp->GetSanStringsFromStart(0));
					last_move_annotations.push_back(temp->GetAnnotationBlack());
					break;
				}

				if( temp->pVarWhite )
				{
					Move* tempVar = temp;
					while( tempVar->pVarWhite )
					{
						TraverseVariation( tempVar->pVarWhite, vec_game_strings, last_move_annotations );
						tempVar = tempVar->pVarWhite;
					}
					
				}

				if( temp->pVarBlack )
				{
					Move* tempVar = temp;
					while( tempVar->pVarBlack )
					{
						TraverseVariation( tempVar->pVarBlack, vec_game_strings, last_move_annotations );
						tempVar = tempVar->pVarBlack;
					}					
				}

				temp->Visit();

				temp = temp->pNext;
			}

			for( int i = 0; i < vec_game_strings.size(); i++ )
			{
				Game* game = new Game();
				game->game_text = vec_game_strings[i];
				if( !game->Parse() )
				{
					// error!
					continue;
				}
				Move* last_move = game->GetLastMove();
				if( last_move->MoveFlagIsSet( MOVE_FLAG_AT_END_WHITE ))
				{
					last_move->InsertAnnotationWhite( last_move_annotations[i] );
				}
				else if( last_move->MoveFlagIsSet( MOVE_FLAG_AT_END_BLACK ) )
				{
					last_move->InsertAnnotationBlack( last_move_annotations[i] );
				}
				game->pRoot->RemoveResultString();
				this->pRoot->Merge(game->pRoot);

				delete game;

			}
		}
		else
		{
			pGame->pRoot->RemoveResultString();
			this->pRoot->Merge(pGame->pRoot);
		}
		//this->pRoot->PushMoveWhite(pGame->pRoot);
		return true;


/*		if( pRoot->info == pGame->pRoot->info )
		{
			if( pGame->pRoot->white != "" )
			{
				if( pGame->pRoot->white != pRoot->white )
				{
					Move* pMoveInsert = pRoot->getWhiteVariationForParse();
					pMoveInsert->CopyNode(pGame->pRoot);
					
					pMoveInsert->pNext = pGame->pRoot->pNext;
					if( pGame->pRoot->pNext )
						pGame->pRoot->pNext->pPrev = pMoveInsert;

					pMoveInsert->pVarWhite = pGame->pRoot->pVarWhite;
					//if( pMoveInsert->pVarWhite )
					//	pMoveInsert->variation_count_white++;
					pMoveInsert->pVarBlack = pGame->pRoot->pVarBlack;
					//if( pMoveInsert->pVarBlack )
					//	pMoveInsert->variation_count_black++;

					pGame->pRoot->pNext = NULL;
					pGame->pRoot->pVarWhite = NULL;
					pGame->pRoot->pVarBlack = NULL;

					return true;
				}
				else if ( pGame->pRoot->black != pRoot->black )
				{
					Move* pMoveInsert = pRoot->getBlackVariationForParse();
					pMoveInsert->CopyNode(pGame->pRoot);
					
					pMoveInsert->pNext = pGame->pRoot->pNext;
					pGame->pRoot->pNext->pPrev = pMoveInsert;
					
					pMoveInsert->white = "";

					pMoveInsert->pVarWhite = pGame->pRoot->pVarWhite;
					//if( pMoveInsert->pVarWhite )
					//	pMoveInsert->variation_count_white++;
					pMoveInsert->pVarBlack = pGame->pRoot->pVarBlack;
					//if( pMoveInsert->pVarBlack )
					//	pMoveInsert->variation_count_black++;

					pGame->pRoot->pNext = NULL;
					pGame->pRoot->pVarWhite = NULL;
					pGame->pRoot->pVarBlack = NULL;

					return true;
				}
				else
				{
					Move* tempIter = pRoot;
					Move* pGameIter = pGame->pRoot;
					// first check to see that game 2 does not match to the end
					while( tempIter && pGameIter)
					{
						if( (tempIter->white != pGameIter->white) && (pGameIter->white != ""))
						{
							// now check the variations ...
							if( pGameIter->pVarWhite )
							{
								Move* temppVarWhite = pGameIter->pVarWhite;
								while( temppVarWhite )
								{
									if( temppVarWhite->white == tempIter->white )
									{
										// match
									}
									temppVarWhite = temppVarWhite->pVarWhite;
								}

							}
							else
							{
								Move* pMoveInsert = tempIter->getWhiteVariationForParse();
								pMoveInsert->CopyNode(pGameIter);

								pMoveInsert->pNext = pGameIter->pNext;
								if( pGameIter->pNext )
									pGameIter->pNext->pPrev = pMoveInsert;
							
								pMoveInsert->pVarWhite = pGameIter->pVarWhite;
								//if( pMoveInsert->pVarWhite )
								//	pMoveInsert->variation_count_white++;
								pMoveInsert->pVarBlack = pGameIter->pVarBlack;
								//if( pMoveInsert->pVarBlack )
								//	pMoveInsert->variation_count_black++;

								pGameIter->pNext = NULL;
								pGameIter->pVarWhite = NULL;
								pGameIter->pVarBlack = NULL;
								return true;
							}
						}
						
						if( (tempIter->black != pGameIter->black) ||
							(pGameIter->white == ""))
						{
							Move* pMoveInsert = tempIter->getBlackVariationForParse();
							pMoveInsert->CopyNode(pGameIter);
							
							pMoveInsert->pNext = pGameIter->pNext;
							if( pGameIter->pNext )
								pGameIter->pNext->pPrev = pMoveInsert;
							
							pMoveInsert->white = "";

							pMoveInsert->pVarWhite = pGameIter->pVarWhite;
							//if( pMoveInsert->pVarWhite )
							//	pMoveInsert->variation_count_white++;
							pMoveInsert->pVarBlack = pGameIter->pVarBlack;
							//if( pMoveInsert->pVarBlack )
							//	pMoveInsert->variation_count_black++;

							Move* pIter = pMoveInsert->pUp->pVarBlack;
							while( pIter )
							{
								pIter->pUp = pMoveInsert->pUp;
								pIter->pPrev = pMoveInsert->pUp;
								pIter = pIter->pVarBlack;
							}

							pGameIter->pNext = NULL;
							pGameIter->pVarWhite = NULL;
							pGameIter->pVarBlack = NULL;
							return true;
						}
						
						if( pGameIter->pVarWhite )
						{
							Move* pMoveInsert = tempIter->getWhiteVariationForParse();
							pMoveInsert->CopyNode(pGameIter->pVarWhite);

							pMoveInsert->pNext = pGameIter->pVarWhite->pNext;
							if( pGameIter->pVarWhite->pNext )
								pGameIter->pVarWhite->pNext->pPrev = pMoveInsert;

							pMoveInsert->pVarWhite = pGameIter->pVarWhite->pVarWhite;
							//if( pMoveInsert->pVarWhite )
							//	pMoveInsert->variation_count_white++;
							pMoveInsert->pVarBlack = pGameIter->pVarWhite->pVarBlack;
							//if( pMoveInsert->pVarBlack )
							//	pMoveInsert->variation_count_black++;

							Move* pMoveInsertVarIter = pMoveInsert;
							while( pMoveInsertVarIter )
							{
								pMoveInsertVarIter->pUp = tempIter;
								pMoveInsertVarIter->pPrev = tempIter;
								pMoveInsertVarIter = pMoveInsertVarIter->pVarWhite;
							}

							//pGameIter->pVarWhite = NULL;
							pGameIter->pVarWhite->pNext = NULL;
							pGameIter->pVarWhite->pVarWhite = NULL;
							pGameIter->pVarWhite->pVarBlack = NULL;
							//return true;
						}
						
						if( pGameIter->pVarBlack )
						{
							Move* pMoveInsert = tempIter->getBlackVariationForParse();
							pMoveInsert->CopyNode(pGameIter->pVarBlack);

							pMoveInsert->pNext = pGameIter->pVarBlack->pNext;
							if( pGameIter->pVarBlack->pNext )
								pGameIter->pVarBlack->pNext->pPrev = pMoveInsert;

							//pMoveInsert->pVarWhite = pGameIter->pVarWhite->pVarWhite;
							//if( pMoveInsert->pVarWhite )
							//	pMoveInsert->variation_count_white++;
							pMoveInsert->pVarBlack = pGameIter->pVarBlack->pVarBlack;
							//if( pMoveInsert->pVarBlack )
							//	pMoveInsert->variation_count_black++;

							Move* pMoveInsertVarIter = pMoveInsert;
							while( pMoveInsertVarIter )
							{
								pMoveInsertVarIter->pUp = tempIter;
								pMoveInsertVarIter->pPrev = tempIter;
								pMoveInsertVarIter = pMoveInsertVarIter->pVarBlack;
							}

							pGameIter->pVarBlack = NULL;
							//pGameIter->pVarWhite->pNext = NULL;
							//pGameIter->pVarWhite->pVarWhite = NULL;
							//pGameIter->pVarWhite->pVarBlack = NULL;
							//return true;
						}
						pGameIter = pGameIter->pNext;
						tempIter = tempIter->pNext;
					}

					return false;
				}

			}
			else
			{
				Move* pMoveInsert = pRoot->getBlackVariationForParse();
				pMoveInsert->CopyNode(pGame->pRoot);
					
				pMoveInsert->pNext = pGame->pRoot->pNext;
				pGame->pRoot->pNext->pPrev = pMoveInsert;
					
				pMoveInsert->white = "";

				pMoveInsert->pVarWhite = pGame->pRoot->pVarWhite;
				//if( pMoveInsert->pVarWhite )
				//	pMoveInsert->variation_count_white++;
				pMoveInsert->pVarBlack = pGame->pRoot->pVarBlack;
				//if( pMoveInsert->pVarBlack )
				//	pMoveInsert->variation_count_black++;

				pGame->pRoot->pNext = NULL;
				pGame->pRoot->pVarWhite = NULL;
				pGame->pRoot->pVarBlack = NULL;

				return true;
			}
		}
		else
		{
			// find the node whose info is equal 
			Move* tempIter = pRoot;
			while( tempIter )
			{
				if( tempIter->info == pGame->pRoot->info )
				{
					if( pGame->pRoot->white != "" )
					{
						//if( pGame->pBase->m_fenBlack == tempIter->pPrev->m_fenBlack )
						{
							Move* pMoveInsert = tempIter->getWhiteVariationForParse();
							pMoveInsert->CopyNode(pGame->pRoot);
							pMoveInsert->pNext = pGame->pRoot->pNext;
							pGame->pRoot->pNext->pPrev = pMoveInsert;
							pMoveInsert->pVarWhite = pGame->pRoot->pVarWhite;
							//if( pMoveInsert->pVarWhite )
							//	pMoveInsert->variation_count_white++;
							pMoveInsert->pVarBlack = pGame->pRoot->pVarBlack;
							//if( pMoveInsert->pVarBlack )
							//	pMoveInsert->variation_count_black++;

							pGame->pRoot->pNext = NULL;
							pGame->pRoot->pVarWhite = NULL;
							pGame->pRoot->pVarBlack = NULL;
							return true;
						}
						//else return false;
					}
					else
					{
						//if( pGame->pBase->m_fenBlack == tempIter->m_fenWhite )
						{
							Move* pMoveInsert = tempIter->getBlackVariationForParse();
							pMoveInsert->CopyNode(pGame->pRoot);
							pMoveInsert->pNext = pGame->pRoot->pNext;
							if( pGame->pRoot->pNext )
								pGame->pRoot->pNext->pPrev = pMoveInsert;
							
							pMoveInsert->pVarWhite = pGame->pRoot->pVarWhite;
							//if( pMoveInsert->pVarWhite )
							//	pMoveInsert->variation_count_white++;
							pMoveInsert->pVarBlack = pGame->pRoot->pVarBlack;
							//if( pMoveInsert->pVarBlack )
							//	pMoveInsert->variation_count_black++;

							Move* pMoveInsertVarIter = pMoveInsert;
							while( pMoveInsertVarIter )
							{
								pMoveInsertVarIter->pUp = tempIter;
								pMoveInsertVarIter->pPrev = tempIter;
								pMoveInsertVarIter = pMoveInsertVarIter->pVarBlack;
							}

							pGame->pRoot->pNext = NULL;
							pGame->pRoot->pVarWhite = NULL;
							pGame->pRoot->pVarBlack = NULL;
							return true;
						}
						//else return false;
					}
				}
				tempIter = tempIter->pNext;
			}
		}
		return false;
		*/
	}

	int GetMoveCount()
	{
		int count = 0;
		return pRoot->GetMoveCount(count);
	}

	void EnumerateVariations()
	{
	//	pRoot->EnumerateVariations();
	}


	int CountMoves()
	{
		int numMoves = 0;
		return pBase->Count(numMoves);
	}

	// current working stable
	bool IsInBase()
	{
		return (this->pCurrMove == this->pBase);
	}

	// deprectaed
	Move* GetSelectedMove(int sel)
	{
		if( sel >= move_counter )
			return NULL;

		Move* movOut = pRoot;
		for( int count = 0; (count < sel); count++)
		{
			movOut = movOut->pNext;
		}

		return movOut;
	}


	// deprectaed
	Move* GetNextMove()
	{
		Move* movOut = pCurrMove;
		

		if( white_black == 0 )
		{
			white_black = 1;
			return movOut;
			
		}
		else
		{
			white_black = 0;
			if( pCurrMove->pNext != NULL )
				pCurrMove = pCurrMove->pNext;
			return movOut;

		}
	}

	// deprectaed
	Move* GetPreviousMove()
	{
		Move* movOut = pCurrMove;
		
		if( movOut->pPrev == NULL )
		{
			//while(movOut->pNext)
			//	movOut = movOut->pNext;
	
			white_black = end_white_black;
		}

		
		if( white_black == 0 )
		{
			white_black = 1;
			if( pCurrMove->pPrev!= NULL )
				pCurrMove = pCurrMove->pPrev;
			return movOut;
			
		}
		else
		{
			white_black = 0;
			
			return movOut;

		}
	}

	Move* GetCurrentMove()
	{
		return pCurrMove;
	}

	//current working stable
	string CurrFenBlack()
	{
		return pCurrMove->getFenBlack();
	}

	//current working stable
	string CurrFenWhite()
	{
		return pCurrMove->getFenWhite();
	}

	//current working stable
	Move* NextMove()
	{
		if( variation_auto_fold )
		{
			pCurrMove->SetDisplayVariations(false);
		}

		if( pCurrMove->pNext != NULL )
		{
			pCurrMove = pCurrMove->pNext;
		}

		if( variation_auto_fold )
		{
			pCurrMove->SetDisplayVariations(true);
		}
		return pCurrMove;
	}
	// current working stable
	Move* PrevMove()
	{

		if( variation_auto_fold )
		{
			pCurrMove->SetDisplayVariations(false);
		}

		if( pCurrMove->pPrev != NULL )
		{
			pCurrMove = pCurrMove->pPrev;
		}

		if( variation_auto_fold )
		{
			pCurrMove->SetDisplayVariations(true);
		}

		return pCurrMove;
	}

	// deprecated for now
	void AddNode(MyChessMove *mv, int black_white, int movNum, string fen="", string pgn_move="")
	{
		/*if( black_white == WHITE )
		{
			if( pCurrMove->whiteMove == *mv )
			{
				return;
			}
			else
			{
				Move* pMove = new Move();
				pMove->whiteMove = *mv;
				if( fen != "" )
					pMove->m_fenWhite = fen;
				if( pgn_move != "" )
					pMove->white = pgn_move;

				//if( movNum > 0 )
				//	pMove->info = tostring(movNum);

				pCurrMove->pVariation[ pCurrMove->variation_count++ ] = pMove;// = new Move();
				
			}
		}
		else
		{
			if( pCurrMove->blackMove == *mv )
			{
				return;
			}
			else
			{
				Move* pMove = new Move();
				pMove->blackMove = *mv;
				if( fen != "" )
					pMove->m_fenWhite = fen;
				if( pgn_move != "" )
					pMove->white = pgn_move;

				//if( movNum > 0 )
				//	pMove->info = tostring(movNum);

				pCurrMove->pVariation[ pCurrMove->variation_count++ ] = pMove;// = new Move();
				
			}
		}*/
	}


	bool AtEnd()
	{
		return (pCurrMove->MoveFlagIsSet(MOVE_FLAG_AT_END_WHITE) || pCurrMove->MoveFlagIsSet(MOVE_FLAG_AT_END_BLACK));
	}


	// deprecated
	bool AtMoveBeforeEnd()
	{
		return (pCurrMove->pNext != NULL);
	}

	//deprecated
	string GetCurrentMoveInfo()
	{
		return pCurrMove->info; 
	}


	
	//Move* Prev	




	void ResetMoveList()
	{
		pCurrMove = pBase;
		selectedMove = 0;
		white_black = 0;
	}

	string parse_error;

	string& GetParseError()
	{
		return parse_error;
	}

	int Parse()
	{
		Appender v;
		vec_moves = new vector<string>;
										
		tokenize2(v, this->game_text.c_str(), " \r\n\n\t{}()");
		/*for( int i = 0; i < v.size(); i++ )
		{
			vec_moves->push_back(v[i]);
			//cout<<v[i]<<endl;
		}*/
		vec_moves = &v;
		Move *pMove = pRoot;
		parse_error = "";
		for(  vec_s_iter = vec_moves->begin(); vec_s_iter!= vec_moves->end(); vec_s_iter++)
		{
			// ERROR CHECKER, DETECT ILLEGAL TOKENS BY EXCLUDING BAD CHARS e.g."\|/'tywlo^£"
			// PARSE THE $ FOR GLYPHS, ALSO (DETECT CHESSBASE META DATA)?
			int dollar = -1;
			dollar = vec_s_iter->find("$");
			if( dollar != -1 )
				continue;
			int open_curly_brace = vec_s_iter->find("{");
			if( open_curly_brace == 0 )
			{
				if( /*pMove->x == 0*/ !pMove->MoveFlagIsSet( MOVE_FLAG_X ) && pMove->pPrev!= NULL)
				{
					vec_s_iter = HandleComment(vec_s_iter, pMove->pPrev, 1);
					if( vec_s_iter == vec_moves->end() )
						break;
				}
				else
				{
					vec_s_iter = HandleComment(vec_s_iter, pMove, 0 );
					if( vec_s_iter == vec_moves->end() )
						break;
				}
			}
			else 
			{
				int open_bracket = vec_s_iter->find("(");
				if( open_bracket == 0)
				{
					// special case of open bracket followed by new line
					if( open_bracket+1 == vec_s_iter->length() )
					{
						if( /*pMove->x == 0*/ !pMove->MoveFlagIsSet( MOVE_FLAG_X ) && pMove->pPrev != NULL )
						{
							vec_s_iter = HandleOpenBracket2(++vec_s_iter, pMove->pPrev->getBlackVariationForParse(), true);
						}
						else
						{
							if( /*pMove->x == 0*/ !pMove->MoveFlagIsSet( MOVE_FLAG_X ) )
								vec_s_iter = HandleOpenBracket2(++vec_s_iter, pMove->getBlackVariationForParse(), true);
							else 
								vec_s_iter = HandleOpenBracket2(++vec_s_iter, pMove->getWhiteVariationForParse(), true);
						}
					}
					else{
						if( /*pMove->x == 0*/!pMove->MoveFlagIsSet( MOVE_FLAG_X ) && pMove->pPrev != NULL )
						{
							vec_s_iter = HandleOpenBracket2(vec_s_iter, pMove->pPrev->getBlackVariationForParse(), false);	
						}
						else
						{
							if( /*pMove->x == 0*/!pMove->MoveFlagIsSet( MOVE_FLAG_X ) )
								vec_s_iter = HandleOpenBracket2(vec_s_iter, pMove->getBlackVariationForParse(), false);	
							else
								vec_s_iter = HandleOpenBracket2(vec_s_iter, pMove->getWhiteVariationForParse(), false);	
					
						}
					}
					
					if( parse_error != "" )
					{
						/*vec_moves->clear();*/ /*delete vec_moves;*/ vec_moves=NULL;;
						return 0;
					}
				}
				else 
				{
					size_t comp = vec_s_iter->compare("1-0");
					size_t comp2 = vec_s_iter->compare("0-1");
					size_t comp3 = vec_s_iter->compare("1/2-1/2");
					size_t comp4 = vec_s_iter->compare("*");
					bool the_end = false;
					if( comp == 0 || comp2 == 0 || comp3 == 0 || comp4 == 0)
					{
							cout<< "result == 0" <<endl;
							the_end = true;
							
							if( /*pMove->x == 1*/pMove->MoveFlagIsSet( MOVE_FLAG_X ) )
							{
								//int ret =
									pMove->result = *vec_s_iter;//SetMove(*vec_s_iter, BLACK);
								//pMove->black = *vec_s_iter;
							/*	if( ret != PASSED_TEST )
								{
									// error
								}*/
								end_white_black = 1;

								//pMove->atEndWhite = true;
								pMove->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );

								///*vec_moves->clear();*/ 
								/*delete vec_moves;*/ vec_moves=NULL;;
								return 1;
							}
							else
							{

								Move *mv;
								mv = pMove;

								mv->result = *vec_s_iter;

								if( (pMove->white == "") && (pMove->black == "") )
								{
									pMove->pPrev->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
								//	pMove->pPrev->atEndBlack = true;
								}
								else 
								{
									pMove->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
								//	pMove->atEndBlack = true;
								}

								move_counter++;

								end_white_black = 0;

								///*vec_moves->clear();*/ 
								/*delete vec_moves;*/ vec_moves=NULL;;
								return 1;
							}
							
								//move_counter;
							// white has no move on triple dots

					}
					
					int full_stop = -1;
					full_stop = vec_s_iter->find(".");

					int triple_dots = -1;
					triple_dots = vec_s_iter->find("...");

					int close_smooth = -1;
					close_smooth = vec_s_iter->find(")");

					if( close_smooth >= 0 ) // ?? WHY THE EMPTY IF ??? SKIPS THE REST
					{
					}
					else if( full_stop > 0 && !(triple_dots>0) )
					{
						if( full_stop != vec_s_iter->length()-1 )
						{
							Move *mv;
							mv = pMove; 

							// ERROR CHECK NUMERICAL FORMAT AND NUMBER CORRECTNESS
							// ERROR CHECK MOVE STRING IS CORRECT FORMAT
							if( pMove->white != "" )
							{
								parse_error = "Error, move number error ";

								/*vec_moves->clear();*/ /*delete vec_moves;*/ vec_moves=NULL;;
								return 0;
							}

							CreateWithNoSpaceAfterFullstop2(vec_s_iter, full_stop, &mv);

							mainlinePlyCount++;
							//moves.push_back(mv);
							//move_counter++;
							//move_iter = moves.begin() + moves.size()-1;//move_counter;
							//pMove->x = 1; // white has no move on triple dots
							pMove->SetMoveFlag( MOVE_FLAG_X );
						}
						else
						{
							Move *mv;
							mv = pMove; 

							// ERROR CHECK NUMERICAL FORMAT AND NUMBER CORRECTNESS
							//mv->info = vec_s_iter->substr(0, full_stop) ;

							if( mv->SetInfo( vec_s_iter->substr(0, full_stop) ) != PASSED_TEST )
							{
								parse_error = "Error, move number error ";

								/*vec_moves->clear();*/ /*delete vec_moves; */vec_moves=NULL;;
								return 0;
							}

						}
					}
					else if(triple_dots>0)
					{
						if( /*pMove->x == 0*/ !pMove->MoveFlagIsSet( MOVE_FLAG_X ) )
						{
							if( /*pMove->pPrev->y== 0*/ !pMove->pPrev->MoveFlagIsSet( MOVE_FLAG_Y ) )
							{
								if( full_stop != vec_s_iter->length()-3 )
								{
									//pMove->x=1;
									//pMove->y = 1;
									pMove->SetMoveFlag( MOVE_FLAG_X );
									pMove->SetMoveFlag( MOVE_FLAG_Y );
									//pMove->info = vec_s_iter->substr(0, full_stop);

									if( pMove->SetInfo( vec_s_iter->substr(0, full_stop) ) != PASSED_TEST )
									{
										parse_error = "Error, move number error ";

										/*vec_moves->clear();*/ /*delete vec_moves;*/ vec_moves=NULL;;
										return 0;
									}
									//pMove->black = vec_s_iter->substr(full_stop+3, vec_s_iter->length());
									int ret = pMove->SetMove(vec_s_iter->substr(full_stop+3, vec_s_iter->length()), BLACK);
								//pMove->black = *vec_s_iter;
									if( ret != PASSED_TEST )
									{
										// error
										parse_error = "Error at move: " + pMove->info + "..." + *vec_s_iter;
										
										/*vec_moves->clear();*/ /*delete vec_moves;*/ vec_moves=NULL;;
										return 0;
									}
									pMove->pNext = new Move();
									Move* mv = pMove;
									pMove = pMove->pNext;
									pMove->pPrev = mv;

									mainlinePlyCount++;

									move_counter++;
								}
								else
								{
									//pMove->info = vec_s_iter->substr(0, full_stop);
									if( pMove->SetInfo( vec_s_iter->substr(0, full_stop) ) != PASSED_TEST )
									{
										parse_error = "Error, move number error ";
										
										/*vec_moves->clear();*/ /*delete vec_moves;*/ vec_moves=NULL;;
										return 0;
									}
									//pMove->x = 1;
									pMove->SetMoveFlag( MOVE_FLAG_X );
								}
							}
							else
							{
								//pMove->x = 1;
								pMove->SetMoveFlag( MOVE_FLAG_X );
								//pMove->white = *vec_s_iter; // possible bug
								// the input string is of the form 18... or 18...Bc5
								int ret = pMove->SetMove(*vec_s_iter, WHITE);
								//pMove->black = *vec_s_iter;
								if( ret != PASSED_TEST )
								{
									parse_error = "Error at move: " + *vec_s_iter;
										// error

									/*vec_moves->clear();*/ /*delete vec_moves;*/ vec_moves=NULL;;
									return 0;
								}

								mainlinePlyCount++;
							}
						}
						else
						{
							
							/*for( int i = triple_dots, j = 0; i < vec_s_iter->length()-3; j++, i++)
							{
								move_iter->black[j] = (*vec_s_iter)[i+3];
							}*/
							// debug strim triple dots
							if( full_stop != vec_s_iter->length()-3 )
							{
								//pMove->y = 1;
								pMove->SetMoveFlag( MOVE_FLAG_Y );
								//pMove->black 
									string toset= vec_s_iter->substr(full_stop+3, vec_s_iter->length());
								int ret = pMove->SetMove(toset, BLACK);
								//pMove->black = *vec_s_iter;
								if( ret != PASSED_TEST )
								{
									parse_error = "Error at move: " + pMove->info + "..." + toset;
										// error
									/*vec_moves->clear();*/ /*delete vec_moves;*/ vec_moves=NULL;;
									return 0;
								}
								pMove->pNext = new Move();
								Move* mv = pMove;
								pMove = pMove->pNext;
								pMove->pPrev = mv;

								move_counter++;

								mainlinePlyCount++;
							}
							else
							{
							//	pMove->info = 
							}
						}
					}
					else if( vec_s_iter->length() != 1 )
					{
						if( /*pMove->x == 0*/!pMove->MoveFlagIsSet( MOVE_FLAG_X ) )
						{
							// ERROR CHECK, MOVE HAS A NUMBER
							//pMove->x = 1; 
							pMove->SetMoveFlag( MOVE_FLAG_X );
							//pMove->white = *vec_s_iter;
							int ret = pMove->SetMove(*vec_s_iter, WHITE);
							//pMove->black = *vec_s_iter;
							if( ret != PASSED_TEST )
							{
								parse_error = "Error at move: " + pMove->info + "." + *vec_s_iter;
								
								/*vec_moves->clear();*/ /*delete vec_moves;*/ vec_moves=NULL;;
								return 0;
							}

							mainlinePlyCount++;
						}
						else
						{
							// ERROR CHECK, SET MOVE STRING IS VALID TYPE
							//pMove->y = 1;
							pMove->SetMoveFlag( MOVE_FLAG_Y );
							//pMove->black = *vec_s_iter;
							int ret = pMove->SetMove(*vec_s_iter, BLACK);
							//pMove->black = *vec_s_iter;
							if( ret != PASSED_TEST )
							{
								parse_error = "Error at move: " + pMove->info + "..." + *vec_s_iter;
								
								/*vec_moves->clear();*/ /*delete vec_moves;*/ vec_moves=NULL;;
								return 0;

							}							
							pMove->pNext = new Move();
							Move* mv = pMove;
							pMove = pMove->pNext;
							pMove->pPrev = mv;

							// debug: gone to next move too early
							// the variation is of the next input for this move sometimes
							move_counter++;

							mainlinePlyCount++;
						}
					}

					/*if( the_end == true )
					{
						if( move_iter->x == 0 )
						{
							move_iter->x = 1; 
							move_iter->white = *vec_s_iter;
						}
						else
						{
							move_iter->y = 1;
							move_iter->black = *vec_s_iter;
							cout<<move_iter->info<<" "<<
								  move_iter->white <<" "<<
								  move_iter->black<<" "<<endl;
						}
						return;
					}*/
				}
			}
		}

		// exit for loop
		if( pMove->white == "1-0" || 
			pMove->white == "0-1" || 
			pMove->white == "1/2-1/2" ||
			pMove->white == "*" )
		{
			//pMove->pPrev->atEndBlack = true;
			pMove->pPrev->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
		}
		else if( pMove->black == "1-0" || 
			pMove->black == "0-1" || 
			pMove->black == "1/2-1/2" ||
			pMove->black == "*" )
		{
			//pMove->atEndWhite = true;
			pMove->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
		}
		else if( pMove->black == "" )
		{
			if( pMove->white == "" )
			{
				if( pMove->pPrev && /*pMove->pPrev->y == 0*/ !pMove->pPrev->MoveFlagIsSet(MOVE_FLAG_Y) )
				{
					if( /*pMove->pPrev->x == 0*/!pMove->pPrev->MoveFlagIsSet(MOVE_FLAG_X)  || 
						pMove->pPrev->white == "1-0" || 
						pMove->pPrev->white == "0-1" || 
						pMove->pPrev->white == "1/2-1/2" ||
						pMove->pPrev->white == "*" )
					{
						if( pMove->pPrev->pPrev )
						{
							//pMove->pPrev->pPrev->atEndBlack = true;
							pMove->pPrev->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						}
					}
					else
					{
						//pMove->pPrev->atEndWhite = true;
						pMove->pPrev->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
					}
				}
				else if( pMove->pPrev )
				{
					pMove->pPrev->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					//	pMove->pPrev->atEndBlack = true;
				}
			}
			else 
			{
				pMove->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
				//pMove->atEndWhite = true;
			}	
			
		}
		else if( pMove->white == "" )
		{
			if( pMove->pPrev &&
				pMove->pPrev->black == "1-0" || 
				pMove->pPrev->black == "0-1" || 
				pMove->pPrev->black == "1/2-1/2" ||
				pMove->pPrev->black == "*" )
			{
				//pMove->pPrev->atEndWhite = true;
				pMove->pPrev->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
			}
			else if( pMove->white == "1-0" || 
				pMove->white == "0-1" || 
				pMove->white == "1/2-1/2" ||
				pMove->white == "*" )
			{
				if( pMove->pPrev && pMove->pPrev->pPrev )
				{
					//pMove->pPrev->pPrev->atEndBlack = true;
					pMove->pPrev->pPrev->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
				}
			}
		}
		/*vec_moves->clear();*/ /*delete vec_moves;*/ vec_moves=NULL;;
		return 1;

		
	}

	vector<string>::iterator 
		InsertWhiteTextBeforeMove(vector<string>::iterator vinpt,
					   Move* pMove, bool white_black)
	{
		// EDITION : COMMENTS MUST CONFORM TO TEXT BEFORE MOVE 
		//cout<< "curly_brace_open at ...";
		for(; vinpt != vec_moves->end(); vinpt++) 
		{
			int close_curly_brace = -1;
			close_curly_brace = vinpt->find("}");
			if( close_curly_brace != -1 )
			{
				//cout<<*vinpt<<"curly brace closes at ... "<<endl;
				if( white_black == 0 )
					pMove->annotation_white.text.push_back(*vinpt);
				else
					pMove->annotation_black.text.push_back(*vinpt);
				//pMove->annotation.text += *vinpt;

				//if( pMove->x != 0 && pMove->y == 0)
				//	pMove->annotation.after_whites_move = true;
				
//				cout<<" "<<pMove->annotation.text;
				
				break;
			}
			else {
				if( /*pMove->y == 0*/ !pMove->MoveFlagIsSet( MOVE_FLAG_Y ) )
					pMove->annotation_white.text.push_back(*vinpt + " ");
				else 
					pMove->annotation_black.text.push_back(*vinpt + " ");
				//pMove->annotation.text += *vinpt + " ";
				
			}
		}
		if( vinpt == vec_moves->end() ) // ERROR MAKE SURE THE CLOSE BRACKET IS DETECTED
			return vinpt;
		else return vinpt++;
		
	}

	vector<string>::iterator 
		InsertBlackTextBeforeMove(vector<string>::iterator vinpt,
					   Move* pMove, bool white_black)
	{
		// EDITION : COMMENTS MUST CONFORM TO TEXT BEFORE MOVE 
		//cout<< "curly_brace_open at ...";
		for(; vinpt != vec_moves->end(); vinpt++) 
		{
			int close_curly_brace = -1;
			close_curly_brace = vinpt->find("}");
			if( close_curly_brace != -1 )
			{
				//cout<<*vinpt<<"curly brace closes at ... "<<endl;
				if( white_black == 0 )
					pMove->annotation_white.text.push_back(*vinpt);
				else
					pMove->annotation_black.text.push_back(*vinpt);
				//pMove->annotation.text += *vinpt;

				//if( pMove->x != 0 && pMove->y == 0)
				//	pMove->annotation.after_whites_move = true;
				
//				cout<<" "<<pMove->annotation.text;
				
				break;
			}
			else {
				if( /*pMove->y == 0*/!pMove->MoveFlagIsSet( MOVE_FLAG_Y ) )
					pMove->annotation_white.text.push_back(*vinpt + " ");
				else 
					pMove->annotation_black.text.push_back(*vinpt + " ");
				//pMove->annotation.text += *vinpt + " ";
				
			}
		}
		if( vinpt == vec_moves->end() ) // ERROR MAKE SURE THE CLOSE BRACKET IS DETECTED
			return vinpt;
		else return vinpt++;
		
	}


	vector<string>::iterator 
		HandleComment(vector<string>::iterator vinpt,
					   Move* pMove, bool white_black)
	{
		// EDITION : COMMENTS MUST CONFORM TO TEXT BEFORE MOVE 
		//cout<< "curly_brace_open at ...";
		for(; vinpt != vec_moves->end(); vinpt++) 
		{
			int close_curly_brace = -1;
			close_curly_brace = vinpt->find("}");
			if( close_curly_brace != -1 )
			{
				//cout<<*vinpt<<"curly brace closes at ... "<<endl;
				if( white_black == 0 )
					pMove->annotation_white.text.push_back(*vinpt);
				else
					pMove->annotation_black.text.push_back(*vinpt);
				//pMove->annotation.text += *vinpt;

				//if( pMove->x != 0 && pMove->y == 0)
				//	pMove->annotation.after_whites_move = true;
				
//				cout<<" "<<pMove->annotation.text;
				
				break;
			}
			else {
				if( /*pMove->y == 0*/!pMove->MoveFlagIsSet( MOVE_FLAG_Y ) )
					pMove->annotation_white.text.push_back(*vinpt + " ");
				else 
					pMove->annotation_black.text.push_back(*vinpt + " ");
				//pMove->annotation.text += *vinpt + " ";
				
			}
		}
		if( vinpt == vec_moves->end() ) // ERROR MAKE SURE THE CLOSE BRACKET IS DETECTED
			return vinpt;
		else return vinpt++;
		
	}



vector<string>::iterator 
		HandleOpenBracket2(vector<string>::iterator vinpt,
						  Move* pmove, bool newline)
	{
		//cout<< "process current open bracket "<< *vinpt <<endl;
		// ERROR ... EXCLUDE BAD CHARS

		int curly_brace_1 = -1;
		curly_brace_1 = vinpt->find("{");

		if( curly_brace_1 != -1 )
		{
			vinpt = HandleComment(vinpt, pmove, 0);	
		}

		int close_curly_brace_1 = -1;
		close_curly_brace_1 = vinpt->find("}");

		if( close_curly_brace_1 != -1 )
		{
			vinpt++;	
		}

		int full_stop = -1;
		full_stop = vinpt->find(".");

		int trip_dot_1 = -1;
		trip_dot_1 = vinpt->find("...");
		
		int length = vinpt->length();
		
		if( (full_stop > 0) && !(trip_dot_1 > 0))
		{
			Move *newmove = pmove;
			//pMove = new Move();
			
			int open_bracket = -1;
			open_bracket = vinpt->find("(");
			if( newline )
			{
				if( open_bracket != -1 )
				{
					// ERROR ... IS FULLSTOP IN THE RIGHT PLACE PRECEDing A NUMBER
					// IS THE NUMBER THE CORRECT ONE?
					//newmove->info = vinpt->substr(open_bracket+1,full_stop-1);
					if( newmove->SetInfo( vinpt->substr(open_bracket+1,full_stop-1)) != PASSED_TEST )
					{
						parse_error = "Error, move number error in variation";
						return vinpt;
					}

					if( (full_stop != vinpt->length()-1) )
					{
						//newmove->white 
							string toset= vinpt->substr(full_stop+1, vinpt->length());
						int ret = newmove->SetMove(toset, WHITE);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
							parse_error = "Error in variation: " + newmove->info + "." + toset;
							return vinpt;
										// error
						}
						//newmove->x =1;
						newmove->SetMoveFlag( MOVE_FLAG_X );
					}
					// ERROR WHAT IF THIS IS SKIPPED?
				}
				else
				{
					if( (full_stop != vinpt->length()-1) )
					{
						//newmove->info = vinpt->substr(0,full_stop);
						if( newmove->SetInfo( vinpt->substr(0,full_stop)) != PASSED_TEST )
						{
							parse_error = "Error, move number error in variation";
							return vinpt;
						}
						//newmove->white 
							string toset= vinpt->substr(full_stop+1, vinpt->length());
						int ret = newmove->SetMove(toset, WHITE);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
							parse_error = "Error in variation: " + newmove->info + "." + toset;
							return vinpt;
										// error
						}
					//	newmove->x =1;
						newmove->SetMoveFlag( MOVE_FLAG_X );
					}
					else
					{
						if( newmove->SetInfo( vinpt->substr(0,full_stop)) != PASSED_TEST )
						{
							parse_error = "Error, move number error in variation";
							return vinpt;
						}
						//newmove->info = vinpt->substr(0,full_stop);
						//if( newmove->info[ne wmove->info.length()-1] == '.')
						//	newmove->info[vinpt->length()-1] = 0;
					}
					// create with no space after full stop?
				}
				//pMove->info="(" + *vinpt;
			}
			else
			{
				if( open_bracket != -1 )
				{
					//newmove->info = vinpt->substr(open_bracket+1,full_stop-1);
					if( newmove->SetInfo( vinpt->substr(open_bracket+1,full_stop-1)) != PASSED_TEST )
					{
						parse_error = "Error, move number error in variation ";
						return vinpt;
					}
					if( (full_stop != vinpt->length()-1) )
					{
						//newmove->white 
							string toset= vinpt->substr(full_stop+1, vinpt->length());
						int ret = newmove->SetMove(toset, WHITE);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
							parse_error = "Error in variation: " + newmove->info + "." + toset;
							return vinpt;
										// error
						}
						//newmove->x =1;
						newmove->SetMoveFlag( MOVE_FLAG_X );
					}
				}
				else
				{
					if( (full_stop != vinpt->length()-1) )
					{
						if( newmove->SetInfo( vinpt->substr(0,full_stop)) != PASSED_TEST )
						{
							parse_error = "Error, move number error in variation ";
							return vinpt;
						}
						//newmove->info = vinpt->substr(0,full_stop);
						//newmove->white 
							string toset= vinpt->substr(full_stop+1, vinpt->length());
						int ret = newmove->SetMove(toset, WHITE);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
							parse_error = "Error in variation: " + newmove->info + "." + toset;
							return vinpt;
										// error
						}
						//newmove->x =1;
						newmove->SetMoveFlag( MOVE_FLAG_X );
					}
					else
					{
						//newmove->info = vinpt->substr(0,full_stop);
						if( newmove->SetInfo( vinpt->substr(0,full_stop)) != PASSED_TEST )
						{
							parse_error = "Error, move number error in variation ";
							return vinpt;
						}
						//if( newmove->info[newmove->info.length()-1] == '.')
						//	newmove->info[vinpt->length()-1] = 0;
					}
				}
				//pMove->info = *vinpt;
			}
			
			//newmove.info = *vinpt;
			//pMove->x = 0;
		//	newmove->x = 1;
			//vmoves->vecMoves.push_back(newmove);
		}
		else if( trip_dot_1 != -1 )
		{
			Move *newmove = pmove;// = new Move();
			int open_bracket = -1;
			open_bracket = vinpt->find("(");
			if( newline )
			{

				if( (full_stop+3 != length) )
				{
					if( open_bracket != -1 )
					{
						string tempstr = vinpt->substr(1, full_stop-1);
						//pmove->info = tempstr;
						if( pmove->SetInfo( tempstr) != PASSED_TEST )
						{
							parse_error = "Error, move number error in variation ";
							return vinpt;
						}
						if( pmove->info[pmove->info.length() -1] == '.' )
							pmove->info[pmove->info.length() -1] = 0;
						string tempstr2 = vinpt->substr(full_stop+3, vinpt->length());
						//pmove->black = tempstr2;
						int ret = newmove->SetMove(tempstr2, BLACK);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
							parse_error = "Error in variation: " + newmove->info + "..." + tempstr2;
							return vinpt;
										// error
						}
						pmove->SetMoveFlag( MOVE_FLAG_X );
						newmove->SetMoveFlag( MOVE_FLAG_Y );
						//pmove->x =1;

						//newmove->y = 1;
						newmove->pNext = new Move;
						pmove = newmove->pNext;
						pmove->pPrev = newmove;
					}
					else
					{
						string tempstr = vinpt->substr(0, full_stop);
						//pmove->info = tempstr;
						if( pmove->SetInfo( tempstr) != PASSED_TEST )
						{
							parse_error = "Error, move number error in variation ";
							return vinpt;
						}
						if( pmove->info[pmove->info.length() -1] == '.' )
							pmove->info[pmove->info.length() -1] = 0;
						
						string tempstr2 = vinpt->substr(full_stop+3, vinpt->length());
						int ret = newmove->SetMove(tempstr2, BLACK);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
										// error
							parse_error = "Error in variation: " + newmove->info + "..." + tempstr2;
							return vinpt;
						}
						//pmove->x =1;
						//newmove->y = 1;
						pmove->SetMoveFlag( MOVE_FLAG_X );
						newmove->SetMoveFlag( MOVE_FLAG_Y );
						newmove->pNext = new Move;
						pmove = newmove->pNext;
						pmove->pPrev = newmove;
					}
				}
				else
				{
					if( open_bracket != -1 )
					{
						string tempstr = vinpt->substr(1, full_stop-1);
						//pmove->info = tempstr;
						if( pmove->SetInfo( tempstr) != PASSED_TEST )
						{
							parse_error = "Error, move number error in variation ";
							return vinpt;
						}
						if( pmove->info[pmove->info.length() -1] == '.' )
							pmove->info[pmove->info.length() -1] = 0;
						//pmove->x =1;
						pmove->SetMoveFlag( MOVE_FLAG_X );
						//newmove->SetMoveFlag( MOVE_FLAG_Y );
					}
					else
					{
						string tempstr = vinpt->substr(0, full_stop);
						//pmove->info = tempstr;
						if( pmove->SetInfo( tempstr) != PASSED_TEST )
						{
							parse_error = "Error, move number error in variation ";
							return vinpt;
						}
						if( pmove->info[pmove->info.length() -1] == '.' )
							pmove->info[pmove->info.length() -1] = 0;
						//pmove->x =1;
						pmove->SetMoveFlag( MOVE_FLAG_X );
						//newmove->SetMoveFlag( MOVE_FLAG_Y );
					}
				}
				//newmove->info = "(" + *vinpt;
				//pMove->info = "(" + *vinpt;
			}
			else
			{
				if( (full_stop+3 != length)) 
				{
					if( open_bracket != -1 )
					{
						string tempstr = vinpt->substr(1, full_stop-1);
						//pmove->info = tempstr;
						if( pmove->SetInfo( tempstr) != PASSED_TEST )
						{
							parse_error = "Error, move number error in variation ";
							return vinpt;
						}
						if( pmove->info[pmove->info.length() -1] == '.' )
							pmove->info[pmove->info.length() -1] = 0;
					//	pmove->x =1;
						//string tempstr = vinpt->substr(0, full_stop);
						//pmove->info = tempstr;
						string tempstr2 = vinpt->substr(full_stop+3, vinpt->length());
						int ret = newmove->SetMove(tempstr2, BLACK);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
							parse_error = "Error in variation: " + newmove->info + "..." + tempstr2;
							return vinpt;
										// error
						}
					//	pmove->x =1;
					//	newmove->y = 1;
						pmove->SetMoveFlag( MOVE_FLAG_X );
						newmove->SetMoveFlag( MOVE_FLAG_Y );

						newmove->pNext = new Move;
						pmove = newmove->pNext;
						pmove->pPrev = newmove;
					}
					else
					{
						string tempstr = vinpt->substr(0, full_stop);
						//pmove->info = tempstr;
						if( pmove->SetInfo( tempstr) != PASSED_TEST )
						{
							parse_error = "Error, move number error in variation ";
							return vinpt;
						}
						if( pmove->info[pmove->info.length() -1] == '.' )
							pmove->info[pmove->info.length() -1] = 0;
						//pmove->x =1;
						//string tempstr = vinpt->substr(0, full_stop);
						//pmove->info = tempstr;
						string tempstr2 = vinpt->substr(full_stop+3, vinpt->length());
						int ret = newmove->SetMove(tempstr2, BLACK);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
							parse_error = "Error in variation: " + newmove->info + "..." + tempstr2;
							return vinpt;
										// error
						}
						//pmove->x =1;
						//newmove->y = 1;

						pmove->SetMoveFlag( MOVE_FLAG_X );
						newmove->SetMoveFlag( MOVE_FLAG_Y );
						
						newmove->pNext = new Move;
						pmove = newmove->pNext;
						pmove->pPrev = newmove;
					}
				}
				else
				{
					if( open_bracket != -1 )
					{
						string tempstr = vinpt->substr(1, full_stop-1);
						//pmove->info = tempstr;
						if( pmove->SetInfo( tempstr) != PASSED_TEST )
						{
							parse_error = "Error, move number error in variation ";
							return vinpt;
						}
						if( pmove->info[pmove->info.length() -1] == '.' )
							pmove->info[pmove->info.length() -1] = 0;
						
						//pmove->x =1;
						pmove->SetMoveFlag( MOVE_FLAG_X );
						//newmove->SetMoveFlag( MOVE_FLAG_Y );
					}
					else
					{
						string tempstr = vinpt->substr(0, full_stop);
					//	pmove->info = tempstr;
						if( pmove->SetInfo( tempstr) != PASSED_TEST )
						{
							parse_error = "Error, move number error in variation ";
							return vinpt;
						}
						//pmove->x =1;
						pmove->SetMoveFlag( MOVE_FLAG_X );
						//newmove->SetMoveFlag( MOVE_FLAG_Y );
					}
				}
			}
			
			//pMove->x = 1;
			//vmoves->vecMoves.push_back(newmove);	
		}

		for(vector<string>::iterator vlocal = vinpt+1; vlocal != vec_moves->end(); vlocal++) 
		{

			if( parse_error != "" )
			{
				return vinpt;
			}
			/*int dollar = -1;
			dollar = vlocal->find("$");
			if( dollar != -1 )
				continue;*/

			int open_curly_brace = vlocal->find("{");
			if( open_curly_brace == 0 )
			{
				if( /*pmove->x == 0*/ !pmove->MoveFlagIsSet( MOVE_FLAG_X ) && pmove->pPrev!=NULL)
				{
					vlocal = HandleComment(vlocal, pmove->pPrev, 1);
				}
				else
				{
					vlocal = HandleComment(vlocal, pmove, 0);	
				}
				//continue;
			}

			int open_bracket = vlocal->find("(");
			if( open_bracket == 0)
			{
				if( open_bracket+1 == vlocal->length() ) 
				{	// rare case of '(' followed by newline
						if( /*pmove->x == 0*/!pmove->MoveFlagIsSet( MOVE_FLAG_X ) && pmove->pPrev != NULL )
						{
							if( (pmove->pPrev->pUp != NULL) && (pmove->pPrev->white == ""))
								vlocal = HandleOpenBracket2(++vlocal, pmove->pPrev->pUp->getBlackVariationForParse(), true);
							else
								vlocal = HandleOpenBracket2(++vlocal, pmove->pPrev->getBlackVariationForParse(), true);
	
						}
						else
						{
							if( /*pmove->x == 0*/!pmove->MoveFlagIsSet( MOVE_FLAG_X )  )
							{
								vlocal = HandleOpenBracket2(++vlocal, pmove->getBlackVariationForParse(), true);
							}
							/*else if( pmove->pPrev != NULL )
							{
							}*/
							else 
							{
								if( pmove->pUp != NULL )
									vlocal = HandleOpenBracket2(++vlocal, pmove->pUp->getWhiteVariationForParse(), true);
								else
									vlocal = HandleOpenBracket2(++vlocal, pmove->getWhiteVariationForParse(), true);
							}
						}
				}
				else
				{
						if( /*pmove->x == 0*/!pmove->MoveFlagIsSet( MOVE_FLAG_X ) && pmove->pPrev != NULL  )
						{
							if( (pmove->pPrev->pUp != NULL) && (pmove->pPrev->white == ""))
								vlocal = HandleOpenBracket2(vlocal, pmove->pPrev->pUp->getBlackVariationForParse(), false);	
							else
								vlocal = HandleOpenBracket2(vlocal, pmove->pPrev->getBlackVariationForParse(), false);	
						}
						else
						{
							if( /*pmove->x == 0*/!pmove->MoveFlagIsSet( MOVE_FLAG_X ) )
							{
								vlocal = HandleOpenBracket2(vlocal, pmove->getBlackVariationForParse(), false);
							}
							else
							{
								if( pmove->pUp != NULL )
									vlocal = HandleOpenBracket2(vlocal, pmove->pUp->getWhiteVariationForParse(), false);	
								else
								{
									vlocal = HandleOpenBracket2(vlocal, pmove->getWhiteVariationForParse(), false);	
								}

							}
					
						}
				}
			}

			int full_stop = -1;
			full_stop = vlocal->find(".");

			int triple_dots = -1;
			triple_dots = vlocal->find("...");

			int close_curly = -1;
			close_curly = vlocal->find("}");

			int close_smooth = -1;
			close_smooth = vlocal->find(")");

			int dollar = -1;
			dollar = vlocal->find("$");


			//vector<Move> *pMoves = &vmoves->vecMoves;
			if( dollar != -1 )
			{
			}
			else if( close_curly>0 && close_smooth >0)
			{
			}
			else if( close_curly == 0 )
			{
			}
			else if( close_smooth == 0 )
			{
				if( pmove->white != "" )
				{
					if( pmove->black != "" )
					{
						//pmove->atEndBlack = true;
						pmove->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					}
					else 
					{
						pmove->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						//pmove->atEndWhite = true;
					}
				}
				else
				{
					// try to set the previous one ...
					// ERROR I CAN ASSUME THIS IS HANDLED BELOW
				}
			}// ERROR ARE THE FULLSTOPS PRECEDED BY NUMERICAL CHARS AND IS THE NUMBER CORRECT?
			else if( full_stop > 0 && !(triple_dots>0) && !(close_curly>0)
				/*&&(!close_curly>0)*/)
			{
				int res = -1;
				res = vlocal->find("}");
				// shamefully enough we need to check that we aren't in 
				// a closed bracket node. I can't return ++vinput from the
				// above function to handle comments.

				// is there a space after the full stop?
				if( full_stop != vlocal->length()-1 && res < 0  )
				{
					Move *newmove = pmove;// = 
					// ERROR: CHECK THE MOVE FORMAT, {piece char}{from file|rank} destination {pawns:promotion}{glyphs||checks||mates etc}
					CreateWithNoSpaceAfterFullstop2(vlocal, full_stop, &pmove);

					//newmove->x = 1; // white has no move on triple dots
					newmove->SetMoveFlag( MOVE_FLAG_X );
					//pMoves->push_back(newmove);
				}
				else
				{
					Move *newmove = pmove;
					if( !(res<0) )
					{
						if( newmove->pPrev )
						{
							// ERROR WHY AM I STUFFING IN THE END OF AN ANNOTATION?
							string annotation = *vlocal;
							if( /*pmove->y == 0*/!pmove->MoveFlagIsSet( MOVE_FLAG_Y ) )
								pmove->annotation_white.text.push_back(*vinpt + " ");
							else 
								pmove->annotation_black.text.push_back(*vinpt + " ");
						}
					}
					else
					{
						// ERROR: SET INFO
						//newmove->info = vlocal->substr(0,full_stop);
						if( newmove->SetInfo( vlocal->substr(0,full_stop)) != PASSED_TEST )
						{
							parse_error = "Error, move number error in variation ";
							return vinpt;
						}
						//newmove->info[vlocal->length()-1] = 0;
					}
					//pmove->pNext = new Move();
					//pmove = pmove->pNext;
					//pmove->pPrev = newmove;
					//pMoves->push_back(newmove);
				}
			}
			else if( triple_dots>0 )
			{
				// ERROR: CHECK FULL STOPS ARE IN CORRECT PLACE
				// AND ARE PRECEDED BY NUMERICAL VALUES
				// AND THE NUMBER IS CORRECT
				if( /*pmove->x == 0*/ !pmove->MoveFlagIsSet( MOVE_FLAG_X)  )
				{
					if( full_stop+3 != vlocal->length() /*&& res > 0*/)
					{
						Move *newmove = pmove;
				//pmove->info = *vlocal;
						string tempstr = vlocal->substr(0, full_stop);
						//pmove->info = tempstr;
						if( pmove->SetInfo( tempstr) != PASSED_TEST )
						{
							parse_error = "Error, move number error in variation ";
							return vinpt;
						}
						string tempstr2 = vlocal->substr(full_stop+3, vlocal->length());
						int ret = pmove->SetMove(tempstr2, BLACK);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
							parse_error = "Error in variation: " + pmove->info + "..." + tempstr2;
							return vinpt;
										// error
						}
						//pmove->y =1;
						pmove->SetMoveFlag( MOVE_FLAG_Y );
							
						pmove->pNext = new Move();
						pmove = pmove->pNext;
						pmove->pPrev = newmove;
					}
					else
					{
						string tempstr = vlocal->substr(0, full_stop);
						//pmove->info = tempstr;
						if( pmove->SetInfo( tempstr) != PASSED_TEST )
						{
							parse_error = "Error, move number error in variation ";
							return vinpt;
						}
						//pmove->x = 1;
						pmove->SetMoveFlag( MOVE_FLAG_X );
					}
				}
				else
				{
					int res = -1;
					res = vlocal->find("}");
				// shamefully enough we need to check that we aren't in 
				// a closed bracket node. I can't return ++vinput from the
				// above function to handle comments.

					// is there a space after the full stop?
					if( full_stop+3 != vlocal->length() /*&& res > 0*/)
					{
						Move *newmove = pmove;// = 
						//newmove->black 
							string tempstr2= vlocal->substr(full_stop+3, vlocal->length());
						int ret = pmove->SetMove(tempstr2, BLACK);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
							parse_error = "Error in variation: " + pmove->info + "..." + tempstr2;
							return vinpt;
										// error
						}	
						//newmove->y = 1; // white has no move on triple dots
						newmove->SetMoveFlag( MOVE_FLAG_Y );
						pmove->pNext = new Move();
						pmove = pmove->pNext;
						pmove->pPrev = newmove;
					//pMoves->push_back(newmove);
					}
					else
					{
						//pmove->x = 1;
						pmove->SetMoveFlag( MOVE_FLAG_X );
					}
				}
				//pMoves->push_back(newmove);
			}
			else if( /*pmove->x == 0*/ !pmove->MoveFlagIsSet(MOVE_FLAG_X) && !(close_curly>0) && !(close_smooth==0) && !(vlocal->length()==0))
			{
				//pmove->x = 1;
				pmove->SetMoveFlag( MOVE_FLAG_X );
				//pmove->white = *vlocal;
				int ret = pmove->SetMove(*vlocal, WHITE);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
							parse_error = "Error in variation: " + pmove->info + "." + *vlocal;
							return vinpt;
										// error
						}
			}
			else if( /*pmove->y == 0*/ !pmove->MoveFlagIsSet(MOVE_FLAG_Y)
					&& /*(pmove->x != 0)*/pmove->MoveFlagIsSet(MOVE_FLAG_X) 
					&& !(close_curly>0) 
					&& !(close_smooth==0) && (vlocal->length() != 0))
			{
				int res = -1;
				res = vlocal->find("}");
				// shamefully enough we need to check that we aren't in 
				// a closed bracket node. I can't return ++vinput from the
				// above function to handle comments.
				if( res < 0  )
				{
					Move *newmove = pmove;
					//pmove->y = 1;
					pmove->SetMoveFlag( MOVE_FLAG_Y );
					//pmove->black = *vlocal;
					int ret = pmove->SetMove(*vlocal, BLACK);
							//pMove->black = *vec_s_iter;
						if( ret != PASSED_TEST )
						{
							parse_error = "Error in variation: " + pmove->info + "..." + *vlocal;
							return vinpt;
										// error
						}
					pmove->pNext = new Move();
					pmove = pmove->pNext;
					pmove->pPrev = newmove;
					//cout<<pmove->info<<" "<<
					//	pmove->white <<" "<<
					//	pmove->black<<" "<<endl;
				}
			}

			int close_bracket = -1;
			close_bracket = vlocal->find(")");
			
			if( close_bracket != -1 )
			{
				if( pmove->white != "" )
				{
					if( pmove->black != "" && 
						pmove->black != " " &&
						!isResultString(pmove->black) &&
						/*pmove->y != 0*/ pmove->MoveFlagIsSet( MOVE_FLAG_Y) )
					{
						//pmove->atEndWhite = false;
						//pmove->atEndBlack = true;
						pmove->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						pmove->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					}
					else if( !isResultString( pmove->white ) && /*pmove->x != 0*/pmove->MoveFlagIsSet( MOVE_FLAG_X) )
					{
						//pmove->atEndWhite = true;
						//pmove->atEndBlack = false;
						pmove->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
						pmove->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					}
					else
					{
						if( pmove->pPrev )
						{
							//pmove->pPrev->atEndWhite = false;
							//pmove->pPrev->atEndBlack = true;
							pmove->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
							pmove->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
						}
					}
				}
				else
				{
					if( pmove->pPrev )
					{
						if( pmove->pPrev->black != "" &&
							!isResultString(pmove->pPrev->black))
						{
							if( /*pmove->pPrev->y != 0*/ pmove->pPrev->MoveFlagIsSet(MOVE_FLAG_Y) )
							{
								//pmove->pPrev->atEndWhite = false;
								//pmove->pPrev->atEndBlack = true;
								pmove->pPrev->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
								pmove->pPrev->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
							}
							else if( pmove->pPrev->white != "" )
							{
								if( /*pmove->pPrev->x != 0*/ pmove->pPrev->MoveFlagIsSet(MOVE_FLAG_X) )
								{
									//pmove->pPrev->atEndWhite = true;
									//pmove->pPrev->atEndBlack = false;
									pmove->pPrev->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
									pmove->pPrev->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
								}
							}
						}
						else if( pmove->pPrev->white != "")
						{
							if( /*pmove->pPrev->x != 0*/pmove->pPrev->MoveFlagIsSet(MOVE_FLAG_X) && 
								!isResultString(pmove->pPrev->white))
							{
								//pmove->pPrev->atEndWhite = true;
								//pmove->pPrev->atEndBlack = false;
								pmove->pPrev->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
								pmove->pPrev->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
							}
							else if( pmove->pPrev->pPrev )
							{
								if( pmove->pPrev->pPrev->black != "" )
								{
									//pmove->pPrev->pPrev->atEndWhite = false;
									//pmove->pPrev->pPrev->atEndBlack = true;
									pmove->pPrev->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
									pmove->pPrev->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
									
								}
								else 
								{
									//pmove->pPrev->pPrev->atEndWhite = true;
									//pmove->pPrev->pPrev->atEndBlack = false;
									pmove->pPrev->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
									pmove->pPrev->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
								}
							}
							
						}
						else if(pmove->pPrev->pPrev)
						{
							if( pmove->pPrev->pPrev->black != "" )
							{
								//pmove->pPrev->pPrev->atEndWhite = false;
								//pmove->pPrev->pPrev->atEndBlack = true;
								pmove->pPrev->pPrev->UnsetMoveFlag( MOVE_FLAG_AT_END_WHITE );
								pmove->pPrev->pPrev->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
							}
							else 
							{
								//pmove->pPrev->pPrev->atEndWhite = true;
								//pmove->pPrev->pPrev->atEndBlack = false;
								pmove->pPrev->pPrev->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
								pmove->pPrev->pPrev->UnsetMoveFlag( MOVE_FLAG_AT_END_BLACK );
							}
						}
					}
					// try to set the previous one ...
				}
				//cout <<"bracket closed"<<endl;
				int erase = -1;
				erase = vlocal->find(")"); // quick hack to handle multiple brackets
				vlocal->erase(erase,1);	// stored in the same string (we didn't split brackets)
										// more convenient than writing a recursive descent interpreter
				return vlocal;
			}
		}
		return vinpt;
	}


	bool isResultString( string str )
	{
		return ((str == "1-0") || (str == "0-1") || (str== "1/2-1/2") || (str == "*"));
	}


	Move* CreateWithNoSpaceAfterFullstop2(vector<string>::iterator it_s,  
										int fullstop, Move **newmove)
	{		
		char fname[255];

		string tempstr = it_s->substr(0, fullstop);
					

		//(*newmove)->info = tempstr;
		if( (*newmove)->SetInfo( tempstr) != PASSED_TEST )
		{
			parse_error = "Error, move number error in variation ";
			return *newmove;
		}


		string tempstr2 = it_s->substr(fullstop+1);

		//(*newmove)->white = tempstr2;
		int ret = (*newmove)->SetMove(tempstr2, WHITE);
							//pMove->black = *vec_s_iter;
		if( ret != PASSED_TEST )
		{
			parse_error = "Error in variation: " + (*newmove)->info + "..." + tempstr2;
			return *newmove;
										// error
		}

		return *newmove;
	}




		// note: some moves are of the format 8. e4 and 8...e5 with a variation in
	// between so these moves are duplicated and so here we have a large
	// watered down function to simply search through all the strings looking
	// for ugly ... text.
	void FixMoves()
	{
		return;

	}


	
	vector<string>::iterator vec_s_iter;
	string game_text;
	vector<string> *vec_moves;
	vector<Move>::iterator move_iter;
	int move_counter;
	vector<HeaderElem> vec_header;
	//vector<Move> moves;
	int game_curr_move;
	int white_black;
	int end_white_black;
	string result;

	

	bool gameBeginsAtStartPos;
};


class MemoryFileContainer
{
public:

	MemoryFileContainer();
	~MemoryFileContainer();

	MemPool * gameIndexMemPool;
	InputMemoryFile *source_mf_p;
	MemoryFile* vm_compressed_games;
	string filename;
	int gameCount;

	bool GetHeader( void* dst, int offset );

	int Index;

	bool Save();
};

#define MEMFILE_CONTAINER_MODE_SEPARATE 1 
#define MEMFILE_CONTAINER_MODE_LINKED 2

class MemFileContainerDataPageList
{
public:

	MemFileContainerDataPageList()
	{
		mode = MEMFILE_CONTAINER_MODE_SEPARATE;
		pMemFileContainer = 0;
		flush_last_file = false;
	}

	~MemFileContainerDataPageList()
	{
		for( int i = 0; i < m_pages.size(); i++ )
		{
			if( m_pages[i] )
			{
				delete m_pages[i];
				m_pages[i] = NULL;
			}
		}
	}


	char * GetDataOffset(int idx);

	MemoryFileContainer* GetMemPageForIndex(int idx);

	bool CopyIndexMemblockMemoryFromOffset( void* dest, int offset );

	vector<MemoryFileContainer*> m_pages;

	int mode;

	MemoryFileContainer* pMemFileContainer; // most recent.

	bool flush_last_file; 
};


class CFileContainer
{
public:

	CFileContainer();
	~CFileContainer();

	CFileMemPool * gameIndexMemPool;
	FILE *source_mf_p; // unsused 
	FILE* vm_compressed_games;
	string filename;
	int gameCount;

	int Index;

	bool GetHeader( void* dst, int offset );

	bool Save();
};

#define CFILE_CONTAINER_MODE_SEPARATE 1 
#define CFILE_CONTAINER_MODE_LINKED 2

class CFileContainerDataPageList
{
public:

	CFileContainerDataPageList()
	{
		mode = CFILE_CONTAINER_MODE_SEPARATE;
		pMemFileContainer = 0;
		flush_last_file = false;
	}

	~CFileContainerDataPageList()
	{
		for( int i = 0; i < m_pages.size(); i++ )
		{
			if( m_pages[i] )
			{
				delete m_pages[i];
				m_pages[i] = NULL;
			}
		}
	}


	char * GetDataOffset(int idx);

	CFileContainer* GetMemPageForIndex(int idx);

	vector<CFileContainer*> m_pages;

	int mode;

	CFileContainer* pMemFileContainer; // most recent.

	bool flush_last_file; 
};


struct IndexRemapRow
{
	int type[8];
};

class CTreeNode1
{
public:

	CompressedChessMove mvWhite;
	CompressedChessMove mvBlack;

	CTreeNode1 *pNext;
	CTreeNode1 *pPrev;
	CTreeNode1 *pUp;
	CTreeNode1 *pVarWhite;
	CTreeNode1 *pVarBlack;
};
/*
	case 1: heap_sort_GameIndexes32M(this->gameIndexes32M, cmpfun_gameIndexes32M_data1, this->virtualGameCount); break;
	case 2: heap_sort_GameIndexes32M(this->gameIndexes32M, cmpfun_gameIndexes32M_bname, this->virtualGameCount); break;
	case 3: heap_sort_GameIndexes32M(this->gameIndexes32M, cmpfun_gameIndexes32M_elow, this->virtualGameCount); break;
	case 4: heap_sort_GameIndexes32M(this->gameIndexes32M, cmpfun_gameIndexes32M_elob, this->virtualGameCount); break;
	case 5: heap_sort_GameIndexes32M(this->gameIndexes32M, cmpfun_gameIndexes32M_Site, this->virtualGameCount); break;
	case 6: heap_sort_GameIndexes32M(this->gameIndexes32M, cmpfun_gameIndexes32M_Event, this->virtualGameCount); break;
	case 7: heap_sort_GameIndexes32M(this->gameIndexes32M, cmpfun_gameIndexes32M_Date, this->virtualGameCount); break;
*/	

/**
* Header Sort Definitions for Filters
*/

/**
* FILTER TYPES
*/
#define SORT 0
#define FIND 1

#define FILTER_NAME 0
#define FILTER_ELO 1
#define FILTER_ECO 2
#define FILTER_EVENT 3
#define FILTER_SITE 4
#define FILTER_DATE 5
#define FILTER_RESULT 6

/**
* Filter Classes
*/
class Filter
{
public:

	Filter()
	{
	}

	virtual int GetType()=0;

	vector<int> indexes;
};

class NameFilter : public Filter
{
public:

	NameFilter()
	{
		nameMin = "";
		nameMax = "";
		color = 2;
	}

	int GetType()
	{
		return FILTER_NAME;
	}

	string nameMin;
	string nameMax;
	int color;
};

class EloFilter : public Filter
{
public:

	EloFilter()
	{
		eloMin = 0;
		eloMax = 9000;
		color = 2;
	}
	int GetType()
	{
		return FILTER_ELO;
	}

	int eloMin;
	int eloMax;
	int color;

};

class EcoFilter : public Filter
{
public:
	EcoFilter()
	{
		ECO_min = "";
		ECO_max = "";
	}
	int GetType()
	{
		return FILTER_ECO;
	}

	string ECO_min;
	string ECO_max;
};

class DateFilter : public Filter
{
public:
	DateFilter()
	{
		dateMin = "";
		dateMax = "";
	}
	int GetType()
	{
		return FILTER_DATE;
	}
	string dateMin;
	string dateMax;
};

class EventFilter : public Filter
{
public:
	EventFilter()
	{
		eventMin = "";
		eventMax = "";
	}

	int GetType()
	{
		return FILTER_EVENT;
	}
	string eventMin;
	string eventMax;
};

class SiteFilter : public Filter
{
public:
	SiteFilter()
	{
		siteMin = "";
		siteMax = "";
	}
	int GetType()
	{
		return FILTER_SITE;
	}
	string siteMin;
	string siteMax;
};

class ResultFilter : public Filter
{
public:
	ResultFilter()
	{
		resMin = 0;
		resMax = 2;
	}
	int GetType()
	{
		return FILTER_RESULT;
	}
	int resMin;
	int resMax;
};

class FilterStage
{
public:

	vector< Filter* > m_filters;
};



class ResultCache
{
public:
	int wwins;
	int bwins;
	int draws;

	//uint64 hash_key;
	//char san[8];
	string san;
	unsigned char position[8][8];

	ResultCache()
	{
		wwins = 0;
		bwins = 0;
		draws = 0;
		/* if san be chars then
		for( int i = 0; i < 8; i++ )
		{
			san[i]=0;
		}*/
		/* if not san be chars*/
		san = ""; /*then san be string = ""*/
	}
	/*
	void SetPosition(unsigned char *pos )
	{
		for( int i = 0; i < 8; i++ )
		{
			for( int j = 0; j < 8; j++ )
			{
				position[i][j]= pos[i][j];
			}
		}
		
	}

	bool PositionsAreEqual( unsigned char pos[8][8])
	{
		for( int i = 0; i < 8; i++ )
		{
			for( int j = 0; j < 8; j++ )
			{
				if( pos[i][j] != position[i][j] )
				{
					return false;
				}
			}
		}
		return true;
	}
	*/

	void operator+=(char result)
	{
		if( result == 0 ){ wwins++; return; }
		if( result == 1 ){ bwins++; return; }
		if( result == 2 ){ draws++; return; }
	}

	void Clear()
	{
		wwins = 0;
		bwins = 0;
		draws = 0;

		/* if san be chars then
		for( int i = 0; i < 8; i++ )
		{
			san[i]=0;
		}*/
		/* if not san be chars*/
		san = ""; /*then san be string = ""*/
	}

	inline int GetNumWhiteWins()
	{
		return wwins;
	}

	inline int GetNumBlackWins()
	{
		return bwins;
	}

	inline int GetNumDraws()
	{
		return draws;
	}

	inline int GetTotal()
	{
		return (wwins + bwins + draws);
	}

	inline float GetWhiteWinsPercentage()
	{
		return 100.0f*(float(wwins))/(float(GetTotal()));
	}

	inline float GetBlackWinsPercentage()
	{
		return 100.0f*(float(bwins))/(float(GetTotal()));
	}

	inline float GetDrawPercentage()
	{
		return 100.0f*(float(draws))/(float(GetTotal()));
	}
};


class ResultCacheExtraStats
{
public:
	int wwins;
	int bwins;
	int draws;

	//uint64 hash_key;
	//char san[8];
	string san;
	unsigned char position[8][8];

	ResultCacheExtraStats()
	{
		wwins = 0;
		bwins = 0;
		draws = 0;
		/* if san be chars then
		for( int i = 0; i < 8; i++ )
		{
			san[i]=0;
		}*/
		/* if not san be chars*/
		san = ""; /*then san be string = ""*/
	}
	/*
	void SetPosition(unsigned char *pos )
	{
		for( int i = 0; i < 8; i++ )
		{
			for( int j = 0; j < 8; j++ )
			{
				position[i][j]= pos[i][j];
			}
		}
		
	}

	bool PositionsAreEqual( unsigned char pos[8][8])
	{
		for( int i = 0; i < 8; i++ )
		{
			for( int j = 0; j < 8; j++ )
			{
				if( pos[i][j] != position[i][j] )
				{
					return false;
				}
			}
		}
		return true;
	}
	*/

	void operator+=(char result)
	{
		if( result == 0 ){ wwins++; return; }
		if( result == 1 ){ bwins++; return; }
		if( result == 2 ){ draws++; return; }
	}

	void Clear()
	{
		wwins = 0;
		bwins = 0;
		draws = 0;

		/* if san be chars then
		for( int i = 0; i < 8; i++ )
		{
			san[i]=0;
		}*/
		/* if not san be chars*/
		san = ""; /*then san be string = ""*/
	}

	inline int GetNumWhiteWins()
	{
		return wwins;
	}

	inline int GetNumBlackWins()
	{
		return bwins;
	}

	inline int GetNumDraws()
	{
		return draws;
	}

	inline int GetTotal()
	{
		return (wwins + bwins + draws);
	}

	inline float GetWhiteWinsPercentage()
	{
		return 100.0f*(float(wwins))/(float(GetTotal()));
	}

	inline float GetBlackWinsPercentage()
	{
		return 100.0f*(float(bwins))/(float(GetTotal()));
	}

	inline float GetDrawPercentage()	{
		return 100.0f*(float(draws))/(float(GetTotal()));
	}
};



class HeaderSortData
{
public:
	HeaderSortData()
	{
		for( int i = 0; i < 10; i++ )
		{
			SortFlags[i] = false;
		}
		lastSort = MOVES;
	}
	enum {MOVES, NUMBER, WNAME, BNAME, WELO, BELO, ECO, EVENT, SITE, DATE, RESULT};
	int lastSort;
	bool SortFlags[10]; 

	void UnsetLastFlag()
	{
		SortFlags[lastSort] = false;
	}

	void UnsetFlag( int flag )
	{
		SortFlags[flag] = false;
	}

	void SetFlag( int flag )
	{
		if( (flag < 0) || (flag >= 10 ))
		{
			return;
		}

		SortFlags[flag] = true;
	}
};

class StoredResultCacheVector
{
public:

	unsigned char pos[8][8];
	vector< ResultCache*> vecResCache;
};

class StoredResultCacheExtraStatsVector
{
public:

	unsigned char pos[8][8];
	vector< ResultCacheExtraStats* > vecResCache;
};


class PositionSearchBoard
{
public:
	
	bool find_pawns;
	bool find_similar;
	//uint64 hash_key;
	//char san[8];
	
	unsigned char position[8][8];
};

class PgnLoad
{
public:
	int sort_method;
//	std::unordered_map<uint64, HashEntry> hash_table;
	vector<string> vec_strings;
	vector<HeaderElem> vec_header;
	vector<string> *vec_moves;
	vector<string>::iterator vec_s_iter;
	vector<Move> moves;
	vector<Move>::iterator move_iter;
	int move_counter;
	int pgn_load_mode;

	// standard
	vector<Game*> games;
	string pgnFileName;
	

	// virtual
	bool vm_fileIsLoaded;
	const char* vm_data;
	MemPool *gameIndexMemPool;
	int virtualGameCount;
	InputMemoryFile *source_mf_p;
	string vmFileName;
	ResultCache temporyResCache;
	vector<MemoryFileContainer*> memFiles;

	MemFileContainerDataPageList memFiles_CONTAINER;

	string vm_read_error;
	GameIndex *startI;
	GameIndex *currI;

	GameIndex32Moves *startI32M;
	GameIndex32Moves *currI32M;
	GameIndex32Moves **gameIndexes32M;
	vector< GameIndex32Moves * > m_vecGameIndexes;



	ExpandingArrayList < SortableGameIndex* > m_GameIndexes;
	vector< SortableGameIndex * > m_SortableIndexes;
	vector< SortableGameIndex * > m_filteredIndexes; // reserve(virtualGameCount)
	bool filter;
	vector< int > tree_filtered_indexes;
	bool sorted_filtered;

	bool load_mode_refresh_list;
	int SORTABLE_INDEX_MODE;

	// databse
	sqlite3 *db;
	sqlite3_stmt    *res;
	int rec_count;
	char *errMSG;
	const char      *tail;
	int error;
	string dbname;


	// compression
	CompressedGameIndex **compressedIndexes;
	HeaderListData headersListData;
	int compressedGameCount;
	bool CompressedGameLoaded;
	string compressedFileName;
	IndexRemapRow ** indexRemapRows;
	
	int sortType;

	FilterStage m_filterStage;

	vector< int > error_games;


	// sorting 
	HeaderSortData m_sortData;



	MemoryFileContainer *pMemFileContainer;



	// position search
	PositionSearchBoard m_positionSearch;

	PgnLoad(){
		move_counter = -1;
		vec_moves = NULL;
		pgnFileName = "";
		
		pgn_load_mode = 0;
		load_mode_refresh_list = true;
		
		vm_data = 0;
		gameIndexMemPool = 0;
		virtualGameCount = 0;
		startI = 0;
		vm_fileIsLoaded = false;
		source_mf_p = 0;
		currI = 0;
		vmFileName = "";

		rec_count = 0;
		db = 0;
		res = 0;
		rec_count =  0;
		errMSG = 0;
		tail = 0;
		error = 0;
		dbname = "";

		compressedIndexes = 0;
		CompressedGameLoaded = false;
		compressedFileName = "";

		sortType= 0;
		indexRemapRows = 0;

		sort_method= 1;
		filter = false;

		sorted_filtered = false;

		pMemFileContainer = 0;

		SORTABLE_INDEX_MODE = -1;//SORTABLE_GAME_INDEX_MODE_RAM;
	}
	PgnLoad(string filename){}
	~PgnLoad()
	{
		Reset();
		
		for( int i = 0; i < memFiles.size(); i++ )
		{
			delete memFiles[i];
		}

		if( db )
		{
			CloseSQLDatabase();
		}	
	}

	bool AreFiltersClear()
	{
		if( m_filterStage.m_filters.size() == 0 )
			return true;
		else return false;
	}

	void ClearFilters()
	{
		for( int i = 0; i < m_filterStage.m_filters.size(); i++ )
		{
			delete m_filterStage.m_filters[i];
		}
		if( m_filterStage.m_filters.size() > 0 )
		{
			m_filterStage.m_filters.clear();
		}

		filter = false;

		if( this->tree_filtered_indexes.size() > 0 )
		{
			this->tree_filtered_indexes.clear();

		}

		for( int j = 0; j < this->virtualGameCount; j++ )
		{
			this->tree_filtered_indexes.push_back(j);
		}

		if( m_filteredIndexes.size() > 0 )
		{
			m_filteredIndexes.clear();
		}

		for( int j = 0; j < this->m_SortableIndexes.size(); j++ )
		{
			this->m_filteredIndexes.push_back(this->m_SortableIndexes[j]);
		}
	}

	void AddNameFilterBoth( string &name1, string &name2 )
	{
		Filter *nm = new NameFilter();
		((NameFilter*)nm)->nameMin = name1;
		((NameFilter*)nm)->nameMax = name2;
		((NameFilter*)nm)->color = 2;

		m_filterStage.m_filters.push_back(nm);
	}

	void AddDateFilter( string &date1, string &date2 )
	{
		Filter *nm = new DateFilter();
		((DateFilter*)nm)->dateMin = date1;
		((DateFilter*)nm)->dateMax = date2;
		//((DateFilter*)nm)-> = 2;

		m_filterStage.m_filters.push_back(nm);
	}

	void AddEventFilter( string &name1, string &name2 )
	{
		Filter *nm = new EventFilter();
		((EventFilter*)nm)->eventMin = name1;
		((EventFilter*)nm)->eventMax = name2;
		//((DateFilter*)nm)-> = 2;

		m_filterStage.m_filters.push_back(nm);
	}

	void AddSiteFilter( string &name1, string &name2 )
	{
		Filter *nm = new SiteFilter();
		((SiteFilter*)nm)->siteMin = name1;
		((SiteFilter*)nm)->siteMax = name2;
		//((DateFilter*)nm)-> = 2;

		m_filterStage.m_filters.push_back(nm);
	}

	void AddNameFilterWhite( string &name1, string &name2 )
	{
		Filter *nm = new NameFilter();
		((NameFilter*)nm)->nameMin = name1;
		((NameFilter*)nm)->nameMax = name2;
		//((DateFilter*)nm)-> = 2;
		((NameFilter*)nm)->color = 0;
		m_filterStage.m_filters.push_back(nm);
	}

	void AddNameFilterBlack( string &name1, string &name2 )
	{
		Filter *nm = new NameFilter();
		((NameFilter*)nm)->nameMin = name1;
		((NameFilter*)nm)->nameMax = name2;
		//((DateFilter*)nm)-> = 2;
		((NameFilter*)nm)->color = 1;
		m_filterStage.m_filters.push_back(nm);
	}

	void AddECOFilter( string &name1, string &name2 )
	{
		Filter *nm = new EcoFilter();
		((EcoFilter*)nm)->ECO_min = name1;
		((EcoFilter*)nm)->ECO_max = name2;
		//((DateFilter*)nm)-> = 2;
		//((NameFilter*)nm)->color = 1;
		m_filterStage.m_filters.push_back(nm);
	}

	void AddEloFilterBoth(int eloMin, int eloMax)
	{
		Filter *nm = new EloFilter();
		((EloFilter*)nm)->eloMin = eloMin;
		((EloFilter*)nm)->eloMax = eloMax;
		((EloFilter*)nm)->color = 2;
		//((DateFilter*)nm)-> = 2;
		//((NameFilter*)nm)->color = 1;
		m_filterStage.m_filters.push_back(nm);
	}

	void AddEloFilterWhite(int eloMin, int eloMax)
	{
		Filter *nm = new EloFilter();
		((EloFilter*)nm)->eloMin = eloMin;
		((EloFilter*)nm)->eloMax = eloMax;
		((EloFilter*)nm)->color = 0;
		//((DateFilter*)nm)-> = 2;
		//((NameFilter*)nm)->color = 1;
		m_filterStage.m_filters.push_back(nm);
	}

	void AddEloFilterBlack(int eloMin, int eloMax)
	{
		Filter *nm = new EloFilter();
		((EloFilter*)nm)->eloMin = eloMin;
		((EloFilter*)nm)->eloMax = eloMax;
		((EloFilter*)nm)->color = 0;
		//((DateFilter*)nm)-> = 2;
		//((NameFilter*)nm)->color = 1;
		m_filterStage.m_filters.push_back(nm);
	}

	/***/

	vector< StoredResultCacheVector* > vecStandardCacheResults;
	vector< StoredResultCacheExtraStatsVector* > vecExtraStatsCacheResults;






	bool IsCompressedGameLoaded()
	{
		return CompressedGameLoaded;
	}

	bool OpenSQLDatabase(string &filename);

	wxString GetRowColumnData(int row, int column);

	void CloseSQLDatabase();

	bool IsVMFileLoaded()
	{
		return vm_fileIsLoaded;
	}
	
	void MoveGameUpInList(int i)
	{
		if( i < games.size() && i > 0)
		{
			Game* temp = games[i];
			games[i] = games[i-1];
			games[i-1] = temp;
		}
	}

	void MoveGameDownInList(int i)
	{
		if( i < games.size()-1 && i >= 0)
		{
			Game* temp = games[i];
			games[i] = games[i+1];
			games[i+1] = temp;
		}
	}


	void Reset()
	{
		SORTABLE_INDEX_MODE = SORTABLE_GAME_INDEX_MODE_RAM;

		vec_strings.clear();
		vec_header.clear();
		if( vec_moves)
		{
			/*vec_moves->clear();*/
		}
		//moves.clear();
	/*	for( int i = 0; i < games.size(); i++)
		{
			if( games[i] != NULL )
			{
				games[i]->ResetMoveList();
				delete games[i];
				games[i] = NULL;
			}
		}
		games.clear();*/
		/*if( gameIndexMemPool )
		{
			delete gameIndexMemPool;
			gameIndexMemPool = NULL;
		}*/

		this->headersListData.Clear();

		if( m_vecGameIndexes.size() != 0 )
		{
			for( int i = 0; i < m_vecGameIndexes.size(); i++ )
			{
				delete m_vecGameIndexes[i];
				m_vecGameIndexes[i] = NULL;
			}
			m_vecGameIndexes.clear();
		}

		if( m_GameIndexes.size() != 0 )
		{
			for( int i = 0; i < m_GameIndexes.size(); i++ )
			{
				delete m_GameIndexes[i];
				m_GameIndexes[i] = NULL;
			}
			//m_GameIndexes.clear();
		}

		virtualGameCount = 0;
		vm_fileIsLoaded = false;
		source_mf_p =0;
		/*if( source_mf_p )
		{

			delete source_mf_p;
			source_mf_p = NULL;
		}*/

		if( startI )
		{
			delete startI;
			startI = NULL;
		}

		if( currI )
		{
			delete currI;
			currI = NULL;
		}

		virtualGameCount = 0;
		vm_data = 0;

		if( CompressedGameLoaded )
		{
			if( compressedIndexes )
			{
				for( int i = 0; i < compressedGameCount; i++ )
				{
					delete compressedIndexes[i];
				}
				delete [] compressedIndexes;
			}
			compressedIndexes = 0;
			
			if( indexRemapRows )
			{
				for( int i = 0; i < compressedGameCount; i++ )
				{
					delete indexRemapRows[i];
				}
				delete [] indexRemapRows;
			}
			
			CompressedGameLoaded= false;
			compressedGameCount = 0;

			
		}

		ClearFilters();
		
		
	}


	void Split( int index )
	{
		
	}


	void SortAscendingRAM(int id);
	void SortDescendingRAM(int id);

	void SortByMoves( );
	void SortByNumber( );
	void SortByWhiteName( );
	void SortByBlackName( );
	void SortByWhiteElo( );
	void SortByBlackElo( );
	void SortByECO( );
	void SortByEvent( );
	void SortBySite( );
	void SortByDate( );
	void SortByResult( );

	void SetLoadMode(int m)
	{
		pgn_load_mode = m;
	}

	string &get_vm_read_error()
	{
		return vm_read_error;
	}

	const char* IndexVM_PGN(const char* sourceName, void* user_data =0);

	const char* IndexRAM_PGN_32M(const char* sourceName, void* user_data =0);

	const char* AddIndexRAM_PGN_32M(const char* sourceName, void* user_data=0);

	const char* IndexVM_PGN_32M(const char* sourceName, void* user_data =0);

	const char* AddIndexVM_PGN_32M(const char* sourceName, void* user_data=0);

	bool ParseHeader32M( const char* src, int dataLen, HeaderListData &headers, GameIndex32Moves *gi);

	bool ParseGame32M( const char* src, int dataLen, HeaderListData &headers, GameIndex32Moves *gi);

	bool SaveDB(string &filename);

	bool SaveGames32M(string &filename);

	bool LoadGames32M(string &filename);

	bool AddGames32M(string &filename);

	bool LoadDB_32_VM(string &filename);

	bool AddDB_32_VM(string &filename);

	bool LoadCompressedGameIndexes(string filename);

	Game* GetRAMGame(int vmIndex);

	void StringFilterRAMGames(string &toMatch);

	void StringFilterRAMGamesIntoResultCache(string &toMatch, vector<ResultCache*> &results, int black_white_move);


	void IntFilterRAMGamesIntoResultCache(vector<unsigned short int> *toMatch, vector<ResultCache*> &results, int black_white_move, ChessBoard &brd);
	void IntFilterRAMGamesIntoResultCache_StartPos( vector<ResultCache*> &results, ChessBoard &brd);


	void IntFilterRAMGamesIntoResultCache_CollectExtraStats(vector<unsigned short int> *toMatch, vector<ResultCacheExtraStats*> &results, int black_white_move, ChessBoard &brd);
	void IntFilterRAMGamesIntoResultCache_CollectExtraStats_Startpos(vector<ResultCacheExtraStats*> &results, ChessBoard &brd);


	void IntFilter_FilteredRAMGamesIntoResultCache(vector<unsigned short int> *toMatch, vector<ResultCache*> &results, int black_white_move, ChessBoard &brd);
	void IntFilter_FilteredRAMGamesIntoResultCache_StartPos( vector<ResultCache*> &results, ChessBoard &brd);
	

	void IntFilter_FilteredRAMGamesIntoResultCache_CollectExtraStats(vector<unsigned short int> *toMatch, vector<ResultCacheExtraStats*> &results, int black_white_move, ChessBoard &brd);
	void IntFilter_FilteredRAMGamesIntoResultCache_CollectExtraStats_Startpos(vector<ResultCacheExtraStats*> &results, ChessBoard &brd);

	
	void PositionSearch(ChessBoard &brd, int ply_count);
	void PositionSearchSimilar(ChessBoard &brd, int ply_count);

	void PositionSearchPawns(ChessBoard &brd, int ply_count);
	void PositionSearchPawnsSimilar(ChessBoard &brd, int ply_count);

	bool FindName( string &name );

	bool FindName( string &name, int id );

	bool FindNameWhite( string &name, int id );

	bool FindNameBlack( string &name, int id );

	bool FilterEco( string &name, int id );

	bool FilterDate( string &name, int id );

	bool FilterSite( string &name, int id );

	bool FilterEvent( string &name, int id );


	void FilterRAMGamesByECO(string &eco);


	Game* GetVMGame(int vmIndex);

	bool CompressGame( PGN::Game *pGame, GameIndex *game_index, ChessBoard *board );

	Game* LoadCompressedGame(int idx, ChessBoard* board);

	bool CompressGameVM( PGN::Game *pGame, GameIndex *game_index, ChessBoard *board );

	string GetHeaderData( string &matchName, int gameID );

	void LoadGameHeaderIntoCurrent( GameIndex* curr, Game* currGame);
	void LoadGameHeaderIntoCurrent32M( GameIndex32Moves* curr, Game* currGame);

	void Startup()
	{

		Game *game = new Game();
		game->pCurrMove = game->pRoot;
		games.push_back(game);
		game->CreateEmptyHeader();
		
	}

	bool MemFileDataIsOk();

	Game * EraseGameAtIndex(int idx)
	{
		if( games.size() > idx )
		{
			if( games[idx] )
			{
				delete games[idx];
				games[idx] = NULL;
			}
			games.erase(games.begin() + idx);
			//NewGame();
			//return games[0];
		}

		if( games.size() == 0 )
		{
			Startup();
			return games.back();
		}
		else
		{
			return games[0];
		}
	}

	void RemoveLastGame()
	{
		vec_strings.clear();
		vec_header.clear();
		if( vec_moves )
		{
			/*vec_moves->clear();*/
		}
		moves.clear();
		//Game* game = *games.end();
		//delete game;
		//game = NULL;
		delete games[games.size()-1];
		games[games.size()-1]=NULL;
		games.erase(games.end()-1);

	}

	void ClearAllAfterFirstGame()
	{
		vec_strings.clear();
		vec_header.clear();
		for( int i = 1; i < games.size(); i++ )
		{
			delete games[i];
			games[i] = NULL;
		}

		games.clear();
	}

	Game* AddGame()
	{
		vec_strings.clear();
		vec_header.clear();
		if( vec_moves )
		{
			/*vec_moves->clear();*/
		}
		moves.clear();

		hash_table.clear();

		Game* game = new Game();
		game->pCurrMove = game->pRoot;
		games.push_back(game);
		game->CreateEmptyHeader();
		return game;
	}

	void ClearHashTable()
	{
		hash_table.clear();
	}


	Game* NewGame()
	{
		vec_strings.clear();
		vec_header.clear();
		if( vec_moves )
		{
			/*vec_moves->clear();*/
		}
		moves.clear();
		for( int i = 0; i < games.size(); i++)
		{
			if( games[i] != NULL )
			{
				games[i]->ResetMoveList();
				delete games[i];
				games[i] = NULL;
			}
		}

		games.clear();
		
		hash_table.clear();
		Game* game = new Game();
		game->pCurrMove = game->pRoot;
		games.push_back(game);
		game->CreateEmptyHeader();
		return game;

	}

	char GetLastMeaningfulChar(string line)
	{
		for( int i = line.length()-1; i >-1; i--)
		{
			if( (line[i] != ' ') && 
				(line[i] != '\r') &&
				(line[i] != '\t'))
				return line[i];
		}
		return 0;
	}

	Game* LoadPGNFromDatabaseSelection(int sel);

	bool PreScanPGN(string &filename);

	Game* LoadPgnFromString(const char* data) // maybe a bit unsafe
	{
		//Appender v;
		//ifstream myfile (filename);
		istringstream myfile(data);
		int currentGame = games.size()-1;
		bool in_header = false;
		int lineNum = 0;
		try
		{
			while ( myfile.good() )
			{
				string line;
				getline (myfile,line);
				int n = line.find(']');
				int n2 = line.find('[');

				if(line.length() == 0 )
					continue;
				
				if( line[0] == '[' && (GetLastMeaningfulChar(line) == ']'))
				//if( (n>0) && (n2==0) && ((n==line.size()-1)||(n==line.size()-2)))
				{
					if( in_header == false )
					{
						Game *game = new Game();
						games.push_back(game);
						currentGame++;
						in_header = true;
					}
					HeaderElem h;
					h.data.assign(line, 0, line.size());
					games[currentGame]->vec_header.push_back(h);
					
					//vec_header.push_back(h);
					//in_header = true;
				}
				else
				{
					if( lineNum == 0 )
						return NULL;

					
					in_header = false;
				}

				if( !in_header ) // then just split the rest by white space
				{ 
					/*istringstream iss(line);
					copy(istream_iterator<string>(iss),
						istream_iterator<string>(),
						back_inserter<vector<string> >(games[currentGame]->vec_moves));
						*/
					/*Appender v;
										
					tokenize2(v, line.c_str(), " \r\n\t{}()");
					for( int i = 0; i < v.size(); i++ )
					{
						//if( v[i].length() != 0 && v[i] != "\n" && v[i] != "\t" && v[i] != " ")
						games[currentGame]->vec_moves.push_back(v[i]);
					//	cout<<v[i]<<endl;
					}*/
					games[currentGame]->game_text += line + " ";
					
					
				}
				lineNum++;
				//cout << line << endl;
			}

			if( games[currentGame]->game_text == "" )
			{
				this->RemoveLastGame();
				return NULL;
			}
			return games[currentGame];
		}
		catch(...)
		{

			return NULL;
		//else cout << "Unable to open file"; 
		}
		return NULL;
	}


	int LoadPgn(string filename, void* user_data=0)
	{
		
		
		if( pgn_load_mode == 0 )
		{
			this->pgnFileName = filename;
			//Appender v;
			ifstream myfile (filename);
			int currentGame = games.size()-1;
			bool in_header = false;
			int lineNum = 0;
			if (myfile.is_open())
			{
				while ( myfile.good() )
				{
					string line;
					getline (myfile,line);
					int n = line.find(']');
					int n2 = line.find('[');

					if(line.length() == 0 )
						continue;
				
					if( line[0] == '[' && ((line[line.length()-1] == ']')||(line[line.length()-2] == ']')))
					{
						if( in_header == false )
						{
							if( games.size() == 1 )
							{
								if( games[0]->game_text == "" )
								{
									delete games[0];
									games[0] = NULL;
								
									games.clear();
								}
							
							}
							Game *game = new Game();
							game->SourceFileName = filename;
							games.push_back(game);
							currentGame = games.size()-1;
							game->sourceGameIndex = currentGame;
							in_header = true;
						}
						HeaderElem h;
						h.data.assign(line, 0, n+1);
						games[currentGame]->vec_header.push_back(h);
						//vec_header.push_back(h);
						//in_header = true;
					}
					else
					{
						in_header = false;
					}

					if( !in_header ) // then just split the rest by white space
					{ 
						/*istringstream iss(line);
						copy(istream_iterator<string>(iss),
							istream_iterator<string>(),
							back_inserter<vector<string> >(vec_moves));
							*/
						/*istringstream iss(line);
						copy(istream_iterator<string>(iss),
							istream_iterator<string>(),
							back_inserter<vector<string> >(games[currentGame]->vec_moves));*/
						games[currentGame]->game_text += line+" ";


					}
					//cout << line << endl;
				}
				myfile.close();
			
			}
			else return 0;
		}
		else if( pgn_load_mode == 1 )
		{
			SORTABLE_INDEX_MODE = SORTABLE_GAME_INDEX_MODE_RAM;
			if( load_mode_refresh_list == true )
			{
				this->IndexRAM_PGN_32M(filename.c_str(),user_data);
			}
			else
			{
				this->AddIndexRAM_PGN_32M(filename.c_str(),user_data);
			}
		}
		else if( pgn_load_mode == 3 )
		{
			SORTABLE_INDEX_MODE = SORTABLE_GAME_INDEX_MODE_RAM;
			if( load_mode_refresh_list == true )
			{
				if( !this->LoadGames32M(filename) )
				{

					return 0;
				}
			}
			else
			{
				if( !this->AddGames32M(filename) )
				{

					return 0;
				}
			}
		}

		return 1;
	}

	int LoadPgnVM(string filename, void* user_data=0)
	{
		SORTABLE_INDEX_MODE = SORTABLE_GAME_INDEX_MODE_VM;
		if( pgn_load_mode == 0 )
		{
			this->pgnFileName = filename;
			//Appender v;
			ifstream myfile (filename);
			int currentGame = games.size()-1;
			bool in_header = false;
			int lineNum = 0;
			if (myfile.is_open())
			{
				while ( myfile.good() )
				{
					string line;
					getline (myfile,line);
					int n = line.find(']');
					int n2 = line.find('[');

					if(line.length() == 0 )
						continue;
				
					if( line[0] == '[' && ((line[line.length()-1] == ']')||(line[line.length()-2] == ']')))
					{
						if( in_header == false )
						{
							if( games.size() == 1 )
							{
								if( games[0]->game_text == "" )
								{
									delete games[0];
									games[0] = NULL;
								
									games.clear();
								}
							
							}
							Game *game = new Game();
							game->SourceFileName = filename;
							games.push_back(game);
							currentGame = games.size()-1;
							game->sourceGameIndex = currentGame;
							in_header = true;
						}
						HeaderElem h;
						h.data.assign(line, 0, n+1);
						games[currentGame]->vec_header.push_back(h);
						//vec_header.push_back(h);
						//in_header = true;
					}
					else
					{
						in_header = false;
					}

					if( !in_header ) // then just split the rest by white space
					{ 
						/*istringstream iss(line);
						copy(istream_iterator<string>(iss),
							istream_iterator<string>(),
							back_inserter<vector<string> >(vec_moves));
							*/
						/*istringstream iss(line);
						copy(istream_iterator<string>(iss),
							istream_iterator<string>(),
							back_inserter<vector<string> >(games[currentGame]->vec_moves));*/
						games[currentGame]->game_text += line+" ";


					}
					//cout << line << endl;
				}
				myfile.close();
			
			}
			else return 0;
		}
		else if( pgn_load_mode == 1 )
		{
			if( load_mode_refresh_list == true )
			{
				this->IndexVM_PGN_32M(filename.c_str(),user_data);
			}
			else
			{
				this->AddIndexVM_PGN_32M(filename.c_str(),user_data);
			}
		}
		else if( pgn_load_mode == 3 )
		{
			if( load_mode_refresh_list == true )
			{
				if( !this->LoadDB_32_VM(filename) )
				{

					return 0;
				}
			}
			else
			{
				if( !this->AddGames32M(filename) )
				{

					return 0;
				}
			}
		}

		return 1;
	}

	

	void ParseGames()
	{
		for( int i = 0; i < games.size(); i++ )
		{
			games[i]->Parse();
		}
	}

	void PrintGames()
	{
		for( int i = 0; i < games.size(); i++ )
		{
		//	games[i]->Print();
		}
	}


	




	// note: some moves are of the format 8. e4 and 8...e5 with a variation in
	// between so these moves are duplicated and so here we have a large
	// watered down function to simply search through all the strings looking
	// for ugly ... text.
	void FixMoves()
	{
		return;

	}



	int FileSize(const char* sFileName)
	{
	  std::ifstream f;
	  f.open(sFileName, std::ios_base::binary | std::ios_base::in);
	  if (!f.good() || f.eof() || !f.is_open()) { return 0; }
	  f.seekg(0, std::ios_base::beg);
	  std::ifstream::pos_type begin_pos = f.tellg();
	  f.seekg(0, std::ios_base::end);
	  return static_cast<int>(f.tellg() - begin_pos);
	}

}; // end class pgnload





}; // end namespace PGN



bool CompressGame( PGN::Game *pGame, GameIndex *game_index, int count, ChessBoard *board );
bool DecompressGame(CompressedGameIndex* compressed_game_index, ChessBoard *board );

#endif