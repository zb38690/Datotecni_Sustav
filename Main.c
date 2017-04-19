#include "meni.h"
#include <limits.h>
#include <time.h>
#include "format.h"
#include "direktorij.h"

int main()
{
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
    dir d;
    dir_ele de;
    ds_block dsb;
    ds_adresa dsa;

    init_disk("blablabla");
    format();

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

//    sb.bmap.obradi(&sb.bmap, &sb.bmap.inode_start, zauzmi_bit);
    citaj_sa_diska(sb.bmap.dsa, &dsb);

    printf("-------------SUPERBLOCK-------------\n");
    printf("ime:                %s\n", sb.ds_ime);
    printf("magic1:             %x\n", sb.magic1);
    printf("velicina bloka:     %d\n", sb.velicina_bloka);
    printf("blok_shift:         %d\n", sb.blok_shift);
    printf("br_blokova:         %d\n", sb.br_blokova);
    printf("koristeni blokovi:  %d\n", sb.koristeni_blokovi);
    printf("magic2:             %x\n", sb.magic2);
    printf("bmap_start:         %d\n", sb.bmap.dsa);
    printf("bmap_inode_start:   %d\n", sb.bmap.inode_start);
    printf("bmap_prostor_start: %d\n", sb.bmap.prostor_start);
    printf("bmap_prostor_stop:  %d\n", sb.bmap.prostor_stop);
    printf("magic3:             %x\n", sb.magic3);
    printf("root_magic:         %x\n", sb.root_direktorij.magic);
    printf("root_inode_br:      %d\n", sb.root_direktorij.inode_br);
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
    printf("\n");
    printf("slobodni inode: %d\n", slobodni_inode(&sb.bmap, &dsb, &sb.bmap.dsa));
    citaj_sa_diska(sb.bmap.dsa, &dsb);
    printf("prostor_start:  %d\n", slobodni_prostor(&sb.bmap, &dsb, &sb.bmap.dsa));
    uinit_disk();

//printf("%lli\n%llu\n", LONG_MAX, ULONG_MAX);
/*
    if(g_meni() == 1)
    {
        meni_1();
        uinit_disk();
    }
    else
    {
        meni_2();
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
