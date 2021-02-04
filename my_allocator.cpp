/* 
 File: my_allocator.cpp
 
 Author: <your name>
 Department of Computer Science
 Texas A&M University
 Date  : <date>
 
 Modified:
 
 This file contains the implementation of the class MyAllocator.
 
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>
#include "my_allocator.hpp"
#include <assert.h>
#include <iostream>

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
/* FUNCTIONS FOR CLASS MyAllocator */
/*--------------------------------------------------------------------------*/

MyAllocator::MyAllocator(size_t _basic_block_size, size_t _size) {
    // We don't do anything yet...
    start = (char*) (std::malloc(_size));
    basic = _basic_block_size;

    memory_size = _size;
    cout<<"BBS:"<<basic<<endl;
    cout<<"mem_size:"<<memory_size<<endl;

    Creat_v_free(_size);
    size_t real_size = vec_of_frees.back().get_Fib_num()*basic;
    total_mem = real_size;
    end_mem = static_cast<char*>(start)+real_size;

    SegmentHeader* seg33 = new (start) SegmentHeader(real_size,true,false,true);
    vec_of_frees.back().Add(seg33);

    cout<<"Size of Vector of Freelists: "<<vec_of_frees.size()<<endl;
    cout<<"Start Address: "<<(void*)start<<endl;
    cout<<"End Address: "<<(void*)end_mem<<endl;
}

MyAllocator::~MyAllocator() {
    // Same here...
	std::free(start);
}


void* MyAllocator::Malloc(size_t _length) {
    int idx_of_list=-1;

    cout<<"Bytes asked for in malloc(): "<<_length<<endl;
    uint min = round(_length);
    cout<<"Minimum memory allocated: "<<min<<endl;

    if(min>memory_size)
    {
        cout<<"Requested memory exceeds total memory length.\n";
        return nullptr;
    }
    bool Stop = false;
    SegmentHeader* foundSeg = nullptr;
    while(!Stop)
    {
        for(int i=0; i<vec_of_frees.size(); i++)
        {
            foundSeg = vec_of_frees[i].getHeader();
            if(foundSeg != nullptr && foundSeg->length==min)
            {
                Stop = true;
                idx_of_list=i;
                cout<<"Returned block of size:"<<vec_of_frees[i].get_Fib_num()*basic<<endl;
                vec_of_frees[i].Remove(foundSeg);
                return ((char*)foundSeg)+sizeof(SegmentHeader);
            }
            if(foundSeg!= nullptr&& foundSeg->length>min)
            {
                Stop = false;
                idx_of_list =i;
                if(idx_of_list >=2)
                {
                    cout<<"Found block size of :"<<vec_of_frees[i].get_Fib_num()*basic<<endl;
                    split(foundSeg,idx_of_list);
                    break;
                }
            }
        }
        if(foundSeg == nullptr && idx_of_list== -1)
        {
            Stop = true;
            cout<<"No more free blocks found.\n";
            return nullptr;
        }
    }

	
}

bool MyAllocator::Free(void* _a)
{
    cout<<"Freeing:\n";
    if (_a == nullptr)
    {
        return false;
    }
    auto* pSegmentHeader = (SegmentHeader*)((char*)_a - sizeof(SegmentHeader));
    pSegmentHeader->CheckValid();
    pSegmentHeader->is_free = true;
    cout<<"Segment size being freed: "<<pSegmentHeader->length<<endl;
    int index=findIDX(pSegmentHeader);
    cout<<"Index of segment being freed: "<<index<<endl;
    SegmentHeader* bud = nullptr;
    bool Stop = false;
    while(!Stop)
    {
        bud = getBud(pSegmentHeader);
        if(bud->is_free && ((char*)bud) < end_mem && (bud) >= pSegmentHeader)
        {
            if(bud->is_left==true)
            {
                cout<<"start: "<<(void*)pSegmentHeader<<endl;
                cout<<"start is left: "<<pSegmentHeader->is_left<<endl;
                cout<<"Bud : "<<(void*)bud<<endl;
                cout<<"BUd is left: "<<bud->is_left<<endl;
                Merge(bud, pSegmentHeader);
                return true;
            }
            if(bud->is_left==false)
            {
                Merge(pSegmentHeader,bud);
                return true;
            }
        }
        else if(!bud->is_free)
        {
            Stop=true;
            vec_of_frees[index].Add(pSegmentHeader);
            return true;
        }
    }
}

void MyAllocator::Creat_v_free(size_t size) const
{
    int prev1 =0;
    int prev2 =1;
    int current =1;
    while(current*basic <= size)
    {

        //vec_of_frees.push_back(FreeList(current)); **************** this is the annoying line that broke my
                                                                                                    //program
        prev1= prev2;
        prev2= current;
        current = prev1 + prev2;
    }
}

void MyAllocator::Merge(SegmentHeader* Seg1, SegmentHeader* Seg2)
{
    PrtVec();
    cout<<"Merging";
    bool pf= Seg2->parent_inf;
    bool pr= Seg1->parent_inf;
    int index1 = findIDX(Seg1);
    int index2 = findIDX(Seg2);
    vec_of_frees[index1].Remove(Seg1);
    cout<<"index1: "<<index1<<endl;
    cout<<"index2: "<<index2<<endl;
    vec_of_frees[index2].Remove(Seg2);
    Seg1->length = Seg1->length+Seg2->length;
    Seg1->parent_inf = pf;
    Seg1->is_left= pr;
    int index_of_Parent = findIDX(Seg1);
    vec_of_frees[index_of_Parent].Add(Seg1);

}

void MyAllocator::split(SegmentHeader* Seg, int idx)
{
    cout<<"Spliting block of size:"<<Seg->length<<endl;
    int index1 = idx -1; //right child
    int index2 = idx - 2; //left child
    bool parentinh = Seg->parent_inf;
    bool parentLR = Seg->is_left;
    vec_of_frees[idx].Remove(Seg);
    SegmentHeader* newSeg = new((char*)Seg+ (size_t)(vec_of_frees[index2].get_Fib_num()*basic)) SegmentHeader(Seg->length-(size_t)(vec_of_frees[index2].get_Fib_num()*basic),true,parentinh,false);
    vec_of_frees[index1].Add(newSeg);

    Seg->length = (size_t)(vec_of_frees[index2].get_Fib_num()*basic);
    Seg->parent_inf = parentLR;
    Seg->is_left = true;
    Seg->is_free =true;
    vec_of_frees[index2].Add(Seg);
}

void MyAllocator::PrtVec() //TESTING...trying to get results for report
{
    for(int i=0;i<vec_of_frees.size(); i++)
    {
        cout<<"Index:"<<i<<", Freelist's Fib Number: "<<vec_of_frees[i].get_Fib_num()<<", Size of each segment header in this list: "<<vec_of_frees[i].get_Fib_num()*basic<<", number of segments in this list: "<<vec_of_frees[i].getsize()<<endl;
    }
}

uint MyAllocator::round(uint x)
{
    uint minimum = x+sizeof(SegmentHeader);
    if(minimum<basic)
    {
        minimum= basic;
    }
    else
    {
        for(auto & vec_of_free : vec_of_frees)
        {
            if(vec_of_free.get_Fib_num()*basic>minimum)
            {
                minimum = vec_of_free.get_Fib_num()*basic;
                break;
            }
        }
    }
    return minimum;
}

int MyAllocator::findIDX(SegmentHeader* seg)
{
    int idx =-1;
    for(int i=0;i<vec_of_frees.size();i++)
    {
        if(vec_of_frees[i].get_Fib_num()*basic ==seg->length)
        {
            idx =i;
            break;
        }
    }
    return idx;
}


SegmentHeader* MyAllocator::getBud(SegmentHeader* Seg)
{
    if(Seg->is_left == false)
    {
        int index = findIDX(Seg);
        size_t back = (size_t)(vec_of_frees[index-1].get_Fib_num()*basic);
        return (SegmentHeader*) (((char*)Seg)-back);
    }
    else
    {
        return (SegmentHeader*)(((char*)Seg)+Seg->length);
    }
}





