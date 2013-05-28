package com.dingvyoung;
import java.awt.Container;
import java.awt.Font;
import java.awt.TextArea;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.Box;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;

import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.CookieStore;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.params.ClientPNames;
import org.apache.http.client.params.CookiePolicy;
import org.apache.http.client.protocol.ClientContext;
import org.apache.http.impl.client.BasicCookieStore;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.protocol.BasicHttpContext;
import org.apache.http.protocol.HTTP;
import org.apache.http.protocol.HttpContext;
import org.apache.http.util.EntityUtils;

public class main {
	private static DefaultHttpClient client = new DefaultHttpClient();
	private static CookieStore cookie = new BasicCookieStore();
	private static HttpContext context = new BasicHttpContext();
	private static String postURL = "http://ictclas.nlpir.org/onlineputong";
	private static String text = null;
	private static TextArea textArea = null;
	
	public static void main(String[] args) {
		//text = args[0];
		BoxLayoutFrame frame = new BoxLayoutFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
		
	}
	
	static class getContext extends Thread{
		public void run(){
			HttpPost apost = new HttpPost(postURL);
			context.setAttribute(ClientContext.COOKIE_STORE, cookie);
			client.getParams().setParameter(ClientPNames.ALLOW_CIRCULAR_REDIRECTS, true);
			client.getParams().setParameter(ClientPNames.COOKIE_POLICY,CookiePolicy.BROWSER_COMPATIBILITY);
			List <NameValuePair> params = new ArrayList<NameValuePair>();
			params.add(new BasicNameValuePair("name1",text));
			HttpResponse response = null;
			try{
				//apost.setEntity(new UrlEncodedFormEntity(params));
				apost.setEntity(new UrlEncodedFormEntity(params,HTTP.UTF_8));
				response = client.execute(apost,context);
				String responseStr = null;
				String resultStr = null;
				if(response.getStatusLine().getStatusCode()==200){
					//如果接收到的不是utf-8编码
					//String str = EntityUtils.toString(response.getEntity());
					//responseStr = new String(str.getBytes("gbk"),"utf-8");
					responseStr = EntityUtils.toString(response.getEntity());
					//System.out.println(responseStr);
					Pattern pattern = Pattern.compile("id=\"shuchu\">(.*)</textarea>");
					Matcher matcher = pattern.matcher(responseStr);
					if(matcher.find()){
					  if(matcher.group(1)!=null){
						  resultStr = matcher.group(1);
						  OutputStreamWriter outputStream = new OutputStreamWriter(new FileOutputStream("untitled"), "UTF-8");  
						  outputStream.write(resultStr, 0, resultStr.length()-1);
						  outputStream.close();
						  Runtime.getRuntime().exec("sh run.sh");
					  }
					}
					else{
						System.out.println("no match");
					}
					//System.out.println(resultStr);
				}
			}
			 catch (UnsupportedEncodingException e2) {
				e2.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	static class BoxLayoutFrame extends JFrame{
		private static final int WIDTH = 800;
		private static final int HEIGHT = 600;
		public BoxLayoutFrame(){
			this.setTitle("文章主题识别方法研究");
			this.setSize(WIDTH,HEIGHT);
			Container con=getContentPane();
			JLabel label_1=new JLabel("文本输入:");
			label_1.setFont(new Font("Serif",Font.BOLD,20));
			textArea = new TextArea();
			JButton button1=new JButton("OK");
			
			Box box_1=Box.createVerticalBox();//水平箱子
			box_1.add(Box.createVerticalStrut(20));//20个不可见额间隔
			box_1.add(label_1);
			box_1.add(textArea);
			box_1.add(Box.createVerticalStrut(20));//20个不可见额间隔
			box_1.add(button1);
			box_1.add(Box.createVerticalStrut(20));//20个不可见额间隔
			con.add(box_1);
			button1.addActionListener(new ButtonListener());
		}
		class ButtonListener implements ActionListener{

			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				text = textArea.getText().replaceAll("　","");
				getContext g1 = new getContext();
				g1.start();
			}
			
		}
	}
}
