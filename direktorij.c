#include "direktorij.h"

void init_dir_op(dir_op *dop)
{
    dop->dodaj = add;
    dop->ispisi = print_all;
    dop->brisi_sve = clear_all;
    dop->brisi_ele = clear_ele;
}
/*
void add_end(dir *d, int inode_br, char *inode_ime)
{
    dir_ele *tmp;

    if(d->head == NULL)
    {
        d->head = (dir_ele*)malloc(sizeof(dir_ele));
        if(d->head)
        {
            d->head->br_inode = inode_br;
            strcpy(d->head->ime_inode, inode_ime);
            d->head->next = NULL;
            return;
        }
    }
    else
    {
        tmp = d->head;
        while(tmp->next != NULL)
        {
            tmp = tmp->next;
        }

        tmp->next = (dir_ele*)malloc(sizeof(dir_ele));

        if(tmp->next)
        {
            tmp->next->br_inode = inode_br;
            strcpy(tmp->next->ime_inode, inode_ime);
            tmp->next->next = NULL;
            return;
        }
    }

    printf("ERROR: Greska u dodavanje elementa...\n");
}
*/
void clear_all(dir *d)
{
    dir_ele *tmp;
    dir_ele *tmp2;

    if(d->head != NULL)
    {
        tmp = d->head;
        while((tmp2 = tmp) != NULL)
        {
            tmp = tmp->next;
            free(tmp2);
        }
        d->head = NULL;
    }
}

void print_all(dir *d)
{
    dir_ele* tmp = d->head;

    while(tmp != NULL)
    {
        printf("%d : %s\n", tmp->br_inode, tmp->ime_inode);
        tmp = tmp->next;
    }
}

int add(dir *d, int inode_br, char *inode_ime)
{
    dir_ele *ptr, *tmp, *tmp2;
    ptr = (dir_ele*)malloc(sizeof(dir_ele));

    ptr->br_inode = inode_br;
    strcpy(ptr->ime_inode, inode_ime);
    ptr->next = NULL;

    if(ptr)
    {
        if(d->head == NULL)
            d->head = ptr;
        else if(strcmp(d->head->ime_inode, ptr->ime_inode) == 0)
        {
//            printf("%s vec postoji...\n", ptr->ime_inode);
            free(ptr);
            return -1;
        }
        else if(strcasecmp(d->head->ime_inode, ptr->ime_inode) > 0)
        {
            ptr->next = d->head;
            d->head = ptr;
        }

        else
        {
            tmp = d->head;
            tmp2 = tmp->next;

            while(tmp != NULL)
            {
                if(tmp2 == NULL)
                {
                    tmp->next = ptr;
                    tmp = NULL;
                }
                else
                {
                    if(strcmp(tmp2->ime_inode, ptr->ime_inode) == 0)
                    {
//                        printf("%s vec postoji...\n", ptr->ime_inode);
                        free(ptr);
                        tmp = NULL;
                        return -1;
                    }
                    else if(strcasecmp(tmp2->ime_inode, ptr->ime_inode) > 0)
                    {
                        ptr->next = tmp2;
                        tmp->next = ptr;
                        tmp = NULL;
                    }
                    else
                    {
                        tmp = tmp->next;
                        tmp2 = tmp2->next;
                    }
                }
            }
        }

        return 1;

    }
    else
//        printf("ERROR: Greska u dodavanje elementa...\n");
        return -2;
}

unsigned int clear_ele(dir *d, char *ime)
{
    dir_ele *tmp, *tmp2;
    unsigned int inode_br;

    if(d->head == NULL)
        return 0;
    else
    {
        tmp = d->head;
        if(strcmp(tmp->ime_inode, ime) == 0)
        {
            d->head = tmp->next;
            inode_br = tmp->br_inode;
            free(tmp);
            return inode_br;
        }
        else
        {
            tmp2 = tmp->next;

            while(tmp2 != NULL)
            {
                if(strcmp(tmp2->ime_inode, ime) == 0)
                {
                    tmp->next = tmp2->next;
                    inode_br = tmp2->br_inode;
                    free(tmp2);
                    return inode_br;
                }
                tmp = tmp2;
                tmp2 = tmp2->next;
            }

            return 0;
        }
    }
}
