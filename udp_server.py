import socket
import struct
import serial
import time

# Nastavení sériové komunikace
ser = serial.Serial('COM4', 9600)  
time.sleep(2)  # Počká na inicializaci

# Interval pro odesílání (v sekundách)
SEND_INTERVAL = 0.1  # Posíláme data maximálně 10× za sekundu

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
            dmx_string = " ".join(map(str, dmx_channels))#[:10]))  # Posíláme jen prvních 10 kanálů
            print(dmx_string)
            ser.write((dmx_string + "\n").encode())  # Odesíláme přes sériovou linku
            last_send_time = current_time  # Uložíme čas posledního odeslání

if __name__ == "__main__":
    try:
        listen_for_sacn_data()
    except KeyboardInterrupt:
        print("\nUkončuji program.")
        ser.close()
