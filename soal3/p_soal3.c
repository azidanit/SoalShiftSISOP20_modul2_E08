#include <dirent.h>
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/stat.h>
#include <sys/wait.h>


int main() 
{ 
    int file_count = 0,i;
    DIR * dirp;
    struct dirent * entry;
  char fileName[100],dirName[100],filedir[100],filedir2[100];
    char *fileArgv[100];
    char *fileArgv2[100];
    char *fileArgv3[100];
    int counterF = 0, counterD=0;
    for (counterF = 0; counterF <100; counterF++,counterD++){
      fileArgv[counterF] = (char*) malloc (100 * sizeof(char));
      fileArgv2[counterD]= (char*) malloc (100 * sizeof(char));
      fileArgv3[counterD]= (char*) malloc (100 * sizeof(char));
    }
    strcpy(fileArgv[0],"mv");
    strcpy(fileArgv2[0], "mv");
    // strcpy(fileArgv2[1],);
    counterF = 1;
    counterD = 1;
  strcpy(fileName,"");
  strcpy(dirName,"");
    dirp = opendir("/home/daffa/Downloads/jpg/"); /* There should be error handling after this */
    while ((entry = readdir(dirp)) != NULL) {
  
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
          strcpy(fileName,"/home/daffa/modul2/jpg/");
          strcat(fileName, entry->d_name);
          
          strcpy(fileArgv[counterF],fileName);
          counterF++;
        } else if(entry->d_type == 4){
           
         if (!strcmp (entry->d_name, ".")) continue;
          if (!strcmp (entry->d_name, ".."))continue;
          
          strcpy(fileArgv3[counterD-1],entry->d_name);
          strcpy(dirName,"/home/daffa/modul2/jpg/");
          strcat(dirName, entry->d_name);
          strcpy(fileArgv2[counterD],dirName);
          counterD++;
          
          
        }
       
  }
  
  strcpy(fileArgv[counterF],"/home/daffa/modul2/sedaap/");
  strcpy(fileArgv2[counterD],"/home/daffa/modul2/indomie/");
   
  fileArgv[counterF+1] = NULL;
  fileArgv2[counterD+1] = NULL;
  

closedir(dirp);

    pid_t child_id1,child_id2,child_id3, child_id4,child_id5;
    int status;
    child_id1 = fork();
    
    if (child_id1 == 0) 
    {   
      char *argv[] = {"mkdir", "-p", "/home/daffa/modul2/indomie", NULL};
      execv("/bin/mkdir", argv);
    }
    else 
    {
       // while(wait(&status) >0 );
        child_id2 = fork();
    }
    if (child_id2 == 0)      
    {
      sleep(5);
      char *argv[] = {"mkdir", "-p", "/home/daffa/modul2/sedaap", NULL};
      execv("/bin/mkdir", argv);
    }
    else
    {
        //while(wait(&status) >0 );
        child_id3 = fork();
    }
    if(child_id3 == 0)
    {
      char *argv[] = {"unzip", "/home/daffa/Downloads/jpg.zip", "-d", "/home/daffa/modul2/",NULL};
      execv("/usr/bin/unzip", argv);
    }
    else 
    {
      while(wait(&status) >0 );
        child_id4 = fork();
    }
     
     if(child_id4 == 0){
       execv("/bin/mv",fileArgv);
     }
    else
    {
       //while(wait(&status) >0 );
        child_id5 = fork();
    }
    if (child_id5 == 0)
    {
       execv("/bin/mv",fileArgv2);
    }
    while(wait(&status) >0 );
    if(child_id5 > 0)
    {
     
      for (i = 0; i < counterD-1 ; i++)
      {
        sprintf(filedir,"/home/daffa/modul2/indomie/%s/coba1.txt",fileArgv3[i]);
        sprintf(filedir2,"/home/daffa/modul2/indomie/%s/coba2.txt",fileArgv3[i]);
        //printf("%s\n",filedir);

        
         if(fork() == 0)
        {
          char *argv[] = {"touch", filedir,NULL};
        execv("/usr/bin/touch",argv);
        }
        sleep(3);
         if(fork() == 0)
        {
        char *argv[] = {"touch", filedir2,NULL};
        execv("/usr/bin/touch",argv);
        }

      }
      

      
    }
  
    
}  