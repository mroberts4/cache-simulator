#include <unistd.h>
#include <stdio.h>
#include "Simulator.h"
#include <string>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
#define CACHEVIEW 0
#define DATA 1
#define DEBUG 0
#define BYTES_FOR_WORD 1
using namespace std; 
int currently_clk=0; 			//Simulates the counter

//*****************************************
// Function implementations before main
//*****************************************

Simulator obj;

Simulator:: Simulator()
{

}

Simulator :: ~Simulator()
{
		
}



//*********************************************************************
// Reads the value from the cache
//
//*********************************************************************
void Simulator::ReadCache( CacheSim &cache1, TheResults &results_1, unsigned long ind1,unsigned long line1, unsigned long address,int sets, int associativity_val,string rep_poli)
{
	
	int free_block_val;
	int position_with_upper = obj.GetPostUpper(cache1, ind1, line1,sets, associativity_val);
	
	if(position_with_upper ==-1)
	{
	
		//This is executed when the set is not full.
		
		
				free_block_val = obj.SpaceSet(cache1, ind1,sets, associativity_val);
				if(free_block_val ==0)
				{
					cache1. ts_access[ind1][free_block_val] = currently_clk; 		//updates the ts_access 
					cout<<"Read Instruction: "<<address<<" Hit"<<endl;
					results_1 . read_the_hits++;
				}
				else
				{
					cache1. ts_access[ind1][free_block_val] = currently_clk; 		//updates the ts_access 
					cout<<"Read Instruction: "<<address<<" miss"<<endl;
					results_1.read_the_Misses++; 
				}
				
		
	}
	//reads the hits
	else
	{
		results_1 . read_the_hits++; 
		cache1.ts_access[ind1][position_with_upper] = currently_clk;
		cache1.ts_load[ind1][position_with_upper] = currently_clk;
		cout<<"Read Instruction: "<<address<<" hit"<<endl;
	}
}
//*********************************************************************
// Writes the values from the cache
//
//*********************************************************************
void Simulator::WriteCache( CacheSim &cache1, TheResults &results_1, unsigned long ind1,unsigned long line1, unsigned long address,int sets, int associativity_val,string rep_poli)
{
	
	int free_block_val;
	int full;
	int rlru, rfifo;

	int pos_access, pos_load; 		//Position of the less counter for LRU (access) and FIFO (load)
	
	int position_with_upper = obj.GetPostUpper(cache1, ind1, line1,sets,associativity_val);
	
	if(position_with_upper ==-1)
	{
		results_1 .write_the_misses++; 
		full = obj.SpaceSet(cache1, ind1,sets, associativity_val);
	
	
		if(full ==1)
		{
			//LRU 
			
			if(rep_poli =="lru") //It is LRU
			{
				pos_access = obj.findLessAccessTSset(cache1, ind1,sets, associativity_val);
				cache1.upper_cache[ind1][pos_access] = line1;
				cache1. Cache_Data[ind1][pos_access] = address;
				cache1. ts_access[ind1][pos_access] = currently_clk;
				cache1. ts_load[ind1][pos_access] = currently_clk;
				cout<<"Write Instruction: "<<address<<" evict"<<endl;
				results_1.evicts++;
			}
			//FIFO
			if(rep_poli == "fifo")
			{
				pos_load = obj.findFirst(cache1, ind1,sets,associativity_val);
				cache1. upper_cache[ind1][pos_load]= line1;
				cache1.Cache_Data[ind1][pos_load] = address;
				cache1. ts_access[ind1][pos_load] = currently_clk;
				cache1. ts_load[ind1][pos_load] = currently_clk;
				cout<<"Write Instruction: "<<address<<" evict"<<endl;
				results_1.evicts++;
		
			}
		}
			//This is executed when the set is not full.
		else 
		{
			free_block_val = obj.SpaceSet(cache1, ind1,sets, associativity_val);
			if(free_block_val == 0)
			{
				cache1.upper_cache[ind1][free_block_val] = line1;
				cache1.ts_access[ind1][free_block_val] = currently_clk;
				cache1.ts_load[ind1][free_block_val] = currently_clk;
				cache1.Cache_Data[ind1][free_block_val] = address;
				cout<<"Write Instruction: "<<address<<" hit"<<endl;
				results_1.write_the_hits++;	
			}
			else 
			{
				cache1.ts_access[ind1][free_block_val] = currently_clk; 		//updates the ts_access
				cout<<"Write Instruction: "<<address<<" miss with Write Allocation"<<endl;
				currently_clk++;//ticks to next cycle
				free_block_val = obj.SpaceSet(cache1, ind1,sets,associativity_val);												
				cache1.upper_cache[ind1][free_block_val] = line1;
				cache1.Cache_Data[ind1][free_block_val] = address;				//write allocates missed address
				cache1.ts_access[ind1][free_block_val] = currently_clk; 		//updates the ts_access
				cache1.ts_load[ind1][free_block_val] = currently_clk;			//updates load
				results_1.write_the_hits++;
			}
			
		}
	}
			//Writes the hit
		else
	{
		full = obj.SpaceSet(cache1, ind1,sets, associativity_val);
		if(full==1)
		{
			//LRU 
			
			if(rep_poli =="lru") //It is LRU
			{
				pos_access = obj.findLessAccessTSset(cache1, ind1,sets, associativity_val);
				cache1.upper_cache[ind1][pos_access] = line1;
				cache1. Cache_Data[ind1][pos_access] = address;
				cache1. ts_access[ind1][pos_access] = currently_clk;
				cache1. ts_load[ind1][pos_access] = currently_clk;
				cout<<"Write Instruction: "<<address<<" evict"<<endl;
				results_1.evicts++;
			}
			//FIFO
			if(rep_poli == "fifo")
			{
				pos_load = obj.findFirst(cache1, ind1,sets, associativity_val);
				cache1. upper_cache[ind1][pos_load]= line1;
				cache1.Cache_Data[ind1][pos_load] = address;
				cache1. ts_access[ind1][pos_load] = currently_clk;
				cache1. ts_load[ind1][pos_load] = currently_clk;
				cout<<"Write Instruction: "<<address<<" evict"<<endl;
				results_1.evicts++;
		
			}
		}
		else
		{
		 
			free_block_val = obj.SpaceSet(cache1, ind1,sets, associativity_val);
			if(free_block_val == 0)
			{
				cache1.upper_cache[ind1][free_block_val] = line1;
				cache1.ts_access[ind1][free_block_val] = currently_clk;
				cache1.ts_load[ind1][free_block_val] = currently_clk;
				cache1.Cache_Data[ind1][free_block_val] = address;
				cout<<"Write Instruction: "<<address<<" hit"<<endl;
				results_1.write_the_hits++;	
			}
			else 
			{
				cache1.ts_access[ind1][free_block_val] = currently_clk; 		//updates the ts_access
				cout<<"Write Instruction: "<<address<<" miss with Write Allocation"<<endl;
				currently_clk++;//ticks to next cycle
				free_block_val = obj.SpaceSet(cache1, ind1,sets,associativity_val);												
				cache1.upper_cache[ind1][free_block_val] = line1;
				cache1.Cache_Data[ind1][free_block_val] = address;				//write allocates missed address
				cache1.ts_access[ind1][free_block_val] = currently_clk; 		//updates the ts_access
				cache1.ts_load[ind1][free_block_val] = currently_clk;			//updates load
				results_1.write_the_hits++;
			}
		
		
		}
		
	}
}
// The LRU replacement function 
int Simulator::findLessAccessTSset (CacheSim &cache1, unsigned long ind1,int num_sets, int associativity_val)
{
	//int block_i; 
	int pos_lessAc = 0; //position when the less ts_access is in the set
	unsigned long less_Ac = cache1.ts_access[ind1][0];
	for(ind1; ind1 <associativity_val; ind1++)
	{
		for(int i=0;i<num_sets;i++)
		{
			if(cache1.ts_access[ind1][i] < less_Ac)
			{
				pos_lessAc = i;
			}
		}
	}
	return pos_lessAc;
}
//Actually the FIFO replacement finder
int Simulator::findFirst (CacheSim &cache1, unsigned long ind1, int num_sets,int associativity_val)
{
	int block_i; 
	int pos1 = 0; //position when first in the set
	unsigned long first = cache1.ts_load[ind1][0];
	for(ind1; ind1 <associativity_val; ind1++)
	{
		for(block_i =0;block_i<num_sets;block_i++)
		{
			if(cache1.ts_load[ind1][block_i] == first)
			{
				//less_Ac = cache1.ts_access[ind1][block_i]
				pos1 = block_i;
				return pos1;
			}
		}
	}
	
}

//*******************************************************************************************************
//Starts the cache and initializes the values to zero in the beginning before the cache configurations 
// are implemented
//*******************************************************************************************************
void Simulator:: StartCache(CacheSim &cache1, int num_sets, int associativity_val)
{
	
	int ind_i, block_i;
	for (ind_i = 0; ind_i < associativity_val; ind_i ++)
	{
		
		for(block_i =0; block_i <num_sets; block_i++)
		{
			cache1.Cache_Data[ind_i][block_i] =0;
			cache1.upper_cache[ind_i][block_i] =0;
			cache1.ts_access[ind_i][block_i] =0;
		
		}
		
	}
	
	//return 0;
}


//*********************************************************************
// Gets the upper portion of the line that's addressed by the CPU
//
//*********************************************************************
unsigned long Simulator::MakeUpper(unsigned long address, int words_per_line, int bytes_for_word)
{
	unsigned long line;
	line = ((address/(unsigned long)words_per_line)/(unsigned long)bytes_for_word);
	return line;	
}

//*********************************************************************
// Looks for the determined upper and the position in a set based on the index number
//
//*********************************************************************
int Simulator::GetPostUpper(CacheSim &cache1, unsigned long index, unsigned long line,int sets, int associativity_val)
{
		
	int block_i;
	for (index; index < associativity_val; index++)
	{
		
		for(block_i =0; block_i <sets; block_i++)
		{
			if(cache1.upper_cache[index][block_i] ==line)
			{
				return block_i;			//The upper inside value has been found. 
			}
		
		}
		
	}
	return -1; 
} 

//*********************************************************************
// Gets the index for the sets in the cache
//
//*********************************************************************
unsigned long Simulator:: MakeIndex(int num_of_lines, unsigned long up,int association)
{
	unsigned long index;
	unsigned long lines =(unsigned long)num_of_lines;
	index = up % lines;
	
	if(index ==association)
	{
		index--;
	}
	
	return index;
	
}

//**************************************************************************************
// The implementation verifies if there are or not any free spots as indicated in the index
//
//**************************************************************************************
int Simulator::SpaceSet(CacheSim &cache1, unsigned long ind1, int sets,int associativity_val)
{
		int block_i;
	for (ind1; ind1 < associativity_val; ind1++)
	{
		
		for(block_i =0; block_i <sets; block_i++)
		{
			if(cache1.Cache_Data[ind1][block_i] ==0)
			{
				return block_i;			//A free space has been found. 
			}
		
		}
		
	}
	return 1; //full
}

//*********************************************************************************************
// The function gives a block in a set by the index of the cache memory with no specific order. 
// It returns the first position of the line.\
//**********************************************************************************************
int Simulator::RandFreeSpace(CacheSim &cache1, unsigned long ind1,int sets, int associativity_val)
{
	int block_i;
	for(ind1; ind1 < associativity_val; ind1++)
	{
		for(block_i=0;block_i<sets;block_i++)
		{
			if(cache1.Cache_Data[ind1][block_i]==0)
			{
				return block_i;
			}
		}
	}
	return -1;
}

//**********************************************************
//This function generates the write and read hits, misses and 
//access count based on the dat file and cache configuration values
//**********************************************************
void Simulator::generateOutput(TheResults &cache_res)
{
	
		std::cout <<"Access Count:" <<cache_res.count_access<<endl;
		std::cout <<"Number of Read Hits:"<< cache_res.read_the_hits<<endl;
		std::cout <<"Number of Read Misses:" <<cache_res.read_the_Misses<<endl;
		std::cout <<"Number of Write Hits:"<<cache_res.write_the_hits<<endl;
		std::cout <<"Number of Write Misses:"<<cache_res.write_the_misses<<endl;
		std::cout <<"Number of Evicts:"<<cache_res.evicts<<endl;
		std::cout <<"Hit Rate:"<<cache_res.hitRate<<endl;
		std::cout <<"Miss Rate:"<<cache_res.missRate<<endl;
		std::cout <<"Hit Time:"<<cache_res.hitTime<<endl;
		
}

//The main to be implemented.
int main(int argc,char** argv)
{
	Simulator obj;
	Configs cache_desc;
	TheResults cache_res;
	CacheSim cache_memory;
	int data = 1;
	
	//Default initialization of the results
	cache_res.count_access =0; 
	cache_res.read_the_hits =0;
    cache_res.read_the_Misses =0;
	cache_res.write_the_hits =0;
    cache_res.write_the_misses =0;
	cache_res.evicts = 0; 	
 	cache_res.hitTime=0;
	
	int RoW; //read or write
	int num_reads =0; 
	int num_writes =0;
	unsigned long index=0;
	int userInput;
	int words_per_line=1;
	int bytes_per_word = 1;
	int num_of_sets =4;

	
	//Cache parameter values
	cache_desc.num_of_lines = 8;
	cache_desc.associativity_val = 2;
	cache_desc.size_of_line = 8;
	cache_desc.data_block_size = 32;
	cache_desc.wbs = 64;
	cache_desc.cache_size = 256;
	cache_desc.writeBackArray[8];

	//Initial Values
	cout<< "=======Cache Parameters=======" <<"\n" ;
	cout<<"--Number of lines:" << cache_desc.num_of_lines<<endl;
	cout<<"--Associativity:"<< cache_desc.associativity_val<<endl;
	cout<<"--Size of the Lines:" << cache_desc.size_of_line<<endl;
	cout<<"--Data Cache Size:" <<  cache_desc.data_block_size<<endl;
	cout<<"--Write Buffer Size:" <<  cache_desc.wbs<<endl;
	cout<<" Please Input the type of Benchmark you want to run."<<endl;
	cout<<"1:LRU"<<endl;
	cout<<"2:FIFO"<<endl;
	cin>>userInput;

	//Instantiation section 
	if(userInput == 1)
	{
		cache_desc.rep_poli = "lru";
		
	}
	else
	{
		cache_desc.rep_poli = "fifo";
	}
	;
	words_per_line = cache_desc.size_of_line/bytes_per_word;
	
	//**allocating space 
	 
	unsigned long address;
	cache_memory.Cache_Data= (int**)malloc(num_of_sets * sizeof(int*));
	for(int i=0; i< num_of_sets; i++)
	{
		cache_memory.Cache_Data[i] = (int*)malloc(cache_desc.associativity_val * sizeof(int*));
	}
	
	//allocating for cache upper
	cache_memory.upper_cache = (unsigned long**)malloc(num_of_sets * sizeof(unsigned long*));
	for(int i=0; i< num_of_sets; i++)
	{
		cache_memory.upper_cache[i] = (unsigned long*)malloc(cache_desc.associativity_val * sizeof(unsigned long*));
	}

		//allocating for access time stamp
	
	cache_memory.ts_access = (unsigned long**)malloc (cache_desc.associativity_val * sizeof(long unsigned));
	
	for(int i=0; i< num_of_sets; i++)
	{
	
		cache_memory.ts_access[i] = (unsigned long*)malloc(num_of_sets * sizeof(unsigned long*));
	}
		//allocating for load time stamp
	
	cache_memory.ts_load = (unsigned long**)malloc (cache_desc.associativity_val * sizeof(long unsigned));
	
	for(int i=0; i< num_of_sets; i++)
	{
	
		cache_memory.ts_load[i] = (unsigned long*)malloc(cache_desc.associativity_val * sizeof(unsigned long*));
	}
	

	//input file
	ifstream infile;
	//open input file
	unsigned long line;
	
	infile.open("traceFile.dat");
	
	if(infile.is_open())
	{
		
		
	
		//start cache
		obj.StartCache(cache_memory,num_of_sets,cache_desc.associativity_val);
		
		string fileLine;
	 	// Keep reading data from a file
  		while (std::getline(infile, fileLine)) 
	  	{
			
			sscanf(fileLine.c_str(), "# %lx %d",&address,&RoW);
			
			cache_res.count_access++;
		
			if(RoW == 0)
			{
				line = obj.MakeUpper(address, words_per_line, bytes_per_word);//create upper for processing
				index = obj.MakeIndex(cache_desc.num_of_lines,line,cache_desc.associativity_val);//create index from line
				obj.ReadCache(cache_memory,cache_res,index,line,address,num_of_sets,cache_desc. associativity_val,cache_desc.rep_poli);
				
			}
			else if (RoW == 1)
			{
				line = obj.MakeUpper(address, words_per_line,bytes_per_word);//create upper for processing
				index = obj.MakeIndex(cache_desc.num_of_lines,line,cache_desc.associativity_val);//create index from line
				obj.WriteCache(cache_memory,cache_res,index,line,address,num_of_sets,cache_desc. associativity_val,cache_desc.rep_poli);
				
			}
			else
			{
			cout <<"undefined operation detected:"<<endl;
			
			}
			
			currently_clk += 1;
		}
	}
	else
	{
		cout<<"It didn't open :("<<endl;
	}
	//hit and miss rate calculation
	cache_res.hitRate = double(cache_res.write_the_hits+cache_res.read_the_hits)/cache_res.count_access;
	cache_res.missRate = double(1-cache_res.hitRate);
	cache_res.hitTime=(cache_res.hitRate*cache_res.count_access);
	//Results from cache Simulator
	obj.generateOutput(cache_res);
	ofstream output;
	obj.~Simulator();
	infile.close();
		
	

	return 0;
}
