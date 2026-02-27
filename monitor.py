import sys
import time
import threading
import serial
from serial.tools import list_ports

from colorama import init, Fore, Style
init(convert=True)  # fuerza conversión ANSI->Win32 en consolas viejas

APP_NAME = "CRONOS"
DEFAULT_BAUD = 9600

GREEN = Fore.GREEN
BRIGHT_GREEN = Style.BRIGHT + Fore.GREEN
CYAN = Fore.CYAN
RESET = Style.RESET_ALL

BANNER = f"""{BRIGHT_GREEN}
  ██████╗ ██████╗  ██████╗ ███╗   ██╗ ██████╗ ███████╗
 ██╔════╝ ██╔══██╗██╔═══██╗████╗  ██║██╔═══██╗██╔════╝
 ██║      ██████╔╝██║   ██║██╔██╗ ██║██║   ██║███████╗
 ██║      ██╔══██╗██║   ██║██║╚██╗██║██║   ██║╚════██║
 ╚██████╗ ██║  ██║╚██████╔╝██║ ╚████║╚██████╔╝███████║
  ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝ ╚══════╝

+--------------------------------+
|fomenta la psicología cientifica|
+--------------------------------+
{RESET}
"""

def guess_device_port():
    ports = list(list_ports.comports())
    keywords = ("arduino", "ch340", "cp210", "usb serial", "wch", "silicon labs", "ftdi")
    for p in ports:
        desc = (p.description or "").lower()
        manuf = (p.manufacturer or "").lower()
        if any(k in desc for k in keywords) or any(k in manuf for k in keywords):
            return p.device
    if len(ports) == 1:
        return ports[0].device
    return None

def choose_port_interactive():
    ports = list(list_ports.comports())
    if not ports:
        print(GREEN + "No se detectaron puertos COM." + RESET)
        sys.exit(1)

    guessed = guess_device_port()

    print(GREEN + "Puertos detectados:" + RESET)
    for i, p in enumerate(ports, 1):
        print(GREEN + f"  {i}) {p.device} - {p.description}" + RESET)

    if guessed:
        ans = input(GREEN + f"\n{APP_NAME} detectado: {guessed}. ¿Es correcto? (Enter = sí / n = no): " + RESET).strip().lower()
        if ans in ("", "s", "si", "sí", "y", "yes"):
            return guessed

    while True:
        val = input(GREEN + "\nEscribe el COM (ej. COM5) o número: " + RESET).strip()
        if val.isdigit():
            idx = int(val)
            if 1 <= idx <= len(ports):
                return ports[idx - 1].device
        if val.upper().startswith("COM"):
            return val.upper()
        print(GREEN + "Entrada no válida." + RESET)

def reader_loop(ser, stop_event):
    while not stop_event.is_set():
        try:
            line = ser.readline()
            if line:
                text = line.decode("utf-8", errors="replace").rstrip()
                print(text)  # Arduino en blanco
        except:
            break

def pulse_dtr_reset(ser):
    ser.setDTR(False)
    time.sleep(0.5)
    ser.setDTR(True)

def main():
    print(BANNER)
    time.sleep(0.2)

    baud = DEFAULT_BAUD
    port = choose_port_interactive()

    print(GREEN + f"\nAbriendo {port} @ {baud}..." + RESET)
    print(GREEN + "Escribe 'reset' para reiniciar el Arduino.\n" + RESET)

    while True:
        try:
            with serial.Serial(port, baud, timeout=1) as ser:
                ser.reset_input_buffer()

                stop_event = threading.Event()
                t = threading.Thread(target=reader_loop, args=(ser, stop_event), daemon=True)
                t.start()

                while True:
                    user_input = input()

                    # Tus comandos en cian
                    print(CYAN + f"> {user_input}" + RESET)

                    if user_input.strip().lower() == "reset":
                        print(GREEN + "\nReiniciando CRONOS...\n" + RESET)
                        pulse_dtr_reset(ser)
                        continue

                    ser.write((user_input + "\n").encode("utf-8", errors="replace"))

        except KeyboardInterrupt:
            print(GREEN + "\nSaliendo." + RESET)
            return
        except serial.SerialException as e:
            print(GREEN + f"No se pudo abrir {port}: {e}" + RESET)
            time.sleep(1)

if __name__ == "__main__":
    main()