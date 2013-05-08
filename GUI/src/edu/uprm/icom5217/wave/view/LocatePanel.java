package edu.uprm.icom5217.wave.view;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;

import net.miginfocom.swing.MigLayout;
import edu.uprm.icom5217.wave.WaveSphere;
import edu.uprm.icom5217.wave.model.Sphere;
import edu.uprm.icom5217.wave.model.SphereList;
import edu.uprm.icom5217.wave.xbee.Xbee;

public class LocatePanel extends JPanel {

	private static final long serialVersionUID = -7898091034199268682L;
	private JLabel theCurentLocationLabel;
	private JLabel locationLabel;
	private JButton cancelButton;

	private static LocatePanel instance;
	private JButton NextButton;

	private int currentSphereIndex;
	
	public static LocatePanel getInstance(){
		if(instance == null)
			instance = new LocatePanel();

		return instance;
	}
	
	public void reset(){
		currentSphereIndex = 0;
	}
	private LocatePanel() {
		setLayout(new MigLayout("fill", "[36.00,grow,center]", "[124.00,grow,bottom]5[55.00,top]5[66.00]5[][]"));
		//add(getPanel(), "cell 0 0,grow");
		add(getTheCurentLocationLabel(), "flowx,cell 0 0");
		add(getLocationLabel(), "cell 0 1");
		add(getNextButton(), "flowx,cell 0 2");
		add(getCancelButton(), "cell 0 2");
		currentSphereIndex = 0;
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
			locationLabel = new JLabel("                   ");
		}
		return locationLabel;
	}
	
	private JButton getCancelButton() {
		if (cancelButton == null) {
			cancelButton = new JButton("Cancel");
			cancelButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					WaveSphere.serial.write(SphereList.getInstance().get(currentSphereIndex).getId(), Xbee.STOP_LOCATE_MODE);
					WaveSphere.serial.setFlag(Xbee.STATUS_MODE);	
					reset();
					MainWindow.normalMode();
				}
			});
			cancelButton.setName("cancelButton");
		}
		return cancelButton;
	}

	public void setLabel(String s){
		locationLabel.setText(s);
		getTheCurentLocationLabel().setText("The current location of " + getCurrentSphere().getName() + " is: ");
	}
	
	public Sphere getCurrentSphere(){
		return SphereList.getInstance().get(currentSphereIndex);
	}
	
	private JButton getNextButton() {
		if (NextButton == null) {
			NextButton = new JButton("Next");
			NextButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent arg0) {
					WaveSphere.serial.write(SphereList.getInstance().get(currentSphereIndex++).getId(), Xbee.STOP_LOCATE_MODE);
					if(currentSphereIndex >= SphereList.getInstance().getSize()) //last sphere
					{
						WaveSphere.serial.setFlag(Xbee.STATUS_MODE);	
						reset();
						MainWindow.normalMode();
					}
					else
					{
						
						Sphere nextSphere = SphereList.getInstance().get(currentSphereIndex);
						//TODO get location of next sphere
						WaveSphere.serial.write(nextSphere.getId(), Xbee.LOCATE_MODE);
					}
				
				}
			});
			NextButton.setName("NextButton");
		}
		return NextButton;
	}
}
