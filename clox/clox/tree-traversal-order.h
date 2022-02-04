#ifndef CLOX_TREE_TRAVERSAL_ORDER_H
#define CLOX_TREE_TRAVERSAL_ORDER_H

enum clox_tree_traversal_order {
    // Depth First Traversals: 
    //
    // Left, Root then Right
    CLOX_TREE_TRAVERSAL_ORDER_INORDER,
    // Root, Left then Right
    CLOX_TREE_TRAVERSAL_ORDER_PREORDER,
    // Left, Right then Root
    CLOX_TREE_TRAVERSAL_ORDER_POSTORDER,
    
    // Breadth-First Traversal:
    CLOX_TREE_TRAVERSAL_ORDER_BREADTH_FIRST,
};

#endif
