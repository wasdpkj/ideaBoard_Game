/**
 * @file ArduboyTones.cpp
 * \brief An Arduino library for playing tones and tone sequences, 
 * intended for the Arduboy game system.
 */

/*****************************************************************************
  ArduboyTones

An Arduino library to play tones and tone sequences.

Specifically written for use by the Arduboy miniature game system
https://www.arduboy.com/
but could work with other Arduino AVR boards that have 16 bit timer 3
available, by changing the port and bit definintions for the pin(s)
if necessary.

Copyright (c) 2017 Scott Allen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*****************************************************************************/

#include "ArduboyTones.h"

// pointer to a function that indicates if sound is enabled
static bool (*outputEnabled)();

static volatile long durationToggleCount = 0;
static volatile bool tonesPlaying = false;
static volatile bool toneSilent;

static volatile uint16_t *tonesStart;
static volatile uint16_t *tonesIndex;
static volatile uint16_t toneSequence[MAX_TONES * 2 + 1];
static volatile bool inProgmem;


ArduboyTones::ArduboyTones(boolean (*outEn)())
{
  outputEnabled = outEn;

  toneSequence[MAX_TONES * 2] = TONES_END;
}

void ArduboyTones::tone(uint16_t freq, uint16_t dur)
{
  inProgmem = false;
  ideaBoard.tone(freq,dur);
}

void ArduboyTones::tone(uint16_t freq1, uint16_t dur1,
                        uint16_t freq2, uint16_t dur2)
{
  inProgmem = false;
  ideaBoard.tone(freq1,dur1);
  delay(dur1);
  ideaBoard.tone(freq2,dur2);
}

void ArduboyTones::tone(uint16_t freq1, uint16_t dur1,
                        uint16_t freq2, uint16_t dur2,
                        uint16_t freq3, uint16_t dur3)
{
  inProgmem = false;
  ideaBoard.tone(freq1,dur1);
  delay(dur1);
  ideaBoard.tone(freq2,dur2);
  delay(dur2);
  ideaBoard.tone(freq3,dur3);
}

void ArduboyTones::tones(const uint16_t *tones)
{
  inProgmem = true;
  tonesStart = tonesIndex = (uint16_t *)tones; // set to start of sequence array  
  nextTone(); // start playing
}

void ArduboyTones::tonesInRAM(uint16_t *tones)
{
  inProgmem = false;
  tonesStart = tonesIndex = tones; // set to start of sequence array
  nextTone(); // start playing
}

void ArduboyTones::noTone()
{
  ideaBoard.noTone();
  tonesPlaying = false;
}

void ArduboyTones::volumeMode(uint8_t mode)
{

}

bool ArduboyTones::playing()
{
  return tonesPlaying;
}

void ArduboyTones::nextTone()
{
  uint16_t freq;
  uint16_t dur;
  long toggleCount;
  uint32_t ocrValue;
  
  while(1){
    freq = getNext(); // get tone frequency
    
    if (freq == TONES_END) { // if freq is actually an "end of sequence" marker
      ideaBoard.noTone(); // stop playing
      return;
    }
    
    tonesPlaying = true;
    
    if (freq == TONES_REPEAT) { // if frequency is actually a "repeat" marker
      tonesIndex = tonesStart; // reset to start of sequence
      freq = getNext();
    }
    
    
    freq &= ~TONE_HIGH_VOLUME; // strip volume indicator from frequency
    
    if (freq == 0) { // if tone is silent
      freq = SILENT_FREQ;
      toneSilent = true;
      //bitClear(TONE_PIN_PORT, TONE_PIN); // set the pin low
    }
    else {
      toneSilent = false;
    }
    
    if (!outputEnabled()) { // if sound has been muted
      toneSilent = true;
    }
    
    
    dur = getNext(); // get tone duration
    
    ideaBoard.tone(freq,dur);
    delay(dur);
  }
}

uint16_t ArduboyTones::getNext()
{
  if (inProgmem) {
    return pgm_read_word(tonesIndex++);
  }
  return *tonesIndex++;
}
