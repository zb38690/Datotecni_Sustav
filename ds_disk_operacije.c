#include "ds_disk_operacije.h"

adresa dohvati_adresu(ds_adresa a)
{
    return (adresa)(a.alokacijska_grupa * a.poz);
}

void pisi_na_disk(ds_adresa a, ds_block b)
{
    adresa x = dohvati_adresu(a);
    long long dest = pozicija();

    if(x < dest)
    {
        int i;
        disk_block db;
        for(i = 0; i < (sizeof(ds_block) / sizeof(db); i++))
        {

        }
    }
}
