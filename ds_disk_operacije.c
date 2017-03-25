#include "ds_disk_operacije.h"

long dohvati_adresu(ds_adresa a)
{
    return (a.alokacijska_grupa * a.poz);
}

long udaljenost(long adresa, long poz)
{
    return (adresa - poz);
}

void pisi_na_disk(ds_adresa a, ds_block dsb)
{
    long adresa = dohvati_adresu(a);
    long poz = pozicija();
    long odrediste = udaljenost(adresa, poz);
    int i;
    disk_blok db[SEKTOR_MULTIPLIER];

    memcpy(db, dsb, sizeof(dsb));

    pisi(odrediste, db[0]);
    for(i = 1; i < SEKTOR_MULTIPLIER; i++)
    {
        pisi(0, db[i]);
    }
}

ds_block *citaj_sa_diska(ds_adresa a)
{
    long adresa = dohvati_adresu(a);
    long poz = pozicija();
    long izvor = udaljenost(adresa, poz);
    int i;
    disk_blok db[SEKTOR_MULTIPLIER];
    ds_block x;

    db[0] = citaj(izvor);

    for(i = 1; i < SEKTOR_MULTIPLIER; i++)
    {
        db[i] = citaj(0);
    }

    memcpy(x, db, sizeof(x));

    return x;
}
