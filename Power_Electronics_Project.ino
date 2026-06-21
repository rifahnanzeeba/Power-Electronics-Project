#include <TimerOne.h>   
#include <IRremote.h>

#include <LiquidCrystal.h>

const int rs = 37, en = 39, d4 = 41, d5 = 43, d6 = 45, d7 = 47;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int sensorPin1=51;
int sensorPin2=53;
int sensorState1 = 0;
int sensorState2 = 0;
int count=0;
int ready_entrer=0;
int ready_sortir=0;


int receiver = 3;

IRrecv irrecv(receiver);

volatile int i=0;               // Variable to use as a counter
volatile boolean zero_cross=0;  // Boolean to store a "switch" to tell us if we have crossed zero
int AC_pin = 13;                    //pp note
int buton1 = 4;                 // first button at pin 4
int buton2 = 5;                 // second button at pin 5
int dim = 128;                  // Dimming level (0-128)  0 = on, 128 = 0ff
int pas = 15;                   // step for count;
int A = 9;                 
int B = 6;
int C = 7;
int D = 8;
int E = 12;
int F = 10;
int G = 11;
int freqStep = 175;    // This is the delay-per-brightness step in microseconds.
char incomingByte;

void setup()


{ pinMode (sensorPin1,INPUT_PULLUP);
  pinMode (sensorPin2, INPUT_PULLUP);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("VISITORS");

  Serial.begin(9600); // initialization
  irrecv.enableIRIn(); // Start the IR receiver (classic remote)
  pinMode(buton1, INPUT);  // set buton1 pin as input
  pinMode(buton2, INPUT);  // set buton1 pin as input
  pinMode(AC_pin, OUTPUT);  
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  attachInterrupt(0, zero_cross_detect, RISING);    // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  Timer1.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(dim_check, freqStep);     
  // Use the TimerOne Library to attach an interrupt
}


void zero_cross_detect() {   
  zero_cross = true;               // set the boolean to true to tell our dimming function that a zero cross has occured
  i=0;
  digitalWrite(AC_pin, LOW);
}                                

// Turn on the TRIAC at the appropriate time
void dim_check() {                  
  if(zero_cross == true) {             
    if(i>=dim) {                    
      digitalWrite(AC_pin, HIGH);  // turn on light      
      i=0;  // reset time step counter                        
      zero_cross=false;    // reset zero cross detection
    }
    else {
      i++;  // increment time step counter                    
    }                               
  }   
} 

//-----( Declare User-written Functions )-----
void translateIR() // takes action based on IR code received

{
  if(IrReceiver.decode()){
  switch(IrReceiver.decodedIRData.command)
  {
 
case 0x45:
 
    dim=128;
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, LOW);
    }
    break;

   
case 0x46: 
    dim=120;
    {
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    digitalWrite(E, LOW);
    digitalWrite(F, LOW);
    digitalWrite(G, LOW);
    }
    break;
   

case 0x47: 
    dim=105;
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, LOW);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, LOW);
    digitalWrite(G, HIGH);
    }
    break;

case 0x44: 
    dim=90;
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, LOW);
    digitalWrite(F, LOW);
    digitalWrite(G, HIGH);
    }
  break;
   
case 0x40: 
    dim=75;
    {
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    digitalWrite(E, LOW);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    }
    break;

case 0x43: 
    dim=60;
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, LOW);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    }
  break;

case 0x7: 
    dim=45;
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    }
  break;

case 0x15: 
    dim=30;
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    digitalWrite(E, LOW);
    digitalWrite(F, LOW);
    digitalWrite(G, LOW);
    }
  break;
   

case 0x9: 
    dim=15;
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    }
  break;
   
case 0x19: 
    dim=00;
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, LOW);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    }
   break;


  case 0x52: 
    {
    if (dim<127) 
   {
    dim = dim + pas;
    if (dim>127)
    {
     dim=128;
    }
    }
    }
   break;

  case 0x18: 
    {
      {
  if (dim>5) 
  {
     dim = dim - pas;
  if (dim<0)
    {
      dim=0;  // in vechiul sketch era 1
    }
   }
   }
   }
  break;
  if(dim>100)
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, LOW);
    }
  
  }

}
}

void loop() 
{  

  if(IrReceiver.decode()) // have we received an IR signal?
  {
    translateIR();
    irrecv.resume(); // receive the next value
  } 

 digitalWrite(buton1, HIGH);
 digitalWrite(buton2, HIGH);
 if (digitalRead(buton1) == LOW)  
   {
  if (dim<127) 
  {
    dim = dim + pas;
    if (dim>127)
    {
      dim=128; // in vechiul sketch era 127
    }
  }
   }
  if (digitalRead(buton2) == LOW)  
   {
  if (dim>5) 
  {
     dim = dim - pas;
  if (dim<0)
    {
      dim=0;  // in vechiul sketch era 1
    }
   }
   }
    while (digitalRead(buton1) == LOW) {  }             
    delay(10); // waiting little bit... 
    while (digitalRead(buton2) == LOW) {  }             
    delay(10); // waiting little bit...   
 if(dim>127)
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, LOW);
    }
   
      
 if(127>dim && dim>115)
    {
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    digitalWrite(E, LOW);
    digitalWrite(F, LOW);
    digitalWrite(G, LOW);
    }

 if(115>dim && dim>105)
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, LOW);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, LOW);
    digitalWrite(G, HIGH);
    }

 if(105>dim && dim>90)
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, LOW);
    digitalWrite(F, LOW);
    digitalWrite(G, HIGH);
    }
   
 if(90>dim && dim>75)
   {
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    digitalWrite(E, LOW);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    }

 if(75>dim && dim>60)
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, LOW);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    }

 if(60>dim && dim>45)
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    }

 if(45>dim && dim>30)
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    digitalWrite(E, LOW);
    digitalWrite(F, LOW);
    digitalWrite(G, LOW);
    }

 if(30>dim && dim>15)
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    }
     
 if(15>dim && dim>00)
    {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, LOW);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    }

  sensorState1 = digitalRead(sensorPin1);
  sensorState2 = digitalRead(sensorPin2);
  if(sensorState1 == LOW){
    ready_entrer=1;
  }
  else if(sensorState2 == LOW){
    ready_sortir=1;
  }
  if(ready_entrer==1 && sensorState2 == LOW){
    count++; 
    ready_entrer=0;
    ready_sortir=0;

  if (dim<=128) 
  {
    dim = dim - pas;
    if (dim>127)
    {
      dim=128; // in vechiul sketch era 127
    }
  }
  
   delay(500);
  }
  else if(ready_sortir==1 && sensorState1 == LOW){
  count--; 
  ready_sortir=0;
  ready_entrer=0;
  if (dim>5) 
  {
     dim= dim + pas;
  if (dim<0)
    {
      dim=0;  // in vechiul sketch era 1
    }
   }
    delay(500);
    
  }
  lcd.setCursor(1, 1);
  lcd.print(count);
 
 Serial.println(dim);
 delay(500);
}
