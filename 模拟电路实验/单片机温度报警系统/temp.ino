#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include<OneWire.h>
#include<DallasTemperature.h>
#define BUS 7
OneWire onewire(BUS);
DallasTemperature sensors(&onewire);
LiquidCrystal_I2C lcd(0x27,16,2);  //配置LCD地址及行列

int pinBuzzer = 2;
int led = 5;
void setup()
{
  lcd.init(); //初始化LCD
  lcd.backlight(); //打开背光
  Serial.begin(9600);
  sensors.begin();
  pinMode(pinBuzzer,OUTPUT);
  pinMode(led,OUTPUT);
}

void loop()
{
  float val;
  sensors.requestTemperatures();
  val = sensors.getTempCByIndex(0);
  float temp = val;
  float farh =(temp*9)/5 + 32;
  lcd.setCursor(1,0);
  lcd.print("Temp(C)=");
  lcd.print(temp);
  lcd.print(".C");
  lcd.setCursor(1,1);
  lcd.print("Temp(F)=");
  lcd.print(farh);
  lcd.print("F");
  if (temp>30){
    digitalWrite(pinBuzzer, HIGH);
    digitalWrite(led, HIGH);
  }
  else{
    digitalWrite(pinBuzzer, LOW);
    digitalWrite(led, LOW);
  }
  delay(3000);//串口输出频率
}
