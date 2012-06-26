/*
// newpokelib9.c 	%I%	%Y%	%E%	%U%
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TURBO_C

#include <alloc.h>

#else  /* SunOS, BSD4.2 e RISC 6000 */

#include <malloc.h>

#endif

#include "util.h"
#include "hrotsem.h"
#include "newpokelib.h"

tree_node * mknode(descr,start,length,line)
char * descr;
long int start;
long int length;
long int line;
{
 tree_node * pnode;

 pnode = (tree_node *) malloc(sizeof(tree_node));

 if(pnode == NULL)
   error_msg(mem_fault);

 pnode->right = pnode->left = NULL;

 (pnode->position).token = strdup(descr);

 if((pnode->position).token == NULL)
   error_msg();

 (pnode->position).start = start;
 (pnode->position).length = length;
 (pnode->position).line = line;

 return pnode;			   
}

tree_node * mkleaf(descr,start,length,line,right, left)
char * descr;
long int start;
long int length;
long int line;
tree_node * right, * left;
{
 tree_node * pnode;

 pnode = (tree_node *) malloc(sizeof(tree_node));

 if(pnode == NULL)
   error_msg();

 pnode->right = right;
 pnode->left = left;

 (pnode->position).token = strdup(descr);

 if((pnode->position).token == NULL)
   error_msg(mem_fault);

 (pnode->position).start = start;
 (pnode->position).length = length;
 (pnode->position).line = line;

 return pnode;
}

tree_node * free_tree(root)
tree_node * root;
{
 if(root->right != NULL)
   free_tree(root->right);


 if(root->left != NULL)
   free_tree(root->left);

 if((root->position).token != NULL)
   free((root->position).token);

 free((root->position).token);

 return NULL;
}

tree_node * ajust_tree(root)
tree_node * root;
{
 long start, length, line;
 tree_node * rootaux;
 
 if(root == NULL)
   return NULL;
 
 if(!strcmp((root->position).token,"$C"))
   { /* Check if contains the behavior ( $C ) */
   if(root->left != NULL && root->right != NULL)
     if(!strcmp((root->left->position).token,"(") &&
        !strcmp((root->right->position).token,")"))
        /* join the leaves into one leaf */
        {
        start = (root->left->position).start;
        length  = (root->right->position).start - start +
                  (root->right->position).length;
        line = (root->left->position).line;
        
        rootaux = root;
        root = mknode("$C",start, length,line);
        root->left= rootaux->left->left;
        root->right = rootaux->right->right;

        /* free node */
        free((rootaux->position).token);
        free(rootaux);

        /* Ajust new tree */
        root = ajust_tree(root);
        }
   }
 else
   {
   if(root->left != NULL)
     root->left = ajust_tree(root->left);
   if(root->right != NULL)
     root->right = ajust_tree(root->right);
   }
 
return root;
}

tree_node * get_last_left_node(root)
tree_node * root;
{
 if(root == NULL)
   return NULL;
 if(root->left == NULL && root->right == NULL)  
   return root;
 else
   return (get_last_left_node(root->left));
}

tree_node * get_last_right_node(root)
tree_node * root;
{
 if(root == NULL)
   return NULL;
 if(root->left == NULL && root->right == NULL)  
   return root;
 else
   return (get_last_right_node(root->right));
}

void record_tree(file, root)
FILE * file;
tree_node * root;
{
 if(root == NULL)
   return;
 
 if(root->left != NULL)
    record_tree(file,root->left);

 fprintf(file,"%-10s %ld %6d %6d\n",(root->position).token,
         (root->position).start, (root->position).length,
         (root->position).line);
 
 if(root->right != NULL)
    record_tree(file,root->right);
 
}

/*
** End of newpokelib9.c
*/






