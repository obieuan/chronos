# README Tecnico - Chronos

## 1. Resumen tecnico

El sketch `chronos/chronos.ino` implementa una prueba de reaccion visual controlada por monitor serial. Registra timestamps con `millis()` para calcular la latencia de respuesta entre encendido de LED y accion del usuario.

## 2. Dependencias

Bibliotecas incluidas:

- `Wire.h`
- `LCD.h`
- `LiquidCrystal_I2C.h`

Objeto LCD:

```cpp
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);
```

## 3. Variables y pines

- Entradas:
  - `RH = 8` (boton derecho)
  - `LH = 9` (boton izquierdo)
- Salidas:
  - `LEDA = 10` (LED ambar, PWM)
  - `LEDR = 11` (LED rojo, PWM)
  - `LEDV = 12` (LED verde, digital)
- Tiempos:
  - `LTIME`: instante de activacion del estimulo
  - `BTIME`: instante de respuesta

## 4. Flujo de ejecucion

## 4.1 `setup()`

- Inicializa serial a `9600`.
- Inicializa LCD I2C, backlight y limpia pantalla.
- Configura pines de entrada/salida.

## 4.2 `loop()`

- Muestra menu serial:
  - `'1'`: prueba normal.
  - `'2'`: prueba aleatoria.
- Espera bloqueo activo (`while (Serial.available() <= 0) {}`).
- Lee opcion con `Serial.read()`.

## 4.3 Modo normal (`case '1'`)

- Muestra instrucciones en LCD con pausas.
- Ejecuta `15` iteraciones (`for i=0..14`).
- En cada iteracion:
  - Espera 5 s.
  - Enciende LED verde, registra `LTIME`.
  - Espera respuesta (cambio en estado relativo de botones).
  - Registra `BTIME`, calcula `BTIME - LTIME`.
  - Apaga LED verde.
  - Espera ambos botones en estado igual para reiniciar.
  - Repite el mismo ciclo con LED rojo.

## 4.4 Modo aleatorio (`case '2'`)

- Mismo bloque inicial de instrucciones.
- Ejecuta `32` iteraciones (`for i=0..31`).
- En cada iteracion:
  - Espera 5 s.
  - `LEDRSTATE = random(2)` decide estimulo.
  - Si `1`: rojo apagado, verde y ambar encendidos.
  - Si `0`: rojo encendido, verde apagado.
  - Registra tiempos y calcula reaccion como en modo normal.
  - Apaga LEDs y espera reinicio con ambos botones.

## 5. Formato de datos serial

Cada evento imprime:

- Etiqueta de LED (`LEDV` / `LEDR`).
- Marca temporal de activacion (`LTIME ms`).
- Lado detectado (`DERECHA:` o `IZQUIERDA:`).
- Marca temporal de boton (`BTIME ms`).
- Latencia (`TIEMPO DE REACCION: <ms>`).

Esto permite registrar los datos con Serial Monitor o Serial Plotter/log externo.

## 6. Observaciones tecnicas del codigo

1. No hay `pinMode(..., INPUT_PULLUP)` ni resistencias definidas en software, por lo que el cableado debe contemplar pull-up/pull-down externos para evitar ruido.
2. En LCD 16x2 se usa `lcd.setCursor(0, 2)`, pero las filas validas son `0` y `1`; esa linea puede no mostrarse correctamente.
3. `random(2)` no usa semilla (`randomSeed`), por lo que la secuencia puede repetirse tras reinicio.
4. La logica de deteccion `while(digitalRead(RH) == digitalRead(LH))` asume una condicion inicial especifica en ambos botones; cualquier rebote puede afectar medidas.
5. Se detecta posible texto con codificacion no UTF-8 en `"REACCI??N"` dentro de serial.

## 7. Mejoras recomendadas

1. Configurar botones con `INPUT_PULLUP` y adaptar la logica activa en `LOW`.
2. Corregir LCD a `setCursor(0, 1)` para segunda fila.
3. Agregar anti-rebote (software o hardware).
4. Sembrar aleatoriedad con `randomSeed(analogRead(pin_flotante))`.
5. Exportar datos en formato CSV para analisis estadistico.
6. Agregar timeout de seguridad en esperas bloqueantes.

## 8. Estructura del repositorio

- `chronos/chronos.ino`: sketch principal.
- `.gitignore`: exclusion de artefactos de compilacion y archivos locales.
- `README.md`: documentacion funcional simple.
- `README_TECNICO.md`: documentacion tecnica y analisis.
