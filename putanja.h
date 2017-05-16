#ifndef PUTANJA_H_INCLUDED
#define PUTANJA_H_INCLUDED
#include <stddef.h>
#include "user.h"
struct
{
    char *ime;
    struct path *next;
}typedef path;

int path_dodaj(path**, char*);
void path_brisi(path**);
static int path_stvori(path**, char*);
void path_printaj(path*, user*);
void path_oslobodi(path*);

#endif // PUTANJA_H_INCLUDED
