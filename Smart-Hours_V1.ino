/*
****************************************************************************************************************************************************
 
                                                                   Smart-Hours_V1


                                                                 by : vla-vi21 (inst)
 
 Author of libraries :  ("GyverButton.h") Alex Gyver
****************************************************************************************************************************************************
 */
#define BTN1 3        // кнопка подключена сюда (BTN_PIN --- КНОПКА --- GND)


#include "GyverButton.h" //подключаем библиотеку для кнопки
GButton butt1(BTN1, HIGH_PULL,  NORM_OPEN);//настраиваем кнопку (подробнее в самой библиотеке)

#include <Wire.h> //подключаем библиотеку для подсветки дисплея
#include <LiquidCrystal_I2C.h> //подключаем библиотеку для управления дисплеем
LiquidCrystal_I2C LCD(0x27, 16, 2); //указываем параметры дисплея


#include <iarduino_RTC.h> //подключаем библиотеку для модуля реального времени
iarduino_RTC time(RTC_DS1302, 8, 6, 7); // для модуля DS1302 - RST, CLK, DAT


#include <AHT10.h>  //подключаем библиотеку для датчика влажности и температуры
AHT10 myAHT20(AHT10_ADDRESS_0X38, AHT20_SENSOR); //настраиваем датчик


void setup() {
  delay(300); //задержка для часов

  LCD.init();  //инициализация подсветки дисплея (wire)
  LCD.backlight(); //инициализация дисплея
  LCD.clear();

  butt1.setDebounce(80); // установка времени антидребезга 
  butt1.setTimeout(300); // установка таймаута удержания

  time.begin(); //инициализация модуля реального времени
  //time.settime(0, 30, 18, 20, 9, 2, 1); // 0  сек, 30 мин, 18 часов, 20, сентябрь, 202, понедельник
}


void loop() {
  static byte clicks ; //переменная для счётчика нажатий
  butt1.tick(); //опрос кнопки


  if (butt1.isClick()) {        //нажатие кнопки
    if (++clicks >= 7) clicks = 0; //счётчик нажатий 
  }
  switch (clicks) { 
    case 1 : task0(); break;
    case 2 : LCD.clear(); break;
    case 3 : task1(); break;
    case 4 : LCD.clear(); break;
    case 5 : task2(); break;
    case 6 : LCD.clear(); break;
  }
}

void task0() { //функция для вывода время
  static uint32_t tmr;
  if (millis() - tmr >= 1000) { //таймер (мс)
    tmr = millis();
    LCD.setCursor(0, 0);
    LCD.print(time.gettime("d M Y, D"));
    LCD.setCursor(4, 1);
    LCD.print(time.gettime("H:i:s"));
  }
}
void task1() { //функция для вывода температуры
  static uint32_t tmr;
  if (millis() - tmr >= 5000) { //таймер (мс)
    tmr = millis();
    LCD.setCursor(2, 0);
    LCD.print("Temperature:");
    LCD.setCursor(5, 1);
    LCD.print( myAHT20.readTemperature());
    LCD.print(" C");
  }
}
void task2() { //функция для вывода влажности воздуха 
  static uint32_t tmr; 
  if (millis() - tmr >= 5000) { //таймер (мс)
    tmr = millis();
    LCD.setCursor(3, 0);
    LCD.print("Humidity:");
    LCD.setCursor(5, 1);
    LCD.print( myAHT20.readHumidity());
    LCD.print("%");
  }
}
