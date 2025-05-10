# Eindopdracht Emerging Technologies

- Arduino scripts kan je vinden in [Code/Arduino](./Code/Arduino/)
- Python scripts kan je vinden in [Code/Python](./Code/Python/)

Het project bestaat uit twee onderdelen met afzonderlijke elektronische componenten. Het eerste onderdeel, de boombox, bevat een scherm, een Raspberry Pi 4 en een rotary encoder, gekoppeld aan een Arduino Nano. Doel van dit onderdeel is het weergeven van een spel op het scherm, bestuurd via de input van de rotary encoder. Het script voor deze toepassing is geschreven in Python, met gebruik van de Pygame-bibliotheek.

Het tweede onderdeel, de touchbox, omvat een Arduino Nano, een I2C capacitieve touchsensor, een DF Player en een speaker met versterker. Dit systeem speelt een specifiek geluid af bij aanraking. Deze geluiden worden dienen afgespeeld te worden wanneer het bijbehorende icoon verschijnt op het scherm van de boombox.

Algemeene connectieschemas Momenteel:
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

