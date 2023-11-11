#include "tree.h"
#include "Dotter.h"

#define RETURN_ERROR(code, message) \
        return Error {code, __LINE__, __FILE__, __func__, message}

#define RETURN_ERROR_AND_DUMP(list, code, message)                                          \
        {                                                                                   \
            tree_dump (tree, Error {code, __LINE__, __FILE__, __func__, message});          \
            tree_graph_dump (tree, Error {code, __LINE__, __FILE__, __func__, message});    \
            return Error {code, __LINE__, __FILE__, __func__, message};                     \
        }

#define PARSE_ERROR(tree, error)            \
        if (error.code != CORRECT)          \
        {                                   \
            tree_dump (tree, error);        \
            tree_graph_dump (tree, error);  \
            return error;                   \
        }

const char* NAME_DOT        = "pic.dot";
const int   MAX_TEXT_SIZE   = 200;

struct Node
{
    Elemt value;
    Node* left;
    Node* right;
};

Error   new_node            (Elemt value, Node** adress);
void    node_ctor           (Elemt value, Node* node);
void    nodes_dtor          (Node* node);
Error   tree_verify         (Tree* tree);
void    tree_dump           (Tree* tree, Error error);
void    nodes_graph_dump    (Node* node, size_t counter);
void    error_graph_dump    (Tree* tree, Error error);
void    print_error         (Error error);

Error new_node (Elemt value, Node** adress)
{
    if (!adress)
        RETURN_ERROR(NULL_POINTER, "Null pointer of pointer of adress.");

    *adress = (Node*) calloc (1, sizeof (Node));

    if (!(*adress))
        RETURN_ERROR(MEM_ALLOC, "Error of allocation memory of node.");

    node_ctor (value, *adress);
    RETURN_ERROR(CORRECT, "");
}

void node_ctor (Elemt value, Node* node)
{
    node->value = value;
    node->left  = NULL;
    node->right = NULL;
}

Error tree_ctor (Tree* tree, const char* name, const char* file, const char* func, int line)
{
    if (!tree)
        RETURN_ERROR(NULL_POINTER, "Null pointer of tree.");

    if (!name || !file || !func || line < 0)
        RETURN_ERROR(INCOR_PARAMS, "Incorrect parameters in constructor");

    tree->size = 0;
    tree->root = NULL;
    tree->name = name;
    tree->file = file;
    tree->func = func;
    tree->line = line;

    RETURN_ERROR(CORRECT, "");
}

Error tree_dtor (Tree* tree)
{
    if (!tree)
        RETURN_ERROR(NULL_POINTER, "Null pointer of pointer of tree.");

    nodes_dtor (tree->root);
    tree->size = 0;
    tree->root = NULL;
    free (tree);
    tree = NULL;
    RETURN_ERROR(CORRECT, "");
}

Error insert_node (Tree* tree, Elemt value)
{
    Error error = tree_verify (tree);
    PARSE_ERROR(tree, error);

    if (!tree->root)
    {
        Node* node = NULL;
        error = new_node (value, &node);
        PARSE_ERROR(tree, error);
        tree->root = node;
        tree->size = 1;
        RETURN_ERROR(CORRECT, "");
    }

    Node* current = tree->root;
    Node* adress  = NULL;
    bool  left    = true;
    while (current)
    {
        adress = current;
        left = (value <= current->value);
        if (value <= current->value)
            current = current->left;
        else
            current = current->right;
    }

    if (left)
        error = new_node (value, &(adress->left));
    else
        error = new_node (value, &(adress->right));
    PARSE_ERROR(tree, error);
    tree->size++;
    RETURN_ERROR(CORRECT, "");
}

void nodes_dtor (Node* node)
{
    if (node->left)
        nodes_dtor (node->left);
    if (node->right)
        nodes_dtor (node->right);
    free (node);
    node = NULL;
}

Error tree_verify (Tree* tree)
{
    if (!tree)                                      RETURN_ERROR(NULL_POINTER,      "Null pointer of tree.");
    if (!tree->file || !tree->func || !tree->name)  RETURN_ERROR(INCOR_PARAMS,      "Null pointer of parameters of tree.");
    RETURN_ERROR(CORRECT, "");
}

void tree_dump (Tree* tree, Error error)
{
    printf (RED_COL);
    printf ("-------------------------------------\n");
    if (error.code != CORRECT)
    {
        print_error (error);
        if (!tree)
        {
            printf (OFF_COL);
            return;
        }

        printf ("Error in tree: %s\n"
                "Called from file: %s, func: %s, line: %d\n",
                tree->name, tree->file, tree->func, tree->line);
    }
    printf ("Size - %llu\n", tree->size);
    printf (YELLOW_COL);
    nodes_print (tree->root, stdout, PRE);
    printf ("\n");
    printf (RED_COL);
    printf ("-------------------------------------\n");
    printf (OFF_COL);
}

Error nodes_print (Node* node, FILE* file, Formats format)
{
    if (!file)
        RETURN_ERROR(NULL_POINTER, "Null pointer of file.");

    if (!node)
    {
        fprintf (file, "nil ");
        RETURN_ERROR(CORRECT, "");
    }

    fprintf (file, "{ ");
    if (format == PRE)
        fprintf (file, "%d ", node->value);
    nodes_print (node->left, file, format);

    if (format == INF)
        fprintf (file, "%d ", node->value);
    nodes_print (node->right, file, format);

    if (format == POST)
        fprintf (file, "%d ", node->value);
    fprintf (file, "} ");

    RETURN_ERROR(CORRECT, "");
}

/*
Error nodes_read (Node* node, FILE* file)
{
    if (!file)
        RETURN_ERROR(NULL_POINTER, "Null pointer of file.");

    if (!node)
    {
        fprintf (file, "nil ");
        return;
    }

    fscanf (file, "{ ");
    fscanf (file, "%d ", node->value);
    nodes_print (node->left, file);
    nodes_print (node->right, file);
    fscanf (file, "} ");
}
*/

void print_error (Error error)
{
    printf ("Error: %s\n"
            "Code: %d\n"
            "File: %s, function: %s, line: %d\n",
            error.message,
            error.code,
            error.file, error.func, error.line);
}

void tree_graph_dump (Tree* tree, Error error)
{
    dtBegin (NAME_DOT);
    nodes_graph_dump (tree->root, 1);
    if (error.code != CORRECT)
        error_graph_dump (tree, error);
    dtEnd ();
    dtRender (NAME_DOT);
}

void nodes_graph_dump (Node* node, size_t counter)
{
    char text[MAX_TEXT_SIZE] = "";
    dtNodeStyle ().shape        ("box")
                  .style        ("rounded, filled")
                  .fontcolor    ("black")
                  .fillcolor    ("#F77152");
    dtLinkStyle ().style        ("bold")
                  .color        ("#4682B4");

    sprintf (text, "%d", node->value);
    dtNode ((int) counter, text);
    if (node->left)
    {
        nodes_graph_dump (node->left, counter * 2 + 1);
        dtLink ((int) counter, (int) counter * 2 + 1, "");
    }
    if (node->right)
    {
        nodes_graph_dump (node->right, counter * 2 + 2);
        dtLink ((int) counter, (int) counter * 2 + 2, "");
    }
}

void error_graph_dump (Tree* tree, Error error)
{
    char text[MAX_TEXT_SIZE] = "";
    dtNodeStyle ().shape        ("box")
                  .style        ("rounded, filled")
                  .fontcolor    ("black")
                  .fillcolor    ("#FFFF00");

    if (!tree)
        sprintf (text,
                "Error: %s\n"
                "Code: %d\n"
                "File: %s, function: %s, line: %d\n",
                error.message, error.code, error.file, error.func, error.line);
    else
        sprintf (text,
                "Error in list: %s\n"
                "Called from file: %s, func: %s, line: %d\n"
                "Error: %s\n"
                "Code: %d\n"
                "File: %s, function: %s, line: %d\n",
                tree->name, tree->file, tree->func, tree->line,
                error.message, error.code, error.file, error.func, error.line);

    dtNode (0, text);
}
