#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#include "bitmaps.h"

#define TFT_CS    10
#define TFT_DC     9
#define TFT_RST   14
#define TFT_SCLK  12
#define TFT_MOSI  11

SPIClass mySPI(FSPI);
Adafruit_ST7789 tft = Adafruit_ST7789(&mySPI, TFT_CS, TFT_DC, TFT_RST);

// Touch pins
#define PIN_A      3
#define PIN_B      8
#define PIN_UP     7
#define PIN_DOWN   5
#define PIN_LEFT   6
#define PIN_RIGHT  4

struct TouchButton {
  const char* label;
  int pin;
  int baseline;
  int raw;
  int diff;
  int threshold;
};

TouchButton buttons[] = {
  {"A",     PIN_A,     0, 0, 0, 18},
  {"B",     PIN_B,     0, 0, 0, 18},
  {"UP",    PIN_UP,    0, 0, 0, 18},
  {"DOWN",  PIN_DOWN,  0, 0, 0, 18},
  {"LEFT",  PIN_LEFT,  0, 0, 0, 18},
  {"RIGHT", PIN_RIGHT, 0, 0, 0, 18},
};

const int NUM_BUTTONS = sizeof(buttons) / sizeof(buttons[0]);
int main_selection = 0; //for current cursor position in main menu
int bonus_selection = 0; //for current cursor position in bonus menu
const int BONUS_MAX = 2; //change this constant if you add more options for the bonus menu
int scene = 0; //keeps track of current "scene"
              //0 for main menu
              //1 for How To
              //2 for Cyber
              //3 for EE
              //4 for CPE
              //5 for Bonus
              //6 for About
              //7 for ECE Faculty page
              //8 for QR code page
int colorIndex = 0;
int faculty = 0;

void calibrateTouch() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    long sum = 0;
    for (int j = 0; j < 40; j++) {
      sum += touchRead(buttons[i].pin);
      delay(10);
    }
    buttons[i].baseline = sum / 40;
  }
}

void updateTouchReadings() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].raw = touchRead(buttons[i].pin);
    buttons[i].diff = buttons[i].baseline - buttons[i].raw;

    if (buttons[i].diff < 0) {
      buttons[i].diff = 0;
    }
  }
}

bool touchTest(int i) {
  return buttons[i].diff < buttons[i].threshold;
}

void displayLogo() {
  tft.drawBitmap(0,0,uah_logo,320,170,ST77XX_WHITE);
}

void drawGithub() {
  tft.fillScreen(ST77XX_BLACK);
  tft.drawBitmap(85,10,github,150,150,ST77XX_WHITE);
}

void drawFaculty(int f) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  __uint16_t faceColor = ST77XX_WHITE;
  switch(f) {
    case 0: //Dr. Beal
      tft.drawBitmap(60,0,beal,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Beal");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("ASSOCIATE PROFESSOR");
      break;
    case 1: //Dr. Bhadani
      tft.drawBitmap(60,0,bhadani,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Bhadani");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("ASSISTANT PROFESSOR");
      break;
    case 2: //Dr. Blakely
      tft.drawBitmap(60,0,blakely,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Blakely");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("ASSISTANT PROFESSOR");
      break;
    case 3: //Bowman
      tft.drawBitmap(60,0,bowman,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Ron Bowman");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("SENIOR LECTURER");
      break;
    case 4: //Dr. Boykin
      tft.drawBitmap(60,0,boykin,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Boykin");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("PROFESSOR");
      break;
    case 5: //Dr. Coe
      tft.drawBitmap(60,0,coe,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Coe");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("ASSOCIATE PROFESSOR");
      break;
    case 6: //Dr. English
      tft.drawBitmap(60,0,english,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. English");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("ASSOCIATE DEAN OF");
      tft.setCursor(10, 60);
      tft.print("UNDERGRADUATE AFFAIRS,");
      tft.setCursor(10, 70);
      tft.print("ASSOCIATE PROFESSOR");
      break;
    case 7: //Dr. Gaede
      tft.drawBitmap(60,0,gaede,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Gaede");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("ASSOCIATE PROVOST,");
      tft.setCursor(10, 60);
      tft.print("ASSOCIATE PROFESSOR");
      break;
    case 8: //Gaur
      tft.drawBitmap(60,0,gaur,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Gaur");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("ASSISTANT PROFESSOR");
      break;
    case 9: //Dr. Guo
      tft.drawBitmap(60,0,guo,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Guo");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("PROFESSOR");
      break;
    case 10: //Hite
      tft.drawBitmap(60,0,hite,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dennis Hite");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("SENIOR LECTURER");
      break;
    case 11: //Dr. Joiner
      tft.drawBitmap(60,0,joiner,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Joiner");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("ASSOCIATE DEPARTMENT CHAIR,");
      tft.setCursor(10, 60);
      tft.print("ASSOCIATE PROFESSOR");
      break;
    case 12: //Mr. Lawrence
      tft.drawBitmap(60,0,jackson_lawrence,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Jackson Lawrence");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("LECTURER");
      break;
    case 13: //Dr. Lindquist
      tft.drawBitmap(60,0,lindquist,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Lindquist");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("VICE PRESIDENT FOR RESEARCH");
      tft.setCursor(10, 60);
      tft.print("AND ECONOMIC DEVELOPMENT,");
      tft.setCursor(10, 70);
      tft.print("PROFESSOR");
      break;
    case 14: //Dr. Milenkovic
      tft.drawBitmap(60,0,milenkovic,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Milenkovic");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("DEPARTMENT CHAIR,");
      tft.setCursor(10, 60);
      tft.print("PROFESSOR");
      break;
    case 15: //Dr. Morris
      tft.drawBitmap(60,0,morris,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Morris");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("DIRECTOR OF CCRE");
      tft.setCursor(10, 70);
      tft.print("EMINENT SCHOLAR OF");
      tft.setCursor(10, 80);
      tft.print("COMPUTER ENGINEERING");
      break;
    case 16: //Dr. Nguyen
      tft.drawBitmap(60,0,nguyen,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Nguyen");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("ASSISTANT PROFESSOR");
      break;
    case 17: //Dr. Pan
      tft.drawBitmap(60,0,pan,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Pan");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("ASSOCIATE PROFESSOR");
      break;
    case 18: //Dr. Pour
      tft.drawBitmap(60,0,pour,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Pour");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("ASSOCIATE PROFESSOR");
      break;
    case 19: //Dr. Sahoo
      tft.drawBitmap(60,0,sahoo,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Sahoo");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("ASSISTANT PROFESSOR");
      break;
    case 20: //Dr. Wells
      tft.drawBitmap(60,0,wells,320,170,faceColor);
      tft.fillRect(0,0,150,170,ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 20);
      tft.print("Dr. Wells");
      tft.setTextSize(1);
      tft.setCursor(10, 50);
      tft.print("PROFESSOR");
      break;
    default:
      break;
  }
}

void drawMenuSelect() {
  tft.fillRect(0,0,150,170,ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);

  tft.drawRect(0,10,130,25,ST77XX_WHITE); //How To
  tft.setCursor(5,20);
  tft.print("How To");

  tft.drawRect(0,35,130,25,ST77XX_WHITE); //Cyber Challenge
  tft.setCursor(5,45);
  tft.print("Cyber Challenge");

  tft.drawRect(0,60,130,25,ST77XX_WHITE); //EE Circuit Maker
  tft.setCursor(5,70);
  tft.print("EE Circuit Connect");

  tft.drawRect(0,85,130,25,ST77XX_WHITE); //CPE Logic Puzzle
  tft.setCursor(5,95);
  tft.print("CPE Logic Puzzle");

  tft.drawRect(0,110,130,25,ST77XX_WHITE); //Bonus
  tft.setCursor(5,120);
  tft.print("Bonus");

  tft.drawRect(0,135,130,25,ST77XX_WHITE); //Touch Test
  tft.setCursor(5,145);
  tft.print("About");
  
  tft.setTextColor(ST77XX_BLACK);

  switch (main_selection) {
    case 0:
      tft.fillRect(0,10,150,25,ST77XX_WHITE); //How To select
      tft.setCursor(10,20);
      tft.print("How To");
      break;
    case 1:
      tft.fillRect(0,35,150,25,ST77XX_WHITE); //Cyber Challenge select
      tft.setCursor(10,45);
      tft.print("Cyber Challenge");
      break;
    case 2:
      tft.fillRect(0,60,150,25,ST77XX_WHITE); //EE Circuit Maker select
      tft.setCursor(10,70);
      tft.print("EE Circuit Connect");
      break;
    case 3:
      tft.fillRect(0,85,150,25,ST77XX_WHITE); //CPE Logic Puzzle select
      tft.setCursor(10,95);
      tft.print("CPE Logic Puzzle");
      break;
    case 4:
      tft.fillRect(0,110,150,25,ST77XX_WHITE); //Bonus
      tft.setCursor(10,120);
      tft.print("Bonus");
      break;
    case 5:
      tft.fillRect(0,135,150,25,ST77XX_WHITE); //About
      tft.setCursor(10,145);
      tft.print("About");
      break;
    default:
      break;
  }

}

void drawBonusMenuSelect() {
  tft.fillRect(0,0,150,170,ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);

  tft.drawRect(0,10,130,25,ST77XX_WHITE); //ECE Faculty
  tft.setCursor(5,20);
  tft.print("ECE Faculty");

  tft.drawRect(0,35,130,25,ST77XX_WHITE); //Github
  tft.setCursor(5,45);
  tft.print("Github");

  tft.drawRect(0,60,130,25,ST77XX_WHITE); //Return
  tft.setCursor(5,70);
  tft.print("Return");

  // bonus menu can be expanded further, just make sure that the bonus selection variable can go up to the correct amount
  // tft.drawRect(0,85,130,25,ST77XX_WHITE); //menu option 4
  // tft.setCursor(5,95);
  // tft.print("");

  // tft.drawRect(0,110,130,25,ST77XX_WHITE); //menu option 5
  // tft.setCursor(5,120);
  // tft.print("");

  // tft.drawRect(0,135,130,25,ST77XX_WHITE); //Tmenu option 6
  // tft.setCursor(5,145);
  // tft.print("");
  
  tft.setTextColor(ST77XX_BLACK);

  switch (bonus_selection) {
    case 0:
      tft.fillRect(0,10,150,25,ST77XX_WHITE); //Faculty select
      tft.setCursor(10,20);
      tft.print("ECE Faculty");
      break;
    case 1:
      tft.fillRect(0,35,150,25,ST77XX_WHITE); //Github select
      tft.setCursor(10,45);
      tft.print("Github");
      break;
    case 2:
      tft.fillRect(0,60,150,25,ST77XX_WHITE); //Return select
      tft.setCursor(10,70);
      tft.print("Return");
      break;
    // case 3:
    //   tft.fillRect(0,85,150,25,ST77XX_WHITE); //option 4
    //   tft.setCursor(10,95);
    //   tft.print("");
    //   break;
    // case 4:
    //   tft.fillRect(0,110,150,25,ST77XX_WHITE); //option 5
    //   tft.setCursor(10,120);
    //   tft.print("");
    //   break;
    // case 5:
    //   tft.fillRect(0,135,150,25,ST77XX_WHITE); //option 6
    //   tft.setCursor(10,145);
    //   tft.print("");
    //   break;
    default:
      break;
  }

}


void drawMainMenu() {
  tft.fillScreen(ST77XX_BLACK);
  tft.drawBitmap(180,20,uah_logo_small,100,32,ST77XX_BLUE);
  tft.setCursor(177,60);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Main Menu");

  tft.setCursor(160,140);
  tft.setTextSize(1);
  tft.print("Up/Down to navigate.");

  tft.setCursor(160,150);
  tft.print("A to select.");
  
  drawMenuSelect();
}

void drawBonusMenu() {
  tft.fillScreen(ST77XX_BLACK);
  tft.drawBitmap(180,20,uah_logo_small,100,32,ST77XX_BLUE);
  tft.setCursor(165,60);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Bonus Menu");

  tft.setCursor(160,140);
  tft.setTextSize(1);
  tft.print("Up/Down to navigate.");

  tft.setCursor(160,150);
  tft.print("A to select.");
  
  drawBonusMenuSelect();
}

int about_Y = 0; //global variable to track the Y position of the about page

void drawAboutPage() {
  tft.fillScreen(ST77XX_BLACK);
  tft.drawBitmap(110,about_Y + 20,uah_logo_small,100,32,ST77XX_BLUE);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);

  tft.setCursor(100,about_Y + 60);
  tft.print("This is CHARGER Card.");
  tft.setCursor(20,about_Y + 80);
  tft.print("A hands on experience to promote the");
  tft.setCursor(20,about_Y + 90);
  tft.print("Electrical and Computer Engineering department");
  tft.setCursor(20,about_Y + 100);
  tft.print("at the University of Alabama in Huntsville.");

  tft.setCursor(120,about_Y + 140);
  tft.print("Developed By");
  tft.setCursor(60,about_Y + 160);
  tft.print("Aadam Saafi");
  tft.setCursor(180,about_Y + 160);
  tft.print("Austin Nash");
  tft.setCursor(40,about_Y + 180);
  tft.print("John D'Alessandro");
  tft.setCursor(180,about_Y + 180);
  tft.print("Keelan Lynch");

  tft.setCursor(100,about_Y + 220);
  tft.print("With Assistance From");
  tft.setCursor(115,about_Y + 240);
  tft.print("Dr. Emil Jovanov");
  tft.setCursor(110,about_Y + 260);
  tft.print("Dr. Buren E. Wells");
  tft.setCursor(115,about_Y + 280);
  tft.print("Dr. Brian Smith");
  tft.setCursor(115,about_Y + 300);
  tft.print("Dr. Aubrey Beal");
  tft.setCursor(120,about_Y + 320);
  tft.print("William Lochte");

  tft.setCursor(40,about_Y + 360);
  tft.print("Requested on behalf of the ECE Department");
  tft.setCursor(120,about_Y + 370);
  tft.print("by Dr. David Coe");

  tft.setCursor(60,about_Y + 410);
  tft.print("Project Sponsored by Dr. David Coe");

  tft.setCursor(60,about_Y + 480);
  tft.print("Univeristy of Alabama in Huntsville");
  tft.setCursor(100,about_Y + 490);
  tft.print("Computer Engineering");
  tft.setCursor(110,about_Y + 500);
  tft.print("Senior Design 2026");
}

void returnMainMenu() {
  scene = 0;
  drawMainMenu();
}

/*********************CYBER CHALLENGE*********************/
// =====================================================
// Game settings
// =====================================================
#define CODE_BITS      8
#define MAX_GUESSES    5

// =====================================================
// Custom colors
// =====================================================
#define COLOR_CORRECT    0x07E0
#define COLOR_WRONG      0x7800
#define COLOR_BG         0x0841
#define COLOR_BUTTON     0x4208
#define COLOR_BUTTON_ON  0x07FF
#define COLOR_PANEL      0x0000
#define COLOR_TEXT       0xFFFF
#define COLOR_INFO       0xFFE0
#define COLOR_ACCENT     0x07FF
#define COLOR_CURSOR     0xF81F

// =====================================================
// Game state
// =====================================================
uint8_t targetNumber = 0;                   // 0-255
uint8_t currentGuess = 0;
uint8_t guesses[MAX_GUESSES];
uint8_t guessScores[MAX_GUESSES];           // number of correct bits per guess
uint8_t guessCount = 0;
uint8_t selectedBit = 7;                    // start on MSB

bool gameWon = false;
bool gameLost = false;

// =====================================================
// Statistics
// =====================================================
uint16_t gamesPlayed = 0;
uint16_t gamesWon = 0;
uint16_t currentStreak = 0;
uint16_t bestStreak = 0;

// =====================================================
// Feedback
// =====================================================
String lastHint = "";

void moveSelectionLeft() {
  if (selectedBit < 7) {
    selectedBit++;
    drawCurrentGuess();
  }
}

void moveSelectionRight() {
  if (selectedBit > 0) {
    selectedBit--;
    drawCurrentGuess();
  }
}

void toggleSelectedBit() {
  currentGuess ^= (1 << selectedBit);
  drawCurrentGuess();
}

void submitGuess() {
  if (gameWon || gameLost) return;
  if (guessCount >= MAX_GUESSES) return;

  guesses[guessCount] = currentGuess;
  guessScores[guessCount] = countCorrectBits(currentGuess, targetNumber);
  guessCount++;

  lastHint = String(guessScores[guessCount - 1]) + " bits correct";

  Serial.print("Guess #");
  Serial.print(guessCount);
  Serial.print(": ");
  Serial.print(getBinaryString(currentGuess));
  Serial.print(" | Correct bits: ");
  Serial.println(guessScores[guessCount - 1]);

  if (currentGuess == targetNumber) {
    gameWon = true;
    gamesPlayed++;
    gamesWon++;
    currentStreak++;
    if (currentStreak > bestStreak) bestStreak = currentStreak;

    drawPreviousGuesses();
    drawHintBar();
    drawStats();
    showVictory();
    return;
  }

  if (guessCount >= MAX_GUESSES) {
    gameLost = true;
    gamesPlayed++;
    currentStreak = 0;

    drawPreviousGuesses();
    drawHintBar();
    drawStats();
    showDefeat();
    return;
  }

  drawPreviousGuesses();
  drawHintBar();
  drawStats();
}

// =====================================================
// Drawing
// =====================================================
void drawUI() {
  tft.fillScreen(COLOR_BG);
  drawTopBar();
  drawCurrentGuess();
  drawPreviousGuesses();
  drawHintBar();
  drawStats();
  drawKeyboard();
}

void drawTopBar() {
  tft.fillRect(0, 0, 320, 24, COLOR_PANEL);

  tft.setTextSize(2);
  tft.setTextColor(COLOR_ACCENT);
  tft.setCursor(6, 5);
  tft.print("CYBER WORDLE");

  tft.setTextSize(1);
  tft.setTextColor(COLOR_INFO);
  tft.setCursor(232, 5);
  tft.print("8-BIT");
}

void drawCurrentGuess() {
  const int startX = 6;
  const int startY = 30;
  const int boxSize = 18;
  const int spacing = 4;

  tft.fillRect(0, startY, 320, 36, COLOR_BG);

  tft.setTextSize(1);
  tft.setTextColor(COLOR_INFO);
  tft.setCursor(startX, startY + 2);
  tft.print("Current:");

  int bitX = startX + 52;

  for (int i = 7; i >= 0; i--) {
    bool bitSet = (currentGuess >> i) & 0x01;
    uint16_t boxColor = bitSet ? COLOR_BUTTON_ON : COLOR_BUTTON;

    tft.fillRect(bitX, startY, boxSize, boxSize, boxColor);

    if (i == selectedBit) {
      tft.drawRect(bitX - 2, startY - 2, boxSize + 4, boxSize + 4, COLOR_CURSOR);
      tft.drawRect(bitX - 1, startY - 1, boxSize + 2, boxSize + 2, COLOR_CURSOR);
    } else {
      tft.drawRect(bitX, startY, boxSize, boxSize, COLOR_TEXT);
    }

    tft.setCursor(bitX + 5, startY + 5);
    tft.setTextColor(ST77XX_BLACK);
    tft.print(bitSet ? "1" : "0");

    bitX += boxSize + spacing;
  }
}

void drawPreviousGuesses() {
  int startX = 8;
  int startY = 70;
  int rowHeight = 12;

  tft.fillRect(0, startY, 320, 56, COLOR_BG);

  tft.setTextSize(1);
  tft.setTextColor(COLOR_ACCENT);
  tft.setCursor(startX, startY);
  tft.print("Guesses:");

  for (int i = 0; i < MAX_GUESSES; i++) {
    int y = startY + 10 + (i * rowHeight);
    tft.setCursor(startX, y);

    if (guesses[i] != 0xFF) {
      tft.setTextColor(COLOR_TEXT);
      tft.print(i + 1);
      tft.print(": ");
      tft.print(getBinaryString(guesses[i]));
      tft.print("  ");
      tft.setTextColor(COLOR_INFO);
      tft.print(guessScores[i]);
      tft.print("/");
      tft.print(CODE_BITS);
    } else {
      tft.setTextColor(COLOR_BUTTON_ON);
      tft.print(i + 1);
      tft.print(": --------");
    }
  }
}

void drawHintBar() {
  int y = 128;
  tft.fillRect(0, y, 320, 18, COLOR_PANEL);

  tft.setTextSize(1);
  tft.setTextColor(COLOR_INFO);
  tft.setCursor(5, y + 5);

  if (lastHint.length() == 0) {
    tft.print("Hint: Awaiting submission...");
  } else {
    tft.print("Hint: ");
    tft.print(lastHint);
  }
}

void drawStats() {
  tft.fillRect(0, 146, 320, 12, COLOR_BG);

  tft.setTextSize(1);
  tft.setTextColor(COLOR_TEXT);
  tft.setCursor(5, 147);
  tft.print("W:");
  tft.print(gamesWon);
  tft.print("/");
  tft.print(gamesPlayed);

  tft.setCursor(60, 147);
  tft.print("Streak:");
  tft.print(currentStreak);

  tft.setCursor(140, 147);
  tft.print("Best:");
  tft.print(bestStreak);
}

void drawKeyboard() {
  int startY = 158;

  tft.fillRect(0, startY, 320, 12, COLOR_PANEL);

  tft.setTextSize(1);

  tft.setCursor(5, startY + 2);
  tft.setTextColor(COLOR_INFO);
  tft.print("L/R Move  U/D Toggle");

  tft.setCursor(195, startY + 2);
  tft.setTextColor(COLOR_CORRECT);
  tft.print("A=OK");

  tft.setCursor(255, startY + 2);
  tft.setTextColor(COLOR_ACCENT);
  tft.print("B=Menu");
}

// =====================================================
// Bit comparison logic
// =====================================================
uint8_t countCorrectBits(uint8_t guess, uint8_t target) {
  uint8_t correct = 0;

  for (int i = 0; i < CODE_BITS; i++) {
    bool guessBit = (guess >> i) & 0x01;
    bool targetBit = (target >> i) & 0x01;
    if (guessBit == targetBit) {
      correct++;
    }
  }

  return correct;
}

// =====================================================
// Result screens
// =====================================================
void showVictory() {
  tft.fillRect(38, 42, 244, 72, COLOR_PANEL);
  tft.drawRect(38, 42, 244, 72, COLOR_CORRECT);
  tft.drawRect(39, 43, 242, 70, COLOR_CORRECT);

  tft.setTextSize(2);
  tft.setTextColor(COLOR_CORRECT);
  tft.setCursor(48, 54);
  tft.print("ACCESS GRANTED");

  tft.setTextSize(1);
  tft.setTextColor(COLOR_TEXT);
  tft.setCursor(60, 82);
  tft.print("Code: ");
  tft.print(getBinaryString(targetNumber));

  tft.setCursor(60, 96);
  tft.print("Press A to play again");
}

void showDefeat() {
  tft.fillRect(34, 42, 252, 76, COLOR_PANEL);
  tft.drawRect(34, 42, 252, 76, COLOR_WRONG);
  tft.drawRect(35, 43, 250, 74, COLOR_WRONG);

  tft.setTextSize(2);
  tft.setTextColor(COLOR_WRONG);
  tft.setCursor(68, 54);
  tft.print("ACCESS DENIED");

  tft.setTextSize(1);
  tft.setTextColor(COLOR_TEXT);
  tft.setCursor(46, 82);
  tft.print("Target: ");
  tft.print(getBinaryString(targetNumber));

  tft.setCursor(46, 96);
  tft.print("Press A to retry");
}

// =====================================================
// Formatting helpers
// =====================================================
String getBinaryString(uint8_t num) {
  String s = "";
  for (int i = 7; i >= 0; i--) {
    s += ((num >> i) & 0x01) ? "1" : "0";
  }
  return s;
}

String getHexString(uint8_t num) {
  const char hexChars[] = "0123456789ABCDEF";
  String s = "";
  s += hexChars[(num >> 4) & 0x0F];
  s += hexChars[num & 0x0F];
  return s;
}

// =====================================================
// Core game flow
// =====================================================
void newCyberGame() {
  targetNumber = random(0, 256);   // 8-bit target
  currentGuess = 0;
  guessCount = 0;
  selectedBit = 7;
  gameWon = false;
  gameLost = false;
  lastHint = "";

  for (int i = 0; i < MAX_GUESSES; i++) {
    guesses[i] = 0xFF;
    guessScores[i] = 0;
  }

  Serial.print("New game. Target: ");
  Serial.print(getBinaryString(targetNumber));
  Serial.print(" (0x");
  Serial.print(getHexString(targetNumber));
  Serial.println(")");

  drawUI();
}

/*************END OF CYBER CHALLENGE CODE*************/

/**********************START EE Activity**********************/

// =====================================================
// Game settings
// =====================================================
#define MAX_NODES   6
#define NODE_RADIUS 6

// =====================================================
// Custom colors
// =====================================================
#define COLOR_DARKGREY 0x4208

// =====================================================
// Component types
// =====================================================
enum ComponentType {
  NONE,
  RESISTOR,
  CAPACITOR,
  INDUCTOR,
  LED,
  BATTERY
};

// =====================================================
// Node structure
// =====================================================
struct Node {
  int x, y;
  int id;
  bool isStart;
  bool isEnd;
  bool isComponent;
  ComponentType compType;
  bool visited;
  int connectedTo[3];
  int connectionCount;
};

Node nodes[MAX_NODES];
int currentNode = -1;
int pathSequence[MAX_NODES];
int pathLength = 0;
bool gameComplete = false;
int currentLevel = 0;
const int totalLevels = 5;

int selectedNode = 0;
bool showHintOverlay = false;
unsigned long hintUntil = 0;

// =====================================================
// Level configurations
// =====================================================
struct Level {
  const char* title;
  const char* objective;
  ComponentType component;
  int nodeCount;
};

Level levels[5] = {
  {"Level 1: Resistor",        "Complete circuit through resistor", RESISTOR, 3},
  {"Level 2: Capacitor",       "Add capacitor to circuit",          CAPACITOR, 4},
  {"Level 3: LED",             "Light up the LED",                  LED,       4},
  {"Level 4: Series Circuit",  "Create series connection",          RESISTOR,  5},
  {"Level 5: Complex Circuit", "Multi-component circuit",           INDUCTOR,  6}
};

// =====================================================
// Forward declarations
// =====================================================
void drawConnection(int node1, int node2, uint16_t color, int offset = 0);
void drawComponentSymbol(int x, int y, ComponentType type, bool active);
void drawCircuit();
void drawControls();
void drawNodeLabel(int nodeIndex, uint16_t textColor);

void showError(const char* message);
void showSuccess();
void showGameComplete();
void resetLevel();

void initLevel(int level);
void setupLevel1();
void setupLevel2();
void setupLevel3();
void setupLevel4();
void setupLevel5();

void handleNodeTouch(int nodeId);
bool checkCircuitComplete();

void eeMoveSelectionLeft();
void eeMveSelectionRight();
void selectCurrentNode();
void undoMove();

void eeMoveSelectionLeft() {
  int activeNodes = levels[currentLevel].nodeCount;
  selectedNode--;
  if (selectedNode < 0) {
    selectedNode = activeNodes - 1;
  }
  drawCircuit();
}

void eeMoveSelectionRight() {
  int activeNodes = levels[currentLevel].nodeCount;
  selectedNode++;
  if (selectedNode >= activeNodes) {
    selectedNode = 0;
  }
  drawCircuit();
}

void selectCurrentNode() {
  handleNodeTouch(selectedNode);
}

void undoMove() {
  if (pathLength <= 0) {
    return;
  }

  int nodeToClear = pathSequence[pathLength - 1];
  nodes[nodeToClear].visited = false;
  pathSequence[pathLength - 1] = -1;
  pathLength--;

  if (pathLength == 0) {
    currentNode = -1;
  } else {
    currentNode = pathSequence[pathLength - 1];
  }

  gameComplete = false;
  drawCircuit();
}

// =====================================================
// Level init
// =====================================================
void initLevel(int level) {
  gameComplete = false;
  currentNode = -1;
  pathLength = 0;

  for (int i = 0; i < MAX_NODES; i++) {
    nodes[i].visited = false;
    nodes[i].connectionCount = 0;
    nodes[i].isStart = false;
    nodes[i].isEnd = false;
    nodes[i].isComponent = false;
    nodes[i].compType = NONE;
    pathSequence[i] = -1;
  }

  switch (level) {
    case 0: setupLevel1(); break;
    case 1: setupLevel2(); break;
    case 2: setupLevel3(); break;
    case 3: setupLevel4(); break;
    case 4: setupLevel5(); break;
  }

  selectedNode = 0;
  showHintOverlay = false;
  hintUntil = 0;
}

void setupLevel1() {
  nodes[0] = {50, 85, 0, true,  false, false, NONE,     false, {1},    1};
  nodes[1] = {160,85, 1, false, false, true,  RESISTOR, false, {0, 2}, 2};
  nodes[2] = {270,85, 2, false, true,  false, NONE,     false, {1},    1};
}

void setupLevel2() {
  nodes[0] = {50, 85, 0, true,  false, false, NONE,      false, {1, 2}, 2};
  nodes[1] = {160,50, 1, false, false, true,  CAPACITOR, false, {0, 3}, 2};
  nodes[2] = {160,120,2, false, false, false, NONE,      false, {0, 3}, 2};
  nodes[3] = {270,85, 3, false, true,  false, NONE,      false, {1, 2}, 2};
}

void setupLevel3() {
  nodes[0] = {50, 85, 0, true,  false, false, NONE, false, {1},    1};
  nodes[1] = {130,85, 1, false, false, false, NONE, false, {0, 2}, 2};
  nodes[2] = {200,85, 2, false, false, true,  LED,  false, {1, 3}, 2};
  nodes[3] = {270,85, 3, false, true,  false, NONE, false, {2},    1};
}

void setupLevel4() {
  nodes[0] = {40, 85, 0, true,  false, false, NONE,      false, {1},    1};
  nodes[1] = {110,85, 1, false, false, true,  RESISTOR,  false, {0, 2}, 2};
  nodes[2] = {180,85, 2, false, false, true,  CAPACITOR, false, {1, 3}, 2};
  nodes[3] = {250,85, 3, false, false, false, NONE,      false, {2, 4}, 2};
  nodes[4] = {290,85, 4, false, true,  false, NONE,      false, {3},    1};
}

void setupLevel5() {
  nodes[0] = {50, 85, 0, true,  false, false, NONE,     false, {1, 2}, 2};
  nodes[1] = {140,50, 1, false, false, true,  RESISTOR, false, {0, 3}, 2};
  nodes[2] = {140,120,2, false, false, true,  INDUCTOR, false, {0, 4}, 2};
  nodes[3] = {230,50, 3, false, false, false, NONE,     false, {1, 5}, 2};
  nodes[4] = {230,120,4, false, false, false, NONE,     false, {2, 5}, 2};
  nodes[5] = {280,85, 5, false, true,  false, NONE,     false, {3, 4}, 2};
}

// =====================================================
// Game logic
// =====================================================
void handleNodeTouch(int nodeId) {
  if (currentNode == -1) {
    if (nodes[nodeId].isStart) {
      currentNode = nodeId;
      nodes[nodeId].visited = true;
      pathSequence[pathLength++] = nodeId;
      drawCircuit();
      Serial.print("Started at node ");
      Serial.println(nodeId);
    } else {
      showError("Must start at START node!");
    }
    return;
  }

  bool isConnected = false;
  for (int i = 0; i < nodes[currentNode].connectionCount; i++) {
    if (nodes[currentNode].connectedTo[i] == nodeId) {
      isConnected = true;
      break;
    }
  }

  if (!isConnected) {
    showError("Not connected!");
    return;
  }

  if (nodes[nodeId].visited) {
    showError("Already visited!");
    return;
  }

  nodes[nodeId].visited = true;
  pathSequence[pathLength++] = nodeId;

  currentNode = nodeId;
  drawCircuit();

  if (nodes[nodeId].isEnd) {
    if (checkCircuitComplete()) {
      gameComplete = true;
      showSuccess();
    } else {
      showError("Circuit incomplete!");
      delay(700);
      resetLevel();
    }
  }

  Serial.print("Moved to node ");
  Serial.println(nodeId);
}

bool checkCircuitComplete() {
  bool hasComponent = false;
  for (int i = 0; i < pathLength; i++) {
    if (nodes[pathSequence[i]].isComponent) {
      hasComponent = true;
      break;
    }
  }
  return hasComponent && nodes[currentNode].isEnd;
}

// =====================================================
// Drawing
// =====================================================
void drawConnection(int node1, int node2, uint16_t color, int offset) {
  int x1 = nodes[node1].x;
  int y1 = nodes[node1].y + offset;
  int x2 = nodes[node2].x;
  int y2 = nodes[node2].y + offset;
  tft.drawLine(x1, y1, x2, y2, color);
}

void drawComponentSymbol(int x, int y, ComponentType type, bool active) {
  uint16_t symbolColor = active ? ST77XX_GREEN : ST77XX_WHITE;
  uint16_t bgColor = ST77XX_BLACK;
  int w = 30;
  int h = 20;

  tft.fillRect(x - w / 2 - 2, y - h / 2 - 2, w + 4, h + 4, bgColor);

  switch (type) {
    case RESISTOR: {
      tft.drawRect(x - 15, y - 6, 30, 12, symbolColor);
      tft.drawRect(x - 14, y - 5, 28, 10, symbolColor);
      tft.drawLine(x - 20, y, x - 15, y, symbolColor);
      tft.drawLine(x + 15, y, x + 20, y, symbolColor);
      tft.setCursor(x - 3, y - 18);
      tft.setTextColor(ST77XX_CYAN);
      tft.setTextSize(1);
      tft.print("R");
      tft.fillCircle(x - 20, y, 3, symbolColor);
      tft.fillCircle(x + 20, y, 3, symbolColor);
      break;
    }

    case CAPACITOR: {
      tft.drawLine(x - 3, y - 12, x - 3, y + 12, symbolColor);
      tft.drawLine(x - 2, y - 12, x - 2, y + 12, symbolColor);
      tft.drawLine(x + 2, y - 12, x + 2, y + 12, symbolColor);
      tft.drawLine(x + 3, y - 12, x + 3, y + 12, symbolColor);
      tft.drawLine(x - 20, y, x - 3, y, symbolColor);
      tft.drawLine(x + 3, y, x + 20, y, symbolColor);
      tft.setCursor(x - 3, y - 20);
      tft.setTextColor(ST77XX_CYAN);
      tft.setTextSize(1);
      tft.print("C");
      tft.fillCircle(x - 20, y, 3, symbolColor);
      tft.fillCircle(x + 20, y, 3, symbolColor);
      break;
    }

    case INDUCTOR: {
      int coilRadius = 4;
      for (int i = -12; i <= 8; i += 8) {
        for (int angle = 0; angle <= 180; angle += 15) {
          float rad1 = angle * 3.14159f / 180.0f;
          float rad2 = (angle + 15) * 3.14159f / 180.0f;
          int x1 = x + i + (int)(coilRadius * cos(rad1));
          int y1 = y - (int)(coilRadius * sin(rad1));
          int x2 = x + i + (int)(coilRadius * cos(rad2));
          int y2 = y - (int)(coilRadius * sin(rad2));
          tft.drawLine(x1, y1, x2, y2, symbolColor);
        }
      }
      tft.drawLine(x - 20, y, x - 16, y, symbolColor);
      tft.drawLine(x + 16, y, x + 20, y, symbolColor);
      tft.setCursor(x - 3, y - 18);
      tft.setTextColor(ST77XX_CYAN);
      tft.setTextSize(1);
      tft.print("L");
      tft.fillCircle(x - 20, y, 3, symbolColor);
      tft.fillCircle(x + 20, y, 3, symbolColor);
      break;
    }

    case LED: {
      uint16_t arrowColor = active ? ST77XX_YELLOW : ST77XX_ORANGE;
      tft.drawLine(x - 8, y - 10, x - 8, y + 10, symbolColor);
      tft.drawLine(x - 8, y - 10, x + 7, y, symbolColor);
      tft.drawLine(x - 8, y + 10, x + 7, y, symbolColor);
      tft.drawLine(x + 7, y - 10, x + 7, y + 10, symbolColor);

      tft.drawLine(x + 5, y - 8, x + 10, y - 12, arrowColor);
      tft.drawLine(x + 10, y - 12, x + 9, y - 10, arrowColor);
      tft.drawLine(x + 10, y - 12, x + 8, y - 11, arrowColor);

      tft.drawLine(x + 8, y - 4, x + 13, y - 8, arrowColor);
      tft.drawLine(x + 13, y - 8, x + 12, y - 6, arrowColor);
      tft.drawLine(x + 13, y - 8, x + 11, y - 7, arrowColor);

      tft.drawLine(x - 20, y, x - 8, y, symbolColor);
      tft.drawLine(x + 7, y, x + 20, y, symbolColor);
      tft.setCursor(x - 8, y + 14);
      tft.setTextColor(arrowColor);
      tft.setTextSize(1);
      tft.print("LED");
      tft.fillCircle(x - 20, y, 3, symbolColor);
      tft.fillCircle(x + 20, y, 3, symbolColor);
      break;
    }

    case BATTERY: {
      tft.drawLine(x - 5, y - 10, x - 5, y + 10, symbolColor);
      tft.drawLine(x - 4, y - 10, x - 4, y + 10, symbolColor);
      tft.drawLine(x + 4, y - 6, x + 4, y + 6, symbolColor);
      tft.drawLine(x + 5, y - 6, x + 5, y + 6, symbolColor);
      tft.drawLine(x - 20, y, x - 5, y, symbolColor);
      tft.drawLine(x + 5, y, x + 20, y, symbolColor);
      tft.setCursor(x + 8, y - 4);
      tft.setTextColor(ST77XX_RED);
      tft.setTextSize(1);
      tft.print("+");
      tft.setCursor(x - 12, y - 4);
      tft.setTextColor(ST77XX_BLUE);
      tft.print("-");
      tft.fillCircle(x - 20, y, 3, symbolColor);
      tft.fillCircle(x + 20, y, 3, symbolColor);
      break;
    }

    case NONE:
      break;
  }
}

void drawCircuit() {
  tft.fillScreen(ST77XX_BLACK);

  tft.setCursor(5, 5);
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(1);
  tft.print(levels[currentLevel].title);

  tft.setCursor(5, 18);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.print(levels[currentLevel].objective);

  int activeNodes = levels[currentLevel].nodeCount;

  for (int i = 0; i < activeNodes; i++) {
    for (int j = 0; j < nodes[i].connectionCount; j++) {
      int targetId = nodes[i].connectedTo[j];
      if (targetId > i) {
        drawConnection(i, targetId, COLOR_DARKGREY);
      }
    }
  }

  for (int i = 1; i < pathLength; i++) {
    drawConnection(pathSequence[i - 1], pathSequence[i], ST77XX_GREEN);
    drawConnection(pathSequence[i - 1], pathSequence[i], ST77XX_GREEN, 1);
  }

  for (int i = 0; i < activeNodes; i++) {
    bool isSelected = (i == selectedNode);
    bool isSuggested = false;

    if (showHintOverlay && currentNode >= 0) {
      for (int j = 0; j < nodes[currentNode].connectionCount; j++) {
        if (nodes[currentNode].connectedTo[j] == i && !nodes[i].visited) {
          isSuggested = true;
        }
      }
    }

    if (nodes[i].isComponent) {
      drawComponentSymbol(nodes[i].x, nodes[i].y, nodes[i].compType, nodes[i].visited);
    } else {
      uint16_t color;
      if (nodes[i].visited) {
        color = ST77XX_GREEN;
      } else if (nodes[i].isStart) {
        color = ST77XX_RED;
      } else if (nodes[i].isEnd) {
        color = ST77XX_BLUE;
      } else {
        color = ST77XX_YELLOW;
      }

      tft.fillCircle(nodes[i].x, nodes[i].y, NODE_RADIUS, color);
      tft.drawCircle(nodes[i].x, nodes[i].y, NODE_RADIUS + 1, ST77XX_WHITE);

      if (nodes[i].isStart) {
        tft.setCursor(nodes[i].x - 3, nodes[i].y + 12);
        tft.setTextColor(ST77XX_RED);
        tft.setTextSize(1); 
        tft.print("S");
      } else if (nodes[i].isEnd) {
        tft.setCursor(nodes[i].x - 3, nodes[i].y + 12);
        tft.setTextColor(ST77XX_BLUE);
        tft.setTextSize(1);
        tft.print("E");
      }
    }

    drawNodeLabel(i, ST77XX_WHITE);

    if (isSelected) {
      tft.drawCircle(nodes[i].x, nodes[i].y, NODE_RADIUS + 6, ST77XX_MAGENTA);
      tft.drawCircle(nodes[i].x, nodes[i].y, NODE_RADIUS + 7, ST77XX_MAGENTA);
    }

    if (isSuggested) {
      tft.drawCircle(nodes[i].x, nodes[i].y, NODE_RADIUS + 10, ST77XX_CYAN);
    }
  }

  drawControls();
}

void drawNodeLabel(int nodeIndex, uint16_t textColor) {
  tft.setTextColor(textColor);
  tft.setTextSize(1);
  tft.setCursor(nodes[nodeIndex].x - 3, nodes[nodeIndex].y - 20);
  tft.print(nodeIndex);
}

void drawControls() {
  tft.fillRect(0, 148, 320, 22, ST77XX_BLACK);
  tft.drawFastHLine(0, 148, 320, COLOR_DARKGREY);

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(4, 152);
  tft.print("L/R Move");

  tft.setCursor(72, 152);
  tft.print("A Select");

  tft.setCursor(138, 152);
  tft.print("Dn Undo");

  tft.setCursor(206, 152);
  tft.print("Up Hint");

  tft.setCursor(266, 152);
  tft.print("B Return");
}

// =====================================================
// UI messages
// =====================================================
void showError(const char* message) {
  tft.fillRect(60, 65, 200, 40, ST77XX_BLACK);
  tft.drawRect(58, 63, 204, 44, ST77XX_RED);
  tft.setCursor(80, 75);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.print(message);
  delay(1000);
  drawCircuit();
}

void showSuccess() {
  delay(300);
  tft.fillRect(50, 60, 220, 50, ST77XX_BLACK);
  tft.drawRect(48, 58, 224, 54, ST77XX_GREEN);
  tft.drawRect(49, 59, 222, 52, ST77XX_GREEN);

  tft.setCursor(70, 70);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.print("COMPLETE!");

  tft.setCursor(60, 92);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  if (currentLevel < totalLevels - 1) {
    tft.print("Touch A to continue");
  } else {
    tft.print("Touch A to restart");
  }
}

void showGameComplete() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(50, 60);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.print("ALL LEVELS");
  tft.setCursor(70, 85);
  tft.print("COMPLETE!");
  tft.setCursor(80, 115);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Great job!");
}

void resetLevel() {
  initLevel(currentLevel);
  drawCircuit();
}

/***********************END EE Activity***********************/

/*********************Start CPE Activity*********************/
//globals
struct LogicComponent {
  int type;         //"AND","OR","NAND","WIRE",etc.
  int in1;          // id of component of the first input
  int in2;          // id of component of the second input
  int val;          // value
  int x;
  int y;
};

struct CpeCursor {
  int x;
  int y;
};

CpeCursor cursor;

int componentCount;
int currentCpeLevel = 0;
bool won;
LogicComponent componentArray[128];

int checkVal(int i) {
  switch(componentArray[i].type) {
    case 0:
      return 0;
      break;
    case 1:
      return 1;
      break;
    case 2:
      componentArray[i].val = checkVal(componentArray[i].in1) & checkVal(componentArray[i].in2);
      break;
    case 3:
      componentArray[i].val = checkVal(componentArray[i].in1) | checkVal(componentArray[i].in2);
      break;
    case 4:
      componentArray[i].val = !checkVal(componentArray[i].in1);
      break;
    case 8:
      if(componentArray[i].in2){componentArray[i].val = checkVal(componentArray[i].in1);}
      break;
    case 9:
      componentArray[i].val = checkVal(componentArray[i].in1);
      break;
  }
  return componentArray[i].val;
}

void drawCursor() {
  tft.drawRect(cursor.x,cursor.y,20,20,ST77XX_WHITE);
}

void drawWire(int x0, int y0, int i) {
  int x1 = componentArray[i].x;
  int y1 = componentArray[i].y;
  __uint16_t color = ST77XX_WHITE;
  if(componentArray[i].val){
    color = ST77XX_GREEN;
  }
  tft.drawLine(x0, y0, x0 - 20, y0, color);
  tft.drawLine(x0 - 20, y0, x0 - 20, y1+10, color);
  tft.drawLine(x0 - 20, y1+10, x1+20, y1+10, color);
}

void drawLogic(int i) {
  int pos_x = componentArray[i].x + 7;
  int pos_y = componentArray[i].y + 7;
  tft.setCursor(pos_x, pos_y);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  switch (componentArray[i].val) {
    case 0:
      tft.print("0");
      break;
    case 1:
      tft.print("1");
      break;
    default:
      break;
  }
}

void drawNot(int i) {
  int x = componentArray[i].x;
  int y = componentArray[i].y;
  tft.drawTriangle(x,y,x+10,y+10,x,y+20,ST77XX_WHITE);
  tft.drawCircle(x + 15,y + 10,3,ST77XX_WHITE);
  drawWire(x,y+10,componentArray[i].in1);

  tft.setCursor(x,y-10);
  tft.print("NOT");
}

void drawAnd(int i) {
  int x = componentArray[i].x;
  int y = componentArray[i].y;
  tft.drawCircle(x+10,y+10,10,ST77XX_WHITE);
  tft.fillRect(x-10,y,20,20,ST77XX_BLACK);
  tft.drawLine(x,y,x,y+20,ST77XX_WHITE);
  tft.drawLine(x,y,x+10,y,ST77XX_WHITE);
  tft.drawLine(x,y+20,x+10,y+20,ST77XX_WHITE);

  drawWire(x,y+7,componentArray[i].in1);
  drawWire(x,y+13,componentArray[i].in2);

  tft.setCursor(x,y-10);
  tft.print("AND");
}

void drawOr(int i) {
  int x = componentArray[i].x;
  int y = componentArray[i].y;
  tft.drawCircle(x+10,y+10,10,ST77XX_WHITE);
  tft.fillRect(x-10,y,20,20,ST77XX_BLACK);
  tft.drawCircle(x-10,y+10,15,ST77XX_WHITE);
  tft.fillRect(x-60,y-20,60,80,ST77XX_BLACK);
  tft.drawLine(x,y,x+10,y,ST77XX_WHITE);
  tft.drawLine(x,y+20,x+10,y+20,ST77XX_WHITE);

  drawWire(x,y+7,componentArray[i].in1);
  drawWire(x,y+13,componentArray[i].in2);

  tft.setCursor(x,y-10);
  tft.print("OR");
}

void drawSwitch(int i) {
  int x = componentArray[i].x;
  int y = componentArray[i].y;
  __uint16_t color = ST77XX_WHITE;
  if(componentArray[i].val){
    color = ST77XX_GREEN;
  }
  tft.drawCircle(x,y + 10,2,color);
  tft.drawCircle(x + 20,y + 10,2,color);
  if(componentArray[i].in2){
    tft.drawLine(x,y+10,x+20,y+10,color);
  }
  else{
    tft.drawLine(x,y+10,x+15,y,color);
  }

  drawWire(x,y+10,componentArray[i].in1);
}

void drawLED(int i) {
  int pos_x = componentArray[i].x + 10;
  int pos_y = componentArray[i].y + 10;
  if(componentArray[i].val){tft.fillCircle(pos_x,pos_y,10,ST77XX_GREEN);}
  else{tft.drawCircle(pos_x,pos_y,10,ST77XX_WHITE);}
  
  drawWire(pos_x-10,pos_y,componentArray[i].in1);

  tft.setCursor(componentArray[i].x,componentArray[i].y-10);
  tft.print("LED");
}

void drawComponent(int i) {
  switch (componentArray[i].type) {
    case 0: //Logical 0
      drawLogic(i);
      break;
    case 1: //Logical 1
      drawLogic(i);
      break;
    case 2: //AND
      drawAnd(i);
      break;
    case 3: //OR
      drawOr(i);
      break;
    case 4: //NOT
      drawNot(i);
      break;
    case 5:
      break;
    case 6: 
      break;
    case 7:
      break;
    case 8: //Switch
      drawSwitch(i);
      break;
    case 9: //LED Out
      drawLED(i);
      break;
    default:
      break;
  }
}

void drawWin() {
  delay(100);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_GREEN);
  tft.setCursor(80, 20);
  tft.print("Level Complete!");
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(80, 140);
  tft.print("Press A for next level");
}

void drawCPELevel() {
  for(int i = 0; i < componentCount; i++) {
    drawComponent(i);
    checkVal(i);
    if(componentArray[i].type==9 && componentArray[i].val==1){won=true;drawWin();}
  }
}

void componentInteract(int i) {
  switch(componentArray[i].type) {
    case 0: 
      componentArray[i].type = 1;
      componentArray[i].val = 1;
      updateCPE();
      break;
    case 1: 
      componentArray[i].type = 0;
      componentArray[i].val = 0;
      updateCPE();
      break;
    case 8:
      componentArray[i].in2 = !componentArray[i].in2;
      updateCPE();
      break;
    default:
      break;
  }
}

void startCPELevel(int l) {
  currentCpeLevel = l;
  if(currentCpeLevel > 7){currentCpeLevel = 0;}
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  switch (l) {
    case 0:
      won = false;
      componentCount = 2;
      tft.setCursor(10, 160);
      tft.print("CPE Activity | Level 0 - Simple connection");
      cursor.x = 20; cursor.y = 20;
      componentArray[0] = {0,-1,-1,0,60,80};  //input 0
      componentArray[1] = {9,0,-1,0,220,80};  //LED
      drawCPELevel();
      drawCursor();
      break;
    case 1:
      won = false;
      componentCount = 3;
      tft.setCursor(10, 160);
      tft.print("CPE Activity | Level 1 - NOT Gate");
      cursor.x = 20; cursor.y = 20;
      componentArray[0] = {1,-1,-1,1,60,80};  //input 1
      componentArray[1] = {4,0,-1,0,140,80};  //NOT gate
      componentArray[2] = {9,1,-1,0,220,80};  //LED
      drawCPELevel();
      drawCursor();
      break;
    case 2:
      won = false;
      componentCount = 4;
      tft.setCursor(10, 160);
      tft.print("CPE Activity | Level 2 - AND Gate");
      cursor.x = 20; cursor.y = 20;
      componentArray[0] = {0,-1,-1,0,60,60}; //input 1
      componentArray[1] = {0,-1,-1,0,60,100}; //input 0
      componentArray[2] = {2,0,1,0,140,80}; //AND gate
      componentArray[3] = {9,2,-1,0,220,80}; //LED
      drawCPELevel();
      drawCursor();
      break;
    case 3:
      won = false;
      componentCount = 4;
      tft.setCursor(10, 160);
      tft.print("CPE Activity | Level 3 - OR Gate");
      cursor.x = 20; cursor.y = 20;
      componentArray[0] = {0,-1,-1,0,60,60}; //input 0
      componentArray[1] = {0,-1,-1,0,60,100}; //input 0
      componentArray[2] = {3,0,1,0,140,80};  //OR gate
      componentArray[3] = {9,2,-1,0,220,80}; //LED
      drawCPELevel();
      drawCursor();
      break;
    case 4:
      won = false;
      componentCount = 6;
      tft.setCursor(10, 160);
      tft.print("CPE Activity | Level 4");
      cursor.x = 20; cursor.y = 20;
      componentArray[0] = {0,-1,-1,0,20,60}; //input 0
      componentArray[1] = {0,-1,-1,0,20,80};//input 0
      componentArray[2] = {0,-1,-1,0,20,100};//input 0
      componentArray[3] = {3,0,1,0,100,60};  //OR gate
      componentArray[4] = {2,3,2,0,160,80};  //AND gate
      componentArray[5] = {9,4,-1,0,220,80}; //LED
      drawCPELevel();
      drawCursor();
      break;
    case 5:
      won = false;
      componentCount = 9;
      tft.setCursor(10, 160);
      tft.print("CPE Activity | Level 5");
      cursor.x = 20; cursor.y = 20;
      componentArray[0] = {0,-1,-1,0,20,40}; //input 0
      componentArray[1] = {0,-1,-1,0,20,60}; //input 0
      componentArray[2] = {0,-1,-1,0,20,80}; //input 0
      componentArray[3] = {0,-1,-1,0,20,100};//input 0
      componentArray[4] = {4,3,-1,0,60,100}; //NOT gate
      componentArray[5] = {2,0,1,0,100,60};  //AND gate
      componentArray[6] = {3,5,2,0,180,60};  //OR gate
      componentArray[7] = {2,6,4,0,220,100};  //AND gate
      componentArray[8] = {9,7,-1,0,280,80}; //LED
      drawCPELevel();
      drawCursor();
      break;
    case 6:
      won = false;
      componentCount = 3;
      tft.setCursor(10, 160);
      tft.print("CPE Activity | Level 6 - Switch");
      cursor.x = 20; cursor.y = 20;
      componentArray[0] = {1,-1,-1,1,60,80};  //input 1
      componentArray[1] = {8,0,0,0,140,80};  //Switch
      componentArray[2] = {9,1,-1,0,220,80};  //LED
      drawCPELevel();
      drawCursor();
      break;
    case 7:
      won = false;
      componentCount = 11;
      tft.setCursor(10, 160);
      tft.print("CPE Activity | Level 5");
      cursor.x = 20; cursor.y = 20;
      componentArray[0] = {0,-1,-1,0,20,40}; //input 0
      componentArray[1] = {0,-1,-1,0,20,60}; //input 0
      componentArray[2] = {0,-1,-1,0,20,80}; //input 0
      componentArray[3] = {0,-1,-1,0,20,100};//input 0
      componentArray[4] = {3,0,1,0,100,40};  //OR gate
      componentArray[5] = {4,3,-1,0,60,100}; //NOT gate
      componentArray[6] = {8,5,0,0,100,100};  //Switch
      componentArray[7] = {2,2,6,0,140,80};  //AND gate
      componentArray[8] = {2,4,7,0,180,60};  //AND gate
      componentArray[9] = {8,8,0,0,220,60};  //Switch
      componentArray[10] = {9,9,-1,0,280,80}; //LED
      drawCPELevel();
      drawCursor();
      break;
    default:
      break;
  }
}

void updateCPE() {
  tft.fillRect(0,0,320,160,ST77XX_BLACK);
  drawCPELevel();
  drawCursor();
}

void startCPE() {
  startCPELevel(currentCpeLevel);
}

void cpeInputHandling(int t){
  switch (t) {
    case 0: //A
      for(int i = 0; i < componentCount; i++) {
        if(cursor.x == componentArray[i].x && cursor.y == componentArray[i].y) {
          componentInteract(i);
        }
      }
      break;
    case 1: //B
      break;
    case 2: //Up
      if(cursor.y > 0){cursor.y = cursor.y - 20;}
      break;
    case 3: //Down
      if(cursor.y < 140){cursor.y = cursor.y + 20;}
      break;
    case 4: //Left
      if(cursor.x > 0){cursor.x = cursor.x - 20;}
      break;
    case 5: //Right
      if(cursor.x < 300){cursor.x = cursor.x + 20;}
      break;
    default:
      break;
  }
  updateCPE();
}

/**********************End CPE Activity**********************/

void enterScene(int s) {
  switch(s) {
    case 0:
      //load how to scene
      break;
    case 1:
      //load cyber challenge
      scene = 2;
      randomSeed(analogRead(0));
      gamesPlayed = 0;
      gamesWon = 0;
      currentStreak = 0;
      bestStreak = 0;
      for (int i = 0; i < MAX_GUESSES; i++) {
        guesses[i] = 0xFF;
        guessScores[i] = 0;
      }
      newCyberGame();
      break;
    case 2:
      //load ee activity
      scene = 3;
      Serial.println("Circuit Learning Game Started");
      initLevel(currentLevel);
      break;
    case 3:
      //load cpe activity
      scene = 4;
      startCPE();
      break;
    case 4:
      //load bonus menu
      scene = 5;
      tft.fillScreen(ST77XX_BLACK);
      drawBonusMenu();
      break;
    case 5:
      //load about page
      scene = 6;
      about_Y = 0;
      drawAboutPage();
      break;
    default:
      break;
  }
}

void enterBonusScene(int s) {
  switch(s) {
    case 0:
      //load faculty scene
      scene = 7;
      drawFaculty(faculty);
      break;
    case 1:
      //load github scene
      scene = 8;
      drawGithub();
      break;
    case 2:
      //return to main
      returnMainMenu();
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    default:
      break;
  }
}

void update_mm(int d) {
  switch(d) {
    case 0: //up
      if(main_selection>0){main_selection--;}
      else{main_selection = 5;}
      drawMenuSelect();
      break;
    case 1: //down
      if(main_selection<5){main_selection++;}
      else{main_selection = 0;}
      drawMenuSelect();
      break;
    case 2: //A
      enterScene(main_selection);
      break;
    default:
      break;
  }
}

void update_bm(int d) {
  switch(d) {
    case 0: //up
      if(bonus_selection>0){bonus_selection--;}
      else{bonus_selection = BONUS_MAX;}
      drawBonusMenuSelect();
      break;
    case 1: //down
      if(bonus_selection<BONUS_MAX){bonus_selection++;}
      else{bonus_selection = 0;}
      drawBonusMenuSelect();
      break;
    case 2: //A
      enterBonusScene(bonus_selection);
      break;
    default:
      break;
  }
}

void update_faculty(int f) {
  switch(f) {
    case 0: //left
      if(faculty>0){
        faculty--;
        drawFaculty(faculty);
        }
      break;
    case 1: //right
      if(faculty<20){
        faculty++;
        drawFaculty(faculty);
        }
      break;
    case 2: //A

    default:
      break;
  }
}

void update_about(int a) {
  switch(a) {
    case 0: //up
      if(about_Y<0){
        about_Y = about_Y + 10;
        drawAboutPage();
        }
      break;
    case 1: //down
      about_Y = about_Y - 10;
      drawAboutPage();
      break;
    default:
      break;
  }
}

void setup() {
  mySPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  tft.init(170, 320);
  tft.setRotation(1);

  tft.fillScreen(ST77XX_BLACK);

  displayLogo();

  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setCursor(50, 150);
  tft.print("Calibrating... Do not touch the pads");

  calibrateTouch();
  tft.fillScreen(ST77XX_BLACK);
  displayLogo();
  delay(10);
  drawMainMenu();
}

void loop() {
  updateTouchReadings();

  switch (scene) {
    case 0: //main menu
      if(touchTest(2)){update_mm(0);} //UP
      if(touchTest(3)){update_mm(1);} //DOWN
      if(touchTest(0)){update_mm(2);} //A
      break;
    case 1: //How to
      break;
    case 2: //Cyber
      if (!gameWon && !gameLost) {
        if (touchTest(4)) moveSelectionLeft();   // LEFT
        if (touchTest(5)) moveSelectionRight();  // RIGHT
        if (touchTest(2)) toggleSelectedBit();   // UP
        if (touchTest(3)) toggleSelectedBit();   // DOWN
        if (touchTest(0)) submitGuess();         // A
        if (touchTest(1)) returnMainMenu();       // B
      } else {
        if (touchTest(0)) {
          newCyberGame();
        }
        if (touchTest(1)) {
          returnMainMenu();
        }
      }
      break;
    case 3: //EE
      if (gameComplete) {
        if (touchTest(0)) {   // A = next level
          currentLevel++;
          if (currentLevel >= totalLevels) {
            currentLevel = 0;
            showGameComplete();
            delay(1500);
          }
          initLevel(currentLevel);
          drawCircuit();
        }
        else if (touchTest(1)) {   // B = restart
          currentLevel = 0;
          initLevel(currentLevel);
          drawCircuit();
        }
      }

      if (touchTest(4)) {          // LEFT
        eeMoveSelectionLeft();
      }
      else if (touchTest(5)) {     // RIGHT
        eeMoveSelectionRight();
      }
      else if (touchTest(0)) {     // A
        selectCurrentNode();
      }
      else if (touchTest(1)) {     // B
        returnMainMenu();
      }
      else if (touchTest(3)) {     // DOWN
        undoMove();
      }
      else if (touchTest(2)) {     // UP
        showHintOverlay = !showHintOverlay;
        drawCircuit();
      }
      break;
    case 4: //CPE
      if (won) {
        if (touchTest(0)){startCPELevel(currentCpeLevel + 1);}  // A
      }
      else {
        if (touchTest(0)){cpeInputHandling(0);}  // A
        if (touchTest(1)){returnMainMenu();}  // B
        if (touchTest(2)){cpeInputHandling(2);}  // UP
        if (touchTest(3)){cpeInputHandling(3);}  // DOWN
        if (touchTest(4)){cpeInputHandling(4);}  // LEFT
        if (touchTest(5)){cpeInputHandling(5);}  // RIGHT
      }
      break;
    case 5: //Bonus
      if(touchTest(2)){update_bm(0);} //UP
      if(touchTest(3)){update_bm(1);} //DOWN
      if(touchTest(0)){update_bm(2);} //A
      break;
    case 6: //About
      if(touchTest(2)){update_about(0);} //UP
      if(touchTest(3)){update_about(1);} //DOWN
      if(touchTest(1)){returnMainMenu();}//B
      break;
    case 7: //Faculty and staff
      if (touchTest(4)){update_faculty(0);}  // left
      if (touchTest(5)){update_faculty(1);}  // right
      if(touchTest(1)){scene = 5; drawBonusMenu();}//B
      break;
    case 8: //Github QR
      if(touchTest(1)){scene = 5; drawBonusMenu();}//B
      break;
    default:
      break;
  }

  delay(60);
}
