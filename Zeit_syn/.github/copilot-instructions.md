# Copilot Instructions for Zeitsync

## Project Overview
This project is a Go application for synchronizing the Windows system time with a timestamp received from an ESP32 device over a serial port. It is intended to be run as a command-line tool on Windows.

## Architecture & Key Files
- `main.go`: The entry point. Handles serial communication, parses incoming time strings, and sets the Windows system time using PowerShell.
- `serial_windows.go`: Implements Windows-specific serial port handling using low-level Win32 API calls via Go's `syscall` package. Not a standalone package—must use the same package name as `main.go` (usually `main`).

## Build & Run
- All Go files in the root directory must use the same package name (typically `main`).
- Build with: `go build -o Zeitsync.exe` (from the project root)
- Run with: `Zeitsync.exe`
- The serial port (e.g., `COM4`) and baud rate are hardcoded in `main.go`. Change as needed.

## Serial Communication
- Uses the `github.com/tarm/serial` package for high-level serial port access.
- For Windows-specific low-level access, see `serial_windows.go`.
- Expects the ESP32 to send a time string in the format `YYYY-MM-DD HH:MM:SS` followed by a newline.

## Setting System Time
- The system time is set using a PowerShell command invoked from Go (`Set-Date`).
- Requires the application to be run with sufficient privileges to change system time.

## Error Handling & Logging
- Errors are logged with clear, user-friendly messages (some in German).
- The application exits after successfully setting the time.

## Project Conventions
- All code is in the root directory; no sub-packages.
- Use `package main` for all files unless refactoring for library use.
- Comments and log messages may be in German.

## External Dependencies
- `github.com/tarm/serial` (install with `go get github.com/tarm/serial`)

## Example Workflow
1. Connect ESP32 to PC and ensure it sends the correct time string.
2. Adjust the COM port in `main.go` if needed.
3. Build and run the application as administrator.
4. The app waits for a time string, sets the system time, then exits.

---

If you add new features or change the serial protocol, update this file with new conventions and workflows.
