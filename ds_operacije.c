#include "ds_operacije.h"

void nulblock(ds_block *dsb)
{
    int i;

    for(i = 0; i < sizeof(ds_block); i++)
        (*dsb)[i] = 0;
}

void oneblock(ds_block *dsb)
{
    int i;

    for(i = 0; i < sizeof(ds_block); i++)
        (*dsb)[i] = 255;
}

void dodaj_korisnika(superblock *sb)
{
    inode k;
    user *usr;
    ds_block dsb;
    unsigned int br_k;

    citaj_sa_diska((sb->bmap.inode_start + 1), &dsb);
    memcpy(&k, dsb, sizeof(inode));

    br_k = (k.tok_podataka.velicina/sizeof(user));
}

ds_adresa kreiraj_dir(superblock *sb, user *usr, dir *d)
{
    ds_adresa a_inode = 0;
    ds_adresa a_data = 0;
    ds_adresa a_bmap = sb->bmap.dsa;
    ds_block dsb;

    while(a_bmap < sb->bmap.inode_start)
    {
        if(a_inode == 0)
        {
            citaj_sa_diska(a_bmap, &dsb);
            a_inode = slobodni_inode(&(sb->bmap), &dsb, &a_bmap);
        }
        if(a_data == 0)
        {
            citaj_sa_diska(a_bmap, &dsb);
            a_data = slobodni_prostor(&(sb->bmap), &dsb, &a_bmap);
        }
        if((a_inode != 0) && (a_data != 0))
        {
            inode i_novi;
            dir d_novi;
            dir_op op;
            char ime[MAX_CHAR_LENGTH];
            init_dir_op(&op);
            int x;

            init_inode(&i_novi, usr, (a_inode - sb->bmap.inode_start), d->br_tren, je_dir);
            i_novi.tok_podataka.direktni[0] = a_data;

            printf("Upišite ime nove datoteke: ");
            fgets(ime, sizeof(ime), stdin);

            ime[strlen(ime) - 1] = '\0';

            while((x = op.dodaj(d, i_novi.inode_br, &ime)) == -1)
            {
                printf("Unjeli ste postojeće ime...\nUpišite neko drugo ime: ");
                fgets(ime, sizeof(ime), stdin);
                ime[strlen(ime) - 1] = '\0';
            }

            if(x != 1)
            {
                printf("Neuspješno dodavanje novog direktorija...\n");
                return 0;
            }
            d_novi.br_roditelj = d->br_tren;
            strcpy(d_novi.ime_roditelj, d->ime_tren);
            d_novi.br_tren = i_novi.inode_br;
            strcpy(d_novi.ime_tren, ime);
            d_novi.head = NULL;

            memcpy(dsb, &i_novi, sizeof(inode));
            pisi_na_disk(a_inode, &dsb);

            memcpy(dsb, &d_novi, sizeof(dir));
            pisi_na_disk(a_data, &dsb);

            citaj_sa_diska(a_bmap, &dsb);
            sb->bmap.obradi(&(sb->bmap), zauzmi_bit, &dsb, &a_inode);
            sb->bmap.obradi(&(sb->bmap), zauzmi_bit, &dsb, &a_data);
            pisi_na_disk(a_bmap, &dsb);

            return a_inode;
        }

        a_bmap++;
    }

    return 0;
}

void pohrani_dir(inode *n, dir *d)
{

}
