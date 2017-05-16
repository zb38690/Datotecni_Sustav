#include "putanja.h"

int path_dodaj(path **head, char *ime)
{

    if(*head == NULL)
    {
        return path_stvori(head, ime);
    }

    path *tmp = *head;

    while(tmp->next != NULL)
    {
        tmp = tmp->next;
    }

    tmp->next = (path*)malloc(sizeof(path));

    if(tmp->next)
    {
        tmp = tmp->next;
        tmp->ime = (char*)malloc((strlen(ime) + 1) * sizeof(char));
        if(tmp->ime)
        {
            strcpy(tmp->ime, ime);
            tmp->next = NULL;
            return 1;
        }
        else
        {
            free(tmp);
        }
    }

    return -1;
}

static int path_stvori(path **head, char *ime)
{
    path *tmp;
    tmp = (path*)malloc(sizeof(path));

    if(tmp)
    {
        tmp->ime = (char*)malloc((strlen(ime) + 1) * sizeof(char));
        if(tmp->ime)
        {
            strcpy(tmp->ime, ime);
            tmp->next = NULL;
            *head = tmp;
            return 1;
        }
        else
        {
            free(tmp);
        }
    }
    return -1;
}


void path_oslobodi(path *head)
{
    path *tmp;
        while((tmp = head) != NULL)
        {
            head = head->next;
            free(tmp->ime);
            free(tmp);
        }
}


void path_printaj(path *head, user *kor)
{
    path *tmp = head;

    printf("/");
    while(tmp != NULL)
    {
        printf("%s", tmp->ime);
        if(tmp->next != NULL)
        {
            printf("/");
        }
        tmp = tmp->next;
    }
    if(kor->u.id == 0)
    {
        printf("# ");
    }
    else
    {
        printf("$ ");
    }

}

void path_brisi(path **head)
{
    path *tmp1, *tmp2;

    if(*head == NULL)
    {
        return;
    }

    tmp1 = *head;

    if(tmp1->next == NULL)
    {
        *head = NULL;
        free(tmp1->ime);
        free(tmp1);
    }
    else
    {
        tmp2 = tmp1->next;
        while(tmp2->next != NULL)
        {
            tmp1 = tmp1->next;
            tmp2 = tmp2->next;
        }

        tmp1->next = NULL;
        free(tmp2->ime);
        free(tmp2);
    }
}
