import java.io.*;
import java.net.*;

public class JavaClient {

    private static final String SERVER_IP = "127.0.0.1";
    private static final int SERVER_PORT = 8080;

    public static void main(String[] args) {

        try ( // try-with-resources handles auto-closing of resources on exit or catch
            Socket socket = new Socket(SERVER_IP, SERVER_PORT);

            InputStream inputStream = socket.getInputStream();
            BufferedReader in = new BufferedReader(new InputStreamReader(inputStream));

            OutputStream outputStream = socket.getOutputStream();
            PrintWriter out = new PrintWriter(outputStream, true);

            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        ) {
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
                String response = in.readLine();
                if (response == null) {
                    System.out.println("Server closed. Exiting");
                    break;
                }
                System.out.println("Server: " + response);
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