#pragma once

#include <stdbool.h>

#include "parsergen.h"
#include "token.h"

/**
 * Represents a node in the parse tree. To change the size of the children array
 * use the parseTreeNodeRealloc function and remember to update references. If
 * you forget to update the reference to the root, the entire tree is lost
 * forever and causes a memory leak or SIGSEGV.
 */
typedef struct parseTreeNode {
	struct parseTreeNode *parent;     ///< points to the parent node.
	tokenType type;					  ///< the type of the node.
	char *value;					  ///< the pointer to the value of the node,
									  ///< if any. This will be set to NULL if
									  ///< the node does not have any value.
	parserRule *rule;				  ///< the parser rule that was applied to
									  ///< this node.
	int childCount;					  ///< the count of the children of the
									  ///< node.
	struct parseTreeNode *children[]; ///< pointers to the children of the node.
} parseTreeNode;

/**
 * Initializes the parser.
 */
void parserInit();

/**
 * Creates the root parse tree node.
 */
void parseTreeCreateRoot();
/**
 * Creates a new node and attaches it to the parent at the specified index.
 *
 * @param parent a pointer to the parent of the node.
 * @param index the index to add a reference to.
 * @param type the type of the node.
 */
void parseTreeCreateNode(parseTreeNode *parent, int index, tokenType type);
/**
 * Resizes the children array of the node and update references to it. This
 * function cannot and will not update references of the root node, remember to
 * update them when you resize the root node.
 *
 * @param node the pointer to the node to resize.
 * @param newChildCount the new size of the children array. It should not be
 * 						smaller than the original size.
 * @return the pointer to the new node. Trivial references (references in its
 * 		   parent and children) will be automatically updated.
 */
parseTreeNode *parseTreeNodeRealloc(parseTreeNode *node, int newChildCount);
/**
 * Apply a rule to a node and create a leaf for every expression in the rule.
 *
 * @param node a pointer to the node to apply a rule to.
 * @param rule a pointer to the rule to apply.
 * @return the pointer to the new node.
 */
parseTreeNode *parseTreeApplyRule(parseTreeNode *node, parserRule *rule);
/**
 * Copies the reference to the string.
 *
 * @param node a pointer to the node.
 * @param str a pointer to the value to copy.
 */
void parseTreeCopyValue(parseTreeNode *node, char* str);
/**
 * Deallocates and destroys the parse tree.
 */
void parseTreeDestroy();
/**
 * @return whether the parser is in an expected state.
 */
bool parserOk();
/**
 * Runs the parser. You need to initialize the lexer before calling this
 * function.
 *
 * @return a pointer to the root of the parse tree.
 */
parseTreeNode *parserExec();

