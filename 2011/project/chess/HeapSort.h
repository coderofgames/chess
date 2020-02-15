#ifndef HEAP_SORT_H
#define HEAP_SORT_H

#include "ExpandableArray.h"

#include "wx/wxprec.h"
#include <vector>
#include "PgnLoad.h"

using std::vector;



/* global data pointers */
extern HeaderListData *g_pHeadersList;
extern PGN::MemFileContainerDataPageList *g_pDataPageList;

/* Function pointers*/
typedef int (*compare_func_objects)(GameIndex*, GameIndex*);
typedef int (*compare_func_objects32)(GameIndex32Moves*, GameIndex32Moves*);
typedef int (*compare_func_SortableGi)(int, int);

/* heap sort functions */
void heap_sort_GameIndexes(GameIndex *This[], compare_func_objects func_pointer, unsigned int len);
void heap_sort_GameIndexes32M(GameIndex32Moves *This[], compare_func_objects32 func_pointer, unsigned int len);
void heap_sort_GameIndexes32M_vector(vector< GameIndex32Moves* > This , compare_func_objects32 func_pointer, unsigned int len);
void heap_sort_GameIndexes32M_Exp( ExpandingArrayList< SortableGameIndex* > &This, compare_func_objects32 func_pointer, unsigned int len);
void heap_sort_SortableGameIndex32M_vector(vector< SortableGameIndex* > &This , compare_func_objects32 func_pointer, unsigned int len);
void heap_sort_SortableGameIndex32M_vector_by_number(vector< SortableGameIndex* > &This , compare_func_SortableGi func_pointer, unsigned int len);
void heap_sort_SortableGameIndex32M_vector_by_Result(vector< SortableGameIndex* > &This , compare_func_SortableGi func_pointer, unsigned int len);
void heap_sort_SortableGameIndex32M_vector_VM(vector< int > &This , compare_func_SortableGi func_pointer, unsigned int len);

/* heap sort compare function declarations */

// sorting by number
int cmpfun_gameIndexes32M_number(int a, int b);
int cmpfun_gameIndexes32M_number_Inv(int a, int b);

// ram sort
// sorting by moves
int cmpfun_gameIndexes32M_moves(GameIndex32Moves* a, GameIndex32Moves* b);
int cmpfun_gameIndexes32M_moves_INV(GameIndex32Moves* a, GameIndex32Moves* b);

// ram sort functions
int cmpfun_gameIndexes32M_data1 (GameIndex32Moves* a, GameIndex32Moves* b);
int cmpfun_gameIndexes32M_wname_Inv (GameIndex32Moves* a, GameIndex32Moves* b);
int cmpfun_gameIndexes32M_elow (GameIndex32Moves* a, GameIndex32Moves* b);
int cmpfun_gameIndexes32M_elow_Inv (GameIndex32Moves* a, GameIndex32Moves* b);
int cmpfun_gameIndexes32M_bname (GameIndex32Moves* a, GameIndex32Moves* b);
int cmpfun_gameIndexes32M_bname_Inv (GameIndex32Moves* a, GameIndex32Moves* b);
int cmpfun_gameIndexes32M_elob (GameIndex32Moves* a, GameIndex32Moves* b);
int cmpfun_gameIndexes32M_elob_Inv (GameIndex32Moves* a, GameIndex32Moves* b);
int cmpfun_gameIndexes32M_Site (GameIndex32Moves* a, GameIndex32Moves* b);
int cmpfun_gameIndexes32M_Site_Inv (GameIndex32Moves* a, GameIndex32Moves* b);
int cmpfun_gameIndexes32M_Eco(GameIndex32Moves* a, GameIndex32Moves* b);
int cmpfun_gameIndexes32M_Eco_Inv(GameIndex32Moves* a, GameIndex32Moves* b);
int cmpfun_gameIndexes32M_Event (GameIndex32Moves* a, GameIndex32Moves* b);
int cmpfun_gameIndexes32M_Event_Inv (GameIndex32Moves* a, GameIndex32Moves* b);
int cmpfun_gameIndexes32M_Date (GameIndex32Moves* a, GameIndex32Moves* b);
int cmpfun_gameIndexes32M_Date_Inv (GameIndex32Moves* a, GameIndex32Moves* b);


// vm sort
// sorting by moves
int cmpfun_gameIndexes32M_moves_VM(int a, int b);
int cmpfun_gameIndexes32M_moves_INV_VM(int a, int b);

// vm sort functions
int cmpfun_gameIndexes32M_data1_VM (int a, int b);
int cmpfun_gameIndexes32M_wname_Inv_VM (int a, int b);
int cmpfun_gameIndexes32M_elow_VM (int a, int b);
int cmpfun_gameIndexes32M_elow_Inv_VM (int a, int b);
int cmpfun_gameIndexes32M_bname_VM (int a, int b);
int cmpfun_gameIndexes32M_bname_Inv_VM (int a, int b);
int cmpfun_gameIndexes32M_elob_VM (int a, int b);
int cmpfun_gameIndexes32M_elob_Inv_VM (int a, int b);
int cmpfun_gameIndexes32M_Site_VM (int a, int b);
int cmpfun_gameIndexes32M_Site_Inv_VM (int a, int b);
int cmpfun_gameIndexes32M_Eco_VM(int a, int b);
int cmpfun_gameIndexes32M_Eco_Inv_VM(int a, int b);
int cmpfun_gameIndexes32M_Event_VM (int a, int b);
int cmpfun_gameIndexes32M_Event_Inv_VM (int a, int b);
int cmpfun_gameIndexes32M_Date_VM (int a, int b);
int cmpfun_gameIndexes32M_Date_Inv_VM (int a, int b);

// old sort functions
int cmpfun_gameIndexes_data1 (GameIndex* a, GameIndex* b);
int cmpfun_gameIndexes_wname_Inv (GameIndex* a, GameIndex* b);
int cmpfun_gameIndexes_bname (GameIndex* a, GameIndex* b);
int cmpfun_gameIndexes_bname_Inv (GameIndex* a, GameIndex* b);
int cmpfun_gameIndexes_Site (GameIndex* a, GameIndex* b);
int cmpfun_gameIndexes_Site_Inv (GameIndex* a, GameIndex* b);
int cmpfun_gameIndexes_Event (GameIndex* a, GameIndex* b);
int cmpfun_gameIndexes_Event_Inv (GameIndex* a, GameIndex* b);
int cmpfun_gameIndexes_Date (GameIndex* a, GameIndex* b);
int cmpfun_gameIndexes_Date_Inv (GameIndex* a, GameIndex* b);

#endif