#include<LiquidCrystal_I2C_Hangul.h>
#include<Wire.h>
hw_timer_t * timer = NULL;
typedef struct object {
  int x; // postion
  int type; //0:<- , 1:->
  int live;  //0:alive, 1:death
} obj;
obj o1[5];
int i, j, c = 2, temp;
LiquidCrystal_I2C_Hangul lcd(0x27, 16, 2);
void IRAM_ATTR onTimer() {
  //lcd.setCursor(0,0);
  //lcd.print("X");
  //Serial.println("x");
  for (i = 0; i < c; i++) {
    if (  o1[i].live == 0) {
      if (o1[i].type == 0 && o1[i].x >= 0 )
        o1[i].x = o1[i].x - 1;
      else if (o1[i].type == 1 && o1[i].x < 16)
        o1[i].x = o1[i].x + 1;
      if (o1[i].type == 0 && o1[i].x == 0 )
        o1[i].live = 1;
      else if (o1[i].type == 1 && o1[i].x == 15 )
        o1[i].live = 1;
    }
  }
  for (i = 0; i < c; i++) {
    if (o1[i].live == 0) {
      temp = o1[i].x;
      for (j = i + 1; j < c; j++) {
        if (o1[j].x == temp) {
          o1[j].live = 1;
          o1[i].live = 1;
        }
        if (o1[i].type == 1 && o1[j].type == 0  && o1[j].x == (temp + 1)) {
          o1[j].live = 1;
          o1[i].live = 1;
        } else if (o1[i].type == 0 && o1[j].type == 1 && o1[j].x == (temp + 1)) {
          o1[j].live = 1;
          o1[i].live = 1;
        }
      }
    }
  }
}
void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  o1[0].x = 15; // postion在10
  o1[0].type = 0; //<-
  o1[0].live = 0;
  o1[1].x = 0; // postion在10
  o1[1].type = 1; //->
  o1[0].live = 0;
  timer = timerBegin(0, 80, true); //1us
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}
void loop() {
  //lcd.clear();
  for (i = 0; i < c; i++) {
    lcd.setCursor(o1[i].x, 0);
    if (o1[i].type == 0 && o1[i].live == 0)
      lcd.print("< ");
    else  if (o1[i].type == 1 && o1[i].live == 0)
      lcd.print(" >");
    else if ( o1[i].live == 1)
      if (o1[i].type == 0 )
        lcd.print("  ");
      else if (o1[i].type == 1 ) {
        lcd.setCursor(o1[i].x + 1, 0);
        lcd.print("  ");
      }
  }
}