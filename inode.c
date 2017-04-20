#include "inode.h"

int init_inode(inode *node, user *korisnik, unsigned int inode_br, unsigned int roditej, void (*fn_pntr)(unsigned short*))
{
    int i;
    node->magic = INODE_MAGIC;
    node->inode_br = inode_br;
    postavi_vrimes(node);
    node->korisnik_id = korisnik->u.id;
    node->grupa_id = korisnik->g.id;
    (*fn_pntr)(&(node->mode));
    node->roditelj = roditej;

    for(i = 0; i < BR_DIREKTNIH; i++)
        node->tok_podataka.direktni[i] = 0;
    node->tok_podataka.indirektni = NULL;
    node->tok_podataka.d_indirektni = NULL;
    node->tok_podataka.velicina = 0;
}
void je_dir(unsigned short *mod)
{
    (*mod) = 735;//      BIN 101 101 111 1
}

void je_dat(unsigned short *mod)
{
    (*mod) = 734;//      BIN 101 101 111 0
}
void postavi_vrimes(inode *n)
{

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    n->vrime_stvaranja = tm.tm_hour;
    n->vrime_stvaranja = n->vrime_stvaranja << 6;
    n->vrime_stvaranja |= tm.tm_min;
    n->vrime_stvaranja = n->vrime_stvaranja << 5;
    n->vrime_stvaranja |= tm.tm_mday;
    n->vrime_stvaranja = n->vrime_stvaranja << 4;
    n->vrime_stvaranja |= tm.tm_mon+1;
    n->vrime_stvaranja = n->vrime_stvaranja << 12;
    n->vrime_stvaranja |= (tm.tm_year+1900);
    n->vrime_mjenjanja = n->vrime_stvaranja;
}

void postavi_vrimem(inode *n)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    n->vrime_mjenjanja = tm.tm_hour;
    n->vrime_mjenjanja = n->vrime_mjenjanja << 6;
    n->vrime_mjenjanja |= tm.tm_min;
    n->vrime_mjenjanja = n->vrime_mjenjanja << 5;
    n->vrime_mjenjanja |= tm.tm_mday;
    n->vrime_mjenjanja = n->vrime_mjenjanja << 4;
    n->vrime_mjenjanja |= tm.tm_mon+1;
    n->vrime_mjenjanja = n->vrime_mjenjanja << 12;
    n->vrime_mjenjanja |= (tm.tm_year+1900);
}

void postavi_mod_d(inode *node, bool directory)
{
        if(directory)
            node->mode |= 1; //  -->    BIN 000 000 000 1
        else
            node->mode &= 1022;// -->   BIN 111 111 111 0
}

void postavi_mod_vc(inode *node, bool owner_r)
{
    if(owner_r)
        node->mode |= 2;// -->          BIN 000 000 001 0
    else
        node->mode &= 1021;// -->       BIN 111 111 110 1
}

void postavi_mod_vp(inode *node, bool owner_w)
{
    if(owner_w)
        node->mode |= 4;// -->          BIN 000 000 010 0
    else
        node->mode &= 1019;// -->       BIN 111 111 101 1
}

void postavi_mod_vi(inode *node, bool owner_x)
{
    if(owner_x)
        node->mode |= 8;// -->          BIN 000 000 100 0
    else
        node->mode &= 1015;// -->       BIN 111 111 011 1
}

void postavi_mod_gc(inode *node, bool group_r)
{
    if(group_r)
        node->mode |= 16;// -->         BIN 000 001 000 0
    else
        node->mode &= 1007;// -->       BIN 111 110 111 1
}

void postavi_mod_gp(inode *node, bool group_w)
{
    if(group_w)
        node->mode |= 32;// -->         BIN 000 010 000 0
    else
        node->mode &= 991;// -->        BIN 111 101 111 1
}

void postavi_mod_gi(inode *node, bool group_x)
{
    if(group_x)
        node->mode |= 64;// -->         BIN 000 100 000 0
    else
        node->mode &= 959;// -->        BIN 111 011 111 1
}

void postavi_mod_oc(inode *node, bool everyone_r)
{
    if(everyone_r)
        node->mode |= 128;// -->        BIN 001 000 000 0
    else
        node->mode &= 895;// -->        BIN 110 111 111 1
}

void postavi_mod_op(inode *node, bool everyone_w)
{
    if(everyone_w)
        node->mode |= 256;// -->        BIN 010 000 000 0
    else
        node->mode &= 767;// -->        BIN 101 111 111 1
}

void postavi_mod_oi(inode *node, bool everyone_x)
{
    if(everyone_x)
        node->mode |= 512;// -->        BIN 100 000 000 0
    else
        node->mode &= 511;// -->        BIN 011 111 111 1
}

ds_adresa *inode_podatke(inode *node)
{
    unsigned int br_bloka;
    ds_adresa *podataka;

    br_bloka = ceil(((float)node->tok_podataka.velicina)/sizeof(ds_block));
    podataka = (ds_adresa*)malloc(br_bloka * sizeof(ds_adresa));

    if(podataka)
    {
        if(br_bloka <= BR_DIREKTNIH)
        {

        }
    }

}

static void ucitaj_direktni(ds_adresa *dsa, inode *node, unsigned int br_blokova)
{
    unsigned int i;
    for(i = 0; ((i < br_blokova) && (i < BR_DIREKTNIH)); i++)
        *(dsa+i) = node->tok_podataka.direktni[i];
}

static void ucitaj_indirektni(ds_adresa *dsa, inode *node, unsigned int br_blokova)
{
    ucitaj_direktni(dsa, node, br_blokova);
    unsigned int i, j;
    unsigned int br_indirektnih = BR_DIREKTNIH + (sizeof(ds_block)/sizeof(ds_adresa));
    ds_block dsb;

    citaj_sa_diska(node->tok_podataka.indirektni, &dsb);

    for(i = BR_DIREKTNIH, j = 0; ((i < br_blokova) && (i < br_indirektnih)); i++, j++)
        memcpy(dsa + i, dsb + (j * sizeof(ds_adresa)), sizeof(ds_adresa));
}


static void ucitaj_dindirektni(ds_adresa *dsa, inode *node, unsigned int br_blokova)
{
    ucitaj_indirektni(dsa, node, br_blokova);
    unsigned int i, j, k;
    unsigned int br_indirektni = BR_DIREKTNIH + (sizeof(ds_block)/sizeof(ds_adresa));
    unsigned int br_dindirektni = (((sizeof(ds_block)/sizeof(ds_adresa)) * (sizeof(ds_block)/sizeof(ds_adresa))) + br_indirektni);
    ds_adresa dsa_i[(sizeof(ds_block) / sizeof(ds_adresa))];
    ds_block dsb;

    citaj_sa_diska(node->tok_podataka.d_indirektni, &dsb);

    for(i = 0; i < (sizeof(ds_block)/sizeof(ds_adresa)); i++)
        memcpy(dsa_i[i], dsb + (i * sizeof(ds_adresa)), sizeof(ds_adresa));

    k = br_indirektni;
    for(i = 0; ((i < (sizeof(ds_block)/sizeof(ds_adresa))) && (k < br_blokova)); i++)
    {
        citaj_sa_diska(dsa_i[i], &dsb);
        for(k, j = 0; ((k < br_blokova) && (j < (sizeof(ds_block)/sizeof(ds_adresa)))); j++, k++)
            memcpy(dsa + k; dsb + (j * sizeof(ds_adresa)), sizeof(ds_adresa));

    }
}
