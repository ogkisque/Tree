#include "tree.h"

const char* FILE_OUTPUT = "output.txt";
const char* FILE_INPUT = "input.txt";

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
    nodes_print (tree.root, stdout, PRE);
    printf ("\n");
    nodes_print (tree.root, stdout, INF);
    printf ("\n");
    nodes_print (tree.root, stdout, POST);
    printf ("\n");
    FILE* file = fopen (FILE_OUTPUT, "w");
    nodes_print (tree.root, file, PRE);
    fclose (file);
    tree_dtor (&tree);

    Tree tree1 = {};
    TREE_CTOR(&tree1);
    FILE* file1 = fopen (FILE_INPUT, "r");
    error = nodes_read (&(tree1.root), file1, INF);
    fclose (file1);
    tree_graph_dump (&tree1, error);
    nodes_print (tree1.root, stdout, INF);
    tree_dtor (&tree1);
    return 0;
}
