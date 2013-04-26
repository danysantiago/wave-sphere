package edu.uprm.icom5217.wave.view;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;

import net.miginfocom.swing.MigLayout;
import edu.uprm.icom5217.wave.WaveSphere;
import edu.uprm.icom5217.wave.xbee.Xbee;

public class LocatePanel extends JPanel {

	private static final long serialVersionUID = -7898091034199268682L;
	private JLabel theCurentLocationLabel;
	private JLabel locationLabel;
	private JButton newButtonButton;

	private static LocatePanel instance;

	public static LocatePanel getInstance(){
		if(instance == null)
			instance = new LocatePanel();

		return instance;
	}


	private LocatePanel() {
		setLayout(new MigLayout("fill", "[36.00,grow,center]", "[147.00,grow]5[]5[]5[][]"));
		//add(getPanel(), "cell 0 0,grow");
		add(getTheCurentLocationLabel(), "flowx,cell 0 0");
		add(getLocationLabel(), "cell 0 0");
		add(getNewButtonButton(), "cell 0 1");
	}



	private JLabel getTheCurentLocationLabel() {
		if (theCurentLocationLabel == null) {
			theCurentLocationLabel = new JLabel("The curent location of the sphere is:    ");
			theCurentLocationLabel.setName("theCurentLocationLabel");
		}
		return theCurentLocationLabel;
	}
	private JLabel getLocationLabel() {
		if (locationLabel == null) {
			locationLabel = new JLabel("");
		}
		return locationLabel;
	}
	private JButton getNewButtonButton() {
		if (newButtonButton == null) {
			newButtonButton = new JButton("Exit");
			newButtonButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					WaveSphere.serial.write(Xbee.STOP_LOCATE_MODE);
					WaveSphere.serial.setFlag(Xbee.STATUS_MODE);	
					WaveSphere.serial.resetSamplingFlag();
					MainWindow.normalMode();
				}
			});
			newButtonButton.setName("newButtonButton");
		}
		return newButtonButton;
	}

	public void setLabel(String s){
		locationLabel.setText(s);
	}
}
