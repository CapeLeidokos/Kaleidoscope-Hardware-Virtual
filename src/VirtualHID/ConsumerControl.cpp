#include "ConsumerControl.h"
#include "Logging.h"
#include "virtual_io.h"

using namespace kaleidoscope::logging;

ConsumerControl_::ConsumerControl_(void) {}
void ConsumerControl_::begin(void) { end(); }
void ConsumerControl_::end(void) { releaseAll(); sendReport(); }
void ConsumerControl_::releaseAll(void) {
  memset(&_report, 0, sizeof(_report));
}

// write(), press(), and release() are essentially taken directly from KeyboardioHID
void ConsumerControl_::write(uint16_t m) {
  press(m);
  release(m);
}
void ConsumerControl_::press(uint16_t m) {
  // search for a free spot
  for (uint8_t i = 0; i < sizeof(_report) / 2; i++) {
    if (_report.keys[i] == 0x00) {
      _report.keys[i] = m;
      break;
    }
  }
}
void ConsumerControl_::release(uint16_t m) {
  // search and release the keypress
  for (uint8_t i = 0; i < sizeof(_report) / 2; i++) {
    if (_report.keys[i] == m) {
      _report.keys[i] = 0x00;
      // no break to delete multiple keys
    }
  }
}

void ConsumerControl_::sendReport() {
  // Only send the report if different from last report
  // (following KeyboardioHID - see comments there)
  if(memcmp(&_lastReport, &_report, sizeof(_report)) == 0)
    return;

  sendReportUnchecked();
  memcpy(&_lastReport, &_report, sizeof(_report));
}

void ConsumerControl_::sendReportUnchecked() {
  log_info("A virtual ConsumerControl HID report was sent.\n");
  logUSBEvent("ConsumerControl HID report", &_report, sizeof(_report));
}

ConsumerControl_ ConsumerControl;
