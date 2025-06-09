#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

const int redLED = 8;
const int yellowLED = 9;
const int greenLED = 10;

// Gumbi
const int player1Button = 11;  // reakcija Igralec 1
const int player2Button = 12;  // reakcija Igralec 2
const int startButton = 13;    // samo za začetek igre

bool gameRunning = false;
bool greenLightOn = false;
unsigned long greenTime = 0;

void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  pinMode(player1Button, INPUT_PULLUP);
  pinMode(player2Button, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP); // pin 13 = edini START

  lcd.begin(16, 2);
  lcd.print("Stisni START");
  Serial.begin(9600);
}

void loop() {
  // Začetek igre samo s pinom 13
  if (!gameRunning && digitalRead(startButton) == LOW) {
    delay(50);
    if (digitalRead(startButton) == LOW) {
      startGame();
    }
  }
}

void startGame() {
  gameRunning = true;
  greenLightOn = false;

  lcd.clear();
  lcd.print("Pripravljeni...");
  delay(1000);

  digitalWrite(redLED, HIGH);
  delay(1000);
  digitalWrite(redLED, LOW);

  digitalWrite(yellowLED, HIGH);
  delay(1000);
  digitalWrite(yellowLED, LOW);

  // Anti-cheat čas pred zeleno
  int waitTime = random(2000, 5000);
  unsigned long preGreenStart = millis();

  while (millis() - preGreenStart < waitTime) {
    if (digitalRead(player1Button) == LOW) {
      showFalseStart(1);
      return;
    } else if (digitalRead(player2Button) == LOW) {
      showFalseStart(2);
      return;
    }
  }

  // ZELENA!
  digitalWrite(greenLED, HIGH);
  greenTime = millis();
  greenLightOn = true;
  lcd.clear();
  lcd.print("ZDAJ!");

  // Čakanje na odziv
  while (true) {
    if (digitalRead(player1Button) == LOW) {
      delay(20);
      if (digitalRead(player1Button) == LOW) {
        unsigned long reaction = millis() - greenTime;
        showWinner(1, reaction);
        return;
      }
    }
    if (digitalRead(player2Button) == LOW) {
      delay(20);
      if (digitalRead(player2Button) == LOW) {
        unsigned long reaction = millis() - greenTime;
        showWinner(2, reaction);
        return;
      }
    }
  }
}

void showWinner(int player, unsigned long reaction) {
  digitalWrite(greenLED, LOW);
  lcd.clear();
  lcd.print("Zmagovalec: ");
  lcd.print("Igralec ");
  lcd.print(player);
  lcd.setCursor(0, 1);
  lcd.print("Cas: ");
  lcd.print(reaction);
  lcd.print(" ms");
  delay(4000);
  resetGame();
}

void showFalseStart(int player) {
  lcd.clear();
  lcd.print("Igralec ");
  lcd.print(player);
  lcd.setCursor(0, 1);
  lcd.print("Prehitel!");
  delay(3000);
  resetGame();
}

void resetGame() {
  digitalWrite(greenLED, LOW);
  gameRunning = false;
  greenLightOn = false;
  lcd.clear();
  lcd.print("Stisni START");
}
