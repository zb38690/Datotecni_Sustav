#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H
#include "global.h"
#include "bitmap.h"
#include "inode.h"
struct
{
   char ds_ime[MAX_CHAR_LENGTH];
   int magic1;
   unsigned int velicina_bloka;
   unsigned int blok_shift;
   unsigned int br_blokova;
   unsigned int koristeni_blokovi;//<--
   int magic2;
   bitmap bmap;
   ds_adresa slobodni_inode;
   ds_adresa slobodni_prostor;
   inode root_direktorij;
   int magic3;

}typedef superblock;

#endif /* SUPERBLOCK_H */

