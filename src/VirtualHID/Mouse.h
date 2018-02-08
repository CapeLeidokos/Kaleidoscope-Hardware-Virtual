#pragma once

// Essentially equivalent to the corresponding header file in KeyboardioHID
// with the goal of having an almost identical interface, with different implementation

#include "GuardedArduino.h"

#define MOUSE_LEFT    (1 << 0)
#define MOUSE_RIGHT   (1 << 1)
#define MOUSE_MIDDLE  (1 << 2)
#define MOUSE_PREV    (1 << 3)
#define MOUSE_NEXT    (1 << 4)
#define MOUSE_ALL     (MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE | MOUSE_PREV | MOUSE_NEXT)

typedef union {
  // Mouse report: 8 buttons, position, wheel
  struct {
    uint8_t buttons;
    int8_t xAxis;
    int8_t yAxis;
    int8_t vWheel;
    int8_t hWheel;
  };
} HID_MouseReport_Data_t;

class Mouse_ {
 public:
  Mouse_(void);
  void begin(void);
  void end(void);
  void click(uint8_t b = MOUSE_LEFT);
  void move(signed char x, signed char y, signed char vWheel = 0, signed char hWheel = 0);
  void press(uint8_t b = MOUSE_LEFT);   // press LEFT by default
  void release(uint8_t b = MOUSE_LEFT); // release LEFT by default
  void releaseAll(void);
  bool isPressed(uint8_t b = MOUSE_LEFT); // check LEFT by default

  void sendReport(void);

 protected:
  HID_MouseReport_Data_t report;
  HID_MouseReport_Data_t lastReport;

 private:
  void sendReportUnchecked(void);
};

extern Mouse_ Mouse;
