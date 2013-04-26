package edu.uprm.icom5217.wave.view;

import java.awt.Font;

import javax.swing.JLabel;
import javax.swing.JPanel;

import net.miginfocom.swing.MigLayout;

public class SphereInfoPanel extends JPanel {
	
	private static final long serialVersionUID = -1992120414454971282L;
	
	private JLabel IdNumberLabel;
	private JLabel ldNumberValueLabel;
	private JLabel spaceAvailableLabel;
	private JLabel spaceAvailableValueLabel;
	private JLabel batteryLevelLabel;
	private JLabel batteryLevelValueLabel;
	
	public SphereInfoPanel() {
		setLayout(new MigLayout("", "[grow][][grow]", "[]10[]10[]10[]"));
		add(getIdNumberLabel(), "flowx,cell 1 0");
		add(getSpaceAvailableLable(), "cell 1 1");
		add(getIdNumberValueLabel(), "cell 1 0");
		add(getSpaceAvailableValueLabel(), "flowx,cell 1 2");
		add(getBatteryLevelValueLabel(), "cell 1 3");
		add(getBatteryLevelLabel(), "cell 1 2");
	}

	private JLabel getIdNumberLabel() {
		if (IdNumberLabel == null) {
			IdNumberLabel = new JLabel("ID Number: ");
			IdNumberLabel.setFont(new Font("Tahoma", Font.BOLD, 11));
			IdNumberLabel.setName("IdNumberLabel");
		}
		return IdNumberLabel;
	}
	
	private JLabel getIdNumberValueLabel() {
		if (ldNumberValueLabel == null) {
			ldNumberValueLabel = new JLabel("");
			ldNumberValueLabel.setFont(new Font("Tahoma", Font.ITALIC, 11));
			ldNumberValueLabel.setName("IdNumberValueLabel");
		}
		return ldNumberValueLabel;
	}
	
	private JLabel getSpaceAvailableLable(){
		if(spaceAvailableLabel ==null){
			spaceAvailableLabel = new JLabel("Memory Available:");
			spaceAvailableLabel.setFont(new Font("Tahoma", Font.BOLD, 11));
		}
		
		return spaceAvailableLabel;
	}
	
	private JLabel getSpaceAvailableValueLabel() {
		if (spaceAvailableValueLabel == null) {
			spaceAvailableValueLabel = new JLabel("");
			spaceAvailableValueLabel.setName("spaceAvailableValueLabel");
		}
		return spaceAvailableValueLabel;
	}
	
	private JLabel getBatteryLevelLabel() {
		if (batteryLevelLabel == null) {
			batteryLevelLabel = new JLabel("Battery Level:");
			batteryLevelLabel.setName("batteryLevelLabel");
			batteryLevelLabel.setFont(new Font("Tahoma", Font.BOLD, 11));
		}
		return batteryLevelLabel;
	}
	
	private JLabel getBatteryLevelValueLabel() {
		if (batteryLevelValueLabel == null) {
			batteryLevelValueLabel = new JLabel("");
			batteryLevelValueLabel.setName("batteryLevelValueLabel");
		}
		return batteryLevelValueLabel;
	}
	
	public void setIdValue(String value){
		getIdNumberValueLabel().setText(value);
	}
	
	public void setMemoryValue(String value){
		getSpaceAvailableValueLabel().setText(value);
	}
	
	public void setBatteryLevelValue(String value){
		getBatteryLevelValueLabel().setText(value);
	}
}
