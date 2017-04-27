#ifndef DIREKTORIJ_H_INCLUDED
#define DIREKTORIJ_H_INCLUDED
#include "global.h"
#include <stddef.h>
struct
{
    int (*dodaj)(struct dir*, unsigned int, char*);
    void (*ispisi)(struct dir*);
    void (*brisi_sve)(struct dir*);
    unsigned int (*brisi_ele)(struct dir*, char*);
}typedef dir_op;

typedef struct dir_element
{
    unsigned int br_inode;
    char ime_inode[MAX_CHAR_LENGTH];
    struct dir_element *next;
}dir_ele;

struct
{
    unsigned int br_roditelj;
    char ime_roditelj[MAX_CHAR_LENGTH];
    unsigned int br_tren;
    char ime_tren[MAX_CHAR_LENGTH];
    dir_ele *head;
}typedef dir;


//static void add_end(dir*, int, char*);
static void clear_all(dir*);
static unsigned int clear_ele(dir*, char*);
static void print_all(dir*);
static int add(dir*, unsigned int, char*);
void init_dir_op(dir_op*);
#endif // DIREKTORIJ_H_INCLUDED
