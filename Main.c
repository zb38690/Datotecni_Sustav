#include "meni.h"
#include <limits.h>
#include <time.h>
#include "format.h"
#include "direktorij.h"
int main()
{
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

    dsa = sb.slobodni_inode;
    citaj_sa_diska(dsa, &dsb);
    memcpy(&r, dsb, sizeof(inode));

    dsa += 1;
    citaj_sa_diska(dsa, &dsb);
    memcpy(&u, dsb, sizeof(inode));

    citaj_sa_diska(r.tok_podataka.direktni[0], &dsb);
    memcpy(&d, dsb, sizeof(dir));
    //memcpy(&de, dsb + sizeof(dir), sizeof(dir_ele));


    printf("-------------SUPERBLOCK-------------\n");
    printf("ime:                %s\n", sb.ds_ime);
    printf("magic1:             %x\n", sb.magic1);
    printf("velicina bloka:     %d\n", sb.velicina_bloka);
    printf("blok_shift:         %d\n", sb.blok_shift);
    printf("br_blokova:         %d\n", sb.br_blokova);
    printf("koristeni blokovi:  %d\n", sb.koristeni_blokovi);
    printf("magic2:             %x\n", sb.magic2);
    printf("bitmap_start:       %d\n", sb.bmap.dsa);
    printf("bitmap_granica:     %d\n", sb.bmap.granica);
    printf("inode_poz:          %d\n", sb.slobodni_inode);
    printf("prostor_poz:        %d\n", sb.slobodni_prostor);
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
    printf("mode[0]:        %d\n", r.mode[0]);
    printf("mode[1]:        %d\n", r.mode[1]);
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
    printf("mode[0]:        %d\n", u.mode[0]);
    printf("mode[1]:        %d\n", u.mode[1]);
    printf("roditelj:       %d\n", u.roditelj);
    printf("inode_br:       %d\n", u.inode_br);
    printf("velicina:       %d\n", u.tok_podataka.velicina);
    printf("\n");
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
/*    ds_adresa dsa;
    ds_block dsb;
    superblock bs;
    init_disk("blablabla");

    format();

    dsa.alokacijska_grupa = 0;
    dsa.poz = 0;

    citaj_sa_diska(dsa, &dsb);

    memcpy(&bs, dsb, sizeof(superblock));

    printf("ime:               %s\n", bs.ds_ime);
    printf("magic1:            %x\n", bs.magic1);
    printf("velicina bloka:    %d\n", bs.velicina_bloka);
    printf("blok_shift:        %d\n", bs.blok_shift);
    printf("br_blokova:        %d\n", bs.br_blokova);
    printf("koristeni blokovi: %d\n", bs.koristeni_blokovi);
    printf("magic2:            %x\n", bs.magic2);
    printf("alokacijske_grupe: %d\n", bs.alokacijske_grupe);
    printf("ag_shift:          %d\n", bs.ag_shift);
    printf("bitmap_start:      %d\n", bs.bmap.dsa.poz);
    printf("bitmap_granica     %d\n", bs.bmap.granica);
    printf("inode_ag:          %d\n", bs.slobodni_inode.alokacijska_grupa);
    printf("inode_poz:         %d\n", bs.slobodni_inode.poz);
    printf("prostor_ag:        %d\n", bs.slobodni_prostor.alokacijska_grupa);
    printf("prostor_poz:       %d\n", bs.slobodni_prostor.poz);
    printf("magic3:            %x\n", bs.magic3);



    uinit_disk();*/
/*
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%d.%d.%d %d:%d:%d\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("%d\n", sizeof(long));*/
    return(0);

}
