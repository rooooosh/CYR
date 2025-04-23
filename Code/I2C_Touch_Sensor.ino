#include <Wire.h>
#include <Adafruit_MPR121.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

Adafruit_MPR121 cap = Adafruit_MPR121();

// Let op: DFPlayer TX → pin 11, RX ← pin 10
SoftwareSerial mySerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
bool dfPlayerOk = false;

uint16_t previousState = 0; // Houdt vorige aanraakstatus bij

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);

    if (!cap.begin(0x5B)) { // Controleer het I2C-adres (kan ook 0x5A of 0x5C zijn)
        Serial.println("MPR121 niet gevonden!");
        while (1);
    }
    Serial.println("MPR121 gestart 🤖");

    dfPlayerOk = myDFPlayer.begin(mySerial);
    if (!dfPlayerOk) {
        Serial.println("DFPlayer Mini niet gevonden!");
    } else {
        myDFPlayer.volume(25);  // Volume 0–30
        Serial.println("DFPlayer gestart 🤖");
    }
}

void loop() {
    uint16_t currentState = cap.touched(); // Lees de huidige status

    for (uint8_t i = 0; i < 12; i++) {
        bool previousTouch = (previousState & (1 << i));
        bool currentTouch = (currentState & (1 << i));

        if (currentTouch && !previousTouch) { // Alleen een nieuwe aanraking registreren als de toets nu wordt aangeraakt maar eerder niet
            if (isMetalTouched()) {
                Serial.print("TOUCH_");
                Serial.println(i);

                if (dfPlayerOk) {
                    myDFPlayer.play(i + 1);  // Speel bestand 0001.mp3 t/m 0012.mp3
                } else {
                    Serial.println("DFPlayer niet actief, geen geluid afgespeeld.");
                }
            }
        }
    }

    previousState = currentState; // Update vorige status
    //delay(100); // Optioneel: afvlakken van input
}

bool isMetalTouched() {
    delay(5);  // Stabilisatie
    uint16_t secondCheck = cap.touched();
    return secondCheck != 0;
}
