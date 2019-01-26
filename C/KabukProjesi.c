
	
	
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <string.h>
	#include <sys/types.h>
	#include <signal.h>
	#include <sys/wait.h>
	#include <fcntl.h>
	#include <termios.h>
	#include <time.h>
	#include "KabukProjesi.h"
	
	
	char *builtin_str[]={
	"quit"
	};
	
	int (*builtin_func[])(char**) = {
	&komut_quit
	};
	
	int komut_builtins_numarasi()
	{
	return sizeof(builtin_str)/sizeof(char *);
	}
	int komut_quit(char **args)
	{
	int durumu;
	while (!waitpid(-1,&durumu,WNOHANG)){}
	exit(0); 
	
	}
	
	#define AUXMAX 256
	int Komut_Isleyici(char * args[])
	{
	int i=0;
	int j=0;
	int dosyaTaniticisi;
	int standartCikis;
	int aux;
	char *args_aux[AUXMAX];
	
	int arkaplan = 0;
	int durum;

	while(args[j] != NULL)
	{
		if ((strcmp(args[j],">") == 0) || (strcmp(args[j],"<") == 0) || (strcmp(args  			[j],"&") == 0)|| (strcmp(args[j],"|") == 0)|| (strcmp(args[j],";") == 0) )
		{
			break;
		}
		args_aux[j] = args[j];
		j++;
	}
	args_aux[j]=NULL;
	
	int m;
	if (args[0] == NULL)
	{
		return 1;
	}
	for (m = 0 ; m < komut_builtins_numarasi() ; m++)
	{
		if (strcmp(args[0],builtin_str[m])==0)
		{
			(*builtin_func[m])(args);
			return 1;
		}
	}
	while (args[i] != NULL && arkaplan == 0)
	{
		if (strcmp(args[i],"&") == 0)
		{
			arkaplan = 1;
		}
		else if (strcmp(args[i],"<") == 0)
		{
			if (args[i+1] == NULL )
			{
				printf ("Yeterli Arguman Yok\n");
			}
			giris_Yonlendirme(args_aux,args[i+1]);
			if(strcmp(args[i-1],"increment")==0)
			{
				dosyaOku(args[i+1]);
	
			}
			
			return 1;
		}
		else if (strcmp(args[i],">") == 0)
		{
			if (args[i+1] == NULL )
			{
				printf ("Yeterli Arguman Yok\n");
			}
			cikis_Yonlendirme(args_aux,args[i+1]);
			return 1;
		}
		else if (strcmp(args[i],"|") == 0)
			{
				int b=0;
				int sayac=0;
				char dosya[MAXLINE]="";
				while(args[b]!=NULL) {
					
					strcat(dosya,args[b]);
					if(strcmp(args[b],"|")==0)
					{
						sayac=sayac+1;
					}
					b++;
				}
				Pipe(args_aux,args[i+1],sayac);
				if (args[i+1] == NULL )
				{
					printf ("Yeterli Arguman Yok\n");
				}
		
				return 1;
			}
			
			else if (strcmp(args[i],";") == 0)
			{
				int k=0;
				char dosya[MAXLINE]="";
				while(args[k]!=NULL)
				{
					
					strcpy(dosya,args[k]);
					SiraliKomutlar(dosya);
					k++;
				}
				
		
				return 1;
			}
	
		i++;
	}
	args_aux[i]==NULL;
	komut_Baslat(args_aux,arkaplan);
	
	return 1;
	}

	void SiraliKomutlar(char args[])
	{
		char *pch; 
		const char s[9]="echo;slp";
		int zaman=0;
		pch=strtok(args,s);// parcalama icin kullanilan ayırıcılar (delimiters)
		while (pch!=0)
  		{
  			
  			printf("%s\n",pch);
  			zaman=atoi(pch);
  			sleep(zaman);	
    		        pch = strtok (NULL, s);
		
	        }	
        }	

	void Pipe(char *args[], char* komut,int indeks) 
	{		
	int fd[2];
	int nbytes;
	pid_t childpid;
	
	char readbuffer[MAXLINE];
	
	pipe(fd);
	if ((childpid = fork()) == -1) {
		perror("fork");
		exit(1);
	}
	if (childpid == 0) {
			close(fd[0]);
			write(fd[1], args[1], (strlen(args[1]) +1));
			exit(0);
		
	} else {
		/* Parent process closes up output side of pipe */
		close(fd[1]);
		nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
		int sayi=atoi(readbuffer);	
		sayi=sayi+indeks;
		printf("%d\n",sayi);		
			}
	
	}
	void dosyaOku(char *girisDosyasi)
	{
	 FILE *ptDosya;
	 char ch;
	 char toplam[MAXLINE];
	 int i=0;
	
	 ptDosya = fopen(girisDosyasi, "r"); // dosyayı aç:
	 int sonuc=0;
	 	
	 // dosya sonuna gelene kadar tüm karakterleri oku:
	 do {
	 	ch = getc(ptDosya);// okunan karakterleri ekrana yaz:
	   toplam[i]=ch;
		i++;
	    
	 } while (ch != EOF);
	  	
	  sonuc=atoi(toplam);
	  sonuc=sonuc+1;
	  printf("%d\n",sonuc);
	  fclose(ptDosya); // dosyayı kapat
	  
	}
	
	void giris_Yonlendirme(char *args[],char* girisDosyasi)
	{
	pid_t pid;
	if (!(access (girisDosyasi,F_OK) != -1))
	{ 
	
		printf("Hata: %s adinda bir giris dosyası bulunamadi\n",girisDosyasi);
		return;
	}
	int err=-1;
	int dosyaTaniticisi;
	if((pid=fork()) == -1)
	{
		printf("Cocuk(Child) olusturulamadi\n");
		return;
	}
	if (pid==0)
	{
		dosyaTaniticisi=open(girisDosyasi, O_RDONLY, 0600);
		dup2(dosyaTaniticisi,STDIN_FILENO);
		close(dosyaTaniticisi);
	
		if(execvp(args[0],args)==err)
		{
			printf("err");
			kill(getpid(),SIGTERM);
		}
	}
	waitpid(pid,NULL,0);
	}
	
	void cikis_Yonlendirme(char *args[],char* cikisDosyasi)
	{
	pid_t pid;
	int err=-1;
	int dosyaTaniticisi;
	if((pid=fork()) == -1)
	{
		printf("Cocuk(Child) olusturulamadi\n");
		return;
	}
	if (pid==0)
	{
		dosyaTaniticisi=open(cikisDosyasi, O_CREAT | O_TRUNC | O_WRONLY, 0600);
		dup2(dosyaTaniticisi,STDOUT_FILENO);
		close(dosyaTaniticisi);
	
		if (execvp(args[0],args)==err)
		{
			printf("err");
			kill(getpid(),SIGTERM);
		}
	}
	waitpid(pid,NULL,0);
	}

	int komut_BaslatmaBg(char **args)
	{
	pid_t pid;
	int durum;
	
	struct sigaction act;
	act.sa_handler = cocuk_Belirteci;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_NOCLDSTOP;
	if (sigaction(SIGCHLD,&act,NULL)<0)
	{
		fprintf(stderr,"sigaction bulunamadı\n");
		return 1;
	}
	pid=fork();
	switch(pid)	
	 {
	 	case 0:
	 	if (execvp(args[0],args) == -1)
		{
			printf("Hata: Komut bulunamadı");
			kill(getpid(),SIGTERM);
		}
		//exit(EXIT_FAILURE);
	 	break;
	 	case -1:
	 		perror("Hata");
	 		break;
	 	default:
	 	printf("Proses PID:%d Degeriyle Olusturuldu\n",pid);
	 		
	 }
	 return 1;
	}
	
	int komut_Baslat(char **args,int arkaplan)
	{
	
	if (arkaplan==0)
	{
		pid_t pid;
		int durum;
		pid=fork();
		if (pid == 0)
		{
			if (execvp(args[0],args) == -1)
			{
				printf("Komut bulunamadı");
				kill(getpid(),SIGTERM);
			}
		}
		else if (pid < 0)
		{
			perror("Hata");
		}
		else
		{
			waitpid(pid,NULL,0);
		}
	}
	else
	{
		komut_BaslatmaBg(args);
	}
	return 1;
	}
	
	void cocuk_Belirteci(int belirtecNo)
	{
	  	int durum, cocuk_deger,pid;
	pid = waitpid(-1, &durum, WNOHANG);
	if (pid > 0)//Arkaplanda çalışmayan processler için bu değer -1 olacağından bu kontrol yapılmıştır.
	{
		if (WIFEXITED(durum))	//Çocuk Normal Bir Şekilde mi Sonlandı ?
	    {
	        cocuk_deger = WEXITSTATUS(durum); // Çocuğun durumu alındı.
	        printf("[%d] retval : %d \n",pid, cocuk_deger); //Çocuğun pid'i ve durumu ekrana yazdırılır.
	    }
	}
	}
	
	void Prompt()
	{
		char hostadi[1500] = "";
		gethostname(hostadi, sizeof(hostadi));
		getcwd(gecerliDizin, 1024);
		printf(BLU "%s:" WHT "*>* ",hostadi);
	}
	
	int main (int argc, char **argv, char **envp)
	{
	printf("\t    C İşletim Sistemi Shell  \n");
	char satir[MAXLINE];
	char *belirtecler[LIMIT];
	int belirtecNumarasi;
	int durum=1;
	environ=envp;
	do {
		
		Prompt();
		memset(satir,'\0',MAXLINE);
		fgets(satir,MAXLINE,stdin);
		if((belirtecler[0] = strtok(satir," \n\t")) == NULL) continue;
		belirtecNumarasi = 1;
		while((belirtecler[belirtecNumarasi] = strtok(NULL, " \n\t")) != NULL)
		belirtecNumarasi++;
		Komut_Isleyici(belirtecler);
	
	    }
	 while(durum);
	
	}
