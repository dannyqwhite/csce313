/* 
    File: free_list.hpp

    Author: <your name>
            Department of Computer Science and Engineering
            Texas A&M University
    Date  : <date>

    Modified:

*/

#ifndef _free_list_hpp_                   // include file only once
#define _free_list_hpp_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

class SegmentHeader {

private:

  static const unsigned COOKIE_VALUE = 0xBADB00;
  unsigned int cookie; /* To check whether this is a genuine header! */

    // You will need additional data here!
  
public:

  explicit SegmentHeader(size_t _length, bool _is_free = true, bool parent_inf=false, bool is_left=false);

  
  ~SegmentHeader();
  /* We probably won't need the destructor. */

  void CheckValid();
  /* Check if the cookie is valid. */




    SegmentHeader* prev;
    SegmentHeader* next;
    bool is_free;
    bool is_left;
//int data;
size_t length;
    bool parent_inf;
};

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS  FreeList */
/*--------------------------------------------------------------------------*/

class FreeList {

 private:
  SegmentHeader* head;
  int size;
  int fibo_number = 0;

  /* Here you add whatever private members you need...*/

public:

  explicit FreeList(int length);
  /* This function initializes a new free-list. */

  ~FreeList(); 
  /* We probably don't need a destructor. */ 

  bool Remove(SegmentHeader * _segment); 
  /* Remove the given segment from the given free list. 
     Returns true if the function succeeds.
  */ 

  bool Add(SegmentHeader * _segment); 
  /* Add the segment to the given free list. */

  int get_Fib_num()
  {
      return fibo_number;
  }

  SegmentHeader* getHeader()
  {
      return head;
  }

    int getsize()
    {
        return size;
    }

  
};

#endif 
