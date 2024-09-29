#include <cstdio>
#include <cstdlib>
#include <inttypes.h>
#include <iomanip>

#include "sim.h"
#include "cache_module.h"
#include "request.h"
#include "prefetch.h"

#define ReadAddrRequest(x, y) requestAddr(x, y, false)
#define WriteAddrRequest(x, y) requestAddr(x, y, true)

using namespace std;

CacheModule *head_node = nullptr;   // Init the head node of the hierarchy linked list to nullptr
uint32_t total_mem_traffic = 0U;
extern uint32_t Prefetch_N, Prefetch_M;
extern vector<vector<uint32_t>> streamBuffer;
vector<uint32_t> rowbuf, rowbuf1, rowbuf2;
/*  "argc" holds the number of command-line arguments.
    "argv[]" holds the arguments themselves.

    Example:
    ./sim 32 8192 4 262144 8 3 10 gcc_trace.txt
    argc = 9
    argv[0] = "./sim"
    argv[1] = "32"
    argv[2] = "8192"
    ... and so on
*/
int main (int argc, char *argv[]) {
   FILE *fp;			// File pointer.
   char *trace_file;		// This variable holds the trace file name.
   cache_params_t params;	// Look at the sim.h header file for the definition of struct cache_params_t.
   char rw;			// This variable holds the request's type (read or write) obtained from the trace.
   uint32_t addr;		// This variable holds the request's address obtained from the trace.
				// The header file <inttypes.h> above defines signed and unsigned integers of various sizes in a machine-agnostic way.  "uint32_t" is an unsigned integer of 32 bits.

   // Exit with an error if the number of command-line arguments is incorrect.
   if (argc != 9) {
      printf("Error: Expected 8 command-line arguments but was provided %d.\n", (argc - 1));
      exit(EXIT_FAILURE);
   }
    
   // "atoi()" (included by <stdlib.h>) converts a string (char *) to an integer (int).
   params.BLOCKSIZE = (uint32_t) atoi(argv[1]);
   params.L1_SIZE   = (uint32_t) atoi(argv[2]);
   params.L1_ASSOC  = (uint32_t) atoi(argv[3]);
   params.L2_SIZE   = (uint32_t) atoi(argv[4]);
   params.L2_ASSOC  = (uint32_t) atoi(argv[5]);
   params.PREF_N    = (uint32_t) atoi(argv[6]);
   params.PREF_M    = (uint32_t) atoi(argv[7]);
   trace_file       = argv[8];

   // Open the trace file for reading.
   fp = fopen(trace_file, "r");
   if (fp == (FILE *) NULL) {
      // Exit with an error if file open failed.
      printf("Error: Unable to open file %s\n", trace_file);
      exit(EXIT_FAILURE);
   }
    
   // Print simulator configuration.
   printf("===== Simulator configuration =====\n");
   printf("BLOCKSIZE:  %u\n", params.BLOCKSIZE);
   printf("L1_SIZE:    %u\n", params.L1_SIZE);
   printf("L1_ASSOC:   %u\n", params.L1_ASSOC);
   printf("L2_SIZE:    %u\n", params.L2_SIZE);
   printf("L2_ASSOC:   %u\n", params.L2_ASSOC);
   printf("PREF_N:     %u\n", params.PREF_N);
   printf("PREF_M:     %u\n", params.PREF_M);
   printf("trace_file: %s\n", trace_file);
   printf("\n");

   CacheModule L1_Cache(params.BLOCKSIZE, params.L1_SIZE, params.L1_ASSOC, "L1");
   CacheModule L2_Cache(params.BLOCKSIZE, params.L2_SIZE, params.L2_ASSOC, "L2");
   
   Prefetch_N = params.PREF_N;
   Prefetch_M = params.PREF_M;

   // if(Prefetch_N != 0) {
   //    streamBuffer.resize(Prefetch_N);
   // }
   rowbuf.push_back(2);
   rowbuf.push_back(1);
   rowbuf.push_back(3);

   rowbuf1.push_back(4);
   rowbuf1.push_back(5);
   rowbuf1.push_back(6);

   rowbuf2.push_back(12);
   rowbuf2.push_back(13);
   rowbuf2.push_back(14);

   streamBuffer.push_back(rowbuf);
   streamBuffer.push_back(rowbuf1);
   streamBuffer.push_back(rowbuf2);   

   uint32_t rowitr = 0, colitr = 0;
   streamBuffer_Search(5, rowitr, colitr);

   printf("iterators are %u,%u\n", rowitr, colitr);

   streamBuffer_Sync(rowitr, colitr);

   // Read requests from the trace file and execute.
   while (fscanf(fp, "%c %x\n", &rw, &addr) == 2) {	// Stay in the loop if fscanf() successfully parsed two tokens as specified.
      if (rw == 'r')
         ReadAddrRequest(head_node, addr);
      else if (rw == 'w')
         WriteAddrRequest(head_node, addr);
      else {
         printf("Error: Unknown request type %c.\n", rw);
	 exit(EXIT_FAILURE);
      }
   }

   CacheModule *ptr = head_node;
   while (ptr != nullptr) {
      ptr->PrintCacheContents();
      ptr = ptr->next_node;
   }

   printStreamBuffer();
   
   cout << "===== Measurements =====" << endl;
   cout << "a. L1 reads: \t" << L1_Cache.Cache_Read_Requests << endl;
   cout << "b. L1 read misses: \t" << L1_Cache.Cache_Read_Miss << endl;
   cout << "c. L1 writes: \t" << L1_Cache.Cache_Write_Requests << endl;
   cout << "d. L1 write misses: \t" << L1_Cache.Cache_Write_Miss << endl;
   if(params.L1_SIZE != 0)
      L1_Cache.Miss_Rate = ((float)(L1_Cache.Cache_Read_Miss + L1_Cache.Cache_Write_Miss)/(float)(L1_Cache.Cache_Read_Requests + L1_Cache.Cache_Write_Requests));
   cout << std::fixed << std::setprecision(4) << "e. L1 miss rate: \t" << L1_Cache.Miss_Rate << endl;
   cout << "f. L1 writebacks: \t" << L1_Cache.Writeback_Nxt_Lvl << endl;
   cout << "g. L1 prefetches: \t" << 0 << endl;

   cout << "h. L2 reads (demand): \t" << setw(10) << L2_Cache.Cache_Read_Requests << endl;
   cout << "i. L2 read misses (demand): \t" << L2_Cache.Cache_Read_Miss << endl;
   cout << "j. L2 reads (prefetch): \t" << 0 << endl;
   cout << "k. L2 read misses (prefetch): \t" << 0 << endl;
   cout << "l. L2 writes: \t" << L2_Cache.Cache_Write_Requests << endl;
   cout << "m. L2 write misses: \t" << L2_Cache.Cache_Write_Miss << endl;
   if(params.L2_SIZE != 0)
      L2_Cache.Miss_Rate = (float)L2_Cache.Cache_Read_Miss/(float)L2_Cache.Cache_Read_Requests;
   cout << std::fixed << std::setprecision(4) << "n. L2 miss rate: \t" << L2_Cache.Miss_Rate << endl;
   cout << "o. L2 writebacks: \t" << L2_Cache.Writeback_Nxt_Lvl << endl;
   cout << "p. L2 prefetches: \t" << 0 << endl;
   cout << "q. memory traffic: \t" << total_mem_traffic << endl;

   return(0);
}
