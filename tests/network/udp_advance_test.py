import socket
import time

def test_room_redis_ack():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    addr = ("127.0.0.1", 8081)

    # 房间消息
    s.sendto(b"MSG:1001:100:hello_room", addr)
    # 带序列号（触发ACK）
    s.sendto(b"SEQ:1001:999", addr)

    while True:
        try:
            data, _ = s.recvfrom(1024)
            print("Recv:", data)
            if b"ACK" in data:
                print("【ACK 半可靠机制正常】")
            if b"hello_room" in data:
                print("【房间广播正常】")
            break
        except:
            time.sleep(0.001)

if __name__ == "__main__":
    test_room_redis_ack()
