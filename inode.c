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
    node->tok_podataka.indirektni = 0;
    node->tok_podataka.d_indirektni = 0;
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

void printaj_mod(inode *node)
{
    short i;

    for(i = 0; i < 10; i++)
    {
        if(node->mode & (int)pow(2, i))
        {
            if(i == 0)
                printf("d");
            else if((i == 1) || (i == 4) || (i == 7))
                printf("r");
            else if((i == 2) || (i == 5) || (i == 8))
                printf("w");
            else
                printf("x");
        }
        else
            printf("-");
    }
    printf(" ");
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
            ucitaj_direktni(podataka, node, br_bloka);
        else if(br_bloka <= (BR_DIREKTNIH + (sizeof(ds_block)/sizeof(ds_adresa))))
            ucitaj_indirektni(podataka, node, br_bloka);
        else
            ucitaj_dindirektni(podataka, node, br_bloka);
    }

    return podataka;
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
        memcpy(dsa_i+i, dsb + (i * sizeof(ds_adresa)), sizeof(ds_adresa));

    k = br_indirektni;
    for(i = 0; ((i < (sizeof(ds_block)/sizeof(ds_adresa))) && (k < br_blokova)); i++)
    {
        citaj_sa_diska(dsa_i[i], &dsb);
        for(k, j = 0; ((k < br_blokova) && (j < (sizeof(ds_block)/sizeof(ds_adresa)))); j++, k++)
            memcpy(dsa + k, dsb + (j * sizeof(ds_adresa)), sizeof(ds_adresa));

    }
}

ds_adresa inode_block(inode *node, unsigned int i_bloka)
{
    if(i_bloka < BR_DIREKTNIH)
        return node->tok_podataka.direktni[(i_bloka)];
    else if(i_bloka < (BR_DIREKTNIH + (sizeof(ds_block)/sizeof(ds_adresa))))
    {
        ds_block dsb;
        ds_adresa dsa[(sizeof(ds_block)/sizeof(ds_adresa))];
        citaj_sa_diska(node->tok_podataka.indirektni, &dsb);
        memcpy(&dsa, dsb, sizeof(dsa));
        return dsa[((i_bloka) - BR_DIREKTNIH)];
    }
    else if(i_bloka < ((BR_DIREKTNIH + (sizeof(ds_block)/sizeof(ds_adresa))) + ((sizeof(ds_block)/sizeof(ds_adresa)) * (sizeof(ds_block)/sizeof(ds_adresa)))))
    {
        ds_block dsb;
        ds_adresa dsa[(sizeof(ds_block)/sizeof(ds_adresa))];
        unsigned int i;
        citaj_sa_diska(node->tok_podataka.d_indirektni, &dsb);
        memcpy(dsa, dsb, sizeof(dsa));

        i = i_bloka - (BR_DIREKTNIH + (sizeof(ds_block)/sizeof(ds_adresa)));
        if(i < (sizeof(ds_block)/sizeof(ds_adresa)))
        {
            citaj_sa_diska((dsa[0]), &dsb);
            memcpy(dsa, dsb, sizeof(dsa));
            return dsa[i];
        }

        i /= (sizeof(ds_block)/sizeof(ds_adresa));

        citaj_sa_diska((dsa[i]), &dsb);
        memcpy(dsa, dsb, sizeof(dsa));

        i *= (sizeof(ds_block)/sizeof(ds_adresa));
        i = ((i_bloka - (BR_DIREKTNIH + (sizeof(ds_block)/sizeof(ds_adresa)))) - i);

        return dsa[i];
    }
}

int inode_postavi_a(inode *node, ds_adresa nova, bitmap *bmap)
{
    unsigned int br_blokova = ceil(((float)node->tok_podataka.velicina)/sizeof(ds_block));// broj blokova koliko pokazuje inode

    if(br_blokova < BR_DIREKTNIH)
    {
        node->tok_podataka.direktni[br_blokova] = nova;
        return 1;
    }
    else if(br_blokova < (BR_DIREKTNIH + (sizeof(ds_block)/sizeof(ds_adresa))))
    {
        ds_block dsb;
        ds_adresa dsa;
        if(br_blokova == BR_DIREKTNIH)
        {
            ds_adresa dsab;// dsab = ds_adresa bitmape
            ds_block dsbb;      // dsbb = ds_block bitmape
            dsab = bmap->dsa;

            dsab = bmap->dsa + (nova/(sizeof(ds_block) * 8));
            citaj_sa_diska(dsab, &dsbb);
            bmap->obradi(bmap, zauzmi_bit, &dsbb, &nova);
            dsa = slobodni_prostor(bmap, &dsbb, &dsab);
            dsab++;

            while((dsa < bmap->prostor_start) && (dsab < bmap->inode_start))
            {
                citaj_sa_diska(dsab, &dsb);
                dsa = slobodni_prostor(bmap, &dsb, &dsab);
                dsab++;
            }

            if(dsa < bmap->prostor_start)
            {
                return -1;
            }
            else
            {
                node->tok_podataka.indirektni = dsa;
                if(dsab == (bmap->dsa + (nova/(sizeof(ds_block) * 8))))
                {
                    bmap->obradi(bmap, zauzmi_bit, &dsbb, dsa);
                    pisi_na_disk(dsab, &dsbb);
                }
                else
                {
                    bmap->obradi(bmap, zauzmi_bit, &dsb, dsa);
                    pisi_na_disk(dsab, &dsb);
                    dsab = bmap->dsa + (nova/(sizeof(ds_block) * 8));
                    pisi_na_disk(dsab, &dsbb);

                }
                nulblock(&dsb);
                memcpy(dsb, &nova, sizeof(ds_adresa));
                pisi_na_disk(dsa, &dsb);
            }
            return 1;
        }
        else
        {
            citaj_sa_diska(node->tok_podataka.indirektni, &dsb);
            memcpy(dsa + ((br_blokova - BR_DIREKTNIH) * sizeof(ds_adresa)), &nova, sizeof(ds_adresa));
            pisi_na_disk(node->tok_podataka.indirektni, &dsb);
            return 1;
        }

    }
    else if(br_blokova < (((BR_DIREKTNIH + (sizeof(ds_block)/sizeof(ds_adresa))) + ((sizeof(ds_block)/sizeof(ds_adresa)) * (sizeof(ds_block)/sizeof(ds_adresa))))))
    {
        if(br_blokova == (BR_DIREKTNIH + (sizeof(ds_block)/sizeof(ds_adresa))))
        {
            ds_block dsb;
            ds_adresa aa, ab, dsa;// A - d_indirektni, B - indirektni

            dsa = bmap_bloka(bmap, &nova);
            citaj_sa_diska(dsa, &dsb);
            bmap->obradi(bmap, zauzmi_bit, &dsb, &nova);
            pisi_na_disk(dsa, &dsb);

            dsa = bmap->dsa;
            citaj_sa_diska(dsa, &dsb);


            aa = slobodni_prostor(bmap, &dsb, &dsa);
            dsa++;

            while((aa < bmap->prostor_start) && (dsa < bmap->inode_start))// trazi adresu za A
            {
                citaj_sa_diska(dsa, &dsb);
                aa = slobodni_prostor(bmap, &dsb, &dsa);
                dsa++;
            }

            if(aa < bmap->prostor_start)
            {
                dsa = bmap_bloka(bmap, &nova);
                citaj_sa_diska(dsa, &dsb);
                bmap->obradi(bmap, oslobodi_bit, &dsb, &nova);
                pisi_na_disk(dsa, &dsb);
                return -1;
            }
            dsa--;
            bmap->obradi(bmap, zauzmi_bit, &dsb, &aa);
            pisi_na_disk(dsa, &dsb);

            ab = slobodni_prostor(bmap, &dsb, &dsa);
            dsa++;

            while((ab < bmap->prostor_start) && (dsa < bmap->inode_start))// trazi adresu za B
            {
                citaj_sa_diska(dsa, &dsb);
                ab = slobodni_prostor(bmap, &dsb, &dsa);
                dsa++;
            }
            if(ab < bmap->prostor_start)
            {
                dsa = bmap_bloka(bmap, &nova);
                citaj_sa_diska(dsa, &dsb);
                bmap->obradi(bmap, oslobodi_bit, &dsb, &nova);
                pisi_na_disk(dsa, &dsb);

                dsa = bmap_bloka(bmap, &aa);
                citaj_sa_diska(dsa, &dsb);
                bmap->obradi(bmap, oslobodi_bit, &dsb, &aa);
                pisi_na_disk(dsa, &dsb);
                return -1;
            }

            dsa--;
            bmap->obradi(bmap, zauzmi_bit, &dsb, &ab);
            pisi_na_disk(dsa, &dsb);


            node->tok_podataka.d_indirektni = aa;

            nulblock(&dsb);
            memcpy(dsb, &ab, sizeof(ds_adresa));
            pisi_na_disk(aa, &dsb);

            nulblock(&dsb);
            memcpy(dsb, &nova, sizeof(ds_adresa));
            pisi_na_disk(ab, &dsb);
            return 1;
        }
        else
        {
            br_blokova -= (BR_DIREKTNIH + (sizeof(ds_block)/sizeof(ds_adresa)));
            ds_block dsb;
            ds_adresa dsa;
            unsigned int dii = (br_blokova/(sizeof(ds_block)/sizeof(ds_adresa)));// double indirekt index
            unsigned int ii = br_blokova%(sizeof(ds_block)/sizeof(ds_adresa));//    indirekt index

            dsa = bmap_bloka(bmap, &nova);
            citaj_sa_diska(dsa, &dsb);
            bmap->obradi(bmap, zauzmi_bit, &dsb, &nova);
            pisi_na_disk(dsa, &dsb);

            if(ii == 0)
            {
                ds_adresa ai;// adresa indirektnog bloka
                dsa = bmap->dsa;
                citaj_sa_diska(dsa, &dsb);

                ai = slobodni_prostor(bmap, &dsb, &dsa);
                dsa++;

                while((ai < bmap->prostor_start) && (dsa < bmap->inode_start))
                {
                    citaj_sa_diska(dsa, &dsb);
                    ai = slobodni_prostor(bmap, &dsb, &dsa);
                    dsa++;
                }

                if(ai < bmap->prostor_start)
                {
                    dsa = bmap_bloka(bmap, &nova);
                    citaj_sa_diska(dsa, &dsb);
                    bmap->obradi(bmap, oslobodi_bit, &dsb, &nova);
                    pisi_na_disk(dsa, &dsb);
                    return -1;
                }

                dsa--;
                bmap->obradi(bmap, zauzmi_bit, &dsb, &ai);
                pisi_na_disk(dsa, &dsb);

                citaj_sa_diska(node->tok_podataka.d_indirektni, &dsb);
                memcpy(dsa + (dii*sizeof(ds_adresa)), &ai, sizeof(ds_adresa));
                pisi_na_disk(node->tok_podataka.d_indirektni, &dsb);

                nulblock(&dsb);
                memcpy(dsb, &nova, sizeof(ds_adresa));
                pisi_na_disk(ai, &dsb);

                return 1;
            }

            else
            {
                citaj_sa_diska(node->tok_podataka.d_indirektni, &dsb);
                memcpy(&dsa, dsb + (dii * (sizeof(ds_adresa))), sizeof(ds_adresa));
                citaj_sa_diska(dsa, &dsb);
                mempcpy(dsb + (ii * (sizeof(ds_adresa))), &nova, sizeof(ds_adresa));
                pisi_na_disk(dsa, &dsb);
            }

        }

    }
}
