#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define Buzzer 9

SoftwareSerial HC12(10,11);

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int forwardBtn = 2;
const int backwardBtn = 3;
const int leftBtn = 4;
const int rightBtn = 5;
const int stopBtn = 6;

String receivedData = "";

void setup()
{
  Serial.begin(9600);
  HC12.begin(9600);

  lcd.init();
  lcd.backlight();

  pinMode(forwardBtn, INPUT_PULLUP);
  pinMode(backwardBtn, INPUT_PULLUP);
  pinMode(leftBtn, INPUT_PULLUP);
  pinMode(rightBtn, INPUT_PULLUP);
  pinMode(stopBtn, INPUT_PULLUP);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);

  lcd.setCursor(0,0);
  lcd.print("War Spy Robot");
  lcd.setCursor(0,1);
  lcd.print("Controller");
  delay(2000);
  lcd.clear();
}

void loop()
{
  if (digitalRead(forwardBtn) == LOW && digitalRead(stopBtn) == LOW)
  {
    HC12.write(6);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Requesting Data..");
    delay(500);
  }

  else if(digitalRead(forwardBtn) == LOW)
  {
    HC12.write(1);
    lcd.setCursor(0,0);
    lcd.print("Moving Forward ");
    delay(200);
  }

  else if(digitalRead(backwardBtn) == LOW)
  {
    HC12.write(2);
    lcd.setCursor(0,0);
    lcd.print("Moving Backward");
    delay(200);
  }

  else if(digitalRead(leftBtn) == LOW)
  {
    HC12.write(3);
    lcd.setCursor(0,0);
    lcd.print("Turning Left   ");
    delay(200);
  }

  else if(digitalRead(rightBtn) == LOW)
  {
    HC12.write(4);
    lcd.setCursor(0,0);
    lcd.print("Turning Right  ");
    delay(200);
  }

  else if(digitalRead(stopBtn) == LOW)
  {
    HC12.write(5);
    lcd.setCursor(0,0);
    lcd.print("Robot Stopped  ");
    delay(200);
  }

  while(HC12.available())
  {
    digitalWrite(Buzzer, HIGH);
    
    char incoming = HC12.read();

    if(incoming=='\n')
    {
      lcd.clear();
      // lcd.setCursor(0,0);
      // lcd.print("Robot Status:");
      lcd.setCursor(0,0);
      lcd.print(receivedData);
      receivedData="";
      delay(1000);
      digitalWrite(Buzzer, LOW);
    }
    else
    {
      receivedData += incoming;
    }
  }
}
