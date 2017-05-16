#include "bitmap.h"

void init_bmap(bitmap *self)
{
    self->obradi = obradi_bmapu;
}

ds_adresa slobodni_inode(const bitmap *bmap, ds_block *dsb, const ds_adresa *dsa)// bitmapa, block iz bitmape, adresa blocka
{

    ds_block maska;// maska ce imat 1 na mjestima slobodnog prostora
    oneblock(&maska);
    int a;

    if((((*dsa) - bmap->dsa)* sizeof(ds_block) * 8) < bmap->inode_start)// dali je pozicija inode tablice > pod poozicije pocetnog bloka
    {
        unsigned int br_bajta = bmap->inode_start / 8;
        unsigned int poz_ubajtu = bmap->inode_start - (br_bajta * 8);
        unsigned int i;
        byte msk;

        for(i = 0; i < br_bajta; i++)
            (*dsb)[i] = maska[i] = 0;// postavljanje 0 (u bajtove) prije pocetka inode tablice

        msk = (pow(2, poz_ubajtu) - 1);
        msk ^= 255;

        (*dsb)[i] &=  maska[i] &= msk;// postavljanje 0 (u bitove) prije pocetka inode tablice

    }

    if(((((*dsa) - bmap->dsa) * sizeof(ds_block) * 8) + ((sizeof(ds_block)) - 1)) > bmap->prostor_start)// dali je krajni blok > pocetka slobodnog prostora
    {
        unsigned int br_bajta = bmap->prostor_start / 8;
        unsigned int poz_ubajtu = bmap->prostor_start - (br_bajta * 8);
        unsigned int i = br_bajta;
        byte msk;

        msk = (pow(2, poz_ubajtu) - 1);

        (*dsb)[i] &= maska[i] &= msk;// postavlja 0 (u bitovima) nakon kraja inode tablice

        for(++i; i < sizeof(ds_block); i++)
            (*dsb)[i] = maska[i] = 0;// postavlja 0 (u bajtovima) nakon kraja inode tablice
    }

    for(a = 0; a < sizeof(ds_block); a++)
        maska[a] ^= (*dsb)[a];// maska i dsb imaju 0 izvan inode tablice. U prostoru inode tablice maska ima 1, dsb svasta

    for(a = 0; a < sizeof(ds_block); a++)
        if(maska[a] != 0)
        {
            short poz = 0;

            while(((short)pow(2, poz) & maska[a]) == 0)
                poz++;

            return (((a*8) + poz) + (sizeof(ds_block) * 8 * ((*dsa) - bmap->dsa)));
        }

    return 0;
}

ds_adresa slobodni_prostor(const bitmap *bmap, ds_block *dsb, const ds_adresa *dsa)
{
    ds_block maska;
    oneblock(&maska);
    int a;

    if((((*dsa) - bmap->dsa)* sizeof(ds_block) * 8) < bmap->prostor_start)
    {
        unsigned int br_bajta = (bmap->prostor_start - (((*dsa) - bmap->dsa)* sizeof(ds_block) * 8)) / 8;
        unsigned int poz_ubajtu = ((bmap->prostor_start - (((*dsa) - bmap->dsa)* sizeof(ds_block) * 8)) * 8) - (br_bajta * 8);
        unsigned int i;
        byte msk;

        for(i = 0; i < br_bajta; i++)
            (*dsb)[i] = maska[i] = 0;

        msk = (pow(2, poz_ubajtu) - 1);
        msk ^= 255;

        (*dsb)[i] &=  maska[i] &= msk;
    }

    if(((((*dsa) - bmap->dsa) * sizeof(ds_block) * 8) + ((sizeof(ds_block) * 8) - 1)) > bmap->prostor_stop)
    {
        unsigned int br_bajta = (bmap->prostor_stop - (((*dsa) - bmap->dsa) * sizeof(ds_block) * 8)) / 8;
        unsigned int poz_ubajtu = (((*dsa) - bmap->dsa) * sizeof(ds_block) * 8) - (br_bajta * 8);
        unsigned int i = br_bajta;
        byte msk;

        msk = (pow(2, poz_ubajtu) - 1);

        (*dsb)[i] &= maska[i] &= msk;

        for(++i; i < sizeof(ds_block); i++)
            (*dsb)[i] = maska[i] = 0;
    }

    for(a = 0; a < sizeof(ds_block); a++)
        maska[a] ^= (*dsb)[a];

    for(a = 0; a < sizeof(ds_block); a++)
        if(maska[a] != 0)
        {
            short poz = 0;

            while(((int)pow(2, poz) & maska[a]) == 0)
                poz++;

            return (((a*8) + poz) + (sizeof(ds_block) * 8 * ((*dsa) - bmap->dsa)));
        }

    return 0;
}

static void obradi_bmapu(bitmap *bmap, void(*fn_pntr)(byte *msk, byte *dsb_b), ds_block *dsb, const ds_adresa *dsa)
{
    if(((*dsa) >= bmap->inode_start) && ((*dsa) < bmap->prostor_stop))
    {
        unsigned int br_bajta = (*dsa) / 8;
        unsigned int poz_ubajtu = (*dsa) - (br_bajta * 8);
        byte b = (short)pow(2, poz_ubajtu);


        while(br_bajta >= sizeof(ds_block))
            br_bajta -= sizeof(ds_block);

        (*fn_pntr)(&b, &((*dsb)[br_bajta]));
    }
}

void zauzmi_bit(byte *msk, byte *dsb_b)
{
    (*dsb_b) |= (*msk);
}

void oslobodi_bit(byte *msk, byte *dsb_b)
{
    (*msk) ^= 255;
    (*dsb_b) &= (*msk);
}

ds_adresa bmap_bloka(const bitmap *bmap, const ds_adresa *dsa)
{
    if(((*dsa) > bmap->dsa) && ((*dsa) < bmap->prostor_stop))
            return bmap->dsa + ((*dsa) / (sizeof(ds_block) * 8));
    return 0;
}
