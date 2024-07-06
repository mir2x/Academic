//Circular Linked List

#include <stdio.h>
#include <stdlib.h>

typedef struct linkedList linkedList;

struct linkedList
{
    int info;                   //Linked List value variable
    linkedList *link;           //Linked List Pointer variable
    linkedList()                //Structure Constructor
    {
        ;
    }

};

void insertion(linkedList **L_location);    //Function for Linked List Insert Operation
void deletation(linkedList **L_location);   //Function for Linked List Delete Operation
void display(linkedList *p);                //Function for Linked List Display Operation

int main(void)
{
    int choice;
    linkedList *L = NULL;

    printf("<<<Circular Linked List>>>\n");
    printf("-----------------\n");
    printf("-----------------\n");
    printf("Enter your choice:\n");
    printf("\tChoose 1 to Insert data.\n");
    printf("\tChoose 2 to Delete data.\n");
    printf("\tChoose 3 to Display data.\n");
    printf("\tChoose 4 to Exit.\n");
    printf("\n\tYour Selected Option: ");
    scanf("%d",&choice);
    while(choice!=4)            //Loop to control choice menu
    {
        switch(choice)
        {
        case 1:
            insertion(&L);
            break;
        case 2:
            deletation(&L);
            break;
        case 3:
            display(L);
            break;
        default:
            printf("Invalid Input Provided.\n");
            break;
        }
        printf("Enter your choice:\n");
        printf("\tChoose 1 to Insert data.\n");
        printf("\tChoose 2 to Delete data.\n");
        printf("\tChoose 3 to Display data.\n");
        printf("\tChoose 4 to Exit.\n");
        printf("\n\tYour Selected Option: ");
        scanf("%d",&choice);
    }

    return 0;
}

void insertion(linkedList **L_location) //Function definition for sorted insertion
{

    int element;
    printf("------------------------------------\n");
    printf("You have selected to Insert new data\n");
    printf("\nPlease insert a data: ");
    scanf("%d",&element);

    linkedList *new_node = (linkedList*)malloc(sizeof(linkedList));

    linkedList *p= *L_location;
    new_node->info = element;

    //If the Linked List is empty
    if(p==NULL)
    {
        new_node->link = new_node;
        *L_location = new_node;
    }
    //If the new data is to be inserted at the beginning
    else if (p->info >= new_node->info)
    {
        while(p->link != *L_location)
            p = p->link;

        p->link = new_node;
        new_node->link = *L_location;
        *L_location = new_node;
    }
    //For other conditions
    else
    {
        while (p->link!= *L_location&& p->link->info < new_node->info)
            p = p->link;

        new_node->link = p->link;
        p->link = new_node;
    }

    printf("Data successfully Inserted to Linked List\n");
    printf("-----------------------------------------\n");
}

void deletation(linkedList **L_location)    //Function definition for deletion
{
    int element;

    printf("------------------------------------\n");
    printf("You have selected to Delete data\n");
    printf("\nPlease provide data which is to be deleted: ");

    scanf("%d",&element);

    linkedList *p=*L_location;

    //If the Linked List has no data
    if(p==NULL)
        printf("Linked List is empty");
    //If the Linked List has only 1 Data
    else if(p->info==element && p->link==p)
    {
        *L_location=NULL;
        printf("Requested Data Deleted");
    }
    //For other conditions
    else
    {
        linkedList *pred =p;
        while(p->info!=element)
        {
            //As this is circular Linked List
            //the loop breaks as soon as it starts again from 1st position
            if(p->link == *L_location)
            {
                printf("Requested Data does not exist in Linked List");
                break;
            }

            pred = p;
            p = p->link;
        }
        //In case searched value was found in Linked List
        if(p->info==element)
        {
            //If the searched value is the first member of Linked List
            if(p==*L_location)
            {
                //Loop to find out to help again connect the last and first
                //member of the Linked List
                while ( pred-> link != *L_location)
                    pred= pred -> link;
                *L_location = p->link;
                pred->link = *L_location;
            }
            //If the searched value is the last member of the Linked List
            else if(p->link==*L_location)
                pred->link=*L_location;
            //If the searched value is in any other state
            else
                pred->link=p->link;
            printf("Requested Data Successfully Deleted");
        }
    }

    printf("\n-------------------------------------------\n");
}

void display(linkedList *p) //Function definition for display
{
    printf("------------------------------------\n");
    printf("You have selected to Display data\n");
    //If the Linked List is not empty, then the if block is executed
    if(p!=NULL)
    {
        linkedList *pred=p;
        //Loop to print Linked List until it again circulates from 1st
        do
        {
            printf("%d ", pred->info);
            pred = pred->link;
        }
        while(pred!=p);
    }
    printf("\n");
    printf("That is all Data that were stored\n");
    printf("---------------------------------\n");
}
