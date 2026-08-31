# AD9833 RPi

Driver SPI para el **AD9833** (DDS / generador de señal programable) en Raspberry Pi, usando la librería **bcm2835** implementada en C++.

## Características

- Comunicación SPI por software con la librería `bcm2835` (no depende del driver del kernel `/dev/spidev`).
- SPI0, modo 0, MSB primero, 8 bits por palabra.
- Configuración de registro de frecuencia (FREQ0 / FREQ1) en Hz.
- Configuración de registro de fase (PHASE0 / PHASE1).
- Control del pin RESET.
- FSYNC gestionado por el chip select (CS0) del SPI hardware.

## Requisitos

- Raspberry Pi con bcm2835 habilitado.
- Librería `bcm2835` instalada (`apt install libbcm2835-dev` o compilada desde http://www.airspayce.com/mikem/bcm2835/).
- `g++` con soporte C++17.

## Compilación

```sh
git clone https://github.com/siliconvalleyar-oss/ad9833_rpi.git
cd ad9833_rpi
make clean && make -j4
```

El binario se genera en `bin/app_spi`. Los objetos temporales quedan en `obj/`.

## Compilación remota (SSH a la Raspberry Pi)

```sh
ssh joy@raspberry.local "cd /home/joy/src && git clone https://github.com/siliconvalleyar-oss/ad9833_rpi.git && cd ad9833_rpi && git pull && make clean && make -j4"
```

## Ejecución

```sh
sudo ./bin/app_spi
```

> bcm2835 requiere permisos de root para acceder a la memoria física.

## Uso

```cpp
#include "spi.hpp"

int main() {
    SPI spi;

    spi.reset();                          // entrada en reset
    spi.set_frequency(1000000, 25.0e6);  // 1 MHz con reloj maestro de 25 MHz
    spi.write(CTRL_B28);                 // salir de reset y usar FREQ0

    return 0;
}
```

## Protocolo AD9833

Cada palabra SPI tiene **16 bits**:
- **D15:D14** — selección de registro:
  - `00` → registro de control
  - `01` → registro de frecuencia FREQ0
  - `10` → registro de frecuencia FREQ1
  - `11` → registro de fase PHASE0 / PHASE1
- **D13:D0** — datos de 14 bits (o 12 bits en el registro de fase).

El registro de frecuencia es de **28 bits**, por lo que se envía en **dos escrituras de 14 bits** (LSBs y MSBs), con el bit `B28` del registro de control activado.

## Estructura del proyecto

```
ad9833_rpi/
├── Makefile          # Build: objetos en obj/, binario en bin/
├── include/
│   └── spi.hpp       # Clase SPI y definiciones del AD9833
└── src/
    ├── main.cc       # Programa de ejemplo
    └── spi.cc        # Implementación SPI con bcm2835
```
