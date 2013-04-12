package edu.uprm.icom5217.wave.view.diagnostic;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;

import net.miginfocom.swing.MigLayout;
import edu.uprm.icom5217.wave.WaveSphere;
import edu.uprm.icom5217.wave.xbee.XBee;

public class DiagnosticWindow extends JDialog {
	
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
	
	private static DiagnosticWindow INSTANCE;
	
	public DiagnosticWindow() {
		setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		setResizable(false);
		setTitle("Diagnosis Of Sphere 1");
		getContentPane().setLayout(new MigLayout("", "[46.00][156.00,grow][118.00][46.00]", "[][][][][][][][][][]"));
		getContentPane().add(getBatteryLabel(), "cell 1 0");
		getContentPane().add(getBatteryValueLabel(), "cell 2 0");
		getContentPane().add(getMemoryLabel(), "cell 1 1");
		getContentPane().add(getMemoryValueLabel(), "cell 2 1");
		getContentPane().add(getLocationLabel(), "cell 1 2");
		getContentPane().add(getLocationValueLabel(), "cell 2 2");
		getContentPane().add(getWirelessLabel(), "cell 1 3");
		getContentPane().add(getWirelssValueLabel(), "cell 2 3");
		getContentPane().add(getAccelerationLabel(), "cell 1 4");
		getContentPane().add(getAccelerationValueLabel(), "cell 2 4");
		getContentPane().add(getGyroLabel(), "cell 1 5");
		getContentPane().add(getGyroValueLabel(), "cell 2 5");
		getContentPane().add(getMagenticLabel(), "cell 1 6");
		getContentPane().add(getMagneticValueLabel(), "cell 2 6");
		getContentPane().add(getStopButton(), "cell 0 9 4 1,alignx center");
		pack();
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
			magenticLabel = new JLabel("Magentic:");
			magenticLabel.setName("magenticLabel");
		}
		return magenticLabel;
	}
	private JLabel getBatteryValueLabel() {
		if (batteryValueLabel == null) {
			batteryValueLabel = new JLabel("60%");
			batteryValueLabel.setName("batteryValueLabel");
		}
		return batteryValueLabel;
	}
	private JLabel getMemoryValueLabel() {
		if (memoryValueLabel == null) {
			memoryValueLabel = new JLabel("30%");
			memoryValueLabel.setName("memoryValueLabel");
		}
		return memoryValueLabel;
	}
	private JLabel getLocationValueLabel() {
		if (locationValueLabel == null) {
			locationValueLabel = new JLabel("157N 20W");
			locationValueLabel.setName("locationValueLabel");
		}
		return locationValueLabel;
	}
	private JLabel getWirelssValueLabel() {
		if (wirelssValueLabel == null) {
			wirelssValueLabel = new JLabel("OK");
			wirelssValueLabel.setName("wirelssValueLabel");
		}
		return wirelssValueLabel;
	}
	private JLabel getAccelerationValueLabel() {
		if (accelerationValueLabel == null) {
			accelerationValueLabel = new JLabel("0 g");
			accelerationValueLabel.setName("accelerationValueLabel");
		}
		return accelerationValueLabel;
	}
	private JLabel getGyroValueLabel() {
		if (gyroValueLabel == null) {
			gyroValueLabel = new JLabel("0\u00B0");
			gyroValueLabel.setName("gyroValueLabel");
		}
		return gyroValueLabel;
	}
	private JLabel getMagneticValueLabel() {
		if (magneticValueLabel == null) {
			magneticValueLabel = new JLabel("0");
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
					WaveSphere.serial.write(XBee.lang.STOP_DIAGNOSTIC_MODE);
					dispose();
					
				}
				
			});
		}
		return stopButton;
	}
	
	private static DiagnosticWindow getInstance(){
		if(INSTANCE == null){
			INSTANCE = new DiagnosticWindow();
		}
		
		return INSTANCE;
	}
	
	public static void display(String sphereId){
		getInstance().setVisible(true);
	}
}
