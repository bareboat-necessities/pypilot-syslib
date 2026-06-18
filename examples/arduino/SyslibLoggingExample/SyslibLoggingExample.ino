#include <Arduino.h>
#include <pypilot_syslib.hpp>

class SerialLogSink : public pypilot_syslib::LogSink {
public:
    void write(const pypilot_syslib::LogRecord& record) override {
        Serial.println(record.message ? record.message : "");
    }
};

SerialLogSink sink;
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
