#include "bitmap.h"

void nulblock(ds_block *dsb)
{
    int i;
    ds_block x;

    for(i = 0; i < sizeof(ds_block); i++)
        x[i] = 0;

    memcpy(dsb, x, sizeof(ds_block));
}
