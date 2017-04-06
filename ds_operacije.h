#ifndef DS_OPERACIJE_H_INCLUDED
#define DS_OPERACIJE_H_INCLUDED
#include "user.h"
#include "inode.h"
#include <time.h>
void stvori_dat(user*, inode*); // tko stvara i di
void stvori_dir(user*, inode*);
void stvori_usr(char*, char*); // ime korisnika i grupe
void mjenj_dat(char*); // novo ime datoteke

void postavi_vrimes(inode*); // vrime stvranja
void postavi_vrimem(inode*); // vrime minjanja

void postavi_mod_d(inode*, bool) // direktorij
void postavi_mod_vc(inode*, bool) // vlasnik cita
void postavi_mod_vp(inode*, bool) // vlasnik pise
void postavi_mod_vi(inode*, bool) // vlasnik izvrsi
void postavi_mod_gc(inode*, bool) // grupa cita
void postavi_mod_gp(inode*, bool) // grupa pise
void postavi_mod_gi(inode*, bool) // grupa izvrsi
void postavi_mod_oc(inode*, bool) // ostali cita
void postavi_mod_op(inode*, bool) // ostali pise
void postavi_mod_oi(inode*, bool) // ostali izvrsi
#endif // DS_OPERACIJE_H_INCLUDED
