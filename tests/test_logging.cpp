#include <cassert>
#include <pypilot_syslib.hpp>

using namespace pypilot_syslib;

class CaptureSink : public LogSink {
public:
    CaptureSink() : count(0), last() {}

    void write(const LogRecord& record) override {
        ++count;
        last = record;
    }

    int count;
    LogRecord last;
};

int main() {
    Logger null_logger;
    assert(!null_logger.has_sink());
    null_logger.info(1000ULL, LogModule::Runtime, LogEvent::SystemStartup, "ignored");

    CaptureSink sink;
    Logger logger(&sink);
    assert(logger.has_sink());
    assert(logger.min_level() == LogLevel::Debug);

    logger.info(2000ULL,
                LogModule::Sensors,
                LogEvent::SourceSelected,
                "source selected",
                7,
                3.5f);
    assert(sink.count == 1);
    assert(sink.last.time_us == 2000ULL);
    assert(sink.last.level == LogLevel::Info);
    assert(sink.last.module == LogModule::Sensors);
    assert(sink.last.event == LogEvent::SourceSelected);
    assert(sink.last.code == 7);
    assert(sink.last.value > 3.49f && sink.last.value < 3.51f);
    assert(sink.last.message != 0);

    logger.set_min_level(LogLevel::Warn);
    logger.info(3000ULL, LogModule::Runtime, LogEvent::ConfigurationLoaded, "filtered");
    assert(sink.count == 1);

    logger.warn(4000ULL, LogModule::Runtime, LogEvent::ConfigurationInvalid, "warned", -2);
    assert(sink.count == 2);
    assert(sink.last.level == LogLevel::Warn);
    assert(sink.last.event == LogEvent::ConfigurationInvalid);
    assert(sink.last.code == -2);

    log_if(&logger, 5000ULL, LogLevel::Error, LogModule::GpsAdapter,
           LogEvent::GpsFixRejected, "bad gps", 12, 1.0f);
    assert(sink.count == 3);
    assert(sink.last.level == LogLevel::Error);
    assert(sink.last.module == LogModule::GpsAdapter);
    assert(sink.last.event == LogEvent::GpsFixRejected);

    log_if(0, 6000ULL, LogLevel::Critical, LogModule::Runtime,
           LogEvent::SystemShutdown, "ignored null logger");
    assert(sink.count == 3);

    NullLogSink null_sink;
    Logger quiet(&null_sink);
    quiet.critical(7000ULL, LogModule::Runtime, LogEvent::SystemShutdown, "null sink accepts call");

    return 0;
}
