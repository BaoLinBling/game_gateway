import socket
import time

def test_login():
    s = socket.socket()
    s.connect(("127.0.0.1", 8080))

    s.send(b"user1")
    data = s.recv(1024)

    print("resp:", data.decode())

    s.close()

if __name__ == "__main__":
    for i in range(50):
        test_login()
        time.sleep(0.05)
