#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#define RESET "\x1B[0m"
#define BOLD "\x1B[1m" //Bold Text Formula

int  oddDaysInMonth[]= {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
char weekDays[7][10] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
char months[12][11]={"January","Februrary","March","April","May","June","July","August","September"
                    ,"October","November","December"};

struct Event{
  char eventDate[12];
  char eventName[30]; 
};

int checkIsLeapYear(int year)  //Check if the year is Leap Year Or Not.
{
       if(year % 4 == 0 && year % 100 == 0 && year % 400 == 0)
       {
        return 1;
       }
       else if(year % 4 == 0 && year % 100 !=0)
       {
        return 1;
       }
       return 0;
}

 int calculateDay(char *fullDate) // Calculate Day for Entered Date.
{
    char *year = malloc(strlen(fullDate));
    strncpy(year, fullDate+6, 4);
    char *date = malloc(strlen(fullDate));
    strncpy(date, fullDate, 2);
    char *month = malloc(strlen(fullDate));
    strncpy(month, fullDate+3, 2);
    int enterYear  = atoi(year);
    int enterMonth = atoi(month);
    int enterDate  = atoi(date);
    int oddDays=0 , temp=enterYear;
    enterYear--;
    enterYear = enterYear % 400;
    if(enterYear >= 300)
    {
      enterYear = enterYear - 300;
      oddDays++; 
    }
    else if(enterYear >= 200)
    {
      enterYear = enterYear - 200;
      oddDays += 3;
    }
    else if(enterYear >= 100)
    {
      enterYear = enterYear - 100;
      oddDays += 5;
    }
    for(int i=temp-enterYear; i<temp; i++)
    {
       oddDays ++;
       if(checkIsLeapYear(i) == 1)
       {
        oddDays ++;
       }
    }
    oddDays = oddDays % 7;
    for(int i=1 ; i<enterMonth; i++)
    {
      oddDays+=oddDaysInMonth[i-1]%7;
    }
    oddDays =  oddDays % 7;
    oddDays += enterDate;
    if(enterMonth > 2 && checkIsLeapYear(temp) == 1)
    {
      oddDays ++;
    }
    oddDays =  oddDays % 7;
    return oddDays;
}

void calculateWeekDate()  // Calculate day for Entered Date
{
   char date[12];
   printf("Enter Date in dd-mm-yy format :- ");
   fgets(date, 11, stdin);
   getchar();
   printf("\nDay on Entered Date is %s *** .\n",weekDays[calculateDay(date)]);
}

void viewAllEvents()
{
   FILE *infile;
   struct Event currEvent;
   infile=fopen("Event.dat","r");
   if(infile == NULL)
   {
    fprintf(stderr, "\nError Opened file\n");
    exit(1);
   }
   char Date[10]="EventDate";
   char EventName[12]="EventName";
   printf("\nAll Your Events Are* \n\n");
   printf("%s      |     %s\n",Date,EventName);
   while(fread(&currEvent, sizeof(struct Event), 1, infile))
   {
     printf("%s     |     %s",currEvent.eventDate,currEvent.eventName);
   }
   fclose(infile);
}

int printCurrentMonth(char *temp, int oddDays, int enterMonth)
{
   char *year = malloc(strlen(temp));
   strncpy(year, temp+6, 4);
   int enterYear  = atoi(year); 
   printf("\n  #******** %s %d********#\n\n",months[enterMonth-1],enterYear);
   printf("  SUN  MON  TUE  WED  THUR  FRI  SAT \n");
   int totalDays=oddDaysInMonth[enterMonth-1];
   if(enterMonth == 2 && checkIsLeapYear(enterYear) == 1)
   totalDays++;
   int countNewWeek=0;
   for(int i=0; i < oddDays; i++)
   {
    printf("     ");
    countNewWeek++;
   }
   for(int i=1; i<=totalDays; i++)
   {
    printf("%5d",i);
    countNewWeek++;
    if(countNewWeek==7)
    {
        printf("\n");
        countNewWeek=0;
    }
   }
   printf("\n");
   return countNewWeek;
}

void printMonth()
{
   char date[8];
   printf("Enter Date in mm-yy format :- ");
   fgets(date, 8 , stdin);
   char temp[15];
   strcpy(temp,"01-");
   strcat(temp,date);
   char *month = malloc(strlen(temp));
   strncpy(month, temp+3, 2);
   int enterMonth = atoi(month);
   int oddDays = calculateDay(temp);
   printCurrentMonth(temp,oddDays,enterMonth);
}

void printYear()
{
   char date[8];
   printf("Enter Year :- ");
   fgets(date, 5 , stdin);
   char temp[15];
   strcpy(temp,"01-01-");
   strcat(temp,date);
   int oddDays = calculateDay(temp);
   for(int i=1; i<=12; i++)
   {
    oddDays=printCurrentMonth(temp,oddDays,i);
   }
}

int searchEvent(char *date)
{
   FILE *fp;
   fp=fopen("Event.dat","r");
   if(!fp)
   {
    return 0;
   }
   int found=0;
   struct Event currEvent;     
   while(fread(&currEvent, sizeof(struct Event), 1, fp))
   {
        if(strcmp(date,currEvent.eventDate) == 0)
        {
            found=1;
            break;
        }
   }
   fclose(fp);
   return found; 
}

void addNewEvent() //Add New Event to the File(Event.dat)
{
   struct Event currEvent;
   printf("Enter Date on Which u want to Save Event in dd-mm-yy format :- ");
   fgets(currEvent.eventDate, 11, stdin);
   getchar();
   if(searchEvent(currEvent.eventDate) == 1)
   {
     printf("\nEvent Already Exist On Enetered Date. Edit Your Event to Add Another Event\n");
     return ;
   }
   printf("Enter Event Name :- ");
   fgets(currEvent.eventName, 29, stdin);
   FILE *outfile;
   outfile = fopen("Event.dat","a");  // Opening File in Append Mode.
   if(outfile == NULL)
   {
    fprintf(stderr, "\nError Opened file\n");
    exit(1);
   }      
   fwrite(&currEvent, sizeof(struct Event), 1, outfile);
   if(fwrite != 0)
   {
    printf("Event Saved Successfully \n");
   }
   else
   {
    printf("Error in Saving Event \n");
   }
   fclose(outfile); 
}

void deleteEvent()
{
   char date[12];
   printf("Enter Date in dd-mm-yy format :- ");
   fgets(date, 11, stdin);
   getchar();
   FILE *fp;
   FILE *fptemp;
   int found=0;
   struct Event currEvent,tempEvent;
   fp=fopen("Event.dat","r");
   if(!fp)
   {
    printf("Unable to Open File");
    exit(0);
   }     
   fptemp=fopen("temp.dat","a");
   if(!fptemp)
   {
    printf("Unable to Open File");
    exit(0);
   }
   while(fread(&currEvent, sizeof(struct Event), 1, fp))
   {
        if(strcmp(date,currEvent.eventDate) == 0)
        {
            tempEvent=currEvent;
            found=1;
        }
        else
        {
            fwrite(&currEvent, sizeof(struct Event), 1, fptemp);
        }
   }
   if(found == 0)
   {
    printf("No Event Exist on Given Date \n");
   }
   else
   {
    printf("Event Name :- %s",currEvent.eventName);
    printf("Deleted Successfully\n");
   }
   fclose(fp);
   fclose(fptemp);
   remove("Event.dat");
   rename("temp.dat","Event.dat");
}

void editEvent()
{
   char date[12];
   printf("Enter Date in dd-mm-yy format :- ");
   fgets(date, 11, stdin);
   getchar(); 
   if(searchEvent(date) == 0)
   {
     printf("No Event Exist on this Date\n");
     return ;
   } 
   char eventName[30];
   printf("Enter New Event on the Entered Date :- ");
   fgets(eventName, 29, stdin);
   struct Event currEvent,tempEvent;
   strcpy(tempEvent.eventDate,date);
   strcpy(tempEvent.eventName,eventName);
   FILE *fp;
   FILE *fptemp;
   fp=fopen("Event.dat","r");
   if(!fp)
   {
    printf("Unable to Open File");
    exit(0);
   }     
   fptemp=fopen("temp.dat","a");
   if(!fptemp)
   {
    printf("Unable to Open File");
    exit(0);
   }
   while(fread(&currEvent, sizeof(struct Event), 1, fp))
   {
        if(strcmp(date,currEvent.eventDate) == 0)
        {

            fwrite(&tempEvent, sizeof(struct Event), 1, fptemp);  
        }
        else
        {
            fwrite(&currEvent, sizeof(struct Event), 1, fptemp);
        }
   }
   printf("\n*Event on %s Edited Successfully\n",currEvent.eventDate);
   fclose(fp);
   fclose(fptemp);
   remove("Event.dat");
   rename("temp.dat","Event.dat");
}

void main()
{
    system("cls");
    printf("%s",BOLD);
    int choice=-1;
    time_t t;
    t = time(NULL);
    struct tm tm = *localtime(&t);
    char newline;
    printf("\n\n");
    do{
      printf("\t\t\t\t\t\t---------------------------------------------\n");
	   printf("\t\t\t\t\t\t|      WELCOMES YOU TO CALENDAR SYSTEM      |\t\t\t\t\t%s %d, %d\n", months[tm.tm_mon], tm.tm_mday, tm.tm_year+1900);
	   printf("\t\t\t\t\t\t---------------------------------------------\n\n\n");
      
      printf("\n -: Choice Selection :- \n");
      printf("\t\t\t\t\t\t");
      printf("1- To View Week Day of a Particular Date\n");
      printf("\t\t\t\t\t\t");             //Completed.
      printf("2- To View Calendar of a Particular Month\n");
      printf("\t\t\t\t\t\t");           //Completed.
      printf("3- To View Calendar of a Particular Year\n"); 
      printf("\t\t\t\t\t\t");        //Completed.
      printf("4- To Add your own Events \n");
      printf("\t\t\t\t\t\t");                        //Completed.
      printf("5- To View All Events \n");
      printf("\t\t\t\t\t\t");                           //Completed.
      printf("6- To Delete your Events By Date \n");
      printf("\t\t\t\t\t\t");               //Completed.  
      printf("7- To Edit Event On Particular Date \n"); 
      printf("\t\t\t\t\t\t");          //Completed.
      printf("8- To Exit from the Programme\n");
      printf("\n\n");                 //Completed.
      printf("\t\t\tEnter your Choice: ");
      scanf("%d", &choice);
      scanf("%c", &newline);
      if(choice == 1)
      {
         calculateWeekDate();
      }
      else if(choice == 2)
      {
         printMonth();
      }
      else if(choice == 3)
      {
         printYear();
      }
      else if(choice == 4)
      {
         addNewEvent();              
      }
      else if(choice == 5)
      {
         viewAllEvents();
      }
      else if(choice == 6)
      {
         deleteEvent();
      }     
      else if(choice == 7)
      {
        editEvent();
      }
      if(choice != 8)
      {
       printf("\nPlease Enter Any Key to Continue \n");
       getch();
       system("cls");      
      }
    }while(choice != 8);
    printf("You Exited from the Programme Successfully**\n"); 
}