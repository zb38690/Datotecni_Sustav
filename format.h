#ifndef FORMAT_H_INCLUDED
#define FORMAT_H_INCLUDED
#include <math.h>
#include <time.h>
#include "ds_disk_operacije.h"
#include "user.h"

void format();
bool formatiran();
void postavi_adrese(superblock*);
void postavi_root(superblock*);
#endif // FORMAT_H_INCLUDED
