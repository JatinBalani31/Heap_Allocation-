#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 1000

typedef struct allocate_name			
{
	char name[20];					//Name of the node
    int size;
    int start;
    struct allocate_name* next;		//Pointer to next node
    struct allocate_name* prev;		//Pointer to previous node
   
}alloc_list;

typedef struct free_list_name
{
	int size;						
    int start;
    struct free_list_name* next;		//Pointer to next node
    struct free_list_name* prev;		//Pointer to prev node						
}free_list;

	free_list* Free;				
	alloc_list* my_alloc;
	
	void allocate();					//Function declaration
	void merge(free_list* fptr);
	void deallocate(char name[]);
	void check_name(char s[]);
	void print_allocated_list();
	void print_free();

int main()
{
    int choice;
    my_alloc= NULL;						    //Initializing allocated list

    Free =(free_list*) malloc (sizeof(free_list));					//Initializing head node of Free list
    Free->next = Free->prev = NULL;
    Free->size = SIZE;						//Making 1000 size heap memory
    Free->start = 0;
    
    printf("\t\t\tMemory allocation 1000");
	int flag=1;	   
	
    while(flag==1)
    {
        printf ("\n\n1.Allocate Memory\n2.Deallocate Memory\n Enter your choice(1/2) : ");
        scanf("%d",&choice);
        if(choice==1)
        {
        	allocate();
        }
        else if(choice==2)
        {
			char name[20];
       		printf("Enter name of block you want to delete :\n");
   		    scanf("%s",&name);
        	deallocate(name);
       	}
       	else
		{
       	 printf("Invalid input\n");
        }
        printf("\n || Status || \n Allocated list:");
        print_allocated_list();
       
	    printf ("\nFree list:\n");
        print_free();  
        
        printf("\nDo you wish to continue ? \nEnter '1' to continue '0' to exit ");
        scanf("%d",&flag);
    }    
    printf("PROCESS EXITED SUCCESSFULLY");
   return 0; 
}

void merge(free_list* fptr)
{
    if (fptr->prev)								//Case if previous node exists and is adjacent
    {
        if (fptr->prev->start + fptr->prev->size == fptr->start)
        {
            fptr->prev->size = fptr->prev->size + fptr->size;
            fptr->prev->next = fptr->next;
            if(fptr->next)
                fptr->next->prev = fptr->prev;
            free_list* temp = fptr;
            fptr = fptr->prev;
            free(temp);
        }
    }
    if (fptr->next)							    //Case if next node exists and is adjacent
    {
        if (fptr->start + fptr->size == fptr->next->start)
        {
            fptr->size += fptr->next->size;		//Changing size of current node
            
            fptr->next = fptr->next->next;		//Changing links
            if(fptr->next)
                fptr->next->prev = fptr;
        }
    }
}
void allocate()
{
    int n;												//To keep track of size to be allocated
    char name[20];										//Labelling block
    
	printf("Enter the memory size to be  allocated :");
    scanf("%d",&n);
    printf("Enter the name of allocation :");
    scanf("%s",&name);
    
	check_name(name);
    free_list* ptr = Free;
    
	while (ptr && ptr->size < n)						//Travesing to the free pointer
    {
        ptr = ptr->next;
    }
    if (ptr == NULL)
        printf("\n!!!!!!!!!!!!!!!!!!!!!! Memory cannot be allocated as size exceeded!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    else
    {
	    if ((ptr->size)-n > 0)
        {
            free_list* temp =(free_list*) malloc(sizeof(free_list));
            temp->start = ptr->start + n;			    			  //Start point of new Node initialised
            temp->next = temp->prev = NULL;
           
            if (ptr == Free)
            {
                temp->next = Free->next;
                Free = temp;
            }
            else
            {
                (ptr->prev)->next = temp;
                temp->next = ptr->next;
                temp->prev = ptr->prev;
                if(ptr->next)
                    ptr->next->prev = temp;
            }
            temp->size = ptr->size - n;
        }
        else
        {
            if(ptr->prev)
            {
                ptr->prev->next = ptr->next;
                if(ptr->next)
                    ptr->next->prev = ptr->prev;
            }
            else
            {
                Free = ptr->next;
                if(Free)
                	Free->prev = NULL;
            }
            
        } 
		alloc_list* alloc_1 =(alloc_list*) malloc (sizeof(alloc_list));		 //Declaring and Initializing Node that we want to add in list
        alloc_1->next = alloc_1->prev = NULL;
        alloc_1->size = n;
       
	    strcpy(alloc_1->name, name);
        alloc_1->start = ptr->start;
        free(ptr);
        
        if (my_alloc == NULL)												//Node is first 
            my_alloc= alloc_1;
        else
        {
            alloc_list* temp_alloc = my_alloc;									
            while (temp_alloc->next && temp_alloc->start < alloc_1->start)
            {
                temp_alloc = temp_alloc->next;
            }
            if (temp_alloc == my_alloc && temp_alloc->start > alloc_1->start)	    //When node is to be allocated at first position itself
            {
                alloc_1->next = temp_alloc;
                temp_alloc->prev = alloc_1;
                my_alloc= alloc_1;
            }
            else if (temp_alloc->next == NULL && temp_alloc->start < alloc_1->start)	//Condition for node at last position
            {
                temp_alloc->next = alloc_1;
                alloc_1->prev = temp_alloc;
                alloc_1->next = NULL;
            }
            else														//When Node is to be inserted in between		
            {
                (temp_alloc->prev)->next = alloc_1;						
                alloc_1->prev = temp_alloc->prev;
                alloc_1->next = temp_alloc;
                temp_alloc->prev = alloc_1;
            }
        }
        printf("Allocated from index: %d to %d",alloc_1->start,alloc_1->start + alloc_1->size );
    }
}

void deallocate(char name[])						//Function to deallocate memory
{
    alloc_list* ptr = my_alloc;						//Finding node with given name
 
    while (ptr && strcmp(ptr->name,name)!=0)
    {
        ptr = ptr->next;
    }
    if (ptr == NULL)							 
    {
        printf("Specified name node not found\n");
    }
    else
    {
        free_list* fptr =(free_list*) malloc(sizeof(free_list));	 //Freelist node that is to be inserted in freelist
       
	    fptr->next = fptr->prev = NULL;
        fptr->size = ptr->size;
        fptr->start = ptr->start;
        
        if (ptr == my_alloc)							//If deallocated memory is first node itself
            my_alloc = ptr->next;
        else
        {
            ptr->prev->next = ptr->next;
            if (ptr->next)
                ptr->next->prev = ptr->prev;
        }
        free_list* temp = Free;
        if(Free == NULL)
        {
            Free = fptr;
        }
        else
        {
            while (temp && temp->next && temp->start < fptr->start)
            {
                temp = temp->next;
            }
            if (temp && temp->next == NULL && temp->start < fptr->start)	//Case when freelist node is to be inserted at end
            {
                temp->next = fptr;
                fptr->prev = temp;
            }
            else
            {
                if (temp && temp == Free)
                {
                    fptr->next = temp;
                    temp->prev = fptr;
                    Free = fptr;
                }
                else if(temp)							   //When freelist node is to be inserted in between
                {
                    fptr->prev = temp->prev;
                    temp->prev->next = fptr;
                    temp->prev = fptr;
                    fptr->next = temp;
                }

            }
        }
        merge(fptr);									   //Merge continuous free blocks
    }
}
void check_name(char s[20])
{
    int flag = 1;
    alloc_list* ptr = my_alloc;
    
	while (ptr && flag==1)
    {
        if(strcmp(ptr->name,s)==0)
        {
            printf("Memory location with the same name exits! Deallocating previous one ...");
            deallocate(s);
            flag = 0;
        }
        ptr = ptr->next;
    }
}
void print_allocated_list()						//Printing allocated list
{
    alloc_list* ptr = my_alloc;    
	while(ptr)
    {
        printf("\n %d to %d ",ptr->start ,ptr->start + ptr->size);
        ptr = ptr->next;
    }
}
void print_free()								//Printing free list
{
    free_list* ptr = Free;
    while (ptr)
    {
        printf("\n %d to %d \n ",ptr->start,ptr->start + ptr->size ) ;
        ptr = ptr->next;
    }
}

