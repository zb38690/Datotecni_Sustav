#ifndef DIREKTORIJ_H_INCLUDED
#define DIREKTORIJ_H_INCLUDED
#include "global.h"
struct
{
    unsigned int br_inode;
    char ime_inode[MAX_CHAR_LENGTH];
    struct dir_ele *next;
}typedef dir_ele;

struct
{
    unsigned int br_roditelj;
    char ime_roditelj[MAX_CHAR_LENGTH];
    unsigned int br_tren;
    char ime_tren[MAX_CHAR_LENGTH];
    dir_ele *head;
}typedef dir;
#endif // DIREKTORIJ_H_INCLUDED
