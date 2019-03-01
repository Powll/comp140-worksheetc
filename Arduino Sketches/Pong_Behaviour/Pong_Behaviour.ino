const int playerOne = A0;
const int playerTwo = A1;
int playerOnePosition = 0;
int playerTwoPosition = 0;  
int playerOneScore = 0;
int playerTwoScore = 0;

const int playerOneLed[3] = {8, 9, 10};
const int playerTwoLed[3] = {5, 6, 7};

const int ledRed = 3;
const int ledYellow = 5;

int incomingByte = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  for(int i = 0; i < 3; i++)
  {
    pinMode(playerOneLed[i], OUTPUT);
    pinMode(playerTwoLed[i], OUTPUT);
  }
}
// Reset scores and display end-game specific LEDs
void resetGame()
{
  playerOneScore = 0;
  playerTwoScore = 0;

  endGameLeds();
}
// activate LEDs based on scores
void updateLeds()
{
  for(int i = 0; i < 3; i++)
  {
    if(i < playerOneScore)
      digitalWrite(playerOneLed[i], HIGH);
    else
      digitalWrite(playerOneLed[i], LOW);

    if(i < playerTwoScore)
      digitalWrite(playerTwoLed[i], HIGH);
    else
      digitalWrite(playerTwoLed[i], LOW);
  }
}
// flash LEDs in a fancy way when the game ends
// I am pretty sure I overcomplicated myself with this
void endGameLeds()
{
  for(int i = 0; i < 3; i++)
  {
    digitalWrite(playerOneLed[i], LOW);
    digitalWrite(playerTwoLed[i], LOW);
  }
  delay(200);
  
  digitalWrite(playerOneLed[0], HIGH);
  digitalWrite(playerTwoLed[0], HIGH);

  delay(200);

  digitalWrite(playerOneLed[1], HIGH);
  digitalWrite(playerTwoLed[1], HIGH);

  delay(100);
  
  digitalWrite(playerOneLed[0], LOW);
  digitalWrite(playerTwoLed[0], LOW);

  delay(200);

  digitalWrite(playerOneLed[2], HIGH);
  digitalWrite(playerTwoLed[2], HIGH);

  delay(100);

  digitalWrite(playerOneLed[1], LOW);
  digitalWrite(playerTwoLed[1], LOW);

  delay(200);

  digitalWrite(playerOneLed[2], LOW);
  digitalWrite(playerTwoLed[2], LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(Serial.available() > 0)
  {
    incomingByte = Serial.read();
    // Get player positions
    if(incomingByte == 'P')
    {

      playerOnePosition = analogRead(playerOne);
      playerTwoPosition = analogRead(playerTwo);

      analogWrite(ledRed, (playerOnePosition / 1023.0f) * 255);
      analogWrite(ledYellow, (playerTwoPosition / 1023.0f) * 255);
      
      Serial.print(getPadded(playerOnePosition));
      Serial.print("-");
      Serial.println(getPadded(playerTwoPosition));
    }
    // Score for either 'L'eft or 'R'ight
    if(incomingByte == 'L' || incomingByte == 'R')
    {
      if(incomingByte == 'L')
      {
       playerOneScore++;
      }
      else
      {
        playerTwoScore++;
      }
      if(playerOneScore == 3 || playerTwoScore == 3)
      {
        resetGame();
      }
    }
    // Reset game if needed
    if(incomingByte == 'Q')
    {
      resetGame();
    }
    // Update LEDs at end of frame
    updateLeds(); 
  }
}

String getPadded(int num)
{
  char buff[5];
  char padded[6];

  sprintf(buff, "%.4u", num); // convert to 4 character string and save it to buff
  
  for(int i = 0; i < 5; i++)
    padded[i] = buff[i];
  padded[5] = '\0';

  return String(padded);

}
// Use for mapping from one range to another
float Remap(float value, float from1, float to1, float from2, float to2)
{
  return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
}
