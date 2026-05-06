import psutil
import time

def collect(pid, duration=30):
    p = psutil.Process(pid)

    mem_peak = 0
    fd_peak = 0

    start = time.time()

    while time.time() - start < duration:
        try:
            mem = p.memory_info().rss / 1024 / 1024
            fd = p.num_fds()

            mem_peak = max(mem_peak, mem)
            fd_peak = max(fd_peak, fd)

            print(f"[metrics] mem={mem:.2f}MB fd={fd}")

            time.sleep(1)
        except:
            break

    print("\n==== RESULT ====")
    print("MEM PEAK:", mem_peak, "MB")
    print("FD PEAK:", fd_peak)
