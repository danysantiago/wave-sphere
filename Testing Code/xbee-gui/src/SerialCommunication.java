

import gnu.io.CommPortIdentifier;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;
import gnu.io.UnsupportedCommOperationException;

import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.TooManyListenersException;


public class SerialCommunication implements SerialPortEventListener {

	private  InputStream inputStream;
	private  PrintStream outputStream;

	private  SerialPort serialPort;

	public SerialCommunication(){

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

	public void serialEvent(SerialPortEvent event){
		switch (event.getEventType()){
		case SerialPortEvent.DATA_AVAILABLE:
			getDataAvailable(event);
			break;

		default:
			System.out.println("Ignored event: " + event.getEventType());
			break;
		}
	}

	private void getDataAvailable(SerialPortEvent event) {
		try{
			if(this.getInputStream().available() > 0){
				for(int i = 0; i < this.getInputStream().available(); i++){
					char c = (char)this.getInputStream().read();
					//System.out.print(c);
					serialWindow.printToTextArea(c);
				}
			}
			}catch(Exception e){
				System.out.println("Error getting data");
			}
	}
	
	@SuppressWarnings("unchecked")
	public ArrayList<String> getSerialPorts(){
		ArrayList<String> serialPorts = new ArrayList<String>();
		
		Enumeration<CommPortIdentifier> portList = CommPortIdentifier.getPortIdentifiers();

		CommPortIdentifier portId = null;


		while (portList.hasMoreElements()) {

			portId = portList.nextElement();

			if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {

				serialPorts.add(portId.getName());

			}
		}
		
		return serialPorts;
	}
	
	public void write(String data){
		outputStream.print(data);
		//outputStream.print("\r\n");
		outputStream.flush();
	}
	
	public void write(int data){
		outputStream.write(data);
		outputStream.flush();

	}

}
