#include "inode.h"

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
            node->mode[0] |= 1; //  --> BIN 0000 0001
        else
            node->mode[0] &= 254;// --> BIN 1111 1110
}

void postavi_mod_vc(inode *node, bool owner_r)
{
    if(owner_r)
        node->mode[0] |= 2;//   --> BIN 0000 0010
    else
        node->mode[0] &= 253;//   --> BIN 1111 1101
}

void postavi_mod_vp(inode *node, bool owner_w)
{
    if(owner_w)
        node->mode[0] |= 4;//   --> BIN 0000 0100
    else
        node->mode[0] &= 251;// --> BIN 1111 1011
}

void postavi_mod_vi(inode *node, bool owner_x)
{
    if(owner_x)
        node->mode[0] |= 8;//   --> BIN 0000 1000
    else
        node->mode[0] &= 247;// --> BIN 1111 0111
}

void postavi_mod_gc(inode *node, bool group_r)
{
    if(group_r)
        node->mode[0] |= 16;//  BIN 0001 0000
    else
        node->mode[0] &= 239;// BIN 1110 1111
}

void postavi_mod_gp(inode *node, bool group_w)
{
    if(group_w)
        node->mode[1] |= 1;//   BIN 0000 0001
    else
        node->mode[1] &= 254;// BIN 1111 1110
}

void postavi_mod_gi(inode *node, bool group_x)
{
    if(group_x)
        node->mode[1] |= 2;//   BIN 0000 0010
    else
        node->mode[1] &= 253;// BIN 1111 1101
}

void postavi_mod_oc(inode *node, bool everyone_r)
{
    if(everyone_r)
        node->mode[1] |= 4;//   BIN 0000 0100
    else
        node->mode[1] &= 251;// BIN 1111 1011
}

void postavi_mod_op(inode *node, bool everyone_w)
{
    if(everyone_w)
        node->mode[1] |= 8;//   BIN 0000 1000
    else
        node->mode[1] &= 247;// BIN 1111 0111
}

void postavi_mod_oi(inode *node, bool everyone_x)
{
    if(everyone_x)
        node->mode[1] |= 16;//  BIN 0001 0000
    else
        node->mode[1] &= 239;// BIN 1110 1111
}
