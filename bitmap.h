#ifndef BITMAP_H_INCLUDED
#define BITMAP_H_INCLUDED
#include "global.h"

struct
{
    ds_adresa dsa;
    ds_adresa inode_start;
    ds_adresa prostor_start;
    ds_adresa prostor_stop;
    void (*obradi)(struct bitmap*, const ds_adresa*, void(*fn_pntr)(byte*, byte*), ds_block*);
}typedef bitmap;

void init_bmap(bitmap*);
ds_adresa slobodni_inode(const bitmap*, ds_block*, const ds_adresa*);
ds_adresa slobodni_prostor(const bitmap*, ds_block*, const ds_adresa*);
static void obradi_bmapu(bitmap*, const ds_adresa*, void(*fn_pntr)(byte*, byte*), ds_block*);
void zauzmi_bit(byte*, byte*);
void oslobodi_bit(byte*, byte*);

#endif // BITMAP_H_INCLUDED
