# Eindopdracht Emerging Technologies

- Arduino scripts kan je vinden in [Code/Arduino](./Code/Arduino/)
- Python scripts kan je vinden in [Code/Python](./Code/Python/)

Het project bestaat uit twee onderdelen met afzonderlijke elektronische componenten. Het eerste onderdeel, de boombox, bevat een scherm, een Raspberry Pi 4 en een rotary encoder, gekoppeld aan een Arduino Nano. Doel van dit onderdeel is het weergeven van een spel op het scherm, bestuurd via de input van de rotary encoder. Het script voor deze toepassing is geschreven in Python, met gebruik van de Pygame-bibliotheek.

Het tweede onderdeel, de touchbox, omvat een Arduino Nano, een I2C capacitieve touchsensor, een DF Player en een speaker met versterker. Dit systeem speelt een specifiek geluid af bij aanraking. Deze geluiden worden dienen afgespeeld te worden wanneer het bijbehorende icoon verschijnt op het scherm van de boombox.

Om een idee te krijgen van de opbouw van dit project te krijgen, worden deze snel voorgesteld op volgende schema's:
<p align="left">
  <img src="CYR PICS/Schema Boombox 1.png" width="33%">
  <img src="CYR PICS/Schema Touchbox 1.png" width="33%">

## Opbouw Boombox
Volgend schema toont aan hoe de Arduino Nano verbonden is aan de rotary encoder:
<p align="left">
  <img src="CYR PICS/Schema Boombox 2.png" width="70%">

Deze Arduino is via USB-A verbonden aan de raspberry Pi 4. De raspberry pi 4 is verbonden aan het scherm volgens volgend schema:

<p align="left">
  <img src="CYR PICS/Connectieschema_Scherm-Rasppi4.jpg" width="70%">



## Opbouw Touchbox
Volgend schema geeft weer hoe de technische componenten van de touchbox met elkaar verbonden zijn.
<p align="left">
  <img src="CYR PICS/Connectieschema_Touchbox.jpg" width="70%">
