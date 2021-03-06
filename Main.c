#include "meni.h"
#include <limits.h>
#include <time.h>
#include "format.h"
#include "direktorij.h"
#include "putanja.h"

void printaj_putanju(user *kor, path *p)
{
    printf("%s@%s:", kor->u.ime, kor->g.ime);
    path_printaj(p, kor);
}

void test_start()
{
    init_disk("test\0");
}
int main()
{/*
    user u[5], *pnt;
    int i;

    for(i = 0; i < 5; i++)
        u[i].g.id = i;

    pnt = (user*)malloc(5 * sizeof(user));

    for(i = 0; i < 5; i++)
        pnt[i].g.id = u[i].g.id;
    for(i = 0; i < 5; i++)
        printf("%d - ", pnt[i].g.id);

    free(pnt);*/

/*
    sve_g *x = NULL, *z;
    grp y;
    int i;
    char c[MAX_CHAR_LENGTH];

    for(i = 0; i < 11; i++)
    {
        y.id = i;
        if(i == 5)
            strcpy(y.ime, "Zdravko\0");
        else
            strcpy(y.ime, "test\0");
        dodaj_g(&x, &y);
    }

    z = x;
    printaj_sg(x);
    oslobodi_g(x);*/

/*
        char c[MAX_CHAR_LENGTH];
        printf("Dali želite se pridružiti postojećoj grupi?  [d] - DA, [n] - NE\n: ");
        fgets(c, sizeof(c), stdin);
        c[strlen(c)-1] = '\0';

while(true)
        {
            if(strcmp(c, "d\0") == 0)
            {
                break;
            }
            else if(strcmp(c, "n\0") == 0)
            {
                break;
            }
            else
            {
                printf("Nevažeći unos...  [d] - DA, [n] - NE\n: ");
                fgets(c, sizeof(c), stdin);
                c[strlen(c)-1] = '\0';
            }
        }*/

/*
    inode node;
    node.tok_podataka.velicina = (BR_DIREKTNIH + (sizeof(ds_block)/sizeof(ds_adresa)) + (((sizeof(ds_block)/sizeof(ds_adresa))) * ((sizeof(ds_block)/sizeof(ds_adresa)))))*SEKTOR;
    int i, j, k, x;
 //   ds_adresa *dsa;
    init_disk("blablabla");
    ds_block dsb;

    for(i = 0; i < 12; i++)
        node.tok_podataka.direktni[i] = i;

    for(i; i < 140; i++)
        memcpy(dsb+((i-12) * sizeof(int)), &i, sizeof(int));

    node.tok_podataka.indirektni = 0;
    node.tok_podataka.d_indirektni = 1;
    pisi_na_disk(node.tok_podataka.indirektni, &dsb);

    for(k = 2; k < 130; k++)
        memcpy(dsb + ((k - 2) * sizeof(int)), &k, sizeof(int));
    pisi_na_disk(node.tok_podataka.d_indirektni, &dsb);

    x = i;
    for(i, k = 2; i < 268; i++, k++)
    {
        for(j = 0; j < 128; j++, x++)
        {
            memcpy(dsb+(j * sizeof(int)), &x, sizeof(int));
        }
        pisi_na_disk(k, &dsb);
    }

//    dsa = inode_podatke(&node);
    k = 0;
    for(i = 0; i < 16524; i++)
    {
        if(i == inode_block(&node, i))
            printf("%d - ", inode_block(&node, i));//k++;
    }
    printf("%d\n", k);
//    free(dsa);
    uinit_disk();*/
/*
    bitmap b;
    init_bmap(&b);
    b.dsa = 1;
    b.inode_start = 4;
    b.prostor_start = 1232;
    b.prostor_stop = 12288;
    ds_block dsb;
    ds_adresa dsa = 12287;

    oneblock(&dsb);
    b.obradi(&b, oslobodi_bit, &dsb, &dsa);

    dsa = 5;

    printf("\n%d\n", bmap_bloka(&b, &dsa));
*/
/*
    int x = 1;
    int y = 1;
    char c[MAX_CHAR_LENGTH];
    dir d;
    dir_op op;
    init_dir_op(&op);
    d.br_roditelj = 0;
    d.br_tren = 0;
    strcpy(d.ime_roditelj, "root\0");
    strcpy(d.ime_tren, "root\0");
    d.head = NULL;

    while(x != 0)
    {
        printf("\n");
        printf("1: dodaj element...\n");
        printf("2: print...\n");
        printf("3: brisi sve...\n");
        printf("4: brisi element...\n");
        printf("0: izlaz...\n");
        printf("\n");
        printf(":::");
        scanf("%d", &x);

        switch(x)
        {
            case 1:
                printf("unesi ime:::");
                scanf("%s", &c);
                op.dodaj(&d, y, &c);
                y++;
                break;
            case 2:
                printf("---------------------------\n");
                printf("dir ime: %s\n", d.ime_tren);
                printf("---------------------------\n");
                op.ispisi(&d);
                printf("---------------------------\n");
                break;
            case 3:
                op.brisi_sve(&d);
                printf("\n");
                break;
            case 4:
                printf("unesi ime:::");
                scanf("%s", &c);
                x = op.brisi_ele(&d, &c);
                if(x != 0)
                    printf("inode_br: %d izbrisan...\n", x);
                else
                    printf("%s nije pronadjen...\n", c);

                x = 4;
                break;
            default:
                op.brisi_sve(&d);
                printf("good bye...\n");
                x = 0;
                break;
        }
    }*/
/*
    superblock sb;
    inode r, u;
    user su, *up;
    dir d;
    dir_ele de;
    ds_block dsb;
    ds_adresa dsa;
    unsigned int vp, bb, uub, i;

    init_disk("disk1");
//    format();

    dsa = 0;

    citaj_sa_diska(dsa, &dsb);
    memcpy(&sb, dsb, sizeof(superblock));

    dsa = sb.bmap.inode_start;
    citaj_sa_diska(dsa, &dsb);
    memcpy(&r, dsb, sizeof(inode));

    dsa += 1;
    citaj_sa_diska(dsa, &dsb);
    memcpy(&u, dsb, sizeof(inode));

    citaj_sa_diska(r.tok_podataka.direktni[0], &dsb);
    memcpy(&d, dsb, sizeof(dir));
    memcpy(&de, dsb + sizeof(dir), sizeof(dir_ele));

    citaj_sa_diska(u.tok_podataka.direktni[0], &dsb);
    memcpy(&su, dsb, sizeof(user));

    citaj_sa_diska(sb.bmap.dsa, &dsb);

    printf("-------------SUPERBLOCK-------------\n");
    printf("ime:                %s\n", sb.ds_ime);
    printf("magic1:             %x\n", sb.magic1);
    printf("velicina bloka:     %d\n", sb.velicina_bloka);
    printf("blok_shift:         %d\n", sb.blok_shift);
    printf("br_blokova:         %d\n", sb.br_blokova);
    printf("koristeni blokovi:  %d\n", sb.koristeni_blokovi);
    printf("max_velicina:       %d MB\n", ((sb.max_velicina/1024) / 1024));
    printf("magic2:             %x\n", sb.magic2);
    printf("bmap_start:         %d\n", sb.bmap.dsa);
    printf("bmap_inode_start:   %d\n", sb.bmap.inode_start);
    printf("bmap_prostor_start: %d\n", sb.bmap.prostor_start);
    printf("bmap_prostor_stop:  %d\n", sb.bmap.prostor_stop);
    printf("magic3:             %x\n", sb.magic3);
    printf("root_magic:         %x\n", sb.root_direktorij.magic);
    printf("root_inode_br:      %d\n", sb.root_direktorij.inode_br);
    printf("user id:            %d\n", sb.usr_id);
    printf("group id:           %d\n", sb.grp_id);
    printf("\n");
    printf("-------------INODE_ROOT-------------\n");
    printf("magic:          %x\n", r.magic);
    printf("vrimes:         %d:%d %d.%d.%d\n", (r.vrime_stvaranja>>27)&31,(r.vrime_stvaranja>>21)&63, (r.vrime_stvaranja >> 16)&31,(r.vrime_stvaranja >> 12)&15, r.vrime_stvaranja&4095);
    printf("vrimem:         %d:%d %d.%d.%d\n", (r.vrime_mjenjanja>>27)&31,(r.vrime_mjenjanja>>21)&63, (r.vrime_mjenjanja >> 16)&31,(r.vrime_mjenjanja >> 12)&15, r.vrime_mjenjanja&4095);
    printf("korisnik_id:    %d\n", r.korisnik_id);
    printf("grupa_id:       %d\n", r.grupa_id);
    printf("mode:           %d\n", r.mode);
    printf("roditelj:       %d\n", r.roditelj);
    printf("inode_br:       %d\n", r.inode_br);
    printf("velicina:       %d\n", r.tok_podataka.velicina);
    printf("direktorij:     %s:%d\n", d.ime_tren, d.br_tren);
    printf("userfile:       %s:%d\n", de.ime_inode, de.br_inode);
    printf("\n");
    printf("-------------INODE_USERS-------------\n");
    printf("magic:          %x\n", u.magic);
    printf("vrimes:         %d:%d %d.%d.%d\n", (u.vrime_stvaranja>>27)&31,(u.vrime_stvaranja>>21)&63, (u.vrime_stvaranja >> 16)&31,(u.vrime_stvaranja >> 12)&15, u.vrime_stvaranja&4095);
    printf("vrimem:         %d:%d %d.%d.%d\n", (u.vrime_mjenjanja>>27)&31,(u.vrime_mjenjanja>>21)&63, (u.vrime_mjenjanja >> 16)&31,(u.vrime_mjenjanja >> 12)&15, u.vrime_mjenjanja&4095);
    printf("korisnik_id:    %d\n", u.korisnik_id);
    printf("grupa_id:       %d\n", u.grupa_id);
    printf("mode:           %d\n", u.mode);
    printf("roditelj:       %d\n", u.roditelj);
    printf("inode_br:       %d\n", u.inode_br);
    printf("velicina:       %d\n", u.tok_podataka.velicina);
    printf("korisnik_ime:   %s\n", su.u.ime);
    printf("korisnik_id:    %d\n", su.u.id);
    printf("grupa_ime:      %s\n", su.g.ime);
    printf("grupa_id:       %d\n", su.g.id);
    printf("sizeof_user     %d\n", sizeof(user));
    printf("\n");
    printf("slobodni inode: %d\n", slobodni_inode(&sb.bmap, &dsb, &sb.bmap.dsa));
    citaj_sa_diska(sb.bmap.dsa, &dsb);
    printf("prostor_start:  %d\n", slobodni_prostor(&sb.bmap, &dsb, &sb.bmap.dsa));

    vp = u.tok_podataka.velicina;
    bb = ceil(((float)u.tok_podataka.velicina)/sizeof(ds_block));
    uub = ((sizeof(ds_block)/sizeof(user)) * sizeof(user));
    up = (user*)malloc(vp);

    for(dsa = 0; dsa < bb; dsa++)
    {
        citaj_sa_diska(inode_block(&u, dsa), dsb);

        if(sizeof(ds_block) <= vp)
        {
            memcpy(up + (dsa * (uub / sizeof(user))), dsb, uub);
            vp -= uub;
        }
        else
        {
            memcpy(up + (dsa * (uub / sizeof(user))), dsb, vp);
        }
    }

    for(dsa = 0; dsa < (u.tok_podataka.velicina / sizeof(user)); dsa++)
        printf("%d :: %s -- %d :: %s\n", up[dsa].u.id, up[dsa].u.ime, up[dsa].g.id, up[dsa].g.ime);
    free(up);

    for(i = 0; i < 7; i++)
    {
        printf("\n\n%d\n", dodaj_korisnika(&sb));
        citaj_sa_diska(3, &dsb);
        memcpy(&u, dsb, sizeof(inode));

        vp = u.tok_podataka.velicina;
        bb = ceil(((float)u.tok_podataka.velicina)/sizeof(ds_block));
        uub = ((sizeof(ds_block)/sizeof(user)) * sizeof(user));
        up = (user*)malloc(vp);

        for(dsa = 0; dsa < bb; dsa++)
        {
            citaj_sa_diska(inode_block(&u, dsa), dsb);

            if(sizeof(ds_block) <= vp)
            {
                memcpy(up + (dsa * (uub / sizeof(user))), dsb, uub);
                vp -= uub;
            }
            else
            {
                memcpy(up + (dsa * (uub / sizeof(user))), dsb, vp);
            }
        }

        for(vp = 0; vp < (u.tok_podataka.velicina / sizeof(user)); vp++)
            printf("%d :: %s -- %d :: %s\n", up[vp].u.id, up[vp].u.ime, up[vp].g.id, up[vp].g.ime);
    }

    uinit_disk();*/

    superblock sb;
    user korisnik;
    char cmd[MAX_CHAR_LENGTH];
    path *put = NULL;
    dir poz;

    if(g_meni() == 1)
    {
        if(!meni_1())// koristi postojeci
        {
            uinit_disk();
            return -1;
        }
        if(formatiran())
        {
            ds_block dsb;
            inode node;

            citaj_sa_diska(0, &dsb);
            memcpy(&sb, dsb, sizeof(superblock));//                     superblock

            citaj_sa_diska(sb.bmap.inode_start, &dsb);
            memcpy(&node, dsb, sizeof(inode));
            citaj_sa_diska(node.tok_podataka.direktni[0], &dsb);
            memcpy(&poz, dsb, sizeof(dir));//                           root direktorij
            poz.head = NULL;
            citaj_dir(&node, &poz);


            citaj_sa_diska(sb.bmap.inode_start + 1, &dsb);
            memcpy(&node, dsb, sizeof(inode));
            citaj_sa_diska(node.tok_podataka.direktni[0], &dsb);
            memcpy(&korisnik, dsb + sizeof(user), sizeof(user));//  korisnik
        }
    }
    else
    {
        if(!meni_2())// kreiraj novi
        {
           uinit_disk();
           return -1;
        }
        char c[MAX_CHAR_LENGTH];
        inode node;
        ds_block dsb;

        format();

        printf("\n");
        citaj_sa_diska(0, &dsb);
        memcpy(&sb, dsb, sizeof(superblock));
        printf("Unesite svoje ime: ");

        fgets(c, sizeof(c), stdin);
        c[strlen(c)-1] = '\0';
        strcpy(korisnik.u.ime, c);
        sb.usr_id++;
        korisnik.u.id = sb.usr_id;

        printf("Unesite ime grupe: ");
        fgets(c, sizeof(c), stdin);
        c[strlen(c)-1] = '\0';
        strcpy(korisnik.g.ime, c);
        sb.grp_id++;
        korisnik.g.id = sb.grp_id;

        citaj_sa_diska(sb.bmap.inode_start, &dsb);
        memcpy(&node, dsb, sizeof(inode));
        citaj_sa_diska(node.tok_podataka.direktni[0], &dsb);
        memcpy(&poz, dsb, sizeof(dir));
        poz.head = NULL;

        citaj_sa_diska((sb.bmap.inode_start + 1), &dsb);
        memcpy(&node, dsb, sizeof(inode));
        citaj_sa_diska(node.tok_podataka.direktni[0], &dsb);
        memcpy(dsb + sizeof(user), &korisnik, sizeof(user));
        node.tok_podataka.velicina = (2*sizeof(user));

        pisi_na_disk(node.tok_podataka.direktni[0], &dsb);

        memcpy(dsb, &node, sizeof(inode));
        pisi_na_disk((sb.bmap.inode_start + 1), &dsb);

        memcpy(dsb, &sb, sizeof(superblock));
        pisi_na_disk(0, &dsb);
    }

    citaj_dir(&sb.root_direktorij, &poz);

    printaj_putanju(&korisnik, put);
    fgets(cmd, sizeof(cmd), stdin);
    cmd[strlen(cmd)-1] = '\0';

    while(strcmp(cmd, "izlaz\0") != 0)
    {

        if(strcmp(cmd, "pomoc\0") == 0)
        {
            printaj_pomoc();
        }
        else if(strcmp(cmd, "listaj\0") == 0)
        {
            inode node_d;
            ds_block dsb;
            unsigned short msk;
            citaj_sa_diska(sb.bmap.inode_start + poz.br_tren, &dsb);
            memcpy(&node_d, dsb, sizeof(inode));

            if(korisnik.u.id == node_d.korisnik_id)
            {
                msk = 2;
            }
            else if(korisnik.g.id == node_d.grupa_id)
            {
                msk = 16;
            }
            else
            {
                msk = 128;
            }

            if(((msk & node_d.mode) != 0) || (korisnik.u.id == 0))
            {
                listaj(&poz);
            }
            else
            {
                printf("Nemate pravo čitat iz direktorija...\n");
            }
        }
        else if(strcmp(cmd, "listaj_sve\0") == 0)
        {
            inode node_d;
            ds_block dsb;
            unsigned short msk;
            citaj_sa_diska(sb.bmap.inode_start + poz.br_tren, &dsb);
            memcpy(&node_d, dsb, sizeof(inode));

            if(korisnik.u.id == node_d.korisnik_id)
            {
                msk = 2;
            }
            else if(korisnik.g.id == node_d.grupa_id)
            {
                msk = 16;
            }
            else
            {
                msk = 128;
            }

            if(((msk & node_d.mode) != 0) || (korisnik.u.id == 0))
            {
                listaj_sve(&sb.bmap, &poz);
            }
            else
            {
                printf("Nemate pravo čitat iz direktorija...\n");
            }
        }
        else if(strcmp(cmd, "listaj_kor") == 0)
        {
            listaj_kor(&sb);
        }
        else if(strcmp(cmd, "kreiraj_dir") == 0)
        {
            inode node_d;
            ds_block dsb;
            unsigned short msk;
            citaj_sa_diska(sb.bmap.inode_start + poz.br_tren, &dsb);
            memcpy(&node_d, dsb, sizeof(inode));

            if(korisnik.u.id == node_d.korisnik_id)
            {
                msk = 4;
            }
            else if(korisnik.g.id == node_d.grupa_id)
            {
                msk = 32;
            }
            else
            {
                msk = 256;
            }

            if(((msk & node_d.mode) != 0) || (korisnik.u.id == 0))
            {
                if(kreiraj_dir(&sb, &korisnik, &poz) > 0)
                {
                    postavi_vrimem(&node_d);
                    node_d.tok_podataka.velicina += sizeof(dir_ele);
                    memcpy(dsb, &node_d, sizeof(inode));
                    pisi_na_disk(sb.bmap.inode_start + poz.br_tren, &dsb);
                }
                else
                {
                    printf("Greška u dodavanju direktorija...\n");
                }
            }
            else
            {
                printf("Nemate pravo kreirat direktorij...\n");
            }
        }
        else if(strcmp(cmd, "mjenjaj_dir") == 0)
        {
            mjenjaj_dir(&sb, &poz, &put, &korisnik);
        }
        else if(strcmp(cmd, "brisi_dir") == 0)
        {
            brisi_dir(&sb, &poz, &korisnik);
        }
        else if(strcmp(cmd, "kreiraj_kor") == 0)
        {
            dodaj_korisnika(&sb);
        }
        else if(strcmp(cmd, "mjenjaj_kor") == 0)
        {
            mjenjaj_kor(&sb, &korisnik);
        }
        else if(strcmp(cmd, "mjenjaj_mod") == 0)
        {
            inode node;
            unsigned int x;
            char c[MAX_CHAR_LENGTH], *ptr;
            ds_block dsb;

            listaj(&poz);

            printf("\nUnesite i-node broj:\n");
            fgets(c, sizeof(c), stdin);
            x = (unsigned int)strtol(c, &ptr, 10);

            citaj_sa_diska(sb.bmap.inode_start + x, &dsb);
            memcpy(&node, dsb, sizeof(inode));

            if(node.magic == INODE_MAGIC)
            {
                unsigned short msk;

                if(korisnik.u.id == node.korisnik_id)
                {
                    msk = 4;
                }
                else if(korisnik.g.id == node.grupa_id)
                {
                    msk = 32;
                }
                else
                {
                    msk = 256;
                }

                if(((msk & node.mode) != 0) || (korisnik.u.id == 0))
                {
                    mjenjaj_mod(&sb, &node);
                }
                else
                {
                    printf("Nemate pravo mjenjat prava...\n");
                }
            }
            memcpy(dsb, &node, sizeof(inode));
            pisi_na_disk(sb.bmap.inode_start + x, &dsb);
        }
        else if(strcmp(cmd, "dir") == 0)
        {
            printf("br_tren: %d - ime_tren:%s\n", poz.br_tren, poz.ime_tren);
            printf("br_roditelj: %d - ime_roditelj:%s\n", poz.br_roditelj, poz.ime_roditelj);
        }
        else
        {
            printf("Nepostojeća naredba...\nUnesite 'pomoc' za ispisivanje naredbih...\n");
        }
        printaj_putanju(&korisnik, put);
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strlen(cmd)-1] = '\0';
    }

    oslobodi_dir(&poz);
    path_oslobodi(put);
    uinit_disk();

/*    int x = 1;
    path *p = NULL;
    char c[MAX_CHAR_LENGTH], *ptr;

    while(x != 0)
    {
        printf("1: dodaj...\n");
        printf("2: brisi...\n");
        printf("3: printaj...\n");
        printf(": ");

        fgets(c, sizeof(c), stdin);
        x = strtol(c, &ptr, 10);

        switch(x)
        {
            case 1:
                printf("Unesi ime: ");
                fgets(c, sizeof(c), stdin);
                c[strlen(c)-1] = '\0';
                path_dodaj(&p, c);
                break;
            case 2:
                path_brisi(&p);
                break;
            case 3:
                path_printaj(p);
                break;
            case 0:
                //path_oslobodi(p);
                break;

        }
    }

    path_oslobodi(p);*/


    return(0);

}

