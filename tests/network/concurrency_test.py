import socket
import threading

HOST = "127.0.0.1"
PORT = 8080

def worker(i):
    try:
        s = socket.socket()
        s.connect((HOST, PORT))
        s.send(f"msg-{i}".encode())
        data = s.recv(1024)
        print(data.decode())
        s.close()
    except Exception as e:
        print("error:", e)

threads = []
for i in range(300):
    t = threading.Thread(target=worker, args=(i,))
    t.start()
    threads.append(t)

for t in threads:
    t.join()

print("DONE")
