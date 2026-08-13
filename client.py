import socket

def test_tcp_client():
    host = '127.0.0.1'
    port = 8080

    print(f"Connecting to C++ HFT Server at {host}:{port}...")

    client_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

    try:
        client_socket.connect((host,port))
        print("Connected successfully to C++ Engine!")

        message = "STOP_LOSS BUY ORDER: AAPL @150.5 Trigger: 148.0 Qty: 100"
        client_socket.sendall(message.encode('utf-8'))
        print(f"Sent to Server: {message}")

        response = client_socket.recv(1024)
        print(f"Received from Server: {response.decode('utf-8')}")

    except Exception as e:
        print(f"Connection failed: {e}")
    finally:
        client_socket.close()
        print("Connection closed")

if __name__ == "__main__":
    test_tcp_client()