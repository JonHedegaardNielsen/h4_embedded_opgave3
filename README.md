# Embedded Opgave 3 – Pointer og Bit Operationer (ATmega328PB)

Programmet kører på ATmega328PB og modtager via UART0 en hukommelsesadresse, en bitposition og en bitværdi fra en terminal på PC'en (f.eks. Hercules). Med udgangspunkt i den modtagede adresse bruger programmet en pointer til at sætte eller cleare den angivne bit i registeret på adressen – alle andre bits i registeret efterlades uændrede.

## Sådan virker programmet

Programmet er bygget op omkring en tilstandsmaskine med 3 tilstande, der styres af en enum (`State` i `include/state_machine.h`):

| Tilstand (enum) | Modtager |
|---|---|
| `RECEIVE_ADDRESS` (`Modtag_Adresse`) | Adresse i hex, afsluttet med `:` |
| `RECEIVE_BIT_POSITION_IN_ADDRESS` (`Modtag_Bit_Position_I_Adresse`) | Ciffer 0–7 |
| `RECEIVE_BIT_POSITION_VALUE` (`Modtag_Bit_Position_Vaerdi`) | Ciffer 0 eller 1 |

Ved start står programmet i `RECEIVE_ADDRESS`. Programmet udskriver en besked på UART, hver gang det skifter tilstand, så man i terminalprogrammet altid kan se, hvilken tilstand programmet er i, og hvad der skal indtastes. Alle modtagede tegn ekko's tilbage til terminalen (minicom ekko som standard ikke lokalt). I `Modtag_Bit_Position_I_Adresse` og `Modtag_Bit_Position_Vaerdi` ignoreres linjeskift (`\r`/`\n` fra Enter), så man kan trykke Enter efter et tegn uden at det tæller som ugyldigt input.

### Tilstand 1: Modtag_Adresse

Programmet læser tegn fra UART indtil, at adressen afsluttes med **Enter eller `:`**. Tegnene opfattes som et hexadecimalt tal, og adressen er 8-bit (port-registrene ligger i datalområdet 0x20–0x5F), så kun de to sidste hex-digits tæller: `0x0025`, `0x25` og `25` giver alle adressen `0x25`, og adressen kan aldrig blive større end `0xFF`. Ved udskrivning vises adressen altid med 4 hex-digits (f.eks. `0x0025`), som opgaven foreskriver. Når adressen afsluttes, skifter programmet til `Modtag_Bit_Position_I_Adresse`.

### Tilstand 2: Modtag_Bit_Position_I_Adresse

- Modtages et gyldigt ciffer (0–7) → gemmes bitpositionen, og programmet skifter til `Modtag_Bit_Position_Vaerdi`.
- Modtages et ugyldigt tegn → programmet skifter tilbage til `Modtag_Adresse`, og der skal indtastes en adresse igen.

### Tilstand 3: Modtag_Bit_Position_Vaerdi

- Modtages et gyldigt ciffer (0 eller 1) → sætter programmet via en pointer, der peger på den modtagede adresse, enten bitpositionen høj (ved 1) eller lav (ved 0). Det sker med read-modify-write (`*p |= (1 << position)` / `*p &= ~(1 << position)`), så kun den ene udpegede bit ændres – resten af adressen er uberørt. Herefter udskrives info på UART og programmet skifter tilbage til `Modtag_Adresse`.
- Modtages et ugyldigt tegn → programmet skifter tilbage til `Modtag_Adresse` uden at foretage sig andet.

## Eksempel på brug

```text
Tilstand: Modtag_Adresse - indtast adresse i hex, afslut med Enter eller ':'
20:
Tilstand: Modtag_Bit_Position_I_Adresse - indtast ciffer 0-7
0
Tilstand: Modtag_Bit_Position_Vaerdi - indtast 0 eller 1
1
Adresse: 0x0021
Bit position: 0
Bit vaerdi: 1
Bit moenster: 0010 0001
```

Adressen, bitpositionen og bitværdien indtastes én ad gangen, når programmet beder om dem. Adressen kan både afsluttes med `:` og `Enter`.

## Programstruktur

- `main.c` – initialiserer UART og kører tilstandsmaskinen i en uendelig løkke.
- `src/state_machine.c` – tilstandsmaskinen med de 3 tilstande, validering af modtagede tegn og bit-manipulation via pointer.
- `src/uart.c` – UART-initialisering samt funktioner til at sende/modtage tegn og udskrive tal i decimalt, hexadecimalt og binært format.
- `include/ProjectDefines.h` – `F_CPU` og `BAUD`.
- `include/state_machine.h` – enum med tilstandene og `AppState`-struct.
- `include/uart.h` – erklæringer for UART-funktionerne.
