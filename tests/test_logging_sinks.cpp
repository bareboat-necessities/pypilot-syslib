#include <cassert>
#include <sstream>
#include <string>
#include <pypilot_syslib.hpp>

using namespace pypilot_syslib;

int main() {
    assert(std::string(log_level_name(LogLevel::Warn)) == "WARN");
    assert(std::string(log_module_name(LogModule::SteeringSignaling)) == "steering-signaling");
    assert(std::string(log_event_name(LogEvent::ServoCommandBlocked)) == "ServoCommandBlocked");

    std::ostringstream out;
    OstreamLogSink sink(out);
    Logger logger(&sink);
    logger.warn(12345ULL,
                LogModule::SteeringSignaling,
                LogEvent::ServoCommandBlocked,
                "blocked servo command",
                19,
                0.25f);

    const std::string line = out.str();
    assert(line.find("12345") != std::string::npos);
    assert(line.find("WARN") != std::string::npos);
    assert(line.find("steering-signaling") != std::string::npos);
    assert(line.find("ServoCommandBlocked") != std::string::npos);
    assert(line.find("blocked servo command") != std::string::npos);
    assert(line.find("code=19") != std::string::npos);
    assert(line.find("value=0.25") != std::string::npos);

    StdoutLogSink stdout_sink;
    Logger stdout_logger(&stdout_sink, LogLevel::Critical);
    stdout_logger.info(1ULL, LogModule::Runtime, LogEvent::SystemStartup, "filtered");

    StderrLogSink stderr_sink;
    Logger stderr_logger(&stderr_sink, LogLevel::Critical);
    stderr_logger.critical(2ULL, LogModule::Runtime, LogEvent::SystemShutdown, "critical shutdown");

    return 0;
}
