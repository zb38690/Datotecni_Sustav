#ifndef DISK_SIMULATOR_H
#define DISK_SIMULATOR_H
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>
#include "global.h"
#define MAX_VELICINA SEKTOR*4294967296LL

typedef byte disk_blok[SEKTOR];

struct
{
    unsigned long long prostor;
    unsigned long poz_glave;
    bool init;
    FILE* p_disk;
    char ime[];
}typedef disk_simulator;

static disk_simulator disk;

void init_disk(char imediska[]);

void uinit_disk();

bool isDiskInit();

char* imediska();

unsigned long long velicina();

void citaj(unsigned long long poz, disk_blok* db);

void pisi(unsigned long long poz, disk_blok podatak);

unsigned long pozicija();
#endif /*DISK_SIMULATOR_H*/
