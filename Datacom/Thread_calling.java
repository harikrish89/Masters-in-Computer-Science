// Header Files

import java.io.*;
import java.net.*;
import java.util.*;

// Thread_calling class process client requests

public class Thread_calling extends Thread 
  {


    private Socket client_Socket;
    private OutputStream outputstream;
    private InputStream inputstream;
    private BufferedReader socket_InputReader;

// Calling a  thread for the given client socket.
    
   public Thread_calling(Socket client_Socket) 
   {
      super();
      this.client_Socket = client_Socket;
   }

// Read the request's based on First Input.

      private String getprocedure() throws IOException 
   {
      String Input_line = socket_InputReader.readLine();
      System.out.println("<Log>: " + Input_line);
      return Input_line;
   }

// Reads the first line of the request and returns the file name.
 
        private String getURLprocedure(String procedure_Input) throws IOException 
   {
        StringTokenizer new_tokens = new StringTokenizer(procedure_Input," ");
        new_tokens.nextToken();
        String file_name = new_tokens.nextToken();
        if (file_name.startsWith("/"))
         return file_name.substring(1);
        else
         return file_name;
   }

// Read the Header.
  
    private void Header_read() throws IOException 
  {
      for (;;) {
        String Input_line = socket_InputReader.readLine();
        if (Input_line == null)
          throw new IOException("No more input on socket.");
        if (Input_line.equals("")) break;
      }
    }

// Processing of the Data from the client and returns backs the value.
    
    public void run() 
   {
      try {

// Connection of Sockets

        inputstream = client_Socket.getInputStream();

        socket_InputReader = new BufferedReader(new InputStreamReader(inputstream));

        outputstream = client_Socket.getOutputStream();

// Read the  HTTP request from the client.
       
 String procedure_Input = getprocedure();

        if(procedure_Input != null) {

// Find the requested resource

            String URL_Input = getURLprocedure(procedure_Input);

// GET and PUT requests.

            if(procedure_Input.toUpperCase().startsWith("GET"))
              doGetProcedure(URL_Input);
            else if(procedure_Input.toUpperCase().startsWith("PUT"))
              doPutProcedure(URL_Input);
            else
              Not_Included();
        }
        client_Socket.close();

      } catch(IOException e) {
        e.printStackTrace();
      }
    }

// Not Implemented Requests.
  
    private void Not_Included() throws IOException 
    {
      long              Length = 0;
      String            status = "";
      String            mime_Type = "";

      status = "501 Not_Included";
      mime_Type = "text/html";
      Length = status.length();

// Sending the response Headers.

      sendResponseHeaders(status, mime_Type,Length);
    }

// Response for Client Put Operation.
  
    private void doPutProcedure(String url_Name) throws IOException 
    {

      byte[]            value = new byte[1024];
      int               bytes_Input;
      long              Length = 0;
      String            status = "";
      String            mime_Type = "";
      FileOutputStream file_write = null;

     try{

      File file_new = new File(url_Name);

      if(file_new.exists())
        status = "200 OK";
      else
        status = "201 Created";

//writing the file contents

      file_write = new FileOutputStream(file_new);

      while((bytes_Input = inputstream.read(value)) != -1) {
         file_write.write(value, 0, bytes_Input);
      }
     file_write.close();

     }catch(Exception ex){
	   ex.printStackTrace();
	   status = "500 Internal Server Error";

	 }finally {
       mime_Type = "text/html";
       Length = status.length();

// Sending the response Headers.

       sendResponseHeaders(status, mime_Type,Length);
       outputstream.close();
	 }
   }


//  Response for Client Get Operation.
  
    private void doGetProcedure(String url_Name) throws IOException 
    {
      RandomAccessFile  file_new = null;
      byte[]            value = new byte[1024];
      int               Count;
      String            file_name;
      String            temporary;
      String            mime_Type = "";
      long              Length = 0;
      String            status = "";


      try {

         file_new = new RandomAccessFile(url_Name, "r");

// Type Content.

         temporary = url_Name.toLowerCase();

         if (temporary.endsWith(".html")) {
            mime_Type = "text/html";
         } else if (temporary.endsWith(".htm")) {
            mime_Type = "text/html";
         } else if (temporary.endsWith(".gif")) {
            mime_Type = "image/gif";
         } else if (temporary.endsWith(".jpg")) {
            mime_Type = "image/jpeg";
         } else if (temporary.endsWith(".txt")) {
            mime_Type = "text/plain";
         } else {
            mime_Type = "application/octet-stream";
         }

// Setting File length and status;

         Length = file_new.length();
         status = "200 OK";

      } catch(FileNotFoundException ex) {
         System.out.println("RequestThread: File not found: " + url_Name);

         mime_Type = "text/plain";
         status = "404 Not Found";
         Length = status.length();
      }

// Sending the response Headers.

        sendResponseHeaders(status, mime_Type,Length);

// Based upon the status it reads the file and send it to output stream.

      if (status.equals("200 OK")) 
     {

// Reading and sending the data from the file.

         try {
            while (true) {
               Count = file_new.read(value);

               if (Count == -1) {
                  break;
               }

               outputstream.write(value, 0, Count);
            }
         } finally {
            file_new.close();
         }

      } else {
         outputstream.write(status.getBytes());
      }
   }


// Sending the Response Headers.
 
   private void sendResponseHeaders(String status,String mime_Type,long Length) throws IOException 
  {
         outputstream.write(("HTTP/1.0 " + status + "\r\n").getBytes());
         outputstream.write(("Server: Httpd\r\n").getBytes());
         outputstream.write(("Date: " + new Date() + "\r\n").getBytes());
         outputstream.write(("Type: " + mime_Type + "\r\n").getBytes());
         outputstream.write(("Length: " + Length + "\r\n").getBytes());
         outputstream.write(("\r\n").getBytes());
  }

}
