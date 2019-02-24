# geiger_mkrw1300
Geiger-Zähler mit Arduino MKRWAN 1300  (nach Bernd Laquai)

In der Anleitung ["GeigerZähler und LoRaWAN Radioaktivitäts-Messstation auf Basis der Arduino Maker-Familie"](http://opengeiger.de/MakerGeigerTTN.pdf) von Bernd Laquai wird beschrieben, wie ein Geigerzähler mit dem MKRW1300 aufgebaut werden kann. Die passende Software ist ebenfalls über diesen Link zu finden sowie die Anleitung zum Einbinden des Gerätes in das TTN-Netz.

Ich habe die Original-Software von Bernd so angepasst, dass ein einfaches kleines Display angesteuert werden kann.

## Display
Das Display ist ein SSD1306 mit 128x64 Pixeln. Angesteuert wird  es über die I2C-Schnittstelle des MKRW1300.

Angezeigt wird im Betrieb der Wert **Counts per Minute** (cpm) in großen Zahlen.  
Unten links wird jeder Sekunde der momentane Zählerwert angezeigt. Dieser Wert steigt bis auf 100, daraus wird dann der cpm-Wert berechnet und der Zähler wieder zurück gesetzt.

Nach den Restart wird auf der Anzeige die DEVEUI für das LoRa angezeigt. Außerdem wird, solange eine Verbindung mit dem LoRa aufgebaut wird, **Joining..** angezeigt. Wenn die Verbindung steht, wird kurz **Joined !** angezeigt. Kann keine Verbindung hergestellt werden, so wird **No LoRa** angezeigt. Dann wird nach 2 sec das Display gelöscht und auf Zählimpulse gewartet.

## Ausgang
Zusätzlich zu der Display-Anzeige wird über Ausgang 6 (der auch an der internen gelben LED angeschlossen ist), ein 10msec breiter positiver Impuls bei jedem Zählimpuls ausgegeben. Er kann verwendet werden um entweder eine LED oder einen Piezo-Buzzer anzusteuern.

## LoRa-TTN-Keys
Die beiden Keys, die für den Zugang zu TTN nötig sind, werden in der extra Datei **appkeys.h** hinterlegt. Diese Datei ist **nicht** hier im GitHub, sie muss individuell erstellt werden.  
Sie hat folgendes Aussehen:
```
String appEui = "0011223344556677";
String appKey = "0011223344556677889900AABBCCDDEEFF";
```
Wobei natürlich die 001122...  durch die eigenen Werte zu ersetzen sind.

## Übersetzen
Die Software ist für **PlatfomrmIO** eingerichtet. D.h. nach dem Clonen aus GitHub kann das Projekt in PlaformIO eingelesen (Open Project), dann übersetzt und in den MKRWAN1300 programmiert werden. 

Aber auch mit der **Arduino-IDE** kann das Projekt übersetzt werden. Dazu einfach die Datei *LoraGeigerTTN_MKRWAN.ino* im Verzeichnis *LoraGeigerTTN_MKRWAN* mit der Arduino-IDE öffnen.
