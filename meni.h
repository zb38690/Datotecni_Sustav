#ifndef G_MENI_H_INCLUDED
#define G_MENI_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "disk_simulator.h"
#include <string.h>
int g_meni();
void meni_1();
void meni_2();
char *dohvati_ime_diska();
bool datoteka_postoji(char*);
void kreiraj_disk(char*);
#endif // G_MENI_H_INCLUDED
