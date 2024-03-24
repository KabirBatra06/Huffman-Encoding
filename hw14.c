#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listIterative.c"
#include "utility.h"

/* count the occurrences in a file */

long *countLetters(FILE *fp)
{
   long *asciiCount = (long *)malloc(sizeof(long)*ASCII_SIZE);
   if (asciiCount == NULL) {
      return NULL;
   }
   int ch;
   for (ch = 0; ch < ASCII_SIZE; ch++) {
      asciiCount[ch] = 0;
   }
   fseek(fp, 0, SEEK_SET);
   while ((ch = fgetc(fp)) != EOF) {
      asciiCount[ch] += 1;
   }
   return asciiCount;
}

ListNode* branch(TreeNode* left, TreeNode* right)
{
   ListNode* retval = malloc(sizeof(ListNode));
   retval->ptr = malloc(sizeof(TreeNode));
   retval->ptr->left = left;
   retval->ptr->right = right;
   retval->ptr->count = left->count + right->count;
   retval->ptr->label = 500; 
   return retval;

}

int isleaf(TreeNode* temp)
{
   if(temp == NULL)
   {
      return 0;
   }
   else if(temp->left == NULL && temp->right == NULL)
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

void binaryprinthelper(TreeNode* head, int idx, char* path, FILE* out2, FILE* out3, unsigned char* whichbit, unsigned char* curbyte)
{
   if(head == NULL)
   {
      return;
   }
   else if(isleaf(head) == 1) 
   {
      path[idx] = '\0';
      fprintf(out2, "%c:%s\n", head->label, path);
      writeBit(out3,1, whichbit, curbyte);
      int n = 128;
      for(int j = 7; j >= 0; j--)
      {
         writeBit(out3,(head->label & n) >> j, whichbit, curbyte);
         n = n/2;
      }   
      return;
   }
   else
   {
      writeBit(out3,0, whichbit, curbyte);
      path[idx] = '0';
      binaryprinthelper(head->left, idx + 1, path, out2, out3, whichbit, curbyte);
      path[idx] = '1';
      binaryprinthelper(head->right, idx + 1, path, out2, out3, whichbit, curbyte);
   }
}

void binaryprint(TreeNode* head, FILE* out2, FILE* out3)
{
   char path[256];
   unsigned char whichbit = 0;
   unsigned char curbyte = 0;

   if(head == NULL)
   {
      return;
   }
   else
   {
      binaryprinthelper(head, 0, path, out2, out3, &whichbit, &curbyte);
      padZero(out3, &whichbit, &curbyte);
   }

}
int main(int argc, char **argv)
{
   if (argc != 5) {
    printf("Not enough arguments");
    return EXIT_FAILURE;
   }
   FILE * inFile = fopen(argv[1], "r");
   if (inFile == NULL) {
      fprintf(stderr, "can't open the input file.  Quit.\n");
      return EXIT_FAILURE;
   }

   /* read and count the occurrences of characters */
   long *asciiCount = countLetters(inFile);
   fclose(inFile);

   if (asciiCount == NULL) {
      fprintf(stderr, "cannot allocate memory to count the characters in input file.  Quit.\n");
      return EXIT_FAILURE;
   }
 
   // Your code should go here

   FILE * out1 = fopen(argv[2], "w");
   if (out1 == NULL) {
      return EXIT_FAILURE;
   }
   FILE * out2 = fopen(argv[3], "w");
   if (out2 == NULL) {
      return EXIT_FAILURE;
   }
   FILE * out3 = fopen(argv[4], "w");
   if (out3 == NULL) {
      return EXIT_FAILURE;
   }


   ListNode* temp = NULL;
   ListNode* head = NULL;
   for(int i = 0; i < ASCII_SIZE; i++)
   {
      if(asciiCount[i] != 0)
      {
         temp = createListNode(i,asciiCount[i]);
         addNode(&(head),temp->ptr);
      }
   }  
   printList(head, out1);

   while(head != NULL && head->next != NULL)
   {
      ListNode* t1 = rmvhead(&(head));
      ListNode* t2 = rmvhead(&(head));
      ListNode* newbranch = branch(t1->ptr, t2->ptr);
      addNode(&(head), newbranch->ptr);
      free(t1);
      free(t2);
   }

   binaryprint(head->ptr, out2, out3);
   freeTree(head->ptr);
   free(head);
   free(asciiCount);



   fclose(out1);
   fclose(out2);
   fclose(out3);
}