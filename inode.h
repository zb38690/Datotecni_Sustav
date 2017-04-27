#ifndef INODE_H
#define INODE_H
#include "global.h"
#include "podatkovni_tok.h"
#include "user.h"
#include "bitmap.h"
#include <stdbool.h>
#include <time.h>
struct
{
    unsigned int magic;
    unsigned int inode_br;
    unsigned int vrime_stvaranja;
    unsigned int vrime_mjenjanja;
    unsigned int korisnik_id;
    unsigned int grupa_id;
    unsigned short mode;
    unsigned int roditelj;
    podatkovni_tok tok_podataka;

} typedef inode;

int init_inode(inode*, user*, unsigned int, unsigned int, void(*fn_pntr)(unsigned short*));
void je_dir(unsigned short*);
void je_dat(unsigned short*);

static void postavi_vrimes(inode*); // vrime stvranja
void postavi_vrimem(inode*); // vrime minjanja

void postavi_mod_d(inode*, bool); // direktorij
void postavi_mod_vc(inode*, bool); // vlasnik cita
void postavi_mod_vp(inode*, bool); // vlasnik pise
void postavi_mod_vi(inode*, bool); // vlasnik izvrsi
void postavi_mod_gc(inode*, bool); // grupa cita
void postavi_mod_gp(inode*, bool); // grupa pise
void postavi_mod_gi(inode*, bool); // grupa izvrsi
void postavi_mod_oc(inode*, bool); // ostali cita
void postavi_mod_op(inode*, bool); // ostali pise
void postavi_mod_oi(inode*, bool); // ostali izvrsi

void printaj_mod(inode*);

ds_adresa *inode_podatke(inode*);
ds_adresa inode_block(inode*, unsigned int);
int inode_postavi_a(inode*, ds_adresa, bitmap*);
static void ucitaj_direktni(ds_adresa*, inode*, unsigned int);
static void ucitaj_indirektni(ds_adresa*, inode*, unsigned int);
static void ucitaj_dindirektni(ds_adresa*, inode*, unsigned int);
#endif // INODE_H_INCLUDED
