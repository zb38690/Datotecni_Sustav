#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H
#include "global.h"

struct
{
   char ds_ime[MAX_CHAR_LENGTH];
   int magic1;
   int velicina_bloka;
   int br_blokova;
   int koristeni_blokovi;
   int magic2;

}typedef superblock;

#endif /* SUPERBLOCK_H */

