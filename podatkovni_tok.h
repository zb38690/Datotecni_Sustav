#ifndef PODATKOVNI_TOK_H_INCLUDED
#define PODATKOVNI_TOK_H_INCLUDED
#include "blokovni_tok.h"
#include "global.h"

struct
{
    blokovni_tok direktni[BR_DIREKTNIH];
    blokovni_tok indirektni;
    blokovni_tok d_indirektni;
    unsigned int velicina;}typedef podatkovni_tok;


#endif // PODATKOVNI_TOK_H_INCLUDED
