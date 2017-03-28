#include "meni.h"
#include <limits.h>
#include <time.h>
#include "ds_disk_operacije.h"

int main()
{

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
    init_disk("blablabla");

    superblock sb, bs;
    ds_adresa dsa;
    ds_block dsb;
    char c[29] = "Jednostavni Datotecni Sustav\0";
    int i;
    strcpy(sb.ds_ime, c);


    sb.magic1 = SUPERBLOCK_MAGIC1;
    sb.magic2 = SUPERBLOCK_MAGIC2;
    sb.magic3 = SUPERBLOCK_MAGIC3;
    sb.velicina_bloka = SEKTOR*SEKTOR_MULTIPLIER;
    sb.blok_shift = log2(SEKTOR);
    sb.br_blokova = velicina()/sb.velicina_bloka;
    sb.koristeni_blokovi = 1;

    dsa.alokacijska_grupa = 0;
    dsa.poz = 0;

    memcpy(dsb, &sb, sizeof(superblock));

    pisi_na_disk(dsa, &dsb);


    citaj_sa_diska(dsa, &dsb);

    memcpy(&bs, dsb, sizeof(superblock));

    printf("ime:               %s\n", bs.ds_ime);
    printf("magic1:            %x\n", bs.magic1);
    printf("magic2:            %x\n", bs.magic2);
    printf("magic3:            %x\n", bs.magic3);
    printf("velicina bloka:    %d\n", bs.velicina_bloka);
    printf("koristeni blokovi: %d\n", bs.koristeni_blokovi);
    printf("blok_shift:        %d\n", bs.blok_shift);
    printf("br_blokova:        %d\n", bs.br_blokova);

    i = ceil(5.0/2);
    printf("\n->%d<-\n", i);
    uinit_disk();

    /*rec nesto;
    rec bla;
    disk_blok db;
    nesto.x = 7;
    nesto.y = 8;
    nesto.z = 9;

    init_disk("file.bin");

    memcpy(db, &nesto, sizeof(nesto));
    pisi(1, db);

    printf(imediska());
    printf("\n");
    printf("%llu\n", velicina());
    printf("%lu\n", pozicija());
    citaj(2, &db);
    memcpy(&bla, db, sizeof(nesto));
    printf("%lu\n", pozicija());
    printf("%d, %d, %d\n", bla.x, bla.y, bla.z);

    uinit_disk();*/
/*
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%d.%d.%d %d:%d:%d\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("%d\n", sizeof(long));*/
    return(0);

}
