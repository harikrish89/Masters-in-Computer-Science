import java.io.*;
import java.net.*;
import java.util.*;

public class MyServer{
	private int server_Port;
	
	public static void main(String[] args){
		int port = 80; //default port
		if(args.length >= 1){
			try{
				port = Integer.parseInt(args[0]);
			} catch(NumberFormatException ex){
				System.out.println("Myserver [port]");
				System.exit(0);
			} 
		}
		(new MyServer(port)).begin();
	}
	
	public MyServer(int port){
		super();
		this.server_Port = port;
	}
	
	public void begin(){
		ServerSocket 	httpSocket;
		Socket			client_Socket;
		ServerThread    requestThread;
		
		try{
			httpSocket = new ServerSocket(server_Port);
			System.out.println("httpServer running on port "+server_Port+".");
			try{
				while(true){
					client_Socket = httpSocket.accept();
					requestThread = new ServerThread(client_Socket);
					requestThread.start();
				}
			} finally{
				httpSocket.close();
			}
		}catch(Exception ex){
			System.out.println(ex.toString());
		}
		System.exit(0);
	}
	
}

 class ServerThread extends Thread{
	private Socket client_Socket;
	private OutputStream out;
	private InputStream in;
	private BufferedReader Socket_Reader;
	
	/*
	//
	//to create a request threas for given client socket
	//
	*/
	public ServerThread(Socket clientSocket){
		super();
		this.client_Socket = clientSocket;
		
	}
	
	
	/*
	//
	*/
	private String getLineMethod() throws IOException{
		String line = Socket_Reader.readLine();
		System.out.println("<Log>: "+line);
		return line;
	}
	
	
	/*
	//
	//
	*/
	private String GetURLFromLineMethod(String methofline) throws IOException{
		StringTokenizer token = new StringTokenizer(methofline," ");
		token.nextToken();
		String file_Name = token.nextToken();
		if(file_Name.startsWith("/"))
			return file_Name.substring(1);
		else
			return file_Name;
	}
	
	/*
	//
	//
	*/
	private void readHeaderSection() throws IOException{
		for(;;){
			String line = Socket_Reader.readLine();
			if(line == null)
				throw new IOException("socket is empty.");
			if(line.equals("")) break;
		}
	}
	
	/*
	//
	//
	*/
	public void run(){
	
		try{
			
			in = client_Socket.getInputStream();
			
			Socket_Reader = new BufferedReader(new InputStreamReader(in));
			
			out = client_Socket.getOutputStream();
			
			String lineMethod = getLineMethod();
			
			if(lineMethod != null){
				
				String relativeURL = GetURLFromLineMethod(lineMethod);
				if(lineMethod.toUpperCase().startsWith("GET"))
					doGet_Method(relativeURL);
				else if(lineMethod.toUpperCase().startsWith("PUT"))
					doPut_Method(relativeURL);
				else
					doDefault();
			}
			client_Socket.close();
			
			
		}catch(IOException e){
		e.printStackTrace();
		}
	
	}
	/*
	//
	//
	*/
	private void doGet_Method(String URL) throws IOException{
		RandomAccessFile file = null;
		byte[] data = new byte[1024];
		int count;
		long cont_Length = 0;
		String status ="";
		String file_name;
		String temp;
		String mime_Type = "";		
		
		try{
			file = new RandomAccessFile(URL,"r");
			
			temp = URL.toLowerCase();
			
			if (temp.endsWith(".html")) {
				mime_Type = "text/html";
			} else if (temp.endsWith(".htm")) {
				mime_Type = "text/html";
			} else if (temp.endsWith(".gif")) {
				mime_Type = "image/gif";
			} else if (temp.endsWith(".jpg")) {
				mime_Type = "image/jpeg";
			} else if (temp.endsWith(".txt")) {
				mime_Type = "text/plain";
			} else {
				mime_Type = "application/octet-stream";
			}
			
			cont_Length = file.length();
			status = "200 OK";
		}catch(FileNotFoundException ex){
			System.out.println(URL+" NOT FOUND");
			mime_Type = "text/plain";
			status = "404 Not Found";
			cont_Length = status.length();
		}
		
		sendHeaderResponse(status,mime_Type,cont_Length);
		
		if(status.equals("200 OK")){
			try{
				while(true){
					count = file.read(data);
				
					if(count == -1){
					break;
					}
					out.write(data, 0, count);
				}
			}finally{
				file.close();
			}
			
		}else{
		out.write(status.getBytes());
		}
		
	}
	/*
	//
	//
	*/
	private void sendHeaderResponse(String status,String mime_Type,long cont_Length)throws IOException{
		out.write(("HTTP/1.0 " + status + "\r\n").getBytes());
        out.write(("Server: Httpd\r\n").getBytes());
        out.write(("Date: " + new Date() + "\r\n").getBytes());
        out.write(("Content-Type: " + mime_Type + "\r\n").getBytes());
        out.write(("Content-Length: " + cont_Length + "\r\n").getBytes());
        out.write(("\r\n").getBytes());
	}
	
	/*
	//
	//
	*/
	private void doPut_Method(String URL) throws IOException{
		byte[] data = new byte[1024];
		String status ="";
		String mime_Type = "";
		int bytes_read;
		long cont_Length = 0;
		FileOutputStream file_To = null;
		
		try{
			File file = new File(URL);
			
			if(file.exists())
				status = "200 OK";
			else
				status = "200 OK File Created";
			
			file_To = new FileOutputStream(file);
			
			while((bytes_read = in.read(data)) != -1){
				file_To.write(data, 0, bytes_read);
			}
			file_To.close();
		}catch(Exception e){
		e.printStackTrace();
		status = "500 server error";
		}finally{
		mime_Type = "text/html";
		cont_Length = status.length();
		
		sendHeaderResponse(status,mime_Type,cont_Length);
		out.close();
		}
	}
	/*
	//
	//
	*/
	private void doDefault() throws IOException{
	String mime_Type = "text/html";
	String status ="501 Not Implemented";
	long cont_Length = status.length();
	
	sendHeaderResponse(status,mime_Type,cont_Length);
	}
}