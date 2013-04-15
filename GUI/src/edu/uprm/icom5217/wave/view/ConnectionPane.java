package edu.uprm.icom5217.wave.view;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.TooManyListenersException;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;

import net.miginfocom.swing.MigLayout;
import edu.uprm.icom5217.wave.WaveSphere;
import edu.uprm.icom5217.wave.model.CommPortModel;
import gnu.io.PortInUseException;
import gnu.io.UnsupportedCommOperationException;

public class ConnectionPane extends JPanel {
	private JLabel pleaseSelectALabel;
	private JComboBox comboBox;
	private JButton connectButton;
	public ConnectionPane() {
		setLayout(new MigLayout("", "[c,grow]", "[c][c]"));
		add(getPleaseSelectALabel(), "flowx,cell 0 0");
		add(getComboBox(), "cell 0 0");
		add(getConnectButton(), "cell 0 1,alignx center");
	}


	private JLabel getPleaseSelectALabel() {
		if (pleaseSelectALabel == null) {
			pleaseSelectALabel = new JLabel("Please select a port to connect to this device:");
			pleaseSelectALabel.setName("pleaseSelectALabel");
		}
		return pleaseSelectALabel;
	}
	private JComboBox getComboBox() {
		if (comboBox == null) {
			comboBox = new JComboBox();
			comboBox.setModel(new CommPortModel());
			comboBox.setName("comboBox");
		}
		return comboBox;
	}
	private JButton getConnectButton() {
		if (connectButton == null) {
			connectButton = new JButton("Connect");
			connectButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent arg0) {
					try {
						WaveSphere.serial.openSerialPort((String) comboBox.getSelectedItem(), 9600);
						MainWindow.normalMode();
					} catch (PortInUseException e) {
						// TODO Auto-generated catch block
						new msgDialog("Error Opening Serial Port:\nPort is being used by another process...");
						e.printStackTrace();
					} catch (UnsupportedCommOperationException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} catch (TooManyListenersException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} catch (NullPointerException e){
						new msgDialog("Error Opening Serial Port:\nPort does not exist");
					}
				}
			});
			connectButton.setName("connectButton");
		}
		return connectButton;
	}
}
