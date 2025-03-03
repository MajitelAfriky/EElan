import socket
import struct

def listen_for_sacn_data():
    # Zeptáme se uživatele na IP adresu hry
    UDP_IP = input("Zadej IP adresu (Enter pro localhost): ").strip()
    if not UDP_IP:
        UDP_IP = "127.0.0.1"  # Výchozí hodnota je localhost

    UDP_PORT = 5568  # Standardní port pro sACN

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))

    print(f"Poslouchám na {UDP_IP}:{UDP_PORT}...")

    while True:
        packet_data, _ = sock.recvfrom(1024)
        
        # Skutečná DMX data začínají na byte indexu 127
        dmx_data_start = 127
        dmx_channels = packet_data[dmx_data_start:dmx_data_start + 512]
        
        # Převedeme na text oddělený mezerami
        print(" ".join(map(str, dmx_channels)))

if __name__ == "__main__":
    listen_for_sacn_data()
