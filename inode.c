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
