#ifndef BITMAP_H_INCLUDED
#define BITMAP_H_INCLUDED
#include "blokovni_tok.h"
#include "global.h"
struct
{
    ds_adresa dsa;
    unsigned int granica;
}typedef bitmap;

void nulblock(ds_block*);
#endif // BITMAP_H_INCLUDED
