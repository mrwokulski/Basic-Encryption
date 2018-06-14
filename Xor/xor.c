//Wiktor Przybylowski 238230 gr1/INF3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getkey(){

        FILE *key;
        char c;
        static char keytab[32];
        int i=0;

        if((key=fopen("key.txt", "r")) == NULL)
        {
          printf("Nie mozna otworzyc key.txt (brak info ile znakow ma miec linia w plain)  \n");
          exit(1);
        }
        c = fgetc(key);
          while(c != EOF){
            keytab[i] = c;
            c = fgetc(key);
            i++;
          }

        fclose(key);
        return keytab;
}

void setup(){

    FILE *plain,*orig;
    char c,*keyt=getkey();
    int i = strlen(keyt);
    int j=0,k;

      if((orig=fopen("orig.txt", "r")) == NULL)
      {
        printf("Nie mozna otworzyc pliku orig.txt  \n");
        exit(1);
      }

      if((plain=fopen("plain.txt", "w")) == NULL)
      {
        printf("Nie mozna otworzyc plain.txt  \n");
        exit(1);
      }
      while((c = fgetc(orig)) != EOF){
        if(j != i)
        {
          fprintf(plain, "%c", c);
          j++;
        }
        if(j == i)
        {
          fprintf(plain, "\n");
          j = 0;
        }
      }
  fclose(orig);
  fclose(plain);
}

void encrypt(){

  FILE *plain,*crypto;
  char c,k,*keyt = getkey();
  int j = 0,i =  strlen(keyt), x;

  if((plain=fopen("plain.txt", "rb")) == NULL)
  {
    printf("Nie mozna otworzyc plain.txt (encrypt)\n");
    exit(1);
  }

  if((crypto=fopen("crypto.txt", "wb")) == NULL)
  {
    printf("Nie mozna otworzyc crypto.txt (encrypt)\n");
    exit(1);
  }

  while((c = fgetc(plain))!= EOF){
    if(j != i && c != 10)
    {
      x = (int)c^keyt[j];
      fprintf(crypto, "%d ", x);
      j++;
    }
    if(j == i)
    {
      fprintf(crypto, "\n");
      j = 0;
    }
  }
fclose(plain);
fclose(crypto);
}

void decrypt(){

  FILE *crypto, *decrypt;
  char c, k, *keyt=getkey();
  int j=0, i=strlen(keyt);
  int cryptarray[i];
  char decryptarray[i],pass[i];

  for(int n = 0; n < i; n++){
    pass[n] = '0';
  }

  if((decrypt=fopen("decrypt.txt", "wb")) == NULL)
  {
    printf("Nie mozna otworzyc decrypt.txt (decrypt)\n");
    exit(1);
  }

  if((crypto=fopen("crypto.txt", "rb")) == NULL)
  {
    printf("Nie mozna otworzyc crypto.txt (decrypt)\n");
    exit(1);
  }

  while((fscanf(crypto, "%d ", &cryptarray[j])) != EOF){
    if(j != i)
    {
      if(pass[j] != '0') fprintf(decrypt, "%c", (cryptarray[j]^pass[j]));
      else{
          if(cryptarray[j] >= 64)
          {
            decryptarray[j] = (char)(cryptarray[j]^32);
            fprintf(decrypt,"%c", decryptarray[j]);
            if(pass[j] == '0') pass[j]=decryptarray[j];
          }
          else fprintf(decrypt,"_");

        }
        j++;
    }
    if(j == i)
    {
      j = 0;
      fprintf(decrypt,"\n");
    }
  }
  for(int l = 0; l < i ; l++){
    printf("%c", pass[l]);
  }
  printf("\n");

  fclose(decrypt);
  fclose(crypto);
}

void menu(int opcja){

  if(opcja == 'p') setup();

  if(opcja == 'e') encrypt();

  if(opcja == 'k') decrypt();
}



int main(int argc, char *argv[]){

char opcja;

  if(argc == 2)
  {
      if(strcmp(argv[1], "-p") == 0)
      opcja = 'p';
      else if(strcmp(argv[1], "-e") == 0)
      opcja = 'e';
      else if(strcmp(argv[1], "-k") == 0)
      opcja = 'k';
      else printf("\nBrak takiej opcji.\n");
  }

  menu(opcja);

  return 0;

}
