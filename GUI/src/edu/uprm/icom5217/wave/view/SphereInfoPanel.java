package edu.uprm.icom5217.wave.view;

import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JPanel;

import net.miginfocom.swing.MigLayout;

public class SphereInfoPanel extends JPanel {
	
	private static final long serialVersionUID = -1992120414454971282L;
	private JLabel IdNumberLabel;
	private JLabel ldNumberValueLabel;
	private JLabel lastLocationLabel;
	private JLabel lastLocationValueLabel;
	private JLabel samplesLable;
	private JLabel spaceAvailableLabel;
	private JLabel spaceAvailableValueLabel;
	private JLabel batteryLevelLabel;
	private JLabel batteryLevelValueLabel;
	private JButton downloadAllSamplesButton;
	private JButton clearAllDataButton;
	private JComboBox comboBox;
	
	public SphereInfoPanel() {
		setLayout(new MigLayout("fill", "[212.00,grow,leading]15[96.00]15[grow]", "[]10[]10[]10[]10[]10[23.00]10[]"));
		add(getIdNumberLabel(), "cell 0 0,alignx right");
		add(getLdNumberValueLabel(), "cell 2 0");
		add(getLastLocationLabel(), "cell 0 1,alignx right");
		add(getLastLocationValueLabel(), "cell 2 1");
		add(getSpaceAvailableLable(), "cell 0 2,alignx right");
		add(getSpaceAvailableValueLabel(), "cell 2 2");
		add(getBatteryLevelLabel(), "cell 0 3,alignx right");
		add(getBatteryLevelValueLabel(), "cell 2 3");
		add(getSamplesLable(), "cell 0 4,alignx right");
		add(getComboBox_1(), "cell 2 4");
		add(getDownloadAllSamplesButton(), "flowx,cell 0 6");
		add(getClearAllDataButton(), "cell 2 6,alignx right");
	}

	

	private JLabel getIdNumberLabel() {
		if (IdNumberLabel == null) {
			IdNumberLabel = new JLabel("ID Number: ");
			IdNumberLabel.setFont(new Font("Tahoma", Font.BOLD, 11));
			IdNumberLabel.setName("IdNumberLabel");
		}
		return IdNumberLabel;
	}
	private JLabel getLdNumberValueLabel() {
		if (ldNumberValueLabel == null) {
			ldNumberValueLabel = new JLabel("097434134");
			ldNumberValueLabel.setFont(new Font("Tahoma", Font.ITALIC, 11));
			ldNumberValueLabel.setName("ldNumberValueLabel");
		}
		return ldNumberValueLabel;
	}
	private JLabel getLastLocationLabel() {
		if (lastLocationLabel == null) {
			lastLocationLabel = new JLabel("Last Location:");
			lastLocationLabel.setFont(new Font("Tahoma", Font.BOLD, 11));
			lastLocationLabel.setName("lastLocationLabel");
		}
		return lastLocationLabel;
	}
	private JLabel getLastLocationValueLabel() {
		if (lastLocationValueLabel == null) {
			lastLocationValueLabel = new JLabel("5\u00B0 20' 36.2394\", 5\u00B0 20' 43.8\"");
			lastLocationValueLabel.setName("lastLocationValueLabel");
		}
		return lastLocationValueLabel;
	}
	private JLabel getSamplesLable() {
		if (samplesLable == null) {
			samplesLable = new JLabel("Sample:");
			samplesLable.setFont(new Font("Tahoma", Font.BOLD, 11));
			samplesLable.setName("samplesLable");
		}
		return samplesLable;
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
			spaceAvailableValueLabel = new JLabel("40MB (10%)");
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
			batteryLevelValueLabel = new JLabel("20%");
			batteryLevelValueLabel.setName("batteryLevelValueLabel");
		}
		return batteryLevelValueLabel;
	}
	private JButton getDownloadAllSamplesButton() {
		if (downloadAllSamplesButton == null) {
			downloadAllSamplesButton = new JButton("Download All Samples");
			downloadAllSamplesButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					JFileChooser fc = new JFileChooser();
					fc.showSaveDialog(MainWindow.getInstance());
				
				}
			});
			downloadAllSamplesButton.setName("downloadAllSamplesButton");
		}
		return downloadAllSamplesButton;
	}
	private JButton getClearAllDataButton() {
		if (clearAllDataButton == null) {
			clearAllDataButton = new JButton("Clear All Data");
			clearAllDataButton.setName("clearAllDataButton");
		}
		return clearAllDataButton;
	}
	private JComboBox getComboBox_1() {
		if (comboBox == null) {
			comboBox = new JComboBox();
			comboBox.setModel(new DefaultComboBoxModel(new String[] {"1", "2", "3", "4", "5", "6", "7"}));
			comboBox.setName("comboBox");
		}
		return comboBox;
	}
}
