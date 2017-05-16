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

int dodaj_korisnika(superblock *sb)
{
    inode k;
    user n_usr;
    ds_block dsb;
    char c[MAX_CHAR_LENGTH];
    unsigned int br_k;
    unsigned int br_blokova;
    unsigned int k_blok;

    citaj_sa_diska((sb->bmap.inode_start + 1), &dsb);
    memcpy(&k, dsb, sizeof(inode));

    br_blokova = ceil(((float)k.tok_podataka.velicina)/sizeof(ds_block));// broj blokova koji je fajl zauze
    br_k = (k.tok_podataka.velicina/sizeof(user));//    broj korisnika
    k_blok = ((sizeof(ds_block))/(sizeof(user)));//     broj korisnika po bloku

    printf("Unesite ime novog korisnika: ");

    fgets(c, sizeof(c), stdin);
    c[strlen(c)-1] = '\0';

    strcpy(n_usr.u.ime, c);
    n_usr.u.id = (sb->usr_id + 1);
    sb->usr_id++;

    printf("Dali se želite pridružit postojećoj grupi?  [d] - DA, [n] - NE\n: ");
    fgets(c, sizeof(c), stdin);
    c[strlen(c)-1] = '\0';

    while(true)
    {
        if(strcmp(c, "d\0") == 0)
        {
            unsigned int i, j;
            sve_g *sg = NULL;
            grp g;
            user *uub;//    useri u bloku

            for(i = 0; i < br_blokova; i++)
            {
                citaj_sa_diska(inode_block(&k, i), &dsb);

                if(br_k < k_blok)
                {
                    uub = (user*)malloc(br_k * sizeof(user));
                    if(uub)
                    {
                        memcpy(uub, dsb, (br_k * sizeof(user)));
                        for(j = 0; j < br_k; j++)
                        {
                            g.id = uub[j].g.id;
                            strcpy(g.ime, uub[j].g.ime);
                            if(dodaj_g(&sg, &g) < 0)
                            {
                                free(uub);
                                return -1;
                            }
                        }
                    }
                    else
                        return -1;

                    free(uub);
                }
                else
                {
                    uub = (user*)malloc(k_blok * sizeof(user));
                    memcpy(uub, dsb, (k_blok * sizeof(user)));
                    for(j = 0; j < k_blok; j++)
                    {
                        g.id = uub[j].g.id;
                        strcpy(g.ime, uub[j].g.ime);
                        if(dodaj_g(&sg, &g) < 0)
                        {
                            free(uub);
                            return -1;
                        }
                    }
                    free(uub);
                    br_k -= k_blok;
                }
            }

            printaj_sg(sg);
            printf("Unesite ID grupe: ");
            fgets(c, sizeof(c), stdin);
            c[strlen(c)-1] = '\0';
            unsigned int izbor = strtol(c, NULL, 10);

            while(dohvati_sgime(&izbor, &n_usr, sg) < 0)
            {
                printf("Nevažeći unos...Pokušajte ponovno...\n: ");
                fgets(c, sizeof(c), stdin);
                c[strlen(c)-1] = '\0';
                izbor = strtol(c, NULL, 10);
            }
            oslobodi_g(sg);
            break;
        }
        else if(strcmp(c, "n\0") == 0)
        {
            printf("Unesite ime nove grupe...\n: ");
            fgets(c, sizeof(c), stdin);
            c[strlen(c)-1] = '\0';

            strcpy(n_usr.g.ime, c);
            n_usr.g.id = (sb->grp_id + 1);
            sb->grp_id++;
            break;
        }
        else
        {
            printf("Nevažeći unos...  [d] - DA, [n] - NE\n: ");
            fgets(c, sizeof(c), stdin);
            c[strlen(c)-1] = '\0';
        }
    }

    br_k = (k.tok_podataka.velicina/sizeof(user));

    if((br_k + 1) <= (k_blok * br_blokova))
    {
        unsigned int i = k_blok;
        citaj_sa_diska(inode_block(&k, (br_blokova - 1)), &dsb);
        i *= br_blokova;
        i -= br_k;
        i = (k_blok - i);
        memcpy(dsb + (i * sizeof(user)), &n_usr, sizeof(user));
        pisi_na_disk(inode_block(&k, (br_blokova - 1)), &dsb);//    pisanje podataka

        k.tok_podataka.velicina = ((br_k + 1) * sizeof(user));
        postavi_vrimem(&k);

        memcpy(dsb, &k, sizeof(inode));
        pisi_na_disk((sb->bmap.inode_start + 1), &dsb);//   pisanje novo stanje inode
        memcpy(dsb, sb, sizeof(superblock));
        pisi_na_disk(0, &dsb);//    pisanje novo stanje superbloka

        return 1;
    }

    else
    {

        ds_adresa dsa, i;

        for(i = sb->bmap.dsa; i < sb->bmap.inode_start; i++)
        {
            citaj_sa_diska(i, &dsb);
            if((dsa = slobodni_prostor(&sb->bmap, &dsb, &i)) != 0)
            {
                if(inode_postavi_a(&k, dsa, &sb->bmap) > 0)
                {
                    sb->bmap.obradi(&sb->bmap, zauzmi_bit, &dsb, &dsa);
                    pisi_na_disk(i, &dsb);
                    sb->koristeni_blokovi++;

                    nulblock(&dsb);
                    memcpy(dsb, &n_usr, sizeof(user));
                    pisi_na_disk(dsa, &dsb);

                    k.tok_podataka.velicina = ((br_k + 1) * sizeof(user));
                    memcpy(dsb, &k, sizeof(inode));
                    pisi_na_disk((sb->bmap.inode_start + 1), &dsb);//   pisanje novo stanje inode

                    memcpy(dsb, sb, sizeof(superblock));
                    pisi_na_disk(0, &dsb);//    pisanje novo stanje superbloka

                    k.tok_podataka.velicina = ((br_k + 1) * sizeof(user));
                    return 1;
                }
            }
        }

        printf("Nema više prostora...\n");
        return -3;
    }
}

void mjenjaj_udat(superblock *sb, inode *node, user *usr, unsigned long *nova_velicina)
{


/*    if(nbr_blokova == sbr_blokova)
    {
        ds_adresa *dsa;
        unsigned int i;
        dsa = inode_podatke(node);

        for(i = 0; i < nbr_blokova; i++)
        {

        }
    }
    else
    {

    }*/
}

ds_adresa kreiraj_dir(superblock *sb, user *usr, dir *d)
{
    ds_adresa a_inode = 0;
    ds_adresa a_data = 0;
    ds_adresa a_bmap = sb->bmap.dsa;
    ds_block dsb;

    while(a_bmap < sb->bmap.inode_start)
    {
        if((a_inode == 0) && (a_bmap <= bmap_bloka(&(sb->bmap), &(sb->bmap.prostor_start))))
        {
            citaj_sa_diska(a_bmap, &dsb);
            a_inode = slobodni_inode(&(sb->bmap), &dsb, &a_bmap);
        }
        if((a_data == 0) && (a_bmap >= bmap_bloka(&(sb->bmap), &(sb->bmap.prostor_start))))
        {
            citaj_sa_diska(a_bmap, &dsb);
            a_data = slobodni_prostor(&(sb->bmap), &dsb, &a_bmap);
        }
        if((a_inode != 0) && (a_data != 0))
        {
            inode i_novi, i_rod;
            dir d_novi;
            dir_op op;
            char ime[MAX_CHAR_LENGTH];
            init_dir_op(&op);
            int x;

            init_inode(&i_novi, usr, (a_inode - sb->bmap.inode_start), d->br_tren, je_dir);
            i_novi.tok_podataka.direktni[0] = a_data;
            i_novi.tok_podataka.velicina = sizeof(dir);

            printf("Upišite ime novog direktorija: ");
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

            citaj_sa_diska(sb->bmap.inode_start + d->br_tren, &dsb);
            memcpy(&i_rod, dsb, sizeof(inode));

            if((i_rod.tok_podataka.velicina + sizeof(dir_ele)) < (ceil((float)i_rod.tok_podataka.velicina/sizeof(ds_block)) * sizeof(ds_block)))
            {
                i_rod.tok_podataka.velicina += sizeof(dir_ele);
                pohrani_dir(&i_rod, d);

                citaj_sa_diska(bmap_bloka(&(sb->bmap), &a_inode), &dsb);
                sb->bmap.obradi(&(sb->bmap), zauzmi_bit, &dsb, &a_inode);
                pisi_na_disk(bmap_bloka(&(sb->bmap), &a_inode), &dsb);

                citaj_sa_diska(bmap_bloka(&(sb->bmap), &a_data), &dsb);
                sb->bmap.obradi(&(sb->bmap), zauzmi_bit, &dsb, &a_data);
                pisi_na_disk(bmap_bloka(&(sb->bmap), &a_data), &dsb);
            }
            else
            {
                ds_adresa dsa;
                ds_block dsb;
                unsigned int i;

                citaj_sa_diska(bmap_bloka(&(sb->bmap), &a_data), &dsb);
                sb->bmap.obradi(&(sb->bmap), zauzmi_bit, &dsb, &a_data);
                pisi_na_disk(bmap_bloka(&(sb->bmap), &a_data), &dsb);

                for(i = bmap_bloka(&(sb->bmap), &(sb->bmap.prostor_start)) /*sb->bmap.dsa*/; i < sb->bmap.inode_start; i++)
                {
                    citaj_sa_diska(i, &dsb);

                    if((dsa = slobodni_prostor(&(sb->bmap), &dsb, &i)) > 0)
                    {
                        if(inode_postavi_a(&i_rod, dsa, &(sb->bmap)) > 0)
                        {
                            pohrani_dir(&i_rod, d);

                            citaj_sa_diska(bmap_bloka(&(sb->bmap), &a_inode), &dsb);
                            sb->bmap.obradi(&(sb->bmap), zauzmi_bit, &dsb, &a_inode);
                            pisi_na_disk(bmap_bloka(&(sb->bmap), &a_inode), &dsb);
                            break;
                        }
                        else
                        {
                            citaj_sa_diska(bmap_bloka(&(sb->bmap), &a_data), &dsb);
                            sb->bmap.obradi(&(sb->bmap), oslobodi_bit, &dsb, &a_data);
                            pisi_na_disk(bmap_bloka(&(sb->bmap), &a_data), &dsb);
                            return 0;
                        }
                    }
                    else
                    {
                        citaj_sa_diska(bmap_bloka(&(sb->bmap), &a_data), &dsb);
                        sb->bmap.obradi(&(sb->bmap), oslobodi_bit, &dsb, &a_data);
                        pisi_na_disk(bmap_bloka(&(sb->bmap), &a_data), &dsb);
                        return 0;
                    }
                }
            }

            memcpy(dsb, &i_novi, sizeof(inode));
            pisi_na_disk(a_inode, &dsb);

            memcpy(dsb, &d_novi, sizeof(dir));
            pisi_na_disk(a_data, &dsb);

            return a_inode;
        }

        a_bmap++;
    }

    return 0;
}
static void pohrani_dir(inode *n, dir *d)
{
    ds_block dsb;
    dir_ele *tmp = d->head, test;
    unsigned int br_blokova;
    unsigned int i, br_deleub = ((sizeof(ds_block) - sizeof(dir)) / sizeof(dir_ele)), br_dele = ((n->tok_podataka.velicina - sizeof(dir)) / sizeof(dir_ele));
    nulblock(&dsb);
    memcpy(dsb, d, sizeof(dir));

    br_blokova = ceil((float)n->tok_podataka.velicina/sizeof(ds_block));

    for(i = 0; (i < br_deleub) && (i < br_dele); i++)
    {
        memcpy(dsb + (sizeof(dir) + (i * sizeof(dir_ele))), tmp, sizeof(dir_ele));
        tmp = tmp->next;
    }

    pisi_na_disk(n->tok_podataka.direktni[0], &dsb);

    br_deleub = sizeof(ds_block) / sizeof(dir_ele);

    for(i = 1; i < br_blokova; i++)
    {
        unsigned int j;
        for(j = 0; (j < br_deleub) && (j < br_dele); j++)
        {
            memcpy(dsb + (j * sizeof(dir_ele)), tmp, sizeof(dir_ele));
            tmp = tmp->next;
        }
        pisi_na_disk(inode_block(n, i), &dsb);
    }
}

void printaj_pomoc()
{
    printf(" ___________________________________________________________________________\n");
    printf("|NAREDBA     -       ZNAČENJE                                               |\n");
    printf("|---------------------------------------------------------------------------|\n");
    printf("|listaj      -       ispisuje datoteke i direktorije                        |\n");
    printf("|listaj_sve  -       ispisuje datoteke i direktorije s dodatnim informacija |\n");
    printf("|listaj_kor  -       ispisuje sve korisnike                                 |\n");
    printf("|kreiraj_dir -       stvaranje novog direktorija                            |\n");
    printf("|mjenjaj_dir -       otvaranje direktorija                                  |\n");
    printf("|kreiraj_kor -       stvaranje novog korisnika                              |\n");
    printf("|mjenjaj_kor -       mjenjanje korisnika                                    |\n");
    printf("|mjenjaj_mod -       mjenjanje prava                                        |\n");
    printf("|izlaz       -       izlaz iz programa                                      |\n");
    printf("|pomoc       -       ispisuje ovaj prozor                                   |\n");
    printf("|___________________________________________________________________________|\n");
    printf("\n");
}

void oslobodi_dir(dir *head)
{
    dir_op op;
    init_dir_op(&op);

    op.brisi_sve(head);
}

void listaj(dir *poz)
{
    dir_op op;
    init_dir_op(&op);

    op.ispisi(poz);
}

void listaj_sve(bitmap *bmap, dir *poz)
{
    ds_block dsb;
    inode node;
    dir_ele *tmp = poz->head;
    user *svek = dohvati_svek(bmap);
    unsigned int br_k, i;

    citaj_sa_diska(bmap->inode_start+1, &dsb);
    memcpy(&node, dsb, sizeof(inode));
    br_k = node.tok_podataka.velicina/sizeof(user);

    if(svek)
    {
        while(tmp != NULL)
        {
            citaj_sa_diska((bmap->inode_start + tmp->br_inode), &dsb);
            memcpy(&node, dsb, sizeof(inode));

            printaj_mod(&node);

            for(i = 0; i < br_k; i++)
            {
                if(svek[i].u.id == node.korisnik_id)
                {
                    printf("%s %s  ", svek[i].u.ime, svek[i].g.ime);
                    break;
                }
            }


            printf("%lu ", node.tok_podataka.velicina);
            printf("%d:%d %d.%d.%d ", (node.vrime_mjenjanja>>27)&31,(node.vrime_mjenjanja>>21)&63, (node.vrime_mjenjanja >> 16)&31,(node.vrime_mjenjanja >> 12)&15, node.vrime_mjenjanja&4095);
            printf("%s\n", tmp->ime_inode);
            tmp = tmp->next;
        }
        //printf("\n");
        free(svek);
    }

}

static user *dohvati_svek(bitmap *bmap)
{
    inode korisnici;
    unsigned int br_blokova;
    unsigned int vp;
    unsigned int kor_ublok = sizeof(ds_block)/sizeof(user);
    unsigned int i, j;
    ds_block dsb;
    user *sve;

    citaj_sa_diska(bmap->inode_start + 1, &dsb);
    memcpy(&korisnici, dsb, sizeof(inode));

    br_blokova = ceil(((float)korisnici.tok_podataka.velicina) / (sizeof(ds_block)));
    vp = korisnici.tok_podataka.velicina;

    sve = (user*)malloc(korisnici.tok_podataka.velicina);
    if(sve)
    {
        for(i = 0; i < br_blokova; i++)
        {
            citaj_sa_diska(inode_block(&korisnici, i), &dsb);

            if(sizeof(ds_block) < vp)
            {
                memcpy(sve + (i * (kor_ublok / sizeof(user))), &dsb, kor_ublok);
                vp -= sizeof(ds_block);
            }
            else
            {
                memcpy(sve + (i * (kor_ublok / sizeof(user))), dsb, vp);
            }

        }

        return sve;
    }
    else
        return NULL;
}

int citaj_dir(inode *node, dir *d)
{
    unsigned long v = node->tok_podataka.velicina;
    unsigned int br_ele = ((v - sizeof(dir))) / sizeof(dir_ele);
    unsigned int i;
    dir_op dop;
    dir_ele dele;
    ds_block dsb;

    init_dir_op(&dop);

    citaj_sa_diska(node->tok_podataka.direktni[0], &dsb);

    if(v <= sizeof(ds_block))
    {
        dir test;
        memcpy(&test, dsb, sizeof(dir));

        for(i = 0; i < br_ele; i++)
        {
            memcpy(&dele, dsb + sizeof(dir) + (i * sizeof(dir_ele)), sizeof(dir_ele));
            dop.dodaj(d, dele.br_inode, dele.ime_inode);
        }
    }
    else
    {
        unsigned int br_blokova = ceil(v / sizeof(ds_block));
        unsigned int ele_ublok = (sizeof(ds_block) - sizeof(dir)) / sizeof(dir_ele);// broj u 1. bloku
        unsigned int j;

        for(i = 0; i < ele_ublok; i++)
        {
            memcpy(&dele, dsb + sizeof(dir) + (i * sizeof(dir_ele)), sizeof(dir_ele));
            dop.dodaj(d, dele.br_inode, dele.ime_inode);
        }

        ele_ublok = sizeof(ds_block) / sizeof(dir_ele);// broj u ostalih blokova

        for(i = 1; i < br_blokova; i++)
        {
            citaj_sa_diska(inode_block(node, i), &dsb);
            for(j = 0; j < ele_ublok; j++)
            {
                memcpy(&dele, dsb + (i * sizeof(dir_ele)), sizeof(dir_ele));
                dop.dodaj(d, dele.br_inode, dele.ime_inode);
            }
        }
    }
}

void listaj_kor(superblock *sb)
{
    user *sve;
    sve = dohvati_svek(&sb->bmap);

    if(sve)
    {
        unsigned int i;

        for(i = 0; i <= sb->usr_id; i++)
        {
            printf("%i - %s@%s\n", sve[i].u.id, sve[i].u.ime, sve[i].g.ime);
        }
    }
    else
    printf("Došlo je do greške...\n");
}

void mjenjaj_kor(superblock *sb, user *usr)
{
    user *sve;
    unsigned int x, i;
    char c[MAX_CHAR_LENGTH];

    sve = dohvati_svek(&sb->bmap);

    if(sve)
    {
        listaj_kor(sb);
        printf("\nUnesite broj korisnika:\n");
        fgets(c, sizeof(c), stdin);
        x = c[0] - '0';


        for(i = 0; i <= sb->usr_id; i++)
        {
            if(x == sve[i].u.id)
            {
                usr->u.id = sve[i].u.id;
                strcpy(usr->u.ime, sve[i].u.ime);
                usr->g.id = sve[i].g.id;
                strcpy(usr->g.ime, sve[i].g.ime);
                return;
            }
        }
        printf("Netočan unos...\n");

    }
}

void mjenjaj_mod(superblock *sb, inode *node)
{
    unsigned int x;
    char c[MAX_CHAR_LENGTH];


    printf("\nUnesite novi mod ([0 - 7] [0 - 7] [0 - 7]):\n");
    fgets(c, sizeof(c), stdin);
    x = ((c[0] - '0') * 100) + ((c[1] - '0') * 10) + (c[2] - '0');

    if((x >= 0) && (x <= 777))
    {
        unsigned short tmp, tmp2;
        tmp = (short)x;

        tmp2 = tmp%10;
        tmp /= 10;

        tmp2 = ((tmp%10) | (tmp2 << 3));
        tmp /= 10;

        tmp2 = ((tmp%10) | (tmp2 << 3));

        tmp = node->mode;
        tmp = (tmp & 1);
        tmp2 = (tmp | (tmp2 << 1));

        node->mode = tmp2;
        return;
    }

    printf("Netočan unos...\n");
}

void mjenjaj_dir(superblock *sb, dir *poz, path **p, user *kor)
{

    char c[MAX_CHAR_LENGTH];
    dir_ele *tmp = poz->head;
    printf("Unesite ime direktorija\n: ");

    fgets(c, sizeof(c), stdin);
    c[strlen(c)-1] = '\0';

    if(strcmp(c, "..\0") == 0)
    {
        unsigned int i, x;
        inode node_d;
        ds_block dsb;

        dir roditej;
        dir_op dop;

        citaj_sa_diska((sb->bmap.inode_start + poz->br_roditelj), &dsb);
        memcpy(&node_d, dsb, sizeof(inode));
        citaj_sa_diska(node_d.tok_podataka.direktni[0], &dsb);
        memcpy(&roditej, dsb, sizeof(dir));

        init_dir_op(&dop);

        poz->br_roditelj = roditej.br_roditelj;
        poz->br_tren = roditej.br_tren;
        strcpy(poz->ime_roditelj, roditej.ime_roditelj);
        strcpy(poz->ime_tren, roditej.ime_tren);
        dop.brisi_sve(poz);
        poz->head = NULL;
        citaj_dir(&node_d, poz);
        path_brisi(p);

        return;
    }

    while(tmp != NULL)
    {
        if(strcmp(tmp->ime_inode, c) == 0)
        {
            inode node_d;
            ds_block dsb;
            unsigned short msk;

            citaj_sa_diska((sb->bmap.inode_start+tmp->br_inode), &dsb);
            memcpy(&node_d, dsb, sizeof(inode));

            if(node_d.mode & 1 == 1)
            {
                if(kor->u.id == node_d.korisnik_id)
                {
                    msk = 8;
                }
                else if(kor->g.id == node_d.grupa_id)
                {
                    msk = 64;
                }
                else
                {
                    msk = 512;
                }

                if(((msk & node_d.mode) != 0) || (kor->u.id == 0))
                {
                    if(path_dodaj(p, tmp->ime_inode) > 0)
                    {
                        dir_op dop;
                        init_dir_op(&dop);

                        poz->br_roditelj = poz->br_tren;
                        poz->br_tren = tmp->br_inode;
                        strcpy(poz->ime_roditelj, poz->ime_tren);
                        strcpy(poz->ime_tren, tmp->ime_inode);
                        dop.brisi_sve(poz);
                        poz->head = NULL;
                        citaj_dir(&node_d, poz);
                    }
                    else
                    {
                        printf("Greška u otvaranju direktorija...\n");
                    }
                }
                else
                {
                    printf("Nemate pravo otvorit direktorij...\n");
                }
            }
            else
            {
                printf("%s nije direktorij...\n", c);
            }

            return;
        }
        tmp = tmp->next;
    }
}

void brisi_dir(superblock *sb, dir *d, user *kor)
{
    char c[MAX_CHAR_LENGTH];
    dir_ele *tmp = d->head;
    printf("Unesite ime direktorija kojega želite brisati...\n: ");

    fgets(c, sizeof(c), stdin);
    c[strlen(c)-1] = '\0';

    while(tmp != NULL)
    {
        if(strcmp(tmp->ime_inode, c) == 0)
        {
            inode node_d;
            ds_block dsb;
            citaj_sa_diska((sb->bmap.inode_start + tmp->br_inode), &dsb);
            memcpy(&node_d, dsb, sizeof(inode));

            if((node_d.mode & 1) == 1)
            {
                unsigned short msk;

                if(kor->u.id == node_d.korisnik_id)
                {
                    msk = 4;
                }
                else if(kor->g.id == node_d.grupa_id)
                {
                    msk = 32;
                }
                else
                {
                    msk = 256;
                }

                if(((msk & node_d.mode) != 0) || (kor->u.id == 0))
                {
                    if(node_d.tok_podataka.velicina > sizeof(dir))
                    {
                        printf("Direktorij nije prazan...\n");
                    }
                    else
                    {
                        ds_adresa *dsa, a_node;
                        ds_block dsb;
                        unsigned int br_blokova = ceil(((float)node_d.tok_podataka.velicina) / sizeof(ds_block));
                        unsigned int i;
                        dir_op dop;

                        init_dir_op(&dop);

                        dsa = inode_podatke(&node_d);

                        for(i = 0; i < br_blokova; i++)
                        {
                            citaj_sa_diska(bmap_bloka(&(sb->bmap), &(dsa[i])), &dsb);
                            sb->bmap.obradi(&(sb->bmap), oslobodi_bit, dsb, &(dsa[i]));
                            pisi_na_disk(bmap_bloka(&(sb->bmap), &(dsa[i])), &dsb);
                        }

                        free(dsa);
                        a_node = (sb->bmap.inode_start + node_d.inode_br);
                        citaj_sa_diska(bmap_bloka(&(sb->bmap), &a_node), &dsb);
                        sb->bmap.obradi(&(sb->bmap), oslobodi_bit, dsb, &a_node);
                        pisi_na_disk(bmap_bloka(&(sb->bmap), &a_node), &dsb);

                        dop.brisi_ele(d, c);

                        citaj_sa_diska((sb->bmap.inode_start + d->br_tren), &dsb);
                        memcpy(&node_d, dsb, sizeof(inode));
                        node_d.tok_podataka.velicina -= sizeof(dir_ele);
                        postavi_vrimem(&node_d);
                        pohrani_dir(&node_d, d);

                        memcpy(dsb, &node_d, sizeof(inode));
                        pisi_na_disk((sb->bmap.inode_start + d->br_tren), &dsb);
                    }
                }
                else
                {
                    printf("Nemate pravo brisat direktorij...\n");
                }
            }
            return;
        }
        tmp = tmp->next;
    }
}
