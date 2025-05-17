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

  uint8_t activeChannels[] = {0, 1, 2, 3, 4, 5};
  for (uint8_t j = 0; j < sizeof(activeChannels) / sizeof(activeChannels[0]); j++) {
    uint8_t i = activeChannels[j];

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
