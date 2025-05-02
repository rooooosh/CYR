/*
 * PIN AANSLUITINGEN
 * GND --> GND (zwart)
 *  +  --> 5V  (rood)
 * SW  --> 12  (geel)
 * DT  --> 3   (groen, data)
 * CLK --> 2   (blauw, clock)
 */

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
