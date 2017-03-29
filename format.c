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
    //sb.koristeni_blokovi;
    sb.magic2 = SUPERBLOCK_MAGIC2;
    sb.alokacijske_grupe = ceil(bitmap/sizeof(ds_block));
    sb.ag_shift = log2(sizeof(ds_block) * 8);
    sb.slobodni_inode = inode_adresa(sb.alokacijske_grupe, sb.ag_shift);
    sb.slobodni_prostor = prostor_adresa(sb.br_blokova, sb.ag_shift, sb.slobodni_inode);

    sb.root_direktorij = sb.slobodni_prostor;// <-----------

    sb.koristeni_blokovi = sb.root_direktorij.poz * (sb.root_direktorij.alokacijska_grupa + 1);

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

ds_adresa inode_adresa(unsigned int alokacijske_grupe, unsigned int ag_shift)
{
    unsigned int cnt = 1;
    unsigned int sb = sizeof(superblock);
    ds_adresa dsa;

    dsa.alokacijska_grupa = 0;

    while(sb > sizeof(ds_block))
    {
        cnt++;
        sb -= sizeof(ds_block);
    }

    cnt+= alokacijske_grupe;

    while(cnt > (1 << ag_shift))
    {
        dsa.alokacijska_grupa++;
        cnt -= (1 << ag_shift);
    }

    dsa.poz = cnt;

    return dsa;
}

ds_adresa prostor_adresa(unsigned int br_blokova, unsigned int ag_shift, ds_adresa slobodni_inode)
{
    unsigned int inodovi = ceil(((float)br_blokova / 100) * INODE_POSTO);
    ds_adresa dsa;
    dsa.alokacijska_grupa = slobodni_inode.alokacijska_grupa;
    dsa.poz = slobodni_inode.poz + inodovi;

    while(dsa.poz > (1 << ag_shift))
    {
        dsa.alokacijska_grupa++;
        dsa.poz -= (1 << ag_shift);
    }

    return dsa;
}
