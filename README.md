# Chronos - Prueba de tiempo de reaccion visual

Proyecto Arduino para una prueba de psicologia donde se mide el tiempo de reaccion de un participante ante un estimulo visual (LED).

## Objetivo

Medir en milisegundos el tiempo entre:

1. Encendido del estimulo visual (LED).
2. Respuesta del usuario al soltar uno de dos botones.

El resultado se muestra por monitor serial.

## Componentes usados

- Arduino (compatible con `analogWrite` en pines 10 y 11).
- Pantalla LCD I2C 16x2.
- 2 botones (izquierda y derecha).
- LEDs: rojo, verde y ambar (segun modo).
- Resistencias y cableado basico.

## Conexiones principales

- Boton derecho: pin `8` (`RH`)
- Boton izquierdo: pin `9` (`LH`)
- LED rojo: pin `11` (`LEDR`)
- LED verde: pin `12` (`LEDV`)
- LED ambar: pin `10` (`LEDA`)
- LCD I2C: direccion `0x27`

## Modos de prueba

- `1` - Prueba normal:
  - Secuencia fija: LED verde y luego LED rojo.
  - 15 iteraciones.
- `2` - Prueba aleatoria:
  - Estimulo aleatorio por iteracion.
  - 32 iteraciones.

## Uso rapido

1. Abrir `chronos/chronos.ino` en Arduino IDE.
2. Cargar el sketch a la placa.
3. Abrir monitor serial a `9600` baudios.
4. Ingresar `1` o `2` segun el modo.
5. Seguir instrucciones en LCD y responder soltando el boton correcto.

## Salida esperada (serial)

Por cada estimulo se imprime:

- LED activado.
- Tiempo de activacion (`LTIME`).
- Lado de respuesta (`DERECHA` o `IZQUIERDA`).
- Tiempo de respuesta (`BTIME`).
- `TIEMPO DE REACCION` en milisegundos.

## Nota de proyecto open source

El codigo es abierto. Si quieres distribucion formal, agrega una licencia explicita (`MIT`, `Apache-2.0`, etc.) para dejar claros los permisos de uso.
