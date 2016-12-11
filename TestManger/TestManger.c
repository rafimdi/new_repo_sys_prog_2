/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOo Header oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOo*/
/*
 *Author – Daniel ziv 205614308 and Redael Mdinaradze 319331823
2. Project – Exercise 2
3. Using – TestManager
4. Description – This Module is responsible for managing the different processes for each checked file
 */

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO Library Includes oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <sys/stat.h>
#include <string.h>
#include <tchar.h>

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO project Includes oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#include "StringConversionTools.h"

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOo Defines oOoOoOoOoOoOoOoOoOoOoOoO*/

#define TIMEOUT_IN_MILLISECONDS 5000
#define BRUTAL_TERMINATION_CODE 0x55
#define STILL_ACTIVE_PROC       259
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
typedef struct proc_info_list
{
	PROCESS_INFORMATION procinfo;
	char command[1000];
	struct proc_info_list *next_proc;

}procInfo_type;
FILETIME SubtractFiletimes(FILETIME Late, FILETIME Early);

void addToProcList(procInfo_type **procinfo_list_hd_ptr,PROCESS_INFORMATION procinfo,char* command)
{
	procInfo_type  *tmp_node = *(procinfo_list_hd_ptr) ;
	procInfo_type *new_proc_node = (procInfo_type*) malloc(sizeof(procInfo_type));
	if(new_proc_node == NULL)
	{
		printf("MEM allocation failed\n");
		exit(-1);
	}
	new_proc_node->procinfo = procinfo;
	new_proc_node->next_proc = NULL ;
	strcpy(new_proc_node->command , command);
	// check if List is empty
	if ( *(procinfo_list_hd_ptr) == NULL)
	{
		*(procinfo_list_hd_ptr) = new_proc_node ;
		return ;
	}
	// check if the new_node goes to the begging of the linked list.
	if( new_proc_node->procinfo.dwProcessId < (*procinfo_list_hd_ptr)->procinfo.dwProcessId)
	{
		new_proc_node->next_proc = *(procinfo_list_hd_ptr);
		*(procinfo_list_hd_ptr) = new_proc_node;
		return ;
	}
	
		while ( tmp_node->next_proc != NULL )
		{
			if (new_proc_node->procinfo.dwProcessId < tmp_node->next_proc->procinfo.dwProcessId)
			{
				new_proc_node->next_proc = tmp_node->next_proc;
				tmp_node->next_proc = new_proc_node;
				return ; // if the function enters this if statement, it will insert to the list, thus a return can be made
			}
			tmp_node = tmp_node->next_proc;
		}
		// if the function gets to this segment ----> the new process' ID is the biggest---> add it to as the last node.
		tmp_node->next_proc = new_proc_node;
		return ;
}

int get_proc_list_len(procInfo_type *procinfo_list_hd_ptr)
{
	int list_len = 0;
	procInfo_type *tmp = procinfo_list_hd_ptr ;
	while (tmp != NULL)
	{
		tmp= tmp->next_proc;
		list_len++;
	}
	return list_len;
}
void get_handle_list_data(HANDLE *ipHandles, procInfo_type *procinfo_list_hd_ptr, int handle_arr_size)
{
	int i;
	procInfo_type *tmp = procinfo_list_hd_ptr;
	for(i = 0; i< handle_arr_size; i++)
	{
		ipHandles[i] = tmp->procinfo.hProcess;
		tmp = tmp->next_proc;
		
	}
}
int LinesInFile(FILE *fp)
{
	int lines=1;
	char ch;
	while(!feof(fp))
	{
		ch = fgetc(fp);
		if(ch == '\n')
		{
			lines++;
		}
	}
	rewind(fp);
	return lines;
}
void ReadFileNameAndProduceLog(FILE* files_to_test, char* output_files_directory, char** output_log_file_path,char** file_name);


/**
*
* Accepts:
* --------
* All inputs are accepted through the argv[] array
* files_to_test - string of the name of the file containing names of all the files we wish to test (argv[1])
*
* output_files_directory - string that represents the location of the directory we wish to put our output files into (argv[2])
*
*proccess_status_check_frequency- integer contating the amount of time we wish to let a process run (argv[3])
* Returns:
* --------
* All the output log files required
*
*Description:
*------------
*TestManager uses the win32 API to create a process that runs the
* command in 'CommandLine'. it uses the win32 API function CreateProcess()
* using default values for most parameters.
*/

void print_running_proc(procInfo_type *running_proc_list,FILE *runtime_logfile)
{
	
	procInfo_type *tmp = running_proc_list;
	FILETIME kernel_time;
	SYSTEMTIME current_time;
	FILETIME current_time_decoded;
	SYSTEMTIME kernel_time_decoded;
	FILETIME substract_time;
	SYSTEMTIME substract_time_decoded;
	FILETIME user_time;
	SYSTEMTIME user_time_decoded;
	FILETIME creation_time;
	FILETIME exit_time;
	FILETIME time_to_decode;
	SYSTEMTIME time_decoded;
	int seconds,milliseconds;
	fprintf(runtime_logfile,"List of running processes:\n");
	while (tmp != NULL)
	{
		Sleep(10);
		GetSystemTime(&current_time);
		SystemTimeToFileTime(&current_time,&current_time_decoded);
		GetProcessTimes(tmp->procinfo.hProcess,&creation_time,&exit_time,&kernel_time,&user_time);
		substract_time=SubtractFiletimes(current_time_decoded,exit_time);
		FileTimeToSystemTime(&substract_time,&substract_time_decoded);
		seconds=(int)(substract_time_decoded.wSecond);
		milliseconds=(int)(substract_time_decoded.wMilliseconds);
		fprintf(runtime_logfile,"Process %d running command %s for %d seconds and %d milliseconds\n",tmp->procinfo.dwProcessId, tmp->command,seconds,milliseconds );
//		fprintf(); //TODO
		tmp = tmp->next_proc ;
	}

}

void print_finish_proc(procInfo_type *end_proc_list, FILE *runtime_logfile)
{
	procInfo_type *tmp = end_proc_list;
	FILETIME kernel_time;
	FILETIME user_time;
	FILETIME creation_time;
	FILETIME exit_time;
	FILETIME substract_time;
	DWORD exit_code;
	unsigned int seconds;
	unsigned int milliseconds;
	SYSTEMTIME exit_time_decoded;
	SYSTEMTIME substract_time_decoded;
	SYSTEMTIME creation_time_decoded;
	SYSTEMTIME local_time;
	fprintf(runtime_logfile,"List of finished processes:\n");
	while (tmp != NULL)
	{
		GetExitCodeProcess(tmp->procinfo.hProcess,&exit_code);
		Sleep(10);
		GetProcessTimes(tmp->procinfo.hProcess,&creation_time,&exit_time,&kernel_time,&user_time);
		substract_time=(SubtractFiletimes(exit_time,creation_time));
		FileTimeToSystemTime(&substract_time,&substract_time_decoded);
		SystemTimeToTzSpecificLocalTime(NULL, &substract_time_decoded, &local_time);
		seconds=substract_time_decoded.wSecond;
		milliseconds=substract_time_decoded.wMilliseconds;
		fprintf(runtime_logfile,"Process %d ran command %s and exited with exit code after %ld seconds and %ld milliseconds\n",tmp->procinfo.dwProcessId, tmp->command,exit_code,seconds,milliseconds );
		tmp = tmp->next_proc ;
//		fprintf();//TODO
	}
}

void close_handle(procInfo_type *list)
{
	procInfo_type *tmp = list ;
	while (tmp != NULL)
	{
		CloseHandle(tmp->procinfo.hProcess); /* Closing the handle to the process */
		CloseHandle(tmp->procinfo.hThread); /* Closing the handle to the main thread of the process */
		tmp = tmp->next_proc;
	}
}
void free_list (procInfo_type *list)
{
	procInfo_type *tmp = list ;
	
	while (tmp != NULL)
	{
		tmp = tmp->next_proc ;
		free(list);
		list = tmp;
	}
}

/**
* calculates the difference ( a - b ) between two times in FILETIME format.
*
* parameters:
* ----------
* Late - the time that will be subtracted from ( a )
* Early - the time that will be subtracted ( b ) : should be earlier than 'Late'
*
* Returns:
* -------
* The difference in times: ( Late - Early )
*/
FILETIME SubtractFiletimes(FILETIME Late, FILETIME Early)
{
	typedef unsigned __int64 Unsigned64BitType;
	Unsigned64BitType Late64BitVal;
	Unsigned64BitType Early64BitVal;
	Unsigned64BitType Difference64BitVal;
	FILETIME DifferenceAsFILETIME;
	const Unsigned64BitType Low32BitsMask = 0x00000000FFFFFFFF;

	Late64BitVal = ((Unsigned64BitType)(Late.dwHighDateTime) << 32) +
		Late.dwLowDateTime;
	Early64BitVal = ((Unsigned64BitType)(Early.dwHighDateTime) << 32) +
		Early.dwLowDateTime;
	Difference64BitVal = Late64BitVal - Early64BitVal;

	DifferenceAsFILETIME.dwLowDateTime =
		(DWORD)(Difference64BitVal & Low32BitsMask);
	DifferenceAsFILETIME.dwHighDateTime =
		(DWORD)(Difference64BitVal >> 32);

	return DifferenceAsFILETIME;
}

int main (int argc,char* argv[])
{
	int i=0;
	//int time_till_wait=(int)(argv[2]-'0'); //What is this? argv[2] is the output directory.
	int lines_in_file;
	int run_proc_len,end_proc_len=0 ;
	char *file_path=NULL;
    char *output_log_file_path=NULL;
    int proccess_check_frequency= (atoi(argv[3]));
	char *runtime_log_file_path=(char*)malloc((strlen(argv[2])+20)*sizeof(char));
	char *command_line=(char*)malloc(sizeof(char)*1000);
	LPTSTR command;
	FILE *fp=NULL;
	FILE *runtime_logfile=NULL;
	PROCESS_INFORMATION procinfo;
	procInfo_type *procinfo_list_hd_ptr = NULL ;
	procInfo_type *running_proc_list = NULL ;
	procInfo_type *end_proc_list = NULL;
	procInfo_type *tmp_proc  ;
	HANDLE* ipHandles; // TODO ---> DONE
	int handle_arr_size ;
	DWORD waitcode=(DWORD)malloc(sizeof(DWORD));
	DWORD exitcode;
	BOOL retVal=0;
	LPDWORD lpExitCode;
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 };
	if (argc<4)
		exit(1);
	fp=fopen(argv[1],"r");
	if(fp==NULL)
	{
		printf("error openning file\n");
		exit(2);
	}
	strcpy(runtime_log_file_path,argv[2]);
	strcat(runtime_log_file_path,"\\");
	strcat(runtime_log_file_path,"runtime_logfile.txt");
	runtime_logfile=fopen(runtime_log_file_path,"w");
	if(runtime_logfile==NULL)
	{
		printf("error openning file\n");
		exit(2);
	}
	lines_in_file=LinesInFile(fp);
	
	for(i=0;i<lines_in_file;i++)
	{
		ReadFileNameAndProduceLog(fp, argv[2],&output_log_file_path,&file_path);
		strcpy(command_line,"TestFile.exe");
		strcat(command_line," ");
		strcat(command_line,file_path);
		strcat(command_line," ");
		strcat(command_line,output_log_file_path);
		command=ConvertCharStringToLPTSTR(command_line);
		//create process
		retVal = CreateProcess(NULL,command,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&startinfo,&procinfo);
		if (retVal == 0)
		{
			fprintf(runtime_logfile,"!!! Failed to create new process to run %s. Error code:%d!!!\n", file_path,GetLastError());
			return;
		}
		else{
		fprintf(runtime_logfile,"Successfully created a process with ID %d to execute %s\n",procinfo.dwProcessId ,command_line);//change i->procees_id
		addToProcList(&procinfo_list_hd_ptr, procinfo,command_line);
		//ipHandles[i]=procinfo[i].hProcess;//TODO --  will also have to define a linked list for ipHandle
		}	
	}
	handle_arr_size = get_proc_list_len(procinfo_list_hd_ptr);
	ipHandles=(HANDLE*)malloc(sizeof(HANDLE)*handle_arr_size);
	get_handle_list_data(ipHandles, procinfo_list_hd_ptr, handle_arr_size);
	run_proc_len = handle_arr_size;
	// start to make sampels to check process status.

	while (end_proc_len < handle_arr_size)
	{
		free_list(running_proc_list);
		free_list(end_proc_list);	
		running_proc_list = NULL ;
		end_proc_list = NULL ;
		WaitForMultipleObjects(handle_arr_size,ipHandles,1,proccess_check_frequency);
		Sleep(10);
		tmp_proc = procinfo_list_hd_ptr ;
		while (tmp_proc != NULL)
		{
			GetExitCodeProcess(tmp_proc->procinfo.hProcess , &exitcode); 
			// will get a new running process list
			if (exitcode == (DWORD)STILL_ACTIVE_PROC)
			{
				addToProcList(&running_proc_list, tmp_proc->procinfo, tmp_proc->command); 
			}
			else
			{
				addToProcList(&end_proc_list, tmp_proc->procinfo, tmp_proc->command);
			}
			tmp_proc = tmp_proc->next_proc ;
		}
		run_proc_len = get_proc_list_len(running_proc_list);
		end_proc_len = get_proc_list_len(end_proc_list);
		if(run_proc_len != 0){
			print_running_proc(running_proc_list, runtime_logfile);
		}
		if(end_proc_len != 0){
			print_finish_proc(end_proc_list, runtime_logfile);}
	}
	// when exits this loop, all the process are finished---> print to log file appropiate message.
	fprintf(runtime_logfile, "All the processes have finished running. Exiting program.\n");
	//Sleep(10); /* Waiting a few milliseconds for the process to terminate */
	//}
	//for(i=0;i<lines_in_file;i++)
	//{
	//	GetExitCodeProcess(procinfo[i].hProcess, &exitcode);
	//	fprintf(runtime_logfile,"The exit code for the process is 0x%x\n", exitcode);
	//	CloseHandle(procinfo[i].hProcess); /* Closing the handle to the process */
	//	CloseHandle(procinfo[i].hThread); /* Closing the handle to the main thread of the process */
	//}

	//close handels & free all lists.
	close_handle(procinfo_list_hd_ptr); // both process & tread handels
	free_list(procinfo_list_hd_ptr);
	free_list(running_proc_list);
	free_list(end_proc_list);

	exit(0);
}

int FindLocationOfExtension(char* str)
{
	int i=0,location_of_last_point;
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
	return location_of_last_point;
}
/**
*
* Accepts:
* --------
*
* files_to_test -  file pointer of the file which contains names of all the files we wish to test
*
* output_files_directory - string that represents the location of the directory we wish to put our output files into
*
* output_log_file_path -pointer to string of the path of a file we wish to write the log to (not a full file_path)
* will be used for output.
*
* Returns:
* --------
* file_name - string of the name of a file we wish to test (not a full file_path)
* output_log_file_path- string of the path of a file we wish to write the log to (not a full file_path)
*
* Description:
* -----------
* ReadFileNameAndProduceLog reads a line from files_to_test and produces the File name
* and output file path for TestFile.exe 
*
*/
void ReadFileNameAndProduceLog(FILE* files_to_test, char* output_files_directory, char** output_log_file_path,char** file_name)
{
	int CUR_MAX = 100;
	char *token;
	char ch=getc(files_to_test);
	int count = 0;
	int length = 0;
	int locat;
	(*file_name) = (char*) malloc(sizeof(char) * CUR_MAX); // allocate buffer.
	if((*file_name)==NULL)
	{
		printf("oh no couldnt allocate mem\n");
		exit (3);
	}
	while ( (ch != '\n') && (ch != EOF) )
	{
	    if(count ==CUR_MAX-1)
	    { // expand if needed
	      CUR_MAX *= 2; // expand to double the current size of anything similar.
	      count = 0;
	      (*file_name) = (char*)realloc((*file_name), CUR_MAX); // re allocate memory.
		  if((*file_name)==NULL)
			{
				printf("oh no couldnt allocate mem\n");
				exit (3);
			}
	    }
		(*file_name)[length] = ch; // stuff in buffer.
	    ch = getc(files_to_test); // read from stream.
	    length++;
	    count++;
	}
	(*file_name)[length]=0;
	(*output_log_file_path)=(char*) malloc(sizeof(char)*(strlen((*file_name)) +strlen(output_files_directory)+ 10));
	if ((*output_log_file_path) == NULL){
			printf("ERROR - MEM allocation failed\n");
			exit(1);
		}
		token=(char*) malloc(sizeof(char) * strlen(*file_name)+10);
		strncpy(token,*file_name,FindLocationOfExtension(*file_name));
		token[FindLocationOfExtension(*file_name)]=0;
		strcpy((*output_log_file_path),output_files_directory);
		strcat((*output_log_file_path),"\\");
		strcat((*output_log_file_path), token);
		strcat((*output_log_file_path),"_log.txt");
		printf("%s\n", *output_log_file_path);
}
