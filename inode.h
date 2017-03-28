#ifndef INODE_H
#define INODE_H
#include "global.h"
#include "podatkovni_tok.h"
struct
{
    unsigned int magic;
    unsigned int inode_br;
    unsigned int vrime_stvaranja;
    unsigned int vrime_mjenjanja;
    unsigned int korisnik_id;
    unsigned int grupa_id;
    byte mode[2];
    ds_adresa roditelj;
    podatkovni_tok tok_podataka;

} typedef inode;

#endif // INODE_H_INCLUDED
