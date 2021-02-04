/* 
    File: my_allocator.hpp

    Author: <author>
            Department of Computer Science and Engineering
            Texas A&M University
    Date  : <date>

    Modified:

*/

#ifndef _my_allocator_hpp_                   // include file only once
#define _my_allocator_hpp_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>
#include <vector>
#include <sys/param.h>
#include "free_list.hpp"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   MyAllocator */
/*--------------------------------------------------------------------------*/

class MyAllocator {

 private:

  // YOU MAY NEED TO ADD YOUR OWN DATA STRUCTURES HERE.
  void* start;
  void* end_mem;
  size_t basic;
  size_t total_mem;
  size_t memory_size;
  std::vector<FreeList> vec_of_frees;
  const std::vector<FreeList> vec_of_frees_2; //created this one because the create v free function
                                              //couldn't take a "this" variable, had to be const
                                              //need to merge THOSE somehow...
                                              //nevermind it doesn't work...
                                              //this is the problem in my program
                                              //sorry
                                              //:(
  
  
  
 public:
  MyAllocator(size_t _basic_block_size, size_t _size);
  /* This function initializes the memory allocator and makes a portion of 
     ’_size’ bytes available. The allocator uses a ’_basic_block_size’ as 
     its minimal unit of allocation. 
     NOTE: In the old days one used 'unsigned int' for the size. The C
     Standards Committee has added the 'size_t' data type to ensure to 
     represent memory sizes. This makes the code more portable. Depending
     on the architecture, 'size_t' may or may not be the same 
     as 'unsigned int'. 
  */

  ~MyAllocator();
  /* This function returns any allocated memory to the operating system. 
  */ 

  void* Malloc(size_t _length);
  /* Allocate _length number of bytes of free memory and returns the 
     address of the allocated portion. Returns nullptr when out of memory. 
  */ 

  bool Free(void* _a); 
  /* Frees the section of physical memory previously allocated 
     using ’Malloc’. Returns true if everything ok. */

  void Merge(SegmentHeader* Seg1, SegmentHeader* Seg2);

  void PrtVec();

  uint round(uint x);

  SegmentHeader* getBud(SegmentHeader* Seg);

  int findIDX(SegmentHeader* seg);

  void split(SegmentHeader* Seg, int idx);

  void Creat_v_free(size_t size) const;



};

#endif 
