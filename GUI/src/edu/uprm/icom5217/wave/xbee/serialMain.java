package edu.uprm.icom5217.wave.xbee;
import gnu.io.PortInUseException;
import gnu.io.UnsupportedCommOperationException;

import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;
import java.util.TooManyListenersException;


public class serialMain {

	public static void main(String[] args) throws IOException, PortInUseException, UnsupportedCommOperationException, TooManyListenersException{
		SerialCommunication serial = new SerialCommunication();

		serial.openSerialPort("COM7", 9600);

		InputStream inputStream = serial.getInputStream();
		PrintStream outputStream = serial.getOutputStream();

		//do stuff
		byte[] b = new byte[2];
		outputStream.print("B");
		outputStream.print("\r\n");
		
		while(true);
		
//		while(!serial.isDataAvailable());
//		
//		int num = inputStream.read(b);
//		System.out.println(num);
//		String s2 = new String(b,"US-ASCII");
//		System.out.println(s2);


		//serial.close();
	}
}
