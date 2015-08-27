//Header files

import java.io.*;
import java.net.*;

// HttpClient is a client process

public class HttpClient 
  {

// private variables needed for Communicating to server

   private String hostname;
   private int port_number;
   private String process;
   private String file_name;
   private InputStream output_server;
   private OutputStream input_server;
   private Socket socket = null;

// Objects are created for the following action to be taken place in constructor

    HttpClient(String hostname, int port_number, String process, String file_name)
   {
	 this.hostname = hostname;
	 this.port_number = port_number;
	 this.process = process;
	 this.file_name = file_name;
	 doProcess();
   }

//Process function creates socket connection

   private void doProcess()
   {
    try 
     {

//Creates a new network socket connection to the desired host and port

      socket = new Socket(hostname, port_number);

//Get input and output streams for the socket

      output_server = socket.getInputStream();
      input_server = socket.getOutputStream();

//checking get or put condition

	  if(process.equalsIgnoreCase("get"))
	    Getprocedure();
	  else if (process.equalsIgnoreCase("put"))
	    Putprocedure();
	  else
	    doNotImplemented();

      socket.close();

    }

//Exception Handling

    catch (Exception e) 
    {
	  e.printStackTrace();
    }
  }

// Sends the GET Request to the Server 

    private void Getprocedure() throws Exception
     {
      String InputLine;

// Sending the GET command to the Web server.

        String request = process + " " + file_name + " " + "HTTP/1.0" + "\n\n";
        input_server.write(request.getBytes());
        BufferedReader br=new BufferedReader(new InputStreamReader(output_server));
        while ((InputLine = br.readLine()) != null) 
          {
          System.out.println(InputLine);
          }
	 }

// Sends the PUT Request to the Server.

   private void Putprocedure() throws Exception 
    {
      FileInputStream  file = null;
      byte[]            value = new byte[1024];
      int               count;
      String InputLine;
      try 
        {

// Sending the PUT command to the Web server.

         String request = process + " " + file_name + " "+ "HTTP/1.0" + "\n\n";
         input_server.write(request.getBytes());

// Open the file.

         file = new FileInputStream(file_name);
         while (true) 
          {
           count = file.read(value);
           if (count == -1) 
           {
               break;
           }
           input_server.write(value, 0, count);
          }

//Closing the socket
 
       socket.shutdownOutput();
       BufferedReader br=new BufferedReader(new InputStreamReader(output_server));
       while ((InputLine = br.readLine()) != null) 
        {
           System.out.println(InputLine);
        }

     }
     catch(Exception e)
      {
	    e.printStackTrace();
      }
     finally 
      {
		output_server.close();
		input_server.close();
        file.close();
      }
   }

 
//Requests which are not implemented
 
private void doNotImplemented()
 {
  System.out.println("Only GET and PUT Procedures are allowed.");
 }

// Main program.

   public static void main(String[] args) 
    {

// Verifying the argument length

      if (args.length != 4) 
      {
        System.out.println("HttpClient server port_number GET/PUT filename");
        System.exit(0);
	  }

//Command line Input

      String hostname = args[0];
      int port_number = Integer.parseInt(args[1]);
      String process = args[2];
      String file_name = args[3];
      HttpClient client = new HttpClient(hostname, port_number, process, file_name);
   }
}
