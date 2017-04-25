#include "meni.h"
#include <limits.h>
#include <time.h>
#include "format.h"
#include "direktorij.h"

void fn(inode *node, ds_adresa *dsa)
{
    int i;
    for(i = 0; i < 12; i++)
        *(dsa+i) = node->tok_podataka.direktni[i];
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

    superblock sb;
    inode r, u;
    user su, *up;
    dir d;
    dir_ele de;
    ds_block dsb;
    ds_adresa dsa;

    init_disk("blablabla");
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

    up = (user*)malloc((u.tok_podataka.velicina / sizeof(user)) * sizeof(user));

    for(dsa = 0; dsa < ceil((float)u.tok_podataka.velicina)/sizeof(ds_block))
    {
        if(dsa < BR_DIREKTNIH)
        {
            citaj_sa_diska(u.tok_podataka.direktni[i], &dsb);
            memcpy()
        }
    }

    for(dsa = 0; dsa < (u.tok_podataka.velicina / sizeof(user)); dsa++)
        printf("%d :: %s -- %d :: %s\n", up[dsa].u.id, up[dsa].u.ime, up[dsa].g.id, up[dsa].g.ime);
    free(up);
    printf("\n\n%d\n", bzvzx(&sb));
    uinit_disk();

//printf("%lli\n%llu\n", LONG_MAX, ULONG_MAX);
/*
    if(g_meni() == 1)
    {
        meni_1();// koristi postojeci
        uinit_disk();
    }
    else
    {
        meni_2();// kreiraj novi
        uinit_disk();
    }
*/
/*
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%d.%d.%d %d:%d:%d\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("%d\n", sizeof(long));*/
    return(0);

}
