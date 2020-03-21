# SoalShiftSISOP20_modul2_E08

Penyelesaian Soal Shift Modul 2 SISOP 2020

Achmad Zidan Akbar / 05111840000005

M RIDHO DAFFA ARDISTA / 05111840000065	

<h3>Nomor 1</h3>
Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan
ketentuan sebagai berikut:

 1. Program menerima 4 argumen berupa:
    - Detik: 0-59 atau * (any value)
    - Menit: 0-59 atau * (any value)
    - Jam: 0-23 atau * (any value)
    - iv. Path file .sh
 2. Program akan mengeluarkan pesan error jika argumen yang diberikan tidak sesuai
 3. Program hanya menerima 1 config cron
 4. Program berjalan di background (daemon)
 5. Tidak boleh menggunakan fungsi system()
Contoh: ./program \* 34 7 /home/somi/test.sh
Program dengan argumen seperti contoh di atas akan menjalankan script test.sh setiap
detik pada jam 07:34.

include lib

    #include <sys/types.h>
    #include <sys/stat.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <unistd.h>
    #include <syslog.h>
    #include <time.h>

Fungsi untuk mengechek apakah filenya ada atau tidak

    int Check(const char * filename){
        FILE *file;
        if (file = fopen(filename, "r")){
            fclose(file);
            return 1;
        }
        return 0;
    }


Mengecek input

    int i, input[5], every[5];
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

Syarat input

    if(((input[1] > 60 || input[1] < 0) && every[1] == 0) 
    || ((input[2] > 60 || input[2] < 0) && every[2] == 0)
    || ((input[3] > 24 || input[3] < 0) && every[3] == 0)
    || Check(argv[4]) == 0 )
    {
        printf ("ERROR\n");
        exit(EXIT_FAILURE);
    }

Variabel untuk menyimpan PID
    pid_t pid, sid;

Menyimpan PID dari Child Process
    pid = fork();   

Keluar saat fork gagal (nilai variabel pid < 0)
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
  
Keluar saat fork berhasil (nilai variabel pid adalah PID dari child process)

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

untuk memastikan file dapat dieksekusi

    umask(0);

Program Utama

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

<h3>Nomor 2</h3>

*Note : Fungsi killer belum sesuai pada mode -b.

Include library

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

Program Utama

    pid_t pid, sid;        // Variabel untuk menyimpan PID

    pid = fork();     // Menyimpan PID dari Child Process

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

    FILE *killer;
    int status;
    killer = fopen("/home/daffa/Desktop/Modul2/killer.sh", "w");

    if (strcmp(argv[1], "-a") == 0)
      fprintf(killer, "#!/bin/bash\nkill -9 -%d\n rm killer", getpid());
    else if (strcmp(argv[1], "-b") == 0)
      fprintf(killer, "#!/bin/bash\nkill %d\n rm killer", getpid());
    
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

     while (1) {
    
    time_t tme = time(NULL);
    struct tm tm = *localtime(&tme);

    char dirName[100],dirpName[100];
    
    char dir[100] = "/home/daffa/Desktop/Modul2/soal3/";
    sprintf(dirName, "%04d-%02d-%02d_%02d:%02d:%02d",tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    strcpy(dirpName,dir);
    strcat(dirpName, dirName);
    pid_t child_id,child_id2,child_id3;
    child_id = fork();
    int status;

membuat directory dari argumen dirpname

    char *argv[] = {"mkdir", "-p", dirpName, NULL};
    execv("/bin/mkdir", argv);


Mendownload gambar dengan ukuran square Epoch Unix dan memasukkannya dalam folder sesuai timestamp

    int t = (int)time(NULL);
    t = (t % 1000)+100;

    time_t nt = time(NULL);
    struct tm now = *localtime(&nt);

    char download[100];
    sprintf(download, "https://picsum.photos/%d", t);

    char fileName[100];
    sprintf(fileName, "/home/daffa/Desktop/Modul2/soal3/%s/%04d-%02d-%02d_%02d:%02d:%02d", dirName, now.tm_year + 1900, now.tm_mon+1, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec);

    char *argv[] = {"wget", download, "-qO", fileName, NULL};
    execv("/usr/bin/wget", argv);

Untuk membuat zip file dari directory yang telah ada

    char zipped[100];
    sprintf(zipped, "/home/daffa/Desktop/Modul2/soal3/%s.zip", dirName);
    char *argv[] = {"zip", "-qrrm", zipped, dirpName, NULL};
    execv("/usr/bin/zip", argv);


<h3>Nomor 3</h3>

Jaya adalah seorang programmer handal mahasiswa informatika. Suatu hari dia
memperoleh tugas yang banyak dan berbeda tetapi harus dikerjakan secara bersamaan
(multiprocessing).

1. Program buatan jaya harus bisa membuat dua direktori di “/home/[USER]/modul2/”. Direktori yang pertama diberi nama “indomie”, lalu lima detik kemudian membuat direktori yang kedua bernama “sedaap”.
2. Kemudian program tersebut harus meng-ekstrak file jpg.zip di direktori “/home/[USER]/modul2/”. Setelah tugas sebelumnya selesai, ternyata tidak hanya itu tugasnya. 
3. Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (di
dalam direktori “home/[USER]/modul2/jpg/”) harus dipindahkan sesuai dengan
pengelompokan, semua file harus dipindahkan ke
“/home/[USER]/modul2/sedaap/” dan semua direktori harus dipindahkan ke
“/home/[USER]/modul2/indomie/”.
4. Untuk setiap direktori yang dipindahkan ke “/home/[USER]/modul2/indomie/”
harus membuat dua file kosong. File yang pertama diberi nama “coba1.txt”, lalu
3 detik kemudian membuat file bernama “coba2.txt”.
(contoh : “/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt”).

Karena Jaya terlalu banyak tugas dia jadi stress, jadi bantulah Jaya agar bisa membuat
program tersebut.

Program : 

lib yang dibutuhkan 

    #include <dirent.h>
    #include <unistd.h> 
    #include <stdio.h> 
    #include <stdlib.h> 
    #include <string.h> 
    #include <sys/stat.h>
    #include <sys/wait.h>

mengecek direcotry pada suatu path

    int Check(const char *path)
    {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
    }

program utama

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
  
    