/*============================================================================
| File:		parser_support.c
| Date:		Mon Sep 30 23:00:16 GMT 2013
| Author:	ellerycoleman@fas.harvard.edu
| Abstract:	Implementation of parser support functions.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


#include "parser_support.h"
#include "calc_tokens.h"






/*====================================================================
 | Function: malloc_op_node
 | Abstract: Creates a node for an arithmetic operator.
 +====================================================================*/
struct node *malloc_op_node(int type, node *left, node *right)
{   
    printf("Entering malloc_op_node ");
    printf("with op '%c', left child '%d', right child '%d'...\n", type,left->val, right->val);

#ifdef DEBUG
    printf("Creating an op node with these args:\n"
           "type: '%c'\n"
	   "left: '%d'\n"
	   "right: '%d'\n",
	   type, left, right);
    printf("   * left child access test  --> %d\n", left->val);
    printf("   * right child access test --> %d\n", right->val);
#endif

    node *nodeptr= malloc(sizeof(node));
    if(nodeptr == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   nodeptr->type= type;
        /* nodeptr->operator= operator; */
	nodeptr->left= left;
	nodeptr->right= right;
    }

    printf("Returning from malloc_op_node...\n");
    return nodeptr;
}







/*====================================================================
 | Function: malloc_number_node
 | Abstract: Creates a node for an integer constant
 +====================================================================*/
node *malloc_number_node(int val)
{   node *nodeptr= malloc(sizeof(node));
    if(nodeptr == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   nodeptr->type= 'k';
        nodeptr->val= val;
    }

    return nodeptr;
}







/*====================================================================
 | Function: print_tree
 | Abstract: Writes parse tree to stdout.
 +====================================================================*/
void print_tree(node *nodeptr)
{   
    printf("(");
    if(nodeptr->type != 'k')
    {   
        /* print left branch of tree */
        if(nodeptr->left->type == NODE_OPERATOR)
        {   print_tree(nodeptr->left);
	}
	else
	{   printf("%d ", nodeptr->left->val);
	}

        /* print root of tree */
	printf("%s ", nodeptr->operator);


        /* print right branch of tree */
	if(nodeptr->right->type == NODE_OPERATOR)
        {   print_tree(nodeptr->right);
	}
	else
	{   printf("%d ", nodeptr->right->val);
	}
    }
    else if(nodeptr->type == 'k')
    {   printf("%d\n", nodeptr->val);
    }
    printf(")");
}






/*====================================================================
 | Function: display_node_type
 | Abstract: Writes node type to stdout.
 +====================================================================*/
char *display_node_type(int i)
{   if(i == 0)
    {   return "Operator Node";
    }
    else
    {   return "Number Node";
    }
}





/*====================================================================
 | Function: eval
 | Abstract: traverses a tree and computes the value of the expression
 |           represented by the tree.
 +====================================================================*/
int eval(node *nodeptr)
{   printf("    eval() called with node type: '%c'\n", nodeptr->type);
    int answer=0;

    switch(nodeptr->type)
    {   case 'k': answer= nodeptr->val;
                  break;

        case '+': printf("      - left node is : %d\n", nodeptr->left->val);
                  printf("      - right node is: %d\n", nodeptr->right->val);
                  answer= eval(nodeptr->left) + eval(nodeptr->right);
	          break;
    }

    return answer;
}





