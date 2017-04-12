#ifndef DS_DISK_OPERACIJE_H_INCLUDED
#define DS_DISK_OPERACIJE_H_INCLUDED
#include "global.h"
#include "disk_simulator.h"
#include "SuperBlock.h"
#include "inode.h"

long dohvati_adresu(ds_adresa);
void pisi_na_disk(ds_adresa, ds_block*);
void citaj_sa_diska(ds_adresa, ds_block *dsb);
long udaljenost(long adresa, long poz);

//ds_block *superblock_dsblock(superblock);

#endif // DS_DISK_OPERACIJE_H_INCLUDED
