# InBayt - PlatformIO Project (XIAO ESP32-S3)

Proyecto skeleton para el dispositivo **INBAYT** organizado en arquitectura simplificada:
`application / framework / drivers / config`.

## Cómo usar

1. Abre este proyecto en PlatformIO (VSCode).
2. Verifica `platformio.ini` — actualmente configurado para `seeed_xiao_esp32s3`. Si tu placa tiene otro identificador, cámbialo.
3. Revisa `lib_deps` en `platformio.ini` para instalar dependencias (PubSubClient, ArduinoJson).
4. Compila y sube.
5. Los certificados y credenciales ya están integrados en `config/Settings.h` con los valores que proporcionaste.

--- 
Archivos principales:
- `src/application/main.cpp`
- `src/framework/mqtt/AWSMQTTClient.*`
- `src/drivers/gpio/DoorRelay.*`
- `src/config/Settings.h` (conteniene WiFi y certificados)
