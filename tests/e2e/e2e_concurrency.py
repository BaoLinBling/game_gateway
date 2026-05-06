import socket
import threading
import time
import os
import multiprocessing
from metrics import collect

HOST = "127.0.0.1"
PORT = 8080

def worker(i):
    try:
        s = socket.socket()
        s.connect((HOST, PORT))
        s.send(f"msg-{i}".encode())
        s.recv(1024)
        s.close()
    except:
        pass

def run_test():
    threads = []
    for i in range(1000):
        t = threading.Thread(target=worker, args=(i,))
        t.start()
        threads.append(t)
    for t in threads:
        t.join()

if __name__ == "__main__":
    # 从项目根目录读取 pid
    pid = int(open("build/gateway.pid").read().strip())

    print("[e2e] wait warmup...")
    time.sleep(2)

    print("[e2e] start 30s metrics")
    p = multiprocessing.Process(target=collect, args=(pid, 10))
    p.start()
    run_test()
    p.join()

    print("[e2e] done")
