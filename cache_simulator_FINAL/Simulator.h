//Includes 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#ifndef SIMULATOR_H
#define SIMULATOR_H
using namespace std;
//Structs to be used

 struct TheResults
{
	
	int read_the_Misses;	
	int write_the_hits; 
	int write_the_misses;
	int count_access;				//Counts the access portions
	int read_the_hits;
	int evicts;
	double hitRate;
	double missRate;
	double hitTime;
};

 struct Configs 
{
	int associativity_val;			//Cache associativity value (number of blocks per set)
	string rep_poli;    	   		//The cache replacement policies (either fifo or lru)
	int data_block_size;			//block size
	int num_of_lines; 				//Number of lines
	int size_of_line;				//size of the line
	int wbs;						//Write buffer size
	int cache_size;					//size of cache
	unsigned long writeBackArray[]; //array that holds addresses as the write buffer 					

};

 struct CacheSim
{
	int **Cache_Data;				//the array name which has the [set][line] in the parameters
	unsigned long **upper_cache;	//Differenciates the two or more blocks in the index that are in the same set
	unsigned long **ts_access;		//The time stamp that uses a counter and is used for the LRU and FIFO algorithms
	unsigned long **ts_load;		//timestamp load counter
};

class Simulator
{
	private:
		
	public:
	
		void generateOutput(TheResults &cache_res);
		void ReadCache( CacheSim &cache1,TheResults &results_1,unsigned long ind1,unsigned long line1, unsigned long address,int sets, int associativity_val,string rep_poli);
		void WriteCache( CacheSim &cache1, TheResults &results_1, unsigned long ind1,unsigned long line1, unsigned long address,int sets, int associativity_val,string rep_poli);
		void StartCache(CacheSim &cache1,int num_sets, int associativity);
		unsigned long MakeUpper(unsigned long address, int words_per_line, int bytes_for_word);
		int GetPostUpper(CacheSim &cache1, unsigned long index, unsigned long line, int sets,int associativity); 
		unsigned long MakeIndex(int num_of_lines,unsigned long up,int association);
		int findLessAccessTSset (CacheSim &cache1, unsigned long ind1, int sets,int associativity);
		int SpaceSet(CacheSim &cache1, unsigned long ind1, int sets,int associativity);
		int RandFreeSpace(CacheSim &cache1, unsigned long ind1, int sets,int associativity);
		int findFirst (CacheSim &cache1, unsigned long ind1, int sets,int associativity);
		
	Simulator ();
	~ Simulator ();
}; 
#endif


