//Header Files

import java.io.*;
import java.net.*; 

//HttpServer is a simple web server.
 
public class HttpServer 
  {

   private int server_port_number;

   public static void main(String[] args) 
   {

      int port_number = 80;

// In command-line arg "port_number" cannot be converted to an integer, print a message and terminate.

      if (args.length >= 1) 
       {
         try {

           port_number = Integer.parseInt(args[0]);

             }
         
         catch(NumberFormatException ex)
             {
            System.out.println("Message: HttpServer [port_number]");
            System.exit(0);
             }
       }

      (new HttpServer(port_number)).begin();
   }

//New HttpServer that listens on the specific port.
  
   public HttpServer(int port_number)
   {
      super();
      this.server_port_number = port_number;
   }

//Igniting the Server

   public void begin() 
   
   {

      ServerSocket http_serverSocket;
      Socket http_clientSocket;
      ServerThread Thread_calling;

//Try statement

    try {

// Creating a server socket.

         http_serverSocket = new ServerSocket(server_port_number);

         System.out.println("HttpServer running on port " + server_port_number + ".");

         try {

            while (true) {

// Accepting Multiple client connection.

               http_clientSocket = http_serverSocket.accept();

// Creating a thread to service the client request.

               Thread_calling = new ServerThread(http_clientSocket);
               Thread_calling.start();
            }

         } finally {
            http_serverSocket.close();
         }

      } catch(Exception ex) {
         System.out.println(ex.toString());
      }

      System.exit(0);
   }
}
