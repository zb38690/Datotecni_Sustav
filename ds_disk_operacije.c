#include "ds_disk_operacije.h"

long dohvati_adresu(ds_adresa a)
{
    return (a.alokacijska_grupa * (sizeof(ds_block)*8)) + a.poz;
}

long udaljenost(long adresa, long poz)
{
    return (adresa - poz);
}

void pisi_na_disk(ds_adresa a, ds_block *dsb)
{
    long odrediste = udaljenost(dohvati_adresu(a), pozicija());
    int i;
    disk_blok db[SEKTOR_MULTIPLIER];

    memcpy(db, dsb, sizeof(ds_block));

    pisi(odrediste, db[0]);
    for(i = 1; i < SEKTOR_MULTIPLIER; i++)
    {
        pisi(0, db[i]);
    }
}

void citaj_sa_diska(ds_adresa a, ds_block *dsb)
{
    long izvor = udaljenost(dohvati_adresu(a), pozicija());
    int i;
    disk_blok db[SEKTOR_MULTIPLIER];

    citaj(izvor, &db[0]);

    for(i = 1; i < SEKTOR_MULTIPLIER; i++)
    {
        citaj(0, &db[i]);
        //memcpy(db[i], citaj(izvor), sizeof(db[i]));
    }

    memcpy(dsb, &db, sizeof(ds_block));

}
