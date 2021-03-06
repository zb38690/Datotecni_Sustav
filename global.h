#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED
#define MAX_CHAR_LENGTH 32
#define BR_DIREKTNIH 12
#define SEKTOR 512
#define SEKTOR_MULTIPLIER 1
#define INODE_POSTO 10
#define SUPERBLOCK_MAGIC1 0x42465331
#define SUPERBLOCK_MAGIC2 0xDD121031
#define SUPERBLOCK_MAGIC3 0x15B6830E
#define INODE_MAGIC 0x3BBE0AD9


typedef unsigned char byte;
typedef byte disk_blok[SEKTOR];
typedef byte ds_block[SEKTOR*SEKTOR_MULTIPLIER];
typedef unsigned int ds_adresa;


#endif // GLOBAL_H_INCLUDED
