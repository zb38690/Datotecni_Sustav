#include "disk_simulator.h"

void init_disk(char imediska[])
{
    if((access(imediska, F_OK) != -1) && (access(imediska, R_OK) != -1) && (access(imediska, W_OK) != -1))
    {
        struct stat st;
        unsigned long long prostor;


        if(stat64(imediska, &st) == 0)
            prostor = st.st_size;
        else
            prostor = NULL;


        if((prostor != NULL) && (prostor <= LONG_MAX))
        {
            strcpy(disk.ime, imediska);

            disk.p_disk = fopen(disk.ime, "r+b");
            disk.poz_glave = 0;
            disk.prostor = prostor;
            disk.init = true;
        }
        else
          printf("Neuspjesna inicijalizacija diska...\n");
    }
    else
        printf("Nepostojeća datoteka/Nema pravo citat,pisat...\n");
}

void uinit_disk()
{
    if(disk.init)
    {
        fclose(disk.p_disk);
        disk.init = false;
    }
}

char* imediska()
{
    if(disk.init)
        return &disk.ime;
    else
        return "Disk nije inicijaliziran...\n";
}

unsigned long long velicina()
{
    if(disk.init)
        return disk.prostor;
    else
        return NULL;
}

disk_blok *citaj(long poz)
{
    disk_blok db;
    if(disk.init)
    {
        fseek(disk.p_disk, sizeof(disk_blok) * poz, SEEK_CUR);
        fread(db, sizeof(disk_blok), 1, disk.p_disk);
        disk.poz_glave = ((ftello64(disk.p_disk))/SEKTOR);
        return db;
    }
    else
        return NULL;
}

void pisi(long poz, disk_blok podatak)
{
    if(disk.init)
    {
        fseek(disk.p_disk, sizeof(disk_blok) * poz, SEEK_CUR);
        fwrite(podatak, sizeof(disk_blok), 1, disk.p_disk);
        disk.poz_glave = ((ftello64(disk.p_disk))/SEKTOR);
    }
}

unsigned long pozicija()
{
    if(disk.init)
        return disk.poz_glave;
    else
        return NULL;
}

bool isDiskInit()
{
    return disk.init;
}
