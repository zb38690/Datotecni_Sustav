#ifndef DS_DISK_OPERACIJE_H_INCLUDED
#define DS_DISK_OPERACIJE_H_INCLUDED
#include "global.h"
#include "disk_simulator.h"
#include "ds_adresa.h"
#define SEKTOR_MULTIPLIER 1
typedef byte ds_block[SEKTOR*SEKTOR_MULTIPLIER];

long dohvati_adresu(ds_adresa);
void pisi_na_disk(ds_adresa, ds_block);
ds_block *citaj_sa_diska(ds_adresa);
long udaljenost(long adresa, long poz);

#endif // DS_DISK_OPERACIJE_H_INCLUDED
