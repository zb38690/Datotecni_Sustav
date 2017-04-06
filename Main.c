#include "meni.h"
#include <limits.h>
#include <time.h>
#include "format.h"

int main()
{
    inode x;
    postavi_vrimes(&x);

    printf("%d:%d - %d.%d.%d",(x.vrime_stvaranja>>27)&31,(x.vrime_stvaranja>>21)&63,(x.vrime_stvaranja >> 16)&31,(x.vrime_stvaranja >> 12)&15 ,x.vrime_stvaranja&4095);



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
