# Eindopdracht Emerging Technologies

- Arduino-scripts zijn te vinden in [Code/Arduino](./Code/Arduino/)
- Python-scripts zijn te vinden in [Code/Python](./Code/Python/)
Het project bestaat uit twee onderdelen met afzonderlijke elektronische componenten. Het eerste onderdeel, de boombox, bevat een Raspberry Pi 7" DSI Touchscreen display (800 x 480 px), een Raspberry Pi 4 en een rotary encoder, gekoppeld aan een Arduino Nano. Dit onderdeel is bedoeld voor het weergeven van een spel op het scherm, bestuurd via de input van de rotary encoder. Het script voor deze toepassing is geschreven in Python, met gebruik van de Pygame-bibliotheek.

Het tweede onderdeel, de touchbox, omvat een Arduino Nano, een I2C-capacitieve touchsensor, een DF Player en een speaker met versterker. Dit systeem speelt een specifiek geluid af bij aanraking. Geluiden worden afgespeeld wanneer het bijbehorende icoon verschijnt op het scherm van de boombox.

Om een idee te krijgen van de opbouw van het project, worden deze snel voorgesteld op onderstaande schema's:
<p align="left">
  <img src="CYR PICS/Schema Boombox 1.png" width="33%">
  <img src="CYR PICS/Schema Touchbox 1.png" width="33%">

## Opbouw Boombox
### Rotary Encoder
Onderstaand schema toont hoe de Arduino Nano verbonden is aan de rotary encoder.
<p align="left">
  <img src="CYR PICS/Schema Boombox 2.png" width="70%">

#### Verificatie Input
Om te verifiëren of de rotary encoder nog volledig werkt en het juiste signaal doorgeeft, wordt het signaal geprint. Om dit te testen is het eenvoudgste om in Arduino IDE de seriële monitor te openen en te verifiëren of alle inputs correct worden doorgegeven.

### Raspberry pi 4 + Scherm
Deze Arduino is via USB-A verbonden aan de Raspberry Pi 4. De Raspberry Pi 4 is verbonden aan het scherm volgens onderstaand schema:


<p align="left">
  <img src="CYR PICS/Connectieschema_Scherm-Rasppi4.jpg" width="70%">

#### Verificatie Output
Bij het opstarten van de Raspberry Pi geeft de ingebouwde LED aan of het systeem functioneel is. Het scherm start automatisch op en toont de interface van de Raspberry Pi. Voor verificatie van de correcte werking van de code en het uitlezen van signalen van de Arduino wordt het ontvangen signaal steeds weergegeven in de seriële monitor.


## Opbouw Touchbox
Onderstaand schema geeft weer hoe de technische componenten van de touchbox met elkaar verbonden zijn.
<p align="left">
  <img src="CYR PICS/Connectieschema_Touchbox.png" width="70%">

#### Verficatie Input
Om te verzekeren dat de DF Player en de touchsensor correct opstarten, bevat de code een sectie die aangeeft of deze componenten correct zijn gedetecteerd en actief zijn. Deze aanpak biedt waardevolle ondersteuning bij het debuggen van het systeem.

#### Verificatie Output
Wanneer een signaal van de touchsensor wordt ontvangen, wordt er een geluid afgespeeld via de speaker. Dit dient als auditieve feedback om te bevestigen dat het systeem correct functioneert.
