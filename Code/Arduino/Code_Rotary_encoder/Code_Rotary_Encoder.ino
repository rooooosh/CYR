/*
 * PIN AANSLUITINGEN
 * GND --> GND (zwart)
 *  +  --> 5V  (rood)
 * SW  --> 12  (geel)
 * DT  --> 3   (groen, data)
 * CLK --> 2   (blauw, clock)
 */
// Variabele om bij te houden of long press al is gedetecteerd
bool longPressGedetecteerd = false;
int aantalClicks = 2;

// Rotary encoder pinnen
const int pinA = 2;
const int pinB = 3;
volatile int pinAstateCurrent = LOW;
volatile int pinAStateLast = LOW;
int countLinks = 0;
int countRechts = 0;
int i = 1;

// Schakelaar
const int switchPin = 12;
int switchState = HIGH;
int vorigeSwitchState = HIGH;
unsigned long switchIngedruktTijd = 0;
const int longPressTijd = 1500;  // 1.5 seconden

// Debounce
bool debounceActief = false;
unsigned long debounceTimer = 0;
const int debounceTijd = 50;

// Toggle vlag
bool enkeleKlikActief = false;

void setup() {
  Serial.begin(115200);

  pinMode(switchPin, INPUT_PULLUP);
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);

  attachInterrupt(digitalPinToInterrupt(pinA), update, CHANGE);
}

void loop() {
  switchState = digitalRead(switchPin);
  unsigned long huidigeTijd = millis();

  // Debounce
  if (debounceActief && (huidigeTijd - debounceTimer > debounceTijd)) {
    debounceActief = false;
  }

  // Check op long press
  if (switchState == LOW && vorigeSwitchState == HIGH) {
    // Knop net ingedrukt
    switchIngedruktTijd = huidigeTijd;
    debounceActief = true;
    debounceTimer = huidigeTijd;
    longPressGedetecteerd = false;
  }

  // Detecteer long press terwijl knop ingedrukt blijft
  if (switchState == LOW && !longPressGedetecteerd && (huidigeTijd - switchIngedruktTijd >= longPressTijd)) {
    // Long press gedetecteerd
    enkeleKlikActief = !enkeleKlikActief;
    Serial.println("dubbelSchakelaarIngedrukt");
    longPressGedetecteerd = true;
  }

  if (switchState == HIGH && vorigeSwitchState == LOW) {
    // Knop net losgelaten
    if (!debounceActief && !longPressGedetecteerd && (huidigeTijd - switchIngedruktTijd < longPressTijd) && enkeleKlikActief) {
      // Enkele korte klik
      switch (i) {
        case 1: Serial.println("schakelaarIngedrukt1"); break;
        case 2: Serial.println("schakelaarIngedrukt2"); break;
        case 3: Serial.println("schakelaarIngedrukt3"); break;
        case 4: Serial.println("schakelaarIngedrukt4"); break;
        case 5: Serial.println("toggleHendel"); break;
      }
    }
  }
  vorigeSwitchState = switchState;
}

void update() {
  pinAstateCurrent = digitalRead(pinA);

  if ((pinAStateLast == LOW) && (pinAstateCurrent == HIGH)) {
    if (digitalRead(pinB) == HIGH) {
      countRechts++;
    } else {
      countLinks++;
    }

    if (countLinks >= aantalClicks) {
      if (enkeleKlikActief && (i > 1)) {
        i--;
        Serial.println("Links");
      }
      countLinks = 0;
      countRechts = 0;
    }

    if (countRechts >= aantalClicks) {
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