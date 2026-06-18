#pragma once

#include "logging.hpp"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <iostream>
#include <ostream>
#endif

namespace pypilot_syslib {

inline const char* log_level_name(LogLevel level) {
    switch (level) {
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info: return "INFO";
        case LogLevel::Warn: return "WARN";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

inline const char* log_module_name(LogModule module) {
    switch (module) {
        case LogModule::Algorithms: return "algorithms";
        case LogModule::DataModel: return "data-model";
        case LogModule::Sensors: return "sensors";
        case LogModule::PilotsLogic: return "pilots-logic";
        case LogModule::GpsAdapter: return "gps-adapter";
        case LogModule::SteeringSignaling: return "steering-signaling";
        case LogModule::ServoProtocol: return "servo-protocol";
        case LogModule::Nmea0183: return "nmea0183";
        case LogModule::SignalK: return "signalk";
        case LogModule::Runtime: return "runtime";
        default: return "unknown";
    }
}

inline const char* log_event_name(LogEvent event) {
    switch (event) {
        case LogEvent::SystemStartup: return "SystemStartup";
        case LogEvent::SystemShutdown: return "SystemShutdown";
        case LogEvent::ConfigurationLoaded: return "ConfigurationLoaded";
        case LogEvent::ConfigurationInvalid: return "ConfigurationInvalid";
        case LogEvent::SourceSelected: return "SourceSelected";
        case LogEvent::SourceRejected: return "SourceRejected";
        case LogEvent::SourceTimedOut: return "SourceTimedOut";
        case LogEvent::SensorSampleRejected: return "SensorSampleRejected";
        case LogEvent::GpsFixAccepted: return "GpsFixAccepted";
        case LogEvent::GpsFixRejected: return "GpsFixRejected";
        case LogEvent::GpsFilterReset: return "GpsFilterReset";
        case LogEvent::GpsFilterPredictionReset: return "GpsFilterPredictionReset";
        case LogEvent::WmmEvaluationInvalid: return "WmmEvaluationInvalid";
        case LogEvent::PilotModeChanged: return "PilotModeChanged";
        case LogEvent::PilotCommandComputed: return "PilotCommandComputed";
        case LogEvent::ApbNavCommandAccepted: return "ApbNavCommandAccepted";
        case LogEvent::ApbNavCommandRateLimited: return "ApbNavCommandRateLimited";
        case LogEvent::RudderCalibrationInvalid: return "RudderCalibrationInvalid";
        case LogEvent::ServoCommandBlocked: return "ServoCommandBlocked";
        case LogEvent::ServoFeedbackFault: return "ServoFeedbackFault";
        default: return "UnknownEvent";
    }
}

#if defined(ARDUINO)
class ArduinoSerialLogSink : public LogSink {
public:
    explicit ArduinoSerialLogSink(Print& out) : out_(&out) {}

    void write(const LogRecord& record) override {
        out_->print(static_cast<unsigned long>(record.time_us));
        out_->print(' ');
        out_->print(log_level_name(record.level));
        out_->print(' ');
        out_->print(log_module_name(record.module));
        out_->print(' ');
        out_->print(log_event_name(record.event));
        out_->print(' ');
        if (record.message) out_->print(record.message);
        out_->print(" code=");
        out_->print(record.code);
        out_->print(" value=");
        out_->println(record.value);
    }

private:
    Print* out_;
};
#else
class OstreamLogSink : public LogSink {
public:
    explicit OstreamLogSink(std::ostream& out) : out_(&out) {}

    void write(const LogRecord& record) override {
        (*out_) << record.time_us << ' '
                << log_level_name(record.level) << ' '
                << log_module_name(record.module) << ' '
                << log_event_name(record.event) << ' '
                << (record.message ? record.message : "")
                << " code=" << record.code
                << " value=" << record.value
                << '\n';
    }

private:
    std::ostream* out_;
};

class StdoutLogSink : public OstreamLogSink {
public:
    StdoutLogSink() : OstreamLogSink(std::cout) {}
};

class StderrLogSink : public OstreamLogSink {
public:
    StderrLogSink() : OstreamLogSink(std::cerr) {}
};
#endif

}  // namespace pypilot_syslib
