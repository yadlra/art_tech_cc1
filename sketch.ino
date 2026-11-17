
#include <Wire.h>
#include <hd44780.h> // you will need to import this library; search hd44780 by Bill Perry
#include <hd44780ioClass/hd44780_I2Cexp.h>

hd44780_I2Cexp lcd;

const int tilt = 2;
bool lastPressed = false;

const char* answers[] = {
  "Yes!",
  "No!",
  "Maybe",
  "Ask again",
  "Definitely!",
  "Not today"
};

void setup() {
  pinMode(tilt, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.backlight();

  randomSeed(analogRead(A0));

  lcd.print("MAGIC 8 BALL");
  lcd.setCursor(0, 1);
  lcd.print("Tilt to ask!");
}

void loop() {
  bool pressed = digitalRead(tilt) == LOW;

  if (pressed && !lastPressed) {
    lcd.clear();
    lcd.print("Thinking...");
    delay(500);

    int answer = random(0, 6);

    lcd.clear();
    lcd.print(answers[answer]);
  }

  lastPressed = pressed;
  delay(25);
}
