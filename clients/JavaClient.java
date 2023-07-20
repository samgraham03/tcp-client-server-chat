import java.io.*;
import java.net.*;

public class JavaClient {

    private static final String SERVER_IP = "127.0.0.1";
    private static final int SERVER_PORT = 8080;

    private static final int BUFFER_SIZE = 1024;

    public static void main(String[] args) {

        try ( // try-with-resources handles auto-closing of resources on exit or catch
            Socket socket = new Socket(SERVER_IP, SERVER_PORT);
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        ) {
            char[] buffer = new char[BUFFER_SIZE];

            while (true) {
                // Get user input
                System.out.print("Enter message ('exit' to quit): ");
                String message = reader.readLine();

                if (message.equalsIgnoreCase("exit")) {
                    break;
                }

                // Send message to server
                out.println(message);

                // Receive message from server
                int recvSize = in.read(buffer);
                if (recvSize == -1) {
                    System.out.println("Server closed. Exiting");
                    break;
                }
                System.out.println("Server: " + new String(buffer));
            }
        } catch (ConnectException e) {
            System.err.println("Error: Connection refused. Make sure the server is running.");
        } catch (SocketTimeoutException e) {
            System.err.println("Error: Connection timed out. Check your network connection.");
        } catch (IOException e) {
            System.err.println("Error occurred while communicating with the server: " + e.getMessage());
            e.printStackTrace();
        }
    }
}