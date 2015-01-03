#include<stdio.h>
#include<stdlib.h>
#include<curses.h>
//#include<iostream.h>
#define CTRL(c)((c)& 037)
int row,col;
main(int argc,char ** argv)
{
int i,l,j,line=0;
int c;
FILE *fd;
if(argc!=2)
{
fprintf(stderr,"Usage:%s file\n",argv[0]);
exit(1);
}
initscr();
cbreak();
nonl();
noecho();
idlok(stdscr,TRUE);
keypad(stdscr,TRUE);
fd=fopen(argv[1],"r+");
while((c=getc(fd))!=EOF)
{
if(c=='\n')
line++;
if(line>LINES-2)
break;
addch(c);
}
fclose(fd);
move(0,0);
refresh();
edit();
fd=fopen(argv[1],"w");
for(i=0;i<LINES-1;i++)
{
j=len(i);
for(l=0;l<j;l++)
putc(mvinch(i,l)&A_CHARTEXT,fd);
putc('\n',fd);
}
fclose(fd);
endwin();
return 0;
}

int len(int lineno)
{
int linelen=COLS-1;
while(linelen >=0 && mvinch(lineno,linelen)== ' ')
linelen--;
return linelen+1;
}

edit()
{
int c;
while(1)
{
 move(row,col);
 refresh();
 c=getch();
 switch(c)
 {
   case 'h':
        case KEY_LEFT:
         if(col>0)
             col--;
         else
             flash();
         break;
  case 'j':
        case KEY_DOWN:
         if(row<LINES-1)
             row++;
         else
             flash();
         break;
  case 'k':
        case KEY_UP:
         if(row>0)
             row--;
         else
             flash();
         break;
  case 'l':
        case KEY_RIGHT:
         if(row<COLS-1)
             col++;
         else
             flash();
         break;
  case KEY_IC:
  case 'i':
       input();
       break;
  case KEY_DC:
  case 'x':
       delch();
       break;
  case KEY_IL:
  case 'o':
       move(++row,col=0);
       insertln();
       input();
       break;
  case KEY_DL:
  case 'd':
       deleteln();
       break;
  case KEY_CLEAR:
  case CTRL('L'):
       wrefresh(curscr);
       break;
  case 'w':
       return;
  case 'q':
       endwin();
  default:
       flash();
       break;
 }
}
}

input()
{
int c;
standout();
mvaddstr(LINES-1,COLS-20,"INPUT MODE");
standend();
move(row,col);
refresh();
for(;;)
{
c=getch();
if(c==CTRL('D')|| c==KEY_EIC)
break;
insch(c);
move(row,++col);
refresh();
}
move(LINES-1,COLS-20);
clrtoeol();
move(row,col);
refresh();
}

