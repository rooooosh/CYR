import pygame
import serial
import threading
import time
import sys

# --- SeriÃ«le poort instellen ---
seriele_poort = '/dev/ttyUSB0'
baudrate = 115200

try:
    ser = serial.Serial(seriele_poort, baudrate, timeout=1)
except serial.serialutil.SerialException:
    print(f"Kan seriÃ«le poort {seriele_poort} niet openen.")
    ser = None

# Vierkanten instellingen
vierkant_grootte = 120
snelheid = 5

# --- Pygame setup ---
pygame.init()
breedte, hoogte = 800, 480
scherm = pygame.display.set_mode((0, 0), pygame.FULLSCREEN)
pygame.display.set_caption("CYR")
klok = pygame.time.Clock()

# Afbeeldingen laden
def laad_afbeeldingen():
    try:
        return {
            "sticks": pygame.transform.scale(pygame.image.load("sticks.png").convert_alpha(), (vierkant_grootte, vierkant_grootte)),
            "hi-hat": pygame.transform.scale(pygame.image.load("hi-hat.png").convert_alpha(), (vierkant_grootte, vierkant_grootte)),
            "Snaredrum": pygame.transform.scale(pygame.image.load("Snaredrum.png").convert_alpha(), (vierkant_grootte, vierkant_grootte)),
            "2 Snaredrum": pygame.transform.scale(pygame.image.load("2 Snaredrum.png").convert_alpha(), (vierkant_grootte, vierkant_grootte))
        }
    except pygame.error as e:
        print(f"Fout bij laden afbeelding: {e}")
        pygame.quit()
        exit()

afbeeldingen = laad_afbeeldingen()

# Kleuren
GRIJS = (200, 200, 200)
DONKERGRIJS = (80, 80, 80)
ZWART = (0, 0, 0)
WIT = (255, 255, 255)
green = (0, 255, 0)
black = (0, 0, 0)


# Spelstatus
spel_gestart = False
starttijd = 0

# Stroken
strook_hoogte = 20
ruimte_tussen_stroken = 160
strook_y_posities = list(range(60, hoogte, ruimte_tussen_stroken))

# Vierkanten
vierkanten = []

#Switch instellingen
x = 200  # X-positie van de switch
y = 300  # Y-positie van de switch

# Ritme (tijd in ms, index van strook, en afbeelding)
ritme = []
huidige_noot_index = 0
start_signaal = False

# Menu instellingen
menu_actief = False
menu_opties = [
    "Have You Ever Seen The Rain\n- Creedence Clerawater",
    "Angels\n- Robbie Williams",
    "Come Together\n- The Beatles",
    "Iris\n- Goo Goo Dolls"
]
geselecteerde_index = 0

# --- SeriÃ«le data lezen ---
def lees_serial():
    global start_signaal, menu_actief, geselecteerde_index, ritme
    if ser is None:
        return
    while True:
        if ser.in_waiting > 0:
            lijn = ser.readline().decode('utf-8').strip()
            print(f"SeriÃ«le input: {lijn}")
            if lijn == "dubbelSchakelaarIngedrukt":
                menu_actief = not menu_actief
            elif menu_actief:
                if lijn == "Rechts":
                    geselecteerde_index = min(len(menu_opties) - 1, geselecteerde_index + 1)
                elif lijn == "Links":
                    geselecteerde_index = max(0, geselecteerde_index - 1)
                elif lijn == "schakelaarIngedrukt1":
                    menu_actief = False
                    speel_liedje(0)
                    stel_ritme_1()
                elif lijn == "schakelaarIngedrukt2":
                    speel_liedje(1)
                    stel_ritme_2()
                elif lijn == "schakelaarIngedrukt3":
                    speel_liedje(2)
                    stel_ritme_3()
                elif lijn == "schakelaarIngedrukt4":
                    speel_liedje(3)
                    stel_ritme_4()

def speel_liedje(index):
    liedjes = [
        "Have_You_Ever_Seen_The_Rain.mp3",
        "Angels.mp3",
        "Come_Together.mp3",
        "Iris.mp3"
    ]
    try:
        pygame.mixer.music.load(liedjes[index])
        pygame.mixer.music.play(loops=0, start=0.0)
    except pygame.error as e:
        print(f"Fout bij laden of afspelen van muziek: {e}")

def stel_ritme_1():
    global ritme
    ritme = [
        {"tijd": 1000, "strook": 0, "afbeelding": "sticks"},
        {"tijd": 1600, "strook": 1, "afbeelding": "hi-hat"},
        {"tijd": 2200, "strook": 2, "afbeelding": "Snaredrum"},
        {"tijd": 2800, "strook": 0, "afbeelding": "2 Snaredrum"},
        {"tijd": 3400, "strook": 1, "afbeelding": "hi-hat"},
        {"tijd": 4000, "strook": 2, "afbeelding": "Snaredrum"},
        {"tijd": 4600, "strook": 0, "afbeelding": "sticks"},
    ]

def stel_ritme_2():
    global ritme
    ritme = [
        {"tijd": 1000, "strook": 0, "afbeelding": "sticks"},
        {"tijd": 1800, "strook": 1, "afbeelding": "hi-hat"},
        {"tijd": 2600, "strook": 2, "afbeelding": "Snaredrum"},
        {"tijd": 3400, "strook": 0, "afbeelding": "2 Snaredrum"},
        {"tijd": 4200, "strook": 1, "afbeelding": "hi-hat"},
        {"tijd": 5000, "strook": 2, "afbeelding": "Snaredrum"},
        {"tijd": 5800, "strook": 0, "afbeelding": "sticks"},
    ]

def stel_ritme_3():
    global ritme
    ritme = [
        {"tijd": 1000, "strook": 1, "afbeelding": "hi-hat"},
        {"tijd": 1700, "strook": 0, "afbeelding": "sticks"},
        {"tijd": 2400, "strook": 2, "afbeelding": "Snaredrum"},
        {"tijd": 3100, "strook": 1, "afbeelding": "hi-hat"},
        {"tijd": 3800, "strook": 0, "afbeelding": "sticks"},
        {"tijd": 4500, "strook": 2, "afbeelding": "2 Snaredrum"},
        {"tijd": 5200, "strook": 1, "afbeelding": "hi-hat"},
    ]

def stel_ritme_4():
    global ritme
    ritme = [
        {"tijd": 1000, "strook": 0, "afbeelding": "sticks"},
        {"tijd": 2500, "strook": 1, "afbeelding": "hi-hat"},
        {"tijd": 4000, "strook": 2, "afbeelding": "Snaredrum"},
        {"tijd": 5500, "strook": 0, "afbeelding": "2 Snaredrum"},
        {"tijd": 7000, "strook": 1, "afbeelding": "hi-hat"},
        {"tijd": 8500, "strook": 2, "afbeelding": "Snaredrum"},
        {"tijd": 10000, "strook": 0, "afbeelding": "sticks"},
    ]

# Start seriÃ«le thread
if ser is not None:
    serial_thread = threading.Thread(target=lees_serial, daemon=True)
    serial_thread.start()

# Achtergrond tekenen
def teken_achtergrond():
    scherm.fill(GRIJS)
    rechter_vlak_breedte = 200
    rechterscherm_balk = pygame.Surface((rechter_vlak_breedte, hoogte), pygame.SRCALPHA)
    rechterscherm_balk.fill((150, 150, 150, 128))
    scherm.blit(rechterscherm_balk, (breedte - rechter_vlak_breedte, 0))

    for y in strook_y_posities:
        pygame.draw.rect(scherm, DONKERGRIJS, (0, y, breedte, strook_hoogte))

# Vierkanten tekenen
def teken_vierkanten():
    for vierkant in vierkanten:
        scherm.blit(vierkant['afbeelding'], (vierkant['x'], vierkant['y']))

def teken_menu():
    menu_breedte = 320
    menu_hoogte = hoogte
    # Maak een surface met alpha (transparantie) en teken afgeronde achtergrond
    menu_oppervlak = pygame.Surface((menu_breedte, menu_hoogte), pygame.SRCALPHA)
    pygame.draw.rect(menu_oppervlak, WIT, (0, 0, menu_breedte, menu_hoogte), border_radius=20)

    # Positie op het scherm
    menu_x = 0
    menu_y = 0
    scherm.blit(menu_oppervlak, (menu_x, menu_y))

    menu_rect = pygame.Rect(menu_x, menu_y, menu_breedte, menu_hoogte,
                            border_top_left_radius=0, border_top_right_radius=20,
                            border_bottom_left_radius=0, border_bottom_right_radius=20)
    pygame.draw.rect(scherm, ZWART, menu_rect, width=3, 
                     border_top_left_radius=0, border_top_right_radius=20,
                     border_bottom_left_radius=0, border_bottom_right_radius=20)

    font = pygame.font.SysFont(None, 26)
    y_offset = 40
    optie_hoogte = 70
    padding = 10
    afgeronde_hoek = 20

    for index, optie in enumerate(menu_opties):
        regels = optie.split('\n')
        optie_rect = pygame.Rect(20, y_offset, menu_breedte - 40, optie_hoogte)

        if index == geselecteerde_index:
            pygame.draw.rect(scherm, ZWART, optie_rect, width=3, border_radius=afgeronde_hoek)

        for i, regel in enumerate(regels):
            tekst = font.render(regel, True, ZWART)
            tekst_rect = tekst.get_rect(center=(optie_rect.centerx, optie_rect.y + 20 + i * 18))
            scherm.blit(tekst, tekst_rect)

        y_offset += optie_hoogte + 10

    # Hier teken je de switch
    draw_switch(200, 300)  # pas de x en y aan zoals gewenst

# Main loop
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                start_signaal = True
            elif event.key == pygame.K_ESCAPE:
                scherm = pygame.display.set_mode((800, 480))
            elif event.key == pygame.K_a:
                scherm = pygame.display.set_mode((0, 0), pygame.FULLSCREEN)

    if start_signaal and not spel_gestart:
        spel_gestart = True
        starttijd = pygame.time.get_ticks()
        menu_actief = False
        huidige_noot_index = 0
        vierkanten.clear()
        start_signaal = False

    if spel_gestart:
        huidige_tijd = pygame.time.get_ticks() - starttijd

        while huidige_noot_index < len(ritme) and huidige_tijd >= ritme[huidige_noot_index]["tijd"]:
            strook_index = ritme[huidige_noot_index]["strook"]
            afbeelding_naam = ritme[huidige_noot_index]["afbeelding"]
            afbeelding_obj = afbeeldingen.get(afbeelding_naam, afbeeldingen["sticks"])
            y = strook_y_posities[strook_index] + (strook_hoogte - vierkant_grootte) // 2
            vierkanten.append({'x': -vierkant_grootte, 'y': y, 'afbeelding': afbeelding_obj})
            huidige_noot_index += 1

        for vierkant in vierkanten:
            vierkant['x'] += snelheid
        vierkanten = [v for v in vierkanten if v['x'] < breedte]

    teken_achtergrond()
    if menu_actief:
        teken_menu()
    elif spel_gestart:
        teken_vierkanten()

    pygame.display.flip()
    klok.tick(60)

pygame.quit()
