/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Virtual -- Test and debug Kaleidoscope sketches, plugins, and core
 * Copyright (C) 2017  Craig Disselkoen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <Arduino.h>
#define HARDWARE_IMPLEMENTATION Virtual

#define COLS 16
#define ROWS 4
#define LED_COUNT 64

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} cRGB;
typedef union {
  uint8_t rows[4];
  uint32_t all;
} keydata_t;

#define CRGB(r, g, b) (cRGB){r, g, b}

class Virtual {
 public:

  typedef enum {
    PRESSED,
    NOT_PRESSED,
    TAP,
  } keystate;

  Virtual(void);
  void setup(void);

  void readMatrix(void);
  void actOnMatrixScan(void);

  void maskKey(KeyAddr keyAddr);
  KS_ROW_COL_FUNC void maskKey(byte row, byte col) {
    maskKey(KeyAddr(row, col));
  }

  void unMaskKey(KeyAddr keyAddr);
  KS_ROW_COL_FUNC void unMaskKey(byte row, byte col) {
    unMaskKey(KeyAddr(row, col));
  }

  bool isKeyMasked(KeyAddr keyAddr);
  KS_ROW_COL_FUNC bool isKeyMasked(byte row, byte col) {
    return isKeyMasked(KeyAddr(row, col));
  }
  void maskHeldKeys(void);

  // For virtual hardware, the current state of all LEDs will be logged to a dedicated file in results upon each call to syncLeds()
  void syncLeds(void);
  void setCrgbAt(LEDAddr /*ledAddr*/, cRGB /*color*/);
  KS_ROW_COL_FUNC void setCrgbAt(byte row, byte col, cRGB /*color*/) {
    setCrgbAt(LEDAddr(row, col));
  }
  void setCrgbAt(uint8_t /*i*/, cRGB /*color*/);
  cRGB getCrgbAt(LEDAddr /*ledAddr*/) const;// not part of the official Kaleidoscope-Hardware API, but an extension we use here
  KS_ROW_COL_FUNC cRGB getCrgbAt(byte row, byte col) const {
    return getCrgbAt(LEDAddr(row, col));
  }
  cRGB getCrgbAt(uint8_t /*i*/) const;

  void scanMatrix(void) {
    readMatrix();
    actOnMatrixScan();
  }

  void setEnableReadMatrix(bool state) {
    _readMatrixEnabled = state;
  }

  void setKeystate(KeyAddr keyAddr, keystate ks);
  KS_ROW_COL_FUNC void setKeystate(byte row, byte col, keystate ks) {
    setKeystate(KeyAddr(row, col), ks);
  }

  keystate getKeystate(KeyAddr keyAddr) const;
  KS_ROW_COL_FUNC keystate getKeystate(byte row, byte col) const {
    return getKeystate(KeyAddr(row, col));
  }

  void attachToHost(void);
  void detachFromHost(void);
  void setKeyscanInterval(uint8_t interval);


  /** Key switch states
   *
   * These methods offer a way to peek at the key switch states, for those cases
   * where we need to deal with the state closest to the hardware. Some methods
   * offer a way to check if a key is pressed, others return the number of
   * pressed keys.
   */
  /**
  * Check if a key is pressed at a given position.
  *
  * @param keyAddr the matrix address of the key.
  *
  * @returns true if the key is pressed, false otherwise.
  */
  bool isKeyswitchPressed(KeyAddr keyAddr);
  /**
   * Check if a key is pressed at a given position.
   *
   * @param row is the row the key is located at in the matrix.
   * @param col is the column the key is located at in the matrix.
   *
   * @returns true if the key is pressed, false otherwise.
   */
  KS_ROW_COL_FUNC bool isKeyswitchPressed(byte row, byte col) {
    return isKeyswitchPressed(KeyAddr(row, col));
  }
  /**
   * Check if a key is pressed at a given position.
   *
   * @param keyIndex is the key index, as calculated by `keyIndex`.
   *
   * @note Key indexes start at 1, not 0!
   *
   * @returns true if the key is pressed, false otherwise.
   */
  bool isKeyswitchPressed(uint8_t keyIndex);
  /**
   * Check the number of key switches currently pressed.
   *
   * @returns the number of keys pressed.
   */
  uint8_t pressedKeyswitchCount();




 private:

  keystate keystates[ROWS][COLS];
  keystate keystates_prev[ROWS][COLS];

  cRGB ledStates[LED_COUNT];

  bool _readMatrixEnabled;

  bool anythingHeld();

  // Super inefficient, but fine for our purposes
  bool mask[ROWS][COLS];
};


/* To be used by the hardware implementations, `keyIndex` tells us the index of
 * a key, from which we can figure out the row and column as needed. The index
 * starts at one, so that plugins that work with a list of key indexes can use
 * zero as a sentinel. This is important, because when we initialize arrays with
 * fewer elements than the declared array size, the remaining elements will be
 * zero. We can use this to avoid having to explicitly add a sentinel in
 * user-facing code.
 */
constexpr byte keyIndex(KeyAddr keyAddr) {
  return keyAddr.offset() + 1;
}
KS_ROW_COL_FUNC constexpr byte keyIndex(byte row, byte col) {
  return keyIndex(KeyAddr(row, col));
}

constexpr byte R0C0  = keyIndex(0, 0);
constexpr byte R0C1  = keyIndex(0, 1);
constexpr byte R0C2  = keyIndex(0, 2);
constexpr byte R0C3  = keyIndex(0, 3);
constexpr byte R0C4  = keyIndex(0, 4);
constexpr byte R0C5  = keyIndex(0, 5);
constexpr byte R0C6  = keyIndex(0, 6);
constexpr byte R0C7  = keyIndex(0, 7);
constexpr byte R1C0  = keyIndex(1, 0);
constexpr byte R1C1  = keyIndex(1, 1);
constexpr byte R1C2  = keyIndex(1, 2);
constexpr byte R1C3  = keyIndex(1, 3);
constexpr byte R1C4  = keyIndex(1, 4);
constexpr byte R1C5  = keyIndex(1, 5);
constexpr byte R1C6  = keyIndex(1, 6);
constexpr byte R1C7  = keyIndex(1, 7);
constexpr byte R2C0  = keyIndex(2, 0);
constexpr byte R2C1  = keyIndex(2, 1);
constexpr byte R2C2  = keyIndex(2, 2);
constexpr byte R2C3  = keyIndex(2, 3);
constexpr byte R2C4  = keyIndex(2, 4);
constexpr byte R2C5  = keyIndex(2, 5);
constexpr byte R2C6  = keyIndex(2, 6);
constexpr byte R2C7  = keyIndex(2, 7);
constexpr byte R3C0  = keyIndex(3, 0);
constexpr byte R3C1  = keyIndex(3, 1);
constexpr byte R3C2  = keyIndex(3, 2);
constexpr byte R3C3  = keyIndex(3, 3);
constexpr byte R3C4  = keyIndex(3, 4);
constexpr byte R3C5  = keyIndex(3, 5);
constexpr byte R3C6  = keyIndex(3, 6);
constexpr byte R3C7  = keyIndex(3, 7);

constexpr byte R0C8  = keyIndex(0, 8);
constexpr byte R0C9  = keyIndex(0, 9);
constexpr byte R0C10 = keyIndex(0, 10);
constexpr byte R0C11 = keyIndex(0, 11);
constexpr byte R0C12 = keyIndex(0, 12);
constexpr byte R1C13 = keyIndex(1, 13);
constexpr byte R1C14 = keyIndex(1, 14);
constexpr byte R1C15 = keyIndex(1, 15);
constexpr byte R2C8  = keyIndex(2, 8);
constexpr byte R2C9  = keyIndex(2, 9);
constexpr byte R2C10 = keyIndex(2, 10);
constexpr byte R2C11 = keyIndex(2, 11);
constexpr byte R2C12 = keyIndex(2, 12);
constexpr byte R2C13 = keyIndex(2, 13);
constexpr byte R2C14 = keyIndex(2, 14);
constexpr byte R2C15 = keyIndex(2, 15);
constexpr byte R3C8  = keyIndex(3, 8);
constexpr byte R3C9  = keyIndex(3, 9);
constexpr byte R3C10 = keyIndex(3, 10);
constexpr byte R3C11 = keyIndex(3, 11);
constexpr byte R3C12 = keyIndex(3, 12);
constexpr byte R3C13 = keyIndex(3, 13);
constexpr byte R3C14 = keyIndex(3, 14);
constexpr byte R3C15 = keyIndex(3, 15);



#define KEYMAP_STACKED(                                                 \
               r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6,                \
               r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6,                \
               r2c0, r2c1, r2c2, r2c3, r2c4, r2c5,                      \
               r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r2c6,                \
               r0c7, r1c7, r2c7, r3c7,                                  \
               r3c6,                                                    \
                                                                        \
               r0c9,  r0c10, r0c11, r0c12, r0c13, r0c14, r0c15,         \
               r1c9,  r1c10, r1c11, r1c12, r1c13, r1c14, r1c15,         \
                      r2c10, r2c11, r2c12, r2c13, r2c14, r2c15,         \
               r2c9,  r3c10, r3c11, r3c12, r3c13, r3c14, r3c15,         \
               r3c8,  r2c8,  r1c8, r0c8,                                \
               r3c9)                                                    \
  {                                                                     \
    {r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6, r0c7, r0c8, r0c9, r0c10, r0c11, r0c12, r0c13, r0c14, r0c15}, \
    {r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1c10, r1c11, r1c12, r1c13, r1c14, r1c15}, \
    {r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, r2c6, r2c7, r2c8, r2c9, r2c10, r2c11, r2c12, r2c13, r2c14, r2c15}, \
    {r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6, r3c7, r3c8, r3c9, r3c10, r3c11, r3c12, r3c13, r3c14, r3c15}, \
  }

#define KEYMAP(                                                                                     \
  r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6,        r0c9,  r0c10, r0c11, r0c12, r0c13, r0c14, r0c15, \
  r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6,        r1c9,  r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, \
  r2c0, r2c1, r2c2, r2c3, r2c4, r2c5,                     r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, \
  r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r2c6,        r2c9,  r3c10, r3c11, r3c12, r3c13, r3c14, r3c15, \
              r0c7, r1c7, r2c7, r3c7,                             r3c8,  r2c8,  r1c8, r0c8,         \
                          r3c6,                                          r3c9)                      \
  {                                                                                                 \
    {r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6, r0c7, r0c8, r0c9, r0c10, r0c11, r0c12, r0c13, r0c14, r0c15}, \
    {r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1c10, r1c11, r1c12, r1c13, r1c14, r1c15}, \
    {r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, r2c6, r2c7, r2c8, r2c9, r2c10, r2c11, r2c12, r2c13, r2c14, r2c15}, \
    {r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6, r3c7, r3c8, r3c9, r3c10, r3c11, r3c12, r3c13, r3c14, r3c15}, \
  }
