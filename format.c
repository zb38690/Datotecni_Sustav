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
    init_bmap(&(sb.bmap));
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

    sb->bmap.inode_start = dsa;

    sbs = ceil(((float)sb->br_blokova / 100) * INODE_POSTO);// sbs postaje velicina inode tablice

    dsa += sbs;

    sb->bmap.prostor_start = dsa;
    sb->bmap.prostor_stop = velicina() / sizeof(ds_block);

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
    char ime[5] = {'r','o','o','t','\0'};

    strcpy(su.u.ime, ime);
    su.u.id = 0;
    strcpy(su.g.ime, ime);
    su.g.id = 0;

    postavi_vrimes(&root);

    root.magic = INODE_MAGIC;
    root.korisnik_id = su.u.id;
    root.grupa_id = su.g.id;
    root.mode = 735;//      BIN 101 101 111 1
    root.inode_br = 0;
    root.roditelj = 0;
    root.tok_podataka.velicina = (sizeof(dir) + sizeof(dir_ele));

    d.br_roditelj = root.inode_br;
    strcpy(d.ime_roditelj, ime);
    d.br_tren = d.br_roditelj;
    strcpy(d.ime_tren, d.ime_roditelj);

    root.tok_podataka.direktni[0] = sb->bmap.prostor_start + 1;

    sb->root_direktorij = root;

    postavi_vrimes(&users);

    users.magic = INODE_MAGIC;
    users.korisnik_id = su.u.id;
    users.grupa_id = su.g.id;
    users.mode = 734;//     BIN 101 101 111 0
    users.inode_br = 1;
    users.roditelj = root.inode_br;
    users.tok_podataka.direktni[0] = sb->bmap.prostor_start;
    users.tok_podataka.velicina = sizeof(user);

    usr.br_inode = users.inode_br;
    strcpy(usr.ime_inode, "users\0");
    usr.next = NULL;
    d.head = &usr;

    memcpy(dsb, &root, sizeof(inode));//            pohranjivanje root inode
    pisi_na_disk(sb->bmap.inode_start, &dsb);


    //                                              pohranjivanje root direktorija

    memcpy(dsb, &d, sizeof(dir));
    memcpy(dsb + sizeof(dir), &usr, sizeof(dir_ele));
    pisi_na_disk(root.tok_podataka.direktni[0], &dsb);



    dsa = sb->bmap.inode_start + 1;
    memcpy(dsb, &users, sizeof(inode));
    pisi_na_disk(dsa, &dsb);
//                                                  Oznacavanje zauzetog bita u inode tablici (root)
    dsa = bmap_bloka(&sb->bmap, &sb->bmap.inode_start);
    citaj_sa_diska(dsa, &dsb);
    sb->bmap.obradi(&sb->bmap, zauzmi_bit, &dsb, &sb->bmap.inode_start);
    pisi_na_disk(dsa, &dsb);
//                                                  Oznacavanje zauzetog bita u inode tablici (users)
    dsa = sb->bmap.inode_start + 1;
    citaj_sa_diska(bmap_bloka(&sb->bmap, &dsa), &dsb);
    sb->bmap.obradi(&sb->bmap, zauzmi_bit, &dsb, &dsa);
    pisi_na_disk(bmap_bloka(&sb->bmap, &dsa), &dsb);

//                                                  Oznacavanje zauzetog bita u prostoru (users)
    dsa = sb->bmap.prostor_start;
    citaj_sa_diska(bmap_bloka(&sb->bmap, &users.tok_podataka.direktni[0]), &dsb);
    sb->bmap.obradi(&sb->bmap, zauzmi_bit, &dsb, &dsa);
    pisi_na_disk(bmap_bloka(&sb->bmap, &users.tok_podataka.direktni[0]), &dsb);
//                                                  Oznacavanje zauzetog bita u prostoru (users)
    dsa = bmap_bloka(&sb->bmap, &root.tok_podataka.direktni[0]);
    citaj_sa_diska(dsa, &dsb);
    sb->bmap.obradi(&sb->bmap, zauzmi_bit, &dsb, &root.tok_podataka.direktni[0]);
    pisi_na_disk(dsa, &dsb);
 }
