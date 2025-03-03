import socket
import struct

# Funkce pro dekódování sACN paketu
def decode_sacn_packet(packet_data):
    # SACS Packet je složený z několika vrstev
    # Předpokládáme, že data začínají od prvního bajtu, zde se podíváme na Root, Framing a DMP vrstvy.
    
    # Root Layer (první část)
    acn_packet_id = packet_data[4:13].decode('ascii')
    flags_length = struct.unpack('>H', packet_data[13:15])[0]
    
    # Framing Layer (druhá část)
    source_name = packet_data[38:102].decode('utf-8').strip('\x00')
    universe = struct.unpack('>H', packet_data[118:120])[0]
    sequence_number = packet_data[120]
    
    # DMP Layer (poslední část)
    dmx_data_start = 127  # Kde začínají DMX data
    dmx_channels = packet_data[dmx_data_start:dmx_data_start + 512]  # Předpokládáme maximálně 512 kanálů

    # Vytiskneme dekódované informace
    print(f"ACN Packet ID: {acn_packet_id}")
    print(f"Source Name: {source_name}")
    print(f"Universe: {universe}")
    print(f"Sequence Number: {sequence_number}")
    print(f"First 10 DMX Channels: {list(dmx_channels[:10])}")  # Vypíšeme prvních 10 kanálů

# Funkce pro příjem a dekódování sACN dat z UDP
def listen_for_sacn_data():
    UDP_IP = "127.0.0.1"
    UDP_PORT = 5568  # Standardní port pro sACN

    # Vytvoření socketu
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))

    print(f"Poslouchám na {UDP_IP}:{UDP_PORT}...")

    while True:
        # Přijmout paket z UDP
        packet_data, addr = sock.recvfrom(1024)
        print(f"Přijato {len(packet_data)} bajtů od {addr}")

        # Dekódování sACN paketu
        decode_sacn_packet(packet_data)

if __name__ == "__main__":
    listen_for_sacn_data()
