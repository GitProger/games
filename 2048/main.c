#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>
 
unsigned int random() {
    static int is_init = 0;
    static unsigned long long x = 0;
    if (!is_init) {
        x = time(NULL);
        is_init = 1;
    }    
    x = (0x343FDULL * x + 0x269EC3ULL) % 0xFFFFFFFFULL;
    return (x & 0x3FFF8000) >> 15;
}

unsigned int map[12][12]; /* our map */
unsigned int score = 0, size = 5; /* size is length of border of the map */
bool color = false;

setpos(int x, int y) { /* the function sets cursor position on the screen */
 COORD C = {x, y};
 SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), C);
}

void setcur(bool mode){
HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO structCursorInfo;
GetConsoleCursorInfo(handle,&structCursorInfo);
structCursorInfo.bVisible= mode?TRUE:FALSE;
SetConsoleCursorInfo(handle,&structCursorInfo);
}

void setscr(int a) {
 char str[16] = "dataX.dat";
 str[4] = (char)(size - 4 + '0');
 FILE *f = fopen(str, "w");
 fprintf(f, "%d", a);
 fclose(f);
}

int gethighscore() {
 int r = 0;
 char str[16] = "dataX.dat";
 str[4] = (char)(size - 4 + '0');
 FILE *f = fopen(str, "r");
 if(!f) { setscr(0); }
 else { 
     fscanf(f, "%d", &r);
 }
  fclose(f);
 return r;
}

bool check() { /* this function returns false if there are no "0" in the map */
 int i,j;      /* else returns true */
 for (i = 0; i < size; i++)
  for (j = 0; j < size; j++)
   if (!map[i][j]) return true;
 return false;
}

void create() { /* this function create new element in the map if it is */
int i, j;       /* possible */
 if(!check()) return;
 
 do {
  i = random() % size;
  j = random() % size;
 } while( map[i][j] );

   map[i][j] = 2;
 if(!(random() % 11))
   map[i][j] = 4;
}

void printwithtab(int tab,int x) { /* this function prints x with tab - */
 int cp = x, rl = 1;               /* amount numerals in x whitespaces */
 while(x /= 10) rl++;
 for(x = 0; x < tab - rl; x++) putchar(' ');
 printf("%d", cp);
}

bool ispowof2(int x) {
 while(!( x&1 )) x >>= 1;
 return x == 1;
}

int log2(int x) {
 if(!x) return -1; 
 if(!ispowof2(x)) return -1;
 int r = 0;
 while(!( x&1 )){
     x >>= 1;
     r++;
 }
 return r;
}

void printcolorint(int len, int x) {
 HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
 CONSOLE_SCREEN_BUFFER_INFO ConsoleInfo;
 GetConsoleScreenBufferInfo(h, &ConsoleInfo);
 WORD OriginalColors = ConsoleInfo.wAttributes; 
 SetConsoleTextAttribute(h, log2(x)+1); 
 printwithtab(len, x);
 SetConsoleTextAttribute(h, OriginalColors); 
}

void print() { /* this function print map with net */
int max = 0, len = 1, i, j, k;
 for (i = 0; i < size; i++)
  for (j = 0; j < size; j++)
   if(max < map[i][j]) max = map[i][j];

while(max /= 10) len++;

  setpos(0,0);
 for (i = 0; i < size; i++) {
  printf("%c\n%c\n", 195, 179);
 }

 for (i = 0; i < size; i++) {
     for (j = 0; j < size; j++) {
         setpos( i * (len + 1) + 1, j * 2 + 1);
         if(map[i][j]) {     
  		     if(color)   
                 printcolorint(len, map[i][j]);
             else
  		         printwithtab (len, map[i][j]);
         } else 
          for (k = 0; k < len; k++) putchar(' ');
             putchar(179);
		     setpos( i * (len + 1) + 1, j * 2);
             for (k = 0; k < len; k++) putchar(196);
             putchar(197);
     }
 }
 setpos(0, 2*size);
 putchar(192);
 for(i = 0; i < size; i++){
     for(j = 0; j < len; j++)
         putchar(196);
     putchar(193);
 }
}

bool win() {
int i, j;
 for (i = 0; i < size; i++)
  for (j = 0; j < size; j++)
   if(map[i][j] == 2048) return true;
 return false;
}

bool lost( ) { /* this function check opportunity of step */ 
 if(check()) return false;
 int i, j;
  for (i = 0; i < size-1; i++)
     for (j = 0; j < size-1; j++)
         if (map[i][j] == map[i+1][j] || map[i][j] == map[i][j+1]) 
             return false;
  for (i = 0; i < size-1; i++)
     if (map[i][size-1] == map[i+1][size-1])
         return false;
  for (i = 0; i < size-1; i++)
     if(map[size-1][i] == map[size-1][i+1])
         return false;
 return true;
}

void move_right();
void move_left();
void move_up();
void move_down();

int main(){
 system("chcp 866"); /* nessasary to use graphics */

char key = 0;
int i, j, dir;
char *str;
int *buffer;
bool docreate;
start: 
 system("cls");
 for (i = 0; i < size; i++)    /*                    */
     for (j = 0; j < size; j++)/* map initialization */ 
         map[i][j] = 0;     /*                    */
size = 5; /* default */
score = 0;
setcur(true);
str = (char*)malloc(2048);

printf("Size of map (4-12): ");
gets(str);
sscanf(str,"%d",&size);
if( size < 4 || size > 12 ) size = 5;
system("cls");

printf("Use colors? 0 - no, 1 - yes.\n");
*str = 0;
while(*str != '1' && *str != '0') gets(str);
 if(*str == '0')
  color = false;
 else
  color = true;
system("cls");

free(str);

 system("cls");
 create();
 create();
 print();
while(true) {

setcur(false);
/* = copy array = */
buffer = (int*)calloc(size*size,sizeof(int));
for(i = 0; i<size; i++)
   for(j = 0; j<size; j++)
       *(buffer + size*i + j) = map[i][j];
/* = copy array = */

docreate = false;
/* = key code checking = */ 
  key = getch();
  switch(key) {
      case(  27 ): goto onExit;
      case(  13 ): goto start;
      case( 'w' ): move_up    ( ); break;
      case( 's' ): move_down  ( ); break;
      case( 'a' ): move_left  ( ); break;
      case( 'd' ): move_right ( ); break;
      default    : {
                    key = getch( );
                   switch(key) {
                       case(  72 ): move_up    ( ); break;
                       case(  80 ): move_down  ( ); break;
                       case(  75 ): move_left  ( ); break; 
                       case(  77 ): move_right ( ); break;
                       default    :              continue;
                   }
      }
  }
/* = key code checking = */

for(i = 0; i<size; i++)
   for(j = 0; j<size; j++)
      if( *(buffer + size*i + j) != map[i][j]) {
        docreate = true;
        break;
      }
 if(docreate) {
     create();
     print();
 }
free(buffer);

    if(score > gethighscore() ) setscr(score);
  if(win()) {
   setpos(0,size*2+4);
   printf("You won!");
  }
  if(lost()) { /* check possiblity of step */
      setpos(0,size*2+3);
      printf("Game over! Press \"Enter\".");
      while(getch() != 13);/* waiting for pressed "enter" */
      system("cls");
      score = 0;
      goto start;
  }
   setpos(0,size*2+1); printf("Score: %d", score);
   setpos(0,size*2+2); printf("High score: %d", gethighscore());
}
 onExit:
 setcur(true);
 return 0;
}
/*========================================================= */
/*= These 4 functions move map to corresponding direction =*/
/*=========================================================*/
void move_right() {
 int i, j, k;
 bool dogoto = true, full = false;
/* offset of array */
 label:
 for (i = size-1; i > -1; i--) { /* i is y */
     for (j = size-1; j > -1; j--) { /* j is x */
         if( !map[j][i] ) {
             for ( k = j; k > 0; k--) { 
                 map[k][i] = map[k-1][i];
             }
             map[0][i] = 0;
              full = false;
             for ( k =  j; k > -1; k--) {
               if( map[k][i] ) {
                  full = true; 
                  break;
               } 
             }
             if(full) { ++j; }
         }
     }
 } 
 if(!dogoto) return;  
/* megre the same elements */
 for (j = size-1; j > -1; j--) {
     for (i = size-2; i > -1; i--) {
         if( map[i][j] == map[i+1][j]) {
             map[i+1][j] *= 2;
             map[i][j] = 0;
             score += map[i+1][j];
             i--;
         }
     }
 }
/* offset of array */
 dogoto = false;
 goto label;
}

void move_left() {
 int i, j, k;
 bool dogoto = true, full = false;
 label:
/* offset of array */
 for (i = 0; i < size; i++) { /* y */
     for (j = 0; j < size; j++) { /* x */
         if( !map[j][i] ) {
             for ( k = j; k < size - 1 ; k++) { 
                 map[k][i] = map[k+1][i];
             }
             map[size-1][i] = 0;
              full = false;
             for ( k = j; k < size - 1 ; k++) {
               if( map[k][i] ) {
                  full = true; 
                  break;
               } 
             }
             if(full) { --j; }
         }
     }
 }   
 if(!dogoto) return;  
/* megre the same elements */
 for (j = 0; j < size; j++) { /* y */
     for (i = 1; i < size; i++) { /* x */
         if( map[i][j] == map[i-1][j]) {
          map[i-1][j] *= 2;
          map[i][j] = 0;
          score += map[i-1][j];
          i++;
         }
     }
 }
/* offset of array */
 dogoto = false;
 goto label;
}

void move_up() {
 int i, j, k;
 bool dogoto = true, full = false;
 label:
/* offset of array */
 for (i = 0; i < size; i++) { /* x */
     for (j = 0; j < size; j++) { /* y */
         if( !map[i][j] ) {
             for ( k = j; k < size - 1 ; k++) { 
                 map[i][k] = map[i][k+1];
             }
             map[i][size-1] = 0;
              full = false;
             for ( k = j; k < size - 1 ; k++) {
               if( map[i][k] ) {
                  full = true; 
                  break;
               } 
             }
             if(full) { --j; }
         }
     }
 }   
 if(!dogoto) return;  
/* megre the same elements */
 for (j = 0; j < size; j++) { /* x */
     for (i = 1; i < size; i++) { /* y */
         if( map[j][i] == map[j][i-1]) {
             map[j][i-1] *= 2;
             map[j][i] = 0;
             score += map[j][i-1];
             i++;
         }
     }
 }

/* offset of array */
 dogoto = false;
 goto label; 
}

void move_down() {
 int i, j, k;
 bool dogoto = true, full = false;
 label:
/* offset of array */
 for (i = size-1; i > -1; i--) { /* i is x */
     for (j = size-1; j > -1; j--) { /* j is y */
         if( !map[i][j] ) {
             for ( k = j; k > 0; k--) { 
                 map[i][k] = map[i][k-1];
             }
             map[i][0] = 0;
              full = false;
             for ( k =  j; k > -1; k--) {
               if( map[i][k] ) {
                  full = true; 
                  break;
               } 
             }
             if(full) { ++j; } /* print(); try animation ? */
         }
     }
 }   
 if(!dogoto) return;  
/* megre the same elements */
 for (j = 0; j < size; j++) { /* x */ 
     for (i = size-1; i > 0; i--) { /* y */
         if( map[j][i] == map[j][i-1]) {
          map[j][i] *= 2;
          map[j][i-1] = 0;
          score += map[j][i];
          i--;
         }
     }
 }
/* offset of array */
dogoto = false;
goto label;
}

