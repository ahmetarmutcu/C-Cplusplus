


#define TRUE 1
#define FALSE !TRUE
#define LIMIT 256 // max number of tokens for a command
#define MAXLINE 1500 //Kullanıcı girişinden maksimum karakter sayısı



#define RED   "\x1B[31m"
#define BLU   "\x1B[34m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"


static char* gecerliDizin;
extern char** environ;//cevreleme yapmak için kullandık

void cocuk_Belirteci(int);
int komut_Baslat(char **args, int arkaplan);
void Pipe(char *args[],char* sonraki,int indeks);
void giris_Yonlendirme(char *args[],char* girisDosyasi);
void cikis_Yonlendirme(char *args[],char* cikisDosyasi);
int komut_quit(char **args);
void dosyaOku(char* girisDosyasi);
void SiraliKomutlar(char args[]);



