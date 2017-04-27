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
        if(a_inode == 0)
        {
            citaj_sa_diska(a_bmap, &dsb);
            a_inode = slobodni_inode(&(sb->bmap), &dsb, &a_bmap);
        }
        if(a_data == 0)
        {
            citaj_sa_diska(a_bmap, &dsb);
            a_data = slobodni_prostor(&(sb->bmap), &dsb, &a_bmap);
        }
        if((a_inode != 0) && (a_data != 0))
        {
            inode i_novi;
            dir d_novi;
            dir_op op;
            char ime[MAX_CHAR_LENGTH];
            init_dir_op(&op);
            int x;

            init_inode(&i_novi, usr, (a_inode - sb->bmap.inode_start), d->br_tren, je_dir);
            i_novi.tok_podataka.direktni[0] = a_data;

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

            memcpy(dsb, &i_novi, sizeof(inode));
            pisi_na_disk(a_inode, &dsb);

            memcpy(dsb, &d_novi, sizeof(dir));
            pisi_na_disk(a_data, &dsb);

            citaj_sa_diska(a_bmap, &dsb);
            sb->bmap.obradi(&(sb->bmap), zauzmi_bit, &dsb, &a_inode);
            sb->bmap.obradi(&(sb->bmap), zauzmi_bit, &dsb, &a_data);
            pisi_na_disk(a_bmap, &dsb);

            return a_inode;
        }

        a_bmap++;
    }

    return 0;
}

void pohrani_dir(inode *n, dir *d)
{

}

void printaj_pomoc()
{
    printf(" ___________________________________________________________________________\n");
    printf("|NAREDBA     -       ZNAČENJE                                               |\n");
    printf("|---------------------------------------------------------------------------|\n");
    printf("|listaj      -       ispisuje datoteke i direktorije                        |\n");
    printf("|listaj_sve  -       ispisuje datoteke i direktorije s dodatnim informacija |\n");
    printf("|izlaz       -       izlaz iz datotečnog sustava                            |\n");
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

    if(svek)
    {
        while(tmp != NULL)
        {
            citaj_sa_diska((bmap->inode_start + tmp->br_inode), &dsb);
            memcpy(&node, dsb, sizeof(inode));

            printaj_mod(&node);
            br_k = node.tok_podataka.velicina/sizeof(user);

            for(i = 0; i < br_k; i++)
            {
                if(svek[i].u.id == node.korisnik_id)
                {
                    printf("%s %s  ", svek[i].u.ime, svek[i].g.ime);
                    break;
                }
            }


            printf("%lu ", node.tok_podataka.velicina);
//        printf("%d:%d %d.%d.%d ", (node.vrime_stvaranja>>27)&31,(node.vrime_stvaranja>>21)&63, (node.vrime_stvaranja >> 16)&31,(node.vrime_stvaranja >> 12)&15, node.vrime_stvaranja&4095);
            printf("%d:%d %d.%d.%d ", (node.vrime_mjenjanja>>27)&31,(node.vrime_mjenjanja>>21)&63, (node.vrime_mjenjanja >> 16)&31,(node.vrime_mjenjanja >> 12)&15, node.vrime_mjenjanja&4095);
            printf("%s\n", tmp->ime_inode);
            tmp = tmp->next;
        }
        printf("\n");
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
