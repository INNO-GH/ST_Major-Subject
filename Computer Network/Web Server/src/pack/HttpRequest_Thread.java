package pack;

import java.io.* ;
import java.net.* ;
import java.util.* ;

// 스테이터스코드 집합클래스
enum StatusCode{
	OK, BAD_REQUEST, FORBIDDEN, NOT_FOUND, HTTP_VERSION_NOT_SUPPORTED, INTERNAL_SERVER_ERROR, CREATED
}

// implements Runnable 통해 쓰레드를 실행
final class HttpRequest_Thread implements Runnable {
    // 응답메세지를 위한 기본적인 인스턴스 변수 설정 + 인풋과 아웃풋을 위한 소켓 설정
	final static String CRLF = "\r\n";
    final static String HTTP_VERSION = "1.1";
    final static String DEFAULT_CONTENT_TYPE = "application/octet-stream";    
    //final static string content Length "";
    final static int BUFFER_IN_SIZE = 2048;
    final static int BUFFER_OUT_SIZE = 2048;
    final static Properties CONTENT_TYPES = new Properties();
    final static EnumMap<StatusCode, String> SCODES = new EnumMap<StatusCode, String> (StatusCode.class);
    
    static {
    	CONTENT_TYPES.setProperty("html", "text/html");
    	CONTENT_TYPES.setProperty("jpg", "image/jpeg");
    	SCODES.put(StatusCode.OK, "200");
    	SCODES.put(StatusCode.CREATED, "201");
    	SCODES.put(StatusCode.BAD_REQUEST, "400");
    	SCODES.put(StatusCode.FORBIDDEN, "403");
    	SCODES.put(StatusCode.NOT_FOUND, "404");
    	SCODES.put(StatusCode.HTTP_VERSION_NOT_SUPPORTED, "505");
    }
    
    StatusCode code;    
    Socket socket; // 소켓 생성
    File requestedFile;
    
    // Constructor(생성자)
    public HttpRequest_Thread(Socket socket) throws Exception {
		this.socket = socket;
		this.code = null;
		this.requestedFile = null;
    }
    
    // 인풋스트림으로 인풋을 받고, 데이터를 분석하고, 적합한 아웃풋을 보내기
    // Implement the run() method of the Runnable interface.
    public void run() {
		try {
		    processRequest();
		} catch (Exception e) {
		    System.out.println(e);
		}
    }

    private void processRequest() throws Exception {		
	
    // 소켓의 인풋스트림과 아웃풋스트림 및 소켓 인풋스트림 버퍼 및 파일의 인풋스트림 생성  
	// Get a reference to the socket's input and output streams.
		InputStream is = null;
		DataOutputStream os = null;
        FileInputStream fis = null ;		
		// Set up input stream filters.
		BufferedReader br = null; 
		try {
			is = socket.getInputStream();
			os = new DataOutputStream(socket.getOutputStream());
			br = new BufferedReader(new InputStreamReader(socket.getInputStream()));	

			// 인풋스트림에서 요청라인을 불러오고, 요청라인에서 에러검사 
			// Get the request line of the HTTP request message.
	        String requestLine = br.readLine();
	        String checkMsg = parseRequestLine(requestLine);	 
	        
	        // 요청의 헤더라인을 지속적으로 불러오기
	        String headerLine = null;
	        int contentLength = -1;
	        while ((headerLine = br.readLine()).length() != 0) {
			    System.out.println(headerLine);
			    if (headerLine.startsWith("Content-Length:")) { // length 정보는 미리 저장 (POST를 위함)
			        contentLength = Integer.parseInt(headerLine.substring("Content-Length:".length()).trim());
			    }
			}
	        
	        // 요청라인에 에러가 없을 때 파일 인풋스트림으로 가져오는데 없으면 NOT FOUND
	        if(checkMsg.equals("GET") || checkMsg.equals("HEAD") || checkMsg.equals("POST")) {
	        	try {
	        		fis = new FileInputStream(requestedFile);
	        	}catch(FileNotFoundException e){
	        		System.out.println("FileNotFoundException while openinf file inputStream");
	        		e.printStackTrace();
	        		code = StatusCode.NOT_FOUND;
	        	}
	        }else { // 요청라인에 에러가 있으면 에러출력
	        	System.out.println();
	        	System.out.println(checkMsg);
	        }
	        
	        // 아웃풋 메세지 보내기
	        sendResponseMessage(fis, os, br, checkMsg, contentLength);
			
		}finally {
			if(os != null)
				os.close();
			if(br != null)
				br.close();
			if(fis != null)
				fis.close();
		}
		socket.close();
    }
    
    // 아웃풋 메세지 함수 (Status Line + Header Line + Entity Body)
    private void sendResponseMessage(FileInputStream fis, DataOutputStream os, BufferedReader br, String checkMsg, int contentLength) throws Exception {
		// TODO Auto-generated method stub
    	
    	// 상태라인 제작
    	String statusLine = "HTTP/" + HTTP_VERSION + " " + SCODES.get(code) + " ";    	
    	String message;
    	switch(code) {
    	case OK:
    		message = "OK";
    		break;
    	case BAD_REQUEST:
    		message = "BAD REQUEST";
    		break;
    	case FORBIDDEN:
    		message = "FORBIDDEN";
    		break;
    	case NOT_FOUND:
    		message = "NOT_FOUND";
    		break;
    	case HTTP_VERSION_NOT_SUPPORTED:
    		message = "HTTP_VERSION_NOT_SUPPORTED";
    		break;  		
    	case INTERNAL_SERVER_ERROR:
    		message = "INTERNAL_SERVER_ERROR";
    		break;
    	case CREATED:
    		message = "CREATED";
    		break;	
    	default:
    		message = "empty";    		
    	}
    	statusLine = statusLine + message;
    	
    	// 엔티티바디 제작
    	String entityBody = "<HTML>" + CRLF + " <HEAD><TITLE>?</TITLE></HEAD>" + CRLF + " <BODY>?</BODY>"+ CRLF;
    	if(code != StatusCode.OK)
    		entityBody = entityBody.replaceAll("\\?", message + " - sent by Student;s Webserver");
    	
    	System.out.println("\nSend HTTP response:"); // 콘솔창에 Status Line 보여줌
    	System.out.println("StatusLine : "+ statusLine);

    	// send the status line. 상태라인 보내기
    	os.writeBytes(statusLine + CRLF);
    	// Construct and the header lines 헤더라인 보내기
    	sendHeaderLines(os);
    	os.writeBytes(CRLF);
    	// Entity Body 메소드에 따라 파일을 보내거나(GET), 헤더만 보내거나(HEAD), 파일을 저장하거나(POST), 오류 처리(오류)
    	// GET 메소드의 경우
    	if(checkMsg.equals("GET")) {
        	System.out.println("entityBody : "+ CRLF + entityBody); // 콘솔창에 entityBody 보여줌
    		System.out.println("sending requested GET file to Client...");
    		System.out.println("----\n");
    		sendBytes(fis, os); // 파일을 불러와 전송
    	}
    	// HEAD 메소드의 경우
    	else if(checkMsg.equals("HEAD")) { 
    		System.out.println("\nsending HEAD data to Client..."); // 위에서 헤더를 보냈으니 종료
    		System.out.println("----\n");
    	}
    	// POST 메소드의 경우
    	else if(checkMsg.equals("POST")) { 
        	System.out.println("entityBody : "+ CRLF + entityBody);
	        // POST 요청 메세지에서, 서버에 저장하라고 보낸, text data를 저장하기
	        if (contentLength > 0) { // data가 존재하는 경우 -> html 파일로 저장
	            char[] postData = new char[contentLength];
	            br.read(postData, 0, contentLength);
	            String postDataStr = new String(postData); // data를 불러오고
	            String filePath = System.getProperty("user.dir") + "\\src" + "\\studentID.html";
	            BufferedWriter writer = new BufferedWriter(new FileWriter(filePath));
	            writer.write(postDataStr); // 저장함
	            writer.flush();
	            writer.close();
	    		os.writeBytes("server save content");	
	        }
	        else { // data가 없는경우 -> data가 없다고 전해주기
	        	os.writeBytes("server no-data to save");	
	        }
    		System.out.println("sending POST complete to Client...");
    		System.out.println("----\n");
    	}
    	// 에러인 경우
    	else { 
        	System.out.println("entityBody : "+ CRLF + entityBody);
    		System.out.println("sending error message to Client...");
    		System.out.println("----\n");
    		os.writeBytes(entityBody);
    	}
	}
    
    // 헤더라인을 만들고 보내는 함수
    private void sendHeaderLines(DataOutputStream os) throws Exception{
    	StringBuffer headerLines = new StringBuffer();
    	
    	String contentTypeLine = "Content-Type: ";
    	String contentLength = "Content-Length: ";
    	System.out.println("code "+ code);
    	
    	switch (code) {
    	case OK:
    		// 오류가 없는 경우에는 콘텐츠에 따라 contentType이 바뀌기 때문에 파일이름을 불러와야 함 + length 1024 고정
    		contentTypeLine += contentType(requestedFile.getName())/*     MIssion 8     */ + CRLF;
    		contentLength += "1024"/*     MIssion 6     */ + CRLF;	
    		break;
    	default:
    		// 오류가 있는 경우에는 파일이 아닌 텍스트 오류 메세지를 보내기 때문에 text/html로 확정한다 + length 1024 고정
    		contentTypeLine += "text/html"/*     MIssion 7     */ + CRLF;
    		contentLength += "1024"/*     MIssion 6     */ + CRLF;
    	}
    	headerLines.append(contentLength);
    	headerLines.append(contentTypeLine);
    	os.writeBytes(headerLines.toString());
    }
    
    // 인풋의 요청라인의 에러검사 (유효성검사)
	private String parseRequestLine(java.lang.String requestLine) {
		// TODO Auto-generated method stub
    	System.out.println("----");
    	System.out.println("Received HTTP request: ");
    	System.out.println(requestLine);
    	
    	// 공백이 3개가 아닌 Case
    	StringTokenizer tokens = new StringTokenizer(requestLine);
    	if(tokens.countTokens() != 3) {
    		code = StatusCode.NOT_FOUND;// Misson 4
    		return "Request line malformed. Returning BAD NOT FOUND.";
    	}
    	
    	// 요청라인에서 메소드와 파일명을 불러온다
    	String method = tokens.nextToken().toUpperCase();
    	String fileName = tokens.nextToken();    	 	
        System.out.println("file name: "+fileName);
        
        // 파일명의 실제 경로로 파일명 지정 및 파일 가져오기
        //Modify to your path
        //fileName = "." + fileName ;
        fileName = System.getProperty("user.dir") + "\\src" + fileName;
        File file = new File(fileName);
        
        // 파일이 존재안하거나 이상한 오류
        if(!file.exists()) {
        	code = StatusCode.NOT_FOUND;
        	return "File not FOUND";        	
        }
        if(!file.canRead()) {
        	code = StatusCode.FORBIDDEN;
        	return "File is not Readdable";    
        }
        
        // 파일이 디렉토리인 경우 없으면 에러 2개이상 에러 1개면 패스
        if(file.isDirectory()) {
        	File[] list = file.listFiles(new FilenameFilter() {
        		public boolean accept(File dir, String f) {
        			if(f.equalsIgnoreCase("index.html"))
        				return true;
        			return false;
        		}
        	});
        	if(list == null || list.length == 0) {
        		code = StatusCode.NOT_FOUND;
        		return "File not FOUND";
        	}
        	else if(list.length != 1){
        		code = StatusCode.INTERNAL_SERVER_ERROR;
            	return "found more than one file";	
        	}
        	file = list[0];
        }
        requestedFile = file;
        
        // 버전 테스트
        String version = tokens.nextToken().toUpperCase();
        if(version.equals("HTTP/1.0")) {
        	code = StatusCode.BAD_REQUEST;// mission 5
        	return "HTTP Version String is malformed.";	
        }
        if(!version.matches("HTTP/([1-9][0-9.]*)")) {
        	code = StatusCode.BAD_REQUEST;
        	return "HTTP Version String is malformed.";
        }
        if(!version.equals("HTTP/1.0") && !version.equals("HTTP/1.1") ) {
        	code = StatusCode.HTTP_VERSION_NOT_SUPPORTED;
        	return "HTTP Version is not supported.";
        }
        
        // 메소드가 올바른지 판단하고, 올바른 메소드로 Return, 메소드가 잘못되어으면 오류리턴
        if(method.equals("GET")) {
        	code = StatusCode.OK;
        	return "GET";
        }
        else if(method.equals("HEAD")) {
        	code = StatusCode.OK;
        	return "HEAD";
        }
        else if(method.equals("POST")) {
        	code = StatusCode.CREATED;
        	return "POST";
        }
        else { // 요청라인의 메소드가 잘못된경우도 exception handling 상황이니 명세에따라 NOT_FOUND
        	code = StatusCode.NOT_FOUND;
        	return "Request line malformed. Returning BAD NOT FOUND.";
        }       
	}
	
    
    // 파일을 불러와 전송해주는 함수
    private static void sendBytes(FileInputStream fis, OutputStream os) throws Exception {
		// Construct a 1K buffer to hold bytes on their way to the socket.
		byte[] buffer = new byte[BUFFER_OUT_SIZE];
		int bytes = 0;
		
		// Copy requested file into the socket's output stream.
		while ((bytes = fis.read(buffer)) != -1) {
		    os.write(buffer, 0, bytes);
		}
    }
    
    // 파일에 따른 콘텐츠타입을 결정해주는 함수
    private static String contentType(String fileName) {
		String fname = fileName.toLowerCase();
		int lastdot = fname.lastIndexOf(".");
		if( (lastdot != -1) && (lastdot != fname.length() -1) ) {
			System.out.println("type : "+CONTENT_TYPES.getProperty(fname.substring(lastdot + 1) ));
			return CONTENT_TYPES.getProperty(fname.substring(lastdot + 1), DEFAULT_CONTENT_TYPE);
		}
		
		return DEFAULT_CONTENT_TYPE;
    }
}