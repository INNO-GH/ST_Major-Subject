package pack;
import java.io.*;
import java.net.*;
import java.net.InetAddress;

class Client {
	public static void main(String argv[]) throws Exception {
		// Create input stream, attached to user
		BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in));
		// Create client socket, connect to server
		Socket clientSocket = new Socket(InetAddress.getLocalHost(), 6789);
		// Create output stream, attached to socket
		DataOutputStream outToServer = new DataOutputStream(clientSocket.getOutputStream());
		// Create input stream, attached to socket
		BufferedReader inFromServer = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
		// Send line to server
		String sentence;
		sentence = inFromUser.readLine();
		outToServer.writeBytes(sentence + '\n');
		// Read line from server
		String modifiedSentence;
		modifiedSentence = inFromServer.readLine();
		System.out.println("FROM SERVER: " + modifiedSentence);
		// Finish connection
		clientSocket.close();
	}
}
