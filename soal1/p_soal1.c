#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>

// Fungski untuk mengechek apakah filenya ada atau tidak
int Check(const char * filename){
    FILE *file;
    if (file = fopen(filename, "r")){
        fclose(file);
        return 1;
    }
    return 0;
}
//Fungsi untuk mendeteksi *
int Notnumber(char argv[])
{
    if (argv[0] == '*') return 1;
  return 0;
}

int main(int argc, char **argv) 
{ 


    int i, input[5], every[5];

//Mengecek input
	for(i = 1; i < argc-1; i++)
    {
        if(Notnumber(argv[i])) 
        {
            every[i] = 1;
        }
        else { input[i] = atoi(argv[i]);
        every[i]=0;
        }
        
    }
  //Syarat input
    if(((input[1] > 60 || input[1] < 0) && every[1] == 0) 
    || ((input[2] > 60 || input[2] < 0) && every[2] == 0)
    || ((input[3] > 24 || input[3] < 0) && every[3] == 0)
    || Check(argv[4]) == 0 )
    {
        printf ("ERROR\n");
        exit(EXIT_FAILURE);
    }
  // Variabel untuk menyimpan PID
    pid_t pid, sid;
    // Menyimpan PID dari Child Process
    pid = fork();   

  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }
  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while (1) 
  {
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
        int total = 0;
        //Mencari rentatng waktu
    if(!every[1])
    {
        if(input[1]<tm.tm_sec) total += (60-(tm.tm_sec-input[1]));
        else total+=input[1]-tm.tm_sec;        
    }
    if(!every[2])
    {
        if(input[2]<tm.tm_min) total += (60-(tm.tm_min-input[2]))*60;
        else (total+=input[2]-tm.tm_min)*60;
    }
    if(!every[3])
    {
        if(input[3]<tm.tm_hour) total += (24-(tm.tm_hour-input[3]))*60*60;
         else total+=(input[3]-tm.tm_hour)*60*60;
    }

      //Crontab
      if ((input[1] == tm.tm_sec || every[1]) && (input[2] == tm.tm_min || every[2]) && (input[3] == tm.tm_hour|| every[3]) )
      {
        pid_t child_id;
        child_id = fork();
        if (child_id == 0)
        {    
          char *bs[] = {"bash", argv[4], NULL};
          execv("/bin/bash", bs);
        }
        sleep(1);
      }
      //Sleep hingga pertintah akan dipanggil
      else
      {
          sleep(total-1);
      }
        
  } 
} 
