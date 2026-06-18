#include <Arduino.h>
#include <pypilot_syslib.hpp>

pypilot_syslib::ArduinoSerialLogSink sink(Serial);
pypilot_syslib::Logger logger(&sink);

void setup() {
    Serial.begin(9600);
    logger.info(1000ULL,
                pypilot_syslib::LogModule::Runtime,
                pypilot_syslib::LogEvent::SystemStartup,
                "syslib logging start");
}

void loop() {
}
