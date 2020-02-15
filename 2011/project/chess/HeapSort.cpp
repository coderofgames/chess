#include "heapsort.h"
#include "headers.h"
#include "PgnLoad.h"

#define uint unsigned int

void heap_sort_GameIndexes(GameIndex *This[], compare_func_objects func_pointer, unsigned int len)
{
  /* heap sort */

  uint half;
  uint parents;

  if (len <= 1)
    return;

  half = len >> 1;
  for (parents = half; parents >= 1; --parents)
  {
    GameIndex *tmp;
    int level = 0;
    uint child;

    child = parents;
    /* bottom-up downheap */

    /* leaf-search for largest child path */
    while (child <= half)
    {
      ++level;
      child += child;
      if ((child < len) &&
          ((*func_pointer)(This[child], This[child - 1]) > 0))
        ++child;
    }
    /* bottom-up-search for rotation point */
    tmp = This[parents - 1];
    for (;;)
    {
      if (parents == child)
        break;
      if ((*func_pointer)(tmp, This[child - 1]) <= 0)
        break;
      child >>= 1;
      --level;
    }
    /* rotate nodes from parents to rotation point */
    for (;level > 0; --level)
    {
      This[(child >> level) - 1] =
        This[(child >> (level - 1)) - 1];
    }
    This[child - 1] = tmp;
  }

  --len;
  do
  {
    GameIndex *tmp;
    int level = 0;
    uint child;

    /* move max element to back of array */
    tmp = This[len];
    This[len] = This[0];
    This[0] = tmp;

    child = parents = 1;
    half = len >> 1;

    /* bottom-up downheap */

    /* leaf-search for largest child path */
    while (child <= half)
    {
      ++level;
      child += child;
      if ((child < len) &&
          ((*func_pointer)(This[child], This[child - 1]) > 0))
        ++child;
    }
    /* bottom-up-search for rotation point */
    for (;;)
    {
      if (parents == child)
        break;
      if ((*func_pointer)(tmp, This[child - 1]) <= 0)
        break;
      child >>= 1;
      --level;
    }
    /* rotate nodes from parents to rotation point */
    for (;level >  0; --level)
    {
      This[(child >> level) - 1] =
        This[(child >> (level - 1)) - 1];
    }
    This[child - 1] = tmp;
  } while (--len >= 1);
}

void heap_sort_GameIndexes32M(GameIndex32Moves *This[], compare_func_objects32 func_pointer, unsigned int len)
{
  /* heap sort */

  uint half;
  uint parents;

  if (len <= 1)
    return;

  half = len >> 1;
  for (parents = half; parents >= 1; --parents)
  {
    GameIndex32Moves *tmp;
    int level = 0;
    uint child;

    child = parents;
    /* bottom-up downheap */

    /* leaf-search for largest child path */
    while (child <= half)
    {
      ++level;
      child += child;
      if ((child < len) &&
          ((*func_pointer)(This[child], This[child - 1]) > 0))
        ++child;
    }
    /* bottom-up-search for rotation point */
    tmp = This[parents - 1];
    for (;;)
    {
      if (parents == child)
        break;
      if ((*func_pointer)(tmp, This[child - 1]) <= 0)
        break;
      child >>= 1;
      --level;
    }
    /* rotate nodes from parents to rotation point */
    for (;level > 0; --level)
    {
      This[(child >> level) - 1] =
        This[(child >> (level - 1)) - 1];
    }
    This[child - 1] = tmp;
  }

  --len;
  do
  {
    GameIndex32Moves *tmp;
    int level = 0;
    uint child;

    /* move max element to back of array */
    tmp = This[len];
    This[len] = This[0];
    This[0] = tmp;

    child = parents = 1;
    half = len >> 1;

    /* bottom-up downheap */

    /* leaf-search for largest child path */
    while (child <= half)
    {
      ++level;
      child += child;
      if ((child < len) &&
          ((*func_pointer)(This[child], This[child - 1]) > 0))
        ++child;
    }
    /* bottom-up-search for rotation point */
    for (;;)
    {
      if (parents == child)
        break;
      if ((*func_pointer)(tmp, This[child - 1]) <= 0)
        break;
      child >>= 1;
      --level;
    }
    /* rotate nodes from parents to rotation point */
    for (;level > 0; --level)
    {
      This[(child >> level) - 1] =
        This[(child >> (level - 1)) - 1];
    }
    This[child - 1] = tmp;
  } while (--len >= 1);
}


void heap_sort_GameIndexes32M_vector(vector< GameIndex32Moves * > This,compare_func_objects32 func_pointer, unsigned int len)
{
  /* heap sort */

  uint half;
  uint parents;

  if (len <= 1)
    return;

  half = len >> 1;
  for (parents = half; parents >= 1; --parents)
  {
    GameIndex32Moves *tmp;
    int level = 0;
    uint child;

    child = parents;
    /* bottom-up downheap */

    /* leaf-search for largest child path */
    while (child <= half)
    {
      ++level;
      child += child;
      if ((child < len) &&
          ((*func_pointer)(This[child], This[child - 1]) > 0))
        ++child;
    }
    /* bottom-up-search for rotation point */
    tmp = This[parents - 1];
    for (;;)
    {
      if (parents == child)
        break;
      if ((*func_pointer)(tmp, This[child - 1]) <= 0)
        break;
      child >>= 1;
      --level;
    }
    /* rotate nodes from parents to rotation point */
    for (;level > 0; --level)
    {
      This[(child >> level) - 1] =
        This[(child >> (level - 1)) - 1];
    }
    This[child - 1] = tmp;
  }

  --len;
  do
  {
    GameIndex32Moves *tmp;
    int level = 0;
    uint child;

    /* move max element to back of array */
    tmp = This[len];
    This[len] = This[0];
    This[0] = tmp;

    child = parents = 1;
    half = len >> 1;

    /* bottom-up downheap */

    /* leaf-search for largest child path */
    while (child <= half)
    {
      ++level;
      child += child;
      if ((child < len) &&
          ((*func_pointer)(This[child], This[child - 1]) > 0))
        ++child;
    }
    /* bottom-up-search for rotation point */
    for (;;)
    {
      if (parents == child)
        break;
      if ((*func_pointer)(tmp, This[child - 1]) <= 0)
        break;
      child >>= 1;
      --level;
    }
    /* rotate nodes from parents to rotation point */
    for (;level > 0; --level)
    {
      This[(child >> level) - 1] =
        This[(child >> (level - 1)) - 1];
    }
    This[child - 1] = tmp;
  } while (--len >= 1);
}

void heap_sort_GameIndexes32M_Exp( ExpandingArrayList< SortableGameIndex* > &This, compare_func_objects32 func_pointer, unsigned int len)
{
  /* heap sort */

  uint half;
  uint parents;

  if (len <= 1)
    return;

  half = len >> 1;
  for (parents = half; parents >= 1; --parents)
  {
    SortableGameIndex *tmp;
    int level = 0;
    uint child;

    child = parents;
    /* bottom-up downheap */

    /* leaf-search for largest child path */
    while (child <= half)
    {
      ++level;
      child += child;
      if ((child < len) &&
		  ((*func_pointer)(This.mData[child]->m_Gi, This.mData[child - 1]->m_Gi) > 0))
        ++child;
    }
    /* bottom-up-search for rotation point */
	tmp = This.mData[parents - 1];
    for (;;)
    {
      if (parents == child)
        break;
      if ((*func_pointer)(tmp->m_Gi, This.mData[child - 1]->m_Gi) <= 0)
        break;
      child >>= 1;
      --level;
    }
    /* rotate nodes from parents to rotation point */
    for (;level > 0; --level)
    {
      This.mData[(child >> level) - 1]=
        This.mData[(child >> (level - 1)) - 1];
    }
    This.mData[child - 1] = tmp;
  }

  --len;
  do
  {
    SortableGameIndex *tmp;
    int level = 0;
    uint child;

    /* move max element to back of array */
    tmp = This.mData[len];
    This.mData[len] = This.mData[0];
    This.mData[0] = tmp;

    child = parents = 1;
    half = len >> 1;

    /* bottom-up downheap */

    /* leaf-search for largest child path */
    while (child <= half)
    {
      ++level;
      child += child;
      if ((child < len) &&
          ((*func_pointer)(This.mData[child]->m_Gi, This.mData[child - 1]->m_Gi) > 0))
        ++child;
    }
    /* bottom-up-search for rotation point */
    for (;;)
    {
      if (parents == child)
        break;
      if ((*func_pointer)(tmp->m_Gi, This.mData[child - 1]->m_Gi) <= 0)
        break;
      child >>= 1;
      --level;
    }
    /* rotate nodes from parents to rotation point */
    for (;level > 0; --level)
    {
      This.mData[(child >> level) - 1]=
        This.mData[(child >> (level - 1)) - 1];
    }
    This.mData[child - 1] = tmp;
  } while (--len >= 1);
}

void heap_sort_SortableGameIndex32M_vector(vector< SortableGameIndex* > &This , compare_func_objects32 func_pointer, unsigned int len)
{
  /* heap sort */

  uint half;
  uint parents;

  if (len <= 1)
    return;

  half = len >> 1;
  for (parents = half; parents >= 1; --parents)
  {
    SortableGameIndex *tmp;
    int level = 0;
    uint child;

    child = parents;
    /* bottom-up downheap */

    /* leaf-search for largest child path */
    while (child <= half)
    {
      ++level;
      child += child;
      if ((child < len) &&
		  ((*func_pointer)(This[child]->m_Gi, This[child - 1]->m_Gi) > 0))
        ++child;
    }
    /* bottom-up-search for rotation point */
    tmp = This[parents - 1];
    for (;;)
    {
      if (parents == child)
        break;
      if ((*func_pointer)(tmp->m_Gi, This[child - 1]->m_Gi) <= 0)
        break;
      child >>= 1;
      --level;
    }
    /* rotate nodes from parents to rotation point */
    for (;level > 0; --level)
    {
      This[(child >> level) - 1] =
        This[(child >> (level - 1)) - 1];
    }
    This[child - 1] = tmp;
  }

  --len;
  do
  {
    SortableGameIndex *tmp;
    int level = 0;
    uint child;

    /* move max element to back of array */
    tmp = This[len];
    This[len] = This[0];
    This[0] = tmp;

    child = parents = 1;
    half = len >> 1;

    /* bottom-up downheap */

    /* leaf-search for largest child path */
    while (child <= half)
    {
      ++level;
      child += child;
      if ((child < len) &&
		  ((*func_pointer)(This[child]->m_Gi, This[child - 1]->m_Gi) > 0))
        ++child;
    }
    /* bottom-up-search for rotation point */
    for (;;)
    {
      if (parents == child)
        break;
      if ((*func_pointer)(tmp->m_Gi, This[child - 1]->m_Gi) <= 0)
        break;
      child >>= 1;
      --level;
    }
    /* rotate nodes from parents to rotation point */
    for (;level > 0; --level)
    {
      This[(child >> level) - 1] =
        This[(child >> (level - 1)) - 1];
    }
    This[child - 1] = tmp;
  } while (--len >= 1);
}

void heap_sort_SortableGameIndex32M_vector_by_number(vector< SortableGameIndex* > &This , compare_func_SortableGi func_pointer, unsigned int len)
	{
  /* heap sort */

  uint half;
  uint parents;

  if (len <= 1)
    return;

  half = len >> 1;
  for (parents = half; parents >= 1; --parents)
  {
    SortableGameIndex *tmp;
    int level = 0;
    uint child;

    child = parents;
    /* bottom-up downheap */

    /* leaf-search for largest child path */
    while (child <= half)
    {
      ++level;
      child += child;
      if ((child < len) &&
		  ((*func_pointer)(This[child]->original_loc, This[child - 1]->original_loc) > 0))
        ++child;
    }
    /* bottom-up-search for rotation point */
    tmp = This[parents - 1];
    for (;;)
    {
      if (parents == child)
        break;
      if ((*func_pointer)(tmp->original_loc, This[child - 1]->original_loc) <= 0)
        break;
      child >>= 1;
      --level;
    }
    /* rotate nodes from parents to rotation point */
    for (;level > 0; --level)
    {
      This[(child >> level) - 1] =
        This[(child >> (level - 1)) - 1];
    }
    This[child - 1] = tmp;
  }

  --len;
  do
  {
    SortableGameIndex *tmp;
    int level = 0;
    uint child;

    /* move max element to back of array */
    tmp = This[len];
    This[len] = This[0];
    This[0] = tmp;

    child = parents = 1;
    half = len >> 1;

    /* bottom-up downheap */

    /* leaf-search for largest child path */
    while (child <= half)
    {
      ++level;
      child += child;
      if ((child < len) &&
		  ((*func_pointer)(This[child]->original_loc, This[child - 1]->original_loc) > 0))
        ++child;
    }
    /* bottom-up-search for rotation point */
    for (;;)
    {
      if (parents == child)
        break;
      if ((*func_pointer)(tmp->original_loc, This[child - 1]->original_loc) <= 0)
        break;
      child >>= 1;
      --level;
    }
    /* rotate nodes from parents to rotation point */
    for (;level > 0; --level)
    {
      This[(child >> level) - 1] =
        This[(child >> (level - 1)) - 1];
    }
    This[child - 1] = tmp;
  } while (--len >= 1);
}


void heap_sort_SortableGameIndex32M_vector_by_Result(vector< SortableGameIndex* > &This , compare_func_SortableGi func_pointer, unsigned int len)
{
  /* heap sort */

  uint half;
  uint parents;

  if (len <= 1)
    return;

  half = len >> 1;
  for (parents = half; parents >= 1; --parents)
  {
    SortableGameIndex *tmp;
    int level = 0;
    uint child;

    child = parents;
    /* bottom-up downheap */

    /* leaf-search for largest child path */
    while (child <= half)
    {
      ++level;
      child += child;
      if ((child < len) &&
		  ((*func_pointer)(This[child]->m_Gi->hdr.result, This[child - 1]->m_Gi->hdr.result) > 0))
        ++child;
    }
    /* bottom-up-search for rotation point */
    tmp = This[parents - 1];
    for (;;)
    {
      if (parents == child)
        break;
      if ((*func_pointer)(tmp->m_Gi->hdr.result, This[child - 1]->m_Gi->hdr.result) <= 0)
        break;
      child >>= 1;
      --level;
    }
    /* rotate nodes from parents to rotation point */
    for (;level > 0; --level)
    {
      This[(child >> level) - 1] =
        This[(child >> (level - 1)) - 1];
    }
    This[child - 1] = tmp;
  }

  --len;
  do
  {
    SortableGameIndex *tmp;
    int level = 0;
    uint child;

    /* move max element to back of array */
    tmp = This[len];
    This[len] = This[0];
    This[0] = tmp;

    child = parents = 1;
    half = len >> 1;

    /* bottom-up downheap */

    /* leaf-search for largest child path */
    while (child <= half)
    {
      ++level;
      child += child;
      if ((child < len) &&
		  ((*func_pointer)(This[child]->m_Gi->hdr.result, This[child - 1]->m_Gi->hdr.result) > 0))
        ++child;
    }
    /* bottom-up-search for rotation point */
    for (;;)
    {
      if (parents == child)
        break;
      if ((*func_pointer)(tmp->m_Gi->hdr.result, This[child - 1]->m_Gi->hdr.result) <= 0)
        break;
      child >>= 1;
      --level;
    }
    /* rotate nodes from parents to rotation point */
    for (;level > 0; --level)
    {
      This[(child >> level) - 1] =
        This[(child >> (level - 1)) - 1];
    }
    This[child - 1] = tmp;
  } while (--len >= 1);
}


void heap_sort_SortableGameIndex32M_vector_VM(vector< int > &This , compare_func_SortableGi func_pointer, unsigned int len)
	{
  /* heap sort */

  uint half;
  uint parents;

  if (len <= 1)
    return;

  half = len >> 1;
  for (parents = half; parents >= 1; --parents)
  {
    int tmp;
    int level = 0;
    uint child;

    child = parents;
    /* bottom-up downheap */

    /* leaf-search for largest child path */
    while (child <= half)
    {
      ++level;
      child += child;
      if ((child < len) &&
		  ((*func_pointer)(This[child], This[child - 1]) > 0))
        ++child;
    }
    /* bottom-up-search for rotation point */
    tmp = This[parents - 1];
    for (;;)
    {
      if (parents == child)
        break;
      if ((*func_pointer)(tmp, This[child - 1]) <= 0)
        break;
      child >>= 1;
      --level;
    }
    /* rotate nodes from parents to rotation point */
    for (;level > 0; --level)
    {
      This[(child >> level) - 1] =
        This[(child >> (level - 1)) - 1];
    }
    This[child - 1] = tmp;
  }

  --len;
  do
  {
    int tmp;
    int level = 0;
    uint child;

    /* move max element to back of array */
    tmp = This[len];
    This[len] = This[0];
    This[0] = tmp;

    child = parents = 1;
    half = len >> 1;

    /* bottom-up downheap */

    /* leaf-search for largest child path */
    while (child <= half)
    {
      ++level;
      child += child;
      if ((child < len) &&
		  ((*func_pointer)(This[child], This[child - 1]) > 0))
        ++child;
    }
    /* bottom-up-search for rotation point */
    for (;;)
    {
      if (parents == child)
        break;
      if ((*func_pointer)(tmp, This[child - 1]) <= 0)
        break;
      child >>= 1;
      --level;
    }
    /* rotate nodes from parents to rotation point */
    for (;level > 0; --level)
    {
      This[(child >> level) - 1] =
        This[(child >> (level - 1)) - 1];
    }
    This[child - 1] = tmp;
  } while (--len >= 1);
}



HeaderListData *g_pHeadersList;


PGN::MemFileContainerDataPageList *g_pDataPageList;


int cmpfun_gameIndexes32M_moves(GameIndex32Moves* a, GameIndex32Moves* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	
	int numA = a->nummoves;
	int numB = b->nummoves;

	int counter = 0;
	
	while( (counter < numA) && (counter < numB) )
	{
		if ( a->moves[counter] > b->moves[counter] )
			return 1;
		else if (a->moves[counter] < b->moves[counter] )
			return -1;
		else
		{
			counter++;
		}
	}

	return 0;
}

int cmpfun_gameIndexes32M_moves_INV(GameIndex32Moves* a, GameIndex32Moves* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	
	int numA = a->nummoves;
	int numB = b->nummoves;

	int counter = 0;
	
	while( (counter < numA) && (counter < numB) )
	{
		if ( a->moves[counter] < b->moves[counter] )
			return 1;
		else if (a->moves[counter] > b->moves[counter] )
			return -1;
		else
		{
			counter++;
		}
	}

	return 0;
}

int cmpfun_gameIndexes32M_number(int a, int b)
{
	if( a > b )
		return 1;
	else if( a < b )
		return -1;
	else return 0;
}
int cmpfun_gameIndexes32M_number_Inv(int a, int b)
{
	if( a < b )
		return 1;
	else if( a > b )
		return -1;
	else return 0;
}


int cmpfun_gameIndexes32M_data1(GameIndex32Moves* a, GameIndex32Moves* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.wname ].id > g_pHeadersList->playerNames.m_set.elems[ b->hdr.wname].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.wname ].id < g_pHeadersList->playerNames.m_set.elems[ b->hdr.wname].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes32M_wname_Inv(GameIndex32Moves* a, GameIndex32Moves* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.wname ].id < g_pHeadersList->playerNames.m_set.elems[ b->hdr.wname].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.wname ].id > g_pHeadersList->playerNames.m_set.elems[ b->hdr.wname].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes32M_elow (GameIndex32Moves* a, GameIndex32Moves* b)
{
	if (g_pHeadersList->playerElos.integers[ a->hdr.whiteElo ] > g_pHeadersList->playerElos.integers[ b->hdr.whiteElo] )
		return 1;
	else if (g_pHeadersList->playerElos.integers[ a->hdr.whiteElo ] < g_pHeadersList->playerElos.integers[ b->hdr.whiteElo] )
		return -1;
	else
		return 0;
}
int cmpfun_gameIndexes32M_elow_Inv (GameIndex32Moves* a, GameIndex32Moves* b)
{
	if (g_pHeadersList->playerElos.integers[ a->hdr.whiteElo ] < g_pHeadersList->playerElos.integers[ b->hdr.whiteElo] )
		return 1;
	else if (g_pHeadersList->playerElos.integers[ a->hdr.whiteElo ] > g_pHeadersList->playerElos.integers[ b->hdr.whiteElo] )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes32M_bname(GameIndex32Moves* a, GameIndex32Moves* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.bname ].id > g_pHeadersList->playerNames.m_set.elems[ b->hdr.bname].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.bname ].id < g_pHeadersList->playerNames.m_set.elems[ b->hdr.bname].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes32M_bname_Inv(GameIndex32Moves* a, GameIndex32Moves* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.bname ].id < g_pHeadersList->playerNames.m_set.elems[ b->hdr.bname].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.bname ].id > g_pHeadersList->playerNames.m_set.elems[ b->hdr.bname].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes32M_elob (GameIndex32Moves* a, GameIndex32Moves* b)
{
	if (g_pHeadersList->playerElos.integers[ a->hdr.blackElo ] > g_pHeadersList->playerElos.integers[ b->hdr.blackElo] )
		return 1;
	else if (g_pHeadersList->playerElos.integers[ a->hdr.blackElo ] < g_pHeadersList->playerElos.integers[ b->hdr.blackElo] )
		return -1;
	else
		return 0;
}
int cmpfun_gameIndexes32M_elob_Inv (GameIndex32Moves* a, GameIndex32Moves* b)
{
	if (g_pHeadersList->playerElos.integers[ a->hdr.blackElo ] < g_pHeadersList->playerElos.integers[ b->hdr.blackElo] )
		return 1;
	else if (g_pHeadersList->playerElos.integers[ a->hdr.blackElo ] > g_pHeadersList->playerElos.integers[ b->hdr.blackElo] )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes32M_Site(GameIndex32Moves* a, GameIndex32Moves* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->siteNames.m_set.elems[ a->hdr.sitename ].id > g_pHeadersList->siteNames.m_set.elems[ b->hdr.sitename].id )
		return 1;
	else if (g_pHeadersList->siteNames.m_set.elems[ a->hdr.sitename].id < g_pHeadersList->siteNames.m_set.elems[ b->hdr.sitename].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes32M_Site_Inv(GameIndex32Moves* a, GameIndex32Moves* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->siteNames.m_set.elems[ a->hdr.sitename ].id < g_pHeadersList->siteNames.m_set.elems[ b->hdr.sitename].id )
		return 1;
	else if (g_pHeadersList->siteNames.m_set.elems[ a->hdr.sitename].id > g_pHeadersList->siteNames.m_set.elems[ b->hdr.sitename].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes32M_Eco(GameIndex32Moves* a, GameIndex32Moves* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->ecoNames.m_set.elems[ a->hdr.eco ].id > g_pHeadersList->ecoNames.m_set.elems[ b->hdr.eco].id )
		return 1;
	else if (g_pHeadersList->ecoNames.m_set.elems[ a->hdr.eco].id < g_pHeadersList->ecoNames.m_set.elems[ b->hdr.eco].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes32M_Eco_Inv(GameIndex32Moves* a, GameIndex32Moves* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->ecoNames.m_set.elems[ a->hdr.eco ].id < g_pHeadersList->ecoNames.m_set.elems[ b->hdr.eco].id )
		return 1;
	else if (g_pHeadersList->ecoNames.m_set.elems[ a->hdr.eco].id > g_pHeadersList->ecoNames.m_set.elems[ b->hdr.eco].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes32M_Event(GameIndex32Moves* a, GameIndex32Moves* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->eventNames.m_set.elems[ a->hdr.eventname ].id > g_pHeadersList->eventNames.m_set.elems[ b->hdr.eventname].id )
		return 1;
	else if (g_pHeadersList->eventNames.m_set.elems[ a->hdr.eventname].id < g_pHeadersList->eventNames.m_set.elems[ b->hdr.eventname].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes32M_Event_Inv(GameIndex32Moves* a, GameIndex32Moves* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->eventNames.m_set.elems[ a->hdr.eventname ].id < g_pHeadersList->eventNames.m_set.elems[ b->hdr.eventname].id )
		return 1;
	else if (g_pHeadersList->eventNames.m_set.elems[ a->hdr.eventname].id > g_pHeadersList->eventNames.m_set.elems[ b->hdr.eventname].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes32M_Date(GameIndex32Moves* a, GameIndex32Moves* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->dateNames.m_set.elems[ a->hdr.date ].id > g_pHeadersList->dateNames.m_set.elems[ b->hdr.date].id )
		return 1;
	else if (g_pHeadersList->dateNames.m_set.elems[ a->hdr.date].id < g_pHeadersList->dateNames.m_set.elems[ b->hdr.date].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes32M_Date_Inv(GameIndex32Moves* a, GameIndex32Moves* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->dateNames.m_set.elems[ a->hdr.date ].id < g_pHeadersList->dateNames.m_set.elems[ b->hdr.date].id )
		return 1;
	else if (g_pHeadersList->dateNames.m_set.elems[ a->hdr.date].id > g_pHeadersList->dateNames.m_set.elems[ b->hdr.date].id )
		return -1;
	else
		return 0;
}


// vm sort functions

int cmpfun_gameIndexes32M_moves_VM(int a, int b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;



			
//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));

	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);
			
	pMfc->GetHeader((void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );
	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));
	RtlCopyMemory(&tempGi32M__A.nummoves, pMfc->vm_compressed_games->data() + tempVMgi__A.game_start, sizeof(unsigned short));
	tempGi32M__A.moves = new unsigned short[tempGi32M__A.nummoves]; // automatically deleted by destructor
	RtlCopyMemory(tempGi32M__A.moves, pMfc->vm_compressed_games->data() + tempVMgi__A.game_start + sizeof(unsigned short), sizeof(unsigned short)*tempGi32M__A.nummoves );





	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
			
	pMfc->GetHeader((void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));
	RtlCopyMemory(&tempGi32M__B.nummoves, pMfc->vm_compressed_games->data() + tempVMgi__B.game_start, sizeof(unsigned short));
	tempGi32M__B.moves = new unsigned short[tempGi32M__B.nummoves]; // automatically deleted by destructor
	RtlCopyMemory(tempGi32M__B.moves, pMfc->vm_compressed_games->data() + tempVMgi__B.game_start + sizeof(unsigned short), sizeof(unsigned short)*tempGi32M__B.nummoves );


	int numA = tempGi32M__A.nummoves;
	int numB = tempGi32M__B.nummoves;

	int counter = 0;
	
	while( (counter < numA) && (counter < numB) )
	{
		if ( tempGi32M__A.moves[counter] > tempGi32M__B.moves[counter] )
		{
			//delete [] tempGi32M__A.moves;
			//delete [] tempGi32M__B.moves;
			return 1;
		}
		else if (tempGi32M__A.moves[counter] < tempGi32M__B.moves[counter] )
		{
			//delete [] tempGi32M__A.moves;
			//delete [] tempGi32M__B.moves;
			return -1;
		}
		else
		{
			counter++;
		}
	}

	//delete [] tempGi32M__A.moves;
	//delete [] tempGi32M__B.moves;

	return 0;
}
int cmpfun_gameIndexes32M_moves_INV_VM(int a, int b)
{
			VM_GameIndex tempVMgi__A;
			GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
//RtlCopyMemory(mb->data, &memoryGameIndex, sizeof(GameIndex));
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);

	pMfc->GetHeader((void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );
	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));
	RtlCopyMemory(&tempGi32M__A.nummoves, pMfc->vm_compressed_games->data() + tempVMgi__A.game_start, sizeof(unsigned short));
	tempGi32M__A.moves = new unsigned short[tempGi32M__A.nummoves]; // automatically deleted by destructor
	RtlCopyMemory(tempGi32M__A.moves, pMfc->vm_compressed_games->data() + tempVMgi__A.game_start + sizeof(unsigned short), sizeof(unsigned short)*tempGi32M__A.nummoves );





	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);

	pMfc->GetHeader((void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));
	RtlCopyMemory(&tempGi32M__B.nummoves, pMfc->vm_compressed_games->data() + tempVMgi__B.game_start, sizeof(unsigned short));
	tempGi32M__B.moves = new unsigned short[tempGi32M__B.nummoves]; // automatically deleted by destructor
	RtlCopyMemory(tempGi32M__B.moves, pMfc->vm_compressed_games->data() + tempVMgi__B.game_start + sizeof(unsigned short), sizeof(unsigned short)*tempGi32M__B.nummoves );


	int numA = tempGi32M__A.nummoves;
	int numB = tempGi32M__B.nummoves;

	int counter = 0;
	
	while( (counter < numA) && (counter < numB) )
	{
		if ( tempGi32M__A.moves[counter] < tempGi32M__B.moves[counter] )
		{
			//delete [] tempGi32M__A.moves;
			//delete [] tempGi32M__B.moves;
			return 1;
		}
		else if (tempGi32M__A.moves[counter] > tempGi32M__B.moves[counter] )
		{
			//delete [] tempGi32M__A.moves;
			//delete [] tempGi32M__B.moves;
			return -1;
		}
		else
		{
			counter++;
		}
	}
			
	//delete [] tempGi32M__A.moves;
	//delete [] tempGi32M__B.moves;
	
	return 0;
}

// vm sort functions
int cmpfun_gameIndexes32M_data1_VM (int a, int b)
{
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
	// Get Header A
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);

	pMfc->GetHeader( (void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );
	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));


	// Get Header B
	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );


	if (g_pHeadersList->playerNames.m_set.elems[ tempGi32M__A.hdr.wname ].id > g_pHeadersList->playerNames.m_set.elems[ tempGi32M__B.hdr.wname].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ tempGi32M__A.hdr.wname ].id < g_pHeadersList->playerNames.m_set.elems[ tempGi32M__B.hdr.wname].id )
		return -1;
	else
		return 0;
}
int cmpfun_gameIndexes32M_wname_Inv_VM (int a, int b)
{
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
	// Get Header A
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);
	
	pMfc->GetHeader( (void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );
	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));


	// Get Header B
	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
	pMfc->GetHeader( (void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));


	if (g_pHeadersList->playerNames.m_set.elems[ tempGi32M__A.hdr.wname ].id < g_pHeadersList->playerNames.m_set.elems[ tempGi32M__B.hdr.wname].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ tempGi32M__A.hdr.wname ].id > g_pHeadersList->playerNames.m_set.elems[ tempGi32M__B.hdr.wname].id )
		return -1;
	else
		return 0;

}
int cmpfun_gameIndexes32M_elow_VM (int a, int b)
{
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
	// Get Header A
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);

	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	pMfc->GetHeader( (void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );
	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));


	// Get Header B
	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	pMfc->GetHeader( (void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );
//	RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));



	if (g_pHeadersList->playerElos.integers[ tempGi32M__A.hdr.whiteElo ] > g_pHeadersList->playerElos.integers[ tempGi32M__B.hdr.whiteElo] )
		return 1;
	else if (g_pHeadersList->playerElos.integers[ tempGi32M__A.hdr.whiteElo ] < g_pHeadersList->playerElos.integers[ tempGi32M__B.hdr.whiteElo] )
		return -1;
	else
		return 0;
}


int cmpfun_gameIndexes32M_elow_Inv_VM (int a, int b)
{
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
	// Get Header A
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);

	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );

	// Get Header B
	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );


	if (g_pHeadersList->playerElos.integers[ tempGi32M__A.hdr.whiteElo ] < g_pHeadersList->playerElos.integers[ tempGi32M__B.hdr.whiteElo] )
		return 1;
	else if (g_pHeadersList->playerElos.integers[ tempGi32M__A.hdr.whiteElo ] > g_pHeadersList->playerElos.integers[ tempGi32M__B.hdr.whiteElo] )
		return -1;
	else
		return 0;

}


int cmpfun_gameIndexes32M_bname_VM (int a, int b)
{
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
	// Get Header A
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);
	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );

	// Get Header B
	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );


	if (g_pHeadersList->playerNames.m_set.elems[ tempGi32M__A.hdr.bname ].id > g_pHeadersList->playerNames.m_set.elems[ tempGi32M__B.hdr.bname].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ tempGi32M__A.hdr.bname ].id < g_pHeadersList->playerNames.m_set.elems[ tempGi32M__B.hdr.bname].id )
		return -1;
	else
		return 0;
}
int cmpfun_gameIndexes32M_bname_Inv_VM (int a, int b)
{
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
	// Get Header A
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);
	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );

	// Get Header B
	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );


	if (g_pHeadersList->playerNames.m_set.elems[ tempGi32M__A.hdr.bname ].id < g_pHeadersList->playerNames.m_set.elems[ tempGi32M__B.hdr.bname].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ tempGi32M__A.hdr.bname ].id > g_pHeadersList->playerNames.m_set.elems[ tempGi32M__B.hdr.bname].id )
		return -1;
	else
		return 0;
}
int cmpfun_gameIndexes32M_elob_VM (int a, int b)
{
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
	// Get Header A
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);
	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );

	// Get Header B
	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );

	if (g_pHeadersList->playerElos.integers[ tempGi32M__A.hdr.blackElo ] > g_pHeadersList->playerElos.integers[ tempGi32M__B.hdr.blackElo] )
		return 1;
	else if (g_pHeadersList->playerElos.integers[ tempGi32M__A.hdr.blackElo ] < g_pHeadersList->playerElos.integers[ tempGi32M__B.hdr.blackElo] )
		return -1;
	else
		return 0;

}
int cmpfun_gameIndexes32M_elob_Inv_VM (int a, int b)
{
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
	// Get Header A
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);
	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );

	// Get Header B
	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );

	if (g_pHeadersList->playerElos.integers[ tempGi32M__A.hdr.blackElo ] < g_pHeadersList->playerElos.integers[ tempGi32M__B.hdr.blackElo] )
		return 1;
	else if (g_pHeadersList->playerElos.integers[ tempGi32M__A.hdr.blackElo ] > g_pHeadersList->playerElos.integers[ tempGi32M__B.hdr.blackElo] )
		return -1;
	else
		return 0;

}
int cmpfun_gameIndexes32M_Site_VM (int a, int b)
{
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
	// Get Header A
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);
	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );

	// Get Header B
	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );

	if (g_pHeadersList->siteNames.m_set.elems[ tempGi32M__A.hdr.sitename ].id > g_pHeadersList->siteNames.m_set.elems[ tempGi32M__B.hdr.sitename].id )
		return 1;
	else if (g_pHeadersList->siteNames.m_set.elems[ tempGi32M__A.hdr.sitename].id < g_pHeadersList->siteNames.m_set.elems[ tempGi32M__B.hdr.sitename].id )
		return -1;
	else
		return 0;

}
int cmpfun_gameIndexes32M_Site_Inv_VM (int a, int b)
{
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
	// Get Header A
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);
	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );

	// Get Header B
	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );


	if (g_pHeadersList->siteNames.m_set.elems[ tempGi32M__A.hdr.sitename ].id < g_pHeadersList->siteNames.m_set.elems[ tempGi32M__B.hdr.sitename].id )
		return 1;
	else if (g_pHeadersList->siteNames.m_set.elems[ tempGi32M__A.hdr.sitename].id > g_pHeadersList->siteNames.m_set.elems[ tempGi32M__B.hdr.sitename].id )
		return -1;
	else
		return 0;

}
int cmpfun_gameIndexes32M_Eco_VM(int a, int b)
{
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
	// Get Header A
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);
	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );

	// Get Header B
	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );



	if (g_pHeadersList->ecoNames.m_set.elems[ tempGi32M__A.hdr.eco ].id > g_pHeadersList->ecoNames.m_set.elems[ tempGi32M__B.hdr.eco].id )
		return 1;
	else if (g_pHeadersList->ecoNames.m_set.elems[ tempGi32M__A.hdr.eco].id < g_pHeadersList->ecoNames.m_set.elems[ tempGi32M__B.hdr.eco].id )
		return -1;
	else
		return 0;
}
int cmpfun_gameIndexes32M_Eco_Inv_VM(int a, int b)
{
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
	// Get Header A
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);
	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );

	// Get Header B
	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );



	if (g_pHeadersList->ecoNames.m_set.elems[ tempGi32M__A.hdr.eco ].id < g_pHeadersList->ecoNames.m_set.elems[ tempGi32M__B.hdr.eco].id )
		return 1;
	else if (g_pHeadersList->ecoNames.m_set.elems[ tempGi32M__A.hdr.eco].id > g_pHeadersList->ecoNames.m_set.elems[ tempGi32M__B.hdr.eco].id )
		return -1;
	else
		return 0;
}
int cmpfun_gameIndexes32M_Event_VM (int a, int b)
{
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
	// Get Header A
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);
	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );

	// Get Header B
	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );



	if (g_pHeadersList->eventNames.m_set.elems[ tempGi32M__A.hdr.eventname ].id > g_pHeadersList->eventNames.m_set.elems[ tempGi32M__B.hdr.eventname].id )
		return 1;
	else if (g_pHeadersList->eventNames.m_set.elems[ tempGi32M__A.hdr.eventname].id < g_pHeadersList->eventNames.m_set.elems[ tempGi32M__B.hdr.eventname].id )
		return -1;
	else
		return 0;
}
int cmpfun_gameIndexes32M_Event_Inv_VM (int a, int b)
{
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
	// Get Header A
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);
	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );

	// Get Header B
	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );



	if (g_pHeadersList->eventNames.m_set.elems[ tempGi32M__A.hdr.eventname ].id < g_pHeadersList->eventNames.m_set.elems[ tempGi32M__B.hdr.eventname].id )
		return 1;
	else if (g_pHeadersList->eventNames.m_set.elems[ tempGi32M__A.hdr.eventname].id > g_pHeadersList->eventNames.m_set.elems[ tempGi32M__B.hdr.eventname].id )
		return -1;
	else
		return 0;
}


int cmpfun_gameIndexes32M_Date_VM (int a, int b)
{
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
	// Get Header A
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);
//	RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );

	// Get Header B
	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );



	if (g_pHeadersList->dateNames.m_set.elems[ tempGi32M__A.hdr.date ].id > g_pHeadersList->dateNames.m_set.elems[ tempGi32M__B.hdr.date].id )
		return 1;
	else if (g_pHeadersList->dateNames.m_set.elems[ tempGi32M__A.hdr.date].id < g_pHeadersList->dateNames.m_set.elems[ tempGi32M__B.hdr.date].id )
		return -1;
	else
		return 0;
}
int cmpfun_gameIndexes32M_Date_Inv_VM (int a, int b)
{
	VM_GameIndex tempVMgi__A;
	GameIndex32Moves tempGi32M__A;

	VM_GameIndex tempVMgi__B;
	GameIndex32Moves tempGi32M__B;

			
	// Get Header A
	PGN::MemoryFileContainer *pMfc = g_pDataPageList->GetMemPageForIndex(a);
	//RtlCopyMemory(&tempVMgi__A, g_pDataPageList->GetDataOffset(a), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__A, a);
	//RtlCopyMemory(&tempGi32M__A.hdr, pMfc->vm_compressed_games->data() + tempVMgi__A.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__A.hdr, tempVMgi__A.header_start );

	// Get Header B
	pMfc = g_pDataPageList->GetMemPageForIndex(b);
	//RtlCopyMemory(&tempVMgi__B, g_pDataPageList->GetDataOffset(b), sizeof(VM_GameIndex));
	g_pDataPageList->CopyIndexMemblockMemoryFromOffset( (void*)&tempVMgi__B, b);
	//RtlCopyMemory(&tempGi32M__B.hdr, pMfc->vm_compressed_games->data() + tempVMgi__B.header_start, sizeof(CompressedHeader));
	pMfc->GetHeader( (void*)&tempGi32M__B.hdr, tempVMgi__B.header_start );


	if (g_pHeadersList->dateNames.m_set.elems[ tempGi32M__A.hdr.date ].id < g_pHeadersList->dateNames.m_set.elems[ tempGi32M__B.hdr.date].id )
		return 1;
	else if (g_pHeadersList->dateNames.m_set.elems[ tempGi32M__A.hdr.date].id > g_pHeadersList->dateNames.m_set.elems[ tempGi32M__B.hdr.date].id )
		return -1;
	else
		return 0;
}






// old sort functions

int cmpfun_gameIndexes_data1(GameIndex* a, GameIndex* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.wname ].id > g_pHeadersList->playerNames.m_set.elems[ b->hdr.wname].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.wname ].id < g_pHeadersList->playerNames.m_set.elems[ b->hdr.wname].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes_wname_Inv(GameIndex* a, GameIndex* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.wname ].id < g_pHeadersList->playerNames.m_set.elems[ b->hdr.wname].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.wname ].id > g_pHeadersList->playerNames.m_set.elems[ b->hdr.wname].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes_bname(GameIndex* a, GameIndex* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.bname ].id > g_pHeadersList->playerNames.m_set.elems[ b->hdr.bname].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.bname ].id < g_pHeadersList->playerNames.m_set.elems[ b->hdr.bname].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes_bname_Inv(GameIndex* a, GameIndex* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.bname ].id < g_pHeadersList->playerNames.m_set.elems[ b->hdr.bname].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.bname ].id > g_pHeadersList->playerNames.m_set.elems[ b->hdr.bname].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes_Site(GameIndex* a, GameIndex* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.sitename ].id > g_pHeadersList->playerNames.m_set.elems[ b->hdr.sitename].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.sitename].id < g_pHeadersList->playerNames.m_set.elems[ b->hdr.sitename].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes_Site_Inv(GameIndex* a, GameIndex* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.sitename ].id < g_pHeadersList->playerNames.m_set.elems[ b->hdr.sitename].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.sitename].id > g_pHeadersList->playerNames.m_set.elems[ b->hdr.sitename].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes_Event(GameIndex* a, GameIndex* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.eventname ].id > g_pHeadersList->playerNames.m_set.elems[ b->hdr.eventname].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.eventname].id < g_pHeadersList->playerNames.m_set.elems[ b->hdr.eventname].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes_Event_Inv(GameIndex* a, GameIndex* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.eventname ].id < g_pHeadersList->playerNames.m_set.elems[ b->hdr.eventname].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.eventname].id > g_pHeadersList->playerNames.m_set.elems[ b->hdr.eventname].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes_Date(GameIndex* a, GameIndex* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.date ].id > g_pHeadersList->playerNames.m_set.elems[ b->hdr.date].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.date].id < g_pHeadersList->playerNames.m_set.elems[ b->hdr.date].id )
		return -1;
	else
		return 0;
}

int cmpfun_gameIndexes_Date_Inv(GameIndex* a, GameIndex* b)
{
//indexedSet.m_set.Get(indexedSet.sortedRemapping[i])	

	
	if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.date ].id < g_pHeadersList->playerNames.m_set.elems[ b->hdr.date].id )
		return 1;
	else if (g_pHeadersList->playerNames.m_set.elems[ a->hdr.date].id > g_pHeadersList->playerNames.m_set.elems[ b->hdr.date].id )
		return -1;
	else
		return 0;
}

