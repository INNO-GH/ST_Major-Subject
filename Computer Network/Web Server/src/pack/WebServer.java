package pack;

import java.io.* ;
import java.net.* ;
import java.util.* ;

public final class WebServer {
	// 소켓을 만들고 여러가지 요청에 대해 여러가지 스레드를 할당
	public static void main(String argv[]) throws Exception {
    	
		// try-catch문 정석-예외문
    	try {
    		// Could get the port number from the command line.
            // int port = (new Integer(argv[0])).intValue();    		
    		
    		// 서버소켓을 제작하고
    		// Establish the listen socket.
    		ServerSocket serversocket = new ServerSocket(8114);		
    		
    		// Process HTTP service requests in an infinite loop.
    		while (true) {
    		    // 요청에 따른 소켓 할당
    			// Listen for a TCP connection request.
    			Socket socket = serversocket.accept();
    		    
    			// 멀티쓰레드 처리를 위한 쓰레드 할당 -> HttpRequest_Thread와 연결
    		    // Construct an object to process the HTTP request message.
    		    HttpRequest_Thread request = new HttpRequest_Thread(socket);
    		    
    		    // Create a new thread to process the request.
    		    Thread thread = new Thread(request);    		    
    		    // Start the thread.
    		    thread.start();
    		}   
    		
    	}catch(IOException e) {
    		System.out.print(e.getMessage());
    	}catch(Exception e) {
    		System.out.print(e.getMessage());
    	}    			
    }
}