import socket
import time

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

for i in range(100):
    msg = f"udp_msg_{i}".encode()
    sock.sendto(msg, ("127.0.0.1", 8081))
    time.sleep(0.01)

print("UDP TEST DONE")
