/*  Blocks
 *
 *      Move blocks around to where they belong.
 *
 *  (C) 2016 William Woody, all rights reserved.
 */

/*  Blocks: A simple Arduboy game.
 *
 *  Copyright © 2016 by William Edward Woody
 *
 *  This program is free software: you can redistribute it and/or modify it 
 *  under the terms of the GNU General Public License as published by the 
 *  Free Software Foundation, either version 3 of the License, or (at your 
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along 
 *  with this program. If not, see <http://www.gnu.org/licenses/>
 */

#include <stdint.h>
#include <avr/pgmspace.h>
#include "Arduboy.h"
#include "levels.h"

/****************************************************************************/
/*                                                                          */
/*  Icons                                                                   */
/*                                                                          */
/****************************************************************************/

/*  BrickPattern
 *
 *      Background brick pattern
 */

static const uint8_t PROGMEM BrickPattern[] = {
    0x80, 0x80, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00
};

/*  BoxPattern
 *
 *      Pattern used to draw a box
 */

static const uint8_t PROGMEM BoxPattern[] = {
    0xFF, 0xC3, 0xA5, 0x99, 0x99, 0xA5, 0xC3, 0xFF  
};

/*  EndPattern
 *   
 *      The pattern used to draw where the box goes
 */

static const uint8_t PROGMEM EndPattern[] = {
    0xAA, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x55
};

/*  PlayerRight
 *   
 *      Player pushing right
 */

static const uint8_t PROGMEM PlayerRight[] = {
    0x00, 0x00, 0xE0, 0x1F, 0x2B, 0xE8, 0x08, 0x0C
};

/*  PlayerRight
 *   
 *      Player pushing left
 */

static const uint8_t PROGMEM PlayerLeft[] = {
    0x0C, 0x08, 0xE8, 0x2B, 0x1F, 0xE0, 0x00, 0x00
};

/*  PlayerUp
 *   
 *      Player pushing up
 */

static const uint8_t PROGMEM PlayerUp[] = {
    0x01, 0x07, 0xE8, 0x16, 0x16, 0xE8, 0x07, 0x01
};

/*  PlayerDown
 *   
 *      Player pushing down
 */

static const uint8_t PROGMEM PlayerDown[] = {
    0x80, 0xE0, 0x17, 0x68, 0x68, 0x17, 0xE0, 0x80
};

/*  PlayerStanding
 *   
 *      Player standing
 */

static const uint8_t PROGMEM PlayerStanding[] = {
    0x00, 0x08, 0xE4, 0x1B, 0x1B, 0xE4, 0x08, 0x00
};

/****************************************************************************/
/*                                                                          */
/*  Constants                                                               */
/*                                                                          */
/****************************************************************************/

#define STATE_START         0       // Start/scores cycle
#define STATE_STARTPLAY     1       // Start at level 1
#define STATE_STARTLEVEL    2       // Start level
#define STATE_PLAY          3       // Play level
#define STATE_ENDLEVEL      4       // Level has ended.

#define PLAYSTATE_STANDING  0
#define PLAYSTATE_LEFT      1
#define PLAYSTATE_RIGHT     2
#define PLAYSTATE_UP        3
#define PLAYSTATE_DOWN      4

#define HIT_NOTHING         255     // Constants for when we don't hit a box
#define HIT_WALL            254

/****************************************************************************/
/*                                                                          */
/*  Globals                                                                 */
/*                                                                          */
/****************************************************************************/

Arduboy arduboy;

static uint8_t GState;              // Current play state.
static uint8_t GLevel;              // Level index
static uint16_t GLevelRef[8];       // references the level with the map
static uint8_t GBoxCount;           // # boxes and ends.
static uint8_t GBoxes[MAXBOXES];    // Location of the boxes
static uint8_t GEnds[MAXBOXES];     // Location where the boxes go
static uint8_t GPlayerPos;          // Where the player is
static uint8_t GPlayerState;        // Draw state for player

static uint8_t oldBtnState;         // Track button state to find down transitions
static uint8_t curBtnState;

static uint16_t GPlayMove;          // Timer used to count # frames between player moving.

/****************************************************************************/
/*                                                                          */
/*  ButtonStates                                                            */
/*                                                                          */
/****************************************************************************/

/*  UpdateButtonState
 *
 *      Update the button state
 */

static void UpdateButtonState()
{
    oldBtnState = curBtnState;
    curBtnState = ArduboyCore::buttonsState();
}

/*  ButtonKeyDown
 *   
 *      Determine if a button was just pressed. This tests the transition
 *  from the previous state. Requires that UpdateButtonState was called.
 */
static boolean ButtonKeyDown(uint8_t flag)
{
    if (flag & oldBtnState) return false;
    if (0 == (flag & curBtnState)) return false;
    return true;
}

/****************************************************************************/
/*                                                                          */
/*  Title/High Scores                                                       */
/*                                                                          */
/****************************************************************************/

/*  PollFireButton
 *   
 *      Poll the fire button
 */

static boolean PollFireButton(int del)
{
    for (int i = 0; i < del; ++i) {
        delay(15);

        UpdateButtonState();
        if (ButtonKeyDown(A_BUTTON)) return true;
    }
    return false;
}

/*  ShowHelpScreen
 *   
 *      Show how to play
 */
static void ShowHelpScreen()
{
    arduboy.clear();
    arduboy.setCursor(0,0);
    arduboy.print("How to play:");

    arduboy.setCursor(10,15);
    arduboy.print("Push the boxes");
    arduboy.drawBitmap(99,15,BoxPattern,8,8,WHITE);
    arduboy.setCursor(10,25);
    arduboy.print("Where they belong");
    arduboy.drawBitmap(117,25,EndPattern,8,8,WHITE);

    arduboy.setCursor(0,40);
    arduboy.print("If you get stuck, you");
    arduboy.setCursor(0,50);
    arduboy.print("can reset the level.");
    arduboy.display();
    
    PollFireButton(1000);
}

/*  ShowTitleScreenDisplay
 * 
 *      Common title display
 */

static void ShowTitleScreenDisplay()
{
    arduboy.clear();
    arduboy.setCursor(28,0);
    arduboy.setTextSize(2);
    arduboy.print("BLOCKS!");

    arduboy.setTextSize(1);
    arduboy.setCursor(14,36);
    arduboy.print("Start Game");
    arduboy.drawCircle(85,40,6,6);

    arduboy.setCursor(30,24);
    arduboy.print("How to play");
    arduboy.drawCircle(105,28,6,6);

    arduboy.setCursor(7,56);
    arduboy.print("(C) 2016 Bill Woody");

    arduboy.display();
}

/*  ShowTitleScreen
 *   
 *      Shows the title screen. This will set the state to the next thing 
 *  to display as appropriate.
 */

static void ShowTitleScreen()
{
    ShowTitleScreenDisplay();

    for (;;) {
        if (!arduboy.nextFrame()) continue;

        UpdateButtonState();
        if (ButtonKeyDown(A_BUTTON)) {
            GState = STATE_STARTPLAY;
            break;
        }
        if (ButtonKeyDown(B_BUTTON)) {
            ShowHelpScreen();
            ShowTitleScreenDisplay();
        }
    }
}

/****************************************************************************/
/*                                                                          */
/*  Game Play                                                               */
/*                                                                          */
/****************************************************************************/

/*  DrawGamePlay
 *
 *      This erases all of the areas where there are no walls, then draws the
 *  game play areas
 */

static void DrawGamePlay()
{
    // Erase ground area
    for (byte y = 0; y < 8; ++y) {
        uint16_t w = GLevelRef[y];
        for (byte x = 0; x < 16; ++x) {
            uint16_t m = (1 << (15 - x));
            if (0 == (w & m)) {
                // Draw block at this location
                arduboy.fillRect(x*8,y*8,8,8,BLACK);
            }
        }
    }

    // Draw boxes
    for (byte i = 0; i < GBoxCount; ++i) {
        uint8_t p = GBoxes[i];
        uint8_t x = p >> 4;
        uint8_t y = p & 0x0F;

        arduboy.drawBitmap(x*8,y*8,BoxPattern,8,8,WHITE);
    }
    
    // Draw endpoints
    for (byte i = 0; i < GBoxCount; ++i) {
        uint8_t p = GEnds[i];
        uint8_t x = p >> 4;
        uint8_t y = p & 0x0F;

        arduboy.drawBitmap(x*8,y*8,EndPattern,8,8,WHITE);
    }

    // Draw player
    uint8_t x = GPlayerPos >> 4;
    uint8_t y = GPlayerPos & 0x0F;
    const uint8_t *GPlayerIcon;
    switch (GPlayerState) {
        default:
        case PLAYSTATE_STANDING:
            GPlayerIcon = PlayerStanding;
            break;
        case PLAYSTATE_LEFT:
            GPlayerIcon = PlayerLeft;
            break;
        case PLAYSTATE_RIGHT:
            GPlayerIcon = PlayerRight;
            break;
        case PLAYSTATE_UP:
            GPlayerIcon = PlayerUp;
            break;
        case PLAYSTATE_DOWN:
            GPlayerIcon = PlayerDown;
            break;
    }
    arduboy.drawBitmap(x*8,y*8,GPlayerIcon,8,8,WHITE);
}


/****************************************************************************/
/*                                                                          */
/*  Draw boundaries                                                         */
/*                                                                          */
/****************************************************************************/

/*  DrawBoundaries
 * 
 *      Draw boundaries
 */
 
static void DrawBoundaries()
{
    uint8_t y,x;
    uint16_t m,w,wp;
    arduboy.clear();

    // Fill bricks
    for (y = 0; y < 8; ++y) {
        w = GLevelRef[y];
        for (x = 0; x < 16; ++x) {
            m = (1 << (15 - x));
            if (w & m) {
                // Draw block at this location
                arduboy.drawBitmap(x*8,y*8,BrickPattern,8,8,WHITE);
            }
        }
    }

    // Draw boundary lines
    w = GLevelRef[0];
    for (y = 1; y < 8; ++y) {
        wp = w;
        w = GLevelRef[y];
        for (x = 0; x < 16; ++x) {
            m = (1 << (15 - x));
            if (0 != (m & (w ^ wp))) {      // If, at bit m, w and wp are not equal
                if (wp & m) {               // if wp is set
                    arduboy.drawFastHLine(x*8,y*8-1,8,WHITE);
                } else {
                    arduboy.drawFastHLine(x*8,y*8,8,WHITE);
                }
            }
        }
    }

    for (y = 0; y < 8; ++y) {
        m = GLevelRef[y];
        for (x = 1; x < 16; ++x) {
            w = 0x03 & (m >> (15 - x));
            if (w == 0x02) {
                arduboy.drawFastVLine(x*8-1,y*8,8,WHITE);
            } else if (w == 0x01) {
                arduboy.drawFastVLine(x*8,y*8,8,WHITE);
            }
        }
    }
}

/*  LoadLevel
 *   
 *      Load the level stored in GLevel
 */
static void LoadLevel()
{
    GetBoundaries(GLevel,GLevelRef);
    GBoxCount = GetBoxes(GLevel,GBoxes);
    GetEnds(GLevel,GEnds);
    GPlayerPos = GetStartPos(GLevel);
    GPlayerState = PLAYSTATE_STANDING;
}

/*  StartLevel
 *   
 *      Start the level. This draws the level, then superimposes the level
 *  number. We then wait for a few secinds, and redraw the level. This
 *  assumes the level is already set
 */
static void StartLevel()
{
    char text[12];

    // Save level for power cycle
    EEPROM.write(EEPROM_STORAGE_SPACE_START,GLevel+1);
    
    // Load the level, draw the play.
    LoadLevel();
    DrawBoundaries();
    DrawGamePlay();

    // Now draw the level
    sprintf(text,"LEVEL %02d",(int)(GLevel + 1));
    arduboy.fillRect(12,22,104,20,BLACK);
    arduboy.drawRect(12,22,104,20,WHITE);
    arduboy.setCursor(16,24);
    arduboy.setTextSize(2);
    arduboy.print(text);
    arduboy.display();  // FINISH ME

    // Wait for a button.
    PollFireButton(100);

    // Now redraw just the level
    DrawBoundaries();
    DrawGamePlay();
    arduboy.display();

    GState = STATE_PLAY;
}

/*  EndLevel
 *   
 *      End the level. Displays a success message, then determines
 *  if there are any more levels to play.
 */
static void EndLevel()
{
    // Display success
    arduboy.fillRect(12,22,104,20,BLACK);
    arduboy.drawRect(12,22,104,20,WHITE);
    arduboy.setCursor(16,24);
    arduboy.setTextSize(2);
    arduboy.print("SUCCESS!");
    arduboy.display();  // FINISH ME

    // Wait for a button
    PollFireButton(100);

    // Now see if we're out of levels
    GLevel++;
    if (GLevel < MAXLEVELS) {
       GState = STATE_STARTLEVEL;
        
    } else {
        // Spin because we're done.
        EEPROM.write(EEPROM_STORAGE_SPACE_START,0); // clear stored level

        for (;;) {
            arduboy.clear();
            arduboy.setCursor(4,32);
            arduboy.setTextSize(2);
            arduboy.print("GAME OVER!");
            arduboy.display();  // FINISH ME

            if (PollFireButton(25)) {
                GState = STATE_START;
                return;      
            }
            arduboy.clear();
            arduboy.display();  // FINISH ME

            if (PollFireButton(15)) {
                GState = STATE_START;
                return;      
            }
        }
    }
}

/****************************************************************************/
/*                                                                          */
/*  Play logic.                                                             */
/*                                                                          */
/****************************************************************************/

/*  DoRunDialog
 *  
 *      Run the dialog
 */

static boolean DoRunDialog(const char *text)
{
    boolean yn = false;
    boolean d;

    arduboy.fillRect(12,22,104,30,BLACK);
    arduboy.drawRect(12,22,104,30,WHITE);

    arduboy.setTextSize(1);
    arduboy.setCursor((128 - strlen(text) * 6)/2,26);
    arduboy.print(text);

    arduboy.setCursor(29,36);
    arduboy.print("Yes");
    arduboy.setCursor(84,36);
    arduboy.print("No");

    arduboy.drawFastHLine(29,45,17,yn ? WHITE : BLACK);
    arduboy.drawFastHLine(84,45,11,yn ? BLACK : WHITE);

    arduboy.display();

    /*
     * Run our own little loop
     */

    for (;;) {
        if (!arduboy.nextFrame()) continue;

        UpdateButtonState();
        if (ButtonKeyDown(A_BUTTON) || ButtonKeyDown(B_BUTTON)) break;

        d = false;
        if (ButtonKeyDown(LEFT_BUTTON) && !yn) {
            yn = true;
            d = true;
        } else if (ButtonKeyDown(RIGHT_BUTTON) && yn) {
            yn = false;
            d = true;
        }

        if (d) {
            arduboy.drawFastHLine(29,45,17,yn ? WHITE : BLACK);
            arduboy.drawFastHLine(84,45,11,yn ? BLACK : WHITE);
            arduboy.display();
        }
    }

    if (!yn) {
        // Now redraw just the level
        DrawBoundaries();
        DrawGamePlay();
        arduboy.display();
    }
    return yn;
}

/*  DoEndGame
 *   
 *      Determine if we should end the game
 */

static void DoEndGame()
{
    boolean yn = DoRunDialog("End game?");
    
    /*
     *  At this point we have our answer. Change the state as needed
     */

    if (yn) {
        // We hit yes. Reset the state to reload the level
        EEPROM.write(EEPROM_STORAGE_SPACE_START,0); // clear stored level
        GState = STATE_START;
    }
}

/*  DoResetGame
 *
 *      This prompts the user if they want to reset or not. We actually run 
 *  a primitive dialog with a YES or NO option the user can pick from.
 */

static void DoResetGame()
{
    boolean yn = DoRunDialog("Reset level?");
    
    /*
     *  At this point we have our answer. Change the state as needed
     */

    if (yn) {
        // We hit yes. Reset the state to reload the level
        GState = STATE_STARTLEVEL;
    }
}

/*  HitDetect
 *
 *      Determine what is in the given location. Given an X/Y position, determines
 *  if we are at a wall, a box or an empty area. Returns the index of the box, or
 *  one of the wall/nothing constants
 */

static uint8_t HitDetect(int8_t xpos, int8_t ypos)
{
    if ((xpos < 0) || (xpos >= 16) || (ypos < 0) || (ypos >= 8)) return HIT_WALL;

    uint16_t m = GLevelRef[ypos];
    uint16_t w = 1 << (15 - xpos);
    if (m & w) return HIT_WALL;

    uint8_t i;
    uint8_t pos = (xpos << 4) | ypos;
    for (i = 0; i < GBoxCount; ++i) {
        if (pos == GBoxes[i]) return i;
    }

    return HIT_NOTHING;
}

/*  DetermineGameEnd
 *   
 *      This runs through all the boxes and sees if there is a box for every
 *  end location. We assume boxes do not overlap, and all we do is check if
 *  every box is on an end pad.
 */

static boolean DetermineGameEnd()
{
    uint8_t bindex;
    uint8_t eindex;

    for (bindex = 0; bindex < GBoxCount; bindex++) {
        uint8_t bpos = GBoxes[bindex];

        // Determine if box is on an end pad, any end pad
        for (eindex = 0; eindex < GBoxCount; eindex++) {
            if (bpos == GEnds[eindex]) break;
        }

        if (eindex >= GBoxCount) return false;
    }

    return true;
}

/*  DoPlayLogic
 *   
 *      Scan for the up/down/left/right buttons, and handle each. Scan for the
 *  B button to reset
 */

static void DoPlayLogic()
{
    UpdateButtonState();
    if (ButtonKeyDown(B_BUTTON)) {
        DoResetGame();
        return;
    }
    if (ButtonKeyDown(A_BUTTON)) {
        DoEndGame();
        return;
    }

    // Left/right/up/down logic
    int8_t deltax = 0;
    int8_t deltay = 0;
    uint8_t moveState;
    if (ButtonKeyDown(LEFT_BUTTON)) {
        deltax = -1;
        moveState = PLAYSTATE_LEFT;
        GPlayMove = 0;
    } else if (ButtonKeyDown(RIGHT_BUTTON)) {
        deltax = 1;
        moveState = PLAYSTATE_RIGHT;
        GPlayMove = 0;
    } else if (ButtonKeyDown(UP_BUTTON)) {
        deltay = -1;
        moveState = PLAYSTATE_UP;
        GPlayMove = 0;
    } else if (ButtonKeyDown(DOWN_BUTTON)) {
        deltay = 1;
        moveState = PLAYSTATE_DOWN;
        GPlayMove = 0;
    } else {
        /* No button pressed. Increment play move time, and reset if we hit
         * past our threshold
         */

        if (GPlayMove < 120) {
            ++GPlayMove;
            if (GPlayMove >= 120) {
                GPlayerState = PLAYSTATE_STANDING;
                DrawGamePlay();
                arduboy.display();    
            }
        }
        return;
    }

    // We have an up/down operation. First, update
    int8_t x = GPlayerPos >> 4;
    int8_t y = GPlayerPos & 0x0F;
    uint8_t hit = HitDetect(x + deltax, y + deltay);
    if (hit == HIT_WALL) return;        // can't walk into walls.
    if (hit == HIT_NOTHING) {
        // Move the player into the blank space
        x += deltax;
        y += deltay;
        GPlayerPos = (x << 4) | y;
        GPlayerState = PLAYSTATE_STANDING;
        DrawGamePlay();
        arduboy.display();
        return;
    }

    // We're pushing against a box. So look like we're pushing a box.
    GPlayerState = moveState;

    // At this point we've hit a box. Determine if what's behind the box is nothing.
    uint8_t hit2 = HitDetect(x + 2 * deltax, y + 2 * deltay);
    if (hit2 == HIT_NOTHING) {
        // Update the box position
        uint8_t bpos = GBoxes[hit];
        x = bpos >> 4;
        y = bpos & 0x0F;
        x += deltax;
        y += deltay;
        GBoxes[hit] = (x << 4) | y;
        
        // Save some calc time: the player goes where the box was
        GPlayerPos = bpos;

        // Determine if we've completed the move task
        if (DetermineGameEnd()) {
            GState = STATE_ENDLEVEL;
        }
    }

    // TODO: Logic. For now, just draw the game state
    DrawGamePlay();
    arduboy.display();
}

/****************************************************************************/
/*                                                                          */
/*  Entry Points                                                            */
/*                                                                          */
/****************************************************************************/

/*  setup
 * 
 *      Primary setup
 */
void setup() 
{
    // See if the user is powering up with a saved level
    uint8_t savedLevel = EEPROM.read(EEPROM_STORAGE_SPACE_START);
    if ((savedLevel == 0) || (savedLevel > MAXLEVELS)) {
        GState = STATE_START;       // Start state
    } else {
        GLevel = savedLevel-1;
        GState = STATE_STARTLEVEL;
    }

    // Now start up with a cleared screen
    arduboy.beginNoLogo();
    arduboy.setFrameRate(60);
    arduboy.clear();
    arduboy.display();
}

/*  loop
 *   
 *      Execution loop running our code.
 */
void loop() 
{
    if (!arduboy.nextFrame()) return;

    switch (GState) {
        case STATE_START:
            ShowTitleScreen();
            break;
            
        case STATE_STARTPLAY:
            GLevel = 0;
            // Falls through to
        case STATE_STARTLEVEL:
            StartLevel();
            break;

        case STATE_PLAY:
            DoPlayLogic();
            break;

        case STATE_ENDLEVEL:
            EndLevel();
            break;
    }
}

