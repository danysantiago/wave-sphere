package edu.uprm.icom5217.wave.view;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;

import net.miginfocom.swing.MigLayout;
import edu.uprm.icom5217.wave.WaveSphere;
import edu.uprm.icom5217.wave.xbee.XBee;

public class LocationModeWindow extends JDialog{
	private JLabel theCurentLocationLabel;
	private JLabel newLabelLabel;
	private JLabel label;
	private JButton newButtonButton;
	public LocationModeWindow() {
		setIconImage(Toolkit.getDefaultToolkit().getImage(LocationModeWindow.class.getResource("/img/wavespherelogo.png")));
		setTitle("Location of Sphere 1");
		setResizable(false);
		getContentPane().setLayout(new MigLayout("fill", "[center]", "[164.00,center][center]"));
		getContentPane().add(getTheCurentLocationLabel(), "flowx,cell 0 0");
		getContentPane().add(getNewLabelLabel(), "cell 0 0");
		getContentPane().add(getLabel(), "cell 0 0");
		getContentPane().add(getNewButtonButton(), "cell 0 1");
		pack();
		setVisible(true);
		setDefaultCloseOperation(DISPOSE_ON_CLOSE);
	}

	private JLabel getTheCurentLocationLabel() {
		if (theCurentLocationLabel == null) {
			theCurentLocationLabel = new JLabel("The curent location of the sphere is:    ");
			theCurentLocationLabel.setName("theCurentLocationLabel");
		}
		return theCurentLocationLabel;
	}
	private JLabel getNewLabelLabel() {
		if (newLabelLabel == null) {
			newLabelLabel = new JLabel("5\u00B0 20' 43.8\"");
			newLabelLabel.setName("newLabelLabel");
		}
		return newLabelLabel;
	}
	private JLabel getLabel() {
		if (label == null) {
			label = new JLabel(", 5\u00B0 20' 36.2394\"");
			label.setName("label");
		}
		return label;
	}
	private JButton getNewButtonButton() {
		if (newButtonButton == null) {
			newButtonButton = new JButton("Exit");
			newButtonButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					//WaveSphere.serial.write(XBee.lang.STOP_LOCATE_MODE);
					dispose();
				}
			});
			newButtonButton.setName("newButtonButton");
		}
		return newButtonButton;
	}
}
