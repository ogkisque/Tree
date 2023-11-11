#ifndef TREE_HEADER
#define TREE_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "colors.h"

typedef int Elemt;

#define TREE_CTOR(tree) \
        tree_ctor (tree, #tree, __FILE__, __func__, __LINE__)

enum Error_codes
{
    CORRECT =           0,
    NULL_POINTER =      1,
    MEM_ALLOC =         2,
    INCOR_PARAMS =      3,
    NEGATIVE_SIZE =     4
};

enum Formats
{
    PRE =   0,
    POST =  1,
    INF =   2
};

struct Node;

struct Tree
{
    Node*       root;
    size_t      size;

    const char* name;
    const char* file;
    const char* func;
    int         line;
};

struct Error
{
    Error_codes code;
    int         line;
    const char* file;
    const char* func;
    const char* message;
};

Error   tree_ctor       (Tree* tree, const char* name, const char* file, const char* func, int line);
Error   tree_dtor       (Tree* tree);
Error   insert_node     (Tree* tree, Elemt value);
Error   nodes_print     (Node* node, FILE* file, Formats format);
Error   nodes_read      (Node** node, FILE* file, Formats format);
void    tree_graph_dump (Tree* tree, Error error);
#endif //TREE_HEADER
