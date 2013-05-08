package edu.uprm.icom5217.wave.xbee;


import edu.uprm.icom5217.wave.WaveSphere;
import edu.uprm.icom5217.wave.model.Sphere;
import edu.uprm.icom5217.wave.model.SphereList;
import edu.uprm.icom5217.wave.utils.SampleFile;
import edu.uprm.icom5217.wave.utils.SensorDataConversion;
import edu.uprm.icom5217.wave.view.LocatePanel;
import edu.uprm.icom5217.wave.view.MainWindow;
import edu.uprm.icom5217.wave.view.RightPanel2;
import edu.uprm.icom5217.wave.view.diagnostic.DiagnosticPanel;
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

import javax.swing.JOptionPane;


public class SerialCommunication implements SerialPortEventListener {

	private char EOF = '\0';

	public Xbee flag;

	private  InputStream inputStream;
	private  PrintStream outputStream;

	private  SerialPort serialPort;

	private StringBuilder sb;

	private SampleFile f;

	public SerialCommunication() throws IOException{
		sb = new StringBuilder();
		flag = Xbee.STATUS_MODE;
	}	

	public void setFile(SampleFile f){
		this.f = f;
	}

	public void setFlag(Xbee command) {
		this.flag = command;
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
					found = true;
					break;
				}
			}
		}

		if (!found) {
			throw new NullPointerException();
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
			//System.out.println("Exception while closing input stream");
		}

		try {
			serialPort.getOutputStream().close();
		} catch (Exception e) {
			//System.out.println("Exception while closing output stream");
		}

		try {
			// this call blocks while thread is attempting to read from inputstream
			serialPort.close();
		} catch (Exception e) {
			//System.out.println("Exception while closing serial port");
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
			break;
		}
	}

	private void getDataAvailable(SerialPortEvent event) {
		try{
			if(this.getInputStream().available() > 0){
				for(int i = 0; i < this.getInputStream().available(); i++){
					char c = (char)this.getInputStream().read();

					switch(flag){
					case STATUS_MODE:
						statusMode(c);
						break;

					case RETRIEVAL_MODE:
						retrievalMode(c);
						break;

					case SAMPLING_MODE:
						samplingMode(c);
						break;

					case LOCATE_MODE:
						samplingMode(c);
						break;

					case DIAGNOSTIC_MODE:
						diagnosticMode(c);
						break;

					default:
						break;
					}
				}
			}
		}catch(Exception e){
		}
	}

	private void retrievalMode(char c) throws IOException {
		sb.append(c);
		if(c=='\n'){

			String s = sb.toString();

			try {

				String[] data = s.split("\t");
				String time = data[0];
				String[] acc = data[1].split(",");
				String[] gyro = data[2].split(",");
				String[] mag = data[3].split(",");

				double[] accData = SensorDataConversion.convertAccData(acc);
				double[] gyrData = SensorDataConversion.convertGyrData(gyro);
				double[] magData = SensorDataConversion.convertMagData(mag);

				f.writeToFile(time + ", ");

				f.writeToFile((SensorDataConversion.AXIS_LABEL[0] + ": " + String.format("%.3f", accData[0]) + ", ")
						+ (SensorDataConversion.AXIS_LABEL[1] + ": " + String.format("%.3f", accData[1]) + " , ")
						+ (SensorDataConversion.AXIS_LABEL[2] + ": " + String.format("%.3f", accData[2]) + ","));

				f.writeToFile((SensorDataConversion.AXIS_LABEL[0] + ": " + String.format("%.3f", gyrData[0]) + ",")
						+ (SensorDataConversion.AXIS_LABEL[1] + ": " + String.format("%.3f", gyrData[1]) + ",")
						+ (SensorDataConversion.AXIS_LABEL[2] + ": " + String.format("%.3f", gyrData[2]) + ","));


				f.writeToFile((SensorDataConversion.AXIS_LABEL[0] + ": " + String.format("%.3f", magData[0]) + ",")
						+ (SensorDataConversion.AXIS_LABEL[1] + ": " + String.format("%.3f", magData[1]) + ",")
						+ (SensorDataConversion.AXIS_LABEL[2] + ": " + String.format("%.3f", magData[2]) + "\n"));
			} catch(Exception e) {

			}
			sb = new StringBuilder();
		}


		if(c == EOF){
			f.flush();
			WaveSphere.serial.setFlag(Xbee.STATUS_MODE);
			MainWindow.normalMode();
			JOptionPane.showMessageDialog(null, "File Saved.");
		}
	}

	private void diagnosticMode(char c) throws IOException {
		sb.append(c);
		if(c=='\n'){

			String s = sb.toString();

			if(s.contains("$")){
				if(s.contains("$GPRMC")){
					//if(!s.contains("V")){
					String[] st = s.split(",");
					s = (s.contains("S")? "-" : "") 
							+ (st[3].length()>0? (st[3].substring(0, 2) + "." 
									+ Float.toString(Float.parseFloat(st[3].substring(2))/60)) : "Invalid data") + st[4] + ", "
									+ (s.contains("W")? "-" : "")
									+ (st[5].length()>0? (st[5].substring(0,3) + "." 
											+ Float.toString(Float.parseFloat(st[5].substring(3))/60)) : "no GPS signal") + st[6];

					DiagnosticPanel.getInstance().setLocationValueLabel(s);
					//}
				}
			}
			else if(s.contains("dB")){
				DiagnosticPanel.getInstance().setWirelssValueLabel("-" + Long.parseLong(s.substring(0,2),16) + "dB");
				}
			else if(s.contains("%b"))
				DiagnosticPanel.getInstance().setBatteryValueLabel(s.substring(0,s.length()-2));
			else if(s.contains("%m"))
				DiagnosticPanel.getInstance().setMemoryValueLabel(s.substring(0,s.length()-2));

			else{

				try {

					String[] data = s.split("\t");
					String[] acc = data[0].split(",");
					String[] gyro = data[1].split(",");
					String[] mag = data[2].split(",");

					double[] accData = SensorDataConversion.convertAccData(acc);
					double[] gyrData = SensorDataConversion.convertGyrData(gyro);
					double[] magData = SensorDataConversion.convertMagData(mag);


					DiagnosticPanel.getInstance().setAccelerationValueLabel((SensorDataConversion.AXIS_LABEL[0] + ": " + String.format("%.3f", accData[0]) + " g, ")
							+ (SensorDataConversion.AXIS_LABEL[1] + ": " + String.format("%.3f", accData[1]) + " g, ")
							+ (SensorDataConversion.AXIS_LABEL[2] + ": " + String.format("%.3f", accData[2]) + " g"));

					DiagnosticPanel.getInstance().setGyroValueLabel((SensorDataConversion.AXIS_LABEL[0] + ": " + String.format("%.3f", gyrData[0]) + " dps, ")
							+ (SensorDataConversion.AXIS_LABEL[1] + ": " + String.format("%.3f", gyrData[1]) + " dps, ")
							+ (SensorDataConversion.AXIS_LABEL[2] + ": " + String.format("%.3f", gyrData[2]) + " dps"));


					DiagnosticPanel.getInstance().setMagneticValueLabel((SensorDataConversion.AXIS_LABEL[0] + ": " + String.format("%.3f", magData[0]) + " gauss, ")
							+ (SensorDataConversion.AXIS_LABEL[1] + ": " + String.format("%.3f", magData[1]) + " gauss, ")
							+ (SensorDataConversion.AXIS_LABEL[2] + ": " + String.format("%.3f", magData[2]) + " gauss"));
				} catch(Exception e) {

				}

			}
			sb = new StringBuilder();

		}
	}

	private void samplingMode(char c) {
		sb.append(c);
		if(c=='\n'){
			String s = sb.toString();
			if(s.contains("$GPRMC")){
				//if(!s.contains("V")){
				String[] st = s.split(",");
				s = (s.contains("S")? "-" : "") 
						+ (st[3].length()>0? (st[3].substring(0, 2) + "." 
								+ Float.toString(Float.parseFloat(st[3].substring(2))/60)) : "00.0000") + st[4] + ", "
								+ (s.contains("W")? "-" : "")
								+ (st[5].length()>0? (st[5].substring(0,3) + "." 
										+ Float.toString(Float.parseFloat(st[5].substring(3))/60)) : "000.0000") + st[6];

				LocatePanel.getInstance().setLabel(s);

				//}
			}
			sb = new StringBuilder();
		}

	}

	private void statusMode(char c) {
		sb.append(c);
		if(c=='\n'){

			String s = sb.toString();


			if(s.contains("%m"))
				RightPanel2.getInstance().setMbLabel(s.substring(0, s.length()-2));
			else if(s.matches("[0-9]{2}%b\\n")){
				RightPanel2.getInstance().setBatteryLabel(s.substring(0, s.length()-2));
				}
			{
				if(s.matches("[0-9]{3}-[0-9]{4}\\n"))
					RightPanel2.getInstance().setBolaIdLabel(s);
			}
			sb = new StringBuilder();
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

	public void write(Xbee command){
		if(!SphereList.getInstance().isEmpty()){
			outputStream.print(SphereList.getInstance().getSelected().getId() + " " + command.getCommand());
			outputStream.flush();
		}
	}

	public void write(int data){
		outputStream.write(data);
		outputStream.flush();

	}

	public void write(String s) {
		outputStream.print(s);
		outputStream.flush();
	}

	public void write(String string, Xbee command){
		outputStream.print(string + " " + command.getCommand());

		outputStream.flush();
	}

}
