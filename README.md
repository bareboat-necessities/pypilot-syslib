# pypilot-syslib

Portable system abstractions shared by the modular pypilot C++ projects.

Initial scope:

- header-only logging API
- Arduino-safe data structures
- Linux and Arduino examples
- optional logger/sink integration

Future scope:

- monotonic clock abstraction
- platform diagnostics
- common status and event helpers

## Logging boundary

This library defines log levels, modules, event IDs, records, sinks, and a small logger facade. Individual projects decide where to call it, but they should use the same event IDs and record format.
