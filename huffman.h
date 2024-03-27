#ifndef HUFFMAN_H
#define HUFFMAN_H	 

#define ASCII_SIZE 256

typedef struct TreeNode {
   int label;
   long count;
   struct TreeNode *left;
   struct TreeNode *right;
} TreeNode;

typedef struct ListNode {
   TreeNode *ptr;
   struct ListNode *next;
} ListNode;

ListNode* createListNode(int label, long count)
{
   ListNode * retval = malloc(sizeof(ListNode));
   retval->ptr = malloc(sizeof(TreeNode));
   retval->ptr->label = label;
   retval->ptr->count = count;
   retval->ptr->left = NULL;
   retval->ptr->right = NULL;
   retval->next = NULL;

   return retval;
}

/*ListNode *CreateTreeNode(int label, long count)
{
	ListNode * ret = malloc(sizeof(ListNode));
	ret->label = label;
    ret->count = count; 
	ret->left = NULL;
	ret->right = NULL;
	return ret;
}*/


#endif
