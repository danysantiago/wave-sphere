package edu.uprm.icom5217.wave.view;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;

import net.miginfocom.swing.MigLayout;
import edu.uprm.icom5217.wave.WaveSphere;
import edu.uprm.icom5217.wave.view.diagnostic.DiagnosticWindow;
import edu.uprm.icom5217.wave.xbee.XBee;

public class RightPanel2 extends JPanel {
	private JLabel idLabel;
	private JButton retrievalModeButton;
	private JButton diagnosticButton;
	private JLabel bolaIdLabel;
	private JButton samplingModeButton;
	private JLabel batterLevelLabel;
	private JLabel label;
	private JLabel memoryAvailableLabel;
	private JLabel mbLabel;
	private JButton turnOffButton;
	public RightPanel2() {
		setLayout(new MigLayout("fill", "[36.00,grow,center]", "[]5[]5[]5[]5[]5[][]"));
		add(getIdLabel(), "flowx,cell 0 0,alignx center");
		add(getBatterLevelLabel(), "flowx,cell 0 1");
		add(getMemoryAvailableLabel(), "flowx,cell 0 2");
		add(getRetrievalModeButton(), "cell 0 3");
		add(getSamplingModeButton(), "cell 0 4");
		add(getDiagnosticButton(), "cell 0 5");
		add(getBolaIdLabel(), "cell 0 0");
		add(getLabel(), "cell 0 1");
		add(getMbLabel(), "cell 0 2");
		add(getTurnOffButton(), "cell 0 6,alignx right");
	}


	private JLabel getIdLabel() {
		if (idLabel == null) {
			idLabel = new JLabel("ID:   ");
			idLabel.setName("idLabel");
		}
		return idLabel;
	}
	private JButton getRetrievalModeButton() {
		if (retrievalModeButton == null) {
			retrievalModeButton = new JButton("Retrieval Mode");
			retrievalModeButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent arg0) {
					WaveSphere.serial.write(XBee.lang.RETRIEVAL_MODE);
					MainWindow.retrievalMode();
				}
			});
			retrievalModeButton.setName("retrievalModeButton");
		}
		return retrievalModeButton;
	}
	private JButton getDiagnosticButton() {
		if (diagnosticButton == null) {
			diagnosticButton = new JButton("Diagnostic Mode");
			diagnosticButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent arg0) {
					WaveSphere.serial.write(XBee.lang.DIAGNOSTIC_MODE);
					DiagnosticWindow.display("");
				}
			});
			diagnosticButton.setName("diagnosticButton");
		}
		return diagnosticButton;
	}
	private JLabel getBolaIdLabel() {
		if (bolaIdLabel == null) {
			WaveSphere.serial.write(XBee.lang.ID);
			//TODO do something to wait for id
			bolaIdLabel = new JLabel("827345980236");
			bolaIdLabel.setName("bolaIdLabel");
		}
		return bolaIdLabel;
	}
	private JButton getSamplingModeButton() {
		if (samplingModeButton == null) {
			samplingModeButton = new JButton("Sampling Mode");
			samplingModeButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					WaveSphere.serial.write(XBee.lang.SAMPLING_MODE);
					MainWindow.samplingMode();
				}
			});
			samplingModeButton.setName("samplingModeButton");
		}
		return samplingModeButton;
	}
	private JLabel getBatterLevelLabel() {
		if (batterLevelLabel == null) {
			batterLevelLabel = new JLabel("Battery Level : ");
			batterLevelLabel.setName("batterLevelLabel");
		}
		return batterLevelLabel;
	}
	private JLabel getLabel() {
		if (label == null) {
			label = new JLabel("60%");
			label.setName("label");
		}
		return label;
	}
	private JLabel getMemoryAvailableLabel() {
		if (memoryAvailableLabel == null) {
			memoryAvailableLabel = new JLabel("Memory Available:  ");
			memoryAvailableLabel.setName("memoryAvailableLabel");
		}
		return memoryAvailableLabel;
	}
	private JLabel getMbLabel() {
		if (mbLabel == null) {
			mbLabel = new JLabel("105MB");
			mbLabel.setName("mbLabel");
		}
		return mbLabel;
	}
	private JButton getTurnOffButton() {
		if (turnOffButton == null) {
			turnOffButton = new JButton("Turn Off");
			turnOffButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					if(getTurnOffButton().getText().contains("Off")){
						WaveSphere.serial.write(XBee.lang.SHUTDOWN_MODE);
						getTurnOffButton().setText("Turn On");
					}
					else{
						//TODO send RF signal
						getTurnOffButton().setText("Turn Off");
					}

				}
			});
			turnOffButton.setName("turnOffButton");
		}
		return turnOffButton;
	}
}
