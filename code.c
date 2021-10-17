#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
 
#define MAX_YR  9999
#define MIN_YR  1900
#define MAX_SIZE_USER_NAME 30
#define MAX_SIZE_PASSWORD  20
#define FILE_NAME  "studentRecordSystem.bin"
 
#define MAX_FATHER_NAME 50
#define MAX_STUDENT_NAME 50
#define MAX_STUDENT_ADDRESS 300
#define FILE_HEADER_SIZE  sizeof(sFileHeader)
 
typedef struct
{
    int yyyy;
    int mm;
    int dd;
} Date;
typedef struct
{
    char username[MAX_SIZE_USER_NAME];
    char password[MAX_SIZE_PASSWORD];
} sFileHeader;
 
typedef struct 
{
    unsigned int student_id;  
    char fatherName[MAX_FATHER_NAME]; 
    char studentName[MAX_STUDENT_NAME]; 
    char studentAddr[MAX_STUDENT_ADDRESS]; 
    Date studentJoiningDate; 
} s_StudentInfo;
 
void printMessageCenter(const char* message)
{
    int len =0;
    int pos = 0;
 
    len = (78 - strlen(message))/2;
    printf("\t\t\t");
    for(pos =0 ; pos < len ; pos++)
    {
 
        printf(" ");
    }
 
    printf("%s",message);
}
 
void headMessage(const char *message)
{
    system("cls");
    printf("\n\t\t\t------------ Student Management System ------------");
 
     
}
 
void welcomeMessage()
{
    
    printf("\n\n\n\t\t\t Loading enter any key to continue.....");
    getch();
}
 
int isNameValid(const char *name)
{
    int validName = 1;
    int len = 0;
    int index = 0;
    len = strlen(name);
    for(index =0; index <len ; ++index)
    {
        if(!(isalpha(name[index])) && (name[index] != '\n') && (name[index] != ' '))
        {
            validName = 0;
            break;
        }
    }
    return validName;
}
 
int  IsLeapYear(int year)
{
    return (((year % 4 == 0) &&
             (year % 100 != 0)) ||
            (year % 400 == 0));
}
 
int isValidDate(Date *validDate)
{
 
    if (validDate->yyyy > MAX_YR ||
            validDate->yyyy < MIN_YR)
        return 0;
    if (validDate->mm < 1 || validDate->mm > 12)
        return 0;
    if (validDate->dd < 1 || validDate->dd > 31)
        return 0;
 
    if (validDate->mm == 2)
    {
        if (IsLeapYear(validDate->yyyy))
            return (validDate->dd <= 29);
        else
            return (validDate->dd <= 28);
    }
 
    if (validDate->mm == 4 || validDate->mm == 6 ||
            validDate->mm == 9 || validDate->mm == 11)
        return (validDate->dd <= 30);
    return 1;
}
 
void addStudentInDataBase()
{
    s_StudentInfo addStudentInfoInDataBase = {0};
    FILE *fp = NULL;
    int status = 0;
    fp = fopen(FILE_NAME,"ab+");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    headMessage("ADD NEW Students");
    printf("\n\n\t\t\t ENTER Student Details:");
    printf("\n\n");
    printf("\n\t\t\tStudent ID : ");
    fflush(stdin);
    scanf("%u",&addStudentInfoInDataBase.student_id);
    do
    {
        printf("\n\t\t\tFather Name : ");
        fflush(stdin);
        fgets(addStudentInfoInDataBase.fatherName,MAX_FATHER_NAME,stdin);
        status = isNameValid(addStudentInfoInDataBase.fatherName);
        if (!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.");
        }
    }
    while(!status);
    do
    {
        printf("\n\t\t\tStudent Name : ");
        fflush(stdin);
        fgets(addStudentInfoInDataBase.studentName,MAX_STUDENT_NAME,stdin);
        status = isNameValid(addStudentInfoInDataBase.studentName);
        if (!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.");
        }
    }
    while(!status);
    do
    {
        printf("\n\t\t\tStudent Address : ");
        fflush(stdin);
        fgets(addStudentInfoInDataBase.studentAddr,MAX_FATHER_NAME,stdin);
        status = isNameValid(addStudentInfoInDataBase.studentAddr);
        if (!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.");
        }
    }
    while(!status);
    do
    {
        //get date year,month and day from user
        printf("\n\t\t\tEnter date in format (day/month/year): ");
        scanf("%d/%d/%d",&addStudentInfoInDataBase.studentJoiningDate.dd,&addStudentInfoInDataBase.studentJoiningDate.mm,&addStudentInfoInDataBase.studentJoiningDate.yyyy);
        //check date validity
        status = isValidDate(&addStudentInfoInDataBase.studentJoiningDate);
        if (!status)
        {
            printf("\n\t\t\tPlease enter a valid date.\n");
        }
    }
    while(!status);
    fwrite(&addStudentInfoInDataBase,sizeof(addStudentInfoInDataBase), 1, fp);
    fclose(fp);
}
 
void searchStudent()
{
    int found = 0;
    int studentId =0;
    s_StudentInfo addStudentInfoInDataBase = {0};
    FILE *fp = NULL;
    fp = fopen(FILE_NAME,"rb");
    if(fp == NULL)
    {
        printf("\n\t\t\tFile is not opened\n");
        exit(1);
    }
    headMessage("Search Students");
 
    if (fseek(fp,FILE_HEADER_SIZE,SEEK_SET) != 0)
    {
        fclose(fp);
        printf("\n\t\t\tFacing issue while reading file\n");
        exit(1);
    }
    printf("\n\n\t\t\tEnter Student ID no to search:");
    fflush(stdin);
    scanf("%u",&studentId);
    while (fread (&addStudentInfoInDataBase, sizeof(addStudentInfoInDataBase), 1, fp))
    {
        if(addStudentInfoInDataBase.student_id == studentId)
        {
            found = 1;
            break;
        }
    }
    if(found)
    {
        printf("\n\t\t\tStudent id : %d\n",addStudentInfoInDataBase.student_id);
        printf("\n\t\t\tStudent name : %s",addStudentInfoInDataBase.studentName);
        printf("\t\t\tFather Name : %s",addStudentInfoInDataBase.fatherName);
        printf("\n\t\t\tStudent Address : %s",addStudentInfoInDataBase.studentAddr);
        printf("\t\t\tStudent Admission Date(day/month/year) : (%d/%d/%d)",addStudentInfoInDataBase.studentJoiningDate.dd,
               addStudentInfoInDataBase.studentJoiningDate.mm, addStudentInfoInDataBase.studentJoiningDate.yyyy);
    }
    else
    {
        printf("\n\t\t\tNo Record");
    }
    fclose(fp);
    printf("\n\n\n\t\t\tPress any key to go to main menu.....");
    fflush(stdin);
    getchar();
}
 
void viewStudent()
{
    int found = 0;
    s_StudentInfo addStudentInfoInDataBase = {0};
    FILE *fp = NULL;
    unsigned int countStudent = 1;
    headMessage("View Student Details");
    fp = fopen(FILE_NAME,"rb");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    if (fseek(fp,FILE_HEADER_SIZE,SEEK_SET) != 0)
    {
        fclose(fp);
        printf("Facing issue while reading file\n");
        exit(1);
    }
    while (fread (&addStudentInfoInDataBase, sizeof(addStudentInfoInDataBase), 1, fp))
    {
        printf("\n\n\t\t\tStudent Count : %d\n\n",countStudent);
        printf("\n\t\t\tStudent id : %u\n",addStudentInfoInDataBase.student_id);
        printf("\t\t\tStudent Name : %s",addStudentInfoInDataBase.studentName);
        printf("\t\t\tFather Name : %s",addStudentInfoInDataBase.fatherName);
        printf("\t\t\tStudent Address : %s",addStudentInfoInDataBase.studentAddr);
        printf("\t\t\tStudent Admission Date(day/month/year) : (%d/%d/%d)\n\n",addStudentInfoInDataBase.studentJoiningDate.dd,
               addStudentInfoInDataBase.studentJoiningDate.mm, addStudentInfoInDataBase.studentJoiningDate.yyyy);
        found = 1;
        ++countStudent;
    }
    fclose(fp);
    if(!found)
    {
        printf("\n\t\t\tNo Record");
    }
    printf("\n\n\t\t\tPress any key to go to main menu.....");
    fflush(stdin);
    getchar();
}
 
void deleteStudent()
{
    int found = 0;
    int studentDelete = 0;
    sFileHeader fileHeaderInfo = {0};
    s_StudentInfo addStudentInfoInDataBase = {0};
    FILE *fp = NULL;
    FILE *tmpFp = NULL;
    headMessage("Delete Student Details");
    fp = fopen(FILE_NAME,"rb");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    tmpFp = fopen("tmp.bin","wb");
    if(tmpFp == NULL)
    {
        fclose(fp);
        printf("File is not opened\n");
        exit(1);
    }
    fread (&fileHeaderInfo,FILE_HEADER_SIZE, 1, fp);
    fwrite(&fileHeaderInfo,FILE_HEADER_SIZE, 1, tmpFp);
    printf("\n\n\t\t\tEnter Student ID NO. for delete:");
    scanf("%d",&studentDelete);
    while (fread (&addStudentInfoInDataBase, sizeof(addStudentInfoInDataBase), 1, fp))
    {
        if(addStudentInfoInDataBase.student_id != studentDelete)
        {
            fwrite(&addStudentInfoInDataBase,sizeof(addStudentInfoInDataBase), 1, tmpFp);
        }
        else
        {
            found = 1;
        }
    }
    (found)? printf("\n\t\t\tRecord deleted successfully....."):printf("\n\t\t\tRecord not found");
    fclose(fp);
    fclose(tmpFp);
    remove(FILE_NAME);
    rename("tmp.bin",FILE_NAME);
}
 
void updateCredential(void)
{
    sFileHeader fileHeaderInfo = {0};
    FILE *fp = NULL;
    unsigned char userName[MAX_SIZE_USER_NAME] = {0};
    unsigned char password[MAX_SIZE_PASSWORD] = {0};
    headMessage("Update Credential");
    fp = fopen(FILE_NAME,"rb+");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    fread (&fileHeaderInfo,FILE_HEADER_SIZE, 1, fp);
    if (fseek(fp,0,SEEK_SET) != 0)
    {
        fclose(fp);
        printf("\n\t\t\tFacing issue while updating password\n");
        exit(1);
    }
    printf("\n\n\t\t\tNew Username:");
    fflush(stdin);
    fgets(userName,MAX_SIZE_USER_NAME,stdin);
    printf("\n\n\t\t\tNew Password:");
    fflush(stdin);
    fgets(password,MAX_SIZE_PASSWORD,stdin);
    strncpy(fileHeaderInfo.username,userName,sizeof(userName));
    strncpy(fileHeaderInfo.password,password,sizeof(password));
    fwrite(&fileHeaderInfo,FILE_HEADER_SIZE, 1, fp);
    fclose(fp);
    printf("\n\t\t\tYour Password has been changed successfully");
    printf("\n\t\t\t Login Again:");
    fflush(stdin);
    getchar();
    exit(1);
}
 
void menu()
{
    int choice = 0;
    do
    {
        headMessage("MAIN MENU");
        printf("\n\n\n\t\t\t1.Add Student");
        printf("\n\t\t\t2.Search Student");
        printf("\n\t\t\t3.View Student");
        printf("\n\t\t\t4.Delete Student");
        printf("\n\t\t\t5.Update Credential");
        printf("\n\t\t\t6.Exit");
        printf("\n\n\n\t\t\tEnter choice => ");
        scanf("%d",&choice);
        switch(choice)
        {
        case 1:
            addStudentInDataBase();
            break;
        case 2:
            searchStudent();
            break;
        case 3:
            viewStudent();
            break;
        case 4:
            deleteStudent();
            break;
        case 5:
            updateCredential();
            break;
        case 0:
            printf("\n\n\n\t\t\t\tThank you!!!\n\n\n\n\n");
            exit(1);
            break;
        default:
            printf("\n\n\n\t\t\tINVALID INPUT!!! Try again...");
        }                                            //Switch Ended
    }
    while(choice!=0);                                        //Loop Ended
}
 
void login()
{
    unsigned char userName[MAX_SIZE_USER_NAME] = {0};
    unsigned char password[MAX_SIZE_PASSWORD] = {0};
    int L=0;
    sFileHeader fileHeaderInfo = {0};
    FILE *fp = NULL;
    headMessage("Login");
    fp = fopen(FILE_NAME,"rb");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    fread (&fileHeaderInfo,FILE_HEADER_SIZE, 1, fp);
    fclose(fp);
    do
    {
        printf("\n\n\n\t\t\t\tUsername:");
        fgets(userName,MAX_SIZE_USER_NAME,stdin);
        printf("\n\t\t\t\tPassword:");
        fgets(password,MAX_SIZE_PASSWORD,stdin);
        if((!strcmp(userName,fileHeaderInfo.username)) && (!strcmp(password,fileHeaderInfo.password)))
        {
            menu();
        }
        else
        {
            printf("\t\t\t\t Login Failed Enter Again Username & Password\n\n");
            L++;
        }
    }
    while(L<=3);
    if(L>3)
    {
        headMessage("Login Failed");
        printf("\t\t\t\tSorry,Unknown User.");
        getch();
        system("cls");
    }
}
 
int isFileExists(const char *path)
{
 
    FILE *fp = fopen(path, "rb");
    int status = 0;
    
    if (fp != NULL)
    {
        status = 1;
  
        fclose(fp);
    }
    return status;
}
void init()
{
    FILE *fp = NULL;
    int status = 0;
    const char defaultUsername[] ="user\n";
    const char defaultPassword[] ="pass\n";
    sFileHeader fileHeaderInfo = {0};
    status = isFileExists(FILE_NAME);
    if(!status)
    {
   
        fp = fopen(FILE_NAME,"wb");
        if(fp != NULL)
        {
 
            strncpy(fileHeaderInfo.password,defaultPassword,sizeof(defaultPassword));
            strncpy(fileHeaderInfo.username,defaultUsername,sizeof(defaultUsername));
            fwrite(&fileHeaderInfo,FILE_HEADER_SIZE, 1, fp);
            fclose(fp);
        }
    }
}
int main()
{
    init();
    welcomeMessage();
    login();
    return 0;
}
