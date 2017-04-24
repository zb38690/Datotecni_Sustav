#include "user.h"

int dodaj_g(sve_g **head, grp *nova)
{

    if(*head == NULL)
    {
        return stvori_g(head, nova);
    }

    sve_g *tmp = *head;

    while(tmp->next != NULL)
    {
        if(tmp->g.id == nova->id)
            return 1;
        tmp = tmp->next;
    }

    if(tmp->g.id == nova->id)
        return 1;

    tmp->next = (sve_g*)malloc(sizeof(sve_g));

    if(tmp->next)
    {
        tmp->next->g.id = nova->id;
        strcpy(tmp->next->g.ime, nova->ime);
        tmp->next->next = NULL;
        return 1;
    }

    return -1;
}

static int stvori_g(sve_g **head, grp *nova)
{
    sve_g *tmp;
    tmp = (sve_g*)malloc(sizeof(sve_g));

    if(tmp)
    {
        tmp->g.id = nova->id;
        strcpy(tmp->g.ime, nova->ime);
        tmp->next = NULL;
        *head = tmp;
        return 1;
    }
    return -1;
}

void oslobodi_g(sve_g *head)
{
    sve_g *tmp;
        while((tmp = head) != NULL)
        {
            head = head->next;
            free(tmp);
        }
}


void printaj_sg(sve_g *head)
{
    sve_g *tmp = head;
    printf("\n----------------------------------------------\n");
    printf("                   GRUPE                      \n");
    printf("----------------------------------------------\n");
    while(tmp != NULL)
    {
        if(tmp->g.id < 10)
           printf("ID:  %d     IME: %s\n", tmp->g.id, tmp->g.ime);
        else
            printf("ID: %d     IME: %s\n", tmp->g.id, tmp->g.ime);

        tmp = tmp->next;
    }
    printf("----------------------------------------------\n");

}

int dohvati_sgime(unsigned int *id, user *usr, sve_g *head)
{
    sve_g *tmp = head;

    while(tmp != NULL)
    {
        if(tmp->g.id == (*id))
        {
            strcpy(usr->g.ime, tmp->g.ime);
            usr->g.id = tmp->g.id;
            return 1;
        }
        else
            tmp = tmp->next;
    }

    return -1;
}
