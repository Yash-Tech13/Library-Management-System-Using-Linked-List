#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Book
{
    int id;
    char Name[20],Author[20];
    struct Book *next;
};

void welcomeMessage()
{
    printf("\n  **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**\n");
    printf("\n        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
    printf("\n        =                 WELCOME                   =");
    printf("\n        =                   TO                      =");
    printf("\n        =                 LIBRARY                   =");
    printf("\n        =               MANAGEMENT                  =");
    printf("\n        =                 SYSTEM                    =");
    printf("\n        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
    printf("\n  **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**\n");
}

struct Book* Create_Available()
{
    struct Book *head;
    head = (struct Book*)malloc(sizeof(struct Book));
    char Name[20] , Author[20];
    printf("\n Enter Data for the Book");
    printf("\n Enter Book ID : ");
    scanf("%d",&head->id);

    printf("\n Enter Book Name : ");
    scanf("%s",Name);
    strcpy(head->Name , Name);

    printf("\n Enter Book Author: ");
    scanf("%s",Author);
    strcpy(head->Author , Author);

    head->next = NULL;
    return head;
}

void Add_Book(int m , struct Book *head)
{
    struct Book *temp;
    char Name[20],Author[20];

    while(head->next != NULL)
    {
        head = head->next;
    }
    
    for(int i=0; i<m ; i++)
    {
        temp = (struct Book*)malloc(sizeof(struct Book));
        printf("\n Enter Data for the Book");
        printf("\n Enter Book ID : ");
        scanf("%d",&temp->id);

        printf("\n Enter Book Name : ");
        scanf("%s",Name);
        strcpy(temp->Name,Name);

        printf("\n Enter Book Author: ");
        scanf("%s",Author);
        strcpy(temp->Author , Author);
        temp->next=NULL;
        head->next = temp;
        head = head->next;
    }
}

struct Book* delete(struct Book *head, int id)
{
    struct Book *temp = head;
    struct Book *prev;

    if(temp != NULL && temp->id == id)
    {
        head = temp->next;
        free(temp);
        return head;
    }

    while(temp != NULL && temp->id != id)
    {
        prev = temp;
        temp = temp->next;
    }

    if(temp == NULL)
        return head;
    
    prev->next = temp->next;

    free(temp);
    
    return head;
}

struct Book* create_list(struct Book *data)
{
    struct Book *head;
    head = (struct Book*)malloc(sizeof(struct Book));

    head->id = data->id;
    strcpy(head->Name, data->Name);
    strcpy(head->Author,data->Author);
    head->next = NULL;

    printf("List created\n");
    return head;
}

void add_to_list(struct Book *data, struct Book *head)
{
    while(head->next != NULL)
    {
        head = head->next;
    }

    struct Book *temp;
    temp = (struct Book*)malloc(sizeof(struct Book));
    head->next = temp;

    temp->id = data->id;
    strcpy(temp->Name, data->Name);
    strcpy(temp->Author,data->Author);
    temp->next = NULL;
}

struct Book* search(struct Book *head, int id)
{
    struct Book *temp,*p;
    int book_found = 0;

    temp = (struct Book *)malloc(sizeof(struct Book));

    p = head;
    while(p != NULL)
    {
        if(p->id == id)
        {
            book_found = 1;
            temp->id = p->id;
            strcpy(temp->Name, p->Name);
            strcpy(temp->Author ,p->Author);
        }
        p = p->next;
    }

    printf("Search Complete\n");
    if(book_found == 1)
        return temp;
    else
        return NULL;   
}

void Display(struct Book *head)
{
    printf("\nID \t Name \t Author");
    while(head!=NULL)
    {
        printf("\n%d \t %s\t %s",head->id,head->Name,head->Author);
        head = head->next;
    }
}

void writeListToFile(struct Book *available, struct Book *issued)
{
    FILE *pFile;
    pFile = fopen("Available.bin","wb");

    if(pFile != NULL)
    {
        struct Book *current = available;
        struct Book *hold = NULL;

        while(current != NULL)
        {
            hold = current->next;

            current->next = NULL;

            fseek(pFile,0,SEEK_END);
            fwrite(current, sizeof(struct Book),1,pFile);

            printf("Writing:%s to file\n",current->Name);

            current->next = hold;
            hold = NULL;
            current = current->next;
        }
        fclose(pFile);
        pFile = NULL;
    }
    else
    {
        printf("FILE OPEN ERROR\n");
    }

    pFile = fopen("Issued.bin","wb");
    if(pFile != NULL)
    {
        struct Book *current = issued;
        struct Book *hold = NULL;

        while(current != NULL)
        {
            hold = current->next;

            current->next = NULL;

            fseek(pFile,0,SEEK_END);
            fwrite(current, sizeof(struct Book),1,pFile);

            printf("Writing:%s to file\n",current->Name);

            current->next = hold;
            hold = NULL;
            current = current->next;
        }
        fclose(pFile);
        pFile = NULL;
    }
    else
    {
        printf("FILE OPEN ERROR\n");
    }
}

struct Book *ReadNextFromFile(struct Book *start, FILE *pFile)
{
    size_t returnValue;
    if(start == NULL)
    {
        start = malloc(sizeof(struct Book));
        returnValue = fread(start, sizeof(struct Book),1,pFile);
        start->next = NULL;
    }
    else
    {
        struct Book *indexBook = start;
        struct Book *newBook = malloc(sizeof(struct Book));
        while(indexBook->next != NULL)
        {
            indexBook = indexBook->next;
        }
        returnValue = fread(newBook, sizeof(struct Book),1,pFile);
        indexBook->next = newBook;
        newBook->next = NULL; 
    }
    return start;
}

struct Book *readAvailableList(struct Book *start)
{
    FILE *pFile;
    pFile = fopen("Available.bin","rb");
    if(pFile != NULL)
    {
        start = NULL;

        fseek(pFile, 0, SEEK_END);
        long fileSize = ftell(pFile);
        rewind(pFile);

        int numEntries = (int)(fileSize / (sizeof(struct Book)));
        printf("Number of Available Entries:%d\n",numEntries);

        int loop = 0;
        for(loop = 0;loop < numEntries; loop++)
        {
            fseek(pFile,(sizeof(struct Book) * loop), SEEK_SET);
            start = ReadNextFromFile(start,pFile);
        }
    }
    else
    {
        printf("FILE OPEN ERROR IN READ\n");
    }
    return start;
}

struct Book *readIssuedList(struct Book *start)
{
    FILE *pFile;
    pFile = fopen("Issued.bin","rb");
    if(pFile != NULL)
    {
        start = NULL;

        fseek(pFile, 0, SEEK_END);
        long fileSize = ftell(pFile);
        rewind(pFile);

        int numEntries = (int)(fileSize / (sizeof(struct Book)));
        printf("Number of Issued Entries:%d\n",numEntries);

        int loop = 0;
        for(loop = 0;loop < numEntries; loop++)
        {
            fseek(pFile,(sizeof(struct Book) * loop), SEEK_SET);
            start = ReadNextFromFile(start,pFile);
        }
    }
    else
    {
        printf("FILE OPEN ERROR IN READ\n");
    }
    return start;
}

int login()
{
    char id[50];
    int attempts = 0;
    User:
    printf("Enter your id: ");
    scanf("%s", id);

    if (strcmp(id,"admin")==0)
    {
        printf("Id is correct\n");
        attempts = 0;
    }

    else
    {
        printf("You have enetred an invalid user id\n");
        attempts++;
        if(attempts<3)
        {
            printf("Please enter id again\n");
            goto User;
        }
        else
            return 1;
    }

    char pass[50];

    pass:
    printf("Enter your password: ");
    scanf("%s", pass);

    if(strcmp(pass,"admin1122")==0)
    {
        printf("You have successly loged in\n");
    }

    else
    {
        printf("You have entered a wrong password\n");
        attempts++;
        if(attempts<3)
        {
            printf("Enter your password again\n");
            goto pass;
        }
        else
            return 1;
    }

    return 0;
}

int main()
{
    struct Book *available_head = NULL ,*issued_head = NULL, *p;
    int menu_check=1,choice;
    int Search_ID,x;

    welcomeMessage();
    
    x = login();

    if(x==0)
    {
        do
        {
            printf("\n1) Donate Books");
            printf("\n2) Issue New Book");
            printf("\n3) Return Issued Book");
            printf("\n4) Display Available Books");
            printf("\n5) Display Issued Books");
            printf("\n6) Write in file");
            printf("\n7) Read Data");
            printf("\n8) Exit");
            printf("\nEnter your choice : ");
            scanf("%d",&choice);

            switch (choice)
            {
                case 1: printf("\n Enter number of Books you want to donate : ");
                int m;
                scanf("%d",&m);
                if(available_head == NULL)
                {
                    available_head = Create_Available();
                    Add_Book(m-1,available_head);
                }
                else
                    Add_Book(m,available_head); 
                break;

                case 2: printf("\nEnter the Id of the Book you want to issue : ");
                scanf("%d",&Search_ID);
                p = search(available_head, Search_ID);
                if(p != NULL && issued_head == NULL)
                {
                    issued_head = create_list(p);
                    available_head = delete(available_head, Search_ID);
                    printf("Delete Done\n");
                }
                else if(p != NULL && issued_head != NULL)
                {
                    add_to_list(p, issued_head);
                    available_head = delete(available_head, Search_ID);
                    printf("Delete Done\n");
                }
                else if( p == NULL)
                {
                    printf("Book not found\n");
                }
                break;

                case 3: printf("\nEnter the Id of the Book you want to return : ");
                scanf("%d",&Search_ID);
                p = search(issued_head, Search_ID);
                if(p != NULL && available_head == NULL)
                {
                    printf("Bool found\n");
                    available_head = create_list(p);
                    issued_head = delete(issued_head, Search_ID);
                    printf("Delete Done\n");
                }
                else if(p != NULL && issued_head != NULL)
                {
                    add_to_list(p, available_head);
                    issued_head = delete(issued_head, Search_ID);
                    printf("Delete Done\n");
                }
                else if( p == NULL)
                {
                    printf("Book not found\n");
                }
                break;

                case 4: 
                if(available_head!=NULL)
                    Display(available_head);
                else
                    printf("\n No Books in this Category");
                break;

                case 5: 
                if(issued_head!=NULL)
                    Display(issued_head);
                else
                    printf("\n No Books in this Category");
                break;

                case 6: writeListToFile(available_head, issued_head);
                break;

                case 7: available_head = readAvailableList(available_head);
                issued_head = readIssuedList(issued_head);
                break;

                case 8: printf("Bye!!\n");
                break;

                default:printf("\nInvalid Choice");
                break;
            }

            if(choice!=8)
            {
                printf("\nDo you want to Continue?");
                printf("\nEnter 0 for NO");
                printf("\nEnter 1 for YES");
                printf("\nEnter your Choice : ");
                scanf("%d",&menu_check);
            }
            else
            {
                menu_check=0;
            }
        }
        while(menu_check==1);
    }
    else
    {
        printf("Maximum attempts reached\n");
    }
    return 0;
}