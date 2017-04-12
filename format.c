#include "format.h"

void format()
{
    superblock sb;
    ds_adresa dsa;
    ds_block dsb;


    char c[29] = "Jednostavni Datotecni Sustav\0";

    strcpy(sb.ds_ime, c);
    sb.magic1 = SUPERBLOCK_MAGIC1;
    sb.velicina_bloka = sizeof(ds_block);
    sb.blok_shift = log2(sizeof(ds_block));
    sb.br_blokova = velicina()/sb.velicina_bloka;
    //sb.koristeni_blokovi = 123456789; <---------------------------------
    sb.magic2 = SUPERBLOCK_MAGIC2;
    postavi_adrese(&sb);
    postavi_root(&sb);
    //sb.root_direktorij = sb.slobodni_prostor;// <-----------

    sb.magic3 = SUPERBLOCK_MAGIC3;

    dsa = 0;

    memcpy(dsb, &sb, sizeof(superblock));

    pisi_na_disk(dsa, &dsb);
}

bool formatiran()
{
    ds_block dsb;
    superblock sb;
    ds_adresa a;

    a = 0;

    citaj_sa_diska(a, &dsb);

    memcpy(&sb, dsb, sizeof(superblock));

    if((sb.magic1 == SUPERBLOCK_MAGIC1) && (sb.magic2 == SUPERBLOCK_MAGIC2) && (sb.magic3 == SUPERBLOCK_MAGIC3))
        return true;

    return false;
}

void postavi_adrese(superblock *sb)
{
    unsigned int sbs = sizeof(superblock);
    float bmap = ((float)(velicina())/(sizeof(ds_block)*8));
    ds_adresa dsa;
    ds_block dsb;

    dsa = 0;

    while(sbs > sizeof(ds_block))// broj blokova koje je superblock zauze
    {
        dsa++;
        sbs -= sizeof(ds_block);
    }

    dsa++;

    sb->bmap.dsa = dsa;

    dsa += ceil(bmap/sizeof(ds_block));// broj blokova koje je bitmapa zauzela

    sb->slobodni_inode = dsa;

    sbs = ceil(((float)sb->br_blokova / 100) * INODE_POSTO);// sbs postaje velicina inode tablice

    sb->bmap.granica = (sbs * sizeof(ds_block) * 8);

    dsa += sbs;

    sb->slobodni_prostor = dsa;
}

void postavi_root(superblock *sb)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    //printf("%d.%d.%d %d:%d:%d\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    inode root;
    inode users;
    user su;
    dir d;
    dir_ele usr;
    ds_adresa dsa;
    ds_block dsb;
    byte *data;
    char ime[5] = {'r','o','o','t','\0'};

    strcpy(su.u.ime, ime);
    su.u.id = 0;
    strcpy(su.g.ime, ime);
    su.g.id = 0;

    postavi_vrimes(&root);

    root.magic = INODE_MAGIC;
    root.korisnik_id = su.u.id;
    root.grupa_id = su.g.id;
    root.mode[0] = 31;//    BIN 0001 1111
    root.mode[1] = 22;//    BIN 0001 0110
    root.inode_br = 0;
    root.roditelj = 0;
    root.tok_podataka.velicina = (sizeof(dir) + sizeof(dir_ele));

    d.br_roditelj = root.inode_br;
    strcpy(d.ime_roditelj, ime);
    d.br_tren = d.br_roditelj;
    strcpy(d.ime_tren, d.ime_roditelj);

    root.tok_podataka.direktni[0] = sb->slobodni_prostor + 1;

    sb->root_direktorij = root;

    postavi_vrimes(&users);

    users.magic = INODE_MAGIC;
    users.korisnik_id = su.u.id;
    users.grupa_id = su.g.id;
    users.mode[0] = 30;//    BIN 0001 1111
    users.mode[1] = 22;//    BIN 0001 0110
    users.inode_br = 1;
    users.roditelj = root.inode_br;
    users.tok_podataka.direktni[0] = sb->slobodni_prostor;
    users.tok_podataka.velicina = sizeof(user);

    usr.br_inode = users.inode_br;
    strcpy(usr.ime_inode, "users\0");
    usr.next = NULL;
    d.head = &usr;

    memcpy(dsb, &root, sizeof(inode));//            pohranjivanje root inode
    pisi_na_disk(sb->slobodni_inode, &dsb);

    //                                              pohranjivanje root direktorija

    data = (byte*)malloc(root.tok_podataka.velicina);
    memcpy(data, &d, sizeof(dir));
    memcpy(data + sizeof(dir), &usr, sizeof(dir_ele));
    memcpy(dsb, data, root.tok_podataka.velicina);
    pisi_na_disk(root.tok_podataka.direktni[0], &dsb);
    free(data);


    dsa = sb->slobodni_inode +1;
    memcpy(dsb, &users, sizeof(inode));
    pisi_na_disk(dsa, &dsb);
}
