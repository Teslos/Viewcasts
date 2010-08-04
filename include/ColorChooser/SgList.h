/* 
 * File SgList.hh
 * Written By: Ofer Ben-Shachar, January 1992
 *
 * Definition for the SgList Class
 *
 */


#ifndef _SGLIST_H
#define _SGLIST_H

enum IStatus { SUCCESS, FAILURE };

enum IBoolean { IFALSE, ITRUE };


class SgListNode {
public:
  void * operator new(size_t size);
  
  void operator delete( void * obj );

  void *object;
  SgListNode *prev;
  SgListNode *next;
};


class SgList
{
   SgListNode *head;	 
   SgListNode *tail;
   SgListNode *currentNode;
   int count;

 public:
  void * operator new(size_t size);
  
  void operator delete( void * obj );

  SgList();
   ~SgList();

   // Iterating on the list
   void InitIterate()   {currentNode = 0;}
   void *Next();
   void *Prev();
   

   // Location functions
   IStatus LocateFront();
   IStatus LocateEnd();
   IStatus Locate(void *object);
   int CurrentPosition();  // 1 is the first entry
   IStatus LocatePosition(int i);   
   
   // List manipulating
   IBoolean IsInList(void *object);
   void *Retrieve();
   void *First();
   void *Last();
   IStatus Insert(void *obj);
   IStatus Append(void *obj);
   IStatus Replace(void *obj); // replace current node with obj
   void *Remove();   // removes the current node from the list and returns it
   IStatus Delete(); // deletes the current node from the list.
   void Clear();     // Clears the List DOES not free the objects.
   IBoolean Empty()  {return ((head == 0) ? ITRUE : IFALSE);}
   Count()  {return count;}
};   
;

#endif	// _SGLIST_H
