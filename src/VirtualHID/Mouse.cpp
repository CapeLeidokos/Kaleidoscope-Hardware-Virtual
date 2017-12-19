#include "Mouse.h"
#include "Logging.h"
#include "virtual_io.h"

using namespace kaleidoscope::logging;

Mouse_::Mouse_(void) {}
void Mouse_::begin(void) { end(); }
void Mouse_::end(void) { releaseAll(); sendReport(); }

void Mouse_::releaseAll(void) {
  memset(&report, 0, sizeof(report));
}

void Mouse_::click(uint8_t b) {
  press(b);
  sendReport();
  release(b);
}

void Mouse_::move(signed char x, signed char y, signed char vWheel, signed char hWheel) {
  report.xAxis = x;
  report.yAxis = y;
  report.vWheel = vWheel;
  report.hWheel = hWheel;
}

void Mouse_::press(uint8_t b) {
  report.buttons |= b;
}

void Mouse_::release(uint8_t b) {
  report.buttons &= ~b;
}

bool Mouse_::isPressed(uint8_t b) {
  return ((b & report.buttons) > 0);
}

void Mouse_::sendReport() {
  // Don't send if last report and this report are both empty
  // (following behavior in KeyboardioHID - see comments there)
  static HID_MouseReport_Data_t emptyReport;
  if(memcmp(&lastReport, &report, sizeof(report)) == 0 &&
     memcmp(&report, &emptyReport, sizeof(report)) == 0)
    return;

  sendReportUnchecked();
  memcpy(&lastReport, &report, sizeof(report));
}

void Mouse_::sendReportUnchecked() {
  log_info("A virtual Mouse HID report was sent.\n");
  logUSBEvent("Mouse HID report", &report, sizeof(report));
}

Mouse_ Mouse;
