/*This code is Build and developed by Binay Kumar Sah. It uses 8*8 led matrix board.
   Baord is driven by 74HC595 SIPO shift register and the movement of the cursor is
   controlled via potentiometer
 */
#define dataROW 5
#define latchROW 6
#define clkROW 7
#define dataCOLOUMN 8
#define latchCOLOUMN 9
#define clkCOLOUMN 10
#define datapad 2       //to control the paddle
#define latchpad 3
#define clkpad 4
#define table A0
int pot_read;
int row[]={1,2,4,8,16,32,64,128};
int coloumn[]={128,64,32,16,8,4,2,1};
int col;
int X=0;
int Y=3;
int i;
int counter=0;
int directionX=1;
int directionY=1;
byte coloumnpad=B00000111;
int SCORE=0;
byte colupdate;  
byte colpad;
int Status=0;
int t;  //for testing the ball position for paddle strike 
int table_shift;
void lightScreen();
void map_table();
int counterdelay;
void screen_update();
void screen_scan();
void pad_update();
void setup()
{ Serial.begin(9600);
  pinMode(dataROW, OUTPUT);
  pinMode(dataCOLOUMN, OUTPUT);
  pinMode(latchROW, OUTPUT);
  pinMode(latchCOLOUMN, OUTPUT);
  pinMode(clkROW, OUTPUT);
  pinMode(clkCOLOUMN, OUTPUT);
  pinMode(table, INPUT);
  pinMode(datapad,OUTPUT);
  pinMode(latchpad,OUTPUT);
  pinMode(clkpad,OUTPUT);
}

void loop()
{ 
  if (Status==0)
  {
    screen_scan();
    screen_update();
    map_table();
    pad_update();
    counterdelay=constrain(counter,0,450);
    delay(500-counterdelay);
    Serial.print("SCORE     ");
    Serial.println(SCORE);
    
  }
  else if(Status==1)
  {
    lightScreen();
    Serial.println(".................................................OOPS ! YOU MISS THE BALL ................................................................");
    Serial.println("...........................................................................................................................................");
    Serial.println("");
    Serial.print("YOUR SCORE=   ");
    Serial.println(SCORE);
    Serial.println("...........................................................................................................................................");
    Serial.println("...........................................................................................................................................");
    Serial.println("....................................PLEASE RESTART TO CONTINUE.............................................................................");
        
  }
}

void map_table()
{
   pot_read=analogRead(table);
   table_shift=map(pot_read,0,1024,0,6);             
   colupdate=(coloumnpad<<table_shift);
}

void screen_update()
{ 
   if (X==0 && (directionY==-1 || directionY==1 || directionY==0))    //when a ball strike left-most part of the screen 
    {directionX=1;} 
   else if (X==7 && (directionY==1 || directionY==-1 || directionY==0)) //when a ball strike right-most part of the screen
    {directionX=-1;}
   else if (Y==0 )
    {
      if (directionX==1 || directionX==-1)
        {
            directionY=1;
        }
      else if (directionX==0)
        { 
          directionX=-1;
          directionY=1;
        }
    }
  
   else if (Y==7 && (directionX==1 || directionX==-1 || directionX==0))     //if it strike at the bottom part of the screen 
       {
        if (colupdate==((t*4)+(t*2)+t))
          { 
            directionX=0;
            directionY=-1;  
            counter=counter+50;
            SCORE=SCORE+10;
          }
        else if (colupdate==((t*2)+t+(t/2)))
          {
            directionY=-1;  
            counter=counter+50;
            SCORE=SCORE+10;
          }
        else if(colupdate==(t+(t/2)+(t/4)))
          {
            directionX=0;
            directionY=-1;
            counter=counter+50;
            SCORE=SCORE+10;
          }
        else
          {
             Status=1;
          }
       }
      
   X=X+directionX;
   Y=Y+directionY;
  
 
}

void screen_scan()
{  
  col= ~coloumn[X];
  t=coloumn[X];
  digitalWrite(latchCOLOUMN, LOW);
  shiftOut(dataCOLOUMN, clkCOLOUMN, MSBFIRST,col);
  digitalWrite(latchCOLOUMN, HIGH);
 
  {
    digitalWrite(latchROW, LOW);
    shiftOut(dataROW, clkROW, MSBFIRST, row[Y]);
    digitalWrite(latchROW, HIGH);
 
  }
 
}


void pad_update()
{
  colpad=~colupdate;
  digitalWrite(latchpad,LOW);
  shiftOut(datapad, clkpad, MSBFIRST,colpad);
  digitalWrite(latchpad,HIGH);
 }


 void lightScreen()
{
  digitalWrite(latchCOLOUMN, LOW);
  shiftOut(dataCOLOUMN, clkCOLOUMN, MSBFIRST,B00000000);
  digitalWrite(latchCOLOUMN, HIGH); 
  
  {
    digitalWrite(latchROW, LOW);
    shiftOut(dataROW, clkROW, MSBFIRST, B11111111);
    digitalWrite(latchROW, HIGH);
 
  }
  delay(1000);
 }
