import java.io.*;//Standard header files for input and output
import java.net.*;//Header files for the support of data communication 

// HttpClient is a simple web client. It supports both GET and PUT.
public class HttpClient {

//private variables needed for the connecting to server
   private String host; // takes the ip address or the host name e.g. www.google.com
   private int port;
   private String action;
   private String fileName;
   private InputStream from_server;
   private OutputStream to_server;
   private Socket socket = null;
//Once the client object is created the following constructor takes care of actions which are to be performed
    HttpClient(String host, int port, String action, String fileName){
	 this.host = host;
	 this.port = port;
	 this.action = action;
	 this.fileName = fileName;
	 doAction();
	}

   private void doAction(){

     try {

      // Open a network socket connection to the specified host and port
      socket = new Socket(host, port);

      // Get input and output streams for the socket
      from_server = socket.getInputStream();
      to_server = socket.getOutputStream();

	//Checking upon which action to apply (get or put)
	  if(action.equalsIgnoreCase("get"))
	    doGetMethod();
	  else if (action.equalsIgnoreCase("put"))
	    doPutMethod();
	  else
	    doNotImplemented();//if get or put are not specified 

      socket.close();

    } catch (Exception e) {
	  e.printStackTrace();
    }
   }

   // Sends the GET Request to the Server and prints the response.
    // Get method prints the output in the command prompt !
    private void doGetMethod() throws Exception{

      String theLine;

      // Send the HTTP GET command to the Web server.
        String req = action + " " + fileName + " "+ "HTTP/1.1" + "\n\n";
        to_server.write(req.getBytes());


     BufferedReader br=new BufferedReader(new InputStreamReader(from_server));

     while ((theLine = br.readLine()) != null) {
          System.out.println(theLine);
      }
	}


   // Sends the PUT Request to the Server and prints the response.
       //format myclient host port_number PUT filename 
  private void doPutMethod() throws Exception {

      FileInputStream  file = null;
      byte[]            data = new byte[1024];
      int               inCount;
      String theLine;

      try {

          // Send the HTTP PUT command to the Web server.
          String req = action + " " + fileName + " "+ "HTTP/1.1" + "\n\n";
          to_server.write(req.getBytes());


         // Open the file.

         file = new FileInputStream(fileName);

         // Read and send file data.

         while (true) {
           inCount = file.read(data);

           if (inCount == -1) {
               break;
           }

           to_server.write(data, 0, inCount);
         }

       // End of stream.
       socket.shutdownOutput();

       BufferedReader br=new BufferedReader(
		              new InputStreamReader(from_server));

        while ((theLine = br.readLine()) != null) {
           System.out.println(theLine);
        }

     }catch(Exception e){
	    e.printStackTrace();
     } finally {
		from_server.close();
		to_server.close();
        file.close();
     }
  }

 
 // Other than GET/PUT Requests.
  
 private void doNotImplemented(){
  System.out.println("Only GET & PUT are implemented.");
 }

// Entry point to the program.
   public static void main(String[] args) {

     // Check the arguments
      if (args.length != 4) {
        System.out.println("HttpClient server port_number GET/PUT filename");
        System.exit(0);
	  }
//Taking inputs from command line!
      String host = args[0];
      int port = Integer.parseInt(args[1]);
      String action = args[2];
      String fileName = args[3];

      HttpClient client = new HttpClient(host, port, action, fileName);
  }

}
