#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <io.h>
#include <stdlib.h> 

struct Tpart {
    int x;
    int y;
    unsigned char direction;
};

int tail, doornot=0;
int a, b, score, hsc; /*a: x of food; b:y of food;hsc: high score*/
char filename[256];
Tpart snake[1500];
bool levelenabled;
////////////////////////////////////////////////////////////
void setscore(int i){
 FILE *F;
 F=fopen("Score.data","w");
 fprintf(F,"%d",i);
 fclose(F);
}

void getscore(void){
 FILE *F;
 _finddata_t data;
 long nFind=_findfirst("Score.data",&data);
 if(nFind!=-1)
 {
   _findclose(nFind);
   F=fopen("Score.data","r");
   fscanf(F,"%d",&hsc);
   fclose(F);
 }else{
 F=fopen("Score.data","w");
 fprintf(F,"%d",hsc);
 fclose(F);
 } 
}
////////////////////////////////////////////////////////////
void setcur(int mode){
HANDLE handle;
handle=GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO structCursorInfo;
GetConsoleCursorInfo(handle,&structCursorInfo);
if(mode)structCursorInfo.bVisible=TRUE;
else structCursorInfo.bVisible=FALSE;
SetConsoleCursorInfo(handle,&structCursorInfo);
}
////////////////////////////////////////////////////////////
void _GOTO(int X, int Y){
COORD C;
C.X=X;
C.Y=Y;
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),C);
}
////////////////////////////////////////////////////////////

bool loadlevel(const char* levelname){
  FILE *F;

 _finddata_t data;
 long nFind=_findfirst(levelname,&data);
 if(nFind!=-1)
 {
   F=fopen(levelname,"r");
  char walls[53];
  int x,y;
  bool level[50][30];  



for(y=0;y<30;++y)
for(x=0;x<50;++x)
   level[x][y]=0;

   for(y=0;y<30;++y){
    fgets(walls,sizeof(walls),F);
        
    for(x=0;x<50;++x){
         if((walls[x]=='1')){
              level[x][y]=true;
              _GOTO(x,y);
              printf("%c",219);
           }else{
                 level[x][y]=false;   
                }
       walls[x]=0;
    }
     
   }


   for(x=0;x<50;++x){
   for(y=0;y<30;++y){
     if(level[x][y]){
        if((a==x)&&(b==y)){
         doornot=0;
         _GOTO(a,b);
         printf("%c",219);
        }

      if((snake[0].x==x)&&(snake[0].y==y))return true;
    }}}

   fclose(F);
 }else{
  _GOTO(0,0);
  printf("Level not found.");
  levelenabled=false;
  while(getch()!=13);
  _GOTO(0,0);
  printf("                ");
 } 
_findclose(nFind);
return false;
}
////////////////////////////////////////////////////////////
//1-up 2-left 3-down 4-left
int main(){

printf("Megaware Snake [Version 0.2]\n(c) Megaware Corporation, 2018. All rights reserved.\n\n");
getch();
_GOTO(0,0);
printf("                            \n                                                     ");

int i,n,speed;


 start:
_GOTO(0,0);
setcur(1);
filename[0]=0;
levelenabled=false;
printf("Press \"Enter\" to nothing, or enter a level name:\n");

gets(filename);
if(filename[0]!=0){
 levelenabled=true;
}

tail=speed=1;


system("cls");
char *str = (char*)malloc(2048);
printf("Speed (1-5): ");
gets(str);
sscanf(str,"%d",&speed);
free(str);
if(speed < 1 || speed > 5)speed=1;
system("cls");

     setcur(0);
     _GOTO(0,0);
     printf("Press \"Enter\" to continue...                      ");
     while(getch()!=13);
   
 for(i=0;i<1500;++i){
  snake[i].x=-1;
  snake[i].y=-1;
  snake[i].direction=0;
 }


 doornot=0;
 char key=0;
_GOTO(0,0); 
 for(i=0;i<31;++i){
     for(n=0;n<52;++n){
         printf(" ");
     }
     printf("\n");
 }

 for(i=0;i<30;++i){
 _GOTO(50,i);
 printf("#");
 }
 _GOTO(0,30);
 printf("###################################################");

 _GOTO(0,31);
 getscore();
 printf("Score: 0    \nHigh score: %d",hsc);
 for(i=0;i<10000000;++i);
 score=0;
 snake[0].x=24;
 snake[0].y=14;
 snake[0].direction=3;
 doornot=0;
 
  for(;;){
/////////////////////////////////////////////////////
//KEYCODES
    for(i = 0; i < 10000 / speed; i++)
    if(kbhit()) { 
         key=getch();
         if( key==27 )return 0;
         if( key==13 )goto start;
         if((key=='d')&&(snake[0].direction!=4))snake[0].direction=2; else 
         if((key=='s')&&(snake[0].direction!=1))snake[0].direction=3; else
         if((key=='a')&&(snake[0].direction!=2))snake[0].direction=4; else
         if((key=='w')&&(snake[0].direction!=3))snake[0].direction=1; else
         key=getch();
    }
         if((key == 80) && (snake[0].direction!=1) && (snake[0].direction!=1) )
            snake[0].direction=3;
         if((key == 75) && (snake[0].direction!=2) && (snake[0].direction!=2) )
            snake[0].direction=4;
         if((key == 72) && (snake[0].direction!=3) && (snake[0].direction!=3) )
            snake[0].direction=1;
         if((key == 77) && (snake[0].direction!=4) && (snake[0].direction!=4) )
            snake[0].direction=2; 
//KEYCODES 
//CLEAR
      setcur(0);
      _GOTO(snake[tail].x,snake[tail].y);
      printf(" ");         
//HEAD MOVE
         if(snake[0].direction==2)snake[0].x=snake[0].x+1; 
         if(snake[0].direction==3)snake[0].y=snake[0].y+1;
         if(snake[0].direction==4)snake[0].x=snake[0].x-1;
         if(snake[0].direction==1)snake[0].y=snake[0].y-1;
          if(snake[0].x< 0)snake[0].x=49;
          if(snake[0].y< 0)snake[0].y=29;
          if(snake[0].x>49)snake[0].x= 0;
          if(snake[0].y>29)snake[0].y= 0;
//HEAD MOVE
//////////////////////////////////////////////////FOOD
       if(doornot%50==0){
       doornot=0;
         _GOTO(a,b);
         printf(" ");
        food:

       a=rand()%50;
       b=rand()%30;
        for(int i=0;i<tail+1;++i)if((snake[i].x==a)&&(snake[i].y==b))goto food;
       _GOTO(a,b);
       printf("$");
       }
       doornot++;
/////////////////////////////////////////////////FOOD
////////////////////////////POINTS

if((snake[0].x==a)&&(snake[0].y==b)){
score++;
     if(key!=9)//cheetes
tail++;
_GOTO(0,31);
getscore();
if(score>hsc)setscore(score);
getscore();
printf("Score: %d\nHich score: %d",score,hsc);
doornot=0;
goto food;
}
////////////////////////////POINTS

/////////////////////////////////////////////////////////////BODY
      for(i=tail;i>0;--i)snake[i].direction=snake[i-1].direction;

      for(n=1;n<tail+1;++n){    
      if(snake[n].direction==3){
         snake[n].x=snake[n-1].x;
         snake[n].y=snake[n-1].y-1;
      }
      if(snake[n].direction==1){
         snake[n].x=snake[n-1].x;
         snake[n].y=snake[n-1].y+1;
      }
      if(snake[n].direction==2){
         snake[n].x=snake[n-1].x-1;
         snake[n].y=snake[n-1].y;
      }
      if(snake[n].direction==4){
         snake[n].x=snake[n-1].x+1;
         snake[n].y=snake[n-1].y;
      }
         if(snake[n].x< 0)snake[n].x=49;
         if(snake[n].y< 0)snake[n].y=29;
         if(snake[n].x>49)snake[n].x= 0;
         if(snake[n].y>29)snake[n].y= 0;

       _GOTO(snake[n].x,snake[n].y);
       printf("@");      
      }
//////////////////////////////////////////////////BODY
//HEAD
      _GOTO(snake[0].x,snake[0].y);
      switch(snake[0].direction){
       case 1: printf("^");
               break; 
       case 2: printf(">");
               break;
       case 3: printf("v");
               break;
       case 4: printf("<");
               break;
      }
//////////////LOSING

if(levelenabled==true)
  if(loadlevel(filename)==true)
    goto die;

for(i=1;i<tail+1;++i){
if((snake[0].x==snake[i].x)&&(snake[0].y==snake[i].y)){
 die:
 _GOTO(snake[0].x,snake[0].y);
 printf("X");
 _GOTO(0,0);
 printf("Game over! Press any key to continue...");
 getch();
 _GOTO(0,0);
 goto start;
}
}
//////////////LOSING
  key=0;  
  }
 return 0;
}
