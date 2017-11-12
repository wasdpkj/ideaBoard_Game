// Space Shooter
// K. M. Kroski
// 14 Dec 2016

#include <Arduboy.h>

Arduboy arduboy;

const byte PROGMEM hit_sound [] = {0x90,83, 0,75, 0x80, 0x90,88, 0,225, 0x80, 0xf0};
const byte PROGMEM death_sound [] = {0x90,83, 0,75, 0x80, 0x90,88, 0,225, 0x80, 0xf0};
const byte PROGMEM game_over_sound [] = {0x90,83, 0,75, 0x80, 0x90,88, 0,225, 0x80, 0xf0};

int upbuffer = 0;
int downbuffer = 0;
int abuffer = 0;
int bbuffer = 0;

byte state = 0;

int score = 0;
byte lives = 3;
byte y_pos = 27;

byte enemy_count = 6;
int enemy[][3] = {
  {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
};

byte bullets_count = 8;
int bullets[][3] = {
  {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
};

byte stars_count = 16;
int stars[][2] = {
  {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, 
  {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}
};

void drawHud() {
  arduboy.fillRect(0, 55, 128, 1, WHITE);
  arduboy.setCursor(0,57);
  arduboy.setTextSize(1);
  arduboy.print(score);

  for (int i = 2; i >= 0; --i) {
    int y_pos = 125 - i * 8;
    if (i+1 > lives) {
      arduboy.drawTriangle(y_pos, 57, y_pos - 2, 63, y_pos + 2, 63, WHITE);
    } else {
      arduboy.fillTriangle(y_pos, 57, y_pos - 2, 63, y_pos + 2, 63, WHITE);
    }
  }
}

void drawStars() {
  for (int i = 0; i < stars_count; i++) {
    stars[i][0] -= 1;

    if (stars[i][0] < 0) {
      stars[i][0] = 128;
      stars[i][1] = random(0, 56);
    }
    
    arduboy.drawPixel(stars[i][0], stars[i][1], WHITE);
  }
}

void drawShip() {
  arduboy.fillTriangle(0, y_pos - 2, 0, y_pos + 2, 6, y_pos, WHITE);
}

void handleButtons() {
  if (state != 1 && arduboy.pressed(B_BUTTON) && bbuffer == 0) {
    bbuffer = 1;
    state = 1;
    resetGame();
  } else if (state != 1) {
    return;
  }
  
  if (arduboy.pressed(UP_BUTTON) && upbuffer == 0) {
    upbuffer = 1;
    y_pos--;
  } else if (arduboy.pressed(DOWN_BUTTON) && downbuffer == 0) {
    downbuffer = 1;
    y_pos++;
  } 

  if (arduboy.pressed(A_BUTTON) && abuffer == 0) {
    abuffer = 10;
    activateBullet(y_pos);
  }

  if (y_pos < 2) {
    y_pos = 2;
  }
  
  if (y_pos > 52) {
    y_pos = 52;
  }
}

void activateBullet(int y_pos) {
  for (int i = 0; i < bullets_count; i++) {
    if (bullets[i][0] == 0) {
      bullets[i][0] = 1;
      bullets[i][1] = 0;
      bullets[i][2] = y_pos;

      score -= 10;
      
      break;
    }
  }
}

void drawBullets() {
  for (int i = 0; i < bullets_count; i++) {
    if (bullets[i][0] == 1) {
      bullets[i][1] += 2;

      arduboy.fillTriangle(bullets[i][1], bullets[i][2] - 1, bullets[i][1], bullets[i][2] + 1, bullets[i][1] + 2, bullets[i][2], WHITE);
    }

    if (bullets[i][1] >= 128) {
      bullets[i][0] = 0;
    }
  }
}

void drawEnemy() {
  for (int i = 0; i < enemy_count; i++) {
    enemy[i][1] -= 2;
    
    if (enemy[i][0] == 1) {
      arduboy.fillRect(enemy[i][1] - 3, enemy[i][2] - 1, 6, 3, WHITE);
      arduboy.drawCircle(enemy[i][1] - 3, enemy[i][2], 3, WHITE);
    }

    if (enemy[i][0] != 0 && enemy[i][0] != 1) {
      enemy[i][1] += 2;
      enemy[i][0]--;

      if (enemy[i][0] == 1) {
        enemy[i][0] = 0;
      }
      
      arduboy.fillCircle(enemy[i][1] - 3, enemy[i][2], 3, WHITE);
    }

    if (enemy[i][1] < -2) {
      enemy[i][0] = 1;
      enemy[i][1] = 132;
      enemy[i][2] = random(3, 52);
    }
  }
}

void checkBulletCollision() {
  for (int i = 0; i < enemy_count; i++) {
    for (int j = 0; j < bullets_count; j++) {
      if (
        enemy[i][0] == 1 && bullets[j][0] == 1 &&
        (abs(enemy[i][1] - bullets[j][1]) * 2 < (9 + 2)) && 
        (abs(enemy[i][2] - bullets[j][2]) * 2 < (6 + 3))
      ) {
        enemy[i][0] = 20;
        bullets[j][0] = 0;
        score += 100;

        arduboy.tunes.playScore(hit_sound);
      }
    }
  }
}

void checkEnemyCollision() {
  for (int i = 0; i < enemy_count; i++) {
    if (
      enemy[i][0] == 1 &&
      (abs(enemy[i][1] - 6) * 2 < (9 + 6)) && 
      (abs(enemy[i][2] - y_pos) * 2 < (6 + 5))
    ) {
      enemy[i][0] = 0;
      lives--;

      for (int i = 0; i < enemy_count; i++) {
        enemy[i][0] = 1;
        enemy[i][1] = 264 + random(0, 127);
        enemy[i][2] = random(3, 52);
      }

      arduboy.invert(true);
      delay(20);
      arduboy.invert(false);

      if (lives <= 0) {
        state = 2;
        
        arduboy.tunes.playScore(game_over_sound);
      } else {
        arduboy.tunes.playScore(death_sound);
      }
      
      break;
    }
  }
}

void resetGame() {
  score = 0;
  lives = 3;
  y_pos = 27;

  for (int i = 0; i < stars_count; i++) {
    stars[i][0] = random(0, 127);
    stars[i][1] = random(0, 56);
  }

  for (int i = 0; i < enemy_count; i++) {
    enemy[i][0] = 1;
    enemy[i][1] = 264 + random(0, 127);
    enemy[i][2] = random(3, 52);
  }
}

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60);

  randomSeed(1);

  resetGame();
}

void loop() {
  if (!arduboy.nextFrame()) { return; }
  
  arduboy.clear();

  downbuffer--;
  if (downbuffer < 0) { downbuffer = 0; }
  upbuffer--;
  if (upbuffer < 0) { upbuffer = 0; }
  abuffer--;
  if (abuffer < 0) { abuffer = 0; }
  bbuffer--;
  if (bbuffer < 0) { bbuffer = 0; }

  handleButtons();

  checkBulletCollision();
  checkEnemyCollision();

  drawStars();
  drawHud();

  switch (state) {
    case 0:
      arduboy.setCursor(10, 20);
      arduboy.setTextSize(2);
      arduboy.print("STARDEATH");
      
      arduboy.setCursor(16, 41);
      arduboy.setTextSize(1);
      arduboy.print("Press B to start");
      break;

    case 1:
      drawShip();
      drawEnemy();
      drawBullets();
      break;

    case 2:
      arduboy.setCursor(10, 20);
      arduboy.setTextSize(2);
      arduboy.print("GAME OVER");
      
      arduboy.setCursor(10, 41);
      arduboy.setTextSize(1);
      arduboy.print("Press B to restart");
      break;
  }
  
  arduboy.display();
}
