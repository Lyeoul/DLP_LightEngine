import socket
import struct
import sys

def send_image(server_ip, server_port, image_path):
    # 이미지 파일을 바이너리로 읽기
    with open(image_path, "rb") as f:
        img_data = f.read()
    img_size = len(img_data)
    print(f"[IMAGE] 이미지 크기: {img_size} bytes")

    # 소켓 연결
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((server_ip, server_port))
    try:
        # 명령어 전송
        sock.sendall(b"IMAGE_SEND\n")
        # 이미지 크기 (4바이트, network byte order)
        sock.sendall(struct.pack('!I', img_size))
        # 이미지 데이터 전송
        sock.sendall(img_data)
        print("[IMAGE] 이미지 전송 완료!")

        # 서버 응답 수신
        result = sock.recv(1024)
        print("[SERVER]", result.decode())
    finally:
        sock.close()

def send_command(server_ip, server_port, cmd):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((server_ip, server_port))
    try:
        # 명령어 전송
        sock.sendall((cmd + "\n").encode())
        # 서버 응답 수신
        result = sock.recv(1024)
        print("[SERVER]", result.decode())
    finally:
        sock.close()

def main():
    # 사용법 안내
    print("DLP 엔진 테스트 클라이언트")
    print("사용법:")
    print("  python dlp_client.py <server_ip> image <image_path>")
    print("  python dlp_client.py <server_ip> led_on")
    print("  python dlp_client.py <server_ip> led_off")
    print("  python dlp_client.py <server_ip> current_get")
    print("  python dlp_client.py <server_ip> current_set <value>")
    print("  python dlp_client.py <server_ip> temp_get")
    print("---------------------------------------------------------")

    if len(sys.argv) < 3:
        print("인수가 부족합니다.")
        return

    server_ip = sys.argv[1]
    server_port = 12345  # 고정 포트

    mode = sys.argv[2].lower()

    if mode == "image" and len(sys.argv) == 4:
        image_path = sys.argv[3]
        send_image(server_ip, server_port, image_path)
    elif mode == "led_on":
        send_command(server_ip, server_port, "LED_ON")
    elif mode == "led_off":
        send_command(server_ip, server_port, "LED_OFF")
    elif mode == "current_get":
        send_command(server_ip, server_port, "CURRENT_GET")
    elif mode == "current_set" and len(sys.argv) == 4:
        value = sys.argv[3]
        send_command(server_ip, server_port, f"CURRENT_SET {value}")
    elif mode == "temp_get":
        send_command(server_ip, server_port, "TEMP_GET")
    else:
        print("알 수 없는 명령 또는 인수가 부족합니다.")

if __name__ == "__main__":
    main()