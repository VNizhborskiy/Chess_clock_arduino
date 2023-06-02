#include <U8g2lib.h>
#include <EncButton.h>

U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, /* CS=*/10, /* reset=*/8);

EncButton<EB_TICK, 7> btn1;     // button pl1
EncButton<EB_TICK, 6> btn2;     // button pl2
EncButton<EB_TICK, 5> btn3;     // button pause
EncButton<EB_TICK, 4> btn_pl1;  // button game pl1
EncButton<EB_TICK, 3> btn_pl2;  // button game pl2


static const unsigned char battery1[] U8X8_PROGMEM = {

  0xfe, 0xff, 0x01, 0xfe, 0xff, 0x01, 0x06, 0x80, 0x07, 0x06, 0x80, 0x07,
  0x06, 0x80, 0x07, 0x06, 0x80, 0x07, 0x06, 0x80, 0x07, 0x06, 0x80, 0x07,
  0xfe, 0xff, 0x01, 0xfe, 0xff, 0x01
};
static const unsigned char battery2[] U8X8_PROGMEM = {
  0xfe, 0xff, 0x01, 0xfe, 0xff, 0x01, 0x7e, 0x80, 0x07, 0x7e, 0x80, 0x07,
  0x7e, 0x80, 0x07, 0x7e, 0x80, 0x07, 0x7e, 0x80, 0x07, 0x7e, 0x80, 0x07,
  0xfe, 0xff, 0x01, 0xfe, 0xff, 0x01

};
static const unsigned char battery3[] U8X8_PROGMEM = {
  0xfe, 0xff, 0x01, 0xfe, 0xff, 0x01, 0xfe, 0x87, 0x07, 0xfe, 0x87, 0x07,
  0xfe, 0x87, 0x07, 0xfe, 0x87, 0x07, 0xfe, 0x87, 0x07, 0xfe, 0x87, 0x01,
  0xfe, 0xff, 0x01, 0xfe, 0xff, 0x01

};
static const unsigned char battery4[] U8X8_PROGMEM = {
  0xfe, 0xff, 0x01, 0xfe, 0xff, 0x01, 0xfe, 0xff, 0x07, 0xfe, 0xff, 0x07,
  0xfe, 0xff, 0x07, 0xfe, 0xff, 0x07, 0xfe, 0xff, 0x07, 0xfe, 0xff, 0x07,
  0xfe, 0xff, 0x01, 0xfe, 0xff, 0x01

};


int count1;
int count2;

unsigned long timing;

int min_us1, sec_us1, min_us2, sec_us2, inc;


int g1 = 1;
int g2 = 1;

void (*resetFunc)(void) = 0;

void new_custom_time(char cursor_num1, char cursor_num2, char cursor_num3, char cursor_num4, char cursor_num5, char cursor_num6, char cursor_num7, char cursor_num8, char cursor_i) {
  do {


    u8g2.setCursor(19, 10);
    u8g2.setFont(u8g2_font_Georgia7px_tr);
    u8g2.print("New custom time");
    u8g2.setCursor(41, 60);
    u8g2.setFont(u8g2_font_chikita_tr);
    u8g2.print(" Increment: ");

    u8g2.setFont(u8g2_font_chikita_tr);
    u8g2.setCursor(7, 23);
    u8g2.print(cursor_num1);
    u8g2.setCursor(20, 23);
    u8g2.print(cursor_num2);
    u8g2.setCursor(39, 23);
    u8g2.print(cursor_num3);
    u8g2.setCursor(52, 23);
    u8g2.print(cursor_num4);

    u8g2.setCursor(71, 23);
    u8g2.print(cursor_num5);
    u8g2.setCursor(84, 23);
    u8g2.print(cursor_num6);
    u8g2.setCursor(103, 23);
    u8g2.print(cursor_num7);
    u8g2.setCursor(116, 23);
    u8g2.print(cursor_num8);

    u8g2.setCursor(35, 60);
    u8g2.print(cursor_i);
  } while (u8g2.nextPage());
}

int r = 0;  // счетчик для перехода из presets в draw_num()
int w = 0;  // счетчик для переключения курсора presets()
void presets(char cursor1, char cursor2, char cursor3, char cursor4) {

  do {
    u8g2.drawFrame(0, 0, 128, 64);
    u8g2.setCursor(46, 13);
    u8g2.setFont(u8g2_font_Georgia7px_tr);
    u8g2.print("Presets");

    u8g2.setCursor(4, 24);
    u8g2.setFont(u8g2_font_chikita_tr);
    u8g2.print(cursor1);
    u8g2.setCursor(10, 24);
    u8g2.setFont(u8g2_font_chikita_tr);
    u8g2.print(" Bullet 1+0");

    u8g2.setCursor(4, 35);
    u8g2.setFont(u8g2_font_chikita_tr);
    u8g2.print(cursor2);
    u8g2.setCursor(10, 35);
    u8g2.setFont(u8g2_font_chikita_tr);
    u8g2.print(" Blitz 3+2");

    u8g2.setCursor(4, 46);
    u8g2.setFont(u8g2_font_chikita_tr);
    u8g2.print(cursor3);
    u8g2.setCursor(10, 46);
    u8g2.setFont(u8g2_font_chikita_tr);
    u8g2.print(" Rapid 10+0");

    u8g2.setCursor(4, 57);
    u8g2.setFont(u8g2_font_chikita_tr);
    u8g2.print(cursor4);
    u8g2.setCursor(10, 57);
    u8g2.setFont(u8g2_font_chikita_tr);
    u8g2.print(" Classical 30+0");

  } while (u8g2.nextPage());
}

/*void user_draw(int min_user1, int sec_user1, int min_user2, int sec_user2, int incr_user) {
  
  do {
    u8g2.drawFrame(0, 0, 128, 64);
    u8g2.setFont(u8g2_font_utopia24_tn);
    u8g2.setCursor(3, 41);

    u8g2.print(min_user1);
    u8g2.print(":");

    u8g2.print(sec_user1);

    u8g2.setCursor(67, 41);

    u8g2.print(min_user2);
    u8g2.print(":");

    u8g2.print(sec_user2);

    u8g2.drawLine(63, 14, 63, 50);
    u8g2.drawLine(64, 14, 64, 50);

    u8g2.setFont(u8g2_font_finderskeepers_tn);
    u8g2.setCursor(92, 60);
    u8g2.print(incr_user);

  } while (u8g2.nextPage());
}*/

void draw_num(int min1, int sec1, int min2, int sec2, char s_min1, char s_sec1, char s_min2, char s_sec2) {
  // Рисование нулей

  if (min1 < 10) {
    s_min1 = '0';
  } else if (min1 >= 10) {
    s_min1 = '\0';
  }
  if (sec1 < 10) {
    s_sec1 = '0';
  } else if (sec1 >= 10) {
    s_sec1 = '\0';
  }
  if (min2 < 10) {
    s_min2 = '0';
  } else if (min2 >= 10) {
    s_min2 = '\0';
  }
  if (sec2 < 10) {
    s_sec2 = '0';
  } else if (sec2 >= 10) {
    s_sec2 = '\0';
  }
  u8g2.firstPage();
  do {
    u8g2.drawFrame(0, 0, 128, 64);
    //u8g2.setFont(u8g2_font_7x13_te); // 54%
    //u8g2.setFont(u8g2_font_7x13_mf); // 37%
    //u8g2.setFont(u8g2_font_7x13_mr); // 33%
    //u8g2.setFont(u8g2_font_tenthinnerguys_tu); // 30%
    //u8g2.setFont(u8g2_font_tooseornament_tn); // 29%

    //u8g2_font_timB18_tn
    //u8g2.setFont(u8g2_font_utopia24_tn);
    //u8g2.setFont();
    u8g2.setFont(u8g2_font_utopia24_tn);
    u8g2.setCursor(3, 41);
    u8g2.print(s_min1);
    u8g2.print(min1);
    u8g2.print(":");
    u8g2.print(s_sec1);
    u8g2.print(sec1);

    u8g2.setCursor(67, 41);
    u8g2.print(s_min2);
    u8g2.print(min2);
    u8g2.print(":");
    u8g2.print(s_sec2);
    u8g2.print(sec2);

    u8g2.drawLine(63, 14, 63, 50);
    u8g2.drawLine(64, 14, 64, 50);

  } while (u8g2.nextPage());
}


void Timer(int minutes1, int seconds1, int minutes2, int seconds2, char symb_min1, char symb_sec1, char symb_min2, char symb_sec2, int increment) {
  int x = 1;  // счетчик для btn1, btn2
  int i = 1;  // счетчк для increment
  //int p = 0;

  while (true) {
    btn_pl1.tick();
    btn_pl2.tick();
    draw_num(minutes1, seconds1, minutes2, seconds2, '\0', '\0', '\0', '\0');
    if (btn_pl1.press()) {
      x = 2;
      break;
    } else if (btn_pl2.press()) {
      x = 1;
      break;
    }
  }
  while (true) {

    if (btn3.click()) {  // pause + reset

      u8g2.firstPage();
      do {
        draw_num(minutes1, seconds1, minutes2, seconds2, '\0', '\0', '\0', '\0');
        u8g2.setFont(u8g2_font_4x6_tr);
        u8g2.setCursor(55, 62);
        u8g2.print("PAUSE");

      } while (u8g2.nextPage());


      while (true) {

        btn3.tick();
        if (btn3.click()) {
          break;
        }
        if (btn3.hold()) {
          resetFunc();
        }
      }
    }
    draw_num(minutes1, seconds1, minutes2, seconds2, '\0', '\0', '\0', '\0');
    if (minutes1 == 0 && seconds1 == 0) {
      do {
        draw_num(minutes1, seconds1, minutes2, seconds2, '\0', '\0', '\0', '\0');
      } while (u8g2.nextPage());
      digitalWrite(8, LOW);
      delay(500);
      digitalWrite(8, HIGH);
      delay(500);
      digitalWrite(8, LOW);
      delay(500);
      digitalWrite(8, HIGH);
      delay(500);
      digitalWrite(8, LOW);
      delay(500);
      digitalWrite(8, HIGH);
      delay(500);
      while (true) {
        btn3.tick();
        if (btn3.click()) {
          resetFunc();
        }
      }
    }
    if (minutes2 == 0 && seconds2 == 0) {
      do {
        draw_num(minutes1, seconds1, minutes2, seconds2, '\0', '\0', '\0', '\0');
      } while (u8g2.nextPage());
      digitalWrite(8, LOW);
      delay(500);
      digitalWrite(8, HIGH);
      delay(500);
      digitalWrite(8, LOW);
      delay(500);
      digitalWrite(8, HIGH);
      delay(500);
      digitalWrite(8, LOW);
      delay(500);
      digitalWrite(8, HIGH);
      delay(500);
      while (true) {
        btn3.tick();
        if (btn3.click()) {
          resetFunc();
        }
      }
    }
    //btn1.tick();
    //btn2.tick();
    btn3.tick();
    btn_pl1.tick();
    btn_pl2.tick();
    if (millis() - timing > 1000) {
      timing = millis();


      if (btn_pl1.press() || btn_pl2.press()) {
        x++;
        if (increment != 0) {
          i++;
        }
      }

      if (x % 2 != 0 && seconds1 != -1) {
        Serial.println(i);
        if (i > 1 && x > 1) {
          seconds2 += increment;
          i = 1;

          switch (seconds2) {
            case 60:
              seconds2 = 0;
              minutes2++;
              break;
            case 61:
              seconds2 = 1;
              minutes2++;
              break;
            case 62:
              seconds2 = 2;
              minutes2++;
              break;
            case 63:
              seconds2 = 3;
              minutes2++;
              break;
            case 64:
              seconds2 = 4;
              minutes2++;
              break;
            case 65:
              seconds2 = 5;
              minutes2++;
              break;
          }

          //continue;
        }
        seconds1--;
      } else if (x % 2 == 0 && seconds2 != -1) {
        if (i > 1 && x > 1) {
          seconds1 += increment;
          i = 1;

          switch (seconds1) {
            case 60:
              seconds1 = 0;
              minutes1++;
              break;
            case 61:
              seconds1 = 1;
              minutes1++;
              break;
            case 62:
              seconds1 = 2;
              minutes1++;
              break;
            case 63:
              seconds1 = 3;
              minutes1++;
              break;
            case 64:
              seconds1 = 4;
              minutes1++;
              break;
            case 65:
              seconds1 = 5;
              minutes1++;
              break;
          }

          //continue;
        }
        seconds2--;
      }

      // Проверки
      if (seconds1 == -1 && minutes1 != 0 && x % 2 != 0) {
        minutes1--;
        seconds1 = 59;

      }

      else if (seconds2 == -1 && minutes2 != 0 && x % 2 == 0) {
        minutes2--;
        seconds2 = 59;
      }
      //
      switch (seconds1) {
        case 60:
          seconds1 = 0;
          minutes1++;
          break;
        case 61:
          seconds1 = 1;
          minutes1++;
          break;
        case 62:
          seconds1 = 2;
          minutes1++;
          break;
        case 63:
          seconds1 = 3;
          minutes1++;
          break;
        case 64:
          seconds1 = 4;
          minutes1++;
          break;
        case 65:
          seconds1 = 5;
          minutes1++;
          break;
      }
      switch (seconds2) {
        case 60:
          seconds2 = 0;
          minutes2++;
          break;
        case 61:
          seconds2 = 1;
          minutes2++;
          break;
        case 62:
          seconds2 = 2;
          minutes2++;
          break;
        case 63:
          seconds2 = 3;
          minutes2++;
          break;
        case 64:
          seconds2 = 4;
          minutes2++;
          break;
        case 65:
          seconds2 = 5;
          minutes2++;
          break;
      }
    }
  }
}

int i = 0;  // счетчик для перехода из меню в след страницу
int k = 0;  // счетчик для переключения курсора menu()

void menu() {

  if (k % 2 != 0) {
    i++;

    //Serial.println(k);
    u8g2.clearDisplay();
    do {
      battery_charge();
      u8g2.drawFrame(0, 0, 128, 64);
      u8g2.setCursor(48, 14);
      u8g2.setFont(u8g2_font_Georgia7px_tr);
      u8g2.print("MENU");
      u8g2.setCursor(10, 30);
      u8g2.setFont(u8g2_font_chikita_tr);
      u8g2.print("# Presets");
      u8g2.setCursor(16, 45);
      u8g2.setFont(u8g2_font_chikita_tr);
      u8g2.print(" New custom time");
    } while (u8g2.nextPage());
  }

  else if (k % 2 == 0 && k != 0) {
    i++;

    //Serial.println(k);
    u8g2.clearDisplay();
    do {
      battery_charge();
      u8g2.drawFrame(0, 0, 128, 64);
      u8g2.setCursor(48, 14);
      u8g2.setFont(u8g2_font_Georgia7px_tr);
      u8g2.print("MENU");
      u8g2.setCursor(14, 30);
      u8g2.setFont(u8g2_font_chikita_tr);
      u8g2.print("  Presets");
      u8g2.setCursor(10, 45);
      u8g2.setFont(u8g2_font_chikita_tr);
      u8g2.print("# New custom time");
    } while (u8g2.nextPage());
  }
}

void battery_charge() {
  float Vbat = (analogRead(A1) * 5.35) / 1024.0;
  u8g2.setCursor(100, 11);
  u8g2.setFont(u8g2_font_chikita_tr);
  u8g2.print(Vbat);
  u8g2.print('V');
  if (Vbat > 3.0 && Vbat < 3.3) {
    do {
      u8g2.drawXBMP(5, 4, 20, 10, battery1);
    } while (u8g2.nextPage());
  }
  if (Vbat > 3.3 && Vbat < 3.6) {
    do {
      u8g2.drawXBMP(5, 4, 20, 10, battery2);
    } while (u8g2.nextPage());
  }
  if (Vbat > 3.6 && Vbat < 3.9) {
    do {
      u8g2.drawXBMP(5, 4, 20, 10, battery3);
    } while (u8g2.nextPage());
  }
  if (Vbat > 3.9 && Vbat < 4.2) {
    do {
      u8g2.drawXBMP(5, 4, 20, 10, battery4);
    } while (u8g2.nextPage());
  }
}

void setup(void) {
  Serial.begin(9600);
  u8g2.begin();
  u8g2.firstPage();
  battery_charge();
  do {


    u8g2.drawFrame(0, 0, 128, 64);
    u8g2.setCursor(48, 14);
    u8g2.setFont(u8g2_font_Georgia7px_tr);
    u8g2.print("MENU");
    u8g2.setCursor(16, 30);
    u8g2.setFont(u8g2_font_chikita_tr);
    u8g2.print(" Presets");
    u8g2.setCursor(16, 45);
    u8g2.setFont(u8g2_font_chikita_tr);
    u8g2.print(" New custom time");
  } while (u8g2.nextPage());
}

void loop(void) {

  btn3.tick();
  if (btn3.click()) {
    k++;
    menu();
  }

  if (btn3.hold() && i % 2 != 0) {
    u8g2.clearDisplay();
    presets('\0', '\0', '\0', '\0');

    while (true) {
      btn3.tick();
      if (btn3.hold() && r != 0) {


        switch (r) {
          case 1:
            u8g2.clearDisplay();
            draw_num(1, 0, 1, 0, '\0', '\0', '\0', '\0');
            while (true) {

              u8g2.clearDisplay();
              Timer(1, 0, 1, 0, '\0', '\0', '\0', '\0', 0);
            }
            break;
          case 2:
            u8g2.clearDisplay();
            draw_num(3, 0, 3, 0, '\0', '\0', '\0', '\0');
            while (true) {
              u8g2.clearDisplay();
              Timer(3, 0, 3, 0, '\0', '\0', '\0', '\0', 2);
            }
            break;
          case 3:
            u8g2.clearDisplay();
            draw_num(10, 0, 10, 0, '\0', '\0', '\0', '\0');
            while (true) {

              u8g2.clearDisplay();
              Timer(10, 0, 10, 0, '\0', '\0', '\0', '\0', 0);
            }
            break;
          case 4:
            u8g2.clearDisplay();
            draw_num(30, 0, 30, 0, '\0', '\0', '\0', '\0');
            while (true) {


              u8g2.clearDisplay();
              Timer(30, 0, 30, 0, '\0', '\0', '\0', '\0', 0);
            }
            break;
        }
      }

      if (btn3.click()) {
        w++;
        if (w == 5) {
          w = 1;
          u8g2.clearDisplay();
        }
        switch (w) {
          case 1:
            presets('#', '\0', '\0', '\0');
            r = 1;
            break;
          case 2:
            u8g2.clearDisplay();
            presets('\0', '#', '\0', '\0');
            r = 2;
            break;
          case 3:
            u8g2.clearDisplay();
            presets('\0', '\0', '#', '\0');
            r = 3;
            break;
          case 4:
            u8g2.clearDisplay();
            presets('\0', '\0', '\0', '#');
            r = 4;
            break;
        }
      }
    }
  }
  if (btn3.hold() && i % 2 == 0 && i != 0) {

    u8g2.clearDisplay();
    draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
    new_custom_time('\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n');
    while (true) {
      btn1.tick();
      btn2.tick();
      btn3.tick();

      if (btn2.click() || count1 == 1) {
        count1 = 0;

        switch (g1) {

          case 1:

            u8g2.clearDisplay();
            draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
            new_custom_time('#', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n');
            while (true) {
              btn1.tick();
              btn2.tick();
              btn3.tick();
              if (btn3.click()) {

                min_us1 += 10;
                if (min_us1 > 100 || min_us1 == 100) {
                  min_us1 = 0;
                }
                u8g2.clearDisplay();
                draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
                new_custom_time('#', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n');
              }
              if (btn2.click()) {
                count1++;

                break;
              } else if (btn1.click()) {

                count2++;
                break;
              }
            }


            g1++;
            break;
          case 2:
            u8g2.clearDisplay();
            draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
            new_custom_time('\n', '#', '\n', '\n', '\n', '\n', '\n', '\n', '\n');
            while (true) {
              btn1.tick();
              btn2.tick();
              btn3.tick();
              if (btn3.click()) {

                min_us1 += 1;
                if (min_us1 > 99) {
                  min_us1 = 0;
                }
                u8g2.clearDisplay();
                draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
                new_custom_time('\n', '#', '\n', '\n', '\n', '\n', '\n', '\n', '\n');
              }
              if (btn2.click()) {
                count1++;

                break;
              } else if (btn1.click()) {

                count2++;
                break;
              }
            }
            g1++;
            break;
          case 3:
            u8g2.clearDisplay();
            draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
            new_custom_time('\n', '\n', '#', '\n', '\n', '\n', '\n', '\n', '\n');
            while (true) {
              btn1.tick();
              btn2.tick();
              btn3.tick();
              if (btn3.click()) {
                sec_us1 += 10;
                if (sec_us1 > 59 || sec_us1 == 60) {
                  sec_us1 = 0;
                }
                u8g2.clearDisplay();
                draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
                new_custom_time('\n', '\n', '#', '\n', '\n', '\n', '\n', '\n', '\n');
              }
              if (btn2.click()) {
                count1++;

                break;
              } else if (btn1.click()) {

                count2++;
                break;
              }
            }
            g1++;
            break;
          case 4:
            u8g2.clearDisplay();
            draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
            new_custom_time('\n', '\n', '\n', '#', '\n', '\n', '\n', '\n', '\n');
            while (true) {
              btn1.tick();
              btn2.tick();
              btn3.tick();
              if (btn3.click()) {
                sec_us1 += 1;
                if (sec_us1 > 59) {
                  sec_us1 = 0;
                }

                u8g2.clearDisplay();
                draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
                new_custom_time('\n', '\n', '\n', '#', '\n', '\n', '\n', '\n', '\n');
              }
              if (btn2.click()) {
                count1++;

                break;
              } else if (btn1.click()) {

                count2++;
                break;
              }
            }
            g1++;
            break;
          case 5:
            u8g2.clearDisplay();
            draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
            new_custom_time('\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '#');
            while (true) {
              btn1.tick();
              btn2.tick();
              btn3.tick();
              if (btn3.click()) {
                Serial.println("Increment pl1 exist");
                inc++;
                if (inc > 5) {
                  inc = 0;
                }
                u8g2.clearDisplay();


                draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
                new_custom_time('\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '#');
                do {
                  u8g2.setFont(u8g2_font_finderskeepers_tn);
                  u8g2.setCursor(92, 60);
                  u8g2.print(inc);
                } while (u8g2.nextPage());
              }
              if (btn2.click()) {
                count1++;

                break;
              } else if (btn1.click()) {

                count2++;
                break;
              } else if (btn3.hold()) {
                u8g2.clearDisplay();
                //Serial.println(s_min_us1);

                draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
                while (true) {

                  u8g2.clearDisplay();
                  Timer(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n', inc);
                }
              }
            }
            g1 = 1;
            break;
        }
      }
      if (btn1.click() || count2 == 1) {
        count2 = 0;

        switch (g2) {

          case 1:

            u8g2.clearDisplay();
            draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
            new_custom_time('\n', '\n', '\n', '\n', '#', '\n', '\n', '\n', '\n');
            while (true) {
              btn1.tick();
              btn2.tick();
              btn3.tick();
              if (btn3.click()) {
                min_us2 += 10;
                if (min_us2 > 100 || min_us2 == 100) {
                  min_us2 = 0;
                }
                u8g2.clearDisplay();
                draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
                new_custom_time('\n', '\n', '\n', '\n', '#', '\n', '\n', '\n', '\n');
              }
              if (btn2.click()) {
                count1++;

                break;
              } else if (btn1.click()) {

                count2++;
                break;
              }
            }
            g2++;
            break;
          case 2:
            u8g2.clearDisplay();
            draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
            new_custom_time('\n', '\n', '\n', '\n', '\n', '#', '\n', '\n', '\n');
            while (true) {
              btn1.tick();
              btn2.tick();
              btn3.tick();
              if (btn3.click()) {
                min_us2 += 1;
                if (min_us2 > 99) {
                  min_us2 = 0;
                }
                u8g2.clearDisplay();
                draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
                new_custom_time('\n', '\n', '\n', '\n', '\n', '#', '\n', '\n', '\n');
              }
              if (btn2.click()) {
                count1++;

                break;
              } else if (btn1.click()) {

                count2++;
                break;
              }
            }
            g2++;
            break;
          case 3:
            u8g2.clearDisplay();
            draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
            new_custom_time('\n', '\n', '\n', '\n', '\n', '\n', '#', '\n', '\n');
            while (true) {
              btn1.tick();
              btn2.tick();
              btn3.tick();
              if (btn3.click()) {
                sec_us2 += 10;
                if (sec_us2 > 59 || sec_us2 == 60) {
                  sec_us2 = 0;
                }
                u8g2.clearDisplay();
                draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
                new_custom_time('\n', '\n', '\n', '\n', '\n', '\n', '#', '\n', '\n');
              }
              if (btn2.click()) {
                count1++;

                break;
              } else if (btn1.click()) {

                count2++;
                break;
              }
            }
            g2++;
            break;
          case 4:
            u8g2.clearDisplay();
            draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
            new_custom_time('\n', '\n', '\n', '\n', '\n', '\n', '\n', '#', '\n');
            while (true) {
              btn1.tick();
              btn2.tick();
              btn3.tick();
              if (btn3.click()) {
                sec_us2 += 1;
                if (sec_us2 > 59) {
                  sec_us2 = 0;
                }
                u8g2.clearDisplay();
                draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
                new_custom_time('\n', '\n', '\n', '\n', '\n', '\n', '\n', '#', '\n');
              }
              if (btn2.click()) {
                count1++;

                break;
              } else if (btn1.click()) {

                count2++;
                break;
              }
            }
            g2++;
            break;
          case 5:
            u8g2.clearDisplay();
            draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
            new_custom_time('\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '#');
            while (true) {
              btn1.tick();
              btn2.tick();
              btn3.tick();
              if (btn3.click()) {
                //Serial.println("Increment pl2 exist");
                inc++;
                if (inc > 5) {
                  inc = 0;
                }
                u8g2.clearDisplay();

                draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
                new_custom_time('\n', '\n', '\n', '\n', '\n', '\n', '\n', '\n', '#');
                do {
                  u8g2.setFont(u8g2_font_finderskeepers_tn);
                  u8g2.setCursor(92, 60);
                  u8g2.print(inc);
                } while (u8g2.nextPage());
              }
              if (btn2.click()) {
                count1++;

                break;
              } else if (btn1.click()) {

                count2++;
                break;
              } else if (btn3.hold()) {

                draw_num(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n');
                while (true) {


                  u8g2.clearDisplay();
                  Timer(min_us1, sec_us1, min_us2, sec_us2, '\n', '\n', '\n', '\n', inc);
                }
              }
            }
            g2 = 1;
            break;
        }
      }
    }
  }
}
// 29.05 4:32