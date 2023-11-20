

void flush()
{
unsigned char i,j;
 for(i=0;i<6;i++)
 {
  for(j=0;j<6;j++)
  {
   if(maze[i][j].temp!=99)               
   {
   maze[i][j].temp=37;
   }
   
  }
 }
}
void fflush()
{
unsigned char i,j;
 for(i=0;i<6;i++)
 {
  for(j=0;j<6;j++)
  {
   maze[i][j].temp=37;
   maze[i][j].visited=0;
   
  }
 }
}

void floodfill()
{
 unsigned char i,j,k,chance=36;
 k=0;
 while(chance>0)
 {
 chance=chance-1;
 for(i=0;i<6;i++)
 {
  for(j=0;j<6;j++)
  {
   if(maze[i][j].temp==k)
   {
   if((i+1<6)&&(maze[i+1][j].temp!=99)&&maze[i][j].temp<maze[i+1][j].temp)
   maze[i+1][j].temp=k+1;
   if((i-1>=0)&&(maze[i-1][j].temp!=99)&&maze[i][j].temp<maze[i-1][j].temp)
   maze[i-1][j].temp=k+1;
   if((j+1<6)&&(maze[i][j+1].temp!=99)&&maze[i][j].temp<maze[i][j+1].temp)
   maze[i][j+1].temp=k+1;
   if((j-1>=0)&&(maze[i][j-1].temp!=99)&&maze[i][j].temp<maze[i][j-1].temp)
   maze[i][j-1].temp=k+1;
   }//if ends here
  } //j loop
 } //i loop
 k=k+1;
}//while loop
}
void floodfilllast()
{
 unsigned char i,j,k,chance=36;
 k=0;
 while(chance>0)
 {
 chance=chance-1;
 for(i=0;i<6;i++)
 {
  for(j=0;j<6;j++)
  {
   if(maze[i][j].temp==k)
   {
   if((i+1<6)&&(maze[i+1][j].visited!=0)&&(maze[i+1][j].temp!=99)&&maze[i][j].temp<maze[i+1][j].temp)
   maze[i+1][j].temp=k+1;
   if((i-1>=0)&&(maze[i-1][j].visited!=0)&&(maze[i-1][j].temp!=99)&&maze[i][j].temp<maze[i-1][j].temp)
   maze[i-1][j].temp=k+1;
   if((j+1<6)&&(maze[i][j+1].visited!=0)&&(maze[i][j+1].temp!=99)&&maze[i][j].temp<maze[i][j+1].temp)
   maze[i][j+1].temp=k+1;
   if((j-1>=0)&&(maze[i][j-1].visited!=0)&&(maze[i][j-1].temp!=99)&&maze[i][j].temp<maze[i][j-1].temp)
   maze[i][j-1].temp=k+1;
   }//if ends here
  } //j loop
 } //i loop
 k=k+1;
}//while loop
}

void moves(unsigned char mx,unsigned char my)
{pathptr=0;
 while(maze[mx][my].temp!=0)
 {
  if((mx+1)<6 &&(maze[mx][my].temp>maze[mx+1][my].temp))
  mx=mx+1;
  else if((mx-1)>=0 &&(maze[mx][my].temp>maze[mx-1][my].temp))
  mx=mx-1;
  else if((my+1)<6 &&(maze[mx][my].temp>maze[mx][my+1].temp))
  my=my+1;
  else if((my-1)>=0 &&(maze[mx][my].temp>maze[mx][my-1].temp))
  my=my-1;
  path[pathptr]=(mx<<4)+my;
  pathptr++;
 
  
 // cout<<"  "<<mx<<"-"<<my;
 }
  // itoa(pathptr);
  //lcd_cmd(0xc0);
 //cout<<endl;
}
void algorun(unsigned char sxy ,unsigned char dxy)
{unsigned char ttemp,hb,lb;
 int i,j;
 flush();
 ttemp=dxy;
 hb=(ttemp>>4)&0x0f;
 lb=ttemp&0x0f;
 maze[hb][lb].temp=0;

 floodfill();
 ttemp=sxy;
 hb=(ttemp>>4)&0x0f;
 lb=ttemp&0x0f;
 
 moves(hb,lb);
  
} 
void finalrun(unsigned char sxy ,unsigned char dxy)
{unsigned char ttemp,hb,lb;
 int i,j;
 flush();
 ttemp=dxy;
 hb=(ttemp>>4)&0x0f;
 lb=ttemp&0x0f;
 maze[hb][lb].temp=0;

 floodfilllast();
 ttemp=sxy;
 hb=(ttemp>>4)&0x0f;
 lb=ttemp&0x0f;
 
 moves(hb,lb);
  
} 
 

     
     
     
 