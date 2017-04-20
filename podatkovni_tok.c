#include "podatkovni_tok.h"

int init_indi(indirektni *indi)
{
    int x = sizeof(ds_block)/sizeof(ds_adresa);
    int i;
    indi->indi_block = (indirektni*)malloc(x);
    if(indi->indi_block)
    {
        for(i = 0; i < x; i++)
            indi->indi_block[i] = NULL;
        return 0;
    }
    return -1;
}

void uinit_indi(indirektni *indi)
{
    free(indi->indi_block);
}
