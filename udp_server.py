import socket
import struct
import serial
import time
import serial.tools.list_ports

def list_serial_ports():
    ports = list(serial.tools.list_ports.comports())
    if not ports:
        print("Nenalezeny žádné COM porty.")
        return []
    
    print("Dostupné COM porty:")
    for i, port in enumerate(ports):
        print(f"[{i + 1}] {port.device} - {port.description}")
    
    return ports

def select_serial_port():
    ports = list_serial_ports()
    if not ports:
        return None
    
    while True:
        try:
            choice = int(input("Vyber číslo COM portu: "))
            if 1 <= choice <= len(ports):
                return ports[choice - 1].device
            else:
                print("Neplatná volba, zkus to znovu.")
        except ValueError:
            print("Zadej číslo.")

# Výběr COM portu
selected_com_port = select_serial_port()
if not selected_com_port:
    print("Nebyl vybrán žádný COM port. Ukončuji program.")
    exit(1)

# Nastavení sériové komunikace
ser = serial.Serial(selected_com_port, 9600)  
time.sleep(2)  # Počká na inicializaci

# Interval pro odesílání (v sekundách)
SEND_INTERVAL = 0.1

def listen_for_sacn_data():
    UDP_IP = input("Zadej IP adresu (Enter pro localhost): ").strip()
    if not UDP_IP:
        UDP_IP = "127.0.0.1"

    UDP_PORT = 5568  # Standardní port pro sACN
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))

    print(f"Poslouchám na {UDP_IP}:{UDP_PORT}...")

    last_send_time = time.time()

    while True:
        packet_data, _ = sock.recvfrom(1024)
        
        dmx_data_start = 127
        dmx_channels = packet_data[dmx_data_start:dmx_data_start + 512]
    
        current_time = time.time()
        if current_time - last_send_time >= SEND_INTERVAL:
            dmx_string = " ".join(map(str, dmx_channels))  # Posíláme všechny kanály
            print(dmx_string)
            ser.write((dmx_string + "\n").encode())  # Odesíláme přes sériovou linku
            last_send_time = current_time  # Uložíme čas posledního odeslání

if __name__ == "__main__":
    try:
        listen_for_sacn_data()
    except KeyboardInterrupt:
        print("\nUkončuji program.")
        ser.close()
