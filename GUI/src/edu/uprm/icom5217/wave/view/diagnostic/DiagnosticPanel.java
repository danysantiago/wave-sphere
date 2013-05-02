package edu.uprm.icom5217.wave.view.diagnostic;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;

import net.miginfocom.swing.MigLayout;
import edu.uprm.icom5217.wave.WaveSphere;
import edu.uprm.icom5217.wave.view.MainWindow;
import edu.uprm.icom5217.wave.xbee.Xbee;

public class DiagnosticPanel extends JPanel {
	
	private static final long serialVersionUID = -5722644987144387448L;
	private JLabel batteryLabel;
	private JLabel memoryLabel;
	private JLabel locationLabel;
	private JLabel wirelessLabel;
	private JLabel accelerationLabel;
	private JLabel gyroLabel;
	private JLabel magenticLabel;
	private JButton stopButton;
	private JLabel batteryValueLabel;
	private JLabel memoryValueLabel;
	private JLabel locationValueLabel;
	private JLabel wirelssValueLabel;
	private JLabel accelerationValueLabel;
	private JLabel gyroValueLabel;
	private JLabel magneticValueLabel;
	
	private static DiagnosticPanel INSTANCE;
	
	public DiagnosticPanel() {
		setLayout(new MigLayout("", "[46.00][156.00,grow][118.00][46.00]", "[][][][][][][][][][]"));
		add(getBatteryLabel(), "cell 1 0");
		add(getBatteryValueLabel(), "cell 2 0");
		add(getMemoryLabel(), "cell 1 1");
		add(getMemoryValueLabel(), "cell 2 1");
		add(getLocationLabel(), "cell 1 2");
		add(getLocationValueLabel(), "cell 2 2");
		add(getWirelessLabel(), "cell 1 3");
		add(getWirelssValueLabel(), "cell 2 3");
		add(getAccelerationLabel(), "cell 1 4");
		add(getAccelerationValueLabel(), "cell 2 4");
		add(getGyroLabel(), "cell 1 5");
		add(getGyroValueLabel(), "cell 2 5");
		add(getMagenticLabel(), "cell 1 6");
		add(getMagneticValueLabel(), "cell 2 6");
		add(getStopButton(), "cell 0 9 4 1,alignx center");
	}
	private JLabel getBatteryLabel() {
		if (batteryLabel == null) {
			batteryLabel = new JLabel("Battery:");
			batteryLabel.setName("batteryLabel");
		}
		return batteryLabel;
	}
	private JLabel getMemoryLabel() {
		if (memoryLabel == null) {
			memoryLabel = new JLabel("Memory:");
			memoryLabel.setName("memoryLabel");
		}
		return memoryLabel;
	}
	private JLabel getLocationLabel() {
		if (locationLabel == null) {
			locationLabel = new JLabel("Location:");
			locationLabel.setName("locationLabel");
		}
		return locationLabel;
	}
	private JLabel getWirelessLabel() {
		if (wirelessLabel == null) {
			wirelessLabel = new JLabel("Wireless:");
			wirelessLabel.setName("wirelessLabel");
		}
		return wirelessLabel;
	}
	private JLabel getAccelerationLabel() {
		if (accelerationLabel == null) {
			accelerationLabel = new JLabel("Acceleration:");
			accelerationLabel.setName("accelerationLabel");
		}
		return accelerationLabel;
	}
	private JLabel getGyroLabel() {
		if (gyroLabel == null) {
			gyroLabel = new JLabel("Angle:");
			gyroLabel.setName("gyroLabel");
		}
		return gyroLabel;
	}
	private JLabel getMagenticLabel() {
		if (magenticLabel == null) {
			magenticLabel = new JLabel("Magnetic:");
			magenticLabel.setName("magenticLabel");
		}
		return magenticLabel;
	}
	private JLabel getBatteryValueLabel() {
		if (batteryValueLabel == null) {
			batteryValueLabel = new JLabel("");
			batteryValueLabel.setName("batteryValueLabel");
		}
		return batteryValueLabel;
	}
	private JLabel getMemoryValueLabel() {
		if (memoryValueLabel == null) {
			memoryValueLabel = new JLabel("");
			memoryValueLabel.setName("memoryValueLabel");
		}
		return memoryValueLabel;
	}
	private JLabel getLocationValueLabel() {
		if (locationValueLabel == null) {
			locationValueLabel = new JLabel("");
			locationValueLabel.setName("locationValueLabel");
		}
		return locationValueLabel;
	}
	private JLabel getWirelssValueLabel() {
		if (wirelssValueLabel == null) {
			wirelssValueLabel = new JLabel("");
			wirelssValueLabel.setName("wirelssValueLabel");
		}
		return wirelssValueLabel;
	}
	private JLabel getAccelerationValueLabel() {
		if (accelerationValueLabel == null) {
			accelerationValueLabel = new JLabel("");
			accelerationValueLabel.setName("accelerationValueLabel");
		}
		return accelerationValueLabel;
	}
	private JLabel getGyroValueLabel() {
		if (gyroValueLabel == null) {
			gyroValueLabel = new JLabel("");
			gyroValueLabel.setName("gyroValueLabel");
		}
		return gyroValueLabel;
	}
	private JLabel getMagneticValueLabel() {
		if (magneticValueLabel == null) {
			magneticValueLabel = new JLabel("");
			magneticValueLabel.setName("magneticValueLabel");
		}
		return magneticValueLabel;
	}
	
	private JButton getStopButton() {
		if (stopButton == null) {
			stopButton = new JButton("Stop Diagnosis");
			stopButton.setName("stopButton");
			stopButton.addActionListener(new ActionListener(){

				@Override
				public void actionPerformed(ActionEvent e) {
					WaveSphere.serial.write(Xbee.STOP_DIAGNOSTIC_MODE);
					WaveSphere.serial.setFlag(Xbee.STATUS_MODE);
					MainWindow.normalMode();
					MainWindow.getInstance().pack();
					MainWindow.getInstance().pack();
				}
				
			});
		}
		return stopButton;
	}
	
	public static DiagnosticPanel getInstance(){
		if(INSTANCE == null){
			INSTANCE = new DiagnosticPanel();
		}
		
		return INSTANCE;
	}
	
	public static void display(String sphereId){
		getInstance().setVisible(true);
	}
	public void setBatteryValueLabel(String s) {
		batteryValueLabel.setText(s);
	}
	public void setMemoryValueLabel(String s) {
		memoryValueLabel.setText(s);
	}
	public void setLocationValueLabel(String s) {
		locationValueLabel.setText(s);
	}
	public void setWirelssValueLabel(String s) {
		wirelssValueLabel.setText(s);
	}
	public void setAccelerationValueLabel(String s) {
		accelerationValueLabel.setText(s);
	}
	public void setGyroValueLabel(String s) {
		gyroValueLabel.setText(s);
	}
	public void setMagneticValueLabel(String s) {
		magneticValueLabel.setText(s);
	}
	
	
}
