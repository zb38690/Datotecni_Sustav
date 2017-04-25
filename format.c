#include "format.h"

void format()
{
    superblock sb;
    ds_adresa dsa;
    ds_block dsb;


    char c[MAX_CHAR_LENGTH] = "Jednostavni Datotecni Sustav\0";

    strcpy(sb.ds_ime, c);
    sb.magic1 = SUPERBLOCK_MAGIC1;
    sb.velicina_bloka = sizeof(ds_block);
    sb.blok_shift = log2(sizeof(ds_block));
    sb.br_blokova = velicina()/sb.velicina_bloka;
    sb.koristeni_blokovi = 1;
    sb.magic2 = SUPERBLOCK_MAGIC2;
    init_bmap(&(sb.bmap));
    postavi_adrese(&sb);
    postavi_root(&sb);

    sb.magic3 = SUPERBLOCK_MAGIC3;

    dsa = 0;

    memcpy(dsb, &sb, sizeof(superblock));
    pisi_na_disk(dsa, &dsb);
}

bool formatiran()
{
    ds_block dsb;
    superblock sb;

    citaj_sa_diska(0, &dsb);

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
    sb->koristeni_blokovi += dsa;

    sb->max_velicina = sizeof(ds_block) * BR_DIREKTNIH;
    sb->max_velicina += (sizeof(ds_block)/sizeof(ds_adresa)) * sizeof(ds_block);
    sb->max_velicina += (sizeof(ds_block)/sizeof(ds_adresa)) * (sizeof(ds_block)/sizeof(ds_adresa)) * sizeof(ds_block);
//    sb->max_velicina += (sizeof(ds_block)/sizeof(ds_adresa)) * (sizeof(ds_block)/sizeof(ds_adresa)) * (sizeof(ds_block)/sizeof(ds_adresa)) * sizeof(ds_block);
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

    init_inode(&root, &su, 0, 0, je_dir);
    root.tok_podataka.velicina = (sizeof(dir) + sizeof(dir_ele));

    d.br_roditelj = root.inode_br;
    strcpy(d.ime_roditelj, ime);
    d.br_tren = d.br_roditelj;
    strcpy(d.ime_tren, d.ime_roditelj);

    root.tok_podataka.direktni[0] = sb->bmap.prostor_start + 1;

    sb->root_direktorij = root;

    init_inode(&users, &su, 1, 0, je_dat);
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
    sb->koristeni_blokovi++;
//                                                  pohranjivanje korisnikov inode
    dsa = sb->bmap.inode_start + 1;
    memcpy(dsb, &users, sizeof(inode));
    pisi_na_disk(dsa, &dsb);
//                                                  pohranjivanje korisnika
    memcpy(dsb, &su, sizeof(user));
    pisi_na_disk(users.tok_podataka.direktni[0], &dsb);
    sb->koristeni_blokovi++;
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

    sb->usr_id = 0;
    sb->grp_id = 0;
 }
