// simple project using Arduino UNO and 128x64 OLED Display to display a menu
// created by upir, 2022
// youtube channel: https://www.youtube.com/upir_upir

// YOUTUBE VIDEO: https://youtu.be/HVHVkKt-ldc
// YOUTUBE VIDEO u8g2 version: https://youtu.be/K5e0lFRvZ2E

// links from the video:
// Flipper Zero menu - https://docs.flipperzero.one/basics/control#M5BZO
// WOKWI start project progress bar - https://wokwi.com/projects/300867986768527882
// image2cpp - https://javl.github.io/image2cpp/
// 128x64 SSD1306 OLED Display: https://s.click.aliexpress.com/e/_DCKdvnh
// Transparent OLED display: https://s.click.aliexpress.com/e/_Dns6eLz
// Arduino UNO: https://s.click.aliexpress.com/e/_AXDw1h
// Arduino UNO MINI: https://store.arduino.cc/products/uno-mini-le
// Big OLED Display: https://s.click.aliexpress.com/e/_ADL0T9
// Arduino breadboard prototyping shield: https://s.click.aliexpress.com/e/_ApbCwx
// u8g fonts (fonts available for u8g library): https://nodemcu-build.com/u8g-fonts.php
// u8g documentation: https://github.com/olikraus/u8glib/wiki/userreference
// Photopea (online Photoshop-like tool): https://www.photopea.com/
// image2cpp (convert images into C code): https://javl.github.io/image2cpp/
// Push buttons - https://s.click.aliexpress.com/e/_DmXS8B9
// LCD displays: https://s.click.aliexpress.com/e/_DBgR45P
// u8g2 documentation: https://github.com/olikraus/u8g2/wiki/u8gvsu8g2
// Image Magick: https://imagemagick.org/index.php
// LCD Image converter: https://lcd-image-converter.riuson.com/en/about/

// Related videos:
// Arduino Parking Sensor - https://youtu.be/sEWw087KOj0
// Turbo pressure gauge with Arduino and OLED display - https://youtu.be/JXmw1xOlBdk
// Arduino Car Cluster with OLED Display - https://youtu.be/El5SJelwV_0
// Knob over OLED Display - https://youtu.be/SmbcNx7tbX8
// Arduino + OLED = 3D ? - https://youtu.be/kBAcaA7NAlA
// Arduino OLED Gauge - https://youtu.be/xI6dXTA02UQ
// Smaller & Faster Arduino - https://youtu.be/4GfPQoIRqW8

#include "U8g2lib.h"
#include <sensorHandle.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0); // [full framebuffer, size = 1024 bytes]

const int NUM_ITEMS = 8;        // number of items in the list and also the number of screenshots and screenshots with QR codes (other screens)
const int MAX_ITEM_LENGTH = 20; // maximum characters for the item name

// 'item_sel_outline', 128x21px
const unsigned char bitmap_item_sel_outline[] PROGMEM = {
    0xF8,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x03,
    0x04,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x04,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0xFC,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x07,
    0xF8,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x03,
};

char menu_items[NUM_ITEMS][MAX_ITEM_LENGTH] = { // array with item names
    {"Info"},
    {"Lengas"},
    {"Suhu"},
    {"EC"},
    {"Nitrogen(N)"},
    {"Phosporus(P)"},
    {"Potassium(K)"},
    {"Setup"}

};
// note - when changing the order of items above, make sure the other arrays referencing bitmaps
// also have the same order, for example array "bitmap_icons" for icons, and other arrays for screenshots and QR codes

//#define BUTTON_UP_PIN 25     // pin for UP button
#define BUTTON_SELECT_PIN 27 // pin for SELECT button
//#define BUTTON_DOWN_PIN 26   // pin for DOWN button

int button_up_clicked = 0;     // only perform action when button is clicked, and wait until another press
int button_select_clicked = 0; // same as above
int button_down_clicked = 0;   // same as above

int item_selected = 0; // which item in the menu is selected

int item_sel_previous; // previous item - used in the menu screen to draw the item before the selected one
int item_sel_next;     // next item - used in the menu screen to draw next item after the selected one

int current_screen = 0; // 0 = menu, 1 = screenshot, 2 = qr

int demo_mode = 0;       // when demo mode is set to 1, it automatically goes over all the screens, 0 = control menu with buttons
int demo_mode_state = 0; // demo mode state = which screen and menu item to display
int demo_mode_delay = 0; // demo mode delay = used to slow down the screen switching

void oled_init()
{
  u8g2.setColorIndex(1); // set the color to white
  u8g2.begin();
  u8g2.setBitmapMode(1);

  // define pins for buttons
  // INPUT_PULLUP means the button is HIGH when not pressed, and LOW when pressed
  // since it´s connected between some pin and GND
  //pinMode(BUTTON_UP_PIN, INPUT_PULLUP);     // up button
  pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP); // select button
  //pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);   // down button
}

void displayValue(String header, String konten, String satuan)
{
  String hd = "";
  String payload = "";
  if (header.length() < 14)
  {
    int a = 14 - header.length();
    if (a > 2)
    {
      for (int i = 0; i < a / 2; i++)
      {
        hd += " ";
      }
      hd += header;
    }
    else
    {
      hd = header;
    }
  }
  if (konten.length() <= 5)
  {
    int n = 5 - konten.length();

    for (int i = 0; i < n; i++)
    {
      payload += " ";
    }
    payload += konten;
  }
  else
  {
    payload = konten.substring(0, 5);
  }
  u8g2.setFont(u8g2_font_moosenooks_tr);
  u8g2.drawStr(0, 12, hd.c_str());

  u8g2.setFont(u8g2_font_logisoso32_tn);
  u8g2.drawStr(0, 60, payload.c_str());

  // u8g2_font_t0_12b_mr
  u8g2.setFont(u8g2_font_t0_12b_mr);
  u8g2.drawStr(100, 60, satuan.c_str());
}

void oled_loop()
{

  if (current_screen == 0)
  { // MENU SCREEN
  /*
    // up and down buttons only work for the menu screen
    if ((digitalRead(BUTTON_UP_PIN) == LOW) && (button_up_clicked == 0))
    {                                    // up button clicked - jump to previous menu item
      item_selected = item_selected - 1; // select previous item
      button_up_clicked = 1;             // set button to clicked to only perform the action once
      if (item_selected < 0)
      { // if first item was selected, jump to last item
        item_selected = NUM_ITEMS - 1;
      }
    }
    else if ((digitalRead(BUTTON_DOWN_PIN) == LOW) && (button_down_clicked == 0))
    {                                    // down button clicked - jump to next menu item
      item_selected = item_selected + 1; // select next item
      button_down_clicked = 1;           // set button to clicked to only perform the action once
      if (item_selected >= NUM_ITEMS)
      { // last item was selected, jump to first menu item
        item_selected = 0;
      }
    }

    if ((digitalRead(BUTTON_UP_PIN) == HIGH) && (button_up_clicked == 1))
    { // unclick
      button_up_clicked = 0;
    }
    if ((digitalRead(BUTTON_DOWN_PIN) == HIGH) && (button_down_clicked == 1))
    { // unclick
      button_down_clicked = 0;
    }
    */
  
  }

  if ((digitalRead(BUTTON_SELECT_PIN) == LOW) && (button_select_clicked == 0))
  {                            // select button clicked, jump between screens
    button_select_clicked = 1; // set button to clicked to only perform the action once
    if (++current_screen > 8)
    {
      current_screen = 0;
    }
  }
  if ((digitalRead(BUTTON_SELECT_PIN) == HIGH) && (button_select_clicked == 1))
  { // unclick
    button_select_clicked = 0;
  }

  // set correct values for the previous and next items
  item_sel_previous = item_selected - 1;
  if (item_sel_previous < 0)
  {
    item_sel_previous = NUM_ITEMS - 1;
  } // previous item would be below first = make it the last
  item_sel_next = item_selected + 1;
  if (item_sel_next >= NUM_ITEMS)
  {
    item_sel_next = 0;
  } // next item would be after last = make it the first

  u8g2.clearBuffer(); // clear buffer for storing display content in RAM

  switch (current_screen)
  {
  case 0:
    displayValue("PH", infoPH(), "  ");
    break;

  case 1: // temp
    displayValue("Temperature", infoTemp(), "°C");
    break;
  case 2: // lengas
    displayValue("Lengas", infoLengas(), "%");
    break;
  case 3: // EC
    displayValue("EC", infoEC(), "uS/m");
    break;
  case 4: // N
    displayValue("Nitrogen(N)", infoN(), "mg/Kg");
    break;
  case 5: // P
    displayValue("Phosporus(P)", infoP(), "mg/Kg");
    break;
  case 6: // K
    displayValue("Potassium(K)", infoK(), "mg/Kg");
    break;
  case 7: // 
    displayValue("Suhu udara", infoAirTemp(), "°C");
    break;
  case 8: // 
    displayValue("Humidity", infoAirHum(), "%");
    break;
    

  default:
    break;
  }
  /*
  if (current_screen == 0)
  { // MENU SCREEN

    // selected item background
    u8g2.drawXBMP(0, 22, 128, 21, bitmap_item_sel_outline);

    // draw previous item as icon + label
    u8g2.setFont(u8g_font_7x14);
    u8g2.drawStr(25, 15, menu_items[item_sel_previous]);
    // u8g2.drawXBMP(4, 2, 16, 16, bitmap_icons[item_sel_previous]);

    // draw selected item as icon + label in bold font
    u8g2.setFont(u8g_font_7x14B);
    u8g2.drawStr(25, 15 + 20 + 2, menu_items[item_selected]);
    // u8g2.drawXBMP(4, 24, 16, 16, bitmap_icons[item_selected]);

    // draw next item as icon + label
    u8g2.setFont(u8g_font_7x14);
    u8g2.drawStr(25, 15 + 20 + 20 + 2 + 2, menu_items[item_sel_next]);
    // u8g2.drawXBMP(4, 46, 16, 16, bitmap_icons[item_sel_next]);

    // draw scrollbar background
    // u8g2.drawXBMP(128 - 8, 0, 8, 64, bitmap_scrollbar_background);

    // draw scrollbar handle
    u8g2.drawBox(125, 64 / NUM_ITEMS * item_selected, 3, 64 / NUM_ITEMS);

    // draw upir logo
    // u8g2.drawXBMP(128 - 16 - 4, 64 - 4, 16, 4, upir_logo);
  }
  else if (current_screen == 1)
  { // SCREENSHOTS SCREEN
    // u8g2.drawXBMP( 0, 0, 128, 64, bitmap_screenshots[item_selected]); // draw screenshot
    // u8g2.clear();
    displayValue("Temperature", "26.70", "°C");
  }
  else if (current_screen == 2)
  { // QR SCREEN
    // u8g2.drawXBMP( 0, 0, 128, 64, bitmap_qr_codes[item_selected]); // draw qr code screenshot
    displayValue("Lengas", "67", "%");
  }
  */

  u8g2.sendBuffer(); // send buffer from RAM to display controller
}