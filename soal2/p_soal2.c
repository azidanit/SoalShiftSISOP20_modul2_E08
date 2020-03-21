 
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>


int main(int argc, char **argv) {

    pid_t pid, sid, parent_id;        // Variabel untuk menyimpan PID
    parent_id = getpid();
     // Menyimpan PID dari Child Process

    pid = fork();
    /* Keluar saat fork gagal
    * (nilai variabel pid < 0) */
    if (pid < 0) {
      printf("error");
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

    FILE *killer;
    int status;
    killer = fopen("/home/daffa/Desktop/Modul2/killer.sh", "w");

    if (strcmp(argv[1], "-a") == 0)
      fprintf(killer, "#!/bin/bash\nkill -9 -%d\n rm killer", getpid());
    else if (strcmp(argv[1], "-b") == 0)
    {
      fprintf(killer, "#!/bin/bash\nkill %d\n rm killer", getpid());
    }
    if (fork() == 0)
    {
      char *argv[] = {"chmod", "+x", "/home/daffa/Desktop/Modul2/killer.sh", NULL};
      execv("/bin/chmod", argv);
    }
    else
    {
      if(fork() == 0)
      {
        while ((wait(&status)) > 0);    

      char *argv[] = {"mv", "/home/daffa/Desktop/Modul2/killer.sh", "/home/daffa/Desktop/Modul2/killer", NULL};
      execv("/bin/mv", argv);
      }
    }
    fclose(killer);

   // int counter = 0,fcounter = 0,zcounter = 0;

  while (1) {
    
    time_t tme = time(NULL);
    struct tm tm = *localtime(&tme);

    char dirName[100],dirpName[100];
    //path dir
    char dir[100] = "/home/daffa/Desktop/Modul2/soal3/";
    //ambil string nama dir karena nama dir nanti dipakai lagi
    sprintf(dirName, "%04d-%02d-%02d_%02d:%02d:%02d",tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    strcpy(dirpName,dir);
    
    strcat(dirpName, dirName);
    pid_t child_id1,child_id2,child_id3;
  
    child_id1 = fork();
  //  child_id2 = fork();
      int status;
    if(child_id1 == 0)
    {
      child_id2 = fork();
      if(child_id2 == 0)
      {
        char *argv[] = {"mkdir", "-p", dirpName, NULL};
        execv("/bin/mkdir", argv);
      }
      else
      {
      while(wait(&status) > 0 );
      for (int i = 1; i <= 20; i++)
      {
        if(fork()==0)
          {
            //nilai detik Epoch Unix
              int t = (int)time(NULL);
              t = (t % 1000)+100;

              time_t nt = time(NULL);
              struct tm now = *localtime(&nt);

              char download[100];
              sprintf(download, "https://picsum.photos/%d", t);

              char fileName[100];
              sprintf(fileName, "/home/daffa/Desktop/Modul2/soal3/%s/%04d-%02d-%02d_%02d:%02d:%02d", 
              dirName, now.tm_year + 1900, now.tm_mon+1, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec);
              //argumen q = silent

              char *argv[] = {"wget", download, "-qO", fileName, NULL};
              execv("/usr/bin/wget", argv);
              
          }
          //setiap lima detik sampai 20 kali
        sleep(5);
      }
      //while(wait(&status) > 0 );
          char zipped[100];
          //path
          sprintf(zipped, "/home/daffa/Desktop/Modul2/soal3/%s.zip", dirName);
          //qrrm = -q -r -rm
          char *argv[] = {"zip", "-qrrm", zipped, dirpName, NULL};
          execv("/usr/bin/zip", argv);
      }
    }
    else 
    sleep(30);
  }
}