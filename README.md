# Embedded Opgave 3 – Pointer og Bit Operationer (ATmega328PB)

Programmet kører på ATmega328PB og modtager via UART0 en hukommelsesadresse, en bitposition og en bitværdi fra en terminal på PC'en (f.eks. Hercules). Med udgangspunkt i den modtagede adresse bruger programmet en pointer til at sætte eller cleare den angivne bit i registeret på adressen – alle andre bits i registeret efterlades uændrede.

## Sådan virker programmet

Programmet er bygget op omkring en tilstandsmaskine med 3 tilstande, der styres af en enum (`State` i `include/state_machine.h`):

| Tilstand (enum) | Modtager |
|---|---|
| `RECEIVE_ADDRESS` (`Modtag_Adresse`) | Adresse i hex, afsluttet med `:` |
| `RECEIVE_BIT_POSITION_IN_ADDRESS` (`Modtag_Bit_Position_I_Adresse`) | Ciffer 0–7 |
| `RECEIVE_BIT_POSITION_VALUE` (`Modtag_Bit_Position_Vaerdi`) | Ciffer 0 eller 1 |

Ved start står programmet i `RECEIVE_ADDRESS`. Programmet udskriver en besked på UART, hver gang det skifter tilstand, så man i terminalprogrammet altid kan se, hvilken tilstand programmet er i, og hvad der skal indtastes. Alle modtagede tegn ekko's tilbage til terminalen (minicom ekko som standard ikke lokalt), og linjeskift (`\r`/`\n` fra Enter) ignoreres i alle tilstande, så man kan trykke Enter efter et tegn uden at det tæller som ugyldigt input.

### Tilstand 1: Modtag_Adresse

Programmet læser tegn fra UART indtil, at der modtages et `:`. Tegnene opfattes som et hexadecimalt tal (f.eks. `0x0025` eller blot `25`). `x`/`X` i et `0x`-præfiks ignoreres, og ugyldige tegn springes over. Bemærk at adressen **skal** afsluttes med `:` – Enter sender kun et linjeskift (`\r`), som ignoreres. Når `:` modtages, skifter programmet til `Modtag_Bit_Position_I_Adresse`.

### Tilstand 2: Modtag_Bit_Position_I_Adresse

- Modtages et gyldigt ciffer (0–7) → gemmes bitpositionen, og programmet skifter til `Modtag_Bit_Position_Vaerdi`.
- Modtages et ugyldigt tegn → programmet skifter tilbage til `Modtag_Adresse`, og der skal indtastes en adresse igen.

### Tilstand 3: Modtag_Bit_Position_Vaerdi

- Modtages et gyldigt ciffer (0 eller 1) → sætter programmet via en pointer, der peger på den modtagede adresse, enten bitpositionen høj (ved 1) eller lav (ved 0). Det sker med read-modify-write (`*p |= (1 << position)` / `*p &= ~(1 << position)`), så kun den ene udpegede bit ændres – resten af porten er uberørt. Herefter udskrives info på UART og programmet skifter tilbage til `Modtag_Adresse`.
- Modtages et ugyldigt tegn → programmet skifter tilbage til `Modtag_Adresse` uden at foretage sig andet.

## UART-output

Ved hver tilstandsskift udskrives en besked med den nye tilstand og hvad der skal indtastes. Efter en vellykket bit-operation udskrives 4 linjer:

```text
Adresse: 0x0025
Bit position: 2
Bit vaerdi: 1
Bit moenster: 0000 0100
```

`Bit moenster` er det aktuelle bitmønster (bit 7 til bit 0) i registeret på adressen.

## Eksempel på brug

Sæt bit 2 i PortB (adresse 0x0025) som output og høj. Først sættes bit 2 i PortB's direction-register (adresse 0x0024), derefter i selve porten:

```text
Indtast: 0x0024 : 2 : 1   → DDRB bit 2 = output
Indtast: 0x0025 : 2 : 1   → PORTB bit 2 = høj
```

(Adressen, bitpositionen og bitværdien indtastes én ad gangen, når programmet beder om dem.)

## UART-indstillinger

- UART0, 9600 baud, 8 data bits, ingen parity, 1 stop bit (8N1)
- Baud-rate beregnes i `include/ProjectDefines.h` ud fra `F_CPU = 16 MHz`

## Programstruktur

- `main.c` – initialiserer UART og kører tilstandsmaskinen i en uendelig løkke.
- `src/state_machine.c` – tilstandsmaskinen med de 3 tilstande, validering af modtagede tegn og bit-manipulation via pointer.
- `src/uart.c` – UART-initialisering samt funktioner til at sende/modtage tegn og udskrive tal i decimalt, hexadecimalt og binært format.
- `include/ProjectDefines.h` – `F_CPU` og `BAUD`.
- `include/state_machine.h` – enum med tilstandene og `AppState`-struct.
- `include/uart.h` – erklæringer for UART-funktionerne.
