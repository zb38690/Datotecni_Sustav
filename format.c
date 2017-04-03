#include "format.h"

void format()
{
    superblock sb;
    ds_adresa dsa;
    ds_block dsb;
    float bitmap = ((float)(velicina())/(sizeof(ds_block)*8));

    char c[29] = "Jednostavni Datotecni Sustav\0";

    strcpy(sb.ds_ime, c);
    sb.magic1 = SUPERBLOCK_MAGIC1;
    sb.velicina_bloka = sizeof(ds_block);
    sb.blok_shift = log2(sizeof(ds_block));
    sb.br_blokova = velicina()/sb.velicina_bloka;
    //sb.koristeni_blokovi = 123456789; <---------------------------------
    sb.alokacijske_grupe = ceil(bitmap/sizeof(ds_block));
    sb.magic2 = SUPERBLOCK_MAGIC2;
    sb.ag_shift = log2(sizeof(ds_block) * 8);

    postavi_adrese(&sb);

    //sb.root_direktorij = sb.slobodni_prostor;// <-----------

    sb.magic3 = SUPERBLOCK_MAGIC3;

    dsa.alokacijska_grupa = 0;
    dsa.poz = 0;

    memcpy(dsb, &sb, sizeof(superblock));

    pisi_na_disk(dsa, &dsb);
}

bool formatiran()
{
    ds_block dsb;
    superblock sb;
    ds_adresa a;

    a.alokacijska_grupa = 0;
    a.poz = 0;

    citaj_sa_diska(a, &dsb);

    memcpy(&sb, dsb, sizeof(superblock));

    if((sb.magic1 == SUPERBLOCK_MAGIC1) && (sb.magic2 == SUPERBLOCK_MAGIC2) && (sb.magic3 == SUPERBLOCK_MAGIC3))
        return true;

    return false;
}

void postavi_adrese(superblock *sb)
{
    unsigned int sbs = sizeof(superblock);
    ds_adresa dsa;
    ds_block dsb;

    dsa.alokacijska_grupa = 0;
    dsa.poz = 0;

    while(sbs > sizeof(ds_block))// broj blokova koje je superblock zauze
    {
        dsa.poz++;
        sbs -= sizeof(ds_block);
    }

    dsa.poz++;

    sb->bmap.dsa = dsa;

    dsa.poz+= sb->alokacijske_grupe;// broj blokova koje je bitmapa zauzela

    while(dsa.poz > (1 << sb->ag_shift))// provjera dali bitmapa prelazi u sljedecu ag
    {
        dsa.alokacijska_grupa++;
        dsa.poz -= (1 << sb->ag_shift);
    }

    sb->slobodni_inode = dsa;

    sbs = ceil(((float)sb->br_blokova / 100) * INODE_POSTO);// sbs postaje velicina inode tablice

    sb->bmap.granica = (sbs * sizeof(ds_block) * 8);

    dsa.poz += sbs;

    while(dsa.poz > (1 << sb->ag_shift))// provjera dali inode tablica prelazi ag
    {
        dsa.alokacijska_grupa++;
        dsa.poz -= (1 << sb->ag_shift);
    }

    sb->slobodni_prostor = dsa;
}
