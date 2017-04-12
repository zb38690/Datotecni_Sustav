#ifndef PODATKOVNI_TOK_H_INCLUDED
#define PODATKOVNI_TOK_H_INCLUDED
#include "global.h"

struct
{
    ds_adresa direktni[BR_DIREKTNIH];
    ds_adresa indirektni;
    ds_adresa d_indirektni;
    unsigned long velicina;
}typedef podatkovni_tok;


#endif // PODATKOVNI_TOK_H_INCLUDED
