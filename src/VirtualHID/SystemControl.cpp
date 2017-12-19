#include "SystemControl.h"
#include "Logging.h"
#include "virtual_io.h"

using namespace kaleidoscope::logging;

SystemControl_::SystemControl_(void) {}
void SystemControl_::begin(void) { releaseAll(); }
void SystemControl_::end(void) { releaseAll(); }
void SystemControl_::releaseAll(void) {
  uint8_t _report = 0x00;
  sendReport(&_report, sizeof(_report));
}

void SystemControl_::write(uint8_t s) {
  press(s);
  release();
}

void SystemControl_::release(void) { releaseAll(); }

void SystemControl_::press(uint8_t s) {
  sendReport(&s, sizeof(s));
}

void SystemControl_::sendReport(void* data, int length) {
  log_info("A virtual SystemControl HID report with value %hhu was sent.\n", *(uint8_t*)data);
  logUSBEvent("SystemControl HID report", data, length);
}

SystemControl_ SystemControl;
