/* 
    File: free_list.cpp

    Author: <your name>
            Department of Computer Science
            Texas A&M University
    Date  : <date>

    Modified: 

    This file contains the implementation of the class FreeList.

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <iostream>
#include "free_list.hpp"
#include <cassert>

/*--------------------------------------------------------------------------*/
/* NAME SPACES */ 
/*--------------------------------------------------------------------------*/

using namespace std;
/* I know, it's a bad habit, but this is a tiny program anyway... */

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR CLASS SegmentHeader */
/*--------------------------------------------------------------------------*/



SegmentHeader::~SegmentHeader() {
  // You may need to add code here.
}


void SegmentHeader::CheckValid() {
  if (cookie != COOKIE_VALUE) {
    cout << "INVALID SEGMENT HEADER!!" << endl;
    assert(false);
    // You will need to check with the debugger to see how we got into this
    // predicament.
  }
}

SegmentHeader::SegmentHeader(size_t _length, bool _is_free, bool parent_inf0, bool is_left0) {
    length = _length;
    is_free = _is_free;
    cookie = COOKIE_VALUE;
    next = nullptr;
    prev = nullptr;
    is_left = is_left0;
    parent_inf = parent_inf0;
}







/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR CLASS FreeList */
/*--------------------------------------------------------------------------*/

FreeList::FreeList(int length)
{
  // You will need to add code here!
  
    head = nullptr;
    size = 0;
    fibo_number = length;
}
  

FreeList::~FreeList() {
  // You may need to add code here.
  
}

bool FreeList::Add(SegmentHeader * _segment) 
{
    if(head == nullptr)
    {
        head  = _segment;
        size++;
        return true;
    }
    else
    {
        _segment->next = head;
        head->prev = _segment;
        head = _segment;
        size++;
        return true;
    }
}

bool FreeList::Remove(SegmentHeader * _segment) 
{
    if(_segment == head)
    {
        head = _segment->next;
        if(_segment->next != nullptr)
        {
            _segment->next->prev = nullptr;
        }
        _segment->is_free = false;
        _segment->next = nullptr;
        _segment->prev = nullptr;
        size--;
        return true;
    }
    else if(_segment->prev != nullptr && _segment->next == nullptr)//if segment is tail
    {
        _segment->prev->next = nullptr;
        _segment->prev= nullptr;
        _segment->next = nullptr;
        _segment->is_free = false;
        size--;
        return true;
    }
    else
    {
        _segment->prev->next = _segment->next;
        _segment->next->prev = _segment->prev;
        _segment->prev = nullptr;
        _segment->next =nullptr;
        _segment->is_free = false;
        size--;
        return true;
    }
}


