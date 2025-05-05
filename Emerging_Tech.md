# CYR-Code

Code Touchbox:

```C++
#include <Wire.h>
#include <Adafruit_MPR121.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

Adafruit_MPR121 cap = Adafruit_MPR121();

// DFPlayer TX → pin 11, RX ← pin 10
SoftwareSerial mySerial(10, 11);
DFRobotDFPlayerMini myDFPlayer;
bool dfPlayerOk = false;

uint16_t previousState = 0;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println("Start systeem... ⚙️");

  if (!cap.begin(0x5B)) {
    Serial.println("MPR121 niet gevonden op 0x5B. Controleer adres.");
    while (1);
  }
  Serial.println("TOUCH SENSOR Online");

  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer niet gevonden! Check bedrading, GND & 5V.");
  } else {
    dfPlayerOk = true;
    Serial.println("MP3 Online");
    myDFPlayer.volume(30); // 0–30
  }
}

void loop() {
  uint16_t currentState = cap.touched();

  for (uint8_t i = 0; i < 7; i++) {
    bool previousTouch = (previousState & (1 << i));
    bool currentTouch = (currentState & (1 << i));

    if (currentTouch && !previousTouch) {
      if (isMetalTouched()) {
        Serial.print("TOUCH_");
        Serial.println(i);

        if (dfPlayerOk) {
          myDFPlayer.play(i + 1);  // Speel 0001.mp3 t/m 0012.mp3
        } else {
          Serial.println("404 systeem fout");
        }
      }
    }
  }

  previousState = currentState;
}

bool isMetalTouched() {
  delay(5);
  uint16_t secondCheck = cap.touched();
  return secondCheck != 0;
}
```


Code Boombox:
Geschreven voor Arduino IDE
```C++
/*
 * PIN AANSLUITINGEN
 * GND --> GND (zwart)
 *  +  --> 5V  (rood)
 * SW  --> 12  (geel)
 * DT  --> 3   (groen, data)
 * CLK --> 2   (blauw, clock)
 */

int aantalClicks = 3;
int i = 1;

// Rotary encoder pinnen
const int pinA = 2;
const int pinB = 3;
volatile int pinAstateCurrent = LOW;
volatile int pinAStateLast = LOW;
int countLinks = 0;
int countRechts = 0;

// Schakelaar
const int switchPin = 12;
int switchState = HIGH;

// Double-click detectie variabelen
unsigned long lastClickTime = 0;
bool wachtOpTweedeKlik = false;
const int doubleClickTijd = 250;  // Strengere double-click tijd (ms)
bool debounceActief = false;
unsigned long debounceTimer = 0;
const int debounceTijd = 50;

// Toggle vlag: als deze true is, worden schakelaarIngedrukt én encoder (Links/Rechts) geactiveerd
bool enkeleKlikActief = false;

void setup() {
  Serial.begin(115200);

  pinMode(switchPin, INPUT_PULLUP);
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);

  // Interrupt aan encoder PinA
  attachInterrupt(digitalPinToInterrupt(pinA), update, CHANGE);
}

void loop() {
  switchState = digitalRead(switchPin);
  unsigned long huidigeTijd = millis();

  // Schakelaar-debounce
  if (debounceActief && (huidigeTijd - debounceTimer > debounceTijd)) {
    debounceActief = false;
  }

  if (switchState == LOW && !debounceActief) {
    debounceActief = true;
    debounceTimer = huidigeTijd;

    if (!wachtOpTweedeKlik) {
      // Eerste klik: start timer
      wachtOpTweedeKlik = true;
      lastClickTime = huidigeTijd;
    } else {
      // Tweede klik binnen doubleClickTijd → toggle modus
      if (huidigeTijd - lastClickTime <= doubleClickTijd) {
        // Toggle de actieve modus; als deze true is, worden encoder- en schakelaarhandelingen uitgevoerd
        enkeleKlikActief = !enkeleKlikActief;
        Serial.println("dubbelSchakelaarIngedrukt");
        wachtOpTweedeKlik = false;
      }
    }
    // Wacht tot de schakelaar weer losgelaten is
    while (digitalRead(switchPin) == LOW)
      ;
  }

  // Als er een enkele klik is gedetecteerd en de doubleclick tijd verstreken is
  if (wachtOpTweedeKlik && (huidigeTijd - lastClickTime > doubleClickTijd)) {
    // Voer de actie alleen uit als de modus actief is
    if (enkeleKlikActief) {
      switch (i) {
        case 1: Serial.println("schakelaarIngedrukt1"); break;
        case 2: Serial.println("schakelaarIngedrukt2"); break;
        case 3: Serial.println("schakelaarIngedrukt3"); break;
        case 4: Serial.println("schakelaarIngedrukt4"); break;
      }
    }
    wachtOpTweedeKlik = false;
  }
}

void update() {
  pinAstateCurrent = digitalRead(pinA);

  if ((pinAStateLast == LOW) && (pinAstateCurrent == HIGH)) {
    if (digitalRead(pinB) == HIGH) {
      countLinks++;
    } else {
      countRechts++;
    }

    // Als voldoende linker pulsen geteld zijn
    if (countLinks >= aantalClicks) {
      // Alleen verwerken als toggle actief is en er binnen de grenzen gegaan kan worden
      if (enkeleKlikActief && (i > 1)) {
        i--;
        Serial.println("Links");
      }
      countLinks = 0;
      countRechts = 0;
    }

    // Als voldoende rechter pulsen geteld zijn
    if (countRechts >= aantalClicks) {
      // Alleen verwerken als toggle actief is en er binnen de grenzen gegaan kan worden
      if (enkeleKlikActief && (i < 4)) {
        i++;
        Serial.println("Rechts");
      }
      countLinks = 0;
      countRechts = 0;
    }
  }

  pinAStateLast = pinAstateCurrent;
}
```

Connectieschema Momenteel:
<p align="left">
  <img src="CYR PICS/Schema Boombox 1.png" width="33%">
  <img src="CYR PICS/Schema Touchbox 1.png" width="33%">


Nadeel:
-     Laptop vereist
-     Wifi vereist
 
<p align="left">
  <img src="CYR PICS/Schema Boombox 2.png" width="50%">


  Nieuwe code: python pygame, 
  geen laptop en wifi vereist -> Arduino verbonden aan raspberry pi via USB en uitgelezen in seriële monitor

```python
import pygame
import serial
import threading
import time
import sys

# --- Seriële poort instellen ---
seriele_poort = '/dev/ttyUSB0'
baudrate = 115200

try:
    ser = serial.Serial(seriele_poort, baudrate, timeout=1)
except serial.serialutil.SerialException:
    print(f"Kan seriële poort {seriele_poort} niet openen.")
    ser = None

# --- Pygame setup ---
pygame.init()
breedte, hoogte = 800, 480
scherm = pygame.display.set_mode((breedte, hoogte))
pygame.display.set_caption("CYR")
klok = pygame.time.Clock()

try:
    sticks_afbeelding = pygame.image.load("sticks.png").convert_alpha()
except pygame.error as e:
    print(f"Fout bij laden afbeelding: {e}")
    pygame.quit()
    exit()
# Kleuren
GRIJS = (200, 200, 200)
DONKERGRIJS = (80, 80, 80)
ZWART = (0, 0, 0)

# Spelstatus
spel_gestart = False
starttijd = 0

# Vierkanten instellingen en verschalen afbeelding
vierkant_grootte = 120
snelheid = 5  # pixels per frame
noot_afbeelding = pygame.transform.scale(sticks_afbeelding, (vierkant_grootte, vierkant_grootte))

# Stroken
strook_hoogte = 20
ruimte_tussen_stroken = 160
strook_y_posities = list(range(60, hoogte, ruimte_tussen_stroken))

# Vierkanten
vierkanten = []

# Ritme (tijd in ms en index van strook)
ritme = [
    {"tijd": 1000, "strook": 0},
    {"tijd": 2000, "strook": 1},
    {"tijd": 3000, "strook": 2},
    {"tijd": 4000, "strook": 0},
    {"tijd": 5000, "strook": 1},
    {"tijd": 6000, "strook": 2},
    {"tijd": 7000, "strook": 0},
    {"tijd": 8000, "strook": 0},
]

huidige_noot_index = 0

# Signaal uit seriële input
start_signaal = False

# --- Seriële data lezen ---
def lees_serial():
    global start_signaal
    if ser is None:
        return
    while True:
        if ser.in_waiting > 0:
            lijn = ser.readline().decode('utf-8').strip()
            if lijn in ["SchakelaarIngedrukt1", "Start", "Begin"]:
                start_signaal = True

# Start seriële thread
if ser is not None:
    serial_thread = threading.Thread(target=lees_serial, daemon=True)
    serial_thread.start()

# --- Achtergrond tekenen ---
def teken_achtergrond():
    scherm.fill(GRIJS)
    rechter_vlak_breedte = 200
    rechterscherm_balk = pygame.Surface((rechter_vlak_breedte, hoogte), pygame.SRCALPHA)
    rechterscherm_balk.fill((150, 150, 150, 128))
    scherm.blit(rechterscherm_balk, (breedte - rechter_vlak_breedte, 0))

    for y in strook_y_posities:
        pygame.draw.rect(scherm, DONKERGRIJS, (0, y, breedte, strook_hoogte))

# --- Vierkanten tekenen ---
def teken_vierkanten():
    for vierkant in vierkanten:
        scherm.blit(noot_afbeelding, (vierkant['x'], vierkant['y']))


# --- Main loop ---
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                start_signaal = True

    if start_signaal and not spel_gestart:
        spel_gestart = True
        starttijd = pygame.time.get_ticks()

    if spel_gestart:
        huidige_tijd = pygame.time.get_ticks() - starttijd

        # Check of het tijd is om een nieuwe noot te genereren
        while huidige_noot_index < len(ritme) and huidige_tijd >= ritme[huidige_noot_index]["tijd"]:
            strook_index = ritme[huidige_noot_index]["strook"]
            y = strook_y_posities[strook_index] + (strook_hoogte - vierkant_grootte) // 2
            vierkanten.append({'x': -vierkant_grootte, 'y': y})
            huidige_noot_index += 1

        # Verplaats de vierkanten
        for vierkant in vierkanten:
            vierkant['x'] += snelheid
        vierkanten = [v for v in vierkanten if v['x'] < breedte]

    teken_achtergrond()
    if spel_gestart:
        teken_vierkanten()

    pygame.display.flip()
    klok.tick(60)

pygame.quit()
 ```
