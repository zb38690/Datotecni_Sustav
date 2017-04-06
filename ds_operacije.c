#include "ds_operacije.h"

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
