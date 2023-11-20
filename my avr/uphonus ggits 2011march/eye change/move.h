 unsigned char pos=1;
 unsigned char zunction=0;
volatile unsigned char robotpath[]={0x00,0x10,0x20,0x30,0x40,0x50,0x51,0x41,0x31,0x21,0x11,0x01,0x02,0x12,0x22,0x32,0x42,0x52,0x53,0x43,0x33,0x23,0x13,0x03,0x04,0x14,0x24,0x34,0x44,0x54,0x55,0x45,0x35,0x25,0x15,0x05,0xEE};
struct arena
{
 unsigned char temp;
 unsigned char visited;//visted=1 unvisited=0
};
unsigned char path[70];
volatile int pathptr=0;
struct arena maze[6][6]; 
void turnright(void);
void turnleft(void);
void turnaround(void);
void move_ab(unsigned char);
void move_ba(unsigned char);
void tracepath(unsigned char);
void goforw(void)
{ 
avoid_zun();
}



void turnright()
{
 eye_read();
 unsigned char r;
   
 r=(Reye & 0b00001110);
 while(r!=0x00)
 {
  eye_read();
  r=(Reye & 0b00001110);
  PORTC=xright;
  steer(pwmmaxspeed-190,pwmmaxspeed-190);
 } 
  r=(Reye & 0b00000100);
while(r!=0x04)
 {
  eye_read();
  r=(Reye & 0b00000100);
  PORTC=xright;
  steer(pwmmaxspeed-190,pwmmaxspeed-190);
 }
  PORTC=brake;steer(255,255);
}
void turnleft()
{
 eye_read();
 unsigned char r;
   
 r=(Reye & 0b00001110);
 while(r!=0x00)
 {
  eye_read();
  r=(Reye & 0b00001110);
  PORTC=xleft;
  steer(pwmmaxspeed-190,pwmmaxspeed-190);
 } 
  r=(Reye & 0b00000100);
while(r!=0x04)
 {
  eye_read();
  r=(Reye & 0b00000100);
  PORTC=xleft;
  steer(pwmmaxspeed-190,pwmmaxspeed-190);
 }
 PORTC=brake;steer(255,255);
}
  
 

void turnaround()
{
 turnleft();
 turnleft();
}


void move_ab(uchar t)
 {
    // a -> next cell
	// b -> current cell
	if(pos==1 && t==0x10 )
	{
 //    forw(24,5);
	}
    else if(pos==1 && t==0x01 )
    {
     turnright();
	 pos=2;
    } 
	else if(pos==2 && t==0x10 )
	{
     turnleft();
	 pos=1;
	}
    else if(pos==2 && t==0x01 )
    {
 //    forw(24,5);
	 
	}
	else if(pos==3 && t==0x10 )
	{
     turnaround();
	 pos=1;
	}
    else if(pos==3 && t==0x01 )
    {
     turnleft();
	 pos=2;
	}
	else if(pos==4 && t==0x10 )
	{
     turnright();
	 pos=1;
	}
    else if(pos==4 && t==0x01 )
    {
    turnaround();
	pos=2;
	
	}
 
	
 }
 
void move_ba(uchar t)
 {
    // a -> next cell
	// b -> current cell
	if(pos==1 && t==0x10 )
	{
     turnaround();
	 pos=3;
	}
    else if(pos==1 && t==0x01 )
    {
     turnleft();
	 pos=4;
    } 
	else if(pos==2 && t==0x10 )
	{
     turnright();
	 pos=3;
	}
    else if(pos==2 && t==0x01 )
    {
	turnaround();
	 pos=4;
  
	}
	else if(pos==3 && t==0x10 )
	{
 //    forw(24,5);
	 pos=3;
	}
    else if(pos==3 && t==0x01 )
    {
     turnright();
	 pos=4;
	}
	else if(pos==4 && t==0x10 )
	{
     turnleft();
	 pos=3;
	}
    else if(pos==4 && t==0x01 )
    {
//    forw(24,5);
	}
 
	
 }
 uchar search_next_move(uchar temp)
 {
  
  while(robotpath[zunction]!=temp)
  {
   zunction++;
  }
  return robotpath[zunction+1];
 }
 void tracepath(uchar ptr)
 {
   uchar a,b;
    b=ptr;//current
    a=search_next_move(ptr);//next    
    if(a>b)
	move_ab(a-b);
	else
	move_ba(b-a);
    
  
 }
 void modify_trace_route()
 {
  unsigned char k,l;
  unsigned char i=zunction;
  while(robotpath[i]!=path[pathptr-1])
  {
   i++;
  }//search for final value of path from floodfill in robot path
  i++;
  for(k=pathptr;robotpath[i]!=0xEE;k++)
  {
   path[k]=robotpath[i];
   i++;
  }
  path[k]=0xEE;
  robotpath[0]=current_cell;
  for(l=0;path[l]!=0xEE;l++)
  {
   robotpath[l+1]=path[l];
  }
   robotpath[l+1]=path[l];
  zunction=0;
 }
 void modify_trace_route_for_final_run()
 {
  unsigned char k;
  unsigned char i=0;
  robotpath[0]=current_cell;
  for(k=1;k<pathptr;k++)
  {
   robotpath[k]=path[k-1];
  }
  robotpath[k]=0x00;
  robotpath[k+1]=0xEE;
  
  zunction=0;
 }