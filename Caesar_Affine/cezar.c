#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int upperS=65;
const int upperL=90;
const int lowerS=97;
const int lowerL=122;
int akey[12]={1,3,5,7,9,11,15,17,19,21,23,25};
int arkey[12]={1,9,21,15,3,19,7,23,11,5,17,25};

//a'*a = 1(mod26)
int reverse(int val){
  int x = 0;
  do {
    x++;
  } while ((val * x) % 26 != 1);
  return x;
}

//NWD()
int nwd(int a, int b){
  while(a != b)
    if(a < b) b -= a; else a -= b;
  if(a==1)
    return 1;
  return 0;
}

//funkcja szyfrujaca E(x,k)=x+k(mod26)
char cezar_s(char c, int k){
  if(c >= upperS && c <= upperL)
  {
    c = (c - upperS + k) % 26 + upperS;
    return c;
  }
  else if(c >= lowerS && c <= lowerL)
  {
    c = (c - lowerS + k) % 26 + lowerS;
    return c;
  }
  else return c;
}

//funkcja deszyfrujaca D(y,k)=y-k(mod 26)
//wszystkie znaki niebedace literkami sa pomijane w szyfrowaniu
char cezar_d(char c, int k){

  if(c >= upperS && c <= upperL)
  {
    c = (c - upperS - k) % 26;
    if(c<0) c += upperS + 26;
    else c += upperS;

    return c;
  }

  else if(c >= lowerS && c <= lowerL)
  {
    c = (c - lowerS - k) % 26;
    if(c<0) c += lowerS + 26;
    else c += lowerS;

    return c;
  }

  else return c;
}

//funkcja szyfrujaca E(x,a,b)=a*x+b(mod 26)
//wszystkie znaki niebedace literkami sa pomijane w szyfrowaniu
char afi_s(char c, int a, int b){

  if(c >= upperS && c <= upperL)
  {
    c = ((a * (c - upperS)) + b ) % 26 + upperS;
    return c;
  }

  else if(c >= lowerS && c <= lowerL)
  {
    c = ((a * (c - lowerS)) + b ) % 26 + lowerS;
    return c;
  }

  else return c;
}

//funkcja deszyfrujaca D(y,a,b)=a'*(x-b)(mod26), gdzie a*a'=1(mod26)
char afi_d(char c, int a, int b){
  int ar = reverse(a);

  if(c >= upperS && c <= upperL)
  {
    c = (ar * (c - upperS - b)) % 26;
    if(c<0) c += upperS + 26;
    else c += upperS;
    return c;
  }

  else if(c >= lowerS && c <= lowerL)
  {
    //printf("D(y,a,b)= %d * (%d - %d)\n",ar,c-97,b);
    c = (ar * (c - lowerS - b)) % 26;
    if(c<0) c += lowerS + 26;
    else c += lowerS;
    return c;
  }

  else return c;
}

void cezar_k(char c1, char c2){
  FILE *newkey;
  int pos1 = -1;
  int pos2 = -1;
  newkey=fopen("new-key.txt","w");

  if(c1 >= upperS && c1 <= upperL && c2 >= lowerS && c2 <= lowerL)
  {
    printf("Klucz nie istnieje. (cezar)\n");
    exit(1);
  }
  if(c2 >= upperS && c2 <= upperL && c1 >= lowerS && c1 <= lowerL)
  {
    printf("Klucz nie istnieje. (cezar)\n");
    exit(1);
  }

  for(int i=0; i<26; i++){
    if(c1 == upperS + i || c1 == lowerS + i)
      pos1 = i;
    if(c2 == upperS + i || c2 == lowerS + i)
      pos2 = i;
  }

  if(pos1 == -1 || pos2 == -1)
  {
    printf("Klucz nie istnieje. (cezar)\n");
    exit(1);
  }

  if(pos1>pos2)
    fprintf(newkey, "%d\n", pos1-pos2);
  else
    fprintf(newkey, "%d\n", pos2-pos1);

  fclose(newkey);
}

void afi_k(char c1, char c2, char c3, char c4){
  FILE *newkey;
  int i = 0,j = 0, tmp, tmp2, tmp3, tmp4;
  int a = 0, b= 0, a2 = 0, b2 = 0;
  int posx = -1, posy = -1, posx2 = -1, posy2 = -1;
  newkey=fopen("new-key.txt","w");

  if(c1 >= upperS && c1 <= upperL && c2 >= lowerS && c2 <= lowerL)
  {
    printf("Nie mozna zgadnac klucza. (afiniczny) v1\n");
    exit(1);
  }
  if(c2 >= upperS && c2 <= upperL && c1 >= lowerS && c1 <= lowerL)
  {
    printf("Nie mozna zgadnac klucza. (afiniczny) v2\n");
    exit(1);
  }
  if(c3 >= upperS && c3 <= upperL && c4 >= lowerS && c4 <= lowerL)
  {
    printf("Nie mozna zgadnac klucza. (afiniczny) v3\n");
    exit(1);
  }
  if(c4 >= upperS && c4 <= upperL && c3 >= lowerS && c3 <= lowerL)
  {
    printf("Nie mozna zgadnac klucza. (afiniczny) v4\n");
    exit(1);
  }

  for(i=0; i<26; i++){
    if(c1 == lowerS + i || c1 == upperS + i)
      posx = i;
    if(c2 == lowerS + i || c2 == upperS + i)
      posy = i;
    if(c3 == lowerS + i || c3 == upperS + i)
      posx2 = i;
    if(c4 == lowerS + i || c4 == upperS + i)
      posy2 = i;
  }

  if(posx == -1 || posy == -1 || posx2 == -1 || posy2 == -1){
    printf("Klucz nie istnieje. (afiniczny)\n");
    exit(1);
  }
  for(i=0; i<12; i++){
    for(j=0; j<26; j++){
      tmp = ((akey[i]*posx)+j)%26;
      tmp2 = (arkey[i]*(posy-j))%26;
      if(tmp2<0)
        tmp2+=26;
      tmp3 = ((akey[i]*posx2)+j)%26;
      tmp4 = (arkey[i]*(posy2-j))%26;
      if(tmp4<0)
        tmp4+=26;
      if( tmp == posy && tmp2 == posx && tmp3 == posy2 && tmp4 == posx2){
        a = akey[i];
        b = j;
      }
    }
  }
  fprintf(newkey, "%d %d\n", a, b);
  fclose(newkey);
}

void cezar_a(){
  FILE *decrypt, *crypto;
  char c;
  if((decrypt=fopen("decrypt.txt","wb+"))!= NULL)
  {
    for(int i=1; i<26; i++){
     if((crypto=fopen("crypto.txt","r"))!=NULL)
     {
      while ((c = fgetc(crypto)) != EOF)
      {
        c = cezar_d(c,i);
        fprintf(decrypt, "%c", c);
      }
      fprintf(decrypt, "\n");
     }
    }
   }
fclose(crypto);
fclose(decrypt);
}

void afi_a(){
FILE *decrypt, *crypto;
char c;

if((decrypt=fopen("decrypt.txt","wb+"))!=NULL)
 {
   for(int i=0; i<12; i++){
     for(int j=0; j<26; j++){
      if((crypto=fopen("crypto.txt","r"))!=NULL)
      {
        while ((c = fgetc(crypto)) != EOF)
        {
          c = afi_d(c,akey[i],j);
          fprintf(decrypt, "%c", c);
        }
        fprintf(decrypt, "\n");
      }
     }
   }
  }
  fclose(crypto);
  fclose(decrypt);
}

void menu(char szyfr, char opcja){
  int a = 0, b = 0, k = 0;
  char c, c2, c3, c4;
  FILE *plain, *crypto, *decrypt, *key;

  if((key=fopen("key.txt", "r")) == NULL)
  {
    printf("Nie mozna otworzyc pliku z kluczem\n");
    exit(1);
  }

  if(szyfr == 'c')
  {
    fscanf(key,"%d",&k);
    if(k < 1 || k > 25){
      printf("Podano nieprawidlowy klucz.\n");
      exit(1);
    }
  }
  else if(szyfr == 'a')
  {
    fscanf(key,"%d%d",&a,&b);
    if(nwd(a,26) == 0 || b < 0 || b > 25)
    {
      printf("Podano nieprawidlowy klucz.\n");
      exit(1);
    }
  } else{
    printf("Nie ma takiej mozliwosci.");
    exit(1);
  }

  if(opcja == 'e')
  {
    if((plain=fopen("plain.txt", "r")) == NULL)
    {
    printf("Nie mozna otworzyc plain.txt");
    exit(1);
    }

    if((crypto=fopen("crypto.txt", "w")) == NULL)
    {
    printf("Nie mozna otworzyc crypto.txt");
    exit(1);
    }

    while ((c = fgetc(plain)) != EOF)
    {
      if(szyfr == 'c')
        c = cezar_s(c,k);
      if(szyfr == 'a')
         c = afi_s(c,a,b);
      fprintf(crypto, "%c", c);
    }
    fclose(plain);
    fclose(crypto);

} else if(opcja == 'd'){
    if((crypto=fopen("crypto.txt", "r")) == NULL)
    {
    printf("Nie mozna otworzyc plain.txt");
    exit(1);
    }

    if((decrypt=fopen("decrypt.txt", "w")) == NULL)
    {
    printf("Nie mozna otworzyc decrypt.txt");
    exit(1);
    }

    while ((c = fgetc(crypto)) != EOF)
    {
      if(szyfr == 'c')
        c = cezar_d(c,k);
      if(szyfr == 'a')
         c = afi_d(c,a,b);
      fprintf(decrypt, "%c", c);
    }
    fclose(crypto);
    fclose(decrypt);
} else if(opcja == 'j'){
    if((plain=fopen("plain.txt", "r")) == NULL)
    {
    printf("Nie mozna otworzyc plain.txt");
    exit(1);
    }

    if((crypto=fopen("crypto.txt", "r")) == NULL)
    {
    printf("Nie mozna otworzyc crypto.txt");
    exit(1);
    }

    c = fgetc(plain);
    c3 = fgetc(plain);
    c2 = fgetc(crypto);
    c4 = fgetc(crypto);

    if(szyfr == 'c')
      cezar_k(c,c2);
    if(szyfr == 'a')
      afi_k(c,c2,c3,c4);

    fclose(plain);
    fclose(crypto);
  } else if(opcja == 'k'){
    if(szyfr == 'c')
      cezar_a();
    if(szyfr == 'a')
       afi_a();
  } else{
    printf("Nieznana czynnosc...");
    exit(1);
  }

}


int main(int argc, char *argv[]){
  char szyfr, opcja;
  if( argc == 3 )
  {
    if(strcmp(argv[1], "-c") == 0)
      szyfr = 'c';
    else if(strcmp(argv[1], "-a")  == 0)
      szyfr = 'a';
    else printf("Cos poszlo nie tak.\n");

    if(strcmp(argv[2], "-e") == 0)
      opcja = 'e';
    else if(strcmp(argv[2], "-d") == 0)
      opcja = 'd';
    else if(strcmp(argv[2], "-j") == 0)
      opcja = 'j';
    else if(strcmp(argv[2], "-k") == 0)
      opcja = 'k';

    menu(szyfr,opcja);
  }

  return 0;
}
