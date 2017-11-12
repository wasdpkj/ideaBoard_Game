//#include <SPI.h>
#include <EEPROM.h>
#include <Arduboy.h>

#include "bitmaps.h"
#include "global.h"

#define WIDTH (128)
#define HEIGHT (64)

#define MIN_GROUND ((int)(0.1 * HEIGHT))
#define MAX_GROUND ((int)(0.5 * HEIGHT))

#define NUM_TANKS   (2)
#define BULLETSPEED (0.8)
#define GRAVITY     (0.005)
#define CANNONSIZE  (5)
#define TANK_WIDTH  (7)
#define TANK_HEIGHT (5)

#define STATE_AIM   (0)
#define STATE_FIRE  (1)

Arduboy arduboy;

Tank tanks[NUM_TANKS];
int curTankIndex = 0;
int state = 0;

boolean prevBDown = false;
//boolean prevADown = false;

double bulletX = 0;
double bulletY = 0;
double bulletDX = 0;
double bulletDY = 0;

// Display the Arduboy logo
void intro()
{
  short x = 128;

  while (x > 46) {
    x -= random(-1, 4);
    
    arduboy.clearDisplay();
    arduboy.setCursor(x, 28);
    arduboy.print("ARDUBOY");
    arduboy.display();
    delay(20);
  }
  
  delay(100);
  arduboy.tunes.tone(987, 160);
  delay(80);
  arduboy.tunes.tone(1318, 400);
  delay(1000);
}

void setup() {
  arduboy.start();
  arduboy.setFrameRate(60);
  arduboy.initRandomSeed(); // sorry, not being called semi-randomly.

  Serial.begin(19200);

  intro();
  
  initRound();
}

void initRound() {
  arduboy.clearDisplay();

  short i;
  short curHeight = random(MIN_GROUND, MAX_GROUND);
  for (i = 0; i < WIDTH; i ++) {
    curHeight += random(-1, 2);
    if (curHeight < MIN_GROUND) {
      curHeight = MIN_GROUND;
    }
    else if (curHeight > MAX_GROUND) {
      curHeight = MAX_GROUND;
    }
    arduboy.drawFastVLine(i, HEIGHT - curHeight, HEIGHT, WHITE);
  }

  Tank baseTank;
  baseTank.health = 3;
  baseTank.aim = 0.5 * PI;
  baseTank.power = 0.5;
  baseTank.y = 0;

  for (i = 0; i < NUM_TANKS; i ++) {
    tanks[i] = baseTank;
    tanks[i].x = (int)((0.2 + (0.6 * i) / (NUM_TANKS - 1)) * WIDTH);
    placeTank(&tanks[i]);
    drawTank(tanks[i]);
  }

  state = STATE_AIM;
  curTankIndex = random(NUM_TANKS);
}

void placeTank(Tank *tank) {
  for (int i = 0; i < HEIGHT; i ++) {
    int whiteCount = 0;
    for (int x = 0; x < TANK_WIDTH; x ++) {
      int pixel = arduboy.getPixel(tank->x + x - 3, i);
      if (pixel == WHITE) {
        whiteCount ++;
        if (whiteCount >= 3) {
          tank->y = i - 1;
          return;
        }
      }
    }
  }
  tank->y = HEIGHT - 1;
}

void drawTank(Tank tank) {
  arduboy.drawBitmap(tank.x - 3, tank.y - 7, tankfill, 8, 8, BLACK);
  arduboy.drawBitmap(tank.x - 3, tank.y - 7, tankline, 8, 8, WHITE);
}

void clearTank(Tank tank) {
  arduboy.drawBitmap(tank.x - 3, tank.y - 7, tankfill, 8, 8, BLACK);
}

bool pointTouchingTank(Tank tank, int bulX, int bulY) {
  return bulX >= tank.x - 3           && bulX <= tank.x + 3 &&
         bulY >= tank.y - TANK_HEIGHT && bulY <= tank.y;
}

void update() {
  if (state == STATE_AIM) {
    updateAim();
  }
  else {
    for (short i = 0; i < 4 && state == STATE_FIRE; i ++) {
      updateBullet();
    }
  }
}

void updateAim() {
  Tank *curTank = &tanks[curTankIndex];

  int aimX = (int)round(CANNONSIZE * cos(curTank->aim));
  int aimY = (int)round(CANNONSIZE * sin(curTank->aim));
  
  arduboy.drawPixel(curTank->x + aimX, curTank->y - aimY - 1, BLACK);

  boolean bDown = arduboy.pressed(B_BUTTON);
  if (bDown && !prevBDown) {
    state = STATE_FIRE;
    bulletX = curTank->x + CANNONSIZE * cos(curTank->aim);
    bulletY = curTank->y - 1 - CANNONSIZE * sin(curTank->aim);
    bulletDX = curTank->power * BULLETSPEED * cos(curTank->aim);
    bulletDY = -curTank->power * BULLETSPEED * sin(curTank->aim);
    arduboy.tunes.tone(523, 50);
    arduboy.drawFastHLine(1, 1, WIDTH-1, BLACK);
    drawTank(*curTank);
    prevBDown = bDown;
    return;
  }

  boolean aDown = arduboy.pressed(A_BUTTON);
  double dAim = 0.05;
  double dPower = 0.01;
  if (aDown) {
    dAim *= 0.5;
    dPower *= 0.5;
  }
  if (arduboy.pressed(LEFT_BUTTON)) {
    curTank->aim += dAim;
    if (curTank->aim > PI) {
      curTank->aim = PI;
    }
  }
  if (arduboy.pressed(RIGHT_BUTTON)) {
    curTank->aim -= dAim;
    if (curTank->aim < 0) {
      curTank->aim = 0;
    }
  }
  if (arduboy.pressed(DOWN_BUTTON)) {
    curTank->power -= dPower;
    if (curTank->power < 0) {
      curTank->power = 0;
    }
  }
  if (arduboy.pressed(UP_BUTTON)) {
    curTank->power += dPower;
    if (curTank->power > 1) {
      curTank->power = 1;
    }
  }

  aimX = (int)round(CANNONSIZE * cos(curTank->aim));
  aimY = (int)round(CANNONSIZE * sin(curTank->aim));
  arduboy.drawPixel(curTank->x + aimX, curTank->y - 1 - aimY, WHITE);
  arduboy.drawFastHLine(1, 1, WIDTH-1, BLACK);
  arduboy.drawFastHLine(1, 1, curTank->power * (WIDTH-2), WHITE);

  drawTank(*curTank);

  prevBDown = bDown;
}

void updateBullet() {
  arduboy.drawPixel((int)round(bulletX), (int)round(bulletY), BLACK);
  
  bulletX += bulletDX;
  bulletY += bulletDY;
  bulletDY += GRAVITY;

  int bulX = (int)round(bulletX);
  int bulY = (int)round(bulletY);

  int pixel = arduboy.getPixel(bulX, bulY);
  if (bulX < 0 || bulX >= WIDTH || bulY < 0) {
    pixel = BLACK;
  }
  if (bulY >= HEIGHT) {
    pixel = WHITE; // bouncing could be more fun
  }
  
  if (pixel == WHITE) {
    state = STATE_AIM;
    curTankIndex ++;
    if (curTankIndex >= NUM_TANKS) {
      curTankIndex = 0;
    }
    // check collision here, I think.
    bool hit = false;
    bool gameover = false;
    int i;
    for (i = 0; i < NUM_TANKS; i ++) {
      if (pointTouchingTank(tanks[i], bulX, bulY)) {
        hit = true;
        tanks[i].health --;
        if (tanks[i].health <= 0) {
          gameover = true;
        }
      }
    }
    
    arduboy.fillCircle(bulX, bulY, 5, WHITE);
    arduboy.display();
    delay(50);
    arduboy.fillCircle(bulX, bulY, 5, BLACK);
    arduboy.display();
    int t;
    if (gameover) {
      // display a win message?
      // for the moment, just start a new round
      arduboy.tunes.tone(587, 40);
      delay(160);
      arduboy.tunes.tone(392, 40);
      delay(160);
      arduboy.tunes.tone(523, 320);
      delay(320);
      initRound();
      return;
    }
    else if (hit) {
      arduboy.tunes.tone(1048, 30);
    }
    else {
      arduboy.tunes.tone(262, 30);
    }
    delay(100);
    for (i = 0; i < NUM_TANKS; i ++) {
      clearTank(tanks[i]);
      placeTank(&tanks[i]);
      drawTank(tanks[i]);
    }
  }
  else {
    arduboy.drawPixel(bulX, bulY, WHITE);
  }
}

void render() {
  arduboy.display();
}

void loop() {
  if (!(arduboy.nextFrame()))
    return;

  //boolean aDown = arduboy.pressed(A_BUTTON);
  //if (aDown && !prevADown) {
  //  initRound();
  //}
  else {
    update();
  }
  render();
  //prevADown = aDown;
}



