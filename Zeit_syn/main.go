package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"os/exec"
	"strings"
	"time"

	"github.com/tarm/serial"

)

func setWindowsTime(t time.Time) error {
	timeStr := t.Format("2006-01-02 15:04:05")
	fmt.Println("🕓 Setze Windows-Zeit via PowerShell:", timeStr)

	cmd := exec.Command("powershell", "-Command", fmt.Sprintf(`Set-Date -Date "%s"`, timeStr))
	output, err := cmd.CombinedOutput()
	if err != nil {
		return fmt.Errorf("❌ Fehler beim Setzen der Zeit: %v\n↪️ Ausgabe: %s", err, string(output))
	}

	fmt.Println("✅ Zeit erfolgreich gesetzt.")
	return nil
}

func main() {
	// COM-Port anpassen falls nötig
	c := &serial.Config{Name: "COM4", Baud: 115200}
	s, err := serial.OpenPort(c)
	if err != nil {
		log.Fatal("❌ Fehler beim Öffnen von COM4:", err)
	}
	defer s.Close()

	fmt.Println("📡 Warte auf Zeit-String vom ESP32...")

	reader := bufio.NewReader(s)

	for {
		line, err := reader.ReadString('\n')
		if err != nil {
			log.Println("❌ Fehler beim Lesen:", err)
			continue
		}

		line = strings.TrimSpace(line)

		if len(line) < len("2006-01-02 15:04:05") {
			log.Println("⚠️ Eingabe zu kurz, warte weiter:", line)
			continue
		}

		t, err := time.Parse("2006-01-02 15:04:05", line)
		if err != nil {
			log.Println("❌ Ungültiges Zeitformat:", err)
			continue
		}

		fmt.Println("📨 Gültige Zeit empfangen:", t)

		if err := setWindowsTime(t); err != nil {
			log.Fatal(err)
		}

		break // Nur einmal Zeit setzen, dann beenden
	}

	os.Exit(0)
}
