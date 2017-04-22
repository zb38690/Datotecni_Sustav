#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED
#include "global.h"
#include <stddef.h>
struct
{
    char ime[MAX_CHAR_LENGTH];
    unsigned int id;
}typedef usr;

struct
{
    char ime[MAX_CHAR_LENGTH];
    unsigned int id;
}typedef grp;

struct
{
    usr u;
    grp g;
}typedef user;

typedef struct sve_grupe
{
    grp g;
    struct sve_grupe *next;
}sve_g;

int dodaj_g(sve_g**, grp*);
static int stvori_g(sve_g **, grp *);
void oslobodi_g(sve_g*);
#endif // USER_H_INCLUDED
