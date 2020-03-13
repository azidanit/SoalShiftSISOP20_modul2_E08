#include <dirent.h>
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/stat.h>
#include <sys/wait.h>


int Check(const char *path)
{
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISDIR(path_stat.st_mode);
}

int main() 
{ 
    int status;
    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;
  char fileName[100],dirName[100];
    char *fileArgv[100];
    char *fileArgv2[100];
    int counterF = 0, counterD=0;
    for (counterF = 0; counterF <100; counterF++,counterD++){
      fileArgv[counterF] = (char*) malloc (100 * sizeof(char));
      fileArgv2[counterD]= (char*) malloc (100 * sizeof(char));
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
        if (!strcmp (entry->d_name, ".")) continue;
          if (!strcmp (entry->d_name, ".."))continue;
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
          strcpy(fileName,"/home/daffa/modul2/jpg/");
          strcat(fileName, entry->d_name);
          // strcat(fileName, " ");
          
          strcpy(fileArgv[counterF],fileName);
          counterF++;
        } else if(entry->d_type == 4){
          
          strcpy(dirName,"/home/daffa/modul2/jpg/");
          strcat(dirName, entry->d_name);
          
          strcpy(fileArgv2[counterF],dirName);
          counterD++;
        }
  }
  strcpy(fileArgv[counterF],"/home/daffa/modul2/sedaap/");
  strcpy(fileArgv2[counterD],"/home/daffa/modul2/indomie/");
  fileArgv[counterF+1] = NULL;
  fileArgv2[counterD+1] = NULL;

closedir(dirp);

    pid_t child_id1,child_id2,child_id3, child_id4, child_id5;
    child_id1 = fork();
    child_id2 = fork();
    child_id3 = fork();
    child_id4 = fork();
    child_id5 = fork();
    if (child_id1 == 0) 
    {   
      char *argv[] = {"mkdir", "-p", "/home/daffa/modul2/indomie", NULL};
      execv("/bin/mkdir", argv);
    }
      if (child_id2 == 0)      
    {
      
      while ((wait(&status)) > 0);
      sleep(5);

      char *argv[] = {"mkdir", "-p", "/home/daffa/modul2/sedaap", NULL};
      execv("/bin/mkdir", argv);
    }
    if(child_id3 == 0)
    {
      while ((wait(&status)) > 0);
  char *argv[] = {"unzip", "/home/daffa/Downloads/jpg.zip", "-d", "/home/daffa/modul2/",NULL};
      execv("usr/bin/unzip", argv);
    }
    if(child_id4 == 0 && child_id3 > 0 && child_id2 > 0 && child_id1 > 0){
      execv("/bin/mv",fileArgv);
    }
    if (child_id5 == 0 && child_id4 > 0 && child_id3 > 0 && child_id2 > 0 && child_id1 > 0)
    {
      execv("/bin/mv",fileArgv2);
    }
    
    
}  