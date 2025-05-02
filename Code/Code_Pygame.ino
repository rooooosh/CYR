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

