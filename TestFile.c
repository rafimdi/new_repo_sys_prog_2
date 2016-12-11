/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOo Header oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/*
 *Author – Daniel ziv 205614308 and Redael Mdinaradze 319331823
2. Project – Exercise 2
3. Using – TestFile
4. Description – This Module is responsible for managing the different threads for each check of a file
 */

#include "TestFile.h"


/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
long int file_size;
int file_size_orginized[5];// we assume no file of size larger then 1024 terabytes exists in the system.
char five_bytes_to_return[6];
char file_extension[5];
int length_of_extension,location_of_last_point;
FILETIME ftCreate, ftAccess, ftWrite;
SYSTEMTIME stUTC_create , stLocal_create;
SYSTEMTIME stUTC_mod, stLocal_mod;
DWORD dwRet_mod, dwRet_create ;
LPTSTR lpszString_mod, lpszString_create;
DWORD dwSize_mod, dwSize_create;

/**
*
* Accepts:
* --------
* str_to_test-a char array (string)
*
* Returns:
* --------
* location of file extension
*
*Description:
*------------
* FindLocationOfExtension finds the location of file extension
*
*/
void FindLocationOfExtension(char* str)
{
	
	int i=0;
	Sleep(10);
	while(str[i]!=0)
	{
		while(str[i]!='.')
		{
			i++;
			break;
		}
		if(str[i]=='.')
		{
			location_of_last_point=i;
			i++;
		}
	}
}	

void FindFileExtension(char* file_name)
{
	Sleep(10);
	FindLocationOfExtension(file_name);
	length_of_extension=strlen(file_name)-location_of_last_point;
	strncpy(file_extension,file_name+(location_of_last_point*sizeof(char)),(length_of_extension));
	file_extension[strlen(file_name)-location_of_last_point]=0;
}

/**
*
* Accepts:
* --------
* 
* files_to_test - string of the name of the file containing names of all the files we wish to test
*
* Returns:
* --------
* size of the file in bytes
*
*Description:
*------------
* ReturnFileSize returns amount of bytes in file file_to_test
*
*/

void ReturnFileSize(FILE* file_to_test)
{
	int i=0;
	
	rewind(file_to_test);
	fseek(file_to_test,0,SEEK_END);
	file_size=ftell(file_to_test);
	while(file_size>0)// this was added (the purpose is obvious)
	{
		file_size_orginized[i]=(int)file_size%1024;
		file_size/=1024;//not sure if this is allowed
		i++ ;
	}
	rewind(file_to_test);
}
//TODO- convert file size to KB,MB etc. 
/**
*
* Accepts:
* --------
* 
* files_to_test - string of the name of the file containing names of all the files we wish to test
*
* five_bytes_arr- a char array (string) size 6 which will be filled with the first 5 bytes (chars) and 0 (string terminator) at the last place
*
* Returns:
* --------
* first 5 bytes output will be delivered using five_bytes_arr and the array will be null terminated
*
*Description:
*------------
* ReturnFiveBytes returns the first 5 bytes (chars) of a file.
*
*/
void ReturnFiveBytes(FILE* file_to_test)
{
	int i=0;
	char ch=getc(file_to_test);
	while(ch!=EOF && i<5)
	{
		five_bytes_to_return[i]=ch;
		i++;		
		 ch=getc(file_to_test);
	}
	five_bytes_to_return[i]=0;
		rewind(file_to_test);
}

/**
*
* Accepts:
* --------
* file name
* 
*
* Output logfile - string that represents the location of the log file we wish to create
*
* Returns:
* --------
* gets file timing data(created & modified)
*
* Description:
*------------
* GetFileTimeStatus uses the win32 get file time data
*/

void GetFileTimeStatus(HANDLE *hFile)
{
	int flag[5] ;
	int check_for_error_index ;
	Sleep(10);
	// Retrieve the file times for the file.
    flag[0] = GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite) ;
	flag[1] = FileTimeToSystemTime(&ftCreate, &stUTC_create);
	flag[2] = FileTimeToSystemTime(&ftWrite, &stUTC_mod);
	flag[3] = SystemTimeToTzSpecificLocalTime(NULL, &stUTC_create, &stLocal_create);
    flag[4] = SystemTimeToTzSpecificLocalTime(NULL, &stUTC_mod, &stLocal_mod);
	Sleep(10);

	for(check_for_error_index = 0; check_for_error_index < 5 ; check_for_error_index++)
	{
		if (flag[check_for_error_index] == 0){
			printf("CreateFile failed with %d\n", GetLastError());
		}
	}
	Sleep(10);
	// Build a string showing the date and time.

	//dwRet_create = StringCchPrintf(lpszString_create, dwSize_create, 
 //       TEXT("%02d/%02d/%d  %02d:%02d"),
 //       stLocal_create.wMonth, stLocal_create.wDay, stLocal_create.wYear,
 //       stLocal_create.wHour, stLocal_create.wMinute);

 //   dwRet_mod = StringCchPrintf(lpszString_mod, dwSize_mod, 
 //       TEXT("%02d/%02d/%d  %02d:%02d"),
 //       stLocal_mod.wMonth, stLocal_mod.wDay, stLocal_mod.wYear,
 //       stLocal_mod.wHour, stLocal_mod.wMinute);

}
/**
*
* Accepts:
* --------
* All inputs are accepted through the argv[] array
* File_path is a string representing the full path of the file we wish to check (technicly because of our assumptions its just the file name)
*
* Output logfile - string that represents the location of the log file we wish to create
*
* Returns:
* --------
* output logfile
*
* Description:
*------------
* TestFile uses the win32 API to create threads that runs checks for the file in file_path
*/

int main (int argc,char* argv[])
{
	int i;
	FILE *file_to_check=NULL;
	FILE *log_file=NULL;					// TODO - correct log file to the right format-not sure
	HANDLE ThreadHandles[NUM_OF_TESTS];   /* An array of thread handles */
	HANDLE hFile ;
	DWORD ThreadIDs[NUM_OF_TESTS];/* An array of threadIDs */
	if (argc<3)
		exit(1);
	file_to_check=fopen(argv[1],"r");
	if(file_to_check==NULL)
	{
		printf("error openning file\n");
		exit(2);
	}
	log_file=fopen(argv[2],"w");
	if(log_file==NULL)
	{
		printf("error openning file\n");
		exit(2);
	}
    hFile = CreateFile(ConvertCharStringToLPTSTR(argv[1]), GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, 0, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
    {
        printf("CreateFile failed with %d\n", GetLastError());
        return 1;
    }


	ThreadHandles[0] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)FindFileExtension,argv[1],0,&ThreadIDs[0]);
	ThreadHandles[1] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ReturnFileSize,file_to_check,0,&ThreadIDs[1]);
	ThreadHandles[2] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ReturnFiveBytes,file_to_check,0,&ThreadIDs[2]);
	ThreadHandles[3] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)GetFileTimeStatus,hFile,0,&ThreadIDs[3]);
	
	WaitForMultipleObjects(4,ThreadHandles,1,INFINITE);
	fprintf(log_file,"the file extension of the tested file is: %s\n",file_extension);
	fprintf(log_file,"the test file size is: ");
	for(i=5;i >= 0; i--)
	{
		if(file_size_orginized[i]!=0)
		{
			
			switch (i)
			{
				case 0:
				fprintf(log_file,"%d bytes ",file_size_orginized[i]);
				break;
				case 1:
				fprintf(log_file,"%d.%d KBs ",file_size_orginized[i] , file_size_orginized[i-1]/10);
				break;
				case 2:
				fprintf(log_file,"%d.%d MBs ",file_size_orginized[i] , file_size_orginized[i-1]/10);
				break;
				case 3:
				fprintf(log_file,"%d.%d GBs ",file_size_orginized[i] , file_size_orginized[i-1]/10);
				break;
				case 4:
				fprintf(log_file,"%d.%d TBs ",file_size_orginized[i] , file_size_orginized[i-1]/10);
				break;
				case 5:
				fprintf(log_file,"%d.%d PBs ",file_size_orginized[i], file_size_orginized[i-1]/10);
				break;
			}
			break;
			

		}
	}// next line has\n added to it for obvious reasons.

		//fprintf(log_file,"\nthe file was created on %02d/%02d/%d  %02d:%02d\n",stLocal_create.wDay, stLocal_create.wMonth, stLocal_create.wYear,


	fprintf(log_file,"\nthe file was created on %02d/%02d/%d  %02d:%02d\n",stLocal_create.wDay, stLocal_create.wMonth, stLocal_create.wYear, stLocal_create.wHour, stLocal_create.wMinute);
	fprintf(log_file,"the file was last modified on %02d/%02d/%d  %02d:%02d\n",stLocal_mod.wDay, stLocal_mod.wMonth, stLocal_mod.wYear,
        stLocal_mod.wHour, stLocal_mod.wMinute );// TODO - cpmplete the format
	fprintf(log_file,"The file’s first 5 bytes are: %s\n",five_bytes_to_return);
	fclose(file_to_check);
	fclose(log_file);
	exit(0);
}