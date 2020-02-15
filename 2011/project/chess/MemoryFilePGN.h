#ifndef MEM_FILE_PGN_H
#define MEM_FILE_PGN_H
#include <windows.h>
#include "PGKey.h"
#include <vector>
#include <string>

using namespace std;
class MemBlock
{
public:

	char *data;
	int size;
	int offset;
};

class MemoryFile;

class MemPool
{
public:

	MemPool(int block_size);
	~MemPool();

	string name;

	
	int blockSize;
	int currOffset;

	MemoryFile *pDataPool;

	char* GetDataStart();

	MemBlock* GetIndex(int i);

	char* GetDataOffset(int i);
	char* GetDataOffset2(int i);

	bool IsOk()
	{
		if( (blockSize != 0) && (currOffset != 0) && 
			(pDataPool != 0) && (pDataPool->data() != 0 ))
		{
			return true;
		}
		else return false;
	}

	int GetBlockSize()
	{
		return blockSize;
	}

	int GetSizeInBytes()
	{
		return currOffset + blockSize;
	}

	int GetCapacity()
	{
		return pDataPool->size();
	}

	int GetMaxNumBlocks()
	{
		return pDataPool->size() / blockSize;
	}

	int GetNumBlocks()
	{
		return currOffset / blockSize;
	}

	MemBlock* GetBlock();

	bool OpenMemoryFileMemory(char* dest, bool overwrite, int size);

	bool OpenMemoryFileMemoryRead(char* dest, bool overwrite, int size);
};


class CFileMemPool
{
public:

	CFileMemPool(int block_size);
	~CFileMemPool();

	string name;

	
	int blockSize;
	int currOffset;

	FILE *pDataPool;

	char* GetDataStart();

	MemBlock* GetIndex(int i);

	char* GetDataOffset(int i);
	char* GetDataOffset2(int i);

	bool IsOk()
	{
		if( (blockSize != 0) && (currOffset != 0) && 
			(pDataPool != 0) && (pDataPool != 0 ))
		{
			return true;
		}
		else return false;
	}

	int GetBlockSize()
	{
		return blockSize;
	}

	int GetSizeInBytes()
	{
		return currOffset + blockSize;
	}

	int GetCapacity()
	{
		return 0;
		//return pDataPool->size();
	}

	int GetMaxNumBlocks()
	{
		return 0;
		//return pDataPool->size() / blockSize;
	}

	int GetNumBlocks()
	{
		return currOffset / blockSize;
	}

	MemBlock* GetBlock();

	bool OpenFileMemory(char* dest, bool overwrite, int size);

	bool OpenFileMemoryRead(char* dest, bool overwrite, int size);
};



class CTreeMoveData
{
public:
	CTreeMoveData()
	{
		moveNum = 0;
		mvWhite = 255;
		mvBlack = 255;
	}
	short int moveNum; // this data might not be neccessary
	unsigned char mvWhite;
	unsigned char mvBlack;
};

class CTreeResultData
{
public:
	CTreeResultData()
	{
		whiteWins = 0;
		blackWins = 0;
		draws = 0;
	}

	void operator+=(CTreeResultData &other)
	{
		whiteWins += other.whiteWins;
		blackWins += other.blackWins;
		draws += other.draws;
	}
	unsigned int whiteWins;
	unsigned int blackWins;
	unsigned int draws;
};


class CTreeNode
{
public:

	CTreeNode()
	{
	 pNext=0;
	 pPrev=0;
	 pVarWhite=0;
	 pVarBlack=0;
	 pUp=0;

	 data.mvWhite=255;
	 data.mvBlack=255;
	 hash_key_white=0;
	 hash_key_black=0;
	}
	~CTreeNode()
	{
		if(pNext)
		{
			delete pNext;
			pNext = NULL;
		}

		if(pVarWhite)
		{
			delete pVarWhite;
			pVarWhite = NULL;
		}

		if(pVarBlack)
		{
			delete pVarBlack;
			pVarBlack = NULL;
		}
	}


	CTreeMoveData data;
//	uint64 hash_key_white;
//	uint64 hash_key_black;
	CTreeResultData resDataW;
	CTreeResultData resDataB;


	uint64 hash_key_white;
	uint64 hash_key_black;

	CTreeNode *pNext;
	CTreeNode *pPrev;
	CTreeNode *pVarWhite;
	CTreeNode *pVarBlack;
	CTreeNode *pUp;

	void Create( CTreeNode* prev, uint64 hash_white, uint64 hash_black, unsigned char w, unsigned char b)
	{
		this->pPrev = prev;

		if( prev )
			prev->pNext = this;

		hash_key_white = hash_white;
		hash_key_black = hash_black;

		data.mvWhite = w;
		data.mvBlack = b;
	}


	// theory ... first write the root game into the memory pool
	// when merge is called, the 2nd game is in memory
	// the 2nd game overwrites the memory pool accumulating 
	// the results

	void Merge(CTreeNode* move, CTreeResultData &gameResult)
	{
		if( move->data.mvWhite == data.mvWhite )
		{
			this->resDataW += gameResult;
			if( move->data.mvBlack == data.mvBlack )
			{
				this->resDataB += gameResult;
				if( pNext )
				{
					if( move->pNext )
					{
						if( pNext->data.mvWhite != 255 )
						{
							pNext->Merge(move->pNext, gameResult);
							return;
						}
						else
						{
							//result = "";

							pNext = move->pNext;
							pNext->pPrev = this;
							


							move->pNext = NULL;
							

							/*this->atEndBlack = false;
							this->atEndWhite = false;
							*/
						}
						return;
					}
				}
				else
				{
					if( move->pNext )
					{
						//result = "";

						pNext = move->pNext;
						pNext->pPrev = this;

						move->pNext = NULL;

						/*this->atEndBlack = false;
						this->atEndWhite = false;
						*/
						return;
					}
				}
			}
			else
			{
				if( data.mvBlack == 255 )
				{
					if( move->data.mvBlack != 255 )
					{
						data.mvBlack = move->data.mvBlack;
						this->resDataB += gameResult;
						if( move->pNext )
						{
							//result = "";

							pNext = move->pNext;
							pNext->pPrev = this;

							move->pNext = NULL;

							//pNext->resDataW += gameResult;
							
							/*this->atEndBlack = false;
							this->atEndWhite = false;
							*/
							return;
						}
						/*this->atEndBlack = true;
						this->atEndWhite = false;
						*/
					}
					else
					{
						if( pNext )
						{
							delete pNext;
							pNext = NULL;
						}
						/*this->atEndBlack = false;
						this->atEndWhite = true;
						*/
						return;
					}
				}
				else 
				{
					if( move->data.mvBlack != 255 )
					{
						if( pVarBlack )
						{
							CTreeNode* temp = pVarBlack;
							bool match = false;
							while( temp )
							{
								if( temp->data.mvBlack == move->data.mvBlack )
								{
									temp->resDataB += gameResult;
									if( temp->pNext )
									{
										if( move->pNext )
										{
											temp->pNext->Merge(move->pNext, gameResult);
											return;
										}
									}
									else if( move->pNext )
									{
										temp->pNext = move->pNext;
										if(move->pNext)
											move->pNext->pPrev = temp;
										move->pUp = this->pUp;

										/*temp->atEndBlack = false;
										temp->atEndWhite = false;
										*/
										return;
									}
								}

								temp = temp->pVarBlack;
							}
						}
						
						CTreeNode* blackVar = this->getBlackVariationForParse();

						blackVar->CopyNode(move);
						blackVar->data.mvWhite = 255;
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
			if( data.mvWhite== 255 )
			{
				if( move->data.mvWhite != 255 )
				{
					//result = "";

					CopyNode(move);

					/*this->atEndBlack = false;
					this->atEndWhite = false;
					*/
					if( data.mvBlack == 255 )
					{
						//this->atEndBlack = true;
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
			
			if( move->data.mvWhite != 255 )
			{
				// check white variations
				// if a variation exists ...
				// ok so check the disk
				if( pVarWhite )
				{
					pVarWhite->Merge(move, gameResult);
					return;
				}

				CTreeNode* whiteVar;
				if( pUp )
				{
					CTreeNode* temp2 = pUp;
					
					// get variation for parse ... 
					// this must access the mempool variations
					// they must contain integers that address the next mempool entries
					// traverse until a null address is found (0x00000) 
					// note, this is a variation from the node above the current node
					// ... the tree in memory must not be too large
					whiteVar = pUp->getWhiteVariationForParse();	
				}
				else
				{
					// this is a variation from the current node
					// so mempool->GetNode(int) should return next,
					// perhaps this can be wrapped up int GetNext()
					whiteVar = getWhiteVariationForParse();
				}

				// RtlCopyMemory?
				whiteVar->CopyNode(move);

				// ok here, must call something that writes the rest of 
				// the new game into the mempool, the next here does not
				// have to actually exist
				
				whiteVar->pNext = move->pNext;
			
				move->pNext = NULL;

				// simply code that makes it easier to delete merged nodes
				if( whiteVar->pNext )
				{
					whiteVar->pNext->pPrev = whiteVar;
				}
			}
			return;
		}
	}

	CTreeNode* getWhiteVariationForParse()
	{
		/*pVariationWhite[variation_count_white] = new Move();
		pVariationWhite[variation_count_white]->pPrev = this;

		return pVariationWhite[variation_count_white++];*/
	//	variation_count_white++;
		if( pVarWhite == NULL )
		{
			pVarWhite = new CTreeNode;
			pVarWhite->pPrev = this;
			pVarWhite->pUp = this;
			return pVarWhite;
		}
		else
		{
			CTreeNode* temp = pVarWhite;
			while( temp->pVarWhite )
			{
				temp = temp->pVarWhite;
			}
			temp->pVarWhite = new CTreeNode;
			temp->pVarWhite->pPrev = this;
			temp->pVarWhite->pUp = this;
			temp = temp->pVarWhite;
			return temp;
		}
	}

	CTreeNode* getBlackVariationForParse()
	{
		//variation_count_black++;
		if( pVarBlack == NULL )
		{
			pVarBlack = new CTreeNode;
			pVarBlack->pPrev = this;
			pVarBlack->pUp = this;
		//	pVarBlack->variation_count_black = variation_count_black;
			return pVarBlack;
		}
		else
		{
			CTreeNode* temp = pVarBlack;
			while( temp->pVarBlack )
			{
				temp = temp->pVarBlack;
			}
			temp->pVarBlack = new CTreeNode;
			temp->pVarBlack->pPrev = this;
			temp->pVarBlack->pUp = this;
		//	temp->pVarBlack->variation_count_black = variation_count_black;
			temp = temp->pVarBlack;
			return temp;
		}
	}

	//void PushMoveBlack(CTreeNode* move, CTreeResultData& gameResult)



	void CopyNode(CTreeNode* move)
	{
		this->data.mvWhite = move->data.mvWhite;
		this->data.mvBlack = move->data.mvBlack;
		this->data.moveNum = move->data.moveNum;

		this->resDataW = move->resDataW;
		this->resDataB = move->resDataB;
	}
};


// TODO
class MemTreeNodeData
{
public:


};

class MemTreeNode
{
public:

	MemTreeNode()
	{
		w = 255;
		b = 255;
		hash_key_white = 0;
		hash_key_black = 0;

		addr_This = 0;
		addr_Up = 0;
		addr_Prev = 0;
		addr_Next = 0;
		addr_VarWhite = 0;
		addr_VarBlack = 0;
	}

	~MemTreeNode()
	{
	}


	//int movNum; /// FAT
	unsigned char w;
	unsigned char b;

	uint64 hash_key_white;  // don't need these here
	uint64 hash_key_black;  // don't need these here

	int addr_This;

	int addr_Up;
	int addr_Prev;
	int addr_Next;      // 4
	int addr_VarWhite;   // 4
	int addr_VarBlack;   // 4

	bool NextExists()
	{
		return addr_Next != 0;
	}

	bool VarWhiteExists()
	{
		return addr_VarWhite != 0;
	}

	bool VarBlackExists()
	{
		return addr_VarBlack != 0;
	}

	void WriteCurrentNode(MemPool *mp)
	{
		RtlCopyMemory((void*)(mp->GetDataStart() + this->addr_This), 
					  (void*)this, 
					  sizeof(MemTreeNode));
	}
	
	MemTreeNode* Next(MemPool *mp)
	{
		MemTreeNode *next = new MemTreeNode();
		RtlCopyMemory(next, mp->GetDataOffset2(this->addr_Next), sizeof(MemTreeNode));
		return next;
	}

	MemTreeNode* Up(MemPool *mp)
	{
		MemTreeNode *next = new MemTreeNode();
		RtlCopyMemory(next, mp->GetDataOffset2(this->addr_Up), sizeof(MemTreeNode));
		return next;
	}

	MemTreeNode* VarBlack(MemPool *mp)
	{		
		MemTreeNode *next = new MemTreeNode();
		RtlCopyMemory(next, mp->GetDataOffset2(this->addr_VarBlack), sizeof(MemTreeNode));
		return next;
	}

	MemTreeNode* VarWhite(MemPool *mp)
	{
		MemTreeNode *next = new MemTreeNode();
		RtlCopyMemory(next, mp->GetDataOffset2(this->addr_VarWhite), sizeof(MemTreeNode));
		return next;
	}

	MemTreeNode* AllocVarBlack(MemPool *mp )
	{
		MemTreeNode *next;
		if( addr_VarBlack == 0 )
		{
			next = new MemTreeNode();
		
			MemBlock *mb = mp->GetBlock();
			next->addr_This = mb->offset;
			next->addr_Prev = this->addr_This;
			next->addr_Up = this->addr_This;
			next->addr_VarBlack = 0;
			next->addr_VarWhite  =0;
			next->addr_Next = 0;

			this->addr_VarBlack = next->addr_This;

			next->WriteCurrentNode(mp);
			WriteCurrentNode(mp);
			delete mb;
		}
		else
		{
			MemTreeNode * temp = VarBlack(mp);
			while( temp )
			{
				if( temp->addr_VarBlack == 0 )
				{
					break;
				}

				MemTreeNode *tempVarBlack = temp->VarBlack(mp);
				delete temp;
				temp = tempVarBlack;
			}

			next = new MemTreeNode();
		
			MemBlock *mb = mp->GetBlock();
			
			next->addr_This = mb->offset;
			next->addr_Prev = this->addr_This;
			next->addr_Up = this->addr_This;
			next->addr_VarBlack = 0;
			next->addr_VarWhite  =0;
			next->addr_Next = 0;

			temp->addr_VarBlack = next->addr_This;

			next->WriteCurrentNode(mp);
			temp->WriteCurrentNode(mp);

			delete temp;

			delete mb;
		}
		return next;
	}

	MemTreeNode* AllocVarWhite(MemPool *mp )
	{
		MemTreeNode *next;
		if( addr_VarWhite == 0 )
		{
			next = new MemTreeNode();
		
			MemBlock *mb = mp->GetBlock();
			next->addr_This = mb->offset;
			next->addr_Prev = this->addr_This;
			next->addr_Up = this->addr_This;
			next->addr_VarBlack = 0;
			next->addr_VarWhite  =0;
			next->addr_Next = 0;

			this->addr_VarWhite = next->addr_This;

			next->WriteCurrentNode(mp);

			WriteCurrentNode(mp);
			delete mb;
		}
		else
		{
			MemTreeNode * temp = VarWhite(mp);
			while( temp )
			{
				if( temp->addr_VarWhite == 0 )
				{
					break;
				}

				MemTreeNode *tempVarWhite = temp->VarWhite(mp);
				delete temp;
				temp = tempVarWhite;
			}

			next = new MemTreeNode();
		
			MemBlock *mb = mp->GetBlock();
			
			next->addr_This = mb->offset;
			next->addr_Prev = this->addr_This;
			next->addr_Up = this->addr_This;
			next->addr_VarBlack = 0;
			next->addr_VarWhite  =0;
			next->addr_Next = 0;

			temp->addr_VarWhite = next->addr_This;

			next->WriteCurrentNode(mp);
			temp->WriteCurrentNode(mp);

			delete temp;

			delete mb;
		}
		return next;
	}

	MemTreeNode* AllocNext(MemPool *mp )
	{
		MemTreeNode *next = new MemTreeNode();


		
			MemBlock *mb = mp->GetBlock();
			next->addr_This = mb->offset;
			next->addr_Prev = this->addr_This;
			next->addr_Up = 0;
			next->addr_VarBlack = 0;
			next->addr_VarWhite  =0;
			next->addr_Next = 0;

			this->addr_Next = next->addr_This;
			
			next->WriteCurrentNode(mp);
			
			WriteCurrentNode(mp);
		
			delete mb;
		
		return next;
	}

	bool WriteRemainingNodes(CTreeNode *nextOnwards, MemPool *mp, CTreeResultData &gameResult)
	{
		if( nextOnwards->pNext )
		{
			CTreeNode *temp = nextOnwards->pNext;

			MemTreeNode *next = this->AllocNext(mp);
			next->AssignNode(temp);
			next->WriteCurrentNode(mp);
			WriteCurrentNode(mp);
			temp = temp->pNext;
			//delete next;
			while( temp )
			{
				MemTreeNode *pnext = next->AllocNext(mp);
				
				next->WriteCurrentNode(mp);

				delete next;
				next = pnext;
				next->AssignNode(temp);
				next->WriteCurrentNode(mp);
				
	
				temp = temp->pNext;
			}

		}

		return true;
	}

	void AddGameResultW(CTreeResultData &gameResult)
	{
	}

	void AddGameResultB(CTreeResultData &gameResult)
	{
	}

	void AssignNode( CTreeNode *move )
	{
		w = move->data.mvWhite;
		b = move->data.mvBlack;
		hash_key_white = move->hash_key_white;
		hash_key_black = move->hash_key_black;
	}

	void Merge2(CTreeNode* move, MemPool *mp, CTreeResultData &gameResult)
	{
		if( move->data.mvWhite == w )
		{
			//this->resDataW += gameResult;
			AddGameResultW(gameResult);

			if( move->data.mvBlack == b )
			{
				//this->resDataB += gameResult;
				AddGameResultB(gameResult);
				if( this->NextExists() )
				{
					if( move->pNext )
					{
						MemTreeNode* pNext = Next(mp);
						if( pNext->w != 255 )
						{
							pNext->Merge2(move->pNext,mp, gameResult);
							pNext->WriteCurrentNode(mp);
							delete pNext;
							return;
						}
						else
						{
							pNext->AssignNode( move->pNext );

							// now, write the moves after move->pNext onto the disk
							pNext->WriteRemainingNodes(move->pNext,mp, gameResult);
							// (which one?)
							
							
							// write remaining nodes to disk
							/*pNext = move->pNext;
							pNext->pPrev = this;
							*/

							// deliting entire next game, no need for this
							//move->pNext = NULL;
							
							// rewrite pNext
							pNext->WriteCurrentNode(mp);
							
							delete pNext;
						}
						return;
					}
				}
				else
				{
					if( move->pNext )
					{
						// alloc next
						MemTreeNode *pNext = this->AllocNext(mp);

						pNext->AssignNode( move->pNext );

						// write remaining nodes to disk
						pNext->WriteRemainingNodes(move->pNext,mp, gameResult);

						//pNext = move->pNext;
						//pNext->pPrev = this;

						//move->pNext = NULL;

						// rewrite pNext
						pNext->WriteCurrentNode(mp);
						WriteCurrentNode(mp);
						// now delete pNext
						delete pNext;

						return;
					}
				}
			}
			else
			{
				// black empty
				if( b == 255 )
				{
					if( move->data.mvBlack != 255 )
					{
						// assign black, need to write to disk
						b = move->data.mvBlack;
						//this->resDataB += gameResult;
						AddGameResultB(gameResult);
						
						if( move->pNext )
						{
							
							// assuming this pNext is empty,
							// assign this pNext as move->pNext,
							// use WriteRemaininNodes()
							
							/*pNext = move->pNext;
							pNext->pPrev = this;

							move->pNext = NULL;
							*/

							// alloc next
							MemTreeNode *pNext = this->AllocNext(mp);

							pNext->AssignNode( move->pNext );
							// using the callers next to address
							pNext->WriteRemainingNodes(move->pNext,mp, gameResult);

							// rewrite pNext
							pNext->WriteCurrentNode(mp);

							WriteCurrentNode(mp);
							// now delete pNext
							delete pNext;

							return;
						}

					}
					else
					{
						// says to remove the next otherwise, pretty easy one

						this->addr_Next = 0;

						/*if( pNext )
						{
							delete pNext;
							pNext = NULL;
						}*/

						
						return;
					}
				}
				else 
				{
					if( move->data.mvBlack != 255 )
					{
						if( this->VarBlackExists() )
						{
							// get variation of blacks move
							//CTreeNode* temp = pVarBlack;
							// Get VarBlack from the mempool
							MemTreeNode *temp = this->VarBlack(mp);

							bool match = false;
							while( temp && (temp->addr_This !=0) )
							{
								if( temp->b == move->data.mvBlack )
								{
									//temp->resDataB += gameResult;
									temp->AddGameResultB(gameResult);
									
									if( temp->NextExists() )
									{
										if( move->pNext )
										{
											MemTreeNode *tempNext = temp->Next(mp);
											tempNext->Merge2(move->pNext, mp, gameResult);
											tempNext->WriteCurrentNode(mp);
											delete tempNext; // can this be done without pointers?
											//temp->pNext->Merge(move->pNext, gameResult);
											delete temp;
											return;
										}
									}
									else if( move->pNext )
									{
										// create a node for temp->pNext
										MemTreeNode *tempNext = temp->AllocNext(mp);
										
										tempNext->AssignNode(move->pNext);
										// now, write the moves after move->pNext onto the disk
										tempNext->WriteRemainingNodes(move->pNext,mp,gameResult);
										
										/*temp->pNext = move->pNext;

										if(move->pNext)
											move->pNext->pPrev = temp;
										move->pUp = this->pUp;
										*/

										tempNext->WriteCurrentNode(mp);

										delete tempNext;
										delete temp;

										return;
									}
								}

								MemTreeNode *tempVarBlack = temp->VarBlack(mp);
								delete temp;
								temp = tempVarBlack ;
							}

							delete temp;
						}
						
						//CTreeNode* blackVar = this->getBlackVariationForParse();
						MemTreeNode *blackVar = this->AllocVarBlack(mp);

						blackVar->AssignNode(move);

						//blackVar->CopyNode(move);
						
						blackVar->w = 255;

						// now call write remaining nodes
						// CONCENTRATE
						blackVar->WriteRemainingNodes(move,mp, gameResult);

						/*if( move->pNext )
							move->pNext->pPrev = blackVar;

						move->pNext = NULL;
						blackVar->pUp = this;
						blackVar->pPrev = this;
						*/
						/*if( blackVar->NextExists() )
						{
							MemTreeNode* blackVarNext = blackVar->Next(mp);
							blackVarNext->addr_Prev = blackVar->addr_This;
							blackVarNext->WriteCurrentNode(mp);
							delete blackVarNext;
						}*/
						blackVar->addr_Up = this->addr_This;
						blackVar->addr_Prev = this->addr_This;
						blackVar->WriteCurrentNode(mp);

						delete blackVar;

						return;
					}
				}
			}
		}
		else
		{
			if( w== 255 )
			{
				if( move->data.mvWhite != 255 )
				{

					//CopyNode(move);
					this->AssignNode(move);

					
					if( b == 255 )
					{
						//this->atEndBlack = true;
					}

					this->WriteRemainingNodes(move, mp, gameResult);
					
					
					/*pNext = move->pNext;
					move->pNext = NULL;
					pVarWhite = move->pVarWhite;
					pVarBlack = move->pVarBlack;
					*/
					/*if( NextExists() )
					{
						
						MemTreeNode* next= Next(mp);

						addr_Next = next->addr_This;

						next->addr_Prev = this->addr_This;

						//pNext->pPrev = this;
						//pNext->pUp = pUp;
						next->WriteCurrentNode(mp);
						this->WriteCurrentNode(mp);

						delete next;
					}*/
					this->WriteCurrentNode(mp);
					return;
				}
				else 
				{
					/*if( pNext )
					{
						delete pNext;
						pNext = NULL;
					}*/
					this->addr_Next = 0;

				}return;
			}
			
			if( move->data.mvWhite != 255 )
			{
				// check white variations
				// if a variation exists ...
				// ok so check the disk
				if( VarWhiteExists() )
				{
					MemTreeNode* pVarWhite = VarWhite(mp);

					pVarWhite->Merge2(move, mp, gameResult);
					//pVarWhite->WriteCurrentNode(mp);

					delete pVarWhite;
					return;
				}

				MemTreeNode* whiteVar;
				if( this->addr_Up != 0 )
				{
					MemTreeNode *temp2 = this->Up(mp);
					//CTreeNode* temp2 = pUp;
					
					// get variation for parse ... 
					// this must access the mempool variations
					// they must contain integers that address the next mempool entries
					// traverse until a null address is found (0x00000) 
					// note, this is a variation from the node above the current node
					// ... the tree in memory must not be too large
					whiteVar = temp2->AllocVarWhite(mp);

					delete temp2;
				}
				else
				{
					// this is a variation from the current node
					// so mempool->GetNode(int) should return next,
					// perhaps this can be wrapped up int GetNext()
					whiteVar = AllocVarWhite(mp);
				}

				// RtlCopyMemory?
				//whiteVar->CopyNode(move);
				whiteVar->AssignNode(move);

				// ok here, must call something that writes the rest of 
				// the new game into the mempool, the next here does not
				// have to actually exist
				
				whiteVar->WriteRemainingNodes( move, mp, gameResult);
			
				//move->pNext = NULL;

				// simply code that makes it easier to delete merged nodes
				if( whiteVar->NextExists() )
				{
					MemTreeNode* whiteVarNext = whiteVar->Next(mp);
					whiteVarNext->addr_Prev = whiteVar->addr_This;
					//whiteVar->pNext->pPrev = whiteVar;
					whiteVarNext->WriteCurrentNode(mp);
					
					delete whiteVarNext;
				}
				whiteVar->WriteCurrentNode(mp);
				//whiteVar->N

				delete whiteVar;
			}
			return;
		}
	}

	
}; // 46 bytes, maybe 4 bytes pad ?

class MemHashNode
{
public:

	unsigned int tpt_Index; // transposition table index

	uint64 hash_key;
};

class Transposition
{
public:

	vector< int > nodes_sharing_hash;
};

class TranspositionTable
{
public:

	vector< Transposition > m_lists;
};



class MemPageSystem
{
public:

	MemBlock *GetBlockIndex(int idx);

	int totalSize;
	vector<MemPool*> m_pages;
};



#endif