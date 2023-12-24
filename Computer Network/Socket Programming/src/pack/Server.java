package pack;
import java.io.*;
import java.net.*;

class Server {
	public static void main(String argv[]) throws Exception {   
		// Create welcoming socket at port 6789
		ServerSocket welcomeSocket = new ServerSocket(6789);
		// Wait on, welcoming socket for contact by client
		while(true) {
			Socket connectionSocket = welcomeSocket.accept();
			// Create input stream, attached to socket
			BufferedReader inFromClient = new BufferedReader(new InputStreamReader(connectionSocket.getInputStream()));
			// Create output stream, attached to socket
			DataOutputStream outToClient = new DataOutputStream(connectionSocket.getOutputStream());
			// Read in line from socket
			String clientSentence;
			String capitalizedSentence;
			clientSentence = inFromClient.readLine();
			capitalizedSentence = clientSentence.toUpperCase() + '\n';
			// Write out line to socket
			outToClient.writeBytes(capitalizedSentence);
		}
	}
}