#include "tree.h"

const char* FILE_NAME = "output.txt";

int main ()
{
    Tree tree = {};
    TREE_CTOR(&tree);
    Error error = insert_node (&tree, 10);
    insert_node (&tree, 5);
    insert_node (&tree, 15);
    insert_node (&tree, 12);
    insert_node (&tree, 16);
    insert_node (&tree, 13);
    insert_node (&tree, 8);
    insert_node (&tree, 2);
    tree_graph_dump (&tree, error);
    nodes_print (tree.root, stdout, PRE);
    printf ("\n\n");
    nodes_print (tree.root, stdout, INF);
    printf ("\n\n");
    nodes_print (tree.root, stdout, POST);
    printf ("\n\n");

    FILE* file = fopen (FILE_NAME, "w");
    nodes_print (tree.root, file, INF);
    fclose (file);
    tree_dtor   (&tree);
    return 0;
}
