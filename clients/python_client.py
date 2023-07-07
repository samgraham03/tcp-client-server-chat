import socket

SERVER_IP = "127.0.0.1"
SERVER_PORT = 8080

try: # "with" context manager handles auto-closing of socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        client_socket.connect((SERVER_IP, SERVER_PORT))

        while True:
            # Get user input
            message = input("Enter message ('exit' to quit): ")

            if message.lower() == "exit":
                break

            message += '\n' # hardcoded fix, C server sends with newline - REVIEW

            # Send message to server
            client_socket.send(message.encode())

            # Receive message from server
            response = client_socket.recv(1024).decode()
            if not response:
                print("Server closed. Exiting")
                break
            print("Server:", response, end='') # hardcoded fix, C server sends with newline - REVIEW
except ConnectionRefusedError:
    print("Error: Connection refused. Make sure the server is running.")
except TimeoutError:
    print("Error: Connection timed out. Check your network connection.")
except Exception as e:
    print("Error occurred while communicating with the server:", str(e))