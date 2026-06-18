#include <pypilot_syslib.hpp>

class CountSink : public pypilot_syslib::LogSink {
public:
    CountSink() : count(0) {}
    void write(const pypilot_syslib::LogRecord&) override { ++count; }
    int count;
};

int main() {
    CountSink sink;
    pypilot_syslib::Logger logger(&sink);
    logger.info(1000ULL,
                pypilot_syslib::LogModule::Runtime,
                pypilot_syslib::LogEvent::SystemStartup,
                "start");
    return sink.count == 1 ? 0 : 1;
}
