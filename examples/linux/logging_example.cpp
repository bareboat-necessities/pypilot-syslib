#include <pypilot_syslib.hpp>

int main() {
    pypilot_syslib::StdoutLogSink sink;
    pypilot_syslib::Logger logger(&sink);
    logger.info(1000ULL,
                pypilot_syslib::LogModule::Runtime,
                pypilot_syslib::LogEvent::SystemStartup,
                "start");
    return 0;
}
