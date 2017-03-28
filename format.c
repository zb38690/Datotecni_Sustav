#include "format.h"

void format()
{
    superblock sb;
    ds_adresa dsa;
    ds_block dsb;
    unsigned float bitmap = ((velicina())/(sizeof(ds_block)*8));

    char c[29] = "Jednostavni Datotecni Sustav\0";

    strcpy(sb.ds_ime, c);
    sb.magic1 = SUPERBLOCK_MAGIC1;
    sb.velicina_bloka = sizeof(ds_block);
    sb.blok_shift = log2(sizeof(ds_block));
    sb.br_blokova = velicina()/sb.velicina_bloka;
    sb.koristeni_blokovi = 1;
    sb.magic2 = SUPERBLOCK_MAGIC2;
    sb.alokacijske_grupe = ceil(bitmap/sizeof(ds_block));
    sb.ag_shift = 000000000000000000000000000000000000;
    ds_adresa slobodni_inode;
    ds_adresa slobodni_prostor;
    ds_adresa root_direktorij;
    sb.magic3 = SUPERBLOCK_MAGIC3;



    sb.koristeni_blokovi = 0;

    dsa.alokacijska_grupa = 0;
    dsa.poz = 0;

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
