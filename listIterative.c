/*  Version that uses pointers to pointers */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "huffman.h"


/*  Insert a value into the list

    loc: the address of a next pointer to an element in the list
        (can also think of this as a pointer to the next pointer)
        (if loc is &head, this inserts at the beginning of the list)
    val: the new value to insert

    result: insert a new node with val in it, pointed to by * loc

    Before:

    loc ----------┐
                  ↓
         ┌-----┬------┐   ┌-----┬------┐
         |  x  | next-|-->|  y  |   ---|--┐
         └-----┴------┘   └-----┴------┘  ┴

    After:

    loc ----------┐
                  ↓
         ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐
         |  x  | next-|-->| val |   ---|-->|  y  |   ---|--┐
         └-----┴------┘   └-----┴------┘   └-----┴------┘  ┴
*/
void insertNode(ListNode * * loc, TreeNode* val) {
    //Allocate the new node
    ListNode * newNode = malloc(sizeof(ListNode));
    //Set up the value
    newNode->ptr = val;
    //The new node points to whatever * nextAddr points to
    newNode->next = * loc;
    //* nextAddr now points to the new node
    * loc = newNode;
}

/*  Remove a value from the list

    loc: the address of a next pointer that _points to the node to remove_
        (can also think of this as the pointer to the pointer)
        (if loc is &head, this removes the first element from the list)

    result: * loc points _past_ the node it used to point to

    Before:

    loc ----------┐
                  ↓
         ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐
         |  x  | next-|-->|  y  |   ---|-->|  z  |   ---|--┐
         └-----┴------┘   └-----┴------┘   └-----┴------┘  ┴

    After:

    loc ----------┐
                  ↓
         ┌-----┬------┐   ┌-----┬------┐
         |  x  | next-|-->|  z  |   ---|--┐
         └-----┴------┘   └-----┴------┘  ┴
*/

void removeNode(ListNode * * loc) {
    //If *loc points to NULL, then there isn't a node to remove
    if (* loc == NULL) return;
    //Save pointer to the thing we're going to delete so we can free it
    ListNode * toDelete = * loc;
    //* loc now points to * loc's *next* node
    * loc = (* loc)->next;
    //Free the removed node
    free(toDelete);
}

ListNode* rmvhead(ListNode * * head)
{
    ListNode* todelete = *head;
    if(todelete != NULL)
    {
        *head = todelete->next;
        todelete->next = NULL;
    }
    
    return todelete;
    
}

/*  Print out the list

    head: pointer to the node at which to start printing
    fp: file pointer to print to (pass stdout to print to stdout)
*/

void printList(ListNode * head, FILE * fp) {
    // cursor pointing to first node in the list
    ListNode * cur = head;
    // as long as the cursor hasn't hit the end of the list
    while (cur != NULL) {
        //print the current value
        fprintf(fp, "%c:%ld \n", cur->ptr->label, cur->ptr->count);
        //move the cursor down the list
        cur = cur->next;
    }
}


/*  Find a pointer to a node that contains a value larger than a key

    loc: the address of the pointer that points to the node where you want to start searching
    key: the value you are looking for

    returns the _address_ of the _pointer_ that points to the first node in the list
    whose val is greater than key. Note: if you want to insert right in front of that
    node, this is the pointer you want to use!

    loc ----------┐
                  ↓
         ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐
         |  2  | next-|-->|  4  |   ---|-->|  6  |   ---|--┐
         └-----┴------┘   └-----┴------┘   └-----┴------┘  ┴

    If we're searching for _6_:

    loc ----------┐                ┌---- return the address of this
                  ↓                ↓  
         ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐
         |  2  | next-|-->|  4  |   ---|-->|  7  |   ---|--┐
         └-----┴------┘   └-----┴------┘   └-----┴------┘  ┴

    Note: if all the values in the linked list are <= key, this will return the address of
    the last next pointer in the list. If we're searching for _8_

    loc ----------┐    return the address of this---┐
                  ↓                                 ↓
         ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐
         |  2  | next-|-->|  4  |   ---|-->|  7  |   ---|--┐
         └-----┴------┘   └-----┴------┘   └-----┴------┘  ┴

*/
ListNode * * findGt(ListNode * * loc, TreeNode * key) {
    //While we're not pointing to an empty list
    while ((* loc) != NULL) {
        //if we're pointing to a value > key, return the address of this pointer
        if ((* loc)->ptr->count > key->count)
        {
            return loc;
        } 
        if ((* loc)->ptr->count == key->count)
        {
            if((* loc)->ptr->label > key->label)
            {
                return loc;
            }
        }
        
        //otherwise, move to the next pointer
        //this is tricky:
        //   1. Get to the pointer to the node itself: * loc
        //   2. Get to the next pointer of the node: (* loc)->next
        //   3. Get the address of that next pointer: &((* loc)->next)
        loc = &((* loc)->next);
    }
    //Return the address of the pointer that points to node > key
    return loc;
}

/*  Add a new value into a sorted list

    loc: the address of the pointer to the list we want to add into
    key: the key to add to the list

    loc ----------┐
                  ↓
         ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐
         |  2  | next-|-->|  4  |   ---|-->|  6  |   ---|--┐
         └-----┴------┘   └-----┴------┘   └-----┴------┘  ┴

    If we're adding 5:

    loc ----------┐
                  ↓
         ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐
         |  2  | next-|-->|  4  |   ---|-->|  5  |   ---|-->|  6  |   ---|--┐
         └-----┴------┘   └-----┴------┘   └-----┴------┘   └-----┴------┘  ┴
*/

void addNode(ListNode * * loc, TreeNode* key) {
    //Find the address of a pointer that points to the node we want to insert in front of:
    ListNode * * insertLoc = findGt(loc, key);
    //Insert in front of that location:
    insertNode(insertLoc, key);
}

/*  Free a list

    list: address of pointer to list
*/
void freeList(ListNode * * list) {
    // while list is not empty
    while (* list != NULL) {
        //remove the first element in the list
        removeNode(list);
    }
}

void freeTreeNode(TreeNode * t) {
	free(t);
}

void freeTree(TreeNode * t) {
	if (t == NULL) return;
	freeTree(t->left);
	freeTree(t->right);
	freeTreeNode(t);
}



