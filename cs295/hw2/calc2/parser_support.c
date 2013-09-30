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
node *malloc_op_node(char *operator, node *child_left, node *child_right)
{   printf("Entering malloc_op_node with op '%s', left child '%d', right child '%d'...\n", operator,child_left->val, child_right->val);
#ifdef DEBUG
    printf("Creating an op node with these args:\n"
           "op: '%s'\n"
	   "left: '%d'\n"
	   "right: '%d'\n",
	   operator, child_left, child_right);
    printf("   * left child access test  --> %d\n", child_left->val);
    printf("   * right child access test --> %d\n", child_right->val);
#endif

    node *nodeptr= malloc(sizeof(node));
    if(nodeptr == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   nodeptr->type= NODE_OPERATOR;
        nodeptr->operator= operator;
	nodeptr->left= child_left;
	nodeptr->right= child_right;
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
    {   nodeptr->type= NODE_NUMBER;
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
    if(nodeptr->type == NODE_OPERATOR)
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
    if(nodeptr->type == NODE_NUMBER)
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









