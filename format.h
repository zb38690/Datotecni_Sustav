#ifndef FORMAT_H_INCLUDED
#define FORMAT_H_INCLUDED
#include <math.h>
#include "ds_disk_operacije.h"

void format();
bool formatiran();
ds_adresa inode_adresa(unsigned int, unsigned int);
ds_adresa prostor_adresa(unsigned int, unsigned int, ds_adresa);
#endif // FORMAT_H_INCLUDED
