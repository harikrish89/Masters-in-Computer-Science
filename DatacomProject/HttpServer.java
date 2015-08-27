import java.io.*;//Standard header files for input output
import java.net.*;//Header files for the support of data communication 

/**
 * HttpServer acts as web server and implements the GET and PUT methods.
 */
public class HttpServer {

   private int serverPort;

   public static void main(String[] args) {

      int port = 80;
      // Get command-lines arguments. If command-line arg "port" cannot be
      // converted to an integer, print a usage message and exit.

      if (args.length >= 1) {
         try {

           port = Integer.parseInt(args[0]);

         } catch(NumberFormatException ex) {
            System.out.println("Usage: HttpServer [port]");
            System.exit(0);
         }
      }

      (new HttpServer(port)).begin();
   }

   /**
    * Creates a new HttpServer. The new HttpServer listens on the specified port.
    */
   public HttpServer(int port) {
      super();
      this.serverPort = port;
   }

   /**
    * Starts the server.
    */
   public void begin() {

      ServerSocket       httpSocket;
      Socket             clientSocket;
      ServerThread  requestThread;

    try {
         // Creates a server socket.
         httpSocket = new ServerSocket(serverPort);

         System.out.println("HttpServer running on port " + serverPort + ".");

         try {

            while (true) {

               // Accepts any requests from client connection.

               clientSocket = httpSocket.accept();

               // Start a thread to give services to the client request.

               requestThread = new ServerThread(clientSocket);
               requestThread.start();
            }

         } finally {
            httpSocket.close();
         }

      } catch(Exception ex) {
         System.out.println(ex.toString());
      }

      System.exit(0);
   }
}
