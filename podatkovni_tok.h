#ifndef PODATKOVNI_TOK_H_INCLUDED
#define PODATKOVNI_TOK_H_INCLUDED
#include "global.h"
#include <stddef.h>
struct
{
    ds_adresa direktni[BR_DIREKTNIH];
    ds_adresa indirektni;
    ds_adresa d_indirektni;
    unsigned long velicina;
}typedef podatkovni_tok;

struct
{
    ds_adresa *indi_block;
}typedef indirektni;

int init_indi(indirektni*);
void uinit_indi(indirektni*);
#endif // PODATKOVNI_TOK_H_INCLUDED
