import java.io.*;//Standard header files for input output
import java.net.*;//Header files for the support of data communication 
import java.util.*;

/**
 * below class gives services to the client requests.
 */
public class ServerThread extends Thread {


    private Socket clientSocket;
    private OutputStream out;
    private InputStream in;
    private BufferedReader socketReader;

   /**
    * Constructs a request thread for the given client socket.
    */
   public ServerThread(Socket clientSocket) {
      super();
      this.clientSocket = clientSocket;
   }

   /*
    *  request's first line is read by this
    */
   private String getMethodLine() throws IOException {
      String line = socketReader.readLine();
      System.out.println("<Access Log>: " + line);
      return line;
   }

   /*
    * request's first line is parsed and returns the file name.
    */
   private String getURLFromMethodLine(String methodLine) throws IOException {
        StringTokenizer tokens = new StringTokenizer(methodLine," ");
        tokens.nextToken();
        String fileName = tokens.nextToken();
        if (fileName.startsWith("/"))
         return fileName.substring(1);
        else
         return fileName;
   }

   /*
    * the request's Headeris read by this.
    */
    private void readHeaderSection() throws IOException {
      for (;;) {
        String line = socketReader.readLine();
        if (line == null)
          throw new IOException("No more input on socket.");
        if (line.equals("")) break;
      }
    }

   /*
    * client side data is gathered and after processing response is sentback to client.
    * 
    */
    public void run() {
      try {

        // socketReader and socketWriter are connected to input and output
        // streams of the socket.

        in = clientSocket.getInputStream();

        socketReader = new BufferedReader(new InputStreamReader(in));

        out = clientSocket.getOutputStream();

        // Read the method line of the HTTP request from client.
        String methodLine = getMethodLine();

        if(methodLine != null) {

            //to Get requested resource
            String relURL = getURLFromMethodLine(methodLine);

            // GET and POST requests are handled
            if(methodLine.toUpperCase().startsWith("GET"))
              doGetMethod(relURL);
            else if(methodLine.toUpperCase().startsWith("PUT"))
              doPutMethod(relURL);
            else
              doNotImplemented();
        }
        clientSocket.close();

      } catch(IOException e) {
        e.printStackTrace();
      }
    }

   /**
    * Sends the response to the client other than GET/PUT Requestes.
    */
    private void doNotImplemented() throws IOException {
      long              contentLength = 0;
      String            status = "";
      String            mimeType = "";

      status = "501 Not Implemented";
      mimeType = "text/html";
      contentLength = status.length();

      // Send the response Headers.
      sendResponseHeaders(status, mimeType,contentLength);
    }

   /**
    *response is sent to the client for the PUT Request.
    */
    private void doPutMethod(String urlName) throws IOException {

      byte[]            data = new byte[1024];
      int               bytes_read;
      long              contentLength = 0;
      String            status = "";
      String            mimeType = "";
      FileOutputStream to_file = null;

     try{

      File file = new File(urlName);

      if(file.exists())
        status = "200 OK";
      else
        status = "201 Created";

      // Get an output stream to write the file contents
      to_file = new FileOutputStream(file);

      while((bytes_read = in.read(data)) != -1) {
         to_file.write(data, 0, bytes_read);
      }
     to_file.close();

     }catch(Exception ex){
	   ex.printStackTrace();
	   status = "500 Internal Server Error";

	 }finally {
       mimeType = "text/html";
       contentLength = status.length();
       // Send the response Headers.
       sendResponseHeaders(status, mimeType,contentLength);
       out.close();
	 }
   }


   /**
    * Sends the response to the client for the GET Request.
    */
    private void doGetMethod(String urlName) throws IOException {
      RandomAccessFile  file = null;
      byte[]            data = new byte[1024];
      int               inCount;
      String            filename;
      String            temp;
      String            mimeType = "";
      long              contentLength = 0;
      String            status = "";


      try {
         // Open the file.

         file = new RandomAccessFile(urlName, "r");

         // Set content type.

         temp = urlName.toLowerCase();

         if (temp.endsWith(".html")) {
            mimeType = "text/html";
         } else if (temp.endsWith(".htm")) {
            mimeType = "text/html";
         } else if (temp.endsWith(".gif")) {
            mimeType = "image/gif";
         } else if (temp.endsWith(".jpg")) {
            mimeType = "image/jpeg";
         } else if (temp.endsWith(".txt")) {
            mimeType = "text/plain";
         } else {
            mimeType = "application/octet-stream";
         }

         // Set content length and status;

         contentLength = file.length();
         status = "200 OK";

      } catch(FileNotFoundException ex) {
         System.out.println("RequestThread: File not found: " + urlName);

         mimeType = "text/plain";
         status = "404 Not Found";
         contentLength = status.length();
      }

      // Send the response Headers.
        sendResponseHeaders(status, mimeType,contentLength);

      // read file and send it to output stream in case status is OK.

      if (status.equals("200 OK")) {

         // file data is read and sent

         try {
            while (true) {
               inCount = file.read(data);

               if (inCount == -1) {
                  break;
               }

               out.write(data, 0, inCount);
            }
         } finally {
            file.close();
         }

      } else {
         out.write(status.getBytes());
      }
   }


   /*
    *  Response Headers are sent by this.
    */
   private void sendResponseHeaders(String status,String mimeType,
                                      long contentLength) throws IOException {
         out.write(("HTTP/1.0 " + status + "\r\n").getBytes());
         out.write(("Server: Httpd\r\n").getBytes());
         out.write(("Date: " + new Date() + "\r\n").getBytes());
         out.write(("Content-Type: " + mimeType + "\r\n").getBytes());
         out.write(("Content-Length: " + contentLength + "\r\n").getBytes());
         out.write(("\r\n").getBytes());
  }

}
