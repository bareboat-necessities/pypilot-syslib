#pragma once

#include <stdint.h>

namespace pypilot_syslib {

enum class LogLevel : uint8_t {
    Debug,
    Info,
    Warn,
    Error,
    Critical
};

enum class LogModule : uint8_t {
    Algorithms,
    DataModel,
    Sensors,
    PilotsLogic,
    GpsAdapter,
    SteeringSignaling,
    ServoProtocol,
    Nmea0183,
    SignalK,
    Runtime
};

enum class LogEvent : uint16_t {
    SystemStartup = 1,
    SystemShutdown = 2,
    ConfigurationLoaded = 3,
    ConfigurationInvalid = 4,
    SourceSelected = 5,
    SourceRejected = 6,
    SourceTimedOut = 7,
    SensorSampleRejected = 8,
    GpsFixAccepted = 9,
    GpsFixRejected = 10,
    GpsFilterReset = 11,
    GpsFilterPredictionReset = 12,
    WmmEvaluationInvalid = 13,
    PilotModeChanged = 14,
    PilotCommandComputed = 15,
    ApbNavCommandAccepted = 16,
    ApbNavCommandRateLimited = 17,
    RudderCalibrationInvalid = 18,
    ServoCommandBlocked = 19,
    ServoFeedbackFault = 20
};

struct LogRecord {
    uint64_t time_us;
    LogLevel level;
    LogModule module;
    LogEvent event;
    int32_t code;
    float value;
    const char* message;

    LogRecord()
        : time_us(0), level(LogLevel::Info), module(LogModule::Runtime),
          event(LogEvent::SystemStartup), code(0), value(0.0f), message(0) {}

    LogRecord(uint64_t time_value_us,
              LogLevel level_value,
              LogModule module_value,
              LogEvent event_value,
              const char* message_value,
              int32_t code_value = 0,
              float value_value = 0.0f)
        : time_us(time_value_us), level(level_value), module(module_value),
          event(event_value), code(code_value), value(value_value), message(message_value) {}
};

class LogSink {
public:
    virtual ~LogSink() {}
    virtual void write(const LogRecord& record) = 0;
};

class NullLogSink : public LogSink {
public:
    void write(const LogRecord&) override {}
};

inline uint8_t log_level_priority(LogLevel level) {
    return static_cast<uint8_t>(level);
}

inline bool log_level_allows(LogLevel record_level, LogLevel min_level) {
    return log_level_priority(record_level) >= log_level_priority(min_level);
}

class Logger {
public:
    Logger() : sink_(0), min_level_(LogLevel::Debug) {}
    explicit Logger(LogSink* sink) : sink_(sink), min_level_(LogLevel::Debug) {}
    Logger(LogSink* sink, LogLevel min_level) : sink_(sink), min_level_(min_level) {}

    void set_sink(LogSink* sink) { sink_ = sink; }
    LogSink* sink() const { return sink_; }
    bool has_sink() const { return sink_ != 0; }

    void set_min_level(LogLevel min_level) { min_level_ = min_level; }
    LogLevel min_level() const { return min_level_; }

    bool should_log(LogLevel level) const {
        return sink_ != 0 && log_level_allows(level, min_level_);
    }

    void log(uint64_t time_us,
             LogLevel level,
             LogModule module,
             LogEvent event,
             const char* message,
             int32_t code = 0,
             float value = 0.0f) {
        if (!should_log(level)) return;
        const LogRecord record(time_us, level, module, event, message, code, value);
        sink_->write(record);
    }

    void debug(uint64_t time_us, LogModule module, LogEvent event, const char* message,
               int32_t code = 0, float value = 0.0f) {
        log(time_us, LogLevel::Debug, module, event, message, code, value);
    }

    void info(uint64_t time_us, LogModule module, LogEvent event, const char* message,
              int32_t code = 0, float value = 0.0f) {
        log(time_us, LogLevel::Info, module, event, message, code, value);
    }

    void warn(uint64_t time_us, LogModule module, LogEvent event, const char* message,
              int32_t code = 0, float value = 0.0f) {
        log(time_us, LogLevel::Warn, module, event, message, code, value);
    }

    void error(uint64_t time_us, LogModule module, LogEvent event, const char* message,
               int32_t code = 0, float value = 0.0f) {
        log(time_us, LogLevel::Error, module, event, message, code, value);
    }

    void critical(uint64_t time_us, LogModule module, LogEvent event, const char* message,
                  int32_t code = 0, float value = 0.0f) {
        log(time_us, LogLevel::Critical, module, event, message, code, value);
    }

private:
    LogSink* sink_;
    LogLevel min_level_;
};

inline void log_if(Logger* logger,
                   uint64_t time_us,
                   LogLevel level,
                   LogModule module,
                   LogEvent event,
                   const char* message,
                   int32_t code = 0,
                   float value = 0.0f) {
    if (!logger) return;
    logger->log(time_us, level, module, event, message, code, value);
}

}  // namespace pypilot_syslib
