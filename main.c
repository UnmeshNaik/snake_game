#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<ncurses.h>
#include <sys/select.h>
#include<sys/ioctl.h>
#include <termios.h>
#include <stropts.h>
#include<stdbool.h>

//terminal resolution 38 142
//positioning cursor at given position
int len=5;
char dir='d';
void gotoxy(int x,int y)
{
	printf("\033[%d;%dH", (y), (x));
}

//clearing the terminal
void clr()
{
	printf("\033[H\033[J");
}

struct snake
{
	int x,y; //Coordinates of this node	
	struct snake *p;
}*o[10],*tail,*head,food;

void print()
{
	struct snake *ptr=tail,*ptr1=tail->p;
	while(ptr!=NULL)
	{
		while(ptr1!=NULL)
		{
			if(ptr1->x==ptr->x && ptr1->y==ptr->y)
			{
				getchar();
				exit(1);
			}
			ptr1=ptr1->p;
		}
		if(head->x==143)	
			exit(1);
		gotoxy(ptr->x,ptr->y);
		printf("*");
		ptr=ptr->p;
		//getchar();
	}
	gotoxy(food.x,food.y);
	printf("*");
	fflush(stdout);
}

void foodfun(int max_y,int max_x)
{
	food.y=rand() % (max_y + 1 - 0) + 0;
	food.x=rand() % (max_x + 1 - 0) + 0;
}

void initial()
{
	int i;
	/*o[0]=(struct snake*)malloc(sizeof(struct snake));
	o[1]=(struct snake*)malloc(sizeof(struct snake));
	o[2]=(struct snake*)malloc(sizeof(struct snake));*/
	for(i=0;i<len;i++)
	{
		o[i]=(struct snake*)malloc(sizeof(struct snake));
	}
	tail=o[0];
	for(i=0;i<len;i++)
	{
		o[i]->p=o[i+1];
		o[i]->x=80+i;
		o[i]->y=20;
	}
	o[len-1]->p=NULL;
	head=o[len-1];
}

void addnode()
{
	o[len]=(struct snake*)malloc(sizeof(struct snake));
	head->p=o[len];
	o[len]->x=head->x;
	o[len]->y=head->y;
	if(dir=='d')
		o[len]->x=head->x+1;
	else if(dir == 'w')
		o[len]->y=head->y-1;
	else if(dir == 's')
		o[len]->y=head->y+1;
	else if(dir == 'a')
		o[len]->x=head->x-1;
	head=o[len];
	head->p=NULL;
	len++;
}

void run()
{
	tail->x=head->x;
	tail->y=head->y;
	if(dir=='d')
		tail->x=head->x+1;
	else if(dir == 'w')
		tail->y=head->y-1;
	else if(dir == 's')
		tail->y=head->y+1;
	else if(dir == 'a')
		tail->x=head->x-1;
	head->p=tail;
	head=tail;
	tail=tail->p;
	head->p=NULL;

	if(head->x==food.x && head->y == food.y)
	{
		addnode();
		foodfun(38,142);
	}
}

// int _kbhit() {
//     static const int STDIN = 0;
//     static bool initialized = false;

//     if (! initialized) {
//         // Use termios to turn off line buffering
//         struct termios term;
//         tcgetattr(STDIN, &term);
//         term.c_lflag &= ~ICANON;
//         tcsetattr(STDIN, TCSANOW, &term);
//         setbuf(stdin, NULL);
//         initialized = true;
//     }

//     int bytesWaiting;
//     ioctl(STDIN, FIONREAD, &bytesWaiting);
//     return bytesWaiting;
// }

int main()
{
	//Getting windows dimensions
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	char ch='d';
	initscr();
    //cbreak();
    noecho();
    //scrollok(stdscr, TRUE);
    //nodelay(stdscr, TRUE); OR timeout(0 or +ve)
    timeout(1);
    initial();
    foodfun(size.ws_row,size.ws_col);
    do{
    	ch=getch();
        if (ch == 'w' && dir!='s' )
        {
            //printw("You pressed G\n");
            dir='w';
        }
        else if(ch == 's' && dir!='w')
        	dir='s';
        else if((ch == 'a' || ch == 'A')&& dir!='d')
        	dir='a';
        else if((ch == 'd' || ch == 'D') && dir != 'a')
        	dir='d';
        napms(50);
        //printw("Running\n");
        clr();
        print();
        run();
    }while(true);
    endwin();
}