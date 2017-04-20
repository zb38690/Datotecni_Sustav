#ifndef DS_OPERACIJE_H_INCLUDED
#define DS_OPERACIJE_H_INCLUDED
#include <stdio.h>
#include "direktorij.h"
#include "inode.h"
#include "SuperBlock.h"



void nulblock(ds_block*);
void oneblock(ds_block*);

void dodaj_korisnika(superblock*);

void kreiraj_dat();
ds_adresa kreiraj_dir(superblock*, user*, dir*);

void citaj_dat();
void citaj_dir();

static void pohrani_dat();
static void pohrani_dir(inode*, dir*);

void brisi_dat();
void brisi_dir();

void preimenuj_dat();
void preimenuj_dir();

void mjenjaj_dat();
#endif // DS_OPERACIJE_H_INCLUDED
