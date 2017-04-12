#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED
#include "global.h"
struct
{
    char ime[MAX_CHAR_LENGTH];
    unsigned int id;
}typedef usr;

struct
{
    char ime[MAX_CHAR_LENGTH];
    unsigned int id;
}typedef grp;

struct
{
    usr u;
    grp g;
}typedef user;
#endif // USER_H_INCLUDED
