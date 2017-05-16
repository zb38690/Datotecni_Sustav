#ifndef DS_OPERACIJE_H_INCLUDED
#define DS_OPERACIJE_H_INCLUDED
#include <stdio.h>
#include "direktorij.h"
#include "inode.h"
#include "SuperBlock.h"
#include "putanja.h"



void nulblock(ds_block*);
void oneblock(ds_block*);

int dodaj_korisnika(superblock*);

//void kreiraj_dat();
ds_adresa kreiraj_dir(superblock*, user*, dir*);

//void citaj_dat();
int citaj_dir(inode*, dir*);

void oslobodi_dir(dir*);

//static void pohrani_dat();
static void pohrani_dir(inode*, dir*);
void mjenjaj_dir(superblock*, dir*, path**, user*);

//void brisi_dat();
void brisi_dir(superblock*, dir*, user*);

//void preimenuj_dat();
//void preimenuj_dir();

void printaj_pomoc();
void listaj(dir*);
void listaj_sve(bitmap*, dir*);
void listaj_kor(superblock*);
void mjenjaj_kor(superblock*, user*);
void mjenjaj_mod(superblock*, inode*);
static user *dohvati_svek(bitmap*);
#endif // DS_OPERACIJE_H_INCLUDED
