const uint8_t SLOW   = 5000;
const uint8_t STEADY = 2000;
const uint8_t FAST   = 1000;

const int led     = 13;
const int ledStop = 4;
const int beep    = 11;
const int button  = 2;
long time;

struct game {
  
  int32_t gameStart;
  int32_t gameEnd;
  int32_t gameTracker;
  boolean gameOn = false; 

}; 

game gabGame; 

void setup() {

  SerialUSB.begin(115200);
  SerialUSB.println("Hello!"); 

  pinMode(led, OUTPUT);
  pinMode(beep, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(button), gameState, FALLING);
  
  blinkBeep(STEADY);

}

void loop(){

  time = millis();
  if (gabGame.gameOn){
    
    SerialUSB.println("Game started."); 

    setupGame(&gabGame, time);

    SerialUSB.println(gabGame.gameStart);
    SerialUSB.println(gabGame.gameEnd);
    SerialUSB.println(gabGame.gameTracker);
    SerialUSB.println(gabGame.gameOn);

    while((gabGame.gameTracker >= gabGame.gameEnd) && gabGame.gameOn){
      
      blinkBeep(SLOW);
      
      SerialUSB.print("Game Time: ");
      SerialUSB.println(gabGame.gameTracker);

      if(gabGame.gameTracker < (gabGame.gameStart * 1/2) ){
        blinkBeep(STEADY);
        SerialUSB.println("Half way.");
      }
      else if (gabGame.gameTracker < (gabGame.gameStart * 1/4) ){
        blinkBeep(FAST);
        SerialUSB.println("Quarter of the way.");
      } 
      if (gabGame.gameTracker - 1 <= gabGame.gameEnd)
        gameEndDisplay();

      gabGame.gameTracker--; 
    } 
  }
}

void gameState() {

  gabGame.gameOn = !gabGame.gameOn;

}

game * setupGame(game * newGame, long currTime){

  uint8_t randomGameEnd = random(60, 75); 
  long timeInSecs = (currTime/1000);

  newGame->gameStart   = timeInSecs + randomGameEnd; 
  newGame->gameEnd     = timeInSecs; 
  newGame->gameTracker = newGame->gameStart; 

  return newGame; 

}

void blinkBeep(uint8_t speed){
    
  if (speed == FAST){
    digitalWrite(led, HIGH);
    delay(FAST); 
    digitalWrite(led,LOW);
    delay(FAST); 
  }
  else if (speed == STEADY){
    digitalWrite(led, HIGH);
    delay(STEADY); 
    digitalWrite(led,LOW);
    delay(STEADY); 
  }
  else if (speed == SLOW){
    digitalWrite(led, HIGH);
    delay(SLOW); 
    digitalWrite(led,LOW);
    delay(SLOW); 
  }
}

void gameEndDisplay(){

  gabGame.gameOn = false;
  digitalWrite(ledStop, HIGH);
  delay(3000);
  digitalWrite(ledStop, LOW);

}


