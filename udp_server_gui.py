import socket
import struct
import serial
import time
import serial.tools.list_ports
import tkinter as tk
from tkinter import ttk, messagebox
import threading

def list_serial_ports():
    ports = list(serial.tools.list_ports.comports())
    return [(port.device, port.description) for port in ports]

def start_program():
    global ser1, ser2, last_packet_var, stop_event
    com1 = com1_var.get().split(" - ")[0]
    com2 = com2_var.get().split(" - ")[0] if com2_var.get() != "Žádný" else None
    ip_address = ip_var.get().strip() or "127.0.0.1"
    
    if not com1:
        messagebox.showerror("Chyba", "Musíš vybrat první COM port!")
        return
    
    try:
        ser1 = serial.Serial(com1, 9600)
        ser2 = serial.Serial(com2, 9600) if com2 else None
        time.sleep(2)  # Počká na inicializaci
        stop_event.clear()
        threading.Thread(target=listen_for_sacn_data, args=(ip_address,), daemon=True).start()
    except Exception as e:
        messagebox.showerror("Chyba", f"Chyba při otevírání COM portu: {e}")

def listen_for_sacn_data(udp_ip):
    UDP_PORT = 5568  # Standardní port pro sACN
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((udp_ip, UDP_PORT))
    
    last_send_time = time.time()
    SEND_INTERVAL = 0.1
    
    while not stop_event.is_set():
        sock.settimeout(1)
        try:
            packet_data, _ = sock.recvfrom(1024)
            dmx_data_start = 127
            dmx_channels = packet_data[dmx_data_start:dmx_data_start + 512]
            
            current_time = time.time()
            if current_time - last_send_time >= SEND_INTERVAL:
                dmx_string = " ".join(map(str, dmx_channels))
                ser1.write((dmx_string + "\n").encode())
                if ser2:
                    ser2.write((dmx_string + "\n").encode())
                last_packet_var.set(dmx_string)
                last_send_time = current_time
        except socket.timeout:
            continue

def stop_program():
    global stop_event
    stop_event.set()
    if ser1:
        ser1.close()
    if ser2:
        ser2.close()

def create_gui():
    ports = list_serial_ports()
    port_names = [f"{port} - {desc}" for port, desc in ports]
    if not port_names:
        port_names = ["Nenalezeny žádné COM porty"]
    port_names.insert(0, "Žádný")
    
    root = tk.Tk()
    root.iconbitmap("station-engineering-black.ico")
    root.title("UDP Server")
    
    global com1_var, com2_var, ip_var, last_packet_var, stop_event
    com1_var = tk.StringVar(value=port_names[1] if len(port_names) > 1 else "")
    com2_var = tk.StringVar(value="Žádný")
    ip_var = tk.StringVar(value="127.0.0.1")
    last_packet_var = tk.StringVar(value="Žádná data")
    stop_event = threading.Event()
    
    last_packet_label = ttk.Label(root, textvariable=last_packet_var, anchor="center", font=("Arial", 12))
    last_packet_label.grid(row=0, column=0, columnspan=2, padx=5, pady=5, sticky="ew")
    
    ttk.Label(root, text="První COM port:").grid(row=1, column=0, padx=5, pady=5)
    ttk.Label(root, text="Druhý COM port:").grid(row=2, column=0, padx=5, pady=5)
    ttk.Label(root, text="IP adresa:").grid(row=3, column=0, padx=5, pady=5)
    
    com1_menu = ttk.Combobox(root, textvariable=com1_var, values=port_names[1:], state="readonly")
    com1_menu.grid(row=1, column=1, padx=5, pady=5)
    
    com2_menu = ttk.Combobox(root, textvariable=com2_var, values=port_names, state="readonly")
    com2_menu.grid(row=2, column=1, padx=5, pady=5)
    
    ip_entry = ttk.Entry(root, textvariable=ip_var)
    ip_entry.grid(row=3, column=1, padx=5, pady=5)
    
    start_button = ttk.Button(root, text="Spustit", command=start_program)
    start_button.grid(row=4, column=0, padx=5, pady=10)
    
    stop_button = ttk.Button(root, text="Zastavit", command=stop_program)
    stop_button.grid(row=4, column=1, padx=5, pady=10)
    
    root.mainloop()

if __name__ == "__main__":
    create_gui()
