#include<Arduino.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>


#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978


#define btnRight     0
#define btnUp        1
#define btnDown      2
#define btnLeft      3
#define btnSelect    4
#define btnNone      5


#define btn1      0
#define btn2      1
#define btn3      2
#define btn4      3
#define btn5      4
#define btnNone      5

#define maxShapes    3
#define maxRotations 2
#define melodyPin    3

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

bool matrix[16][4];

int currentX, currentY, currentRot, currentShape, prevKey, gameSpeed, score, highScore, melodyNote;
unsigned long timeToMove;
unsigned long melodyDelay;
boolean melodyOn;

//notes in the melody:
static const int PROGMEM melody[] = {
  NOTE_E5, NOTE_E3, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, 
  NOTE_B4, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A3, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E3, NOTE_E5,
  NOTE_E3, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_B2, 
  NOTE_C3, NOTE_D3, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_G5, 
  NOTE_F5, NOTE_E5, NOTE_C3, 0, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_E5, 
  NOTE_G4, NOTE_C5, NOTE_E4, NOTE_A4, NOTE_E3, NOTE_A4, 0, 
  NOTE_E5, NOTE_E3, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, 
  NOTE_B4, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A3, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E3, NOTE_E5,
  NOTE_E3, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_B2, 
  NOTE_C3, NOTE_D3, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_G5, 
  NOTE_F5, NOTE_E5, NOTE_C3, 0, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_E5, 
  NOTE_G4, NOTE_C5, NOTE_E4, NOTE_A4, NOTE_E3, NOTE_A4, 0,
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_A3, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_GS3, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3, 
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_C4, NOTE_E3, NOTE_E4, NOTE_E3, NOTE_A4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_GS4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_E5, NOTE_E3, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, 
  NOTE_B4, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A3, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E3, NOTE_E5,
  NOTE_E3, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_B2, 
  NOTE_C3, NOTE_D3, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_G5, 
  NOTE_F5, NOTE_E5, NOTE_C3, 0, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_E5, 
  NOTE_G4, NOTE_C5, NOTE_E4, NOTE_A4, NOTE_E3, NOTE_A4, 0, 
  NOTE_E5, NOTE_E3, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, 
  NOTE_B4, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A3, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E3, NOTE_E5,
  NOTE_E3, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_B2, 
  NOTE_C3, NOTE_D3, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_G5, 
  NOTE_F5, NOTE_E5, NOTE_C3, 0, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_E5, 
  NOTE_G4, NOTE_C5, NOTE_E4, NOTE_A4, NOTE_E3, NOTE_A4, 0,
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_A3, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_GS3, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3, 
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_C4, NOTE_E3, NOTE_E4, NOTE_E3, NOTE_A4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_GS4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3,
};

//note durations: 4 = quarter note, 8 = eighth note, etc
static const int PROGMEM noteDurations[] = {
  8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,4,8,8,16,16,8,8,8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,4,
  8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,4,8,8,16,16,8,8,8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,4,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,4,8,8,16,16,8,8,8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,4,
  8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,4,8,8,16,16,8,8,8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,4,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
};

byte cfull[] = {B11111, B11111, B11111, B00000, B11111, B11111, B11111, B11111};
byte ctop[] = {B11111, B11111, B11111, B11111, B00000, B00000, B00000, B00000};
byte cbottom[] = {B00000, B00000, B00000, B00000, B11111, B11111, B11111, B11111,};
byte shapes[3][2][3] = {
  {{B111, B000, B000}, {B100, B100, B100}}, //I
  {{B010, B110, B000}, {B110, B010, B000}}, //J
  {{B100, B110, B000}, {B110, B100, B000}} //L
};

void drawScreen() {
  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i, 0);
    if (matrix[i][0] && matrix[i][1]) lcd.write(byte(2));
    else if (matrix[i][0]) lcd.write(byte(0));
    else if (matrix[i][1]) lcd.write(byte(1));
    else lcd.print(' ');
    lcd.setCursor(i, 1);
    if (matrix[i][2] && matrix[i][3]) lcd.write(byte(2));
    else if (matrix[i][2]) lcd.write(byte(0));
    else if (matrix[i][3]) lcd.write(byte(1));
    else lcd.print(' ');
  }
}

void newShape() {
  currentX = 1;
  currentY = 0;
  currentRot = 0;
  currentShape = rand() % maxShapes;
}

void putShape(boolean visible, int shape, int rot, int x, int y) {
  for (int i = 0; i < 3; i++) {
    if ((shapes[shape][rot][i] >> 2) & 1) matrix[y + i][x] = visible;
    if ((shapes[shape][rot][i] >> 1) & 1) matrix[y + i][x + 1] = visible;
    if (shapes[shape][rot][i] & 1) matrix[y + i][x + 2] = visible;
  }
}

void drawShape() {
  putShape(true, currentShape, currentRot, currentX, currentY);
}

void clearShape() {
  putShape(false, currentShape, currentRot, currentX, currentY);
}

void clearDisplay() {
  for (int i = 0; i < 16; i++) {
    for (int n = 0; n < 4; n++) {
      matrix[i][n] = 0;
    }
  }
}

boolean onScreen(int x, int y) {
  if (x < 0) return false;
  if (x > 3) return false;
  if (y < 0) return false;
  if (y > 15) return false;
  return true;
}

boolean isValid(int rot, int x, int y) {
  boolean okay = true;
  for (int i = 0; ((i < 3) && (okay)); i++) {
    if ((shapes[currentShape][rot][i] >> 2) & 1) {
      if (!onScreen(x, y + i) || matrix[y + i][x]) okay = false;
    }

    if ((shapes[currentShape][rot][i] >> 1) & 1) {
      if (!onScreen(x + 1, y + i) || matrix[y + i][x + 1]) okay = false;
    }

    if (shapes[currentShape][rot][i] & 1) {
      if (!onScreen(x + 2, y + i) || matrix[y + i][x + 2]) okay = false;
    }
  }
  return okay;
}

boolean isValid(int x, int y) {
  return isValid(currentRot, x, y);
}

boolean moveDown() {
  clearShape();
  if (isValid(currentX, currentY + 1)) {
    currentY++;
    drawShape();
    drawScreen();
    return true;
  }
  drawShape();
  return false;
}

boolean moveLeft() {
  clearShape();
  if (isValid(currentX - 1, currentY)) {
    currentX--;
    drawShape();
    drawScreen();
    return true;
  }
  drawShape();
  return false;
}

boolean moveRight() {
  clearShape();
  if (isValid(currentX + 1, currentY)) {
    currentX++;
    drawShape();
    drawScreen();
    return true;
  }
  drawShape();
  return false;
}

boolean rotate() {
  int r;
  clearShape();
  if (currentRot == maxRotations - 1) r = 0;
  else r = currentRot + 1;
  if (isValid(r, currentX, currentY)) {
    currentRot = r;
    drawShape();
    drawScreen();
    return true;
  }
  drawShape();
  return false;
}

void flashLine(int y) {
  for (int t = 0; t < 3; t++) {
    for (int i = 0; i < 4; i++) matrix[y][i] = 1;
    drawScreen();
    tone(melodyPin,NOTE_E5,50);
    //delay(50);
    for (int i = 0; i < 4; i++) matrix[y][i] = 0;
    drawScreen();
    //delay(50);
    tone(melodyPin,NOTE_C5,50);
  }
}

int clearLines() {
  int lineCount = 0;
  boolean tmpmatrix[16][4];
  for (int i = 0; i < 16; i++) {
    for (int n = 0; n < 4; n++) {
      tmpmatrix[i][n] = 0;
    }
  }
  int tmpY = 15;
 
  for (int y = 15; y >= 0; y--) {
    if (matrix[y][0] && matrix[y][1] && matrix[y][2] && matrix[y][3]) {
      flashLine(y);
      lineCount++;
    } else {
      for (int x = 0; x < 4; x++) tmpmatrix[tmpY][x] = matrix[y][x];
      tmpY--;
    }
  }
  if (lineCount > 0) {
    for (int i = 0; i < 16; i++) {
      for (int n = 0; n < 4; n++) {
        matrix[i][n] = tmpmatrix[i][n];
      }
    }
  }
  return lineCount;
}

int getKey() {
   if (digitalRead(A0) == 0) return btn1; 
   else if (digitalRead(A1) == 0) return btn2; 
   else if (digitalRead(A2) == 0) return btn3; 
   else if (digitalRead(A3) == 0) return btn4; 
   else if (digitalRead(A4) == 0) return btn5; 
  else return btnNone;
  delay(20000);

  
  int b = analogRead(A0);
  if (b > 1000) {
    return btnNone;
  delay(8);
  }
  else if (b < 50) return btn1; //0 //0
  else if (b < 200) return btn2; //140 //143
  else if (b < 400) return btn3; //320 // 329
  else if (b < 600) return btn4; //497 //505
  else if (b < 850) return btn5; //647 //744
  
}

int getHighScore() {
  long two = EEPROM.read(0);
  long one = EEPROM.read(1);
  return ((two << 0) & 0xFF) + ((one << 8) & 0xFFFF);
}

void saveHighScore() {
  EEPROM.write(0, (score & 0xFF));
  EEPROM.write(1, ((score >> 8) & 0xFF));
  highScore = score;
}

int getNote(int k) {
  return pgm_read_word_near(melody + k);
}

int getNoteDuration(int k) {
  return pgm_read_word_near(noteDurations + k);
}

void playNote() {
    if (!melodyOn) return;
    if (millis() > melodyDelay) {
      int noteDuration = 1000/getNoteDuration(melodyNote);
      tone(melodyPin, getNote(melodyNote),noteDuration);
      if (melodyNote < 385) melodyNote++;
      else melodyNote = 0;
      int pauseBetweenNotes = noteDuration * 1.30;
      melodyDelay = millis()+pauseBetweenNotes;
    }
}

void initialize() {
  newShape();
  clearDisplay();
  gameSpeed = 1000;
  score = 0;
  timeToMove = millis() + gameSpeed;
  melodyDelay = 0;
  highScore = getHighScore();
  melodyNote = 0;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  digitalWrite(A0, INPUT_PULLUP);
  pinMode(A1, INPUT);
  digitalWrite(A1, INPUT_PULLUP);
  pinMode(A2, INPUT);
  digitalWrite(A2, INPUT_PULLUP);
  pinMode(A3, INPUT);
  digitalWrite(A3, INPUT_PULLUP);
  pinMode(A4, INPUT);
  digitalWrite(A4, INPUT_PULLUP);
  pinMode(melodyPin, OUTPUT);
  melodyOn = true;
  lcd.begin(16, 2);
  lcd.createChar(0, ctop);
  lcd.createChar(1, cbottom);
  lcd.createChar(2, cfull);
  Serial.begin(9600);
  randomSeed(A1);
  initialize();
  lcd.clear();
  lcd.setCursor(1, 0);
 // lcd.print("Tromino");
  lcd.print("Arduino Tetris");
  lcd.setCursor(0, 1);
  lcd.print("by: ANAV SOBTI");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Highscore:");
  lcd.setCursor(0, 1);
  lcd.print(highScore);
  delay(200);
}

void loop() {
  // put your main code here, to run repeatedly:
  newShape();
  drawShape();
  drawScreen();
  while (moveDown()) {
    while (millis() < timeToMove) {
      playNote();
      int k = getKey();
      if (k != prevKey) {
         Serial.println(analogRead(A0));
        switch (k) {
          case btn1:
          while(moveDown()){playNote();}
          //  moveLeft();
            break;
          case btn2:
          moveLeft();
          //  moveRight();
            break;
          case btn3:
          moveRight();
           // rotate();
            break;
          case btn4:
           rotate();
            //while(moveDown()){playNote();}
            break;
          case btn5:
            melodyOn = !melodyOn;
            break;
        }
        prevKey = k;
      }
    }
    timeToMove = millis() + gameSpeed;
  }
  int cleared = clearLines();
  
 // gameSpeed = gameSpeed - (cleared * 20);
  score = score + (10 * (cleared + 1));
  if (currentY == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over");
    lcd.setCursor(0, 1);
    lcd.print("Score:");
    lcd.print(score);
    if (score > highScore) saveHighScore();
    while (1) {
      delay(5000);
    }
  }
}