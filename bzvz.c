#ifndef BZVZ_H_INCLUDED
#define BZVZ_H_INCLUDED
#include "SuperBlock.h"
#include <stdio.h>
int bzvzx(superblock *sb)
{
    inode k;
    user n_usr;
    ds_block dsb;
    char c[MAX_CHAR_LENGTH];
    unsigned int br_k;
    unsigned int br_blokova;
    unsigned int k_blok;

    citaj_sa_diska((sb->bmap.inode_start + 1), &dsb);
    memcpy(&k, dsb, sizeof(inode));

    br_blokova = ceil(((float)k.tok_podataka.velicina)/sizeof(ds_block));// broj blokova koji je fajl zauze
    br_k = (k.tok_podataka.velicina/sizeof(user));//    broj korisnika
    k_blok = ((sizeof(ds_block))/(sizeof(user)));//     broj korisnika po bloku

    printf("Unesite ime novog korisnika: ");

    fgets(c, sizeof(c), stdin);
    c[strlen(c)-1] = '\0';

    strcpy(n_usr.u.ime, c);
    n_usr.u.id = (sb->usr_id + 1);
    sb->usr_id++;

    printf("Dali se želite pridružit postojećoj grupi?  [d] - DA, [n] - NE\n: ");
    fgets(c, sizeof(c), stdin);
    c[strlen(c)-1] = '\0';

    while(true)
    {
        if(strcmp(c, "d\0") == 0)
        {
            unsigned int i, j;
            sve_g *sg = NULL;
            grp g;
            user *uub;//    useri u bloku

            for(i = 0; i < br_blokova; i++)
            {
                citaj_sa_diska(inode_block(&k, i), &dsb);

                if(br_k < k_blok)
                {
                    uub = (user*)malloc(br_k * sizeof(user));
                    if(uub)
                    {
                        memcpy(uub, dsb, (br_k * sizeof(user)));
                        for(j = 0; j < br_k; j++)
                        {
                            printf("%d\n", uub[j].g.id);
                            g.id = uub[j].g.id;
                            strcpy(g.ime, uub[j].g.ime);
                            if(dodaj_g(&sg, &g) < 0)
                            {
                                free(uub);
                                return -1;
                            }
                        }
                    }
                    else
                        return -1;

                    free(uub);
                }
                else
                {
                    uub = (user*)malloc(k_blok * sizeof(user));
                    memcpy(uub, dsb, (k_blok * sizeof(user)));
                    for(j = 0; j < br_k; j++)
                    {
                        g.id = uub[j].g.id;
                        strcpy(g.ime, uub[j].g.ime);
                        if(dodaj_g(&sg, &g) < 0)
                        {
                            free(uub);
                            return -1;
                        }
                    }
                    free(uub);
                    br_k -= k_blok;
                }
            }

            printaj_sg(sg);
            printf("Unesite ID grupe: ");
            fgets(c, sizeof(c), stdin);
            c[strlen(c)-1] = '\0';
            unsigned int izbor = strtol(c, NULL, 10);

            while(dohvati_sgime(&izbor, &n_usr, sg) < 0)
            {
                printf("Nevažeći unos...Pokušajte ponovno...\n: ");
                fgets(c, sizeof(c), stdin);
                c[strlen(c)-1] = '\0';
                izbor = strtol(c, NULL, 10);
            }
            oslobodi_g(sg);
            break;
        }
        else if(strcmp(c, "n\0") == 0)
        {
            printf("Unesite ime nove grupe...\n: ");
            fgets(c, sizeof(c), stdin);
            c[strlen(c)-1] = '\0';

            strcpy(n_usr.g.ime, c);
            n_usr.g.id = (sb->grp_id + 1);
            sb->grp_id++;
            break;
        }
        else
        {
            printf("Nevažeći unos...  [d] - DA, [n] - NE\n: ");
            fgets(c, sizeof(c), stdin);
            c[strlen(c)-1] = '\0';
        }
    }

    br_k = (k.tok_podataka.velicina/sizeof(user));

    if((br_k + 1) <= (k_blok * br_blokova))
    {
        citaj_sa_diska(inode_block(&k, (br_blokova - 1)), &dsb);
        memcpy(dsb + (br_k * sizeof(user)), &n_usr, sizeof(user));
        pisi_na_disk(inode_block(&k, (br_blokova - 1)), &dsb);//    pisanje podataka

        k.tok_podataka.velicina = ((br_k + 1) * sizeof(user));
        memcpy(dsb, &k, sizeof(inode));
        pisi_na_disk((sb->bmap.inode_start + 1), &dsb);//   pisanje novo stanje inode
        memcpy(dsb, sb, sizeof(superblock));
        pisi_na_disk(0, &dsb);//    pisanje novo stanje superbloka

        return 1;
    }

    else////////////////////////////////////////////////////////////ZA DOVRSIT
    {

        ds_adresa dsa, i;

        for(i = sb->bmap.dsa; i < sb->bmap.inode_start; i++)
        {
            citaj_sa_diska(i, &dsb);
            if((dsa = slobodni_prostor(&sb->bmap, &dsb, &i)) != 0)
            {
                if(inode_postavi_a(&k, dsa, &sb->bmap) > 0)
                {
                    sb->bmap.obradi(&(sb->bmap), zauzmi_bit, &dsb, &i);
                    pisi_na_disk(i, &dsb);

                    nulblock(&dsb);
                    memcpy(dsb, &n_usr, sizeof(user));
                    pisi_na_disk(dsa, &dsb);

                    k.tok_podataka.velicina = ((br_k + 1) * sizeof(user));
                    memcpy(dsb, &k, sizeof(inode));
                    pisi_na_disk((sb->bmap.inode_start + 1), &dsb);//   pisanje novo stanje inode

                    memcpy(dsb, sb, sizeof(superblock));
                    pisi_na_disk(0, &dsb);//    pisanje novo stanje superbloka

                    k.tok_podataka.velicina = ((br_k + 1) * sizeof(user));
                    return 1;
                }
            /*
                ds_block dsbb;
                nulblock(&dsbb);
                memcpy(dsbb, &n_usr, sizeof(user));
                pisi_na_disk(dsa, &dsbb);

                */
            }
        }

        printf("Nema više prostora...\n");
        return -3;
    }
}
#endif // BZVZ_H_INCLUDED
