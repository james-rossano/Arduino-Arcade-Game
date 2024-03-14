//James Rossano
//4-13-2023
//Arduino Slave Code

#include <FastLED.h>

#include <Wire.h>
#define WIRE_ADDR 9 //pin that the two boards are connected to
#define ANSWERSIZE 5

#define LED_PIN 11 //pin that the LED strip is connected to
#define NUM_LEDS 150 //total number of LEDs on strip

int LEDcount = 2; //current LED index
bool isRunning = true;
String userIn;

int target1 = 44; //where button 1 is centered at
int target2 = 91; //where button 2 is centered at

bool player1Press = false; //determines whether player 1 has already pressed button
bool player1Turn = true; //determines whether its player 1 turn
bool player2Press = false; //determines whether player 2 has already pressed button
bool player2Turn = false; //determines whether its player 2 turn

int turnSwitch = 74; //LED that designated turn switching from player 1 to player 2

int redV = 0;
int blueV = 255;

int totalScore1 = 0;
int totalScore2 = 0;
int player1Wins = 0;
int player2Wins = 0;

int curRev = 0; //revolution that the game is on
int revolutions = 5; //max number of revolutions/rounds in the game

int revSpeed = 23; //speed of light moving (lower is faster)

CRGB leds[NUM_LEDS];
 
void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();
  FastLED.show();
  
  Wire.begin(WIRE_ADDR);
  Wire.onReceive(receiveEvent);
  
  Serial.begin(9600);
}
 
void receiveEvent() { //runs when button is pressed
  while (0 < Wire.available()) {
    byte x = Wire.read(); //reads the LED position
     
    //Serial.println(x);
    if(int(x) == 1 && player1Turn == true){
      isRunning = false; //stops the LEDs from continuing normal loop
      player1Press = true;
      
    } else if(int(x) == 2 && player2Turn == true){
      isRunning = false; //stops the LEDs from continuing normal loop
      player2Press = true;
      
    } else{
      //Serial.println("invalid input");
    }
  }
}

 
void loop() { 
  if(curRev < revolutions){
    if(isRunning == true){ //runs while it hasn't gone over max revolutions and runs while button hasnt been pressed
      leds[LEDcount] = CRGB(redV, 0, blueV);
      leds[LEDcount-1] = CRGB(0, 0, 0);
      LEDcount++;
      FastLED.setBrightness(200);
      FastLED.show();
    } else{
  
      if(player1Press == true && player1Turn == true){ //displays score of player 1 press and doesnt let them press button again
        totalScore1 = abs(LEDcount - target1);
        Serial.println("Distance From Target:");
        Serial.println(totalScore1);
        player1Turn = false;
      }
  
      if(player2Press == true && player2Turn == true){ //displays score of player 2 press and doesnt let them press button again
        totalScore2 = abs(LEDcount - target2);
        Serial.println("Distance From Target:");
        Serial.println(totalScore2);
        player2Turn = false;
      }
      
      delay(1000); //briefly pauses the LEDs when the button is pressed
      isRunning = true; //sets the LEDs to start moving again
      
    }

    if(LEDcount >= NUM_LEDS){ //resets when LED reaches end of strip
    
      if(player1Press == true){
        if(player2Press == true){
          if(totalScore1 < totalScore2){
            Serial.println("Player 1 Wins Round");
            player1Wins+=1;
            fill_solid(leds, NUM_LEDS, CRGB::Blue);
            FastLED.setBrightness(200);
            FastLED.show();
            delay(300);
          } else if(totalScore2 < totalScore1){
            Serial.println("Player 2 Wins Round");
            player2Wins+=1;
            fill_solid(leds, NUM_LEDS, CRGB::Red);
            FastLED.setBrightness(200);
            FastLED.show();
            delay(300);
          } else{
            Serial.println("Round is a tie");
            fill_solid(leds, NUM_LEDS, CRGB::Purple);
            FastLED.setBrightness(200);
            FastLED.show();
            delay(300);
          }
        } else{
          Serial.println("Player 1 Wins Round");
          player1Wins+=1;
          fill_solid(leds, NUM_LEDS, CRGB::Blue);
          FastLED.setBrightness(200);
          FastLED.show();
          delay(300);
        }
      } else if(player2Press == true){
        Serial.println("Player 2 Wins Round");
        player2Wins+=1;
        fill_solid(leds, NUM_LEDS, CRGB::Red);
        FastLED.setBrightness(200);
        FastLED.show();
        delay(300);
      } else{
        Serial.println("No One Wins Round");
        fill_solid(leds, NUM_LEDS, CRGB::Purple);
        FastLED.setBrightness(200);
        FastLED.show();
        delay(300);
      }
      
      FastLED.clear();
      FastLED.show();
      
      player2Turn = false;
      player1Turn = true;
      player1Press = false;
      player2Press = false;
      totalScore1 = 0;
      totalScore2 = 0;

      blueV = 255;
      redV = 0;
      
      LEDcount = 2;
      curRev += 1;
      
    }
      
    if(LEDcount == turnSwitch){ //switches from player 1 turn to player 2 turn
      player2Turn = true;
      player1Turn = false;
      redV = 255;
      blueV = 0;
    }
  
    delay(revSpeed);
  }

  if(curRev >= revolutions){
    if(curRev != 100){
      Serial.println("Game Is Over");
      if(player1Wins > player2Wins){
        Serial.println("Players 1 Wins");
        fill_solid(leds, NUM_LEDS, CRGB::Blue);
        FastLED.setBrightness(200);
        FastLED.show();
      } else if(player2Wins > player1Wins){
        Serial.println("Players 2 Wins");
        fill_solid(leds, NUM_LEDS, CRGB::Red);
        FastLED.setBrightness(200);
        FastLED.show();
      } else{
        Serial.println("Players Tie");
        fill_solid(leds, NUM_LEDS, CRGB::Purple);
        FastLED.setBrightness(200);
        FastLED.show();
      }
    }
    curRev = 100;
    while (Serial.available() == 0){ 
      //Wait for user input  
    }  
    userIn = Serial.readString();
    Serial.println(userIn);
    if(userIn == "n\n"){
      curRev = 0;
      totalScore1 = 0;
      totalScore2 = 0;
      player1Wins = 0;
      player2Wins = 0;
      isRunning = true;
      LEDcount = 2;
      player1Press = false; 
      player1Turn = true; 
      player2Press = false; 
      player2Turn = false;
      FastLED.clear();
      FastLED.show();
    }
  }
}
