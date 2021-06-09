#include "pitches.h"

const int button[] = {9,10,11,12}; //The four button input pins
const int ledpin[] = {5,6,7,8};  // LED pins
const int melody[] = {NOTE_C4, NOTE_C3, NOTE_G3, NOTE_A3};
const int buzzerPin = 3;
int randomArray[100]; //Intentionally long to store up to 100 inputs (doubtful anyone will get this far)
volatile int buttonstate = LOW; 
volatile int inputArray[100]; 
volatile int turn = 0;  // turn counter

void setup()
{
  for(int c = 0; c<4 ; c++){
    pinMode(button[c],INPUT_PULLUP);
    pinMode(ledpin[c],OUTPUT);
  }
  pinMode(3,OUTPUT);
  randomSeed(analogRead(0)); //Added to generate "more randomness" with the randomArray for the output function
  for (int y = 0; y <= 99; y++)
  {
    randomArray[y] = random(0, 3); //Assigning a random number (1-4) to the randomArray[y], y being the turn count
  }  
}


void loop()
{
  if(turn == 0) start();
  for (int y = 0; y <= turn; y++){ 
    flash_sequence();
    get_input();
    if(check()){
      turn++;
    }else{
      turn=-1;
    }
  }
  if(turn == -1)  fail();
}

void start() { 
  tone(3, NOTE_G3, 4);
  tone(3, NOTE_G2, 8);
  tone(3, NOTE_G3, 8);
  for(int i = 3; i>=0 ; i--){
    for (int y = 0; y < 4; y++)
    { //Flashes lights for start
      digitalWrite(ledpin[y], HIGH);
    }
    delay(2000);
    for (int y = 0; y < 4; y++)
    { //Flashes lights for start
      digitalWrite(ledpin[y], LOW);
    }
    delay(2000);
  }
  
}
  
void flash_sequence(){
  for (int x = 0; x <= turn; x++){
    digitalWrite(ledpin[randomArray[x]],HIGH);
    tone(3, melody[randomArray[x]], 4);
    delay(250);
    digitalWrite(ledpin[randomArray[x]],LOW);
    delay(250);  
  }
}

void get_input() { //Function for allowing user input and checking input against the generated array
  int buttons_pressed = 0;
  while(buttons_pressed <= turn){
    for (int x = 0; x <= 4; x++){ 
      if(!digitalRead(button[x])){
        inputArray[buttons_pressed]=x;
        buttons_pressed++;
        delay(200);
      }
    }
  }
}

bool check(){
  for(int x=0; x<=turn;x++){
    if(inputArray[x]!=randomArray[x]){
      return false;
    }
    return true;
  }
}

void fail() { //Function used if the player fails to match the sequence

  for (int x = 0; x < 4; x++)
  { //Flashes lights for failure
    digitalWrite(ledpin[x], HIGH);
    tone(3, NOTE_G2, 8);
  }
  delay(2000);
  for (int x = 0; x < 4; x++)
  { //Flashes lights for failure
    digitalWrite(ledpin[x], LOW);
  }
  //Reset turn to 0
  turn = 0;
  delay(2000);
}
