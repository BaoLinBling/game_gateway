import socket
import struct

HOST = "127.0.0.1"
PORT = 8080

def build_packet(cmd, payload: bytes):
    length = 8 + len(payload)
    return struct.pack(">IHH", length, 1, cmd) + payload

s = socket.socket()
s.connect((HOST, PORT))
payload = b"hello_login"
packet = build_packet(1001, payload)
s.sendall(packet)
resp = s.recv(1024)
print("recv:", resp)
s.close()
