#ifndef DEFINESIMAGESANDSOUNDS_h
#define DEFINESIMAGESANDSOUNDS_h
#include <Arduboy.h>

#define FPSDELAY      1000/50
#define BUTTONCD      200
#define GameSeeds     4
#define TileSize      8
#define MinLevelWidth 12

#define RoomFeatures  3
#define RoomWidth     16
#define RoomHeight    16
#define RoomBytes     (RoomHeight/8) * RoomWidth
#define MapRooms      3
#define MapObjects    30
#define SpriteCap     10

#define POINTSMushroom  100
#define POINTSKill      20

PROGMEM const unsigned char gameover [] = {
  0xff, 0x80, 0x0, 0x3f, 0xc3, 0xfc, 0x7, 0xf8, 0x0, 0x80, 0xff, 0xff, 0xe0, 0x42, 0x7, 0xfc, 0xf, 0xf0, 0x80, 0x83, 0x0, 0x20, 0x42, 0x4, 0x4c, 0x8, 0x10, 0x80, 0x83, 0x0, 0x24, 0x42, 0x4, 0x4c, 0x88, 0x10, 0x8f, 0xf3, 0x0, 0x24, 0x42, 0x64, 0x4c, 0x88, 0x10, 0x89, 0x3, 0x23, 0x20, 0x42, 0x64, 0x4c, 0x9, 0x90, 0x89, 0x3, 0x23, 0x20, 0x42, 0x64, 0x4c, 0x9, 0x90, 0x89, 0x3, 0x23, 0x20, 0x42, 0x64, 0x4c, 0x9, 0xf0, 0x89, 0x13, 0x23, 0x21, 0xc2, 0x64, 0x4c, 0x39, 0x0, 0x89, 0x13, 0x23, 0x27, 0xc2, 0x4, 0xc, 0xf9, 0x0, 0x81, 0x3, 0x23, 0x20, 0x42, 0x4, 0xc, 0x9, 0x0, 0x81, 0x3, 0x23, 0x20, 0x42, 0x6, 0xc, 0x9, 0x0, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xfd, 0xf7, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

enum SpriteDataLayout {
  SpriteWidth = 0,
  SpriteHeight,
  SpriteMaxFrame,
  SpriteCyclesPerFrame,
  SpriteFlags,
  SpriteSpeed,
  SpriteIntelligence,
  SpriteMasks,
  SpriteImageData
};

PROGMEM const unsigned char SmallSquarioSprite[] {
  8, 8, 1, 1,// Width, Height, # of Frames, Cycles per Frame
  0, 0, 0,    // Flags, Speed, AI Setting
  1,          // Number of masks
  0b00000000, // Frame 1 Data
  0b00111110,
  0b11100010,
  0b00100010,
  0b11100010,
  0b00100010,
  0b00111110,
  0b00000000,

  0b00000000, // Frame 1 Mask
  0b10111110,
  0b11111110,
  0b00111110,
  0b11111110,
  0b00111110,
  0b10111110,
  0b00000000,

};
PROGMEM const unsigned char BigSquarioSprite[] {
  8, 16, 1, 1, // Width, Height, # of Frames, Cycles per Frame
  0, 0, 0,     // Flags, Speed, AI Setting
  1,          // Number of masks
  0b11111110, // Frame 1 data
  0b00000010,
  0b00000010,
  0b00000010,
  0b00000010,
  0b00000010,
  0b00000010,
  0b11111110,
  0b00011111,
  0b00010000,
  0b11110000,
  0b00010000,
  0b11110000,
  0b00010000,
  0b00010000,
  0b00011111,

  0b11111110, // Frame 1 mask
  0b11111110,
  0b11111110,
  0b11111110,
  0b11111110,
  0b11111110,
  0b11111110,
  0b11111110,
  0b10011111,
  0b00011111,
  0b11111111,
  0b00011111,
  0b11111111,
  0b00011111,
  0b00011111,
  0b10011111
};
PROGMEM const unsigned char TriangleoSprite[] {
  8, 8, 1, 1, // Width, Height, # of Frames, Cycles per Frame
  0b10,       // Flags
  1, 0b11,    // Speed, AI Setting
  1,          // Number of masks
  0b01000000, // Frame 1 data
  0b01110000,
  0b11001100,
  0b01000011,
  0b11000010,
  0b01001100,
  0b01110000,
  0b01000000,

  0b00000000, // Frame 1 mask
  0b00111000,
  0b11111110,
  0b00111111,
  0b11111111,
  0b00111110,
  0b00111000,
  0b00000000
};
PROGMEM const unsigned char StarmanoSprite[] {
  8, 8, 1, 1, // Width, Height, # of Frames, Cycles per Frame
  0b10,       // Flags
  2, 0b1111,  // Speed, AI Setting
  1,          // Number of masks
  0b10000100, // Frame 1 data
  0b11101100,
  0b01111100,
  0b00111111,
  0b00111111,
  0b01111100,
  0b11101100,
  0b10000100,

  0b10000100, // Frame 1 Mask
  0b11101100,
  0b01111100,
  0b00111111,
  0b00111111,
  0b01111100,
  0b11101100,
  0b10000100,

};
PROGMEM const unsigned char SmileoSprite[] {
  8, 8, 1, 1, // Width, Height, # of Frames, Cycles per Frame
  0b10,       // Flags
  2, 0b1,     // Speed, AI Setting
  1,          // Number of masks
  0b00111100, // Frame 1 data
  0b01000010,
  0b10000001,
  0b10101001,
  0b10100001,
  0b10101001,
  0b01000010,
  0b00111100,

  0b00111100, // Mask 1 data
  0b01111110,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b01111110,
  0b00111100

};
PROGMEM const unsigned char MushroomSprite[] {
  8, 8, 1, 1, // Width, Height, # of Frames, Cycles per Frame
  0b10,       // Flags
  1, 0b1,     // Speed, AI Setting
  1,          // Number of masks
  0b00011000, // Frame 1 data
  0b00100100,
  0b11101010,
  0b10100001,
  0b10101001,
  0b11110010,
  0b00100100,
  0b00011000,

  0b00011000, // Frame 1 mask
  0b00111100,
  0b11111110,
  0b11111111,
  0b11111111,
  0b11111110,
  0b00111100,
  0b00011000
};
PROGMEM const unsigned char BoltSprite[] {
  16, 16, 1, 1, // Width, Height, # of Frames, Cycles per Frame
  0b11,         // Flags
  2, 0,         // Speed, AI Setting
  1,            // Number of masks

  0b00000000, // Frame 1 data
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b10000000,
  0b01010000,
  0b00101000,
  0b00000110,
  0b00000001,
  0b01000001,
  0b11100010,
  0b00100010,
  0b00010100,
  0b00001100,
  0b00001000,
  0b10000000, // row2
  0b01000000,
  0b01110000,
  0b00101000,
  0b00100101,
  0b00010010,
  0b00001000,
  0b00001110,
  0b00000110,
  0b00000001,
  0b00000001,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,

  0b00000000, // Mask 1 data
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b10000000,
  0b11010000,
  0b11101000,
  0b11111110,
  0b11111111,
  0b01111111,
  0b11111110,
  0b00111110,
  0b00011100,
  0b00001100,
  0b00001000,

  0b10000000, // row2
  0b01000000,
  0b01110000,
  0b00111000,
  0b00111101,
  0b00011111,
  0b00001111,
  0b00001111,
  0b00000111,
  0b00000001,
  0b00000001,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
};

PROGMEM const unsigned char tMask[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
PROGMEM const unsigned char tBrick[] = { 0xFF, 0xA9, 0xA9, 0xB9, 0xA9, 0xEF, 0xA9, 0xFF };	// 3	Empty
PROGMEM const unsigned char tQBlock[] = { 0xFF, 0x81, 0x85, 0xB5, 0x95, 0x9D, 0x81, 0xFF };
PROGMEM const unsigned char tBQBlock[] =		{ 0xFF, 0x81, 0xA5, 0x81, 0x81, 0xA5, 0x81, 0xFF };	// 9

PROGMEM const unsigned char tPipeLeft[] = 		{ 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00 };	// 4
PROGMEM const unsigned char tPipeRight[] = 		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00 };	// 5

PROGMEM const unsigned char tPipeCapLeft[] = 	{ 0xFF, 0x81, 0x81, 0xFD, 0x81, 0x81, 0x81, 0x81 };	// 6
PROGMEM const unsigned char tPipeCapRight[] = { 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF };	// 7
PROGMEM const unsigned char tTopPipeCapLeft [] = {
  0b11111111, //0xFF,
  0b10000001, //0x81,
  0b10000001,
  0b10111111, //0xFD,
  0b10000001,
  0b10000001,
  0b10000001,
  0b10000001
};
PROGMEM const unsigned char tTopPipeCapRight [] = {
  0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF
};
// 96x32
PROGMEM const unsigned char TitleSquarioText [] = {
  0x80, 0x60, 0x10, 0x08, 0x0C, 0x06, 0x02, 0x03, 0x81, 0x41, 0x61, 0x21, 0xE1, 0x01, 0x01, 0x01,
  0x0F, 0x06, 0x0E, 0x08, 0x30, 0xC0, 0x60, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60,
  0x40, 0xC0, 0x80, 0x80, 0xC0, 0x20, 0x20, 0x20, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x60,
  0x20, 0xC0, 0x80, 0x00, 0x80, 0xC0, 0x40, 0x40, 0x40, 0x40, 0x40, 0xC0, 0x80, 0xC0, 0x40, 0x40,
  0xC0, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0E, 0x1A, 0x32, 0xE2, 0xC6, 0x5C,
  0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x0C, 0x10, 0x20, 0x60, 0xC0, 0x80, 0x0F, 0x1D, 0x38, 0x20, 0x78, 0x67, 0xC0, 0xC0, 0x80,
  0x80, 0x80, 0x80, 0x80, 0xC0, 0x61, 0x1F, 0x00, 0x00, 0x80, 0x60, 0x20, 0x20, 0x20, 0xE0, 0x00,
  0x00, 0x00, 0x01, 0x07, 0xFC, 0x00, 0x00, 0x00, 0x07, 0xFC, 0x80, 0x00, 0x80, 0xC0, 0x7F, 0x00,
  0x00, 0x00, 0xFF, 0x0F, 0x19, 0x10, 0x18, 0x0C, 0x04, 0x0C, 0xF8, 0x00, 0x01, 0x03, 0xFF, 0x60,
  0x01, 0x03, 0x06, 0xF2, 0x13, 0x11, 0x10, 0x10, 0x30, 0x21, 0x23, 0xFE, 0xA3, 0x01, 0x01, 0x07,
  0x1C, 0xFC, 0x86, 0x02, 0x83, 0xC1, 0x41, 0x41, 0x41, 0xC3, 0x06, 0x0C, 0x18, 0x30, 0xC0, 0x00,
  0x00, 0x00, 0xF0, 0x18, 0x08, 0x04, 0x0F, 0x0A, 0x1E, 0x34, 0x6C, 0x68, 0x38, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x03, 0x06, 0x0C, 0x18, 0xE0, 0x00, 0x03, 0x06, 0x04, 0x04, 0x06, 0x03, 0x00,
  0x00, 0x80, 0x80, 0xE0, 0x3F, 0x60, 0x40, 0x40, 0x80, 0x80, 0x81, 0x81, 0x81, 0x80, 0x00, 0x00,
  0x00, 0x00, 0xFF, 0x03, 0x01, 0x79, 0x49, 0x49, 0x49, 0x49, 0x39, 0x00, 0x00, 0x00, 0xFF, 0x70,
  0xC0, 0x00, 0x04, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x80, 0x00,
  0x80, 0xF8, 0xCF, 0x80, 0x83, 0x06, 0x0C, 0x08, 0x08, 0x0D, 0x07, 0x00, 0x00, 0x00, 0x01, 0xFF,
  0x00, 0x00, 0x01, 0x03, 0x06, 0x04, 0x0C, 0x08, 0x18, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
  0x10, 0x10, 0x10, 0x10, 0x18, 0x0C, 0x03, 0x07, 0x04, 0x0C, 0x08, 0x08, 0x08, 0x08, 0x0C, 0x0C,
  0x18, 0x10, 0x10, 0x11, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04,
  0x0C, 0x0C, 0x07, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x03, 0x0E, 0x08, 0x09, 0x0F,
  0x07, 0x07, 0x04, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x02,
  0x03, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x03, 0x02, 0x02, 0x02, 0x03, 0x01, 0x01, 0x01
};
// 53x32
PROGMEM const unsigned char TitleSquarioGuys [] = {
  0x00, 0x80, 0x80, 0x40, 0x40, 0x40, 0x80, 0x80, 0x20, 0x40, 0xF8, 0x40, 0x20, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x80, 0xF0, 0x80, 0xC0, 0x00, 0x00, 0xC2,
  0x3E, 0x02, 0x02, 0x02, 0x82, 0x02, 0x02, 0x02, 0x03, 0x01, 0x81, 0x01, 0x01, 0x01, 0xFF, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xC0, 0x0E, 0x31, 0x20, 0x45, 0x44, 0x45, 0x24, 0x31, 0x0E, 0x03, 0x00,
  0x00, 0x00, 0x80, 0x00, 0xC0, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
  0x06, 0x08, 0x18, 0x10, 0x7F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00,
  0x00, 0x00, 0x00, 0x87, 0x78, 0x10, 0x10, 0x1C, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x8C,
  0x86, 0x8B, 0x83, 0x8B, 0x86, 0x8C, 0xF8, 0x09, 0x09, 0x0F, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x1E, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x1F, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
};
PROGMEM const unsigned char OverworldBG [] = { // 64x16
  0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0xFC, 0x20, 0x20, 0x20, 0x20, 0xF8, 0x08, 0x08, 0x08,
  0x08, 0x08, 0xE8, 0x28, 0x38, 0x20, 0x20, 0x3E, 0x22, 0x22, 0xE2, 0x02, 0x02, 0xFE, 0x20, 0x20,
  0x20, 0x20, 0x20, 0xE0, 0x00, 0x00, 0x00, 0xC0, 0x40, 0x40, 0x40, 0x78, 0x4F, 0xC9, 0x09, 0x09,
  0x09, 0x0F, 0x08, 0x08, 0x08, 0x08, 0xF8, 0x20, 0x20, 0x20, 0x20, 0xF8, 0x08, 0x08, 0x0C, 0xFC,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xFF, 0x01, 0x01, 0x01, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x1F
};
// 37x24
PROGMEM const unsigned char UndergroundBricks [] = {
  0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x54, 0x80,
  0x44, 0x80, 0x44, 0x80, 0x44, 0x80, 0x44, 0x00, 0x44, 0x00, 0x44, 0x00, 0x54, 0x00, 0x40, 0x00,
  0x40, 0x00, 0x40, 0x00, 0x40, 0x50, 0xA0, 0x50, 0xA0, 0x55, 0xA0, 0x51, 0xA0, 0x55, 0x20, 0x00,
  0x20, 0x00, 0x20, 0x55, 0x02, 0x41, 0x02, 0x41, 0x02, 0x41, 0x02, 0x55, 0x02, 0x10, 0x02, 0x10,
  0x02, 0x95, 0x00, 0x90, 0x00, 0x90, 0x00, 0x10, 0x00, 0x15, 0x15, 0x00, 0x10, 0x00, 0x10, 0x00,
  0x10, 0x00, 0x15, 0x00, 0x10, 0x00, 0x10, 0x00, 0x15, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00,
  0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA, 0x00, 0x80, 0x00, 0xAA, 0x00, 0x00, 0x00, 0x00,
};
PROGMEM const byte Verse [] = {
  0x90, 47, 0, 170,
  0x80, 0, 170,
  0x90, 47, 0, 170,
  0x80,
  0x90, 54, 0, 170,
  0x80, 0, 170,
  0x90, 54, 0, 170,
  0x80,
  0x90, 50, 0, 170,
  0x80, 0, 170,
  0x90, 50, 0, 170,
  0x80,
  0x90, 52, 0, 170,
  0x80, 0, 170,
  0x90, 52, 0, 170,
  0x80,
  0x90, 52, 0, 170,
  0x80, 0, 170,
  0x90, 52, 0, 170,
  0x80, 0, 170,
  0xf0
};
PROGMEM const byte PreChorus [] = {
  0x90, 47, 0, 85, 0x80, 0, 85, 0x90, 45, 0, 170, 0x80, 0, 170, 0x90, 47, 0, 170, 0x80, 1, 84,
  0x90, 61, 0, 170, 0x80, 0, 170, 0x90, 61, 0, 170, 0x80, 0x90, 61, 0, 170, 0x80, 0x90, 61, 0, 170,
  0x80, 0x90, 61, 0, 170, 0x80, 0x90, 62, 0, 170, 0x80, 0x90, 59, 1, 84, 0x80, 0xf0
};
PROGMEM const byte Chorus [] = {
  0x90, 64, 0, 227, 0x80, 0, 227, 0x90, 64, 0, 227, 0x80, 0, 227, 0x90, 62, 3, 141, 0x80, 0x90, 62,
  0, 227, 0x80, 0, 227, 0x90, 62, 0, 227, 0x80, 0, 227, 0x90, 61, 3, 141, 0x80, 0x90, 62, 0, 227,
  0x80, 0, 227, 0x90, 62, 0, 227, 0x80, 0, 227, 0x90, 61, 3, 141, 0x80, 0x90, 61, 0, 227, 0x80, 0, 227,
  0x90, 61, 0, 227, 0x80, 0, 227, 0x90, 59, 3, 141, 0x80, 0xf0
};
#define nG3S         415  // 0x01
#define nA3          440  // 0x02
#define nB3          494  // 0x03
#define nC4          523  // 0x04
#define nD4          587  // 0x05
#define nE4          659  // 0x06
#define nG4S         831  // 0x07
#define nA4          880  // 0x08
#define nB4          988  // 0x09
#define nC5          1047  // 0x0A
#define nD5          1175  // 0x0B
#define nE5          1319  // 0x0C
#define nF5          1397  // 0x0D
#define nG5          1568  // 0x0E
#define nA5          1760  // 0x0F

#define nA4S         932

#define nB1          123  // 0x1
#define nD2          147  // 0x2
#define nF2          175  // 0x3
#define nA2          220  // 0x4
#define nD3          294  // 0x5
#define nB5          1975 // 0xF
#define nC5S         1109 // 0x9
#define nD5S         1245 // 0xB
#define nC6          2094 // 0xD
#define nE6          2638 // 0xE
PROGMEM const unsigned int SFXFrequencies [] {
  nB1, nD2, nF2,
  nA2, nD3, nA3, nD4,
  nC5, nC5S, nD5, nD5S, nE5,
  nC6, nE6, nB5
};
PROGMEM const byte SFX_Pipe [] = { 0, 32, 0xD1, 0xC1, 0xB1, 0xA3, 0xD1, 0xC1, 0xB1, 0xA1, 0xFF };
PROGMEM const byte SFX_Hit [] = { 0, 16, 0xC1, 0xB1, 0xA1, 0x91, 0xFF };
PROGMEM const byte SFX_Jump [] = { 0, 16, 0x21, 0x11, 0x31, 0x21, 0xFF };
PROGMEM const byte SFX_Mushroom [] = { 0, 64, 0x41, 0x41, 0x51, 0x51, 0x61, 0x61, 0x71, 0x71, 0xFF };
PROGMEM const byte SFX_Coin [] = { 0, 16, 0xF1, 0xE4, 0xFF };
PROGMEM const byte SFX_Life [] = { 0, 32, 0xE3, 0xD3, 0xE3, 0xD3, 0xC5, 0x85, 0xFF };
#endif