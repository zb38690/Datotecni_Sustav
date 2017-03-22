#include "meni.h"
#define MB_MULTIPLIER 2048
int g_meni()
{
    int x;
    char c[MAX_CHAR_LENGTH];
    printf("* * * * * * * ZAVRŠNI RAD: DATOTEČNI SUSTAV * * * * * * *\n");
    printf("*                                                       *\n");
    printf("*            (0) Kreiraj novi                           *\n");
    printf("*            (1) Koristi postojeći                      *\n");
    printf("*                                                       *\n");
    printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
    printf("::");

    fgets(c, sizeof(c), stdin);
    x = c[0] - '0';
    while((x != 0 && x != 1) || strlen(c) != 2)
    {
        printf("Dozvoljen unos '0' ili '1'...pokušaj ponovno...\n::");
        fgets(c, sizeof(c), stdin);
        x = c[0] - '0';
    }
    return(x);
}

bool meni_1()
{
    char *ime;

    while(!isDiskInit() && ime)
    {
        ime = dohvati_ime_diska();
        init_disk(ime);
    }

    if(ime)
    {
        printf("Uspješna inicijalizacija diska...\n");
        free(ime);
        return true;
    }
    else
    {
        printf("Neuspješna inicializacija diska...\n");
        return false;
    }

}

bool meni_2()
{
    char *ime;

    printf("Upišite željeno ime diska...\n");
    ime = dohvati_ime_diska();

    while(datoteka_postoji(ime) && ime)
    {
        printf("Datoteka postoji. Izaberite drugo ime.\n");
        ime = dohvati_ime_diska();
    }


    if(!ime)
    {
        printf("Neuspješna inicializacija diska...\n");
        return false;
    }
    kreiraj_disk(ime);
    init_disk(ime);
    free(ime);
    return true;
}

char* dohvati_ime_diska()
{
    char c[MAX_CHAR_LENGTH];
    char *ime = NULL;
    printf("Ime diska: ");
    fgets(c, sizeof(c), stdin);

    c[strlen(c)-1] = '\0';
    ime = (char *)malloc(strlen(c));

    if(!ime)
        return NULL;

    strcpy(ime, c);

    return ime;
}

bool datoteka_postoji(char *ime)
{
    if( access( ime, F_OK ) != -1 )
        return true;
    else
        return false;
}

void kreiraj_disk(char* ime)
{
    char c[MAX_CHAR_LENGTH];
    char *ptr;
    int x, i = 0;
    byte b[SEKTOR];
    FILE *fp;

    printf("Upišite željenu veličinu diska (u MB)...\nVeličina:");
    fgets(c, sizeof(c), stdin);

    x = strtol(c, &ptr, 10);

    while(x < 1 || x > 2048)
    {
        printf("Dozvoljeno samo između 1 i 2048...\nVeličina: ");
        fgets(c, sizeof(c), stdin);
        x = strtol(c, &ptr, 10);
    }

    for(i; i<SEKTOR; i++)
        b[i] = 0;


    fp = fopen(ime, "wb");

    i = 0;


    for(i; i < (x*MB_MULTIPLIER); i++)
        fwrite(b, sizeof(b), 1, fp);

    fclose(fp);

    printf("Disk: %s od %dMB uspješno kreiran\n", ime, x);
}
