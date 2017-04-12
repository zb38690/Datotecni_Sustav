#include "ds_operacije.h"

void nulblock(ds_block *dsb)
{
    int i;
    ds_block x;

    for(i = 0; i < sizeof(ds_block); i++)
        x[i] = 0;

    memcpy(dsb, x, sizeof(ds_block));
}

void pohrani_dir(inode *n, dir *d)
{
    ds_block dsb;
    unsigned long v = sizeof(dir);
    unsigned short cnt = 0;


    dir_ele *ptr = d->head;


    while(ptr != NULL)
    {
        v += sizeof(dir_ele);

        if(v > sizeof(ds_block))
        {

            v -= sizeof(ds_block);
            // ODE TREBA FIND NEW ADDRESS !!!!!!!!!!
            pisi_na_disk(n->tok_podataka.direktni[cnt], &dsb);
            cnt++;

        }
        //dsb |= ptr;
       // ptr = ptr->next;


    }

}
