import java.io.*;
import java.net.*;

/*
//
// Myclient is simple web-client, that suports both GET and PUT.
//
*/
public class MyClient{
	
	//declaring varible that are needed for connecting to server.
	private String host_Name;
	private int port;
	private String action_Type;
	private String file_Name;
	private InputStream server_From;
	private OutputStream server_To;
	private Socket socket = null;
	// end of decleration of private variables
	
	/*
	//
	// MyClient constructor that takes care of the actions to be performed
	//
	*/
	MyClient(String host_Name, int port, String action_Type, String file_Name){
		this.host_Name = host_Name;
		this.port = port;
		this.action_Type = action_Type;
		this.file_Name = file_Name;
		execute_get_put(); //calls execute_get_put()
	}
	
	/*
	//
	//execute_get_put() opens a network socket connection for specified host_Name and port
	//gets the input stream and output stream
	//checks which action to apply i.e either get to apply or put to apply
	//
	*/
	private void execute_get_put(){
		
		try{
			socket = new Socket(host_Name,port);// to open new socket connection
			server_From = socket.getInputStream(); // get input stream
			server_To = socket.getOutputStream(); //  get outputstream
			
			if(action_Type.equalsIgnoreCase("get"))
				doGet_Method(); // calls doGet_Method
			else if(action_Type.equalsIgnoreCase("put"))
				doPut_Method(); // calls doPut_Method
			else
				doDefault(); // calls doDefault
			
			socket.close(); // closes socket
		}
		catch(Exception e){
			e.printStackTrace();
		}
	}
	
	
	/*
	//doGet_Method send GET request to the server and prints the responce.
	// the output is printed in command prompt.
	//
	*/
	private void doGet_Method() throws Exception {
		
		String line;
		
		String myRequest = action_Type + " " + file_Name + " "+ "HTTP/1.1" + "\n\n"; // http get command to web server.
		server_To.write(myRequest.getBytes());
		
		BufferedReader br = new BufferedReader(new InputStreamReader(server_From));
		
		while((line = br.readLine()) != null){
		System.out.println(line);
		}
	}
	
	/*
	//
	//doPut_Method send PUT request to the server and prints the responce.
	//
	*/
	private void doPut_Method() throws Exception{
	
	FileInputStream file = null;
	byte[]			data = new byte[1024];
	int  count;
	String line;
	
	try {
		String myRequest = action_Type + " " + file_Name + " "+ "HTTP/1.1" + "\n\n"; // http get command to web server.
		server_To.write(myRequest.getBytes());
		
		file = new FileInputStream(file_Name);
		
		while(true){
			count = file.read(data);
			if(count == -1){
			break;
			}
			server_To.write(data, 0, count);
		}
		
		socket.shutdownOutput();
		
		BufferedReader br = new BufferedReader(new InputStreamReader(server_From));
		
		while((line = br.readLine()) != null){
		System.out.println(line);
		}
	}
	catch(Exception e){
			e.printStackTrace();
	}
	finally{
		server_From.close();
		server_To.close();
		file.close();
	}
	}
	/*
	//doDefault to handle any othe cases other than get and put
	//it just displays "only GET and PUT methods are handeled."
	*/
	private void doDefault(){
	System.out.println("only GET and PUT methods are handeled.");
	}
	
	/*
	//main method
	//shold pass 4 arguments 
	*/
	public static void main(String[] args){
		
		if(args.length != 4){
		System.out.println("MyClient server_name port_number GET/PUT filename");
		System.exit(0);
		}
		
		String host_name = args[0];
		int port = Integer.parseInt(args[1]);
		String action_Type = args[2];
		String file_Name = args[3];
		
		MyClient client = new MyClient(host_name,port,action_Type,file_Name);
	}

}

