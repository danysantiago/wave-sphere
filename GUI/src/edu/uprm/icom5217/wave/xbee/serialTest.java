package edu.uprm.icom5217.wave.xbee;

import gnu.io.CommPortIdentifier;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;
import gnu.io.UnsupportedCommOperationException;

import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;
import java.util.Enumeration;
import java.util.TooManyListenersException;


public class serialTest implements SerialPortEventListener {

	private  InputStream inputStream;
	private  PrintStream outputStream;

	private  SerialPort serialPort;

	private boolean dataAvailable = false;

	public serialTest(){

	}	

	public  void openSerialPort(String port, int baudRate) throws PortInUseException, UnsupportedCommOperationException, TooManyListenersException, IOException {
		openSerialPort(port, "XBee", 0, baudRate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE, SerialPort.FLOWCONTROL_NONE);
	}

	public  void openSerialPort(String port, String appName, int timeout, int baudRate) throws PortInUseException, UnsupportedCommOperationException, TooManyListenersException, IOException {
		openSerialPort(port, appName, timeout, baudRate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE, SerialPort.FLOWCONTROL_NONE);
	}

	@SuppressWarnings("unchecked")
	public  void openSerialPort(String port, String appName, int timeout, int baudRate, int dataBits, int stopBits, int parity, int flowControl) throws PortInUseException, UnsupportedCommOperationException, TooManyListenersException, IOException{
		// Apparently you can't query for a specific port, but instead must iterate
		Enumeration<CommPortIdentifier> portList = CommPortIdentifier.getPortIdentifiers();

		CommPortIdentifier portId = null;

		boolean found = false;

		while (portList.hasMoreElements()) {

			portId = portList.nextElement();

			if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {

				//log.debug("Found port: " + portId.getName());

				if (portId.getName().equals(port)) {
					//log.debug("Using Port: " + portId.getName());
					found = true;
					break;
				}
			}
		}

		if (!found) {
			System.out.println("Could not find port: " + port);
			System.exit(1);
		}


		serialPort = (SerialPort) portId.open(appName, timeout);

		serialPort.setSerialPortParams(baudRate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
		serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);

		// activate the DATA_AVAILABLE notifier
		serialPort.notifyOnDataAvailable(true);

		// activate the OUTPUT_BUFFER_EMPTY notifier
		//serialPort.notifyOnOutputEmpty(true);

		serialPort.addEventListener(this);

		inputStream = serialPort.getInputStream();
		outputStream = new PrintStream(serialPort.getOutputStream(), true);
	}

	/**
	 * Shuts down RXTX
	 */
	public void close() {
		try {
			serialPort.getInputStream().close();
		} catch (Exception e) {
			System.out.println("Exception while closing input stream");
		}

		try {
			serialPort.getOutputStream().close();
		} catch (Exception e) {
			System.out.println("Exception while closing output stream");
		}

		try {
			// this call blocks while thread is attempting to read from inputstream
			serialPort.close();
		} catch (Exception e) {
			System.out.println("Exception while closing serial port");
		}
	}

	public PrintStream getOutputStream() {
		return outputStream;
	}

	public InputStream getInputStream() {
		return inputStream;
	}

	public boolean isDataAvailable() {
		return dataAvailable;
	}

	public void toggleDataAvailable() {
		this.dataAvailable = !dataAvailable;
	}

	public void serialEvent(SerialPortEvent event){
		switch (event.getEventType()){
			case SerialPortEvent.DATA_AVAILABLE:
				getDataAvailable(event);
				break;

			default:
				System.out.println("Fuck you.");
				break;
		}
	}

	private void getDataAvailable(SerialPortEvent event) {
		// TODO Auto-generated method stub
		try{
		if(this.getInputStream().available() > 0){
			for(int i = 0; i < this.getInputStream().available(); i++){
				System.out.print((char)this.getInputStream().read());
			}
		}}catch(Exception e){
			
		}
	}

	//	public void serialEvent(SerialPortEvent event) {
	//		switch (event.getEventType()) {	
	//			case SerialPortEvent.DATA_AVAILABLE:
	//
	//				try {
	//					if (this.getInputStream().available() > 0) {
	//						try {
	//							System.out.println("serialEvent: " + serialPort.getInputStream().available() + " bytes available");
	//							synchronized (this) {
	//								this.notify();										
	//							}
	//						} catch (Exception e) {
	//							System.out.println("Error in handleSerialData method");
	//						}				
	//					} else {
	//						System.out.println("We were notified of new data but available() is returning 0");
	//					}
	//				} catch (IOException ex) {
	//					// it's best not to throw the exception because the RXTX thread may not be prepared to handle
	//					System.out.println("RXTX error in serialEvent method");
	//				}
	//			default:
	//				System.out.println("Ignoring serial port event type: " + event.getEventType());
	//		}		
	//	}
}
