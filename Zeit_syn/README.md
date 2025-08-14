
# Von Copilot erstellte Anleitung für Zeitsync

## Projektübersicht
Dieses Projekt ist eine Go-Anwendung zur Synchronisierung der Windows-Systemzeit mit einem Zeitstempel, der von einem ESP32-Gerät über eine serielle Schnittstelle empfangen wird. Es ist als Kommandozeilentool unter Windows vorgesehen.

## Architektur & Schlüsseldateien
- `main.go`: Der Einstiegspunkt. Verarbeitet die serielle Kommunikation, analysiert eingehende Zeitstrings und stellt die Windows-Systemzeit mit PowerShell ein.
- `serial_windows.go`: Implementiert die Windows-spezifische Handhabung serieller Schnittstellen mithilfe von Low-Level-Win32-API-Aufrufen über das Go-Paket `syscall`. Kein eigenständiges Paket – muss denselben Paketnamen wie `main.go` verwenden (normalerweise `main`).

## Erstellen & Ausführen
- Alle Go-Dateien im Stammverzeichnis müssen denselben Paketnamen verwenden (normalerweise `main`).
– Erstellen mit: „go build -o Zeitsync.exe“ (vom Projektstamm)
– Ausführen mit: „Zeitsync.exe“
– Der serielle Port (z. B. „COM4“) und die Baudrate sind in „main.go“ fest codiert. Bei Bedarf ändern.

## Serielle Kommunikation
– Verwendet das Paket „github.com/tarm/serial“ für den High-Level-Zugriff auf den seriellen Port.
– Windows-spezifischer Low-Level-Zugriff: siehe „serial_windows.go“.
– Erwartet vom ESP32 eine Zeitangabe im Format „JJJJ-MM-TT HH:MM:SS“, gefolgt von einem Zeilenumbruch.

## Systemzeit einstellen
– Die Systemzeit wird mit einem PowerShell-Befehl aus Go („Set-Date“) eingestellt.
– Die Anwendung muss mit ausreichenden Berechtigungen zum Ändern der Systemzeit ausgeführt werden.

## Fehlerbehandlung und -protokollierung
- Fehler werden mit klaren, benutzerfreundlichen Meldungen (teilweise auf Deutsch) protokolliert.
- Die Anwendung wird nach erfolgreicher Zeiteinstellung beendet.

## Projektkonventionen
- Der gesamte Code befindet sich im Stammverzeichnis; keine Unterpakete.
- Verwenden Sie „package main“ für alle Dateien, außer bei Refactoring für die Bibliotheksnutzung.
- Kommentare und Protokollmeldungen können auf Deutsch sein.

## Externe Abhängigkeiten
- „github.com/tarm/serial“ (Installation mit „go get github.com/tarm/serial“)

## Beispiel-Workflow
1. Verbinden Sie den ESP32 mit dem PC und stellen Sie sicher, dass er den korrekten Zeitstring sendet.
2. Passen Sie den COM-Port in „main.go“ bei Bedarf an.
3. Erstellen und starten Sie die Anwendung als Administrator.
4. Die App wartet auf einen Zeitstring, stellt die Systemzeit ein und beendet sich dann.

---

Wenn Sie neue Funktionen hinzufügen oder das serielle Protokoll ändern, aktualisieren Sie diese Datei mit neuen Konventionen und Arbeitsabläufen.
