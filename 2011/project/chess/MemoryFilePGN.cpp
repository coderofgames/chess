//#include "stdafx.h"
#include "headers.h"
#include "MemoryFilePGN.h"
#include "Memfile.h"
#include "PGNLoad.h"
#include "ByteBoard.h"
// WIN32
#include <windows.h>
#include <tlhelp32.h>
// #endif
#include <stdio.h>
#include <conio.h>
#include <string.h>
// WIN32
#include <winbase.h>
#include "HeapSort.h"

#include "wx/busyinfo.h"

//#include "MiscUtils.h"
//#endif

const char* memFileData;
GameIndex *start;

GameIndex **arrayGi;

bool pgnMode = true;
InputMemoryFile *source_mf2;

MemoryFile *mem_pool_mf;



int SortableGameIndex::GetHdrWhiteNameIDX()
{
	return m_Gi->hdr.wname;
}
int SortableGameIndex::GetHdrBlackNameIDX()
{
	return m_Gi->hdr.bname;
}
int SortableGameIndex::GetHdrWhiteEloIDX()
{
	return m_Gi->hdr.whiteElo;
}
int SortableGameIndex::GetHdrBlackEloIDX()
{
	return m_Gi->hdr.blackElo;
}
int SortableGameIndex::GetHdrEventIDX()
{
	return m_Gi->hdr.eventname;
}
int SortableGameIndex::GetHdrSiteIDX()
{
	return m_Gi->hdr.sitename;
}
int SortableGameIndex::GetHdrDateIDX()
{
	return m_Gi->hdr.date;
}
int SortableGameIndex::GetHdrResult()
{
	return m_Gi->hdr.result;
}





MemPool::MemPool(int block_size)
{
	blockSize = block_size;
	currOffset = 0;
	pDataPool = 0;
}

MemPool::~MemPool()
{
	//pDataPool->Cl
	if(pDataPool)
	{
		delete pDataPool;
		
	}

}

char* MemPool::GetDataStart(){
	return pDataPool->data();
}

MemBlock* MemPool::GetBlock()
{
	MemBlock * mb = new MemBlock();
	mb->data = (char*)(pDataPool->data() + currOffset);
	mb->offset = currOffset;
	mb->size = blockSize;
	currOffset += blockSize;
		
	return mb;
}

MemBlock* MemPool::GetIndex(int i)
{
	MemBlock * mb = new MemBlock();
	mb->data = (char*)(pDataPool->data() + i*blockSize);
	mb->offset = i*blockSize;

	return mb;
}

char* MemPool::GetDataOffset(int i)
{
	return (char*)(pDataPool->data() + i*blockSize);
}

char* MemPool::GetDataOffset2(int i)
{
	return (char*)(pDataPool->data() + i);
}

bool MemPool::OpenMemoryFileMemory(char* dest, bool overwrite, int size)
{
	name = dest;
	pDataPool = new MemoryFile(dest, overwrite ?
		MemoryFile::if_exists_truncate_if_not_exists_create :
		MemoryFile::if_exists_fail_if_not_exists_create);
	if (! pDataPool->data()) return false;
	pDataPool->resize(size);
	if (size != pDataPool->size()) return false;
	/*std::copy(source_mf.data(), source_mf.data() + source_mf.size(),
		dest_mf.data());*/
	return true;
}
bool MemPool::OpenMemoryFileMemoryRead(char* dest, bool overwrite, int size)
{
	name = dest;
	pDataPool = new MemoryFile(dest, MemoryFile::if_exists_keep_if_dont_exists_fail);
	if (! pDataPool->data()) return false;
	pDataPool->resize(size);
	if (size != pDataPool->size()) return false;
	/*std::copy(source_mf.data(), source_mf.data() + source_mf.size(),
		dest_mf.data());*/
	return true;
}



//===============================================================
CFileMemPool::CFileMemPool(int block_size)
{
	blockSize = block_size;
	currOffset = 0;
	pDataPool = 0;
}

CFileMemPool::~CFileMemPool()
{
	//pDataPool->Cl
	if(pDataPool)
	{
		delete pDataPool;
		
	}

}

char* CFileMemPool::GetDataStart(){
	//return pDataPool->data();
	return 0;
}

MemBlock* CFileMemPool::GetBlock()
{
	MemBlock * mb = new MemBlock();
/*	mb->data = (char*)(pDataPool->data() + currOffset);
	mb->offset = currOffset;
	mb->size = blockSize;
	currOffset += blockSize;
		*/
	return mb;
}

MemBlock* CFileMemPool::GetIndex(int i)
{
	MemBlock * mb = new MemBlock();
	if( fseek(pDataPool, i*this->blockSize, SEEK_SET) == 0)
	{
		fread((void*)mb->data, sizeof(blockSize),1, pDataPool);
		mb->offset = i*blockSize;
		return mb;
	}
	//mb->data = (char*)(pDataPool->data() + i*blockSize);
	//mb->offset = i*blockSize;
	delete mb;
	return 0;
}

char* CFileMemPool::GetDataOffset(int i)
{
//	return (char*)(pDataPool->data() + i*blockSize);
	return 0;
}

char* CFileMemPool::GetDataOffset2(int i)
{
	//return (char*)(pDataPool->data() + i);
	return 0;
}

bool CFileMemPool::OpenFileMemory(char* dest, bool overwrite, int size)
{
	name = dest;
	pDataPool = fopen( dest, "w+b");
	/*pDataPool = new MemoryFile(dest, overwrite ?
		MemoryFile::if_exists_truncate_if_not_exists_create :
		MemoryFile::if_exists_fail_if_not_exists_create);
	if (! pDataPool->data()) return false;
	pDataPool->resize(size);*/
	//if (size != pDataPool->size()) return false;
	if( !pDataPool )
		return false;
	/*std::copy(source_mf.data(), source_mf.data() + source_mf.size(),
		dest_mf.data());*/
	return true;
}
bool CFileMemPool::OpenFileMemoryRead(char* dest, bool overwrite, int size)
{
	name = dest;
	pDataPool = fopen( dest, "w+b");

	if( !pDataPool )
		return false;
	/*std::copy(source_mf.data(), source_mf.data() + source_mf.size(),
		dest_mf.data());*/
	return true;
}

//=============================================================
MemBlock *MemPageSystem::GetBlockIndex(int idx)
{
	int counter = 1;
	int currMax = this->m_pages[0]->GetMaxNumBlocks();

	while( idx > currMax && counter < m_pages.size() )
	{
		currMax += this->m_pages[counter++]->GetMaxNumBlocks();
	}

	if( idx > currMax )
	{

		return 0;
	}

	return this->m_pages[counter-1]->GetBlock();
}

string GetSubstringInQuotes(string &inpt)
{
	int openQuotes = inpt.find('"');
	int closeQuotes = inpt.find_last_of('"');

	int length = inpt.length();
	int size = (length-openQuotes+1) - (length - closeQuotes);//.c_str()
	return inpt.substr(openQuotes+1, size-2);
}



bool NewLine(const char* data, int i)
{
	return (data[i] == '\r') || (data[i] == '\r\n');
}

bool NextLinePattern(const char* data, int i)
{
	return ((data[i+1] == '\n' && data[i+2] == '\r')|| 
			(data[i+1] == '\r\n') ||
			(data[i+1] == '\n' && data[i+2] == '\n'));
}


bool CompressGame( PGN::Game *pGame, GameIndex *game_index, int count, ChessBoard *board )
{

		int numPly= 0;//pGame->CountMoves();

		PGN::Move *pCountMoves = pGame->pRoot;
		while( pCountMoves )
		{
			if( pCountMoves->white != "" )
				numPly++;
			else break;

			if( pCountMoves->black != "" )
				numPly++;
			else break;

			pCountMoves = pCountMoves->pNext;
		}
		game_index->moves.Allocate(numPly);

		PGN::Move * tempMove = pGame->pRoot;

		//cout<< "Testing Compress" << endl;

		board->SetInitialPosition();
		int plyCount = 0;
		while( tempMove )
		{
			if( (plyCount >= game_index->moves.plyCount) || (tempMove->white == "") )
				break;
			board->GenLegalMoves();
			board->parseMove(tempMove->white);
			tempMove->SetFenWhite(board->Fen());
			tempMove->SetWhiteMoveSquares(board->currMove);
			board->SetBoard(tempMove->m_fenWhite);
			bool legalMoveWhiteFound = false;
			for( unsigned char nmLm = 0; nmLm < (unsigned char)board->numLegalMoves; nmLm++ )
			{
				if( tempMove->whiteMove == board->legal_moves[nmLm].mv )
				{
					legalMoveWhiteFound=  true;
					game_index->moves.SetMove(plyCount, nmLm );
					break;
				}
			}
			if( !legalMoveWhiteFound )
				return false;

			plyCount++;
			
			if(( plyCount >= game_index->moves.plyCount )||(tempMove->black == "") )
				break;

			board->GenLegalMoves();
			board->parseMove(tempMove->black);
			tempMove->SetFenBlack(board->Fen());
			tempMove->SetBlackMoveSquares(board->currMove);
			board->SetBoard(tempMove->m_fenBlack);

			bool legalMoveBlackFound = false;
			for( unsigned char nmLm = 0; nmLm < (unsigned char)board->numLegalMoves; nmLm++ )
			{
				if( tempMove->blackMove == board->legal_moves[nmLm].mv )
				{
					legalMoveBlackFound= true;
					game_index->moves.SetMove(plyCount, nmLm );
					break;
				}
			}
			if( !legalMoveBlackFound )
				return false;

			plyCount++;


			tempMove = tempMove->pNext;
		}

		//cout<< "Testing Decompress" << endl;



	
}


bool DecompressGame(CompressedGameIndex* compressed_game_index, ChessBoard* board )
{
		//PGN::Move* tempMove = pGame->pRoot;

		int plyCount = 0;
	
	board->SetInitialPosition();
	int decompressMissed = 0;
	string fen;
	for( int i = 0; i < compressed_game_index->moves.plyCount; i++ )
		{
			if(( plyCount >= compressed_game_index->moves.plyCount ))//||(tempMove->white == "") )
			{
				break;
			}
			unsigned char indexWhite = compressed_game_index->moves.GetMove(plyCount);
			board->GenLegalMoves();
			MyChessMove mvWhite = board->legal_moves[indexWhite].mv;
			string whiteMvString = mvWhite.toString();
			whiteMvString[1]--;
			whiteMvString[3]--;
			string whiteSAN = board->EPN_Move_to_PGN_Move(whiteMvString, (mvWhite.promo ==-1?false:true), (mvWhite.promo ==-1?0:mvWhite.promo));
			
			fen = board->Fen();
			cout<< plyCount/2 + 1  << ". "<<whiteSAN << " ";//}


			board->SetBoard(fen);
			plyCount++;

			if(( plyCount >= compressed_game_index->moves.plyCount ))//||(tempMove->black == "") )
			{
				break;
			}
			unsigned char indexBlack = compressed_game_index->moves.GetMove(plyCount);
			board->GenLegalMoves();
			MyChessMove mvBlack = board->legal_moves[indexBlack].mv;
			string blackMvString = mvBlack.toString();
			blackMvString[1]--;
			blackMvString[3]--;
			string blackSAN = board->EPN_Move_to_PGN_Move(blackMvString, (mvBlack.promo ==-1?false:true), (mvBlack.promo ==-1?0:mvBlack.promo));
			fen = "";//board->Fen();
			fen = board->Fen();
			board->SetBoard(fen);
			//if( tempMove->black.find(blackSAN) != -1 ){}
			//{
				cout<<blackSAN << " ";
		//	}
			//else
		//	{
			//	cout<< "Missed: "<<blackSAN<<"|"<<tempMove->black<<endl;
		//		decompressMissed++;
		//	}

			//board->SetBoard(tempMove->m_fenBlack);
			plyCount++;
			//tempMove = tempMove->pNext;
		}
	return true;
}


bool CopyMemFile(const char* source, const char* dest, bool overwrite, ChessBoard* board)
{
	board = new ChessBoard();
    InputMemoryFile source_mf(source);//,MemoryFile::if_exists_truncate_if_not_exists_create);
    if (! source_mf.data()) 
	{
		cout<<"Error 1"<<endl;
			return false;
	}
	//fscanf(source_mf.handle(),
	const char* data = source_mf.data();
	bool before_header = true;
	bool in_header = false;
	bool before_game = false;
	GameIndex *start = new GameIndex;
	GameIndex *curr = start;
	int game_count = 0;
	HeaderElemC *h;

	int gameCount = 0;
	for(uint64 i = 0; (i < source_mf.size()-1) && (data[i] != EOF); i++ )
	{

		if( data[i] == '[' )
		{
			/**h = new HeaderElem();*/
			if( in_header == false )
			{
				
				in_header = true;
			}

			if( before_header )
			{
				curr->header_start = i;
				before_header = false;
			}
		}
		
		if( NewLine(data,i) )
		{
			
			if( i+2 < source_mf.size()-1 &&
				data[i+1] != EOF && data[i+2] != EOF)
			{
				if( NextLinePattern(data,i))
				{
					uint64 j;
					for( j = i+2; j < source_mf.size() && data[j]!= EOF; j++)
					{
						char c = data[j];
						if( ((data[j] == '[')&& before_game) || ((data[j] > '0' && data[j] < '9')&&!before_game) )
							break;
					}
				
					//cout<< " "<<data[j] <<endl;
					if( in_header && !before_game)
					{
						/**curr->elems.push_back(h);*/
						curr->header_end = i;//-(int)curr->header_start;
						curr->game_start = j;//-(int)curr->header_start;
						before_game = true;
						in_header = false;
					}
					else if( before_game && !in_header)
					{
						game_count++;
						curr->game_end = i;//-(int)curr->header_start;
						// parse header here
						gameCount++;

						curr->pNext = new GameIndex;
						curr = curr->pNext;
						before_header = true;
						before_game = false;
						in_header = false;
					}
						
					//cout<< string(data+i,data+i+1);
					continue;
				}

			}
		
			/**if (in_header)
			  curr->elems.push_back(h);*/
		}
		else
		{
			/**if( in_header )
				h->data += data[i];*/
		}
	}

	if( curr->game_end == 0 )
	{
		curr->game_end = source_mf.size()-1;

	}


	GameIndex *temp = start;
/*	while( temp )
	{
		if( temp->header_start == -1 )
			break;
		cout<< "Indexes Start Header: "<<temp->header_start<<
			"  Indexes End Header: "<< temp->header_end<<
			"  Indexes Start Game: "<< temp->game_start <<
			"  Indexes Game End: "<< temp->game_end << endl;


		temp = temp->pNext;
	}*/

	PGN::Game* firstGame = new PGN::Game;
	firstGame->game_text = "";
	for( int z = start->game_start; z <start->game_end; z++)
	{
		if( NewLine(data, z) )
		{
			firstGame->game_text += " ";
			
		}
		else
			firstGame->game_text += data[z];
	}
		int winRes = 0;
		/*for(int v = 0; v < start->elems.size(); v++ )
		{
			if( start->elems[v]->data.find("result") != -1 )
			{
				string res = GetSubstringInQuotes(start->elems[v]->data);
				if( res == "0-1" )
					winRes = 0 ;
				if( res == "1-0" )
					winRes = 1;
				if( res == "1/2-1/2" )
					winRes = 2;
			}
		}*/		
		if( !firstGame->Parse() )
		{
			cout<< "Error in first Game: "<< firstGame->parse_error << endl;
		}

		int numPly= 0;//firstGame->CountMoves();

		PGN::Move *pCountMoves = firstGame->pRoot;
		while( pCountMoves )
		{
			if( pCountMoves->white != "" )
				numPly++;
			else break;

			if( pCountMoves->black != "" )
				numPly++;
			else break;

			pCountMoves = pCountMoves->pNext;
		}
		start->moves.Allocate(numPly);

		PGN::Move * tempMove = firstGame->pRoot;

		cout<< "Testing Compress" << endl;

		board->SetInitialPosition();
		int plyCount = 0;
		while( tempMove )
		{
			if( (plyCount >= start->moves.plyCount) || (tempMove->white == "") )
				break;
			board->GenLegalMoves();
			board->parseMove(tempMove->white);
			tempMove->SetFenWhite(board->Fen());
			tempMove->SetWhiteMoveSquares(board->currMove);
			board->SetBoard(tempMove->m_fenWhite);
			for( unsigned char nmLm = 0; nmLm < (unsigned char)board->numLegalMoves; nmLm++ )
			{
				if( tempMove->whiteMove == board->legal_moves[nmLm].mv )
				{
					start->moves.SetMove(plyCount, nmLm );
					break;
				}
			}
			plyCount++;
			
			if(( plyCount >= start->moves.plyCount )||(tempMove->black == "") )
				break;

			board->GenLegalMoves();
			board->parseMove(tempMove->black);
			tempMove->SetFenBlack(board->Fen());
			tempMove->SetBlackMoveSquares(board->currMove);
			board->SetBoard(tempMove->m_fenBlack);
			for( unsigned char nmLm = 0; nmLm < (unsigned char)board->numLegalMoves; nmLm++ )
			{
				if( tempMove->blackMove == board->legal_moves[nmLm].mv )
				{
					start->moves.SetMove(plyCount, nmLm );
					break;
				}
			}

			plyCount++;


			tempMove = tempMove->pNext;
		}

		cout<< "Testing Decompress" << endl;

		tempMove = firstGame->pRoot;

		plyCount = 0;
		board->SetInitialPosition();
		while( tempMove )
		{
			if(( plyCount >= start->moves.plyCount ))//||(tempMove->white == "") )
				break;
			unsigned char indexWhite = start->moves.GetMove(plyCount);
			board->GenLegalMoves();
			MyChessMove mvWhite = board->legal_moves[indexWhite].mv;
			string whiteMvString = mvWhite.toString();
			whiteMvString[1]--;
			whiteMvString[3]--;
			string whiteSAN = board->EPN_Move_to_PGN_Move(whiteMvString, false, 0);
			//if( tempMove->white.find(whiteSAN) != -1 )
			//	cout<< plyCount/2 + 1  << ". "<<whiteSAN << " ";

			board->SetBoard(tempMove->m_fenWhite);
			plyCount++;

			if(( plyCount >= start->moves.plyCount ))//||(tempMove->black == "") )
				break;

			unsigned char indexBlack = start->moves.GetMove(plyCount);
			board->GenLegalMoves();
			MyChessMove mvBlack = board->legal_moves[indexBlack].mv;
			string blackMvString = mvBlack.toString();
			blackMvString[1]--;
			blackMvString[3]--;
			string blackSAN = board->EPN_Move_to_PGN_Move(blackMvString, false, 0);
			//if( tempMove->black.find(blackSAN) != -1 )
			//	cout<<blackSAN << " ";

			board->SetBoard(tempMove->m_fenBlack);
			plyCount++;
			tempMove = tempMove->pNext;
		}

		delete firstGame;

		cout<< "Finished Testing Compression!"<<endl;

	cout<< "Retrieving The Middle Game" <<endl;

	temp = start;

	HeaderListData headersListData;
	while( temp )
	{
		uint64 header_start = temp->header_start;
		uint64 header_end = temp->header_end;
		bool start_line = false;
		bool end_line = false;
		vector< HeaderElemC* > vpH;
		HeaderElemC *pH;
		for( uint64 h = header_start; h < header_end; h++ )
		{
			if( data[h] == '[' )
			{
				start_line = true;
				end_line = false;
				pH = new HeaderElemC;
				continue;
			}
			if( data[h] == ']' )
			{
				vpH.push_back(pH);
				end_line = true;
				start_line = false;
				continue;
			}

			if( start_line )
			{
				pH->data += data[h];
			}
		}

		for( unsigned int v = 0; v < vpH.size(); v++ )
		{
			if( ((vpH[v]->data.find("White ") != -1) || (vpH[v]->data.find("white ") != -1))&& 
				  (vpH[v]->data.find("WhiteELO ") == -1)&&(vpH[v]->data.find("WhiteElo ") == -1))
			{
				temp->hdr.wname=
				headersListData.AddPlayerName( GetSubstringInQuotes(vpH[v]->data) + "\n");
				//temp->
			}
			if( ((vpH[v]->data.find("Black ") != -1) || (vpH[v]->data.find("black ") != -1))&& 
				 (vpH[v]->data.find("BlackELO ") == -1)&&(vpH[v]->data.find("BlackElo ") == -1))
			{
				temp->hdr.bname=
				headersListData.AddPlayerName( GetSubstringInQuotes(vpH[v]->data) +"\n");
			}
			if( vpH[v]->data.find("Event ")!= -1 )
			{
				temp->hdr.eventname=
				headersListData.AddEventName( GetSubstringInQuotes(vpH[v]->data) +"\n");
		
			}
			if( vpH[v]->data.find("Site ")!= -1 )
			{
				temp->hdr.sitename=
				headersListData.AddSiteName( GetSubstringInQuotes(vpH[v]->data) +"\n");
		
			}
			if( vpH[v]->data.find("Date ")!= -1 )
			{
				temp->hdr.date=
				headersListData.AddDateName( GetSubstringInQuotes(vpH[v]->data) +"\n");
			}

			if( vpH[v]->data.find("Result ") != -1 )
			{
				string res = GetSubstringInQuotes(vpH[v]->data);
				if( res == "0-1" )
				{
				//	cout<< res << endl;
					winRes = 0 ;
				}
				if( res == "1-0" )
				{
				//	cout<< res << endl;
					winRes = 1;
				}
				if( res == "1/2-1/2" )
				{
				//	cout<< res << endl;
					winRes = 2;
				}

				temp->hdr.result = (unsigned char)winRes;
			}
		}


		temp = temp->pNext;
	}

	//NameSet playerNames;

	
	temp = start;
	int currCount = 0;
	gameCount = 0;
	while( temp )
	{
		int winRes = 0;

		gameCount++;
		if( gameCount == 1000 )
			cout<< "1000 Games"<<endl;
		if( gameCount == 10000 )
			cout <<"10000 Games" <<endl;
		if( gameCount == 50000)
			cout<< "50000 Games" << endl;
		if( gameCount == 100000 )
			cout<< "100000 Games" <<endl;
		if( gameCount == 200000 )
			cout<<"200000 Games"<<endl;
		if( gameCount == 300000 )
			cout<<"300000 Games"<<endl;
		if( gameCount == 400000 )
			cout<<"400000 Games"<<endl;
		if( gameCount == 500000 )
			cout<<"500000 Games"<<endl;
		if( gameCount == 600000 )
			cout<<"600000 Games"<<endl;
		if( gameCount == 700000 )
			cout<<"700000 Games"<<endl;
		if( gameCount == 800000 )
			cout<<"800000 Games"<<endl;
		if( gameCount == 900000 )
			cout<<"900000 Games"<<endl;
		if( gameCount == 1000000 )
			cout<<"1000000 Games"<<endl;

		PGN::Game *game = new PGN::Game;
	//	game->game_text = string( data[temp->game_start], data[temp->game_end]);
		for( int z = temp->game_start; z <temp->game_end; z++)
		{
			if( NewLine( data, z ) )
			{
				game->game_text += " ";
			}
			else
				game->game_text += data[z];
		}


		//cout<< game->game_text << endl;
		
		if( !game->Parse() )
		{
			cout<< "error parsing game: " << game->parse_error<<endl;
		}
		
		CompressGame( game, temp, gameCount, board );

		delete game;


		//firstGame->MergeGame(game);
		//delete game;


		temp = temp->pNext;
	}

	string tempStr = source;
	int dot_pos = tempStr.find(".");

	// query size of file
	// load the last compressed entry already in the file, 
	// write the next one on to the end ... or get file size, set that as the start of the 
	// next one, and simply write the data, for the name lists, they need to be loaded
	// up again, another name has to be pushed into the name list, and then it 
	// is saved again...
	// see above for any memory optimizations, perhaps too much is in memory, it takes too
	// long etc

	string outputfileName = tempStr.substr(0, dot_pos-1);
	headersListData.WriteLists(outputfileName);

	string cgf = outputfileName + "_cg.pgc3";
	string cif = outputfileName + "_ci.idx";
	FILE* compressedGameFile = fopen( cgf.c_str(), "wb");
	FILE* compressedIndexFile = fopen( cif.c_str(), "wb");

	fwrite( (void*)&gameCount, sizeof(int), 1, compressedGameFile);

	CompressedGameIndex *createC = new CompressedGameIndex;
	
	GameIndex *saveGameIndex = start;
	int offset = 0;
	createC->start = offset;
	while( saveGameIndex )
	{
		saveGameIndex->Save(compressedGameFile, offset);
		CompressedGameIndex *createC2 = new CompressedGameIndex;
		createC->pNext = createC2;
		createC->end = offset - createC->start; // num bytes
		fwrite((void*)&createC->start, sizeof(createC->start), 1, compressedIndexFile);
		fwrite((void*)&createC->end, sizeof(createC->end), 1, compressedIndexFile);
		createC2->start = offset;
		createC = createC2;
		saveGameIndex = saveGameIndex->pNext;
	}
	fclose(compressedGameFile);
	fclose(compressedIndexFile);

	

	//playerNames.WriteToFile( string("Smyslov Player Names.txt") );
	


	/*outputfileName += ".indx";
	FILE * IndexFile;
	IndexFile = fopen(outputfileName.c_str(), "wb");
	
	temp = start;
	while( temp )
	{
		GameIndex *temp2 = temp;

		fwrite((void*)&temp->header_start, sizeof(temp->header_start), 1, IndexFile);
		fwrite((void*)&temp->header_end, sizeof(temp->header_end), 1, IndexFile);
		fwrite((void*)&temp->game_start, sizeof(temp->game_start), 1, IndexFile);
		fwrite((void*)&temp->game_end, sizeof(temp->game_end), 1, IndexFile);
		temp = temp->pNext;
		delete temp2;
	}

	fclose(IndexFile);*/



	return true;
	//string str(data-3, data+3);
	//cout<<str<<endl;
    MemoryFile dest_mf(dest, overwrite ?
        MemoryFile::if_exists_truncate_if_not_exists_create :
        MemoryFile::if_exists_fail_if_not_exists_create);
    if (! dest_mf.data()) return false;
    dest_mf.resize(source_mf.size());
    if (source_mf.size() != dest_mf.size()) return false;
    std::copy(source_mf.data(), source_mf.data() + source_mf.size(),
        dest_mf.data());
    return true;
}


MemoryFile* CopyMemoryFile(MemoryFile *source_mf, const char* dest, bool overwrite, int size)
{
    if (! source_mf->data()) return false;
    MemoryFile *dest_mf = new MemoryFile(dest, overwrite ?
        MemoryFile::if_exists_truncate_if_not_exists_create :
        MemoryFile::if_exists_fail_if_not_exists_create);
    if (! dest_mf->data()) return 0;
    dest_mf->resize(size);
    if (size != dest_mf->size()) return 0;
    std::copy(source_mf->data(), source_mf->data() + size,
        dest_mf->data());
    return dest_mf;
}

bool WriteVM_PGNIndexFile(string &outputfileName, GameIndex* pStart)
{
	outputfileName += ".indx";
	FILE * IndexFile;
	IndexFile = fopen(outputfileName.c_str(), "wb");
	
	if( !IndexFile )
		return false;

	GameIndex *temp = pStart;
	while( temp )
	{
		GameIndex *temp2 = temp;

		fwrite((void*)&temp->header_start, sizeof(temp->header_start), 1, IndexFile);
		fwrite((void*)&temp->header_end, sizeof(temp->header_end), 1, IndexFile);
		fwrite((void*)&temp->game_start, sizeof(temp->game_start), 1, IndexFile);
		fwrite((void*)&temp->game_end, sizeof(temp->game_end), 1, IndexFile);
		temp = temp->pNext;
		//delete temp2;
	}

	fclose(IndexFile);
}

#include <sys/stat.h>
const char* IndexVM_PGN(const char* source, const char* dest, bool overwrite, GameIndex *start_game, GameIndex **arrayGii, InputMemoryFile *source_mf_p, int &game_count, MemPool **mempool)
{

	*mempool = new MemPool(sizeof(GameIndex));
	(*mempool)->OpenMemoryFileMemory("GameIndexData.dat", true, 2000000000); 

	// first check to see if a file exists 
	string tempStr = source;
	int dot_pos = tempStr.find(".");

	// query size of file
	// load the last compressed entry already in the file, 
	// write the next one on to the end ... or get file size, set that as the start of the 
	// next one, and simply write the data, for the name lists, they need to be loaded
	// up again, another name has to be pushed into the name list, and then it 
	// is saved again...
	// see above for any memory optimizations, perhaps too much is in memory, it takes too
	// long etc

	string outputfileName = tempStr.substr(0, dot_pos-1);
	string preloadFileName = outputfileName;
	preloadFileName += ".indx";

	// should be a VM file, this file should exist not as this file name
	/*
	struct stat   buffer;   
    if( (stat (preloadFileName.c_str(), &buffer) == 0))
	{
		cout<< "Found File"<<endl;
		FILE* pIndexFile = fopen(preloadFileName.c_str(), "rb");
		if( pIndexFile )
		{
			start_game = new GameIndex;
			
			//GameIndex *ptemp = start_game;
			int offset = 0;
			game_count = 0;
			int retVal = 0;
			while(offset < buffer.st_size )
			{
				MemBlock *mb = (*mempool)->GetBlock();

				GameIndex ptemp;

				retVal = fread((void*)&ptemp.header_start, sizeof(ptemp.header_start), 1, pIndexFile);
				offset += retVal;
				if( retVal == 0 )
				{
					break;
				}
				retVal = fread((void*)&ptemp.header_end, sizeof(ptemp.header_end), 1, pIndexFile); 
				offset += retVal;
				if( retVal == 0 )
				{
					break;
				}
				retVal = fread((void*)&ptemp.game_start, sizeof(ptemp.game_start), 1, pIndexFile);
				offset += retVal;
				if( retVal == 0 )
				{
					break;
				}
				retVal = fread((void*)&ptemp.game_end, sizeof(ptemp.game_end), 1, pIndexFile);
				offset += retVal;
				if( retVal == 0 )
				{
					break;
				}
				game_count++;

				RtlCopyMemory(mb->data, &ptemp, sizeof(GameIndex));
				
				delete mb;

				//GameIndex *pTemp2 = new GameIndex;
				//ptemp->pNext = pTemp2;
				//ptemp = pTemp2;
			}

			//delete ptemp;

			fclose(pIndexFile);
			source_mf_p = new InputMemoryFile(source);//,MemoryFile::if_exists_truncate_if_not_exists_create);
			if (! source_mf_p->data()) 
			{
				cout<<"Error 1"<<endl;
				return 0;
			}
			//arrayGi = new GameIndex*[game_count];

			//GameIndex *temp = start_game ;


			return source_mf_p->data();
		}

	}

	*/

//	board = new ChessBoard();
    source_mf_p = new InputMemoryFile(source);//,MemoryFile::if_exists_truncate_if_not_exists_create);
    if (! source_mf_p->data()) 
	{
		cout<<"Error 1"<<endl;
			return 0;
	}
	//fscanf(source_mf.handle(),
	const char* data = source_mf_p->data();
	memFileData = data;
	bool before_header = true;
	bool in_header = false;
	bool before_game = false;
	start_game = new GameIndex;
	GameIndex *curr = start_game;
	GameIndex *pStart = start_game;
	game_count = 0;
	HeaderElemC *h;

	GameIndex memoryGameIndex;
	MemBlock *mb = (*mempool)->GetBlock();

	int gameCount = 0;
	for(uint64 i = 0; (i < source_mf_p->size()-1) && (data[i] != EOF); i++ )
	{

		if( data[i] == '[' )
		{
			/**h = new HeaderElem();*/
			if( in_header == false )
			{
				
				in_header = true;
			}

			if( before_header )
			{
				curr->header_start = i;
				memoryGameIndex.header_start = i;
				before_header = false;
			}
		}
		
		if( NewLine(data,i) )
		{
			
			if( i+2 < source_mf_p->size()-1 &&
				data[i+1] != EOF && data[i+2] != EOF)
			{
				if( NextLinePattern(data,i))
				{
					uint64 j;
					for( j = i+2; j < source_mf_p->size() && data[j]!= EOF; j++)
					{
						char c = data[j];
						if( ((data[j] == '[')&& before_game) || ((data[j] > '0' && data[j] < '9')&&!before_game) )
							break;
					}
				
					//cout<< " "<<data[j] <<endl;
					if( in_header && !before_game)
					{
						/**curr->elems.push_back(h);*/
						curr->header_end = i;//-(int)curr->header_start;
						curr->game_start = j;//-(int)curr->header_start;
						memoryGameIndex.header_end = i;
						memoryGameIndex.game_start = j;
						before_game = true;
						in_header = false;
					}
					else if( before_game && !in_header)
					{
						game_count++;
						memoryGameIndex.game_end = i;
						curr->game_end = i;//-(int)curr->header_start;
						// parse header here
						gameCount++;

						RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));

						delete mb;

						mb = (*mempool)->GetBlock();
						//curr->pNext = new GameIndex;
						//curr = curr->pNext;
						before_header = true;
						before_game = false;
						in_header = false;
					}
						
					//cout<< string(data+i,data+i+1);
					continue;
				}

			}
		
			/**if (in_header)
			  curr->elems.push_back(h);*/
		}
		else
		{
			/**if( in_header )
				h->data += data[i];*/
		}
	}

	if( curr->game_end == 0 )
	{
		curr->game_end = source_mf_p->size()-1;

	}


	/*arrayGi = new GameIndex*[game_count];

	GameIndex *temp = pStart;
	for( int i = 0; i < game_count; i++ )
	{
		arrayGi[i] = temp;
		temp = temp->pNext;
	}
	*/


	/*if( !WriteVM_PGNIndexFile(outputfileName, pStart ) )
	{
		// error
	}*/

	return data;
}

bool CopyGameIndexMemPool(MemPool **src, MemPool **dst, string &error )
{
	if( *src == NULL )
	{
		error = "Lost Source Memfile";
		return false;
	}

	if( *dst != NULL )
	{
		error = "Destination For Memory Copy out of memory";
		return false;
	}

	*dst = new MemPool((*src)->GetBlockSize());

	if( !((*dst)->OpenMemoryFileMemory("IndexMemory.mem", true, (*src)->GetSizeInBytes())))
	{
		error = "Failed to resize virtual memory";
		return false;
	}

	if (! (*dst)->GetDataStart()) 
	{
		error = "virtual memory has no data";
		return false;
	}

    //dest_mf.resize(source_mf.size());
    //if (source_mf.size() != dest_mf.size()) return false;
    std::copy((*src)->GetDataStart(), (*src)->GetDataStart() + (*src)->GetSizeInBytes(),
        (*dst)->GetDataStart());

	return true;
}


bool filter_func(char c)
{
	return c==',' || c=='\'';
}
string GetHeaderData( string &matchName, GameIndex *temp, char* data )
{
	//const char* data = memFileData;// = data;
	uint64 header_start = temp->header_start;
	uint64 header_end = temp->header_end;
	bool start_line = false;
	bool end_line = false;
	string str = "";
	for( uint64 h = header_start; h < header_end; h++ )
	{
		if( data[h] == '[' )
		{
			str = "";
			start_line = true;
			end_line = false;
			//pH = new HeaderElem;
			continue;
		}
		if( data[h] == ']' )
		{
			if( str.length() > matchName.length() )
			{
				int first_space = str.find(" ");
				if( str.substr(0, matchName.length()) == matchName )
				{
					//if( first_space == matchName.length() )
					{
						string toRet =  GetSubstringInQuotes(str);
						if( toRet.length() > 0 )
							toRet.resize( std::remove_if( toRet.begin(), toRet.end(), filter_func) - toRet.begin());
						else return "empty";
						
						return toRet;
					}
				}
			}
			//vpH.push_back(pH);
			end_line = true;
			start_line = false;
			continue;
		}

		if( start_line )
		{
			str += data[h];
			//pH->data += data[h];
		}
	}

	

	return "empty";
}

void LoadCompressedLists(string source, ChessBoard *board)
{
	//HeaderList
	HeaderListData headersListData;
	
	string tempStr = source;
	int dot_pos = tempStr.find(".");

	string outputfileName = tempStr.substr(0, dot_pos-1);

	headersListData.ReadLists(outputfileName);

	string cgf = outputfileName + "_cg.pgc3";
	string cif = outputfileName + "_ci.idx";
	FILE* compressedGame = fopen(cgf.c_str(), "rb");
	FILE* compressIndx = fopen(cif.c_str(), "rb");

	int gameCount = 0;
	fread( (void*)&gameCount, sizeof(int), 1, compressedGame);

	CompressedGameIndex *cgi = new CompressedGameIndex;
	CompressedGameIndex *start = cgi;
		cgi->start = 0;
		int offset = 0;
		cgi->hdr.Read(compressedGame, offset);
		cgi->moves.Read(compressedGame, offset);
	for( int i = 0; i < gameCount; i++ )
	{
		CompressedGameIndex *cgi2 = new CompressedGameIndex;
		cgi2->start = offset;
		cgi2->hdr.Read(compressedGame, offset);
		cgi2->moves.Read(compressedGame, offset);
		cgi->pNext = cgi2;
		cgi = cgi2;
	}

	CompressedGameIndex *centerGame = start;
	for( int i = 0; i < 4; i++ )
	{
		centerGame = centerGame->pNext;
	}

	// printing the header
//	cout<< headersListData.LookupEventName(centerGame->hdr.eventname).c_str() << endl;
//	cout<< headersListData.LookupSiteName(centerGame->hdr.sitename).c_str() << endl;
//	cout<< headersListData.LookupDateName(centerGame->hdr.date).c_str() << endl;
	cout<< headersListData.LookupPlayerName(centerGame->hdr.wname).c_str() << " " <<centerGame->hdr.wname<< endl;
	cout<< headersListData.LookupPlayerName(centerGame->hdr.bname).c_str()<< " " << centerGame->hdr.bname << endl;

	// now decompress
	DecompressGame( centerGame, board);


	
	if( centerGame->hdr.result == 0 )
	{
			cout<< "1-0" << endl;
	}
	if( centerGame->hdr.result == 0 )
	{
			cout<< "0-1" << endl;
	}
	if( centerGame->hdr.result == 0 )
	{
			cout<< "1/2-1/2" << endl;
	}

	for( int i = 0; i < headersListData.playerNames.m_set.elems.size(); i++ )
	{
		if( i == centerGame->hdr.bname )
			cout<< headersListData.playerNames.m_set.elems[i].str << endl;
	}

	cout<< headersListData.playerNames.m_set.elems[centerGame->hdr.wname].str <<endl;
	cout<< headersListData.playerNames.m_set.elems[centerGame->hdr.bname].str <<endl;

}





vector<string> output;

// This is the callback function to display the select data in the table 
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	if( output.size() != 0 )
		output.clear();
  int i;
  for(i=0; i<argc; i++)
  {
//    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	output.push_back(argv[i] ? argv[i] : "NULL");
  }
 // printf("\n");
  return 0;
}

namespace PGN {


MemoryFileContainer::MemoryFileContainer()
	{
		gameIndexMemPool = 0;
		source_mf_p = 0;
		vm_compressed_games = 0;
		gameCount = 0;
		filename =  "";
		Index = 0;
	}


MemoryFileContainer::~MemoryFileContainer()
	{
		delete source_mf_p;
		delete gameIndexMemPool;
	}

bool MemoryFileContainer::Save()
{
	int findDot =  filename.find_last_of(".");
	if( findDot == -1 )
	{
		return false;
	}
	string truncFilename = filename.substr(0,findDot);
	truncFilename += "_list.dat";
	FILE* toSave = fopen(truncFilename.c_str(), "wb");
		
	if( !toSave )
	{
		return false;
	}

	fwrite( &this->gameCount, sizeof(this->gameCount), 1, toSave);
	string mempoolName = this->gameIndexMemPool->name;
	mempoolName += "\n";
	fwrite( (void*)mempoolName.c_str(), sizeof(char), mempoolName.size(), toSave);

	
}

bool MemoryFileContainer::GetHeader( void* dst, int offset )
{
	return 	RtlCopyMemory(dst, vm_compressed_games->data() + offset, sizeof(CompressedHeader));

}


char * MemFileContainerDataPageList::GetDataOffset(int idx)
{
	static int last_file = -1;
	int tempIndex = idx;
	for( int i = 0; i < this->m_pages.size(); i++ )
	{
		if( tempIndex >= m_pages[i]->gameCount )
		{
			tempIndex -= m_pages[i]->gameCount;
		}
		else
		{
			if( this->flush_last_file )
			{
				if( last_file != i )
				{
					if( (last_file != -1) && (last_file < m_pages.size()) )
					{
						m_pages[last_file]->vm_compressed_games->flush();
					}
					last_file = i;

				}
			}

			return m_pages[i]->gameIndexMemPool->GetDataOffset(tempIndex);
		}
	}

	return 0;
}

MemoryFileContainer* MemFileContainerDataPageList::GetMemPageForIndex(int idx)
{
	static int last_mem_page = -1;
	int tempIndex = idx;
	for( int i = 0; i < this->m_pages.size(); i++ )
	{
		if( tempIndex >= m_pages[i]->gameCount )
		{
			tempIndex -= m_pages[i]->gameCount;
		}
		else
		{
			if( this->flush_last_file )
			{
				if( last_mem_page != i )
				{
					if( (last_mem_page != -1) && (last_mem_page < m_pages.size()) )
					{
						m_pages[last_mem_page]->vm_compressed_games->flush();
					}
					last_mem_page = i;

				}
			}
			return m_pages[i];
		}
	}

	return 0;
}


bool MemFileContainerDataPageList::CopyIndexMemblockMemoryFromOffset( void* dest, int offset )
{
	return RtlCopyMemory(dest, GetDataOffset(offset), sizeof(VM_GameIndex));
	
}




CFileContainer::CFileContainer()
	{
		gameIndexMemPool = 0;
		source_mf_p = 0;
		vm_compressed_games = 0;
		gameCount = 0;
		filename =  "";
		Index = 0;
	}


CFileContainer::~CFileContainer()
	{
		if( source_mf_p )
			fclose( source_mf_p );
		if( gameIndexMemPool )
			delete gameIndexMemPool;
		//delete gameIndexMemPool;
		if( vm_compressed_games )
			fclose( vm_compressed_games );
	}

bool CFileContainer::Save()
{
	int findDot =  filename.find_last_of(".");
	if( findDot == -1 )
	{
		return false;
	}
	string truncFilename = filename.substr(0,findDot);
	truncFilename += "_list.dat";
	FILE* toSave = fopen(truncFilename.c_str(), "wb");
		
	if( !toSave )
	{
		return false;
	}

	fwrite( &this->gameCount, sizeof(this->gameCount), 1, toSave);
	string mempoolName = this->gameIndexMemPool->name;
	mempoolName += "\n";
	fwrite( (void*)mempoolName.c_str(), sizeof(char), mempoolName.size(), toSave);

	
}

bool CFileContainer::GetHeader( void* dst, int offset )
{
	return 0;//	RtlCopyMemory(dst, vm_compressed_games->data() + offset, sizeof(CompressedHeader));

}


char * CFileContainerDataPageList::GetDataOffset(int idx)
{
	static int last_file = -1;
	int tempIndex = idx;
	for( int i = 0; i < this->m_pages.size(); i++ )
	{
		if( tempIndex >= m_pages[i]->gameCount )
		{
			tempIndex -= m_pages[i]->gameCount;
		}
		else
		{


			return m_pages[i]->gameIndexMemPool->GetDataOffset(tempIndex);
		}
	}

	return 0;
}

CFileContainer* CFileContainerDataPageList::GetMemPageForIndex(int idx)
{
	static int last_mem_page = -1;
	int tempIndex = idx;
	for( int i = 0; i < this->m_pages.size(); i++ )
	{
		if( tempIndex >= m_pages[i]->gameCount )
		{
			tempIndex -= m_pages[i]->gameCount;
		}
		else
		{

			return m_pages[i];
		}
	}

	return 0;
}

void PgnLoad::StringFilterRAMGames(string &toMatch)
{
	temporyResCache.Clear();

	if( tree_filtered_indexes.size() > 0 )
		tree_filtered_indexes.clear();
	

	bool match1 = true;
	bool match2 = true;
	for( int i = 0; i < this->virtualGameCount; i++ )
	{
		match1 = true;
		match2 = true;
		
		int b = 0;
		//string gameText = "";
		for( int v = m_SortableIndexes[i]->m_Gi->game_start; 
			(v < m_SortableIndexes[i]->m_Gi->game_end) && (b < toMatch.length()); v++ )
		{
			
			if( NewLine( vm_data, v ) )
			{
				//continue;
			}
			else if( (toMatch[b] == ' ') && (vm_data[v] != ' '))
			{
				b++;
				v--;	
			}
			else if(  (toMatch[b] != ' ') && (vm_data[v] == ' ') )
			{
			}
			else if( (char)(vm_data[v]) != (char)(toMatch[b++]) )
			{
				match1 = false;
			}
		}

		if( match1 )// || match2 )
		{
			tree_filtered_indexes.push_back(i);
			temporyResCache += m_SortableIndexes[i]->m_Gi->hdr.result;
		}
		
	}

	filter = true;
}

void PgnLoad::IntFilterRAMGamesIntoResultCache_StartPos( vector<ResultCache*> &results, ChessBoard &brd)
{
	temporyResCache.Clear();
	if( tree_filtered_indexes.size() > 0 )
		tree_filtered_indexes.clear();

	if( SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
	{
		
		if( filter == true )
		{

			
			vector< SortableGameIndex*> temp;

			temp.reserve(m_filteredIndexes.size());

			brd.SetInitialPosition();
		


	
			int currPly = 0;
			for( int i = 0; i< this->m_filteredIndexes.size(); i++ )
			{
				currPly = 0;
				brd.SetInitialPosition();

				if( this->m_filteredIndexes[i]->m_Gi->nummoves == 0 )
					continue;

				bool match = true;

				if( match )
				{
					//tree_filtered_indexes.push_back(i);
					temp.push_back(m_filteredIndexes[i]);
					if( m_filteredIndexes[i]->m_Gi->nummoves <= 1 )
					{
						continue;
					}
					else
					{
						MyChessMove mv;
						mv.packed = m_filteredIndexes[i]->m_Gi->moves[0];
						bool promo = false;
						if( mv.to.pad == 1 )
						{
							promo = true;
							mv.promo = mv.from.pad;
						}

						string nonsan = mv.toString();
						nonsan[1]--;
						nonsan[3]--;
						string san = brd.EPN_Move_to_PGN_Move(nonsan, promo, mv.promo );

						bool foundCache = false;
						for( int pt = 0; pt < results.size(); pt++ )
						{
							if( results[pt]->san == san )
							{
								foundCache = true;
								(*results[pt]) += m_filteredIndexes[i]->m_Gi->hdr.result;
								break;
							}
						}

						if( (foundCache == false) && (results.size() < 30))
						{
							ResultCache *resNew=  new ResultCache();

							for( int a = 0; a < 8; a++ )
							{
								for( int b = 0; b < 8; b++ )
								{
									resNew->position[a][b] = brd.board[a][b];
								}
							}

							resNew->san = san;
							*resNew += m_filteredIndexes[i]->m_Gi->hdr.result;
							results.push_back( resNew);
						}			
					}
				}
			}

			m_filteredIndexes = temp;
		}
		else
		{
			if( m_filteredIndexes.size() > 0 )
				m_filteredIndexes.clear();
			


			brd.SetInitialPosition();
		


	
			int currPly = 0;
			for( int i = 0; i< this->virtualGameCount; i++ )
			{
				currPly = 0;
				brd.SetInitialPosition();

				if( this->m_SortableIndexes[i]->m_Gi->nummoves == 0 )
					continue;

				bool match = true;

				if( match )
				{
					//tree_filtered_indexes.push_back(i);
					m_filteredIndexes.push_back(m_SortableIndexes[i]);
					if( m_SortableIndexes[i]->m_Gi->nummoves <= 1 )
					{
						continue;
					}
					else
					{
						MyChessMove mv;
						mv.packed = m_SortableIndexes[i]->m_Gi->moves[0];
						bool promo = false;
						if( mv.to.pad == 1 )
						{
							promo = true;
							mv.promo = mv.from.pad;
						}

						string nonsan = mv.toString();
						nonsan[1]--;
						nonsan[3]--;
						string san = brd.EPN_Move_to_PGN_Move(nonsan, promo, mv.promo );

						bool foundCache = false;
						for( int pt = 0; pt < results.size(); pt++ )
						{
							if( results[pt]->san == san )
							{
								foundCache = true;
								(*results[pt]) += m_SortableIndexes[i]->m_Gi->hdr.result;
								break;
							}
						}

						if( (foundCache == false) && (results.size() < 30))
						{
							ResultCache *resNew=  new ResultCache();

							for( int a = 0; a < 8; a++ )
							{
								for( int b = 0; b < 8; b++ )
								{
									resNew->position[a][b] = brd.board[a][b];
								}
							}

							resNew->san = san;
							*resNew += m_SortableIndexes[i]->m_Gi->hdr.result;
							results.push_back( resNew);
						}			
					}
				}
			}
		}
	}
	else
	{
		//double accumulatingEloAverage = 0.0f;
		brd.SetInitialPosition();

		static VM_GameIndex tempVMgi;
		static GameIndex32Moves tempGi32M;

		int max_num_moves = 600;
		tempGi32M.moves = new unsigned short[max_num_moves];
		int currPly = 0;

		//memFiles_CONTAINER.flush_last_file = true;
		for( int i = 0; i< this->virtualGameCount; i++ )
		{

				

	//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
			MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(i);
			//RtlCopyMemory(&tempVMgi, memFiles_CONTAINER.GetDataOffset(i), sizeof(VM_GameIndex));
			memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, i);
			pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );

			//RtlCopyMemory(&tempGi32M.hdr, pMfc->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));
			RtlCopyMemory(&tempGi32M.nummoves, pMfc->vm_compressed_games->data() + tempVMgi.game_start, sizeof(unsigned short));
			
			if( tempGi32M.nummoves < max_num_moves )
			{
				RtlCopyMemory(tempGi32M.moves, pMfc->vm_compressed_games->data() + tempVMgi.game_start + sizeof(unsigned short), sizeof(unsigned short)*tempGi32M.nummoves );
			}
			else
			{
				continue;
			}
			currPly = 0;
			brd.SetInitialPosition();

			if( tempGi32M.nummoves == 0 )
			{
				//delete [] tempGi32M.moves;
				continue;
			}
			bool match = true;

			if( match )
			{
				 
				tree_filtered_indexes.push_back(i);
				//m_filteredIndexes.push_back(m_SortableIndexes[i]);
				if( tempGi32M.nummoves <= 1 )
				{
				//	delete [] tempGi32M.moves;
					continue;
				}
				else
				{
					MyChessMove mv;
					mv.packed = tempGi32M.moves[0];
					bool promo = false;
					if( mv.to.pad == 1 )
					{
						promo = true;
						mv.promo = mv.from.pad;
					}

					string nonsan = mv.toString();
					nonsan[1]--;
					nonsan[3]--;
					string san = brd.EPN_Move_to_PGN_Move(nonsan, promo, mv.promo );

					/*if( brd.mov != BLACK )
					{
						accumulatingEloAverage += headersListData.LookupElo(tempGi32M.hdr.blackElo);
					}
					else
					{
					}*/

					bool foundCache = false;
					for( int pt = 0; pt < results.size(); pt++ )
					{
						if( results[pt]->san == san )
						{
							foundCache = true;
							(*results[pt]) += tempGi32M.hdr.result;
							break;
						}
					}

					if( (foundCache == false) && (results.size() < 30))
					{
						ResultCache *resNew=  new ResultCache();

						for( int a = 0; a < 8; a++ )
						{
							for( int b = 0; b < 8; b++ )
							{
								resNew->position[a][b] = brd.board[a][b];
							}
						}

						resNew->san = san;
						*resNew += tempGi32M.hdr.result;
						results.push_back( resNew);
					}			
				}
			}


		}
		delete [] tempGi32M.moves;
		tempGi32M.moves = 0;

		//memFiles_CONTAINER.flush_last_file = false;
	}
	filter = true;
}

void PgnLoad::IntFilterRAMGamesIntoResultCache_CollectExtraStats(vector<unsigned short int> *toMatch, vector<ResultCacheExtraStats*> &results, int black_white_move, ChessBoard &brd)
	
{
	temporyResCache.Clear();
	if( tree_filtered_indexes.size() > 0 )
		tree_filtered_indexes.clear();

	if( SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
	{
		

		if( m_filteredIndexes.size() > 0 )
			m_filteredIndexes.clear();



		brd.SetInitialPosition();
		int plyCount = toMatch->size();
		for( int e = plyCount -1; e > -1; e-- )
		{
			MyChessMove mv;
			mv.packed = (*toMatch)[e];

			bool promo = false;
			if( mv.to.pad == 1 )
			{
				promo = true;
				mv.promo = mv.from.pad;
			}

			brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);
		}

		for( int a = 0; a < 8; a++ )
		{
			for( int b = 0; b < 8; b++ )
			{
				this->temporyResCache.position[a][b] = brd.board[a][b];
			}
		}
	
		int currPly = 0;
		for( int i = 0; i< this->virtualGameCount; i++ )
		{
			currPly = 0;
			brd.SetInitialPosition();

			if( this->m_SortableIndexes[i]->m_Gi->nummoves < plyCount )
				continue;

			bool match = true;
			for( int e = 0; e < plyCount; e++ )
			{
				MyChessMove mv;
				//mv.packed = (*toMatch)[e];
				mv.packed = this->m_SortableIndexes[i]->m_Gi->moves[e];
				bool promo = false;
				if( mv.to.pad == 1 )
				{
					promo = true;
					mv.promo = mv.from.pad;
				}

				brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);
			}
		
			for( int a = 0; a < 8; a++ )
			{
				for( int b = 0; b < 8; b++ )
				{
					if( this->temporyResCache.position[a][b] != brd.board[a][b] )
					{
						match = false;
					}
				}
			}

			if( match )
			{
				//tree_filtered_indexes.push_back(i);
				m_filteredIndexes.push_back(m_SortableIndexes[i]);
				if( m_SortableIndexes[i]->m_Gi->nummoves <= plyCount+1 )
				{
					continue;
				}
				else
				{
					MyChessMove mv;
					mv.packed = m_SortableIndexes[i]->m_Gi->moves[plyCount];
					bool promo = false;
					if( mv.to.pad == 1 )
					{
						promo = true;
						mv.promo = mv.from.pad;
					}

					string nonsan = mv.toString();
					nonsan[1]--;
					nonsan[3]--;
					string san = brd.EPN_Move_to_PGN_Move(nonsan, promo, mv.promo );

					bool foundCache = false;
					for( int pt = 0; pt < results.size(); pt++ )
					{
						if( results[pt]->san == san )
						{
							foundCache = true;
							(*results[pt]) += m_SortableIndexes[i]->m_Gi->hdr.result;
							break;
						}
					}

					if( (foundCache == false) && (results.size() < 30))
					{
						ResultCacheExtraStats *resNew=  new ResultCacheExtraStats();

						for( int a = 0; a < 8; a++ )
						{
							for( int b = 0; b < 8; b++ )
							{
								resNew->position[a][b] = brd.board[a][b];
							}
						}

						resNew->san = san;
						*resNew += m_SortableIndexes[i]->m_Gi->hdr.result;
						results.push_back( resNew);
					}			
				}
			}
		}
	}
	else
	{
		//double accumulatingEloAverage = 0.0f;
		brd.SetInitialPosition();
		int plyCount = toMatch->size();
		for( int e = plyCount -1; e > -1; e-- )
		{
			MyChessMove mv;
			mv.packed = (*toMatch)[e];

			bool promo = false;
			if( mv.to.pad == 1 )
			{
				promo = true;
				mv.promo = mv.from.pad;
			}

			brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);
		}

		for( int a = 0; a < 8; a++ )
		{
			for( int b = 0; b < 8; b++ )
			{
				this->temporyResCache.position[a][b] = brd.board[a][b];
			}
		}
	
		int currPly = 0;
		VM_GameIndex tempVMgi;
		GameIndex32Moves tempGi32M;
		int max_num_moves = 600;
		tempGi32M.moves = new unsigned short[max_num_moves];
		for( int i = 0; i< this->virtualGameCount; i++ )
		{

				

	//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
			MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(i);
			//RtlCopyMemory(&tempVMgi, memFiles_CONTAINER.GetDataOffset(i), sizeof(VM_GameIndex));
			memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, i);
			pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );
			

			//RtlCopyMemory(&tempGi32M.hdr, pMfc->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));
			RtlCopyMemory(&tempGi32M.nummoves, pMfc->vm_compressed_games->data() + tempVMgi.game_start, sizeof(unsigned short));
			
			if( tempGi32M.nummoves < max_num_moves )
			{
				RtlCopyMemory(tempGi32M.moves, pMfc->vm_compressed_games->data() + tempVMgi.game_start + sizeof(unsigned short), sizeof(unsigned short)*tempGi32M.nummoves );
			}
			else
			{
				continue;
			}
			currPly = 0;
			brd.SetInitialPosition();

			if( tempGi32M.nummoves < plyCount )
			{
				//delete [] tempGi32M.moves;
				continue;
			}

			bool match = true;
			for( int e = 0; e < plyCount; e++ )
			{
				MyChessMove mv;
				//mv.packed = (*toMatch)[e];
				mv.packed = tempGi32M.moves[e];
				bool promo = false;
				if( mv.to.pad == 1 )
				{
					promo = true;
					mv.promo = mv.from.pad;
				}

				brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);
			}
		
			for( int a = 0; a < 8; a++ )
			{
				for( int b = 0; b < 8; b++ )
				{
					if( this->temporyResCache.position[a][b] != brd.board[a][b] )
					{
						match = false;
					}
				}
			}

			if( match )
			{
				 
				tree_filtered_indexes.push_back(i);
				//m_filteredIndexes.push_back(m_SortableIndexes[i]);
				if( tempGi32M.nummoves <= plyCount+1 )
				{
					//delete [] tempGi32M.moves;
					continue;
				}
				else
				{
					MyChessMove mv;
					mv.packed = tempGi32M.moves[plyCount];
					bool promo = false;
					if( mv.to.pad == 1 )
					{
						promo = true;
						mv.promo = mv.from.pad;
					}

					string nonsan = mv.toString();
					nonsan[1]--;
					nonsan[3]--;
					string san = brd.EPN_Move_to_PGN_Move(nonsan, promo, mv.promo );

					/*if( brd.mov != BLACK )
					{
						accumulatingEloAverage += headersListData.LookupElo(tempGi32M.hdr.blackElo);
					}
					else
					{
					}*/

					bool foundCache = false;
					for( int pt = 0; pt < results.size(); pt++ )
					{
						if( results[pt]->san == san )
						{
							foundCache = true;
							(*results[pt]) += tempGi32M.hdr.result;
							break;
						}
					}

					if( (foundCache == false) && (results.size() < 30))
					{
						ResultCacheExtraStats *resNew=  new ResultCacheExtraStats();

						for( int a = 0; a < 8; a++ )
						{
							for( int b = 0; b < 8; b++ )
							{
								resNew->position[a][b] = brd.board[a][b];
							}
						}

						resNew->san = san;
						*resNew += tempGi32M.hdr.result;
						results.push_back( resNew);
					}			
				}
			}

			
		}
		delete [] tempGi32M.moves;
		tempGi32M.moves = 0;
	}
	filter = true;
}

void PgnLoad::IntFilterRAMGamesIntoResultCache_CollectExtraStats_Startpos(vector<ResultCacheExtraStats*> &results, ChessBoard &brd)
{
}

void PgnLoad::IntFilterRAMGamesIntoResultCache(vector<unsigned short int> *toMatch, vector<ResultCache*> &results, int black_white_move, ChessBoard &brd)
{
	temporyResCache.Clear();
	if( tree_filtered_indexes.size() > 0 )
		tree_filtered_indexes.clear();

	if( SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
	{
		

		if( m_filteredIndexes.size() > 0 )
			m_filteredIndexes.clear();



		brd.SetInitialPosition();
		int plyCount = toMatch->size();
		for( int e = plyCount -1; e > -1; e-- )
		{
			MyChessMove mv;
			mv.packed = (*toMatch)[e];

			bool promo = false;
			if( mv.to.pad == 1 )
			{
				promo = true;
				mv.promo = mv.from.pad;
			}

			brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);
		}

	/*	for( int cd=0; cd < vecStandardCacheResults.size(); cd++ )
		{
			bool matchFound = true;
			for( int cd_a = 0; cd_a < 8; cd_a++ )
			{
				for( int cd_b =0; cd_b < 8; cd_b++ )
				{
					if( vecStandardCacheResults[cd]->pos[cd_a][cd_b] != brd.board[cd_a][cd_b] )
					{
						matchFound = false;
						break;
					}
					
				}

				if( !matchFound )
					break;
			}

			if( matchFound )
			{
				for( int rcd = 0; rcd < vecStandardCacheResults[cd]->vecResCache.size(); rcd++ )
				{
					results.push_back( vecStandardCacheResults[cd]->vecResCache[rcd] );
				}
				return;
			}
		}*/

		StoredResultCacheVector *vecStorage = new StoredResultCacheVector();

		for( int a = 0; a < 8; a++ )
		{
			for( int b = 0; b < 8; b++ )
			{
				this->temporyResCache.position[a][b] = brd.board[a][b];
				/*vecStorage->pos[a][b] = brd.board[a][b];*/
			}
		}
	
		int currPly = 0;
		for( int i = 0; i< this->virtualGameCount; i++ )
		{
			currPly = 0;
			brd.SetInitialPosition();

			if( this->m_SortableIndexes[i]->m_Gi->nummoves < plyCount )
				continue;

			bool match = true;
			for( int e = 0; e < plyCount; e++ )
			{
				MyChessMove mv;
				//mv.packed = (*toMatch)[e];
				mv.packed = this->m_SortableIndexes[i]->m_Gi->moves[e];
				bool promo = false;
				if( mv.to.pad == 1 )
				{
					promo = true;
					mv.promo = mv.from.pad;
				}

				brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);
			}
		
			for( int a = 0; a < 8; a++ )
			{
				for( int b = 0; b < 8; b++ )
				{
					if( this->temporyResCache.position[a][b] != brd.board[a][b] )
					{
						match = false;
					}
				}
			}

			if( match )
			{
				//tree_filtered_indexes.push_back(i);
				m_filteredIndexes.push_back(m_SortableIndexes[i]);
				if( m_SortableIndexes[i]->m_Gi->nummoves <= plyCount+1 )
				{
					continue;
				}
				else
				{
					MyChessMove mv;
					mv.packed = m_SortableIndexes[i]->m_Gi->moves[plyCount];
					bool promo = false;
					if( mv.to.pad == 1 )
					{
						promo = true;
						mv.promo = mv.from.pad;
					}

					string nonsan = mv.toString();
					nonsan[1]--;
					nonsan[3]--;
					string san = brd.EPN_Move_to_PGN_Move(nonsan, promo, mv.promo );

					bool foundCache = false;
					for( int pt = 0; pt < results.size(); pt++ )
					{
						if( results[pt]->san == san )
						{
							foundCache = true;
							(*results[pt]) += m_SortableIndexes[i]->m_Gi->hdr.result;
							break;
						}
					}

					if( (foundCache == false) && (results.size() < 30))
					{
						ResultCache *resNew=  new ResultCache();

						for( int a = 0; a < 8; a++ )
						{
							for( int b = 0; b < 8; b++ )
							{
								resNew->position[a][b] = brd.board[a][b];
							}
						}

						resNew->san = san;
						*resNew += m_SortableIndexes[i]->m_Gi->hdr.result;
						results.push_back( resNew);
					}			
				}
			}
		}

		
		/*
		for( int cx = 0; cx < results.size(); cx++ )
		{
			vecStorage->vecResCache.push_back(results[cx]);
		}

		vecStandardCacheResults.push_back(vecStorage);*/
	}
	else
	{
		brd.SetInitialPosition();
		int plyCount = toMatch->size();
		for( int e = plyCount -1; e > -1; e-- )
		{
			MyChessMove mv;
			mv.packed = (*toMatch)[e];

			bool promo = false;
			if( mv.to.pad == 1 )
			{
				promo = true;
				mv.promo = mv.from.pad;
			}

			brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);
		}

	/*	for( int cd=0; cd < vecStandardCacheResults.size(); cd++ )
		{
			bool matchFound = true;
			for( int cd_a = 0; cd_a < 8; cd_a++ )
			{
				for( int cd_b =0; cd_b < 8; cd_b++ )
				{
					if( vecStandardCacheResults[cd]->pos[cd_a][cd_b] != brd.board[cd_a][cd_b] )
					{
						matchFound = false;
						break;
					}
					
				}

				if( !matchFound )
					break;
			}

			if( matchFound )
			{
				for( int rcd = 0; rcd < vecStandardCacheResults[cd]->vecResCache.size(); rcd++ )
				{
					results.push_back( vecStandardCacheResults[cd]->vecResCache[rcd] );
				}
				return;
			}
		}

		StoredResultCacheVector *vecStorage = new StoredResultCacheVector();
		*/
		for( int a = 0; a < 8; a++ )
		{
			for( int b = 0; b < 8; b++ )
			{
				this->temporyResCache.position[a][b] = brd.board[a][b];
			/*	vecStorage->pos[a][b] = brd.board[a][b];*/
			}
		}
	
		int currPly = 0;
		
		VM_GameIndex tempVMgi;
		GameIndex32Moves tempGi32M;
		int max_num_moves = 600;
		tempGi32M.moves = new unsigned short[max_num_moves];
		//memFiles_CONTAINER.flush_last_file = true;
		for( int i = 0; i< this->virtualGameCount; i++ )
		{

				

	//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
			MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(i);
			//RtlCopyMemory(&tempVMgi, memFiles_CONTAINER.GetDataOffset(i), sizeof(VM_GameIndex));

			memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, i);
			pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );			

			//RtlCopyMemory(&tempGi32M.hdr, pMfc->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));
			RtlCopyMemory(&tempGi32M.nummoves, pMfc->vm_compressed_games->data() + tempVMgi.game_start, sizeof(unsigned short));
			
			if( tempGi32M.nummoves < max_num_moves )
			{
				RtlCopyMemory(tempGi32M.moves, pMfc->vm_compressed_games->data() + tempVMgi.game_start + sizeof(unsigned short), sizeof(unsigned short)*tempGi32M.nummoves );
			}
			else
			{
				continue;
			}
			currPly = 0;
			brd.SetInitialPosition();

			if( tempGi32M.nummoves < plyCount )
			{
				//delete [] tempGi32M.moves;
				//tempGi32M.moves = 0;
				continue;
			}

			bool match = true;
			for( int e = 0; e < plyCount; e++ )
			{
				MyChessMove mv;
				//mv.packed = (*toMatch)[e];
				mv.packed = tempGi32M.moves[e];
				bool promo = false;
				if( mv.to.pad == 1 )
				{
					promo = true;
					mv.promo = mv.from.pad;
				}

				brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);
			}
		
			for( int a = 0; a < 8; a++ )
			{
				for( int b = 0; b < 8; b++ )
				{
					if( this->temporyResCache.position[a][b] != brd.board[a][b] )
					{
						match = false;
					}
				}
			}

			if( match )
			{
				tree_filtered_indexes.push_back(i);
				//m_filteredIndexes.push_back(m_SortableIndexes[i]);
				if( tempGi32M.nummoves <= plyCount+1 )
				{

					continue;
				}
				else
				{
					MyChessMove mv;
					mv.packed = tempGi32M.moves[plyCount];
					bool promo = false;
					if( mv.to.pad == 1 )
					{
						promo = true;
						mv.promo = mv.from.pad;
					}

					string nonsan = mv.toString();
					nonsan[1]--;
					nonsan[3]--;
					string san = brd.EPN_Move_to_PGN_Move(nonsan, promo, mv.promo );

					bool foundCache = false;
					for( int pt = 0; pt < results.size(); pt++ )
					{
						if( results[pt]->san == san )
						{
							foundCache = true;
							(*results[pt]) += tempGi32M.hdr.result;
							break;
						}
					}

					if( (foundCache == false) && (results.size() < 30))
					{
						ResultCache *resNew=  new ResultCache();

						for( int a = 0; a < 8; a++ )
						{
							for( int b = 0; b < 8; b++ )
							{
								resNew->position[a][b] = brd.board[a][b];
							}
						}

						resNew->san = san;
						*resNew += tempGi32M.hdr.result;
						results.push_back( resNew);
					}			
				}
			}


		}

	/*	for( int cx = 0; cx < results.size(); cx++ )
		{
			vecStorage->vecResCache.push_back(results[cx]);
		}

		vecStandardCacheResults.push_back(vecStorage);*/

		delete [] tempGi32M.moves;
		tempGi32M.moves = 0;
		//memFiles_CONTAINER.flush_last_file = false;
	}
	filter = true;
}

void PgnLoad::IntFilter_FilteredRAMGamesIntoResultCache_StartPos( vector<ResultCache*> &results, ChessBoard &brd)
{
}


void PgnLoad::IntFilter_FilteredRAMGamesIntoResultCache(vector<unsigned short int> *toMatch, vector<ResultCache*> &results, int black_white_move, ChessBoard &brd)
{
	temporyResCache.Clear();



	if( SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
	{
		
		vector<SortableGameIndex*> temp;
		temp.reserve(m_filteredIndexes.size());
	/*	if( m_filteredIndexes.size() > 0 )
			m_filteredIndexes.clear();

		if( tree_filtered_indexes.size() > 0 )
			tree_filtered_indexes.clear();*/

		brd.SetInitialPosition();
		int plyCount = toMatch->size();
		for( int e = plyCount -1; e > -1; e-- )
		{
			MyChessMove mv;
			mv.packed = (*toMatch)[e];

			bool promo = false;
			if( mv.to.pad == 1 )
			{
				promo = true;
				mv.promo = mv.from.pad;
			}

			brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);
		}

	/*	for( int cd=0; cd < vecStandardCacheResults.size(); cd++ )
		{
			bool matchFound = true;
			for( int cd_a = 0; cd_a < 8; cd_a++ )
			{
				for( int cd_b =0; cd_b < 8; cd_b++ )
				{
					if( vecStandardCacheResults[cd]->pos[cd_a][cd_b] != brd.board[cd_a][cd_b] )
					{
						matchFound = false;
						break;
					}
					
				}

				if( !matchFound )
					break;
			}

			if( matchFound )
			{
				for( int rcd = 0; rcd < vecStandardCacheResults[cd]->vecResCache.size(); rcd++ )
				{
					results.push_back( vecStandardCacheResults[cd]->vecResCache[rcd] );
				}
				return;
			}
		}

		StoredResultCacheVector *vecStorage = new StoredResultCacheVector();
		*/
		for( int a = 0; a < 8; a++ )
		{
			for( int b = 0; b < 8; b++ )
			{
				this->temporyResCache.position[a][b] = brd.board[a][b];
				/*vecStorage->pos[a][b] = brd.board[a][b];*/
			}
		}
	
		int currPly = 0;
		for( int i = 0; i< this->m_filteredIndexes.size(); i++ )
		{
			currPly = 0;
			brd.SetInitialPosition();

			if( this->m_filteredIndexes[i]->m_Gi->nummoves < plyCount )
				continue;

			bool match = true;
			for( int e = 0; e < plyCount; e++ )
			{
				MyChessMove mv;
				//mv.packed = (*toMatch)[e];
				mv.packed = this->m_filteredIndexes[i]->m_Gi->moves[e];
				bool promo = false;
				if( mv.to.pad == 1 )
				{
					promo = true;
					mv.promo = mv.from.pad;
				}

				brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);
			}
		
			for( int a = 0; a < 8; a++ )
			{
				for( int b = 0; b < 8; b++ )
				{
					if( this->temporyResCache.position[a][b] != brd.board[a][b] )
					{
						match = false;
					}
				}
			}

			if( match )
			{
				//tree_filtered_indexes.push_back(i);
				temp.push_back(m_filteredIndexes[i]);
				if( m_filteredIndexes[i]->m_Gi->nummoves <= plyCount+1 )
				{
					continue;
				}
				else
				{
					MyChessMove mv;
					mv.packed = m_filteredIndexes[i]->m_Gi->moves[plyCount];
					bool promo = false;
					if( mv.to.pad == 1 )
					{
						promo = true;
						mv.promo = mv.from.pad;
					}

					string nonsan = mv.toString();
					nonsan[1]--;
					nonsan[3]--;
					string san = brd.EPN_Move_to_PGN_Move(nonsan, promo, mv.promo );

					bool foundCache = false;
					for( int pt = 0; pt < results.size(); pt++ )
					{
						if( results[pt]->san == san )
						{
							foundCache = true;
							(*results[pt]) += m_filteredIndexes[i]->m_Gi->hdr.result;
							break;
						}
					}

					if( (foundCache == false) && (results.size() < 30))
					{
						ResultCache *resNew=  new ResultCache();

						for( int a = 0; a < 8; a++ )
						{
							for( int b = 0; b < 8; b++ )
							{
								resNew->position[a][b] = brd.board[a][b];
							}
						}

						resNew->san = san;
						*resNew += m_filteredIndexes[i]->m_Gi->hdr.result;
						results.push_back( resNew);
					}			
				}
			}
		}
	/*	for( int cx = 0; cx < results.size(); cx++ )
		{
			vecStorage->vecResCache.push_back(results[cx]);
		}

		vecStandardCacheResults.push_back(vecStorage);*/

		m_filteredIndexes = temp;
	}
	else
	{
		brd.SetInitialPosition();
		int plyCount = toMatch->size();

		//vector< int > temp;
		Filter *curr_filter = this->m_filterStage.m_filters[this->m_filterStage.m_filters.size()-1];
		tree_filtered_indexes.clear();

		for( int e = plyCount -1; e > -1; e-- )
		{
			MyChessMove mv;
			mv.packed = (*toMatch)[e];

			bool promo = false;
			if( mv.to.pad == 1 )
			{
				promo = true;
				mv.promo = mv.from.pad;
			}

			brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);
		}

/*		for( int cd=0; cd < vecStandardCacheResults.size(); cd++ )
		{
			bool matchFound = true;
			for( int cd_a = 0; cd_a < 8; cd_a++ )
			{
				for( int cd_b =0; cd_b < 8; cd_b++ )
				{
					if( vecStandardCacheResults[cd]->pos[cd_a][cd_b] != brd.board[cd_a][cd_b] )
					{
						matchFound = false;
						break;
					}
					
				}

				if( !matchFound )
					break;
			}

			if( matchFound )
			{
				for( int rcd = 0; rcd < vecStandardCacheResults[cd]->vecResCache.size(); rcd++ )
				{
					results.push_back( vecStandardCacheResults[cd]->vecResCache[rcd] );
				}
				return;
			}
		}

		StoredResultCacheVector *vecStorage = new StoredResultCacheVector();
		*/
		for( int a = 0; a < 8; a++ )
		{
			for( int b = 0; b < 8; b++ )
			{
				this->temporyResCache.position[a][b] = brd.board[a][b];
				/*vecStorage->pos[a][b] = brd.board[a][b];*/
			}
		}
	
		int currPly = 0;
		
		VM_GameIndex tempVMgi;
		GameIndex32Moves tempGi32M;
		int max_num_moves = 600;
		tempGi32M.moves = new unsigned short[max_num_moves];

		for( int i = 0; i< curr_filter->indexes.size(); i++ )
		{

				

	//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
			MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(curr_filter->indexes[i]);
			//RtlCopyMemory(&tempVMgi, memFiles_CONTAINER.GetDataOffset(curr_filter->indexes[i]), sizeof(VM_GameIndex));
			memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, i);
			pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );
			

			//RtlCopyMemory(&tempGi32M.hdr, pMfc->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));
			RtlCopyMemory(&tempGi32M.nummoves, pMfc->vm_compressed_games->data() + tempVMgi.game_start, sizeof(unsigned short));
			
			if( tempGi32M.nummoves < max_num_moves )
			{
				RtlCopyMemory(tempGi32M.moves, pMfc->vm_compressed_games->data() + tempVMgi.game_start + sizeof(unsigned short), sizeof(unsigned short)*tempGi32M.nummoves );
			}
			else
			{
				continue;
			}
			currPly = 0;
			brd.SetInitialPosition();

			if( tempGi32M.nummoves < plyCount )
			{
				//delete [] tempGi32M.moves;
				//tempGi32M.moves = 0;
				continue;
			}

			bool match = true;
			for( int e = 0; e < plyCount; e++ )
			{
				MyChessMove mv;
				//mv.packed = (*toMatch)[e];
				mv.packed = tempGi32M.moves[e];
				bool promo = false;
				if( mv.to.pad == 1 )
				{
					promo = true;
					mv.promo = mv.from.pad;
				}

				brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);
			}
		
			for( int a = 0; a < 8; a++ )
			{
				for( int b = 0; b < 8; b++ )
				{
					if( this->temporyResCache.position[a][b] != brd.board[a][b] )
					{
						match = false;
					}
				}
			}

			if( match )
			{
				tree_filtered_indexes.push_back(curr_filter->indexes[i]);
				//m_filteredIndexes.push_back(m_SortableIndexes[i]);
				if( tempGi32M.nummoves <= plyCount+1 )
				{

					continue;
				}
				else
				{
					MyChessMove mv;
					mv.packed = tempGi32M.moves[plyCount];
					bool promo = false;
					if( mv.to.pad == 1 )
					{
						promo = true;
						mv.promo = mv.from.pad;
					}

					string nonsan = mv.toString();
					nonsan[1]--;
					nonsan[3]--;
					string san = brd.EPN_Move_to_PGN_Move(nonsan, promo, mv.promo );

					bool foundCache = false;
					for( int pt = 0; pt < results.size(); pt++ )
					{
						if( results[pt]->san == san )
						{
							foundCache = true;
							(*results[pt]) += tempGi32M.hdr.result;
							break;
						}
					}

					if( (foundCache == false) && (results.size() < 30))
					{
						ResultCache *resNew=  new ResultCache();

						for( int a = 0; a < 8; a++ )
						{
							for( int b = 0; b < 8; b++ )
							{
								resNew->position[a][b] = brd.board[a][b];
							}
						}

						resNew->san = san;
						*resNew += tempGi32M.hdr.result;
						results.push_back( resNew);
					}			
				}
			}


		}

	/*	for( int cx = 0; cx < results.size(); cx++ )
		{
			vecStorage->vecResCache.push_back(results[cx]);
		}

		vecStandardCacheResults.push_back(vecStorage);*/

		delete [] tempGi32M.moves;
		tempGi32M.moves = 0;

	}

	
	filter = true;
}

void PgnLoad::IntFilter_FilteredRAMGamesIntoResultCache_CollectExtraStats(vector<unsigned short int> *toMatch, vector<ResultCacheExtraStats*> &results, int black_white_move, ChessBoard &brd)
{
}

void PgnLoad::IntFilter_FilteredRAMGamesIntoResultCache_CollectExtraStats_Startpos(vector<ResultCacheExtraStats*> &results, ChessBoard &brd)
{
}

void PgnLoad::PositionSearch(ChessBoard &brd, int ply_count)
{
	temporyResCache.Clear();
	if( tree_filtered_indexes.size() > 0 )
		tree_filtered_indexes.clear();

	bool search_entire_game = false;
	int plyCount = ply_count; // compatibility
	if( ply_count == -1 )
	{
		search_entire_game = true;
	}

	if( SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
	{
		if( m_filteredIndexes.size() > 0 )
			m_filteredIndexes.clear();

		int currPly = 0;
		for( int i = 0; i< this->virtualGameCount; i++ )
		{
			currPly = 0;
			brd.SetInitialPosition();


			if( search_entire_game )
			{
				plyCount =this->m_SortableIndexes[i]->m_Gi->nummoves; 
			}
			else if( (this->m_SortableIndexes[i]->m_Gi->nummoves < plyCount) )
			{
				continue;
			}

			bool match = true;
			for( int e = 0; e < plyCount; e++ )
			{
				MyChessMove mv;
				//mv.packed = (*toMatch)[e];
				mv.packed = this->m_SortableIndexes[i]->m_Gi->moves[e];
				bool promo = false;
				if( mv.to.pad == 1 )
				{
					promo = true;
					mv.promo = mv.from.pad;
				}

				brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);

				int missCount = 0;
				match = true;
				for( int a = 0; a < 8; a++ )
				{
					for( int b = 0; b < 8; b++ )
					{
						if( this->m_positionSearch.position[a][b] != brd.board[a][b] )
						{
							match = false;
							missCount++;
						}
					}
				}

				if( match )
				{
					break;
				}

				
			}
		
			if( match )
			{
				m_filteredIndexes.push_back(m_SortableIndexes[i]);
			}
		}
	}
	else
	{
		int currPly = 0;
		
		VM_GameIndex tempVMgi;
		GameIndex32Moves tempGi32M;
		int max_num_moves = 600;
		tempGi32M.moves = new unsigned short[max_num_moves];
		for( int i = 0; i< this->virtualGameCount; i++ )
		{

			MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(i);

			memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, i);
			pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );			

			RtlCopyMemory(&tempGi32M.nummoves, pMfc->vm_compressed_games->data() + tempVMgi.game_start, sizeof(unsigned short));

			if( tempGi32M.nummoves < max_num_moves )
			{
				RtlCopyMemory(tempGi32M.moves, pMfc->vm_compressed_games->data() + tempVMgi.game_start + sizeof(unsigned short), sizeof(unsigned short)*tempGi32M.nummoves );
			}
			else
			{
				continue;
			}
			currPly = 0;
			brd.SetInitialPosition();

			if( search_entire_game )
			{
				plyCount =tempGi32M.nummoves; 
			}
			else if( (tempGi32M.nummoves < plyCount) )
			{
				continue;
			}

			bool match = true;
			for( int e = 0; e < plyCount; e++ )
			{
				MyChessMove mv;
				mv.packed = tempGi32M.moves[e];
				bool promo = false;

				if( mv.to.pad == 1 )
				{
					promo = true;
					mv.promo = mv.from.pad;
				}

				brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);

				int missCount = 0;
				match = true;
				for( int a = 0; a < 8; a++ )
				{
					for( int b = 0; b < 8; b++ )
					{
						if( this->m_positionSearch.position[a][b] != brd.board[a][b] )
						{
							match = false;
							missCount++;
						}
					}
				}

				if( match )
				{
					break;
				}

				
			}
		

			if( match )
			{
				tree_filtered_indexes.push_back(i);
			}
		}

		delete [] tempGi32M.moves;
		tempGi32M.moves = 0;
	}
	filter = true;
}

void PgnLoad::PositionSearchSimilar(ChessBoard &brd, int ply_count)
{
	temporyResCache.Clear();
	if( tree_filtered_indexes.size() > 0 )
		tree_filtered_indexes.clear();

	bool search_entire_game = false;
	int plyCount = ply_count; // compatibility
	if( ply_count == -1 )
	{
		search_entire_game = true;
	}

	if( SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
	{
		if( m_filteredIndexes.size() > 0 )
			m_filteredIndexes.clear();

		int currPly = 0;
		for( int i = 0; i< this->virtualGameCount; i++ )
		{
			currPly = 0;
			brd.SetInitialPosition();


			if( search_entire_game )
			{
				plyCount =this->m_SortableIndexes[i]->m_Gi->nummoves; 
			}
			else if( (this->m_SortableIndexes[i]->m_Gi->nummoves < plyCount) )
			{
				continue;
			}

			bool match = true;
			for( int e = 0; e < plyCount; e++ )
			{
				MyChessMove mv;
				//mv.packed = (*toMatch)[e];
				mv.packed = this->m_SortableIndexes[i]->m_Gi->moves[e];
				bool promo = false;
				if( mv.to.pad == 1 )
				{
					promo = true;
					mv.promo = mv.from.pad;
				}

				brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);

				int missCount = 0;
				match = true;
				for( int a = 0; a < 8; a++ )
				{
					for( int b = 0; b < 8; b++ )
					{
						if( this->m_positionSearch.position[a][b] != brd.board[a][b] )
						{
							match = false;
							missCount++;
						}
					}
				}

				if( match )
				{
					break;
				}
			}
		
			if( match )
			{
				m_filteredIndexes.push_back(m_SortableIndexes[i]);
			}
		}
	}
	else
	{
		int currPly = 0;
		
		VM_GameIndex tempVMgi;
		GameIndex32Moves tempGi32M;
		int max_num_moves = 600;
		tempGi32M.moves = new unsigned short[max_num_moves];
		for( int i = 0; i< this->virtualGameCount; i++ )
		{

			MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(i);

			memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, i);
			pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );			

			RtlCopyMemory(&tempGi32M.nummoves, pMfc->vm_compressed_games->data() + tempVMgi.game_start, sizeof(unsigned short));

			if( tempGi32M.nummoves < max_num_moves )
			{
				RtlCopyMemory(tempGi32M.moves, pMfc->vm_compressed_games->data() + tempVMgi.game_start + sizeof(unsigned short), sizeof(unsigned short)*tempGi32M.nummoves );
			}
			else
			{
				continue;
			}
			currPly = 0;
			brd.SetInitialPosition();

			if( search_entire_game )
			{
				plyCount =tempGi32M.nummoves; 
			}
			else if( (tempGi32M.nummoves < plyCount) )
			{
				continue;
			}

			bool match = true;
			for( int e = 0; e < plyCount; e++ )
			{
				MyChessMove mv;
				mv.packed = tempGi32M.moves[e];
				bool promo = false;

				if( mv.to.pad == 1 )
				{
					promo = true;
					mv.promo = mv.from.pad;
				}

				brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);

				int missCount = 0;
				match = true;
				for( int a = 0; a < 8; a++ )
				{
					for( int b = 0; b < 8; b++ )
					{
						if( this->m_positionSearch.position[a][b] != brd.board[a][b] )
						{
							match = false;
							missCount++;
						}
					}
				}

				if( match )
				{
					break;
				}

			}
		

			if( match )
			{
				tree_filtered_indexes.push_back(i);
			}
		}

		delete [] tempGi32M.moves;
		tempGi32M.moves = 0;
	}
	filter = true;
}

void PgnLoad::PositionSearchPawns(ChessBoard &brd, int ply_count)
{
	temporyResCache.Clear();
	if( tree_filtered_indexes.size() > 0 )
		tree_filtered_indexes.clear();

	bool search_entire_game = false;
	int plyCount = ply_count; // compatibility
	if( ply_count == -1 )
	{
		search_entire_game = true;
	}

	if( SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
	{
		if( m_filteredIndexes.size() > 0 )
			m_filteredIndexes.clear();

		int currPly = 0;
		for( int i = 0; i< this->virtualGameCount; i++ )
		{
			currPly = 0;
			brd.SetInitialPosition();


			if( search_entire_game )
			{
				plyCount =this->m_SortableIndexes[i]->m_Gi->nummoves; 
			}
			else if( (this->m_SortableIndexes[i]->m_Gi->nummoves < plyCount) )
			{
				continue;
			}

			bool match = true;
			for( int e = 0; e < plyCount; e++ )
			{
				MyChessMove mv;
				//mv.packed = (*toMatch)[e];
				mv.packed = this->m_SortableIndexes[i]->m_Gi->moves[e];
				bool promo = false;
				if( mv.to.pad == 1 )
				{
					promo = true;
					mv.promo = mv.from.pad;
				}

				brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);

				int missCount = 0;
				match = true;
				for( int a = 0; a < 8; a++ )
				{
					for( int b = 0; b < 8; b++ )
					{
						if( (brd.board[a][b] == ChessBoard::wPawn) ||
							(brd.board[a][b] == ChessBoard::bPawn) )
						{

							if( this->m_positionSearch.position[a][b] != brd.board[a][b] )
							{
								match = false;
								missCount++;
							}
						}
					}
				}

				if( match )
				{
					break;
				}

				
			}
		
			if( match )
			{
				m_filteredIndexes.push_back(m_SortableIndexes[i]);
			}
		}
	}
	else
	{
		int currPly = 0;
		
		VM_GameIndex tempVMgi;
		GameIndex32Moves tempGi32M;
		int max_num_moves = 600;
		tempGi32M.moves = new unsigned short[max_num_moves];
		for( int i = 0; i< this->virtualGameCount; i++ )
		{

			MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(i);

			memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, i);
			pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );			

			RtlCopyMemory(&tempGi32M.nummoves, pMfc->vm_compressed_games->data() + tempVMgi.game_start, sizeof(unsigned short));

			if( tempGi32M.nummoves < max_num_moves )
			{
				RtlCopyMemory(tempGi32M.moves, pMfc->vm_compressed_games->data() + tempVMgi.game_start + sizeof(unsigned short), sizeof(unsigned short)*tempGi32M.nummoves );
			}
			else
			{
				continue;
			}
			currPly = 0;
			brd.SetInitialPosition();

			if( search_entire_game )
			{
				plyCount =tempGi32M.nummoves; 
			}
			else if( (tempGi32M.nummoves < plyCount) )
			{
				continue;
			}

			bool match = true;
			for( int e = 0; e < plyCount; e++ )
			{
				MyChessMove mv;
				mv.packed = tempGi32M.moves[e];
				bool promo = false;

				if( mv.to.pad == 1 )
				{
					promo = true;
					mv.promo = mv.from.pad;
				}

				brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);

				int missCount = 0;
				match = true;
				for( int a = 0; a < 8; a++ )
				{
					for( int b = 0; b < 8; b++ )
					{
						if( (brd.board[a][b] == ChessBoard::wPawn) ||
							(brd.board[a][b] == ChessBoard::bPawn) )
						{
							if( this->m_positionSearch.position[a][b] != brd.board[a][b] )
							{
								match = false;
								missCount++;
							}
						}

					}
				}

				if( match )
				{
					break;
				}

				
			}
		

			if( match )
			{
				tree_filtered_indexes.push_back(i);
			}
		}

		delete [] tempGi32M.moves;
		tempGi32M.moves = 0;
	}
	filter = true;
}

void PgnLoad::PositionSearchPawnsSimilar(ChessBoard &brd, int ply_count)
{
	temporyResCache.Clear();
	if( tree_filtered_indexes.size() > 0 )
		tree_filtered_indexes.clear();

	bool search_entire_game = false;
	int plyCount = ply_count; // compatibility
	if( ply_count == -1 )
	{
		search_entire_game = true;
	}

	if( SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
	{
		if( m_filteredIndexes.size() > 0 )
			m_filteredIndexes.clear();

		int currPly = 0;
		for( int i = 0; i< this->virtualGameCount; i++ )
		{
			currPly = 0;
			brd.SetInitialPosition();


			if( search_entire_game )
			{
				plyCount =this->m_SortableIndexes[i]->m_Gi->nummoves; 
			}
			else if( (this->m_SortableIndexes[i]->m_Gi->nummoves < plyCount) )
			{
				continue;
			}

			bool match = true;
			for( int e = 0; e < plyCount; e++ )
			{
				MyChessMove mv;
				//mv.packed = (*toMatch)[e];
				mv.packed = this->m_SortableIndexes[i]->m_Gi->moves[e];
				bool promo = false;
				if( mv.to.pad == 1 )
				{
					promo = true;
					mv.promo = mv.from.pad;
				}

				brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);

				int missCount = 0;
				match = true;
				for( int a = 0; a < 8; a++ )
				{
					for( int b = 0; b < 8; b++ )
					{
						if( this->m_positionSearch.position[a][b] != brd.board[a][b] )
						{
							match = false;
							missCount++;
						}
					}
				}

				if( match )
				{
					break;
				}
			}
		
			if( match )
			{
				m_filteredIndexes.push_back(m_SortableIndexes[i]);
			}
		}
	}
	else
	{
		int currPly = 0;
		
		VM_GameIndex tempVMgi;
		GameIndex32Moves tempGi32M;
		int max_num_moves = 600;
		tempGi32M.moves = new unsigned short[max_num_moves];
		for( int i = 0; i< this->virtualGameCount; i++ )
		{

			MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(i);

			memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, i);
			pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );			

			RtlCopyMemory(&tempGi32M.nummoves, pMfc->vm_compressed_games->data() + tempVMgi.game_start, sizeof(unsigned short));

			if( tempGi32M.nummoves < max_num_moves )
			{
				RtlCopyMemory(tempGi32M.moves, pMfc->vm_compressed_games->data() + tempVMgi.game_start + sizeof(unsigned short), sizeof(unsigned short)*tempGi32M.nummoves );
			}
			else
			{
				continue;
			}
			currPly = 0;
			brd.SetInitialPosition();

			if( search_entire_game )
			{
				plyCount =tempGi32M.nummoves; 
			}
			else if( (tempGi32M.nummoves < plyCount) )
			{
				continue;
			}

			bool match = true;
			for( int e = 0; e < plyCount; e++ )
			{
				MyChessMove mv;
				mv.packed = tempGi32M.moves[e];
				bool promo = false;

				if( mv.to.pad == 1 )
				{
					promo = true;
					mv.promo = mv.from.pad;
				}

				brd.makemove2(mv.from.file, mv.from.rank, mv.to.file, mv.to.rank, promo, mv.promo);

				int missCount = 0;
				match = true;
				for( int a = 0; a < 8; a++ )
				{
					for( int b = 0; b < 8; b++ )
					{
						if( this->m_positionSearch.position[a][b] != brd.board[a][b] )
						{
							match = false;
							missCount++;
						}
					}
				}

				if( match )
				{
					break;
				}
			}
		

			if( match )
			{
				tree_filtered_indexes.push_back(i);
			}
		}

		delete [] tempGi32M.moves;
		tempGi32M.moves = 0;
	}
	filter = true;
}


void PgnLoad::StringFilterRAMGamesIntoResultCache(string &toMatch, vector<ResultCache*> &results, int black_white_move)
{
	temporyResCache.Clear();

	if( m_filteredIndexes.size() > 0 )
		m_filteredIndexes.clear();
	
	if( tree_filtered_indexes.size() > 0 )
		tree_filtered_indexes.clear();
	

	bool match1 = true;
	bool match2 = true;
	for( int i = 0; i < this->virtualGameCount; i++ )
	{
		match1 = true;
		match2 = true;

		if( (((int)m_SortableIndexes[i]->m_Gi->game_end - (int)m_SortableIndexes[i]->m_Gi->game_start) < (int)100) ||
			(((int)m_SortableIndexes[i]->m_Gi->game_end - (int)m_SortableIndexes[i]->m_Gi->game_start) < (int)toMatch.length()) )
		{
		}
		else
		{
		
			int b = 0;
			//string gameText = "";
			int v=0;
			for( v = m_SortableIndexes[i]->m_Gi->game_start; 
				(v < m_SortableIndexes[i]->m_Gi->game_end) && (b < toMatch.length()); v++ )
			{
			
				if( NewLine( vm_data, v ) )
				{
					//continue;
				}
				else if( (toMatch[b] == ' ') && (vm_data[v] != ' '))
				{
					b++;
					v--;	
				}
				else if(  (toMatch[b] != ' ') && (vm_data[v] == ' ') )
				{
				}
				else if( (char)(vm_data[v]) != (char)(toMatch[b++]) )
				{
					match1 = false;
				}
			}

			if( match1 )// || match2 )
			{
				//tree_filtered_indexes.push_back(i);
				m_filteredIndexes.push_back(m_SortableIndexes[i]);
				temporyResCache += m_SortableIndexes[i]->m_Gi->hdr.result;

				string nextMove="";
			
				if( vm_data[v] == ' ' || NewLine(vm_data,v) )
					v++;

				if( !(v < m_SortableIndexes[i]->m_Gi->game_end) )
					continue;

				if( black_white_move == WHITE )
				{
					while( (vm_data[v] != '.') && !NewLine(vm_data,v) && (v < m_SortableIndexes[i]->m_Gi->game_end) ) 
					{
						v++;
					}

					if( (vm_data[v] == '.') || (vm_data[v] == ' ') || NewLine(vm_data,v) )
					{
						v++;
						if( (vm_data[v] == ' ') || NewLine(vm_data,v) )
							v++;
					}
					else
					{
						continue;
					}
				}
				while( (vm_data[v] != ' ') && !NewLine(vm_data,v) && (v < m_SortableIndexes[i]->m_Gi->game_end) )
				{
					nextMove += vm_data[v++];
				}

				bool bFoundRes = false;
				for( int cc = 0; cc < results.size(); cc++ )
				{
					if( results[cc]->san == nextMove )
					{
						bFoundRes = true;
						*(results[cc]) += m_SortableIndexes[i]->m_Gi->hdr.result;
						break;
					}
				}
				if( (bFoundRes == false) && !(results.size() > 30) )
				{
					ResultCache *newRes = new ResultCache;
					*newRes += m_SortableIndexes[i]->m_Gi->hdr.result;
					newRes->san = nextMove;
					results.push_back(newRes);
				}	
			}
		}
	}

	filter = true;
}

bool PgnLoad::FindName( string &name )
{
	return true;
}

bool PgnLoad::FindName( string &name, int id )
{


	if( /*filter !=*/ true )
	{
		if( tree_filtered_indexes.size() > 0 )
			tree_filtered_indexes.clear();

		Filter *nm = new NameFilter();
		((NameFilter*)nm)->nameMin = name;
		((NameFilter*)nm)->nameMax = name;
		((NameFilter*)nm)->color = 2;

		m_filterStage.m_filters.push_back(nm);

		if( SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
		{
			vector<SortableGameIndex*> temp;
			temp.reserve(m_filteredIndexes.size());

			for( int i = 0; i < m_filteredIndexes.size(); i++ )
			{
				if( (m_filteredIndexes[i]->m_Gi->hdr.wname == id) || (m_filteredIndexes[i]->m_Gi->hdr.bname == id) )
				{
					temp.push_back( m_filteredIndexes[i] );
				}
			}

			m_filteredIndexes = temp;
		}	
		else
		{
			VM_GameIndex tempVMgi;
			GameIndex32Moves tempGi32M;


			for( int i = 0; i< this->virtualGameCount; i++ )
			{

				

		//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
				MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(i);
				//RtlCopyMemory(&tempVMgi, memFiles_CONTAINER.GetDataOffset(i), sizeof(VM_GameIndex));
			memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, i);
			pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );
			

				//RtlCopyMemory(&tempGi32M.hdr, pMfc->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));

				if( (tempGi32M.hdr.wname == id) || (tempGi32M.hdr.bname == id) )
				{
					tree_filtered_indexes.push_back(i);
					((NameFilter*)nm)->indexes.push_back(i);
				}
			}

		}
	}

	filter = true;
	return true;

}

bool PgnLoad::FindNameWhite( string &name, int id )
{


	if( /*filter !=*/ true )
	{
		if( tree_filtered_indexes.size() > 0 )
			tree_filtered_indexes.clear();

		Filter *nm = new NameFilter();
		((NameFilter*)nm)->nameMin = name;
		((NameFilter*)nm)->nameMax = name;
		((NameFilter*)nm)->color = 0;

		m_filterStage.m_filters.push_back(nm);

		if( SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
		{
			vector<SortableGameIndex*> temp;
			temp.reserve(m_filteredIndexes.size());

			for( int i = 0; i < m_filteredIndexes.size(); i++ )
			{
				if( m_filteredIndexes[i]->m_Gi->hdr.wname == id )
				{
					temp.push_back( m_filteredIndexes[i] );
				}
			}

			m_filteredIndexes = temp;
		}
		else
		{		
			VM_GameIndex tempVMgi;
			GameIndex32Moves tempGi32M;


			for( int i = 0; i< this->virtualGameCount; i++ )
			{

				

		//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
				MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(i);
				//RtlCopyMemory(&tempVMgi, memFiles_CONTAINER.GetDataOffset(i), sizeof(VM_GameIndex));
				memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, i);
				pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );
			

				//RtlCopyMemory(&tempGi32M.hdr, pMfc->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));

				if( (tempGi32M.hdr.wname == id) )
				{
					tree_filtered_indexes.push_back(i);
					((NameFilter*)nm)->indexes.push_back(i);
				}
			}

		}
	}
	filter = true;
	return true;

	
}

bool PgnLoad::FindNameBlack( string &name, int id )
{

	
	if( /*filter != */ true )
	{
		if( tree_filtered_indexes.size() > 0 )
			tree_filtered_indexes.clear();

		Filter *nm = new NameFilter();
		((NameFilter*)nm)->nameMin = name;
		((NameFilter*)nm)->nameMax = name;
		((NameFilter*)nm)->color = 1;

		m_filterStage.m_filters.push_back(nm);

		if( SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
		{
			vector<SortableGameIndex*> temp;
			temp.reserve(m_filteredIndexes.size());

			for( int i = 0; i < m_filteredIndexes.size(); i++ )
			{
				if( (m_filteredIndexes[i]->m_Gi->hdr.bname == id) )
				{
					temp.push_back( m_filteredIndexes[i] );
				}
			}

			m_filteredIndexes = temp;
		}	
		else
		{		
			VM_GameIndex tempVMgi;
			GameIndex32Moves tempGi32M;
			

			for( int i = 0; i< this->virtualGameCount; i++ )
			{

				

		//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
				MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(i);
				//RtlCopyMemory(&tempVMgi, memFiles_CONTAINER.GetDataOffset(i), sizeof(VM_GameIndex));
				memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, i);
				pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );
			

				//RtlCopyMemory(&tempGi32M.hdr, pMfc->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));

				if( (tempGi32M.hdr.bname == id) )
				{
					tree_filtered_indexes.push_back(i);
					((NameFilter*)nm)->indexes.push_back(i);
				}
			}

		}
	}

	filter = true;
	return true;
}

bool PgnLoad::FilterEco( string &name, int id )
{

	if( tree_filtered_indexes.size() > 0 )
		tree_filtered_indexes.clear();

	Filter *nm = new EcoFilter();
	((EcoFilter*)nm)->ECO_min = name;
	((EcoFilter*)nm)->ECO_max = name;


	m_filterStage.m_filters.push_back(nm);

	if( SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
	{
		vector<SortableGameIndex*> temp;
		temp.reserve(m_filteredIndexes.size());

		for( int i = 0; i < m_filteredIndexes.size(); i++ )
		{
			if( (m_filteredIndexes[i]->m_Gi->hdr.eco == id) )
			{
				temp.push_back( m_filteredIndexes[i] );
			}
		}

		m_filteredIndexes = temp;
	}
	else
	{		
		VM_GameIndex tempVMgi;
		GameIndex32Moves tempGi32M;


		for( int i = 0; i< this->virtualGameCount; i++ )
		{

				

	//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
			MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(i);
		//	RtlCopyMemory(&tempVMgi, memFiles_CONTAINER.GetDataOffset(i), sizeof(VM_GameIndex));
			memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, i);
			pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );
			

			//RtlCopyMemory(&tempGi32M.hdr, pMfc->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));

			if( (tempGi32M.hdr.eco == id) )
			{
				tree_filtered_indexes.push_back(i);
				((NameFilter*)nm)->indexes.push_back(i);
			}
		}

	}

	filter = true;
	return true;
}

bool PgnLoad::FilterDate( string &name, int id )
{


	if( filter != true )
	{
		if( tree_filtered_indexes.size() > 0 )
			tree_filtered_indexes.clear();

		Filter *nm = new DateFilter();
		((DateFilter*)nm)->dateMin = name;
		((DateFilter*)nm)->dateMax = name;


		m_filterStage.m_filters.push_back(nm);

		if( SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
		{
			vector<SortableGameIndex*> temp;
			temp.reserve(m_filteredIndexes.size());

			for( int i = 0; i < m_filteredIndexes.size(); i++ )
			{
				if( (m_filteredIndexes[i]->m_Gi->hdr.date == id) )
				{
					temp.push_back( m_filteredIndexes[i] );
				}
			}

			m_filteredIndexes = temp;
		}
		else
		{		
			VM_GameIndex tempVMgi;
			GameIndex32Moves tempGi32M;


			for( int i = 0; i< this->virtualGameCount; i++ )
			{

				

		//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
				MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(i);
				//RtlCopyMemory(&tempVMgi, memFiles_CONTAINER.GetDataOffset(i), sizeof(VM_GameIndex));
				memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, i);
				pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );
			

				//RtlCopyMemory(&tempGi32M.hdr, pMfc->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));

				if( (tempGi32M.hdr.date == id) )
				{
					tree_filtered_indexes.push_back(i);
					((NameFilter*)nm)->indexes.push_back(i);
				}
			}

		}
	}

	filter = true;
	return true;
}

bool PgnLoad::FilterSite( string &name, int id )
{


	if( /*filter != */  true )
	{
		if( tree_filtered_indexes.size() > 0 )
			tree_filtered_indexes.clear();

		Filter *nm = new SiteFilter();
		((SiteFilter*)nm)->siteMin = name;
		((SiteFilter*)nm)->siteMax = name;


		m_filterStage.m_filters.push_back(nm);

		if( SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
		{
			vector<SortableGameIndex*> temp;
			temp.reserve(m_filteredIndexes.size());

			for( int i = 0; i < m_filteredIndexes.size(); i++ )
			{
				if( (m_filteredIndexes[i]->m_Gi->hdr.sitename == id) )
				{
					temp.push_back( m_filteredIndexes[i] );
				}
			}

			m_filteredIndexes = temp;
		}
		else
		{		
			VM_GameIndex tempVMgi;
			GameIndex32Moves tempGi32M;


			for( int i = 0; i< this->virtualGameCount; i++ )
			{

				

		//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
				MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(i);
//				RtlCopyMemory(&tempVMgi, memFiles_CONTAINER.GetDataOffset(i), sizeof(VM_GameIndex));

				memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, i);
				pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );			

//				RtlCopyMemory(&tempGi32M.hdr, pMfc->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));

				if( (tempGi32M.hdr.sitename == id) )
				{
					tree_filtered_indexes.push_back(i);
					((NameFilter*)nm)->indexes.push_back(i);
				}
			}

		}
	}

	filter = true;
	return true;
}

bool PgnLoad::FilterEvent( string &name, int id )
{
	temporyResCache.Clear();

	if( /*filter !=*/ true )
	{
		if( tree_filtered_indexes.size() > 0 )
			tree_filtered_indexes.clear();

		Filter *nm = new EventFilter();
		((EventFilter*)nm)->eventMin = name;
		((EventFilter*)nm)->eventMax = name;


		m_filterStage.m_filters.push_back(nm);

		if( SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
		{
			vector<SortableGameIndex*> temp;
			temp.reserve(m_filteredIndexes.size());

			for( int i = 0; i < m_filteredIndexes.size(); i++ )
			{
				if( (m_filteredIndexes[i]->m_Gi->hdr.eventname == id) )
				{
					temp.push_back( m_filteredIndexes[i] );
				}
			}

			m_filteredIndexes = temp;
		}
		else
		{		
			VM_GameIndex tempVMgi;
			GameIndex32Moves tempGi32M;


			for( int i = 0; i< this->virtualGameCount; i++ )
			{

				

		//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
				MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(i);
//				RtlCopyMemory(&tempVMgi, memFiles_CONTAINER.GetDataOffset(i), sizeof(VM_GameIndex));
				memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, i);
				pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );
			

//				RtlCopyMemory(&tempGi32M.hdr, pMfc->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));

				if( (tempGi32M.hdr.eventname == id) )
				{
					tree_filtered_indexes.push_back(i);
					((NameFilter*)nm)->indexes.push_back(i);
				}
			}

		}
	}

	filter = true;
	return true;
}


void PgnLoad::FilterRAMGamesByECO(string &eco)
{
	m_filteredIndexes.clear();
	SortAscendingRAM(6);
	
	bool found = false;
	for( int i = 0; i < this->virtualGameCount; i++ )
	{
		if( this->headersListData.LookupEventName( m_SortableIndexes[i]->m_Gi->hdr.eco ) == eco )
		{
			found = true;
			//tree_filtered_indexes.push_back(i);
			m_filteredIndexes.push_back(m_SortableIndexes[i]);
		}
		else
		{
			if ( found ) break;
		}
	}

	filter = true;
}

bool PgnLoad::ParseHeader32M( const char* src, int dataLen, HeaderListData &headers, GameIndex32Moves *gi)
{
	for( int i = 0; i < dataLen; i++ )
	{
		char c = src[i];

		if( c == '[' )
		{
			string s = "";
			string inQuotes = "";
			string type = "";
			s+=c;
			int first_space = 0;
			bool openQuotes = false;
			while( (i < dataLen) && (src[i++] != ']') )
			{
				if( src[i] == ' ' )
				{
					if( type[type.length()-1] != ' ')
					{
						type+= src[i]; // adding space after text to distinguish White from WhiteElo
					}
					first_space = i;
				}
				if( src[i] == '"' && openQuotes == false)
				{
					openQuotes = true;
				}
				else if( src[i] == '"' && openQuotes == true )
				{
					openQuotes = false;

				}
				else if( openQuotes )
				{
					inQuotes += src[i];
				}

				if( !openQuotes && first_space == 0)
				{
					type+= src[i];
				}
			}

			// change them to uppercase.

			if( src[i] == '[' ) i--;

			
			if( type == "White ")
			{
				gi->hdr.wname = headers.AddPlayerName(inQuotes);
			}
			else if( type == "Black ")
			{
				gi->hdr.bname = headers.AddPlayerName(inQuotes);
			}
			else if( type == "Event ")
			{
				gi->hdr.eventname = headers.AddEventName(inQuotes);
			}
			else if( type == "Site ")
			{
				gi->hdr.sitename = headers.AddSiteName(inQuotes);
			}
			else if( type == "Date ")
			{
				gi->hdr.date = headers.AddDateName(inQuotes);
			}
			else if ( type == "Result ")
			{
				if( inQuotes == "1-0" )
				{
					gi->hdr.result = 0;
				}
				else if ( inQuotes == "0-1" )
				{
					gi->hdr.result = 1;
				}
				else if( inQuotes == "1/2-1/2")
				{
					gi->hdr.result = 2;
				}
				 
			}
			else if( type == "WhiteElo ")
			{
				gi->hdr.whiteElo = headers.AddELO(atoi(inQuotes.c_str()));
			}
			else if( type == "BlackElo ")
			{
				gi->hdr.blackElo = headers.AddELO(atoi(inQuotes.c_str()));
			}
			else if( type == "ECO " )
			{
				gi->hdr.eco = headers.AddEco(inQuotes);
			}
		}
	}
	return true;
}

bool PgnLoad::ParseGame32M( const char* src, int dataLen, HeaderListData &headers, GameIndex32Moves *gi)
{
	static ChessBoard brd;
	PGN::Game game;
	for( int i = 0; i < dataLen; i++ )
	{
		game.game_text += src[i];
	}

	if( game.Parse() == 0 )
	{
		gi->nummoves = 10;
		gi->moves = new unsigned short[gi->nummoves];
		return false;
	}
	gi->nummoves = game.mainlinePlyCount;
	gi->moves = new unsigned short[gi->nummoves];
	
	PGN::Move* pMove = game.pRoot;

	int counter = 0;
	brd.SetInitialPosition();
	while( pMove && (counter < gi->nummoves))
	{
		brd.parseMove(pMove->white );
		if( brd.GetParseError() == 1 )
		{
			return false;
		}

		MyChessMove mvW = *brd.GetLastMove();
		mvW.Compress();
		
		gi->moves[counter++]=mvW.packed;

		if( counter >= gi->nummoves )
			break;

		brd.parseMove(pMove->black );
		if( brd.GetParseError() == 1 )
		{
			return false;
		}

		MyChessMove mvB = *brd.GetLastMove();
		mvB.Compress();

		//pMove->blackMove.Compress();
		gi->moves[counter++]=mvB.packed;
		
		pMove = pMove->pNext;
	}

	return true;
}

bool PgnLoad::SaveGames32M(string &filename)
{
	FILE* file = fopen(filename.c_str(), "wb");

	if( !file )
		return false;

	int dot_pos = filename.find_last_of(".");

	if( dot_pos == -1 )
		return false;
	
	string headersListOutput = filename.substr(0,dot_pos-3);
	//headersListOutput += ".hls";
	this->headersListData.WriteLists(headersListOutput);

	fwrite( (void*)&(this->virtualGameCount), sizeof( int ), 1, file );
	for( int i = 0; i < this->virtualGameCount; i++ )
	{
		if( this->m_SortableIndexes[i] )
		{
			this->m_SortableIndexes[i]->m_Gi->hdr.Save(file);
			fwrite((void*) &(this->m_SortableIndexes[i]->m_Gi->nummoves), sizeof( unsigned short int ),1, file);
			fwrite((void*) this->m_SortableIndexes[i]->m_Gi->moves, sizeof(unsigned short int ),this->m_SortableIndexes[i]->m_Gi->nummoves, file);
		}
	}

	fclose(file);

	return true;
}

bool PgnLoad::LoadGames32M(string &filename)
{
	//sort_method = 1;
	
	FILE* file = fopen(filename.c_str(), "rb");

	if( !file )
		return false;

	int dot_pos = filename.find_last_of(".");

	if( dot_pos == -1 )
		return false;

	string headersListOutput = filename.substr(0,dot_pos-3);
	//headersListOutput += ".hls";
	this->headersListData.ReadLists(headersListOutput);

	fread( (void*)&this->virtualGameCount, sizeof( this->virtualGameCount ), 1, file );

	this->m_SortableIndexes.reserve(virtualGameCount);// = new GameIndex32Moves*[this->virtualGameCount];

	for( int i = 0; i < this->virtualGameCount; i++ )
	{
		SortableGameIndex *sgi = new SortableGameIndex();
		sgi->m_Gi = new GameIndex32Moves();
		sgi->original_loc = i;
		this->m_SortableIndexes.push_back( sgi );//( new GameIndex32Moves(), i) );
		int offset = 0;
		this->m_SortableIndexes [i]->m_Gi->hdr.Read(file, offset);
		//fread( (void*) &(this->gameIndexes32M [i]->hdr), sizeof( int )*7 + sizeof(unsigned char), 1, file );
		fread( (void*) &this->m_SortableIndexes [i]->m_Gi->nummoves, sizeof( unsigned short ), 1, file );
		this->m_SortableIndexes [i]->m_Gi->moves = new unsigned short int[ this->m_SortableIndexes [i]->m_Gi->nummoves ];
		fread( (void*) this->m_SortableIndexes[i]->m_Gi->moves, sizeof( unsigned short int ), this->m_SortableIndexes [i]->m_Gi->nummoves , file );
	}

	fclose( file );

	headersListData.PerformSort();
	//headersListData.DeleteTrees();

		// pointer is SET
	g_pHeadersList = &headersListData;

	this->vm_fileIsLoaded = true;

	m_filteredIndexes.reserve(this->virtualGameCount);

	return true;
}


bool PgnLoad::AddGames32M(string &filename)
{
	//sort_method = 1;
	
	FILE* file = fopen(filename.c_str(), "rb");

	if( !file )
		return false;

	int dot_pos = filename.find_last_of(".");

	if( dot_pos == -1 )
		return false;

	string headersListOutput = filename.substr(0,dot_pos-3);
	//headersListOutput += ".hls";
	this->headersListData.ReadLists(headersListOutput);

	int tempGameCount=0;
	fread( (void*)&tempGameCount, sizeof( int ), 1, file );

	/*GameIndex32Moves** gii32m = new GameIndex32Moves*[this->virtualGameCount + tempGameCount];
	for( int i = 0; i < this->virtualGameCount; i++ )
	{
		gii32m[i] = this->gameIndexes32M[i];

	}
	
	delete [] gameIndexes32M;

	gameIndexes32M = gii32m;*/
	//this->gameIndexes32M = new GameIndex32Moves*[this->virtualGameCount];

	m_SortableIndexes.reserve(virtualGameCount + tempGameCount);
	for( int i = 0; i < tempGameCount; i++ )
	{
		SortableGameIndex *sgi = new SortableGameIndex();// new GameIndex32Moves(), i )
		sgi->m_Gi = new GameIndex32Moves();
		sgi->original_loc = i;
		this->m_SortableIndexes.push_back( sgi );
		int offset = 0;
		this->m_SortableIndexes[m_SortableIndexes.size()-1]->m_Gi->hdr.Read(file, offset);
		//fread( (void*) &(this->gameIndexes32M [i]->hdr), sizeof( int )*7 + sizeof(unsigned char), 1, file );
		fread( (void*) &this->m_SortableIndexes [m_SortableIndexes.size()-1]->m_Gi->nummoves, sizeof( unsigned short ), 1, file );
		this->m_SortableIndexes [m_SortableIndexes.size()-1]->m_Gi->moves = new unsigned short int[ this->m_SortableIndexes [m_SortableIndexes.size()-1]->m_Gi->nummoves ];
		fread( (void*) this->m_SortableIndexes[m_SortableIndexes.size()-1]->m_Gi->moves, sizeof( unsigned short int ), this->m_SortableIndexes [m_SortableIndexes.size()-1]->m_Gi->nummoves , file );
	}

	this->virtualGameCount += tempGameCount;

	fclose( file );

	headersListData.PerformSort();
	//headersListData.DeleteTrees();

		// pointer is SET
	g_pHeadersList = &headersListData;

	this->vm_fileIsLoaded = true;

	m_filteredIndexes.reserve(this->virtualGameCount);

	return true;
}

const char* PgnLoad::IndexRAM_PGN_32M(const char* sourceName, void* user_data )
{
	/*if( source_mf_p )
	{
		delete source_mf_p;
		source_mf_p = NULL;

	}*/

	InputMemoryFile *source_mf = 0;
	memFiles_CONTAINER.pMemFileContainer= 0;
	
	bool bFoundMemFile = false;
	for( int i = 0; i < this->memFiles_CONTAINER.m_pages.size(); i++ )
	{
		if( this->memFiles_CONTAINER.m_pages[i]->filename == string(sourceName) )
		{
			bFoundMemFile = true;
			source_mf = memFiles_CONTAINER.m_pages[i]->source_mf_p;
			memFiles_CONTAINER.pMemFileContainer = this->memFiles_CONTAINER.m_pages[i];
		}
	}

	if( bFoundMemFile == false )
	{
		source_mf = new InputMemoryFile(sourceName);//,MemoryFile::if_exists_truncate_if_not_exists_create);
		memFiles_CONTAINER.pMemFileContainer = new MemoryFileContainer();
		memFiles_CONTAINER.pMemFileContainer->source_mf_p = source_mf;
		memFiles_CONTAINER.pMemFileContainer->filename = string(sourceName);

		this->memFiles_CONTAINER.m_pages.push_back(memFiles_CONTAINER.pMemFileContainer);
		memFiles_CONTAINER.pMemFileContainer->Index = memFiles_CONTAINER.m_pages.size()-1;
		
	}

	source_mf_p = source_mf;


	if (! source_mf->data()) 
	{
		vm_read_error = "Cannot access VM data";
			return 0;
	}

	const char* data = source_mf->data();
	vm_data = data;
	bool before_header = true;
	bool in_header = false;
	bool before_game = false;

//	this->headersListData.Clear();
	virtualGameCount = 0;
	int gameCount = 0;


//	wxProgressDialog progress("Progress Dialog", "Indexing PGN, Please Wait", 100, (wxWindow*)user_data);

	//progress.Show(true);

	int fifty_percent = (source_mf->size()-1);
	//========================================================================
	//  pre-scan
	//========================================================================

	if( bFoundMemFile == false )
	{
		
		for(uint64 i = 0; (i < source_mf->size()-1) && (data[i] != EOF); i++ )
		{
			double progVal = double(i) / double(fifty_percent);
			progVal *= 100;
			progVal /= 2;
	//		progress.Update(1, "Pre-Scanning ... Please Wait");
			if( data[i] == '[' )
			{
				/**h = new HeaderElem();*/
				if( (in_header == false) && (before_game == false))
				{
				
					in_header = true;
				}

				if( before_header )
				{
					before_header = false;
				}
			}

			char cc = data[i];
		
			if( NewLine(data,i) )
			{
			
				if( i+2 < source_mf->size()-1 &&
					data[i+1] != EOF && data[i+2] != EOF)
				{
					if( NextLinePattern(data,i))
					{
						uint64 j;
						for( j = i+2; j < source_mf->size() && data[j]!= EOF; j++)
						{
							char c = data[j];
							if( ((data[j] == '[')&& before_game) || ((data[j] > '0' && data[j] < '9')&&!before_game) )
								break;
						}
				
						if( in_header && !before_game)
						{

							before_game = true;
							in_header = false;
						}
						else if( before_game && !in_header)
						{
							virtualGameCount++;
						
						
							// parse header here
							gameCount++;

							before_header = true;
							before_game = false;
							in_header = false;
						}
						

						continue;
					}

				}
			}
			else
			{

			}
		}

		memFiles_CONTAINER.pMemFileContainer->gameCount = virtualGameCount;
	}
	else
	{
		gameCount = memFiles_CONTAINER.pMemFileContainer->gameCount;
	}

	//========================================================================
	// End pre-scan
	//========================================================================

/*	if( pMemFileContainer->gameIndexMemPool == NULL )
	{
		pMemFileContainer->gameIndexMemPool = new MemPool(sizeof(GameIndex));
		string newFileName = string(sourceName)+"IndexData.dat";
		if(!pMemFileContainer->gameIndexMemPool->OpenMemoryFileMemory((char*)newFileName.c_str(), true, gameCount * sizeof(GameIndex)))
		{
			wxMessageBox("Could Not Index Memory File");
			return 0;
		}
		gameIndexMemPool = pMemFileContainer->gameIndexMemPool;
		//virtualGameCount
	}
	else
	{
		vm_fileIsLoaded = true;
		gameIndexMemPool = pMemFileContainer->gameIndexMemPool;
		virtualGameCount = pMemFileContainer->gameCount;
		return vm_data;
	}*/

	//this->gameIndexes32M = new GameIndex32Moves*[virtualGameCount];

	// first check to see if a file exists 
	string tempStr = sourceName;
	int dot_pos = tempStr.find(".");

	// query size of file
	// load the last compressed entry already in the file, 
	// write the next one on to the end ... or get file size, set that as the start of the 
	// next one, and simply write the data, for the name lists, they need to be loaded
	// up again, another name has to be pushed into the name list, and then it 
	// is saved again...
	// see above for any memory optimizations, perhaps too much is in memory, it takes too
	// long etc

	string outputfileName = tempStr.substr(0, dot_pos-1);
	string preloadFileName = outputfileName;
	preloadFileName += ".indx";

	// should be a VM file, this file should exist not as this file name


//	board = new ChessBoard();

	//fscanf(source_mf.handle(),
	//const char* data = source_mf_p->data();
	//vm_data = data;
	before_header = true;
	in_header = false;
	before_game = false;

	if( startI32M )
	{
		delete startI;
		startI32M = NULL;
	}
	startI32M = new GameIndex32Moves;
	GameIndex32Moves *curr = startI32M;
	GameIndex32Moves *pStart = startI32M;
	HeaderElemC *h;

	//gameIndexes32M[0] = new GameIndex32Moves();
//	m_vecGameIndexes.push_back( new GameIndex32Moves() ) ;

	m_SortableIndexes.reserve(virtualGameCount);

	virtualGameCount = 0;
	//GameIndex memoryGameIndex;
	//MemBlock *mb = gameIndexMemPool->GetBlock();
	
	
	gameCount = 0;

	m_SortableIndexes.push_back(new SortableGameIndex(new GameIndex32Moves(), 0));

	for(uint64 i = 0; (i < source_mf_p->size()-1) && (data[i] != EOF); i++ )
	{
		double progVal = double(i) / double(fifty_percent);
		progVal *= 100;
		//progVal /= 2;
		//progress.Update(1, "Indexing ... Please Wait");
		
		if( data[i] == '[' )
		{
			/**h = new HeaderElem();*/
			if( (in_header == false) && (before_game == false) )
			{
				
				in_header = true;
			}

			if( before_header )
			{
			//	curr->header_start = i;
		//		memoryGameIndex.header_start = i;
				//gameIndexes32M[gameCount]->header_start = i;
				//m_vecGameIndexes[gameCount]->header_start = i;
				m_SortableIndexes[gameCount]->m_Gi->header_start = i;
				before_header = false;
			}
		}
		
		if( NewLine(data,i) )
		{
			
			if( i+2 < source_mf_p->size()-1 &&
				data[i+1] != EOF && data[i+2] != EOF)
			{
				if( NextLinePattern(data,i))
				{
					uint64 j;
					for( j = i+2; j < source_mf_p->size() && data[j]!= EOF; j++)
					{
						char c = data[j];
						if( ((data[j] == '[')&& before_game) || ((data[j] > '0' && data[j] < '9')&&!before_game) )
							break;
					}
				
					//cout<< " "<<data[j] <<endl;
					if( in_header && !before_game)
					{
						/**curr->elems.push_back(h);*/
						//curr->header_end = i;//-(int)curr->header_start;
						//curr->game_start = j;//-(int)curr->header_start;
			//			memoryGameIndex.header_end = i;
			//			memoryGameIndex.game_start = j;
						//gameIndexes32M[gameCount]->header_end = i;
						//gameIndexes32M[gameCount]->game_start = j;
						//m_vecGameIndexes[gameCount]->header_end = i;
						//m_vecGameIndexes[gameCount]->game_start = j;
						m_SortableIndexes[gameCount]->m_Gi->header_end = i;
						m_SortableIndexes[gameCount]->m_Gi->game_start = j;
						before_game = true;
						in_header = false;
					}
					else if( before_game && !in_header)
					{
						//gameIndexes32M[gameCount]->game_end = i;
						//m_vecGameIndexes[gameCount]->game_end = i;
						m_SortableIndexes[gameCount]->m_Gi->game_end = i;

						if( (m_SortableIndexes[gameCount]->m_Gi->game_end -
							 m_SortableIndexes[gameCount]->m_Gi->game_start) < 60 )
						{
						}
						else
						{
						virtualGameCount++;
				//		memoryGameIndex.game_end = i;
						//curr->game_end = i;//-(int)curr->header_start;
						// parse header here
						gameCount++;

						m_SortableIndexes.push_back(new SortableGameIndex(new GameIndex32Moves(), gameCount));
						}
						//gameIndexes32M[gameCount] = new GameIndex32Moves();
						//m_vecGameIndexes.push_back(new GameIndex32Moves());
				//		RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
						
				//		delete mb;

				//		mb = gameIndexMemPool->GetBlock();
						//curr->pNext = new GameIndex;
						//curr = curr->pNext;
						before_header = true;
						before_game = false;
						in_header = false;
					}
						
					//cout<< string(data+i,data+i+1);
					continue;
				}

			}
		
			/**if (in_header)
			  curr->elems.push_back(h);*/
		}
		else
		{
			/**if( in_header )
				h->data += data[i];*/
		}
	}

	//delete mb;

	//progress.Close(true);

	if( curr->game_end == 0 )
	{
		curr->game_end = source_mf_p->size()-1;

	}

	for( int i = 0; i < gameCount; i++ )
	{
		ParseHeader32M( data + m_SortableIndexes[i]->m_Gi->header_start, 
			this->m_SortableIndexes[i]->m_Gi->header_end - this->m_SortableIndexes[i]->m_Gi->header_start, 
			headersListData, 
			this->m_SortableIndexes[i]->m_Gi);

		ParseGame32M( data + this->m_SortableIndexes[i]->m_Gi->game_start, 
			this->m_SortableIndexes[i]->m_Gi->game_end - this->m_SortableIndexes[i]->m_Gi->game_start, 
			headersListData, 
			this->m_SortableIndexes[i]->m_Gi);


			
	}

	// pointer is SET
	g_pHeadersList = &headersListData;

	headersListData.PerformSort();
	//headersListData.DeleteTrees();

	vm_fileIsLoaded = true;


	m_filteredIndexes.reserve(this->virtualGameCount);

	for( int i = 0; i < this->virtualGameCount; i++ )
	{
		m_filteredIndexes.push_back(m_SortableIndexes[i]);
	}

	/*arrayGi = new GameIndex*[game_count];

	GameIndex *temp = pStart;
	for( int i = 0; i < game_count; i++ )
	{
		arrayGi[i] = temp;
		temp = temp->pNext;
	}
	*/


	/*if( !WriteVM_PGNIndexFile(outputfileName, pStart ) )
	{
		// error
	}*/
	string srcNm(sourceName);
	int lastDotScrNm = srcNm.find_last_of(".");
	string toSaveSrcNm = "";
	toSaveSrcNm = srcNm.substr(0,lastDotScrNm);
	toSaveSrcNm += ".pgnc2";
	SaveGames32M(toSaveSrcNm);

	return data;
}

const char* PgnLoad::AddIndexRAM_PGN_32M(const char* sourceName, void* user_data)

{
	/*if( source_mf_p )
	{
		delete source_mf_p;
		source_mf_p = NULL;

	}*/

	int old_vgc = virtualGameCount;
	InputMemoryFile *source_mf = 0;
	memFiles_CONTAINER.pMemFileContainer= 0;

	bool bFoundMemFile = false;
	for( int i = 0; i < this->memFiles_CONTAINER.m_pages.size(); i++ )
	{
		if( this->memFiles_CONTAINER.m_pages[i]->filename == string(sourceName) )
		{
			bFoundMemFile = true;
			source_mf = memFiles_CONTAINER.m_pages[i]->source_mf_p;
			memFiles_CONTAINER.pMemFileContainer = this->memFiles_CONTAINER.m_pages[i];
		}
	}

	if( bFoundMemFile == false )
	{
		source_mf = new InputMemoryFile(sourceName);//,MemoryFile::if_exists_truncate_if_not_exists_create);
		memFiles_CONTAINER.pMemFileContainer = new MemoryFileContainer();
		memFiles_CONTAINER.pMemFileContainer->source_mf_p = source_mf;
		memFiles_CONTAINER.pMemFileContainer->filename = string(sourceName);

		this->memFiles_CONTAINER.m_pages.push_back(memFiles_CONTAINER.pMemFileContainer);
		memFiles_CONTAINER.pMemFileContainer->Index = memFiles_CONTAINER.m_pages.size()-1;
		
	}

	source_mf_p = source_mf;


	if (! source_mf->data()) 
	{
		vm_read_error = "Cannot access VM data";
			return 0;
	}

	const char* data = source_mf->data();
	vm_data = data;
	bool before_header = true;
	bool in_header = false;
	bool before_game = false;

//	this->headersListData.Clear();
	virtualGameCount = 0;
	int gameCount = 0;


//	wxProgressDialog progress("Progress Dialog", "Indexing PGN, Please Wait", 100, (wxWindow*)user_data);

	//progress.Show(true);

	int fifty_percent = (source_mf->size()-1);
	//========================================================================
	//  pre-scan
	//========================================================================

	if( bFoundMemFile == false )
	{
		
		for(uint64 i = 0; (i < source_mf->size()-1) && (data[i] != EOF); i++ )
		{
			double progVal = double(i) / double(fifty_percent);
			progVal *= 100;
			progVal /= 2;
	//		progress.Update(1, "Pre-Scanning ... Please Wait");
			if( data[i] == '[' )
			{
				/**h = new HeaderElem();*/
				if( (in_header == false) && (before_game == false))
				{
				
					in_header = true;
				}

				if( before_header )
				{
					before_header = false;
				}
			}

			char cc = data[i];
		
			if( NewLine(data,i) )
			{
			
				if( i+2 < source_mf->size()-1 &&
					data[i+1] != EOF && data[i+2] != EOF)
				{
					if( NextLinePattern(data,i))
					{
						uint64 j;
						for( j = i+2; j < source_mf->size() && data[j]!= EOF; j++)
						{
							char c = data[j];
							if( ((data[j] == '[')&& before_game) || ((data[j] > '0' && data[j] < '9')&&!before_game) )
								break;
						}
				
						if( in_header && !before_game)
						{

							before_game = true;
							in_header = false;
						}
						else if( before_game && !in_header)
						{
							virtualGameCount++;
						
						
							// parse header here
							gameCount++;

							before_header = true;
							before_game = false;
							in_header = false;
						}
						

						continue;
					}

				}
			}
			else
			{

			}
		}

		memFiles_CONTAINER.pMemFileContainer->gameCount = virtualGameCount;
	}
	else
	{
		gameCount = memFiles_CONTAINER.pMemFileContainer->gameCount;
	}

	//========================================================================
	// End pre-scan
	//========================================================================

/*	if( pMemFileContainer->gameIndexMemPool == NULL )
	{
		pMemFileContainer->gameIndexMemPool = new MemPool(sizeof(GameIndex));
		string newFileName = string(sourceName)+"IndexData.dat";
		if(!pMemFileContainer->gameIndexMemPool->OpenMemoryFileMemory((char*)newFileName.c_str(), true, gameCount * sizeof(GameIndex)))
		{
			wxMessageBox("Could Not Index Memory File");
			return 0;
		}
		gameIndexMemPool = pMemFileContainer->gameIndexMemPool;
		//virtualGameCount
	}
	else
	{
		vm_fileIsLoaded = true;
		gameIndexMemPool = pMemFileContainer->gameIndexMemPool;
		virtualGameCount = pMemFileContainer->gameCount;
		return vm_data;
	}*/

	//this->gameIndexes32M = new GameIndex32Moves*[virtualGameCount];

	// first check to see if a file exists 
	string tempStr = sourceName;
	int dot_pos = tempStr.find(".");

	// query size of file
	// load the last compressed entry already in the file, 
	// write the next one on to the end ... or get file size, set that as the start of the 
	// next one, and simply write the data, for the name lists, they need to be loaded
	// up again, another name has to be pushed into the name list, and then it 
	// is saved again...
	// see above for any memory optimizations, perhaps too much is in memory, it takes too
	// long etc

	string outputfileName = tempStr.substr(0, dot_pos-1);
	string preloadFileName = outputfileName;
	preloadFileName += ".indx";

	// should be a VM file, this file should exist not as this file name


//	board = new ChessBoard();

	//fscanf(source_mf.handle(),
	//const char* data = source_mf_p->data();
	//vm_data = data;
	before_header = true;
	in_header = false;
	before_game = false;

	if( startI32M )
	{
		delete startI;
		startI32M = NULL;
	}
	startI32M = new GameIndex32Moves;
	GameIndex32Moves *curr = startI32M;
	GameIndex32Moves *pStart = startI32M;
	HeaderElemC *h;

	//gameIndexes32M[0] = new GameIndex32Moves();
//	m_vecGameIndexes.push_back( new GameIndex32Moves() ) ;

	m_SortableIndexes.reserve(old_vgc + virtualGameCount);

	virtualGameCount = 0;
	//GameIndex memoryGameIndex;
	//MemBlock *mb = gameIndexMemPool->GetBlock();
	
	
	gameCount = 0;

	m_SortableIndexes.push_back(new SortableGameIndex(new GameIndex32Moves(), old_vgc));

	for(uint64 i = 0; (i < source_mf_p->size()-1) && (data[i] != EOF); i++ )
	{
		double progVal = double(i) / double(fifty_percent);
		progVal *= 100;
		//progVal /= 2;
		//progress.Update(1, "Indexing ... Please Wait");
		
		if( data[i] == '[' )
		{
			/**h = new HeaderElem();*/
			if( (in_header == false) && (before_game == false) )
			{
				
				in_header = true;
			}

			if( before_header )
			{
			//	curr->header_start = i;
		//		memoryGameIndex.header_start = i;
				//gameIndexes32M[gameCount]->header_start = i;
				//m_vecGameIndexes[gameCount]->header_start = i;
				m_SortableIndexes[old_vgc + gameCount]->m_Gi->header_start = i;
				before_header = false;
			}
		}
		
		if( NewLine(data,i) )
		{
			
			if( i+2 < source_mf_p->size()-1 &&
				data[i+1] != EOF && data[i+2] != EOF)
			{
				if( NextLinePattern(data,i))
				{
					uint64 j;
					for( j = i+2; j < source_mf_p->size() && data[j]!= EOF; j++)
					{
						char c = data[j];
						if( ((data[j] == '[')&& before_game) || ((data[j] > '0' && data[j] < '9')&&!before_game) )
							break;
					}
				
					//cout<< " "<<data[j] <<endl;
					if( in_header && !before_game)
					{
						/**curr->elems.push_back(h);*/
						//curr->header_end = i;//-(int)curr->header_start;
						//curr->game_start = j;//-(int)curr->header_start;
			//			memoryGameIndex.header_end = i;
			//			memoryGameIndex.game_start = j;
						//gameIndexes32M[gameCount]->header_end = i;
						//gameIndexes32M[gameCount]->game_start = j;
						//m_vecGameIndexes[gameCount]->header_end = i;
						//m_vecGameIndexes[gameCount]->game_start = j;
						m_SortableIndexes[old_vgc + gameCount]->m_Gi->header_end = i;
						m_SortableIndexes[old_vgc + gameCount]->m_Gi->game_start = j;
						before_game = true;
						in_header = false;
					}
					else if( before_game && !in_header)
					{
						//gameIndexes32M[gameCount]->game_end = i;
						//m_vecGameIndexes[gameCount]->game_end = i;
						m_SortableIndexes[old_vgc + gameCount]->m_Gi->game_end = i;

						if( (m_SortableIndexes[old_vgc+gameCount]->m_Gi->game_end -
							 m_SortableIndexes[old_vgc+gameCount]->m_Gi->game_start) < 60 )
						{
						}
						else
						{
						virtualGameCount++;
				//		memoryGameIndex.game_end = i;
						//curr->game_end = i;//-(int)curr->header_start;
						// parse header here
						gameCount++;

						m_SortableIndexes.push_back(new SortableGameIndex(new GameIndex32Moves(), old_vgc+gameCount));
						}						//gameIndexes32M[gameCount] = new GameIndex32Moves();
						//m_vecGameIndexes.push_back(new GameIndex32Moves());
				//		RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
						
				//		delete mb;

				//		mb = gameIndexMemPool->GetBlock();
						//curr->pNext = new GameIndex;
						//curr = curr->pNext;
						before_header = true;
						before_game = false;
						in_header = false;
					}
						
					//cout<< string(data+i,data+i+1);
					continue;
				}

			}
		
			/**if (in_header)
			  curr->elems.push_back(h);*/
		}
		else
		{
			/**if( in_header )
				h->data += data[i];*/
		}
	}

	//delete mb;

	//progress.Close(true);

	if( curr->game_end == 0 )
	{
		curr->game_end = source_mf_p->size()-1;

	}

	for( int i = 0; i < gameCount; i++ )
	{
		ParseHeader32M( data + m_SortableIndexes[old_vgc + i]->m_Gi->header_start, 
			this->m_SortableIndexes[old_vgc + i]->m_Gi->header_end - this->m_SortableIndexes[old_vgc + i]->m_Gi->header_start, 
			headersListData, 
			this->m_SortableIndexes[old_vgc + i]->m_Gi);

		ParseGame32M( data + this->m_SortableIndexes[old_vgc + i]->m_Gi->game_start, 
			this->m_SortableIndexes[old_vgc + i]->m_Gi->game_end - this->m_SortableIndexes[old_vgc + i]->m_Gi->game_start, 
			headersListData, 
			this->m_SortableIndexes[old_vgc + i]->m_Gi);
			
	}

	this->virtualGameCount += old_vgc;

	// pointer is SET
	g_pHeadersList = &headersListData;

	//headersListData.
	headersListData.PerformSort();
	//headersListData.DeleteTrees();

	vm_fileIsLoaded = true;

	m_filteredIndexes.reserve(this->virtualGameCount);

	for( int i = 0; i < this->virtualGameCount; i++ )
	{
		m_filteredIndexes.push_back(m_SortableIndexes[i]);
	}
	/*arrayGi = new GameIndex*[game_count];

	GameIndex *temp = pStart;
	for( int i = 0; i < game_count; i++ )
	{
		arrayGi[i] = temp;
		temp = temp->pNext;
	}
	*/


	/*if( !WriteVM_PGNIndexFile(outputfileName, pStart ) )
	{
		// error
	}*/
	string srcNm(sourceName);
	int lastDotScrNm = srcNm.find_last_of(".");
	string toSaveSrcNm = "";
	toSaveSrcNm = srcNm.substr(0,lastDotScrNm);
	toSaveSrcNm += ".pgnc2";
	SaveGames32M(toSaveSrcNm);

	return data;
}

const char* PgnLoad::IndexVM_PGN_32M(const char* sourceName, void* user_data )
{
	/*if( source_mf_p )
	{
		delete source_mf_p;
		source_mf_p = NULL;

	}*/

	wxWindow *pFrame = (wxWindow*)user_data;

//	int max = 100;
//	wxBusyInfo busyLoading("Loading File, Please be Patient");
	
 /*   wxProgressDialog dialog(wxT("PGN Loading Dialog"),
                            wxT("An informative message"),
                            max,    // range
                            pFrame,   // parent
                            wxPD_CAN_ABORT |
                            wxPD_CAN_SKIP |
                           // wxPD_APP_MODAL |
                            // wxPD_AUTO_HIDE | -- try this as well
                            wxPD_ELAPSED_TIME |
                            wxPD_ESTIMATED_TIME |
                            wxPD_REMAINING_TIME
                            | wxPD_SMOOTH // - makes indeterminate mode bar on WinXP very small
                            );

bool skip = true;
							bool cont = dialog.Pulse("Prescanning", &skip);

							if ( !cont )
							{
								if ( wxMessageBox(wxT("Do you really want to cancel?"),
												  wxT("Progress dialog question"),  // caption
												  wxYES_NO | wxICON_QUESTION) == wxYES )
								{
									return 0;
								}
				
							}
*/
	InputMemoryFile *source_mf = 0;
	memFiles_CONTAINER.pMemFileContainer= 0;

	bool bFoundMemFile = false;
	for( int i = 0; i < this->memFiles_CONTAINER.m_pages.size(); i++ )
	{
		if( this->memFiles_CONTAINER.m_pages[i]->filename == string(sourceName) )
		{
			bFoundMemFile = true;
			source_mf = memFiles_CONTAINER.m_pages[i]->source_mf_p;
			memFiles_CONTAINER.pMemFileContainer = this->memFiles_CONTAINER.m_pages[i];
		}
	}

	if( bFoundMemFile == false )
	{
		source_mf = new InputMemoryFile(sourceName);//,MemoryFile::if_exists_truncate_if_not_exists_create);
		memFiles_CONTAINER.pMemFileContainer = new MemoryFileContainer();
		memFiles_CONTAINER.pMemFileContainer->source_mf_p = source_mf;
		memFiles_CONTAINER.pMemFileContainer->filename = string(sourceName);

		this->memFiles_CONTAINER.m_pages.push_back(memFiles_CONTAINER.pMemFileContainer);
		memFiles_CONTAINER.pMemFileContainer->Index = this->memFiles_CONTAINER.m_pages.size()-1;
		
	}

	source_mf_p = source_mf;


	if (! source_mf->data()) 
	{
		vm_read_error = "Cannot access VM data";
			return 0;
	}

	const char* data = source_mf->data();
	vm_data = data;
	bool before_header = true;
	bool in_header = false;
	bool before_game = false;

//	this->headersListData.Clear();
	virtualGameCount = 0;
	int gameCount = 0;


//	wxProgressDialog progress("Progress Dialog", "Indexing PGN, Please Wait", 100, (wxWindow*)user_data);

	//progress.Show(true);

	int fifty_percent = (source_mf->size()-1);
	//========================================================================
	//  pre-scan
	//========================================================================

	if( bFoundMemFile == false )
	{
		bool cont = true;
		for(uint64 i = 0; (i < source_mf->size()-1) && (data[i] != EOF); i++ )
		{
			double progVal = double(i) / double(fifty_percent);
			progVal *= 100;
			progVal /= 2;


	//		progress.Update(1, "Pre-Scanning ... Please Wait");
			if( data[i] == '[' )
			{
				/**h = new HeaderElem();*/
				if( (in_header == false) && (before_game == false))
				{
				
					in_header = true;
				}

				if( before_header )
				{
					before_header = false;
				}
			}

			char cc = data[i];
		
			if( NewLine(data,i) )
			{
			
				if( i+2 < source_mf->size()-1 &&
					data[i+1] != EOF && data[i+2] != EOF)
				{
					if( NextLinePattern(data,i))
					{
						uint64 j;
						for( j = i+2; j < source_mf->size() && data[j]!= EOF; j++)
						{
							char c = data[j];
							if( ((data[j] == '[')&& before_game) || ((data[j] > '0' && data[j] < '9')&&!before_game) )
								break;
						}
				
						if( in_header && !before_game)
						{

							before_game = true;
							in_header = false;
						}
						else if( before_game && !in_header)
						{
							virtualGameCount++;
						
						
							// parse header here
							gameCount++;

							before_header = true;
							before_game = false;
							in_header = false;

						}
						

						continue;
					}

				}
			}
			else
			{

			}
		}

		memFiles_CONTAINER.pMemFileContainer->gameCount = virtualGameCount;
	}
	else
	{
		gameCount = memFiles_CONTAINER.pMemFileContainer->gameCount;
	}

	//========================================================================
	// End pre-scan
	//========================================================================

	wxMessageBox(wxString::Format("GameCount: %d", gameCount));

	if( memFiles_CONTAINER.pMemFileContainer->gameIndexMemPool == NULL )
	{
		memFiles_CONTAINER.pMemFileContainer->gameIndexMemPool = new MemPool(sizeof(VM_GameIndex));
		string newFileName = string(sourceName)+"IndexData.dat";
		if(!memFiles_CONTAINER.pMemFileContainer->gameIndexMemPool->OpenMemoryFileMemory((char*)newFileName.c_str(), true, gameCount * sizeof(VM_GameIndex)))
		{
			wxMessageBox("Could Not Index Memory File");
			return 0;
		}
		gameIndexMemPool = memFiles_CONTAINER.pMemFileContainer->gameIndexMemPool;
		//virtualGameCount
	}
	else
	{
		vm_fileIsLoaded = true;
		gameIndexMemPool = memFiles_CONTAINER.pMemFileContainer->gameIndexMemPool;
		virtualGameCount = memFiles_CONTAINER.pMemFileContainer->gameCount;
		return vm_data;
	}

	//this->gameIndexes32M = new GameIndex32Moves*[virtualGameCount];

	// first check to see if a file exists 
	string tempStr = sourceName;
	int dot_pos = tempStr.find(".");

	// query size of file
	// load the last compressed entry already in the file, 
	// write the next one on to the end ... or get file size, set that as the start of the 
	// next one, and simply write the data, for the name lists, they need to be loaded
	// up again, another name has to be pushed into the name list, and then it 
	// is saved again...
	// see above for any memory optimizations, perhaps too much is in memory, it takes too
	// long etc

	string outputfileName = tempStr.substr(0, dot_pos-1);
	string preloadFileName = outputfileName;
	preloadFileName += ".indx";

	// should be a VM file, this file should exist not as this file name


//	board = new ChessBoard();

	//fscanf(source_mf.handle(),
	//const char* data = source_mf_p->data();
	//vm_data = data;
	before_header = true;
	in_header = false;
	before_game = false;

	if( startI32M )
	{
		delete startI;
		startI32M = NULL;
	}
	startI32M = new GameIndex32Moves;
	GameIndex32Moves *curr = startI32M;
	GameIndex32Moves *pStart = startI32M;
	HeaderElemC *h;

	//gameIndexes32M[0] = new GameIndex32Moves();
//	m_vecGameIndexes.push_back( new GameIndex32Moves() ) ;

	//m_SortableIndexes.Allocate(virtualGameCount);
	int over100Val = virtualGameCount / 100;
	int percentageTicks = 0;

	virtualGameCount = 0;
	VM_GameIndex memoryGameIndex;
	MemBlock *mb = gameIndexMemPool->GetBlock();


	/*						cont = dialog.Pulse( "Indexing Games", &skip);
						

						if( !cont || !skip )
						{
						   if ( wxMessageBox(wxT("Do you really want to cancel?"),
											  wxT("Progress dialog question"),  // caption
											  wxYES_NO | wxICON_QUESTION) == wxYES )
								return 0;
						}
	
	*/
	gameCount = 0;

	//m_SortableIndexes.push_back(new SortableGameIndex(new GameIndex32Moves(), 0));

	for(uint64 i = 0; (i < source_mf_p->size()-1) && (data[i] != EOF); i++ )
	{
		double progVal = double(i) / double(fifty_percent);
		progVal *= 100;
		//progVal /= 2;
		//progress.Update(1, "Indexing ... Please Wait");

		
		if( data[i] == '[' )
		{
			/**h = new HeaderElem();*/
			if( (in_header == false) && (before_game == false) )
			{
				
				in_header = true;
			}

			if( before_header )
			{
			//	curr->header_start = i;
				memoryGameIndex.header_start = i;
				//gameIndexes32M[gameCount]->header_start = i;
				//m_vecGameIndexes[gameCount]->header_start = i;
				//m_SortableIndexes[gameCount]->m_Gi->header_start = i;
				before_header = false;
			}
		}
		
		if( NewLine(data,i) )
		{
			
			if( i+2 < source_mf_p->size()-1 &&
				data[i+1] != EOF && data[i+2] != EOF)
			{
				if( NextLinePattern(data,i))
				{
					uint64 j;
					for( j = i+2; j < source_mf_p->size() && data[j]!= EOF; j++)
					{
						char c = data[j];
						if( ((data[j] == '[')&& before_game) || ((data[j] > '0' && data[j] < '9')&&!before_game) )
							break;
					}
				
					//cout<< " "<<data[j] <<endl;
					if( in_header && !before_game)
					{
						/**curr->elems.push_back(h);*/
						//curr->header_end = i;//-(int)curr->header_start;
						//curr->game_start = j;//-(int)curr->header_start;
						memoryGameIndex.header_end = i;
						memoryGameIndex.game_start = j;
						//gameIndexes32M[gameCount]->header_end = i;
						//gameIndexes32M[gameCount]->game_start = j;
						//m_vecGameIndexes[gameCount]->header_end = i;
						//m_vecGameIndexes[gameCount]->game_start = j;
						//m_SortableIndexes[gameCount]->m_Gi->header_end = i;
						//m_SortableIndexes[gameCount]->m_Gi->game_start = j;
						before_game = true;
						in_header = false;
					}
					else if( before_game && !in_header)
					{
						//gameIndexes32M[gameCount]->game_end = i;
						//m_vecGameIndexes[gameCount]->game_end = i;
						//m_SortableIndexes[gameCount]->m_Gi->game_end = i;

						virtualGameCount++;
						memoryGameIndex.game_end = i;
						//curr->game_end = i;//-(int)curr->header_start;
						// parse header here
						gameCount++;

						//m_SortableIndexes.push_back(new SortableGameIndex(new GameIndex32Moves(), gameCount));
						//gameIndexes32M[gameCount] = new GameIndex32Moves();
						//m_vecGameIndexes.push_back(new GameIndex32Moves());
						RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(VM_GameIndex));
						
							delete mb;

						mb = gameIndexMemPool->GetBlock();

						/*percentageTicks++;

						bool skip = true;
						bool cont = true;
						*/
 
						//curr->pNext = new GameIndex;
						//curr = curr->pNext;
						before_header = true;
						before_game = false;
						in_header = false;
					}
						
					//cout<< string(data+i,data+i+1);
					continue;
				}

			}
		
			/**if (in_header)
			  curr->elems.push_back(h);*/
		}
		else
		{
			/**if( in_header )
				h->data += data[i];*/
		}
	}

	//delete mb;

	//progress.Close(true);



	if( curr->game_end == 0 )
	{
		curr->game_end = source_mf_p->size()-1;

	}

	string srcNm(sourceName);
	int lastDotScrNm = srcNm.find_last_of(".");
	string toSaveSrcNm = "";
	toSaveSrcNm = srcNm.substr(0,lastDotScrNm);
	string toSaveNonTemp = toSaveSrcNm +".vmdat";
	toSaveSrcNm += "_temp.vmdat";

	memFiles_CONTAINER.pMemFileContainer->vm_compressed_games = new MemoryFile(toSaveSrcNm.c_str(),
		MemoryFile::e_open_mode::if_exists_truncate_if_not_exists_create);

	memFiles_CONTAINER.pMemFileContainer->vm_compressed_games->resize(gameCount*sizeof(CompressedHeader)+ gameCount*sizeof(unsigned short)*1000);
	//gameIndexMemPool->
	//SaveGames32M(toSaveSrcNm);

	VM_GameIndex tempVMgi;
	GameIndex32Moves tempGi32M;
	int currOffset = 0;
	
	for( int i = 0; i < gameCount; i++ )
	{

/*		bool cont = true;
		bool skip = true;
		cont = dialog.Update(i/(gameCount/100), "Parsing Games", &skip);

		if( !cont || !skip )
		{
			if ( wxMessageBox(wxT("Do you really want to cancel?"),
								wxT("Progress dialog question"),  // caption
								wxYES_NO | wxICON_QUESTION) == wxYES )
				return 0;
		}*/
		//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
		RtlCopyMemory(&tempVMgi, gameIndexMemPool->GetDataOffset(i), sizeof(VM_GameIndex));

		ParseHeader32M( data + tempVMgi.header_start, 
			tempVMgi.header_end - tempVMgi.header_start, 
			headersListData,
			&tempGi32M);

		// now that this information has been generated, it needs
		// to be written to the file - AND it should probably be a VM file.
		// Also, the INDEX needs to be updated, that is "tempVMgi"
		// The "tempGi32M with its header" is written to a file and the 
		// offsets are put into the tempVMgi ...
		// or ... a temporary Array is created, maybe with a vector< GameIndex32Moves* >
		// then this array is written to disk, the memory pool data is created (overwritten) 
		// with the new information for accessing the compressed data.
		// eventually all the files can be linked together so the need for 1 database will
		// be unneccessary 
		if( ParseGame32M( data + tempVMgi.game_start, 
			tempVMgi.game_end - tempVMgi.game_start, 
			headersListData, 
			&tempGi32M) == false )
		{
			this->error_games.push_back(i);
		}


		tempVMgi.header_start = currOffset;

		// copy the compressed header
		RtlCopyMemory( memFiles_CONTAINER.pMemFileContainer->vm_compressed_games->data() + currOffset, 
			&tempGi32M.hdr, sizeof(CompressedHeader));

		currOffset += sizeof(CompressedHeader);
	
		tempVMgi.header_end = currOffset;





		RtlCopyMemory( memFiles_CONTAINER.pMemFileContainer->vm_compressed_games->data() + currOffset, 
			&tempGi32M.nummoves, sizeof(unsigned short int));

		tempVMgi.game_start = currOffset;


		currOffset += sizeof(unsigned short int);





		RtlCopyMemory( memFiles_CONTAINER.pMemFileContainer->vm_compressed_games->data() + currOffset, 
			tempGi32M.moves, sizeof(unsigned short int)*tempGi32M.nummoves );

		currOffset += sizeof(unsigned short int)*tempGi32M.nummoves;

		tempVMgi.game_end = currOffset;




		// now write the game index back into the pool.
		RtlCopyMemory(gameIndexMemPool->GetDataOffset(i), &tempVMgi , sizeof(VM_GameIndex));

		if( tempGi32M.moves )
		{
			delete tempGi32M.moves;
			tempGi32M.moves = 0;
		}
	}

	// now get the last index and set that as the size of the new file.

	RtlCopyMemory(&tempVMgi, gameIndexMemPool->GetDataOffset(gameCount-1), sizeof(VM_GameIndex));
	MemoryFile *nonTemp = CopyMemoryFile(memFiles_CONTAINER.pMemFileContainer->vm_compressed_games, toSaveNonTemp.c_str(), true, tempVMgi.game_end);


	delete memFiles_CONTAINER.pMemFileContainer->vm_compressed_games;
	memFiles_CONTAINER.pMemFileContainer->vm_compressed_games = new MemoryFile(toSaveSrcNm.c_str(),
		MemoryFile::e_open_mode::if_exists_truncate_if_not_exists_create);

	memFiles_CONTAINER.pMemFileContainer->vm_compressed_games->resize(sizeof(unsigned short)*1000);
	//gameIndexMemPool->	
	delete memFiles_CONTAINER.pMemFileContainer->vm_compressed_games;

	memFiles_CONTAINER.pMemFileContainer->vm_compressed_games = nonTemp;
	//nonTemp->resize(tempVMgi.game_end);
	//gameIndexMemPool->



	// pointer is SET
	g_pHeadersList = &headersListData;
	g_pDataPageList = &memFiles_CONTAINER;
	headersListData.PerformSort();
	//headersListData.DeleteTrees();

	vm_fileIsLoaded = true;

	tree_filtered_indexes.reserve(this->virtualGameCount);

	for( int i = 0; i < virtualGameCount; i++ )
	{
		tree_filtered_indexes.push_back(i);
	}

	/*arrayGi = new GameIndex*[game_count];

	GameIndex *temp = pStart;
	for( int i = 0; i < game_count; i++ )
	{
		arrayGi[i] = temp;
		temp = temp->pNext;
	}
	*/


	/*if( !WriteVM_PGNIndexFile(outputfileName, pStart ) )
	{
		// error
	}*/
	/*string srcNm(sourceName);
	int lastDotScrNm = srcNm.find_last_of(".");
	string toSaveSrcNm = "";
	toSaveSrcNm = srcNm.substr(0,lastDotScrNm);
	toSaveSrcNm += ".pgnc2";
	SaveGames32M(toSaveSrcNm);
	*/
	return data;
}

const char* PgnLoad::AddIndexVM_PGN_32M(const char* sourceName, void* user_data)

{
	/*if( source_mf_p )
	{
		delete source_mf_p;
		source_mf_p = NULL;

	}*/

	int old_vgc = virtualGameCount;
	InputMemoryFile *source_mf = 0;
	memFiles_CONTAINER.pMemFileContainer= 0;

	bool bFoundMemFile = false;
	for( int i = 0; i < this->memFiles_CONTAINER.m_pages.size(); i++ )
	{
		if( this->memFiles_CONTAINER.m_pages[i]->filename == string(sourceName) )
		{
			bFoundMemFile = true;
			source_mf = memFiles_CONTAINER.m_pages[i]->source_mf_p;
			memFiles_CONTAINER.pMemFileContainer = this->memFiles_CONTAINER.m_pages[i];
		}
	}

	if( bFoundMemFile == false )
	{
		source_mf = new InputMemoryFile(sourceName);//,MemoryFile::if_exists_truncate_if_not_exists_create);
		memFiles_CONTAINER.pMemFileContainer = new MemoryFileContainer();
		memFiles_CONTAINER.pMemFileContainer->source_mf_p = source_mf;
		memFiles_CONTAINER.pMemFileContainer->filename = string(sourceName);

		this->memFiles_CONTAINER.m_pages.push_back(memFiles_CONTAINER.pMemFileContainer);
		memFiles_CONTAINER.pMemFileContainer->Index = this->memFiles_CONTAINER.m_pages.size()-1;

		
	}

	source_mf_p = source_mf;


	if (! source_mf->data()) 
	{
		vm_read_error = "Cannot access VM data";
			return 0;
	}

	const char* data = source_mf->data();
	vm_data = data;
	bool before_header = true;
	bool in_header = false;
	bool before_game = false;

//	this->headersListData.Clear();
	virtualGameCount = 0;
	int gameCount = 0;


//	wxProgressDialog progress("Progress Dialog", "Indexing PGN, Please Wait", 100, (wxWindow*)user_data);

	//progress.Show(true);

	int fifty_percent = (source_mf->size()-1);
	//========================================================================
	//  pre-scan
	//========================================================================

	if( bFoundMemFile == false )
	{
		
		for(uint64 i = 0; (i < source_mf->size()-1) && (data[i] != EOF); i++ )
		{
			double progVal = double(i) / double(fifty_percent);
			progVal *= 100;
			progVal /= 2;
	//		progress.Update(1, "Pre-Scanning ... Please Wait");
			if( data[i] == '[' )
			{
				/**h = new HeaderElem();*/
				if( (in_header == false) && (before_game == false))
				{
				
					in_header = true;
				}

				if( before_header )
				{
					before_header = false;
				}
			}

			char cc = data[i];
		
			if( NewLine(data,i) )
			{
			
				if( i+2 < source_mf->size()-1 &&
					data[i+1] != EOF && data[i+2] != EOF)
				{
					if( NextLinePattern(data,i))
					{
						uint64 j;
						for( j = i+2; j < source_mf->size() && data[j]!= EOF; j++)
						{
							char c = data[j];
							if( ((data[j] == '[')&& before_game) || ((data[j] > '0' && data[j] < '9')&&!before_game) )
								break;
						}
				
						if( in_header && !before_game)
						{

							before_game = true;
							in_header = false;
						}
						else if( before_game && !in_header)
						{
							virtualGameCount++;
						
						
							// parse header here
							gameCount++;

							before_header = true;
							before_game = false;
							in_header = false;
						}
						

						continue;
					}

				}
			}
			else
			{

			}
		}

		memFiles_CONTAINER.pMemFileContainer->gameCount = virtualGameCount;
	}
	else
	{
		gameCount = memFiles_CONTAINER.pMemFileContainer->gameCount;
	}

	//========================================================================
	// End pre-scan
	//========================================================================

	if( memFiles_CONTAINER.pMemFileContainer->gameIndexMemPool == NULL )
	{
		memFiles_CONTAINER.pMemFileContainer->gameIndexMemPool = new MemPool(sizeof(VM_GameIndex));
		string newFileName = string(sourceName)+"IndexData.dat";
		if(!memFiles_CONTAINER.pMemFileContainer->gameIndexMemPool->OpenMemoryFileMemory((char*)newFileName.c_str(), true, gameCount * sizeof(VM_GameIndex)))
		{
			wxMessageBox("Could Not Index Memory File");
			return 0;
		}
		gameIndexMemPool = memFiles_CONTAINER.pMemFileContainer->gameIndexMemPool;
		//virtualGameCount
	}
	else
	{
		vm_fileIsLoaded = true;
		gameIndexMemPool = memFiles_CONTAINER.pMemFileContainer->gameIndexMemPool;
		virtualGameCount = memFiles_CONTAINER.pMemFileContainer->gameCount;
		return vm_data;
	}

	//this->gameIndexes32M = new GameIndex32Moves*[virtualGameCount];

	// first check to see if a file exists 
	string tempStr = sourceName;
	int dot_pos = tempStr.find(".");

	// query size of file
	// load the last compressed entry already in the file, 
	// write the next one on to the end ... or get file size, set that as the start of the 
	// next one, and simply write the data, for the name lists, they need to be loaded
	// up again, another name has to be pushed into the name list, and then it 
	// is saved again...
	// see above for any memory optimizations, perhaps too much is in memory, it takes too
	// long etc

	string outputfileName = tempStr.substr(0, dot_pos-1);
	string preloadFileName = outputfileName;
	preloadFileName += ".indx";

	// should be a VM file, this file should exist not as this file name


//	board = new ChessBoard();

	//fscanf(source_mf.handle(),
	//const char* data = source_mf_p->data();
	//vm_data = data;
	before_header = true;
	in_header = false;
	before_game = false;

	if( startI32M )
	{
		delete startI;
		startI32M = NULL;
	}
	startI32M = new GameIndex32Moves;
	GameIndex32Moves *curr = startI32M;
	GameIndex32Moves *pStart = startI32M;
	HeaderElemC *h;

	//gameIndexes32M[0] = new GameIndex32Moves();
//	m_vecGameIndexes.push_back( new GameIndex32Moves() ) ;

	//m_SortableIndexes.reserve(old_vgc + virtualGameCount);

	virtualGameCount = 0;
	VM_GameIndex memoryGameIndex;
	MemBlock *mb = memFiles_CONTAINER.pMemFileContainer->gameIndexMemPool->GetBlock();
	
	
	gameCount = 0;

//	m_SortableIndexes.push_back(new SortableGameIndex(new GameIndex32Moves(), old_vgc));

	for(uint64 i = 0; (i < source_mf_p->size()-1) && (data[i] != EOF); i++ )
	{
		double progVal = double(i) / double(fifty_percent);
		progVal *= 100;
		//progVal /= 2;
		//progress.Update(1, "Indexing ... Please Wait");
		
		if( data[i] == '[' )
		{
			/**h = new HeaderElem();*/
			if( (in_header == false) && (before_game == false) )
			{
				
				in_header = true;
			}

			if( before_header )
			{
			//	curr->header_start = i;
				memoryGameIndex.header_start = i;
				//gameIndexes32M[gameCount]->header_start = i;
				//m_vecGameIndexes[gameCount]->header_start = i;
				//m_SortableIndexes[gameCount]->m_Gi->header_start = i;
				before_header = false;
			}
		}
		
		if( NewLine(data,i) )
		{
			
			if( i+2 < source_mf_p->size()-1 &&
				data[i+1] != EOF && data[i+2] != EOF)
			{
				if( NextLinePattern(data,i))
				{
					uint64 j;
					for( j = i+2; j < source_mf_p->size() && data[j]!= EOF; j++)
					{
						char c = data[j];
						if( ((data[j] == '[')&& before_game) || ((data[j] > '0' && data[j] < '9')&&!before_game) )
							break;
					}
				
					//cout<< " "<<data[j] <<endl;
					if( in_header && !before_game)
					{
						/**curr->elems.push_back(h);*/
						//curr->header_end = i;//-(int)curr->header_start;
						//curr->game_start = j;//-(int)curr->header_start;
						memoryGameIndex.header_end = i;
						memoryGameIndex.game_start = j;
						//gameIndexes32M[gameCount]->header_end = i;
						//gameIndexes32M[gameCount]->game_start = j;
						//m_vecGameIndexes[gameCount]->header_end = i;
						//m_vecGameIndexes[gameCount]->game_start = j;
						//m_SortableIndexes[gameCount]->m_Gi->header_end = i;
						//m_SortableIndexes[gameCount]->m_Gi->game_start = j;
						before_game = true;
						in_header = false;
					}
					else if( before_game && !in_header)
					{
						//gameIndexes32M[gameCount]->game_end = i;
						//m_vecGameIndexes[gameCount]->game_end = i;
						//m_SortableIndexes[gameCount]->m_Gi->game_end = i;

						virtualGameCount++;
						memoryGameIndex.game_end = i;
						//curr->game_end = i;//-(int)curr->header_start;
						// parse header here
						gameCount++;

						//m_SortableIndexes.push_back(new SortableGameIndex(new GameIndex32Moves(), gameCount));
						//gameIndexes32M[gameCount] = new GameIndex32Moves();
						//m_vecGameIndexes.push_back(new GameIndex32Moves());
						RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(VM_GameIndex));
						
							delete mb;

						mb = memFiles_CONTAINER.pMemFileContainer->gameIndexMemPool->GetBlock();
						//curr->pNext = new GameIndex;
						//curr = curr->pNext;
						before_header = true;
						before_game = false;
						in_header = false;
					}
						
					//cout<< string(data+i,data+i+1);
					continue;
				}

			}
		
			/**if (in_header)
			  curr->elems.push_back(h);*/
		}
		else
		{
			/**if( in_header )
				h->data += data[i];*/
		}
	}

	//delete mb;

	//progress.Close(true);

	if( curr->game_end == 0 )
	{
		curr->game_end = source_mf_p->size()-1;

	}

	string srcNm(sourceName);
	int lastDotScrNm = srcNm.find_last_of(".");
	string toSaveSrcNm = "";
	toSaveSrcNm = srcNm.substr(0,lastDotScrNm);
	string toSaveNonTemp = toSaveSrcNm +".vmdat";
	toSaveSrcNm += "_temp.vmdat";

	

	memFiles_CONTAINER.pMemFileContainer->vm_compressed_games = new MemoryFile(toSaveSrcNm.c_str(),
		MemoryFile::e_open_mode::if_exists_truncate_if_not_exists_create);

	memFiles_CONTAINER.pMemFileContainer->vm_compressed_games->resize(gameCount*sizeof(CompressedHeader)+ gameCount*sizeof(unsigned short)*1000);
	//gameIndexMemPool->
	//SaveGames32M(toSaveSrcNm);
	if( memFiles_CONTAINER.pMemFileContainer->vm_compressed_games->size() != gameCount*sizeof(CompressedHeader)+ gameCount*sizeof(unsigned short)*1000)
	{
		wxMessageBox("Failed to create 2nd Memory File, Error!");
		return 0;
	}

	VM_GameIndex tempVMgi;
	GameIndex32Moves tempGi32M;
	int currOffset = 0;
	for( int i = 0; i < gameCount; i++ )
	{
		//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
		RtlCopyMemory(&tempVMgi, memFiles_CONTAINER.pMemFileContainer->gameIndexMemPool->GetDataOffset(i), sizeof(VM_GameIndex));

		ParseHeader32M( data + tempVMgi.header_start, 
			tempVMgi.header_end - tempVMgi.header_start, 
			headersListData,
			&tempGi32M);

		// now that this information has been generated, it needs
		// to be written to the file - AND it should probably be a VM file.
		// Also, the INDEX needs to be updated, that is "tempVMgi"
		// The "tempGi32M with its header" is written to a file and the 
		// offsets are put into the tempVMgi ...
		// or ... a temporary Array is created, maybe with a vector< GameIndex32Moves* >
		// then this array is written to disk, the memory pool data is created (overwritten) 
		// with the new information for accessing the compressed data.
		// eventually all the files can be linked together so the need for 1 database will
		// be unneccessary 
		ParseGame32M( data + tempVMgi.game_start, 
			tempVMgi.game_end - tempVMgi.game_start, 
			headersListData, 
			&tempGi32M);


		tempVMgi.header_start = currOffset;

		// copy the compressed header
		RtlCopyMemory( memFiles_CONTAINER.pMemFileContainer->vm_compressed_games->data() + currOffset, 
			&tempGi32M.hdr, sizeof(CompressedHeader));

		currOffset += sizeof(CompressedHeader);
	
		tempVMgi.header_end = currOffset;





		RtlCopyMemory( memFiles_CONTAINER.pMemFileContainer->vm_compressed_games->data() + currOffset, 
			&tempGi32M.nummoves, sizeof(unsigned short int));

		tempVMgi.game_start = currOffset;

		currOffset += sizeof(unsigned short int);





		RtlCopyMemory( memFiles_CONTAINER.pMemFileContainer->vm_compressed_games->data() + currOffset, 
			tempGi32M.moves, sizeof(unsigned short int)*tempGi32M.nummoves );

		currOffset += sizeof(unsigned short int)*tempGi32M.nummoves;

		tempVMgi.game_end = currOffset;




		// now write the game index back into the pool.
		RtlCopyMemory(memFiles_CONTAINER.pMemFileContainer->gameIndexMemPool->GetDataOffset(i), &tempVMgi , sizeof(VM_GameIndex));

		if( tempGi32M.moves )
		{
			delete tempGi32M.moves;
			tempGi32M.moves = 0;
		}
	}

	// TIDY UP BIG CRAPPY MEMFILES WITH OVER ESTIMATED SIZES
	RtlCopyMemory(&tempVMgi, gameIndexMemPool->GetDataOffset(gameCount-1), sizeof(VM_GameIndex));
	MemoryFile *nonTemp = CopyMemoryFile(memFiles_CONTAINER.pMemFileContainer->vm_compressed_games, toSaveNonTemp.c_str(), true, tempVMgi.game_end);


	delete memFiles_CONTAINER.pMemFileContainer->vm_compressed_games;
	memFiles_CONTAINER.pMemFileContainer->vm_compressed_games = new MemoryFile(toSaveSrcNm.c_str(),
		MemoryFile::e_open_mode::if_exists_truncate_if_not_exists_create);

	memFiles_CONTAINER.pMemFileContainer->vm_compressed_games->resize(sizeof(unsigned short)*1000);
	//gameIndexMemPool->	
	delete memFiles_CONTAINER.pMemFileContainer->vm_compressed_games;

	memFiles_CONTAINER.pMemFileContainer->vm_compressed_games = nonTemp;



	memFiles_CONTAINER.mode = MEMFILE_CONTAINER_MODE_LINKED;

	this->virtualGameCount += old_vgc;

	tree_filtered_indexes.reserve(this->virtualGameCount);

	for( int i = 0; i < virtualGameCount; i++ )
	{
		tree_filtered_indexes.push_back(i);
	}

	// pointer is SET
	g_pHeadersList = &headersListData;
	g_pDataPageList = &memFiles_CONTAINER;

	//headersListData.
	headersListData.PerformSort();
	//headersListData.DeleteTrees();

	vm_fileIsLoaded = true;
	/*arrayGi = new GameIndex*[game_count];

	GameIndex *temp = pStart;
	for( int i = 0; i < game_count; i++ )
	{
		arrayGi[i] = temp;
		temp = temp->pNext;
	}
	*/


	/*if( !WriteVM_PGNIndexFile(outputfileName, pStart ) )
	{
		// error
	}*/

//	FILE* file = fopen(memFiles_CONTAINER.m_pages[0]->filename.c_str(), "wb");

//	if( !file )
//		return data;

/*	int dot_pos2 = memFiles_CONTAINER.m_pages[0]->filename.find_last_of(".");

	if( dot_pos2 == -1 )
		return data;
	
	string headersListOutput = memFiles_CONTAINER.m_pages[0]->filename.substr(0,dot_pos2-3);
	string pagesData = headersListOutput + "_pages.dat";



	//headersListOutput += ".hls";
	this->headersListData.WriteLists(headersListOutput);

//	fclose(file);
	
	
	FILE * DataPageInfo = fopen(pagesData.c_str(), "wb");

	int numPages = memFiles_CONTAINER.m_pages.size();

	fwrite( &numPages, sizeof(int), 1, DataPageInfo);
	
	for( int i = 0; i < numPages; i++ )
	{
		string tempStrFilename = memFiles_CONTAINER.m_pages[i]->filename;
		tempStrFilename += "\n";
		fwrite(tempStrFilename.c_str(), sizeof(char), tempStrFilename.length(), DataPageInfo);  
	}

	fclose(DataPageInfo);*/

	return data;
}

const char* PgnLoad::IndexVM_PGN(const char* sourceName, void* user_data)
{
	/*if( source_mf_p )
	{
		delete source_mf_p;
		source_mf_p = NULL;

	}*/

	InputMemoryFile *source_mf = 0;
	pMemFileContainer= 0;

	bool bFoundMemFile = false;
	for( int i = 0; i < this->memFiles.size(); i++ )
	{
		if( this->memFiles[i]->filename == string(sourceName) )
		{
			bFoundMemFile = true;
			source_mf = memFiles[i]->source_mf_p;
			pMemFileContainer = this->memFiles[i];
		}
	}

	if( bFoundMemFile == false )
	{
		source_mf = new InputMemoryFile(sourceName);//,MemoryFile::if_exists_truncate_if_not_exists_create);
		pMemFileContainer = new MemoryFileContainer();
		pMemFileContainer->source_mf_p = source_mf;
		pMemFileContainer->filename = string(sourceName);

		this->memFiles.push_back(pMemFileContainer);

		
	}

	source_mf_p = source_mf;


	if (! source_mf->data()) 
	{
		vm_read_error = "Cannot access VM data";
			return 0;
	}

	const char* data = source_mf->data();
	vm_data = data;
	bool before_header = true;
	bool in_header = false;
	bool before_game = false;

	virtualGameCount = 0;
	int gameCount = 0;


//	wxProgressDialog progress("Progress Dialog", "Indexing PGN, Please Wait", 100, (wxWindow*)user_data);

	//progress.Show(true);

	int fifty_percent = (source_mf->size()-1);
	//========================================================================
	//  pre-scan
	//========================================================================

	if( bFoundMemFile == false )
	{
		
		for(uint64 i = 0; (i < source_mf->size()-1) && (data[i] != EOF); i++ )
		{
			double progVal = double(i) / double(fifty_percent);
			progVal *= 100;
			progVal /= 2;
	//		progress.Update(1, "Pre-Scanning ... Please Wait");
			if( data[i] == '[' )
			{
				/**h = new HeaderElem();*/
				if( (in_header == false) && (before_game == false))
				{
				
					in_header = true;
				}

				if( before_header )
				{
					before_header = false;
				}
			}

			char cc = data[i];
		
			if( NewLine(data,i) )
			{
			
				if( i+2 < source_mf->size()-1 &&
					data[i+1] != EOF && data[i+2] != EOF)
				{
					if( NextLinePattern(data,i))
					{
						uint64 j;
						for( j = i+2; j < source_mf->size() && data[j]!= EOF; j++)
						{
							char c = data[j];
							if( ((data[j] == '[')&& before_game) || ((data[j] > '0' && data[j] < '9')&&!before_game) )
								break;
						}
				
						if( in_header && !before_game)
						{

							before_game = true;
							in_header = false;
						}
						else if( before_game && !in_header)
						{
							virtualGameCount++;
						
						
							// parse header here
							gameCount++;

							before_header = true;
							before_game = false;
							in_header = false;
						}
						

						continue;
					}

				}
			}
			else
			{

			}
		}

		pMemFileContainer->gameCount = virtualGameCount;
	}
	else
	{
		gameCount = pMemFileContainer->gameCount;
	}

	//========================================================================
	// End pre-scan
	//========================================================================

	if( pMemFileContainer->gameIndexMemPool == NULL )
	{
		pMemFileContainer->gameIndexMemPool = new MemPool(sizeof(GameIndex));
		string newFileName = string(sourceName)+"IndexData.dat";
		if(!pMemFileContainer->gameIndexMemPool->OpenMemoryFileMemory((char*)newFileName.c_str(), true, gameCount * sizeof(GameIndex)))
		{
			wxMessageBox("Could Not Index Memory File");
			return 0;
		}
		gameIndexMemPool = pMemFileContainer->gameIndexMemPool;
		//virtualGameCount
	}
	else
	{
		vm_fileIsLoaded = true;
		gameIndexMemPool = pMemFileContainer->gameIndexMemPool;
		virtualGameCount = pMemFileContainer->gameCount;
		return vm_data;
	}

	// first check to see if a file exists 
	string tempStr = sourceName;
	int dot_pos = tempStr.find(".");

	// query size of file
	// load the last compressed entry already in the file, 
	// write the next one on to the end ... or get file size, set that as the start of the 
	// next one, and simply write the data, for the name lists, they need to be loaded
	// up again, another name has to be pushed into the name list, and then it 
	// is saved again...
	// see above for any memory optimizations, perhaps too much is in memory, it takes too
	// long etc

	string outputfileName = tempStr.substr(0, dot_pos-1);
	string preloadFileName = outputfileName;
	preloadFileName += ".indx";

	// should be a VM file, this file should exist not as this file name
	/*
	struct stat   buffer;   
    if( (stat (preloadFileName.c_str(), &buffer) == 0))
	{
		cout<< "Found File"<<endl;
		FILE* pIndexFile = fopen(preloadFileName.c_str(), "rb");
		if( pIndexFile )
		{
			start_game = new GameIndex;
			
			//GameIndex *ptemp = start_game;
			int offset = 0;
			game_count = 0;
			int retVal = 0;
			while(offset < buffer.st_size )
			{
				MemBlock *mb = (*mempool)->GetBlock();

				GameIndex ptemp;

				retVal = fread((void*)&ptemp.header_start, sizeof(ptemp.header_start), 1, pIndexFile);
				offset += retVal;
				if( retVal == 0 )
				{
					break;
				}
				retVal = fread((void*)&ptemp.header_end, sizeof(ptemp.header_end), 1, pIndexFile); 
				offset += retVal;
				if( retVal == 0 )
				{
					break;
				}
				retVal = fread((void*)&ptemp.game_start, sizeof(ptemp.game_start), 1, pIndexFile);
				offset += retVal;
				if( retVal == 0 )
				{
					break;
				}
				retVal = fread((void*)&ptemp.game_end, sizeof(ptemp.game_end), 1, pIndexFile);
				offset += retVal;
				if( retVal == 0 )
				{
					break;
				}
				game_count++;

				RtlCopyMemory(mb->data, &ptemp, sizeof(GameIndex));
				
				delete mb;

				//GameIndex *pTemp2 = new GameIndex;
				//ptemp->pNext = pTemp2;
				//ptemp = pTemp2;
			}

			//delete ptemp;

			fclose(pIndexFile);
			source_mf_p = new InputMemoryFile(source);//,MemoryFile::if_exists_truncate_if_not_exists_create);
			if (! source_mf_p->data()) 
			{
				cout<<"Error 1"<<endl;
				return 0;
			}
			//arrayGi = new GameIndex*[game_count];

			//GameIndex *temp = start_game ;


			return source_mf_p->data();
		}

	}

	*/

//	board = new ChessBoard();

	//fscanf(source_mf.handle(),
	//const char* data = source_mf_p->data();
	//vm_data = data;
	before_header = true;
	in_header = false;
	before_game = false;

	if( startI )
	{
		delete startI;
		startI = NULL;
	}
	startI = new GameIndex;
	GameIndex *curr = startI;
	GameIndex *pStart = startI;
	HeaderElemC *h;
	virtualGameCount = 0;
	GameIndex memoryGameIndex;
	MemBlock *mb = gameIndexMemPool->GetBlock();
	
	
	gameCount = 0;
	for(uint64 i = 0; (i < source_mf_p->size()-1) && (data[i] != EOF); i++ )
	{
		double progVal = double(i) / double(fifty_percent);
		progVal *= 100;
		//progVal /= 2;
		//progress.Update(1, "Indexing ... Please Wait");
		
		if( data[i] == '[' )
		{
			/**h = new HeaderElem();*/
			if( (in_header == false) && (before_game == false) )
			{
				
				in_header = true;
			}

			if( before_header )
			{
				curr->header_start = i;
				memoryGameIndex.header_start = i;
				before_header = false;
			}
		}
		
		if( NewLine(data,i) )
		{
			
			if( i+2 < source_mf_p->size()-1 &&
				data[i+1] != EOF && data[i+2] != EOF)
			{
				if( NextLinePattern(data,i))
				{
					uint64 j;
					for( j = i+2; j < source_mf_p->size() && data[j]!= EOF; j++)
					{
						char c = data[j];
						if( ((data[j] == '[')&& before_game) || ((data[j] > '0' && data[j] < '9')&&!before_game) )
							break;
					}
				
					//cout<< " "<<data[j] <<endl;
					if( in_header && !before_game)
					{
						/**curr->elems.push_back(h);*/
						curr->header_end = i;//-(int)curr->header_start;
						curr->game_start = j;//-(int)curr->header_start;
						memoryGameIndex.header_end = i;
						memoryGameIndex.game_start = j;
						before_game = true;
						in_header = false;
					}
					else if( before_game && !in_header)
					{
						virtualGameCount++;
						memoryGameIndex.game_end = i;
						curr->game_end = i;//-(int)curr->header_start;
						// parse header here
						gameCount++;

						RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));

						delete mb;

						mb = gameIndexMemPool->GetBlock();
						//curr->pNext = new GameIndex;
						//curr = curr->pNext;
						before_header = true;
						before_game = false;
						in_header = false;
					}
						
					//cout<< string(data+i,data+i+1);
					continue;
				}

			}
		
			/**if (in_header)
			  curr->elems.push_back(h);*/
		}
		else
		{
			/**if( in_header )
				h->data += data[i];*/
		}
	}

	delete mb;

	//progress.Close(true);

	if( curr->game_end == 0 )
	{
		curr->game_end = source_mf_p->size()-1;

	}

	vm_fileIsLoaded = true;
	/*arrayGi = new GameIndex*[game_count];

	GameIndex *temp = pStart;
	for( int i = 0; i < game_count; i++ )
	{
		arrayGi[i] = temp;
		temp = temp->pNext;
	}
	*/


	/*if( !WriteVM_PGNIndexFile(outputfileName, pStart ) )
	{
		// error
	}*/
}

// Add BlockSize for each fileyy
bool PgnLoad::SaveDB(string &filename)
{
	int dot_pos2 = filename.find_last_of(".");

	if( dot_pos2 == -1 )
		return false;
	
	string headersListOutput = filename.substr(0,dot_pos2);
	



	//headersListOutput += ".hls";
	this->headersListData.WriteLists(headersListOutput);


	string pagesData = filename.substr(0,dot_pos2) + "_pages.dat";

//	fclose(file);
	
	
	FILE * DataPageInfo = fopen(pagesData.c_str(), "wb");

	if( !DataPageInfo )
	{
		return false;
	}

	int numPages = memFiles_CONTAINER.m_pages.size();

	fwrite( &numPages, sizeof(int), 1, DataPageInfo);

	for( int i = 0; i < numPages; i++ )
	{
		fwrite(&memFiles_CONTAINER.m_pages[i]->gameCount, sizeof(int), 1, DataPageInfo);
	}
	
	for( int i = 0; i < numPages; i++ )
	{
		string tempStrFilename = memFiles_CONTAINER.m_pages[i]->filename;
		tempStrFilename += "\n";
		
		fwrite(tempStrFilename.c_str(), sizeof(char), tempStrFilename.length(), DataPageInfo);  
	}

	fclose(DataPageInfo);
}

bool PgnLoad::LoadDB_32_VM(string &filename)
{
	virtualGameCount = 0;
	int dot_pos2 = filename.find_last_of(".");

	if( dot_pos2 == -1 )
		return false;
	
	string headersListOutput = filename.substr(0,dot_pos2);
	



	//headersListOutput += ".hls";
	this->headersListData.ReadLists(headersListOutput);
	headersListData.PerformSort();
	//headersListData.DeleteTrees();

	


	string pagesData = filename.substr(0,dot_pos2) + "_pages.dat";

	FILE * DataPageInfo = fopen(pagesData.c_str(), "rb");

	if( !DataPageInfo )
	{
		return false;
	}

	int numPages = 0;//memFiles_CONTAINER.m_pages.size();

	fread( &numPages, sizeof(int), 1, DataPageInfo);

	vector<string> vec_strings;
	
	vector<int> vec_gameCount;

	for( int i = 0; i < numPages; i++ )
	{
		int tempGameCount = 0;
		fread(&tempGameCount, sizeof(int), 1, DataPageInfo);
		vec_gameCount.push_back(tempGameCount);
	}

	{
		string tempStrFilename = "";//memFiles_CONTAINER.m_pages[i]->filename;
		//tempStrFilename += "\n";
		char c;
		
		string name = "";
		do {
			c = fgetc (DataPageInfo);
				
			if( c == '\n' )
			{
				vec_strings.push_back(name);
				name = "";
			}
			else
			{
				name += c;
			}
		} while (c != EOF);
    
		//fclose (inpt);
		
	}

	fclose(DataPageInfo);

	for( int i = 0; i < vec_strings.size(); i++ )
	{

		string srcNm(vec_strings[i]);
		int lastDotScrNm = srcNm.find_last_of(".");
		string toSaveSrcNm = "";
		toSaveSrcNm = srcNm.substr(0,lastDotScrNm);
		toSaveSrcNm += ".vmdat";


		memFiles_CONTAINER.m_pages.push_back(new MemoryFileContainer());

		memFiles_CONTAINER.m_pages[i]->filename = srcNm;
		memFiles_CONTAINER.m_pages[i]->Index = i;
		memFiles_CONTAINER.m_pages[i]->gameCount = vec_gameCount[i];

		memFiles_CONTAINER.m_pages[i]->vm_compressed_games = new MemoryFile(toSaveSrcNm.c_str(),
			MemoryFile::e_open_mode::if_exists_keep_if_dont_exists_fail);


		memFiles_CONTAINER.m_pages[i]->gameIndexMemPool = new MemPool(sizeof(VM_GameIndex));
		
		
		
		string newFileName = string(srcNm)+"IndexData.dat";

		if(!memFiles_CONTAINER.m_pages[i]->gameIndexMemPool->OpenMemoryFileMemoryRead((char*)newFileName.c_str(), true, vec_gameCount[i] * sizeof(VM_GameIndex)))
		{
			wxMessageBox("Could Not Index Memory File");
			return 0;
		}

		virtualGameCount += vec_gameCount[i];

	
	}

	tree_filtered_indexes.reserve(this->virtualGameCount);

	for( int i = 0; i < virtualGameCount; i++ )
	{
		tree_filtered_indexes.push_back(i);
	}

	g_pHeadersList = &headersListData;
	g_pDataPageList = &memFiles_CONTAINER;

	SORTABLE_INDEX_MODE = SORTABLE_GAME_INDEX_MODE_VM;
	memFiles_CONTAINER.mode = MEMFILE_CONTAINER_MODE_LINKED;
	filter = false;
	vm_fileIsLoaded = true;
	sort_method = 1;

}

bool PgnLoad::AddDB_32_VM(string &filename)
{

	return false;
}


void PgnLoad::SortAscendingRAM(int id)
{
	// GET FILTER TYPE
	// THEN SORT
	// THEN RE-APPLY FILTER
	sorted_filtered = false;
	if( filter != true )
	{
		switch(id)
		{
		case 0:
		case 1: heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_data1, this->m_SortableIndexes.size()); break;
		case 2: heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_bname, this->m_SortableIndexes.size()); break;
		case 3: heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_elow, this->m_SortableIndexes.size()); break;
		case 4: heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_elob, this->m_SortableIndexes.size()); break;
		case 5: heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_Site, this->m_SortableIndexes.size()); break;
		case 6: heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_Event, this->m_SortableIndexes.size()); break;
		case 7: heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_Date, this->m_SortableIndexes.size()); break;
		//case 8: heap_sort_GameIndexes32M(this->gameIndexes32M, cmpfun_gameIndexes32m_data1, this->virtualGameCount); break;
		default:
			heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_data1, this->m_SortableIndexes.size()); break;
			break;
		}
	}
	else
	{
		//m_filteredIndexes.push_back(m_SortableIndexes[i]);
		sorted_filtered = true;
		switch(id)
		{
		case 0:
		case 1: heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_data1, this->m_filteredIndexes.size()); break;
		case 2: heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_bname, this->m_filteredIndexes.size()); break;
		case 3: heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_elow, this->m_filteredIndexes.size()); break;
		case 4: heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_elob, this->m_filteredIndexes.size()); break;
		case 5: heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_Site, this->m_filteredIndexes.size()); break;
		case 6: heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_Event, this->m_filteredIndexes.size()); break;
		case 7: heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_Date, this->m_filteredIndexes.size()); break;
		//case 8: heap_sort_GameIndexes32M(this->gameIndexes32M, cmpfun_gameIndexes32m_data1, this->virtualGameCount); break;
		default:
			heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_data1, this->m_filteredIndexes.size()); break;
			break;
		}
	}
}
void PgnLoad::SortDescendingRAM(int id)
{
	sorted_filtered = false;
	if( filter != true )
	{
		switch(id)
		{
		case 0:
		case 1: heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_wname_Inv, this->m_SortableIndexes.size()); break;
		case 2: heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_bname_Inv, this->m_SortableIndexes.size()); break;
		case 3: heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_elow_Inv, this->m_SortableIndexes.size()); break;
		case 4: heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_elob_Inv, this->m_SortableIndexes.size()); break;
		case 5: heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_Site_Inv, this->m_SortableIndexes.size()); break;
		case 6: heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_Event_Inv, this->m_SortableIndexes.size()); break;
		case 7: heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_Date_Inv, this->m_SortableIndexes.size()); break;
		//case 8: heap_sort_GameIndexes32M(this->gameIndexes32M, cmpfun_gameIndexes32m_data1, this->virtualGameCount); break;
		default:
			heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_moves, this->m_SortableIndexes.size()); break;
			break;
		}
	}
	else
	{
		sorted_filtered = true;
		//m_filteredIndexes.push_back(m_SortableIndexes[i]);
		switch(id)
		{
		case 0:
		case 1: heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_wname_Inv, this->m_filteredIndexes.size()); break;
		case 2: heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_bname_Inv, this->m_filteredIndexes.size()); break;
		case 3: heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_elow_Inv, this->m_filteredIndexes.size()); break;
		case 4: heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_elob_Inv, this->m_filteredIndexes.size()); break;
		case 5: heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_Site_Inv, this->m_filteredIndexes.size()); break;
		case 6: heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_Event_Inv, this->m_filteredIndexes.size()); break;
		case 7: heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_Date_Inv, this->m_filteredIndexes.size()); break;
		//case 8: heap_sort_GameIndexes32M(this->gameIndexes32M, cmpfun_gameIndexes32m_data1, this->virtualGameCount); break;
		default:
			heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_data1, this->m_filteredIndexes.size()); break;
			break;
		}
	}
}

bool PgnLoad::CompressGame( PGN::Game *pGame, GameIndex *game_index, ChessBoard *board )
{
		int numPly= 0;//pGame->CountMoves();

		PGN::Move *pCountMoves = pGame->pRoot;
		while( pCountMoves )
		{
			if( pCountMoves->white != "" )
				numPly++;
			else break;

			if( pCountMoves->black != "" )
				numPly++;
			else break;

			pCountMoves = pCountMoves->pNext;
		}
		game_index->moves.Allocate(numPly);

		PGN::Move * tempMove = pGame->pRoot;

		//cout<< "Testing Compress" << endl;

		board->SetInitialPosition();
		int plyCount = 0;
		while( tempMove )
		{
			if( (plyCount >= game_index->moves.plyCount) || (tempMove->white == "") )
				break;
			board->GenLegalMoves();
			board->parseMove(tempMove->white);
			tempMove->SetFenWhite(board->Fen());
			tempMove->SetWhiteMoveSquares(board->currMove);
			board->SetBoard(tempMove->m_fenWhite);
			bool legalMoveWhiteFound = false;
			for( unsigned char nmLm = 0; nmLm < (unsigned char)board->numLegalMoves; nmLm++ )
			{
				if( tempMove->whiteMove == board->legal_moves[nmLm].mv )
				{
					legalMoveWhiteFound=  true;
					game_index->moves.SetMove(plyCount, nmLm );
					break;
				}
			}
			if( !legalMoveWhiteFound )
				return false;

			plyCount++;
			
			if(( plyCount >= game_index->moves.plyCount )||(tempMove->black == "") )
				break;

			board->GenLegalMoves();
			board->parseMove(tempMove->black);
			tempMove->SetFenBlack(board->Fen());
			tempMove->SetBlackMoveSquares(board->currMove);
			board->SetBoard(tempMove->m_fenBlack);

			bool legalMoveBlackFound = false;
			for( unsigned char nmLm = 0; nmLm < (unsigned char)board->numLegalMoves; nmLm++ )
			{
				if( tempMove->blackMove == board->legal_moves[nmLm].mv )
				{
					legalMoveBlackFound= true;
					game_index->moves.SetMove(plyCount, nmLm );
					break;
				}
			}
			if( !legalMoveBlackFound )
				return false;

			plyCount++;


			tempMove = tempMove->pNext;
		}

	return true;
}

bool PgnLoad::CompressGameVM( PGN::Game *pGame, GameIndex *game_index, ChessBoard *board )
{
		int numPly= 0;//pGame->CountMoves();

		string gameStr = "";
		for( uint64 v = game_index->game_start; v < game_index->game_end; v++ )
		{
			pGame->game_text += vm_data[v];
		}

		if( !pGame->Parse() )
		{
			return false;
		}

		PGN::Move *pCountMoves = pGame->pRoot;
		while( pCountMoves )
		{
			if( pCountMoves->white != "" )
				numPly++;
			else break;

			if( pCountMoves->black != "" )
				numPly++;
			else break;

			pCountMoves = pCountMoves->pNext;
		}
		game_index->moves.Allocate(numPly);

		PGN::Move * tempMove = pGame->pRoot;

		//cout<< "Testing Compress" << endl;

		board->SetInitialPosition();
		int plyCount = 0;
		while( tempMove )
		{
			if( (plyCount >= game_index->moves.plyCount) || (tempMove->white == "") )
				break;
			board->GenLegalMoves();
			board->parseMove(tempMove->white);
			tempMove->SetFenWhite(board->Fen());
			tempMove->SetWhiteMoveSquares(board->currMove);
			board->SetBoard(tempMove->m_fenWhite);
			bool legalMoveWhiteFound = false;
			for( unsigned char nmLm = 0; nmLm < (unsigned char)board->numLegalMoves; nmLm++ )
			{
				if( tempMove->whiteMove == board->legal_moves[nmLm].mv )
				{
					legalMoveWhiteFound=  true;
					game_index->moves.SetMove(plyCount, nmLm );
					break;
				}
			}
			if( !legalMoveWhiteFound )
				return false;

			plyCount++;
			
			if(( plyCount >= game_index->moves.plyCount )||(tempMove->black == "") )
				break;

			board->GenLegalMoves();
			board->parseMove(tempMove->black);
			tempMove->SetFenBlack(board->Fen());
			tempMove->SetBlackMoveSquares(board->currMove);
			board->SetBoard(tempMove->m_fenBlack);

			bool legalMoveBlackFound = false;
			for( unsigned char nmLm = 0; nmLm < (unsigned char)board->numLegalMoves; nmLm++ )
			{
				if( tempMove->blackMove == board->legal_moves[nmLm].mv )
				{
					legalMoveBlackFound= true;
					game_index->moves.SetMove(plyCount, nmLm );
					break;
				}
			}
			if( !legalMoveBlackFound )
				return false;

			plyCount++;


			tempMove = tempMove->pNext;
		}
}

string PgnLoad::GetHeaderData( string &matchName, int gameID )
{
	GameIndex *temp = new GameIndex();
	RtlCopyMemory(temp, gameIndexMemPool->GetDataOffset(gameID), sizeof(GameIndex));

	//const char* data = memFileData;// = data;
	uint64 header_start = temp->header_start;
	uint64 header_end = temp->header_end;
	bool start_line = false;
	bool end_line = false;
	string str = "";
	for( uint64 h = header_start; h < header_end; h++ )
	{
		if( vm_data[h] == '[' )
		{
			str = "";
			start_line = true;
			end_line = false;
			//pH = new HeaderElem;
			continue;
		}
		if( vm_data[h] == ']' )
		{
			if( str.length() > matchName.length() )
			{
				int first_space = str.find(" ");
				if( str.substr(0, matchName.length()) == matchName )
				{
					//if( first_space == matchName.length() )
					{
						string toRet =  GetSubstringInQuotes(str);
						if( toRet.length() > 0 )
							toRet.resize( std::remove_if( toRet.begin(), toRet.end(), filter_func) - toRet.begin());
						else return "empty";
						
						return toRet;
					}
				}
			}
			//vpH.push_back(pH);
			end_line = true;
			start_line = false;
			continue;
		}

		if( start_line )
		{
			str += vm_data[h];
			//pH->data += data[h];
		}
	}

	delete temp;

	return "empty";
}

Game* PgnLoad::GetRAMGame(int vmIndex)
{
	Game* game = new Game();

	if( this->sort_method == 1 )
	{
		vmIndex++;
		/*if( (filter && !sorted_filtered ))
		{
			//if( !sorted_filtered || (sorted_filtered && filter))
			
		}
		else 
		{
			
		}*/
		//if( this->currI32M )
		//{
		//	delete this->currI32M;
		//}

		//this->currI32M = new GameIndex32M;
	
		//RtlCopyMemory(this->currI, gameIndexMemPool->GetDataOffset(vmIndex), sizeof(GameIndex));

		if( vmIndex > this->virtualGameCount )
		{
			return game;
		}
		else
		{
			if( this->filter == true )
			{
				if( vmIndex < this->m_filteredIndexes.size() )
				{
					ChessBoard tempBoard;
					tempBoard.SetInitialPosition();
					Move* tempBoardMv = game->pRoot;
					bool endWhite = false;

					int infoCount = 1;
					for( int vv = 0; vv < m_filteredIndexes[vmIndex]->m_Gi->nummoves; vv++ )
					{
						MyChessMove mv;
						mv.packed = m_filteredIndexes[vmIndex]->m_Gi->moves[vv++];
						if( mv.to.pad == 1 )
						{
							mv.promo = mv.from.pad;
						}
						string mvStringW = mv.toString();
						mvStringW[1]--;
						mvStringW[3]--;
					
						string wsan = tempBoard.EPN_Move_to_PGN_Move(mvStringW, (mv.to.pad==1?true:false),mv.promo); 
					
						if(tempBoardMv)
						{
							char buff[10];
							sprintf( buff, "%d", infoCount++ );
							tempBoardMv->info += buff;
							tempBoardMv->white = wsan;
							tempBoardMv->whiteMove = mv;
							tempBoardMv->SetFenWhite( tempBoard.Fen() );
						}


						tempBoardMv->SetMoveFlag( MOVE_FLAG_X );

						if( (vv >= m_filteredIndexes[vmIndex]->m_Gi->nummoves) )
						{
						
							endWhite = true;
							break;
						}

						MyChessMove mv2;
						mv2.packed = m_filteredIndexes[vmIndex]->m_Gi->moves[vv];
						if( mv2.to.pad == 1 )
						{
							mv2.promo = mv2.from.pad;
						}
						string mvStringB = mv2.toString();
						mvStringB[1]--;
						mvStringB[3]--;
					
						string bsan = tempBoard.EPN_Move_to_PGN_Move(mvStringB, (mv2.to.pad==1?true:false),mv2.promo); 
					
						tempBoardMv->black = bsan;
						tempBoardMv->blackMove = mv2;
						tempBoardMv->SetFenBlack( tempBoard.Fen() );

						tempBoardMv->SetMoveFlag( MOVE_FLAG_Y );

						if( vv < m_filteredIndexes[vmIndex]->m_Gi->nummoves-1)
						{
							tempBoardMv->pNext = new Move();
							tempBoardMv->pNext->pPrev = tempBoardMv;
							tempBoardMv = tempBoardMv->pNext;
						}
					

					}

					if( endWhite )
					{
						tempBoardMv->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
					}
					else
			
					{
						tempBoardMv->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					}

					HeaderElem h;
					h.data = "[White \"";

					h.data += headersListData.LookupPlayerName( m_filteredIndexes[vmIndex]->m_Gi->hdr.wname );
					h.data += "\"]";

					HeaderElem h2;
					h2.data = "[Black \"";
					h2.data += headersListData.LookupPlayerName( m_filteredIndexes[vmIndex]->m_Gi->hdr.bname );
					h2.data += "\"]";

					HeaderElem h7;
					h7.data = "[WhiteElo \"";
					char welobuff[20];
					sprintf(welobuff, "%d",headersListData.LookupElo( m_filteredIndexes[vmIndex]->m_Gi->hdr.whiteElo ));
					h7.data += welobuff;//headersListData.LookupElo( m_filteredIndexes[vmIndex]->m_Gi->hdr.whiteElo );
					h7.data += "\"]";

					HeaderElem h8;
					h8.data = "[BlackElo \"";
				//	h8.data += headersListData.LookupElo( m_filteredIndexes[vmIndex]->m_Gi->hdr.blackElo );
					char belobuff[20];
					sprintf(belobuff, "%d",headersListData.LookupElo( m_filteredIndexes[vmIndex]->m_Gi->hdr.blackElo ));
					h8.data += belobuff;					
					h8.data += "\"]";

					HeaderElem h9;
					h9.data = "[ECO \"";
					h9.data += headersListData.LookupEcoName( m_filteredIndexes[vmIndex]->m_Gi->hdr.eco );
					h9.data += "\"]";

					HeaderElem h3;
					h3.data = "[Event \"";
					h3.data += headersListData.LookupEventName( m_filteredIndexes[vmIndex]->m_Gi->hdr.eventname );
					h3.data += "\"]";

					HeaderElem h4;
					h4.data = "[Site \"";
					h4.data += headersListData.LookupSiteName( m_filteredIndexes[vmIndex]->m_Gi->hdr.sitename );
					h4.data += "\"]";

					HeaderElem h5;
					h5.data = "[Result \"";

					if( m_filteredIndexes[vmIndex]->m_Gi->hdr.result == (unsigned char)0 )
					{
						string resultNode = "1-0";
						h5.data += resultNode;
						
						game->AddResultNode(resultNode);
					}
					else if( m_filteredIndexes[vmIndex]->m_Gi->hdr.result == (unsigned char)1 )
					{
						string resultNode = "0-1";
						h5.data += resultNode;
						game->AddResultNode(resultNode);
					}
					else if( m_filteredIndexes[vmIndex]->m_Gi->hdr.result == (unsigned char)2 )
					{
						string resultNode = "1/2-1/2";
						h5.data += resultNode;
						game->AddResultNode(resultNode);
					}

					h5.data += "\"]";

					HeaderElem h6;
					h6.data = "[Date \"";
					h6.data += headersListData.LookupDateName( m_filteredIndexes[vmIndex]->m_Gi->hdr.date );
					h6.data += "\"]";

					game->vec_header.push_back(h);
					game->vec_header.push_back(h2);
					game->vec_header.push_back(h3);
					game->vec_header.push_back(h4);
					game->vec_header.push_back(h6);
					game->vec_header.push_back(h7);
					game->vec_header.push_back(h8);
					game->vec_header.push_back(h9);

					

				}
				else 
				{
					return game;
				}
			}
			else
			{
			//	wxMessageBox(wxString::Format("%d, %d",this->gameIndexes32M[vmIndex]->game_end, this->gameIndexes32M[vmIndex]->game_start ));
				//LoadGameHeaderIntoCurrent32M(this->gameIndexes32M[vmIndex], game);

				/*for( uint64 v = this->gameIndexes32M[vmIndex]->game_start; v < this->gameIndexes32M[vmIndex]->game_end; v++ )
				{
					game->game_text += vm_data[v];
				}*/

				ChessBoard tempBoard;
				tempBoard.SetInitialPosition();
				Move* tempBoardMv = game->pRoot;
				bool endWhite = false;

				int infoCount = 1;
				for( int vv = 0; vv < m_SortableIndexes[vmIndex]->m_Gi->nummoves; vv++ )
				{
					MyChessMove mv;
					mv.packed = m_SortableIndexes[vmIndex]->m_Gi->moves[vv++];
					if( mv.to.pad == 1 )
					{
						mv.promo = mv.from.pad;
					}
					string mvStringW = mv.toString();
					mvStringW[1]--;
					mvStringW[3]--;
					
					string wsan = tempBoard.EPN_Move_to_PGN_Move(mvStringW, (mv.to.pad==1?true:false),mv.promo); 
					
					if(tempBoardMv)
					{
						char buff[10];
						sprintf( buff, "%d", infoCount++ );
						tempBoardMv->info += buff;
						tempBoardMv->white = wsan;
						tempBoardMv->whiteMove = mv;
						tempBoardMv->SetFenWhite( tempBoard.Fen() );
					}


					tempBoardMv->SetMoveFlag( MOVE_FLAG_X );

					if( (vv >= m_SortableIndexes[vmIndex]->m_Gi->nummoves) )
					{
						
						endWhite = true;
						break;
					}

					MyChessMove mv2;
					mv2.packed = m_SortableIndexes[vmIndex]->m_Gi->moves[vv];
					if( mv2.to.pad == 1 )
					{
						mv2.promo = mv2.from.pad;
					}
					string mvStringB = mv2.toString();
					mvStringB[1]--;
					mvStringB[3]--;
					
					string bsan = tempBoard.EPN_Move_to_PGN_Move(mvStringB, (mv2.to.pad==1?true:false),mv2.promo); 
					
					tempBoardMv->black = bsan;
					tempBoardMv->blackMove = mv2;
					tempBoardMv->SetFenBlack( tempBoard.Fen() );

					tempBoardMv->SetMoveFlag( MOVE_FLAG_Y );

					if( vv < m_SortableIndexes[vmIndex]->m_Gi->nummoves-1)
					{
						tempBoardMv->pNext = new Move();
						tempBoardMv->pNext->pPrev = tempBoardMv;
						tempBoardMv = tempBoardMv->pNext;
					}
					

				}

				if( endWhite )
				{
					tempBoardMv->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
				}
				else
			
				{
					tempBoardMv->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
				}

				HeaderElem h;
				h.data = "[White \"";

				h.data += headersListData.LookupPlayerName( m_SortableIndexes[vmIndex]->m_Gi->hdr.wname );
				h.data += "\"]";

				HeaderElem h2;
				h2.data = "[Black \"";
				h2.data += headersListData.LookupPlayerName( m_SortableIndexes[vmIndex]->m_Gi->hdr.bname );
				h2.data += "\"]";

				HeaderElem h7;
				h7.data = "[WhiteElo \"";
				char welobuff[20];
				sprintf(welobuff, "%d",headersListData.LookupElo( m_SortableIndexes[vmIndex]->m_Gi->hdr.whiteElo ));
				h7.data += welobuff;//headersListData.LookupElo( m_filteredIndexes[vmIndex]->m_Gi->hdr.whiteElo );
				h7.data += "\"]";

				HeaderElem h8;
				h8.data = "[BlackElo \"";
			//	h8.data += headersListData.LookupElo( m_filteredIndexes[vmIndex]->m_Gi->hdr.blackElo );
				char belobuff[20];
				sprintf(belobuff, "%d",headersListData.LookupElo( m_SortableIndexes[vmIndex]->m_Gi->hdr.blackElo ));
				h8.data += belobuff;					
				h8.data += "\"]";

				HeaderElem h9;
				h9.data = "[ECO \"";
				h9.data += headersListData.LookupEcoName( m_SortableIndexes[vmIndex]->m_Gi->hdr.eco );
				h9.data += "\"]";

				HeaderElem h3;
				h3.data = "[Event \"";
				h3.data += headersListData.LookupEventName( m_SortableIndexes[vmIndex]->m_Gi->hdr.eventname );
				h3.data += "\"]";

				HeaderElem h4;
				h4.data = "[Site \"";
				h4.data += headersListData.LookupSiteName( m_SortableIndexes[vmIndex]->m_Gi->hdr.sitename );
				h4.data += "\"]";

				HeaderElem h5;
				h5.data = "[Result \"";

				if( m_SortableIndexes[vmIndex]->m_Gi->hdr.result == (unsigned char)0 )
				{
					string resultNode = "1-0";
					h5.data += resultNode;
					game->AddResultNode(resultNode);
				}
				else if( m_SortableIndexes[vmIndex]->m_Gi->hdr.result == (unsigned char)1 )
				{
					string resultNode = "0-1";
					h5.data += resultNode;
					game->AddResultNode(resultNode);
				}
				else if( m_SortableIndexes[vmIndex]->m_Gi->hdr.result == (unsigned char)2 )
				{
						string resultNode = "1/2-1/2";
						h5.data += resultNode;
						game->AddResultNode(resultNode);
				}

				h5.data += "\"]";

				HeaderElem h6;
				h6.data = "[Date \"";
				h6.data += headersListData.LookupDateName( m_SortableIndexes[vmIndex]->m_Gi->hdr.date );
				h6.data += "\"]";

				game->vec_header.push_back(h);
				game->vec_header.push_back(h2);
				game->vec_header.push_back(h3);
				game->vec_header.push_back(h4);
				game->vec_header.push_back(h6);
				game->vec_header.push_back(h7);
				game->vec_header.push_back(h8);
				game->vec_header.push_back(h9);
				
			}
		}
	}
	else
	{
		if( this->currI )
		{
			delete this->currI;
		}

		this->currI = new GameIndex;
	
		RtlCopyMemory(this->currI, gameIndexMemPool->GetDataOffset(vmIndex), sizeof(GameIndex));


		LoadGameHeaderIntoCurrent(currI, game);

	
		for( uint64 v = currI->game_start; v < currI->game_end; v++ )
		{
			game->game_text += vm_data[v];
		}



	
	}
	game->SourceFileName = this->vmFileName;
	game->sourceGameIndex = vmIndex;
	return game;
}

Game* PgnLoad::GetVMGame(int vmIndex)
{
	Game* game = new Game();

	//vmIndex--;
	int index = 0;
	if( filter == true )
	{
		index = tree_filtered_indexes[vmIndex];
	}
	else
	{
		index  = vmIndex;
	}
	
		if( this->currI )
		{
			delete this->currI;
		}

		this->currI = new GameIndex;

	if( memFiles_CONTAINER.mode == MEMFILE_CONTAINER_MODE_SEPARATE )
	{
	
				VM_GameIndex tempVMgi;
				GameIndex32Moves tempGi32M;
				

		//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
				MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(index);
				memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, index);
				pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );
				//RtlCopyMemory(&tempVMgi, gameIndexMemPool->GetDataOffset(index), sizeof(VM_GameIndex));
				//RtlCopyMemory(&tempGi32M.hdr, this->memFiles_CONTAINER.pMemFileContainer->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));
				RtlCopyMemory(&tempGi32M.nummoves, this->memFiles_CONTAINER.pMemFileContainer->vm_compressed_games->data() + tempVMgi.game_start, sizeof(unsigned short));
				tempGi32M.moves = new unsigned short[tempGi32M.nummoves];
				RtlCopyMemory(tempGi32M.moves, this->memFiles_CONTAINER.pMemFileContainer->vm_compressed_games->data() + tempVMgi.game_start + sizeof(unsigned short), sizeof(unsigned short)*tempGi32M.nummoves );



					ChessBoard tempBoard;
					tempBoard.SetInitialPosition();
					Move* tempBoardMv = game->pRoot;
					bool endWhite = false;

					int infoCount = 1;
					for( int vv = 0; vv < tempGi32M.nummoves; vv++ )
					{
						MyChessMove mv;
						mv.packed = tempGi32M.moves[vv++];
						if( mv.to.pad == 1 )
						{
							mv.promo = mv.from.pad;
						}
						string mvStringW = mv.toString();
						mvStringW[1]--;
						mvStringW[3]--;
					
						string wsan = tempBoard.EPN_Move_to_PGN_Move(mvStringW, (mv.to.pad==1?true:false),mv.promo); 
					
						if(tempBoardMv)
						{
							char buff[10];
							sprintf( buff, "%d", infoCount++ );
							tempBoardMv->info += buff;
							tempBoardMv->white = wsan;
							tempBoardMv->whiteMove = mv;
							tempBoardMv->SetFenWhite( tempBoard.Fen() );
						}


						tempBoardMv->SetMoveFlag( MOVE_FLAG_X );

						if( (vv >= tempGi32M.nummoves) )
						{
						
							endWhite = true;
							break;
						}

						MyChessMove mv2;
						mv2.packed = tempGi32M.moves[vv];
						if( mv2.to.pad == 1 )
						{
							mv2.promo = mv2.from.pad;
						}
						string mvStringB = mv2.toString();
						mvStringB[1]--;
						mvStringB[3]--;
					
						string bsan = tempBoard.EPN_Move_to_PGN_Move(mvStringB, (mv2.to.pad==1?true:false),mv2.promo); 
					
						tempBoardMv->black = bsan;
						tempBoardMv->blackMove = mv2;
						tempBoardMv->SetFenBlack( tempBoard.Fen() );

						tempBoardMv->SetMoveFlag( MOVE_FLAG_Y );

						if( vv < tempGi32M.nummoves-1)
						{
							tempBoardMv->pNext = new Move();
							tempBoardMv->pNext->pPrev = tempBoardMv;
							tempBoardMv = tempBoardMv->pNext;
						}
					

					}

					if( endWhite )
					{
						tempBoardMv->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
					}
					else
			
					{
						tempBoardMv->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					}

					HeaderElem h;
					h.data = "[White \"";

					h.data += headersListData.LookupPlayerName( tempGi32M.hdr.wname );
					h.data += "\"]";

					HeaderElem h2;
					h2.data = "[Black \"";
					h2.data += headersListData.LookupPlayerName( tempGi32M.hdr.bname );
					h2.data += "\"]";

					HeaderElem h7;
					h7.data = "[WhiteElo \"";
					char welobuff[20];
					sprintf(welobuff, "%d",headersListData.LookupElo( tempGi32M.hdr.whiteElo ));
					h7.data += welobuff;//headersListData.LookupElo( tempGi32M.hdr.whiteElo );
					h7.data += "\"]";

					HeaderElem h8;
					h8.data = "[BlackElo \"";
					char belobuff[20];
					sprintf(belobuff, "%d",headersListData.LookupElo( tempGi32M.hdr.blackElo ));
					h8.data += belobuff;
					//h8.data += headersListData.LookupElo( tempGi32M.hdr.blackElo );
					h8.data += "\"]";

					HeaderElem h9;
					h9.data = "[ECO \"";
					h9.data += headersListData.LookupEcoName( tempGi32M.hdr.eco );
					h9.data += "\"]";

					HeaderElem h3;
					h3.data = "[Event \"";
					h3.data += headersListData.LookupEventName( tempGi32M.hdr.eventname );
					h3.data += "\"]";

					HeaderElem h4;
					h4.data = "[Site \"";
					h4.data += headersListData.LookupSiteName( tempGi32M.hdr.sitename );
					h4.data += "\"]";

					HeaderElem h5;
					h5.data = "[Result \"";

					if( tempGi32M.hdr.result == (unsigned char)0 )
					{
						string resultNode = "1-0";
						h5.data += resultNode;
						game->AddResultNode(resultNode);
					}
					else if( tempGi32M.hdr.result == (unsigned char)1 )
					{
						string resultNode = "0-1";
						h5.data += resultNode;
						game->AddResultNode(resultNode);
					}
					else if( tempGi32M.hdr.result == (unsigned char)2 )
					{
						string resultNode = "1/2-1/2";
						h5.data += resultNode;
						game->AddResultNode(resultNode);
					}

					h5.data += "\"]";

					HeaderElem h6;
					h6.data = "[Date \"";
					h6.data += headersListData.LookupDateName( tempGi32M.hdr.date );
					h6.data += "\"]";

					game->vec_header.push_back(h);
					game->vec_header.push_back(h2);
					game->vec_header.push_back(h3);
					game->vec_header.push_back(h4);
					game->vec_header.push_back(h6);
					game->vec_header.push_back(h7);
					game->vec_header.push_back(h8);
					game->vec_header.push_back(h9);

					delete tempGi32M.moves ;
					tempGi32M.moves = 0;
	}
	else
	{
				VM_GameIndex tempVMgi;
				GameIndex32Moves tempGi32M;
				

		//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));

				MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(index);
				memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, index);
				pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );
				//RtlCopyMemory(&tempVMgi, memFiles_CONTAINER.GetDataOffset(index), sizeof(VM_GameIndex));
				
				//MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(index);
				//RtlCopyMemory(&tempGi32M.hdr, pMfc->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));
				RtlCopyMemory(&tempGi32M.nummoves, pMfc->vm_compressed_games->data() + tempVMgi.game_start, sizeof(unsigned short));
				tempGi32M.moves = new unsigned short[tempGi32M.nummoves];
				RtlCopyMemory(tempGi32M.moves, pMfc->vm_compressed_games->data() + tempVMgi.game_start + sizeof(unsigned short), sizeof(unsigned short)*tempGi32M.nummoves );



					ChessBoard tempBoard;
					tempBoard.SetInitialPosition();
					Move* tempBoardMv = game->pRoot;
					bool endWhite = false;

					int infoCount = 1;
					for( int vv = 0; vv < tempGi32M.nummoves; vv++ )
					{
						MyChessMove mv;
						mv.packed = tempGi32M.moves[vv++];
						if( mv.to.pad == 1 )
						{
							mv.promo = mv.from.pad;
						}
						string mvStringW = mv.toString();
						mvStringW[1]--;
						mvStringW[3]--;
					
						string wsan = tempBoard.EPN_Move_to_PGN_Move(mvStringW, (mv.to.pad==1?true:false),mv.promo); 
					
						if(tempBoardMv)
						{
							char buff[10];
							sprintf( buff, "%d", infoCount++ );
							tempBoardMv->info += buff;
							tempBoardMv->white = wsan;
							tempBoardMv->whiteMove = mv;
							tempBoardMv->SetFenWhite( tempBoard.Fen() );
						}


						tempBoardMv->SetMoveFlag( MOVE_FLAG_X );

						if( (vv >= tempGi32M.nummoves) )
						{
						
							endWhite = true;
							break;
						}

						MyChessMove mv2;
						mv2.packed = tempGi32M.moves[vv];
						if( mv2.to.pad == 1 )
						{
							mv2.promo = mv2.from.pad;
						}
						string mvStringB = mv2.toString();
						mvStringB[1]--;
						mvStringB[3]--;
					
						string bsan = tempBoard.EPN_Move_to_PGN_Move(mvStringB, (mv2.to.pad==1?true:false),mv2.promo); 
					
						tempBoardMv->black = bsan;
						tempBoardMv->blackMove = mv2;
						tempBoardMv->SetFenBlack( tempBoard.Fen() );

						tempBoardMv->SetMoveFlag( MOVE_FLAG_Y );

						if( vv < tempGi32M.nummoves-1)
						{
							tempBoardMv->pNext = new Move();
							tempBoardMv->pNext->pPrev = tempBoardMv;
							tempBoardMv = tempBoardMv->pNext;
						}
					

					}

					if( endWhite )
					{
						tempBoardMv->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
					}
					else
			
					{
						tempBoardMv->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
					}

					HeaderElem h;
					h.data = "[White \"";

					h.data += headersListData.LookupPlayerName( tempGi32M.hdr.wname );
					h.data += "\"]";

					HeaderElem h2;
					h2.data = "[Black \"";
					h2.data += headersListData.LookupPlayerName( tempGi32M.hdr.bname );
					h2.data += "\"]";

					HeaderElem h7;
					h7.data = "[WhiteElo \"";
					char welobuff[20];
					sprintf(welobuff, "%d",headersListData.LookupElo( tempGi32M.hdr.whiteElo ));
					h7.data += welobuff;//headersListData.LookupElo( tempGi32M.hdr.whiteElo );
					h7.data += "\"]";

					HeaderElem h8;
					h8.data = "[BlackElo \"";
					char belobuff[20];
					sprintf(belobuff, "%d",headersListData.LookupElo( tempGi32M.hdr.blackElo ));
					h8.data += belobuff;
					//h8.data += headersListData.LookupElo( tempGi32M.hdr.blackElo );
					h8.data += "\"]";

					HeaderElem h9;
					h9.data = "[ECO \"";
					h9.data += headersListData.LookupEcoName( tempGi32M.hdr.eco );
					h9.data += "\"]";

					HeaderElem h3;
					h3.data = "[Event \"";
					h3.data += headersListData.LookupEventName( tempGi32M.hdr.eventname );
					h3.data += "\"]";

					HeaderElem h4;
					h4.data = "[Site \"";
					h4.data += headersListData.LookupSiteName( tempGi32M.hdr.sitename );
					h4.data += "\"]";

					HeaderElem h5;
					h5.data = "[Result \"";

					if( tempGi32M.hdr.result == (unsigned char)0 )
					{
						string resultNode = "1-0";
						h5.data += resultNode;
						game->AddResultNode(resultNode);
					}
					else if( tempGi32M.hdr.result == (unsigned char)1 )
					{
						string resultNode = "0-1";
						h5.data += resultNode;
						game->AddResultNode(resultNode);
					}
					else if( tempGi32M.hdr.result == (unsigned char)2 )
					{
						string resultNode = "1/2-1/2";
						h5.data += resultNode;
						game->AddResultNode(resultNode);
					}

					h5.data += "\"]";

					HeaderElem h6;
					h6.data = "[Date \"";
					h6.data += headersListData.LookupDateName( tempGi32M.hdr.date );
					h6.data += "\"]";

					game->vec_header.push_back(h);
					game->vec_header.push_back(h2);
					game->vec_header.push_back(h3);
					game->vec_header.push_back(h4);
					game->vec_header.push_back(h6);
					game->vec_header.push_back(h7);
					game->vec_header.push_back(h8);
					game->vec_header.push_back(h9);

					delete tempGi32M.moves ;
					tempGi32M.moves = 0;
	}
	
	game->SourceFileName = this->vmFileName;
	game->sourceGameIndex = index;
	return game;
}

bool PgnLoad::MemFileDataIsOk()
{

	if( vm_fileIsLoaded && (vm_data!=0) && (virtualGameCount!=0) && 
		(gameIndexMemPool != 0 ) && (gameIndexMemPool->IsOk()) &&
		(source_mf_p!=0) && (source_mf_p->data() != 0 ) && (vm_read_error==""))
	{
		return true;
	}
	else return false;

}

bool PgnLoad::LoadCompressedGameIndexes(string filename)
{
	this->compressedFileName = filename;

	string tempStr = filename;
	
	int dot_pos = tempStr.find_last_of(".");

	string outputfileName =  tempStr.substr(0, dot_pos-3);


	headersListData.ReadLists(outputfileName);

	string cgf = outputfileName + "_cg.pgc3";
	string cif = outputfileName + "_ci.idx";
	string cirmp = outputfileName + "_cirmp.idx";
	FILE* compressedGame = fopen(cgf.c_str(), "rb");
	FILE* compressIndx = fopen(cif.c_str(), "rb");
	FILE* compressIndxRmp = fopen(cirmp.c_str(), "rb");

	if( !compressedGame )
	{
		return false;
	}

	//int 
	compressedGameCount = 0;
	fread( (void*)&compressedGameCount, sizeof(int), 1, compressedGame);

	compressedIndexes = new CompressedGameIndex*[compressedGameCount];

	int offset = 0;

	for( int i = 0; i < compressedGameCount; i++ )
	{
		compressedIndexes[i] = new CompressedGameIndex();
		compressedIndexes[i]->start = offset;
		compressedIndexes[i]->hdr.Read(compressedGame, offset);
		compressedIndexes[i]->moves.Read(compressedGame, offset);
	}

	CompressedGameLoaded = true;

/*	this->indexRemapRows = new IndexRemapRow*[compressedGameCount];

	int fileOffset;
	fread( (void*)&fileOffset, sizeof(int), 1, compressIndxRmp);
	for( int i = 0; i < compressedGameCount; i++ )
	{
		indexRemapRows[i] = new IndexRemapRow();
		fread((void*)indexRemapRows[i], sizeof(IndexRemapRow),1, compressIndxRmp);
	}*/

	fclose(compressedGame);
	fclose(compressIndx);
	fclose(compressIndxRmp);// = fopen(cirmp.c_str(), "rb");

	return true;
}

Game* PgnLoad::LoadCompressedGame(int idx, ChessBoard* board)
{

	Game* game = new Game();

	if( this->sortType == 0 )
	{
		game->sourceGameIndex = idx;
	}
	else
	{
		game->sourceGameIndex = indexRemapRows[idx]->type[sortType-1];
					
	}
	game->SourceFileName = compressedFileName;

	Move* pRoot = game->pRoot;
	Move* temp = pRoot;

		int plyCount = 0;

	CompressedGameIndex *compressed_game = compressedIndexes[game->sourceGameIndex];
	
	board->SetInitialPosition();
	int decompressMissed = 0;
	string fen;
	for( int i = 0; i < compressed_game->moves.plyCount; i++ )
	{
		if(( plyCount >= compressed_game->moves.plyCount ))//||(tempMove->white == "") )
		{
			break;
		}
		unsigned char indexWhite = compressed_game->moves.GetMove(plyCount);
		board->GenLegalMoves();
		MyChessMove mvWhite = board->legal_moves[indexWhite].mv;
		string whiteMvString = mvWhite.toString();
		whiteMvString[1]--;
		whiteMvString[3]--;
		string whiteSAN = board->EPN_Move_to_PGN_Move(whiteMvString, (mvWhite.promo ==-1?false:true), (mvWhite.promo ==-1?0:mvWhite.promo));
		temp->SetWhiteMoveSquares( mvWhite );
		temp->white = whiteSAN;
		//temp->x = 1;
		temp->SetMoveFlag( MOVE_FLAG_X );
		fen = board->Fen();

		temp->SetFenWhite(fen);
		//cout<< plyCount/2 + 1  << ". "<<whiteSAN << " ";//}


		board->SetBoard(fen);
		plyCount++;

		if(( plyCount >= compressed_game->moves.plyCount ))//||(tempMove->black == "") )
		{
			char plyBuff_exist_early[20];
			if( plyCount - 1 == 0 )
			{
				sprintf(plyBuff_exist_early, "%d", 1);
			}
			else 
			{
				sprintf(plyBuff_exist_early, "%d", (plyCount - 1) / 2 + 1);
			
			}
			temp->info = plyBuff_exist_early;
			//temp->atEndWhite = true;
			temp->SetMoveFlag( MOVE_FLAG_AT_END_WHITE );
			break;
		}
		unsigned char indexBlack = compressed_game->moves.GetMove(plyCount);
		board->GenLegalMoves();
		MyChessMove mvBlack = board->legal_moves[indexBlack].mv;
		string blackMvString = mvBlack.toString();
		blackMvString[1]--;
		blackMvString[3]--;
		string blackSAN = board->EPN_Move_to_PGN_Move(blackMvString, (mvBlack.promo ==-1?false:true), (mvBlack.promo ==-1?0:mvBlack.promo));
		fen = "";//board->Fen();
		temp->SetBlackMoveSquares( mvBlack );
		temp->black = blackSAN;
		//temp->y = 1;
		temp->SetMoveFlag( MOVE_FLAG_Y );
		fen = board->Fen();
		temp->SetFenBlack(fen);

		board->SetBoard(fen);
		
		// set the ply
		char plyBuff[20];
		if( plyCount - 1 == 0 )
		{
			sprintf(plyBuff, "%d", 1);
		}
		else 
		{
			sprintf(plyBuff, "%d", (plyCount - 1) / 2 + 1);
			
		}
		
		temp->info = plyBuff;

		plyCount++;

		if( i!= compressed_game->moves.plyCount-1 )
		{
			if( temp->pNext == NULL )
			{
				temp->pNext = new Move();
				temp->pNext->pPrev = temp;
				temp->pNext->pUp = NULL;
				temp->pNext->pNext = NULL;
			}

			temp = temp->pNext;
		}
		else
		{
			//temp->atEndBlack = true;
			temp->SetMoveFlag( MOVE_FLAG_AT_END_BLACK );
		}

	}

	board->SetInitialPosition();

	game->SetHasGeneratedFens();

	// now the header

	HeaderElem h;
	h.data = "[White \"";

	h.data += headersListData.LookupPlayerName( compressed_game->hdr.wname );
	h.data += "\"]";

	HeaderElem h2;
	h2.data = "[Black \"";
	h2.data += headersListData.LookupPlayerName( compressed_game->hdr.bname );
	h2.data += "\"]";

	HeaderElem h3;
	h3.data = "[Event \"";
	h3.data += headersListData.LookupEventName( compressed_game->hdr.eventname );
	h3.data += "\"]";

	HeaderElem h4;
	h4.data = "[Site \"";
	h4.data += headersListData.LookupSiteName( compressed_game->hdr.sitename );
	h4.data += "\"]";

	HeaderElem h5;
	h5.data = "[Result \"";

	if( compressed_game->hdr.result == (unsigned char)0 )
	{
		h5.data += "1-0";
	}
	else if( compressed_game->hdr.result == (unsigned char)1 )
	{
		h5.data += "0-1";
	}
	else if( compressed_game->hdr.result == (unsigned char)2 )
	{
		h5.data += "1/2-1/2";
	}

	h5.data += "\"]";

	HeaderElem h6;
	h6.data = "[Date \"";
	h6.data += headersListData.LookupDateName( compressed_game->hdr.date );
	h6.data += "\"]";

	game->vec_header.push_back(h);
	game->vec_header.push_back(h2);
	game->vec_header.push_back(h3);
	game->vec_header.push_back(h4);
	game->vec_header.push_back(h6);
	game->vec_header.push_back(h5);

	return game;

}

void PgnLoad::LoadGameHeaderIntoCurrent(GameIndex *curr, Game* currGame)
{
		uint64 header_start = curr->header_start;
		uint64 header_end = curr->header_end;
		bool start_line = false;
		bool end_line = false;
		//vector< HeaderElemC* > vpH;
		HeaderElem pH;
		for( uint64 h = header_start; h < header_end; h++ )
		{
			if( vm_data[h] == '[' )
			{
				start_line = true;
				end_line = false;
				//pH = new HeaderElem;
				pH.data = "";
				continue;
			}
			if( vm_data[h] == ']' )
			{
				currGame->vec_header.push_back(pH);
				end_line = true;
				start_line = false;
				continue;
			}

			if( start_line )
			{
				pH.data += vm_data[h];
			}
		}
}

void PgnLoad::LoadGameHeaderIntoCurrent32M( GameIndex32Moves* curr, Game* currGame)
{
	uint64 header_start = curr->header_start;
		uint64 header_end = curr->header_end;
		bool start_line = false;
		bool end_line = false;
		//vector< HeaderElemC* > vpH;
		HeaderElem pH;
		for( uint64 h = header_start; h < header_end; h++ )
		{
			if( vm_data[h] == '[' )
			{
				start_line = true;
				end_line = false;
				//pH = new HeaderElem;
				pH.data = "";
				continue;
			}
			if( vm_data[h] == ']' )
			{
				currGame->vec_header.push_back(pH);
				end_line = true;
				start_line = false;
				continue;
			}

			if( start_line )
			{
				pH.data += vm_data[h];
			}
		}
}

bool PgnLoad::PreScanPGN(string &filename)
{
	return false;
}


bool PgnLoad::OpenSQLDatabase(string &filename)
{
	this->dbname = filename;

	error = sqlite3_open(filename.c_str(), &db);
	if (error != SQLITE_OK)
    {
		wxMessageBox( wxString::Format( "Can't open database: %s\n", sqlite3_errmsg(db)));
		sqlite3_close (db); 
		return false;
    }

	



	error = sqlite3_exec(db, "SELECT count(*) FROM mytable;", callback, 0, &errMSG);
	if (error!= SQLITE_OK)
    {
		wxMessageBox( wxString::Format( "Can't count records in Database: %s\n", sqlite3_errmsg(db)));
		sqlite3_close (db); 
		return false;
    }	

	this->rec_count = 10000000;

	for( int i = 0; i < output.size(); i++ )
	{
		if( output[i] != "NULL")
		{
			wxMessageBox(output[i]);
			this->rec_count = atoi(output[i].c_str());
		}
	}

	return true;
}

Game* PgnLoad::LoadPGNFromDatabaseSelection(int sel)
{
	string sendToDB = "select Moves FROM mytable WHERE rowid = ";//1"

	char buff[50];
	sprintf( buff, "%d", sel );

	sendToDB+= buff;

	error = sqlite3_exec(db,sendToDB.c_str(), callback, 0, &errMSG);
    if( error!=SQLITE_OK )
	{
		wxMessageBox(wxString::Format("SQL error: %s\n", errMSG));
		sqlite3_free((void*)errMSG);

		sqlite3_close (db); 

		return 0;
    }

	Game* game = new Game();
	game->sourceGameIndex = sel;
	game->SourceFileName = dbname;

	if( output.size() != 0 )
	{
		game->game_text = output[0];
	}


	sendToDB = "select * FROM mytable WHERE rowid = ";//1"
	sendToDB+= buff;

	error = sqlite3_exec(db,sendToDB.c_str(), callback, 0, &errMSG);
    if( error!=SQLITE_OK )
	{
		wxMessageBox(wxString::Format("SQL error: %s\n", errMSG));
		sqlite3_free((void*)errMSG);

		sqlite3_close (db); 

		return 0;
	}

	for( int i = 1; i < output.size()-1; i++ )
	{
	//	wxMessageBox(output[i]);
		HeaderElem h;
		if( i == 1 )
		{
			h.data = "[White \"";
		}
		else if( i == 2 )
		{
			h.data = "[Black \"";
		}
		else if( i==3)
		{
			h.data = "[WhiteElo \"";
		}
		else if( i==4)
		{
			h.data = "[BlackElo \"";
		} 
		else if( i == 5)
		{
			h.data = "[Event \"";
		}
		else if( i==6)
		{
			h.data = "[Site \"";
		}
		else if( i==7)
		{
			h.data = "[Date \"";
		}
		else if( i==8)
		{
			h.data = "[Result \"";
		}
		else if( i==9)
		{
			h.data = "[ECO \"";
		}
		h.data += output[i];
		h.data += "\"]";
		game->vec_header.push_back(h);
	}

	
	game->ParseHeader();

	return game;
}

wxString PgnLoad::GetRowColumnData(int item, int column)
{
	if( (pgn_load_mode ==1) || (pgn_load_mode ==3) )
	{

		
		
		
		if( SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_VM )
		{
			VM_GameIndex tempVMgi;
			GameIndex32Moves tempGi32M;
			if( memFiles_CONTAINER.mode == MEMFILE_CONTAINER_MODE_SEPARATE )
			{
				if( filter == true  )
				{
					MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(this->tree_filtered_indexes[item]);
					memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, this->tree_filtered_indexes[item]);
					pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );
				//	RtlCopyMemory(&tempVMgi, gameIndexMemPool->GetDataOffset(this->tree_filtered_indexes[item]), sizeof(VM_GameIndex));
				//	RtlCopyMemory(&tempGi32M.hdr, this->memFiles_CONTAINER.pMemFileContainer->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));
				}
				else
				{
					MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(item);
					memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, item);
					pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );
		//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
					//RtlCopyMemory(&tempVMgi, gameIndexMemPool->GetDataOffset(item), sizeof(VM_GameIndex));
					//RtlCopyMemory(&tempGi32M.hdr, this->memFiles_CONTAINER.pMemFileContainer->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));

				}
			}
			else
			{
				if( filter == true )
				{
					MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(this->tree_filtered_indexes[item]);
					memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, this->tree_filtered_indexes[item]);
					pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );

				//	RtlCopyMemory(&tempVMgi, memFiles_CONTAINER.GetDataOffset(this->tree_filtered_indexes[item]), sizeof(VM_GameIndex));
				//	MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(this->tree_filtered_indexes[item]);
				//	RtlCopyMemory(&tempGi32M.hdr, pMfc->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));
				}
				else
				{
					MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(item);
					memFiles_CONTAINER.CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi, item);
					pMfc->GetHeader((void*)&tempGi32M.hdr, tempVMgi.header_start );
		//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
					//RtlCopyMemory(&tempVMgi, memFiles_CONTAINER.GetDataOffset(item), sizeof(VM_GameIndex));
					//MemoryFileContainer *pMfc = memFiles_CONTAINER.GetMemPageForIndex(item);
					//RtlCopyMemory(&tempGi32M.hdr, pMfc->vm_compressed_games->data() + tempVMgi.header_start, sizeof(CompressedHeader));

				}
			}

			if( column == 0 )
			{
				return wxString::Format("%d", item );
			}
			else if( column == 1)
			{
				return headersListData.LookupPlayerName(tempGi32M.hdr.wname);
			}
			else if( column == 2 )
			{
				return headersListData.LookupPlayerName(tempGi32M.hdr.bname);
			}
			else if( column == 3)
			{
				return wxString::Format("%d",headersListData.LookupElo(tempGi32M.hdr.whiteElo));
			}
			else if( column == 4 )
			{
				return wxString::Format("%d", headersListData.LookupElo(tempGi32M.hdr.blackElo));
			}
			else if( column == 5 )
			{
				return headersListData.LookupEcoName(tempGi32M.hdr.eco);

			}
			else if( column == 6 )
			{
				return headersListData.LookupEventName(tempGi32M.hdr.eventname);

			}
			else if( column == 7 )
			{
				return headersListData.LookupSiteName(tempGi32M.hdr.sitename);

			}
			else if( column == 8 )
			{
				return headersListData.LookupDateName(tempGi32M.hdr.date);

			}
			else if( column == 9 )
			{

				if( tempGi32M.hdr.result == (unsigned char)0 )
				{
					return wxString( "1-0");
				
				}
				else if( tempGi32M.hdr.result == (unsigned char)1 )
				{
					return wxString( "0-1" );
				}
				else if( tempGi32M.hdr.result == (unsigned char)2 )
				{
					return wxString( "1/2-1/2");
				}
			}				
				
		}
		else
		{
			vector< SortableGameIndex*> *vecIndx;

			if( filter == true )
			{
				vecIndx = &m_filteredIndexes;
			}
			else
			{
				vecIndx = &m_SortableIndexes;
			}

			if( column == 0 )
			{
				if( filter == true )
				{
					return wxString::Format("%d", (*vecIndx)[(int)item]->original_loc );
				}
				else
				{
					return wxString::Format("%d", (int)item );
				}
			}
			else if( column == 1)
			{
				return headersListData.LookupPlayerName((*vecIndx)[(int)item]->m_Gi->hdr.wname);
			}
			else if( column == 2 )
			{
				return headersListData.LookupPlayerName((*vecIndx)[(int)item]->m_Gi->hdr.bname);
			}
			else if( column == 3)
			{
				return wxString::Format("%d",headersListData.LookupElo((*vecIndx)[(int)item]->m_Gi->hdr.whiteElo));
			}
			else if( column == 4 )
			{
				return wxString::Format("%d", headersListData.LookupElo((*vecIndx)[(int)item]->m_Gi->hdr.blackElo));
			}
			else if( column == 5 )
			{
				return headersListData.LookupEcoName((*vecIndx)[(int)item]->m_Gi->hdr.eco);

			}
			else if( column == 6 )
			{
				return headersListData.LookupEventName((*vecIndx)[(int)item]->m_Gi->hdr.eventname);

			}
			else if( column == 7 )
			{
				return headersListData.LookupSiteName((*vecIndx)[(int)item]->m_Gi->hdr.sitename);

			}
			else if( column == 8 )
			{
				return headersListData.LookupDateName((*vecIndx)[(int)item]->m_Gi->hdr.date);

			}
			else if( column == 9 )
			{

				if( (*vecIndx)[(int)item]->m_Gi->hdr.result == (unsigned char)0 )
				{
					return wxString( "1-0");
				
				}
				else if( (*vecIndx)[(int)item]->m_Gi->hdr.result == (unsigned char)1 )
				{
					return wxString( "0-1" );
				}
				else if( (*vecIndx)[(int)item]->m_Gi->hdr.result == (unsigned char)2 )
				{
					return wxString( "1/2-1/2");
				}
			}
			
			
	
		}
			
		
	}
	else
	{
		return "";

    }

}



void PgnLoad::CloseSQLDatabase()
{
	sqlite3_close (db); 
}



void PgnLoad::SortByMoves( )
{
	if( SORTABLE_INDEX_MODE != SORTABLE_GAME_INDEX_MODE_VM )
	{
		if( m_sortData.SortFlags[HeaderSortData::MOVES] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_moves, this->m_SortableIndexes.size());
			}
			else
			{
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_moves, this->m_filteredIndexes.size());
				sorted_filtered = true;
			}
			m_sortData.UnsetFlag(HeaderSortData::MOVES);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::MOVES );

			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_moves_INV, this->m_SortableIndexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_moves_INV, this->m_filteredIndexes.size());
			}
		}
	}
	else
	{
		if( m_sortData.SortFlags[HeaderSortData::MOVES] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_moves_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_moves_VM, this->tree_filtered_indexes.size());
				sorted_filtered = true;
			}
			m_sortData.UnsetFlag(HeaderSortData::MOVES);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::MOVES );

			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_moves_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_moves_VM, this->tree_filtered_indexes.size());
			}
		}
	}
}
void PgnLoad::SortByNumber( )
{
	if( SORTABLE_INDEX_MODE != SORTABLE_GAME_INDEX_MODE_VM )
	{
		if( m_sortData.SortFlags[HeaderSortData::NUMBER] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_by_number(this->m_SortableIndexes, cmpfun_gameIndexes32M_number_Inv, this->m_SortableIndexes.size());

			
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_by_number(this->m_filteredIndexes, cmpfun_gameIndexes32M_number_Inv, this->m_filteredIndexes.size());
			}
			m_sortData.UnsetFlag(HeaderSortData::NUMBER);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::NUMBER );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_by_number(this->m_SortableIndexes, cmpfun_gameIndexes32M_number, this->m_SortableIndexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_by_number(this->m_filteredIndexes, cmpfun_gameIndexes32M_number, this->m_filteredIndexes.size());

			}
		}
	}
	else
	{

	}
}
void PgnLoad::SortByWhiteName( )
{
	if( SORTABLE_INDEX_MODE != SORTABLE_GAME_INDEX_MODE_VM )
	{
		if( m_sortData.SortFlags[HeaderSortData::WNAME] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_wname_Inv, this->m_SortableIndexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_wname_Inv, this->m_filteredIndexes.size());
			}
		
			m_sortData.UnsetFlag(HeaderSortData::WNAME);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::WNAME );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_data1, this->m_SortableIndexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_data1, this->m_filteredIndexes.size());
			}
		}
	}
	else
	{
		filter = true;
		if( m_sortData.SortFlags[HeaderSortData::WNAME] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_wname_Inv_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_wname_Inv_VM, this->tree_filtered_indexes.size());
			}
		
			m_sortData.UnsetFlag(HeaderSortData::WNAME);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::WNAME );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_data1_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_data1_VM, this->tree_filtered_indexes.size());
			}
		}
	}
}
void PgnLoad::SortByBlackName( )
{
	if( SORTABLE_INDEX_MODE != SORTABLE_GAME_INDEX_MODE_VM )
	{
		if( m_sortData.SortFlags[HeaderSortData::BNAME] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_bname_Inv, this->m_SortableIndexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_bname_Inv, this->m_filteredIndexes.size());
			}
			m_sortData.UnsetFlag(HeaderSortData::BNAME);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::BNAME );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_bname, this->m_SortableIndexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_bname, this->m_filteredIndexes.size());
			}
		}
	}
	else
	{
		if( m_sortData.SortFlags[HeaderSortData::BNAME] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_bname_Inv_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_bname_Inv_VM, this->tree_filtered_indexes.size());
			}
			m_sortData.UnsetFlag(HeaderSortData::BNAME);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::BNAME );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_bname_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_bname_VM, this->tree_filtered_indexes.size());
			}
		}
	}
}
void PgnLoad::SortByWhiteElo( )
{
	if( SORTABLE_INDEX_MODE != SORTABLE_GAME_INDEX_MODE_VM )
	{
		if( m_sortData.SortFlags[HeaderSortData::WELO] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_elow_Inv, this->m_SortableIndexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_elow_Inv, this->m_filteredIndexes.size());
			}
			m_sortData.UnsetFlag(HeaderSortData::WELO);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::WELO );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_elow, this->m_SortableIndexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_elow, this->m_filteredIndexes.size());
			}
		}
	}
	else
	{
		if( m_sortData.SortFlags[HeaderSortData::WELO] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_elow_Inv_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_elow_Inv_VM, this->tree_filtered_indexes.size());
			}
			m_sortData.UnsetFlag(HeaderSortData::WELO);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::WELO );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_elow_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_elow_VM, this->tree_filtered_indexes.size());
			}
		}
	}
}
void PgnLoad::SortByBlackElo( )
{
	if( SORTABLE_INDEX_MODE != SORTABLE_GAME_INDEX_MODE_VM )
	{
		if( m_sortData.SortFlags[HeaderSortData::BELO] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_elob_Inv, this->m_SortableIndexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_elob_Inv, this->m_filteredIndexes.size());
			}
			m_sortData.UnsetFlag(HeaderSortData::BELO);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::BELO );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_elob, this->m_SortableIndexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_elob, this->m_filteredIndexes.size());
			}
		}
	}
	else
	{
		
		if( m_sortData.SortFlags[HeaderSortData::BELO] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_elob_Inv_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_elob_Inv_VM, this->tree_filtered_indexes.size());
			}
			m_sortData.UnsetFlag(HeaderSortData::BELO);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::BELO );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_elob_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_elob_VM, this->tree_filtered_indexes.size());
			}
		}
	}
}
void PgnLoad::SortByECO( )
{
	if( SORTABLE_INDEX_MODE != SORTABLE_GAME_INDEX_MODE_VM )
	{
		if( m_sortData.SortFlags[HeaderSortData::ECO] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_Eco_Inv, this->m_SortableIndexes.size());
			}
			else
			{
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_Eco_Inv, this->m_filteredIndexes.size());
				sorted_filtered = true;
			}
			m_sortData.UnsetFlag(HeaderSortData::ECO);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::ECO );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_Eco, this->m_SortableIndexes.size());
			}
			else
			{
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_Eco, this->m_filteredIndexes.size());
	
				sorted_filtered = true;
			}
		}
	}
	else
	{
		if( m_sortData.SortFlags[HeaderSortData::ECO] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_Eco_Inv_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_Eco_Inv_VM, this->tree_filtered_indexes.size());
				sorted_filtered = true;
			}
			m_sortData.UnsetFlag(HeaderSortData::ECO);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::ECO );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_Eco_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_Eco_VM, this->tree_filtered_indexes.size());
	
				sorted_filtered = true;
			}
		}
	}
}
void PgnLoad::SortByEvent( )
{
	if( SORTABLE_INDEX_MODE != SORTABLE_GAME_INDEX_MODE_VM )
	{
		if( m_sortData.SortFlags[HeaderSortData::EVENT] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_Event_Inv, this->m_SortableIndexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_Event_Inv, this->m_filteredIndexes.size());
			}
			m_sortData.UnsetFlag(HeaderSortData::EVENT);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::EVENT );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_Event, this->m_SortableIndexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_Event, this->m_filteredIndexes.size());
			}
		}
	}
	else
	{
		if( m_sortData.SortFlags[HeaderSortData::EVENT] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_Event_Inv_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_Event_Inv_VM, this->tree_filtered_indexes.size());
			}
			m_sortData.UnsetFlag(HeaderSortData::EVENT);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::EVENT );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_Event_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_Event_VM, this->tree_filtered_indexes.size());
			}
		}
	}
}
void PgnLoad::SortBySite( )
{
	if( SORTABLE_INDEX_MODE != SORTABLE_GAME_INDEX_MODE_VM )
	{
		if( m_sortData.SortFlags[HeaderSortData::SITE] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_Site_Inv, this->m_SortableIndexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_Site_Inv, this->m_filteredIndexes.size());
			}
			m_sortData.UnsetFlag(HeaderSortData::SITE);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::SITE );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_Site, this->m_SortableIndexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_Site, this->m_filteredIndexes.size());
			}
		}
	}
	else
	{
		if( m_sortData.SortFlags[HeaderSortData::SITE] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_Site_Inv_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_Site_Inv_VM, this->tree_filtered_indexes.size());
			}
			m_sortData.UnsetFlag(HeaderSortData::SITE);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::SITE );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_Site_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_Site_VM, this->tree_filtered_indexes.size());
			}
		}
	}
}
void PgnLoad::SortByDate( )
{
	if( SORTABLE_INDEX_MODE != SORTABLE_GAME_INDEX_MODE_VM )
	{
		if( m_sortData.SortFlags[HeaderSortData::DATE] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_Date_Inv, this->m_SortableIndexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_Date_Inv, this->m_filteredIndexes.size());
			}
			m_sortData.UnsetFlag(HeaderSortData::DATE);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::DATE );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector(this->m_SortableIndexes, cmpfun_gameIndexes32M_Date, this->m_SortableIndexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector(this->m_filteredIndexes, cmpfun_gameIndexes32M_Date, this->m_filteredIndexes.size());
			}
		}
	}
	else
	{
		if( m_sortData.SortFlags[HeaderSortData::DATE] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_Date_Inv_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_Date_Inv_VM, this->tree_filtered_indexes.size());
			}
			m_sortData.UnsetFlag(HeaderSortData::DATE);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::DATE );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_Date_VM, this->tree_filtered_indexes.size());
			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_VM(this->tree_filtered_indexes, cmpfun_gameIndexes32M_Date_VM, this->tree_filtered_indexes.size());
			}
		}
	}

}
void PgnLoad::SortByResult( )
{
	if( SORTABLE_INDEX_MODE != SORTABLE_GAME_INDEX_MODE_VM )
	{
		if( m_sortData.SortFlags[HeaderSortData::RESULT] )
		{
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_by_Result(this->m_SortableIndexes, cmpfun_gameIndexes32M_number_Inv, this->m_SortableIndexes.size());

			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_by_Result(this->m_filteredIndexes, cmpfun_gameIndexes32M_number_Inv, this->m_filteredIndexes.size());

			}
			m_sortData.UnsetFlag(HeaderSortData::RESULT);
		}
		else
		{
			m_sortData.UnsetLastFlag();
			m_sortData.SetFlag( HeaderSortData::RESULT );
			if( filter != true )
			{
				heap_sort_SortableGameIndex32M_vector_by_Result(this->m_SortableIndexes, cmpfun_gameIndexes32M_number, this->m_SortableIndexes.size());

			}
			else
			{
				sorted_filtered = true;
				heap_sort_SortableGameIndex32M_vector_by_Result(this->m_filteredIndexes, cmpfun_gameIndexes32M_number, this->m_filteredIndexes.size());

			}
		}
	}
	else
	{

	}

}

}; 