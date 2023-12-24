package pack;

import java.io.*; 
import java.net.*;
import javax.swing.*; // 이미지 GUI 관련 라이브러리
import java.awt.*;
import java.awt.image.*; // 이미지 읽고 쓰기 및 로드 관련 라이브러리
import javax.imageio.*;

public class WebClient {
	public static void main(String argv[]) throws Exception {
		// 클라이언트 정의 -> 사용자입력 스트림 제작, 기본 변수 선언
		WebClient client = new WebClient();
		BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in));
		String urlString;
		String method;
		String data;
		String result;
		
		// URL과 Method를 입력받고, Method에 따라 판단하여 진행 
		while(true) {
			// URL과 Method 입력
			System.out.print("URL : ");
			urlString = inFromUser.readLine();
			System.out.print("Method : ");
			method = inFromUser.readLine();
			
			// 판단 진행 부분
			if("GET".equals(method)) {
				result = client.getWebContentByGet(urlString, "UTF-8", 10000);
			}
			else if("HEAD".equals(method)) {
				result = client.getWebContentByHead(urlString, "UTF-8", 10000);
			}
			else if("POST".equals(method)) {
				System.out.print("Data : "); // POST할 data 입력
				data = inFromUser.readLine(); 
				result = client.getWebContentByPost(urlString, data, "UTF-8", 10000);
			}
			else {
				result = "Wrong Method\n";
			}
			System.out.println();
			
			// 결과를 출력하는 부분
			if(result == null) {
				System.out.println("Error");
				System.out.println();
			}
			else {
				System.out.println(result);
			}
		}
	}
	
	// Get Method로 서버로 보내고 서버로부터 응답을 받음
	public String getWebContentByGet(String urlString, final String charset, int timeout) throws IOException {
		if(urlString == null || urlString.length() == 0) { // urlString이 null이거나 0인경우, 유효하지 않기 때문에, null을 반환
			return null;
		}
		urlString = (urlString.startsWith("http://") || urlString.startsWith("https://")) ? urlString
				: ("http://" + urlString).intern(); // urlString이 http:// or https://로 시작하지 않는 경우, 추가 + intern()은 문자열을 상수로 관리하게 함
		
		URL url = new URL(urlString); // url 객체를 생성 (java.net)
		HttpURLConnection conn = (HttpURLConnection) url.openConnection(); // url로 가서 서버와 연결하는 객체
		
		// 여기서는 conn의 HTTP 요청 헤더에 대한 내용을 설정
		conn.setRequestMethod("GET"); // conn -> GET request
		conn.setRequestProperty("User-Agent", "2019043836/KIMINHO/WEBCLIENT/COMPUTERNETWORK"); // Agent
		conn.setRequestProperty("Accept", "text/html"); //text/html
		conn.setConnectTimeout(timeout); // 연결에서 timeout 생기면 예외  
		try {
			if (conn.getResponseCode() != HttpURLConnection.HTTP_OK) { // Connection -> 응답받은 메세지가 OK가 아닌경우 null
				return null;
			}
		} catch (IOException e) { // 예외정보 출력
			e.printStackTrace();
			return null;
		}
		
		// 서버에 위내용으로 보내고 아래내용으로 받는다
		
		// 이미지를 받는경우
		if (urlString.endsWith(".jpg")) {
		    InputStream input = conn.getInputStream(); // inputStream으로 데이터를 읽어온다
		    BufferedImage image = ImageIO.read(input); // 이미지 읽기
		    
		    BufferedImage convertedImage = new BufferedImage(image.getWidth(), image.getHeight(), BufferedImage.TYPE_INT_RGB);
		    Graphics2D g = convertedImage.createGraphics(); // GUI에 출력하기위해 표준 RGB로 이미지를 변환
		    g.drawImage(image, 0, 0, null);
		    input.close();
		    g.dispose();

		    ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
		    ImageIO.write(convertedImage, "jpg", outputStream); // 이미지를 ByteArrayOutputStream에 쓰기
		    byte[] imageBytes = outputStream.toByteArray();
		    outputStream.close();

		    displayResult(imageBytes); // GUI로 출력하고, 출력했다는 메세지를 리턴
			if(conn != null) { // HTTP 연결을 닫음
				conn.disconnect();
			}
		    return "GUI image\n";
		}
		
		// 텍스트를 받는경우
		InputStream input = conn.getInputStream(); // inputStream으로 데이터를 읽어온다
		BufferedReader reader = new BufferedReader(new InputStreamReader(input, charset));
		String line = null;
		StringBuffer sb = new StringBuffer();
		while ((line = reader.readLine()) != null) { // 반복적으로 읽어옴
			sb.append(line).append("\r\n");
		}
		
		if(reader != null) { // reader를 닫음
			reader.close();
		}
		if(conn != null) { // HTTP 연결을 닫음
			conn.disconnect();
		}
		return sb.toString(); // 서버로부터 받은 결과를 반환
	}
	
	// Head Method로 서버로 보내고 서버로부터 응답을 받음
	public String getWebContentByHead(String urlString, final String charset, int timeout) throws IOException {
		// Get와 같이 url검사 및 url/conn 객체 생성
		if(urlString == null || urlString.length() == 0) {
			return null;
		}
		urlString = (urlString.startsWith("http://") || urlString.startsWith("https://")) ? urlString
				: ("http://" + urlString).intern();
		
		URL url = new URL(urlString);
		HttpURLConnection conn = (HttpURLConnection) url.openConnection();
		
		// 위와 같이 conn의 HTTP 요청 헤더에 대한 내용을 설정
		conn.setRequestMethod("HEAD"); // conn -> GET request
		conn.setRequestProperty("User-Agent", "2019043836/KIMINHO/WEBCLIENT/COMPUTERNETWORK"); // Agent
		conn.setRequestProperty("Accept", "text/html"); //text/html
		conn.setConnectTimeout(timeout); // 연결에서 timeout 생기면 예외  
		try {
			if (conn.getResponseCode() != HttpURLConnection.HTTP_OK) { // Connection -> 응답받은 메세지가 OK가 아닌경우 null
				return null;
			}
		} catch (IOException e) { // 예외정보 출력
			e.printStackTrace();
			return null;
		}
		
		// 서버에 위내용으로 보내고 아래내용으로 받는다
		
		// 헤더라인 텍스트만 받는다 (에러가 있으면, 에러가 출력되고, 없으면, 빈상태)
		InputStream input = conn.getInputStream(); // inputStream으로 데이터를 읽어온다
		BufferedReader reader = new BufferedReader(new InputStreamReader(input, charset));
		String line = null;
		StringBuffer sb = new StringBuffer();
		while ((line = reader.readLine()) != null) { // 반복적으로 읽어옴
			sb.append(line).append("\r\n");
		}
		
		if(reader != null) { // reader를 닫음
			reader.close();
		}
		if(conn != null) { // HTTP 연결을 닫음
			conn.disconnect();
		}
		return sb.toString(); // 서버로부터 받은 결과를 반환
	}
	
	// Post Method로 서버로 보내고 서버로부터 응답을 받음
	public String getWebContentByPost(String urlString, String data, final String charset, int timeout) throws IOException {
		// 위와 똑같이 urlString 검사 및 수정
		if (urlString == null || urlString.length() == 0) {
			return null;
		}
		urlString = (urlString.startsWith("http://") || urlString.startsWith("https//")) ? urlString
				: ("http://" + urlString).intern();
		
		// 위와 똑같이 url 객체와 connection 객체 설정
		URL url = new URL(urlString);
		HttpURLConnection conn = (HttpURLConnection) url.openConnection();
		
		conn.setDoOutput(true); // 서버에 출력스트림으로 데이터 보낼것을 나타냄
		conn.setDoInput(true); // 서버로부터 입력스트림으로 데이터 읽을것임을 나타냄
		conn.setRequestMethod("POST"); // Post Method를 사용할것
		conn.setUseCaches(false); // 캐시사용 비활성화 (서버자체에 데이터를 보내야함)
		conn.setInstanceFollowRedirects(true); // 리다이렉션 활성화 (의도한 서버가 다른 경로를 길을 안내하는 것을 수락)
		
		// GET처럼 정보 설정
		conn.setRequestProperty("Content-Type", "text/xml;charset=UTF-8");
		conn.setRequestProperty("User-Agent", "2019043836/KIMINHO/WEBCLIENT/COMPUTERNETWORK");
		conn.setRequestProperty("Accept", "text/xml");
		conn.setConnectTimeout(timeout);
		
		// 서버와 연결
		conn.connect();
		
		// 아웃풋스트림으로 서버에 data를 전송한다
		DataOutputStream out = new DataOutputStream(conn.getOutputStream());
		byte[] content = data.getBytes("UTF-8"); // 데이터를 변환	
		out.write(content); // content를 스트림에 작성
		out.flush(); // 스트림의 모든 데이터를 서버로 전송
		out.close(); // 출력 스트림을 닫음
		
		// 서버에 위내용으로 보내고 아래내용으로 받는다
		
		// 위처럼 서버로부터 메세지 받고, 결과를 리턴
		InputStream input = conn.getInputStream();
		BufferedReader reader = new BufferedReader(new InputStreamReader(input, charset));
		String line = null;
		StringBuffer sb = new StringBuffer();
		while ((line = reader.readLine()) != null) {
			sb.append(line).append("\r\n");
		}
		
		if(reader != null) { // reader를 닫음
			reader.close();
		}
		if(conn != null) { // HTTP 연결을 닫음
			conn.disconnect();
		}
		
		return sb.toString(); // 서버로부터 받은 결과를 반환
	}
	
	// GUI를 통한 이미지 판단
	public static void displayResult(byte[] result) {
	    JFrame frame = new JFrame("Result Display"); // 새로운 GUI 프레임 생성
        
	    ImageIcon imageIcon = new ImageIcon(result); // result를 이미지로 담고
        Image image = imageIcon.getImage();
        Image scaledImage = image.getScaledInstance(400, 300, Image.SCALE_SMOOTH);
        ImageIcon scaledImageIcon = new ImageIcon(scaledImage); // 이미지의 스케일을 조정
        JLabel label = new JLabel(scaledImageIcon);
        frame.getContentPane().add(new JScrollPane(label)); // 조정된 이미지에 맞게 창의 스크롤을 조정할 수 있게 함
	    
	    frame.setSize(400, 300); // 창의 크기를 설정
	    frame.setLocationRelativeTo(null); // 창을 화면 중앙에 위치시키기
	    frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // 창을 닫으면 프로그램을 종료
        frame.setVisible(true); // 창을 화면에 나타나게 함
	}
}

