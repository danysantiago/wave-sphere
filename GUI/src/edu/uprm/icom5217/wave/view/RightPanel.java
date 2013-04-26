package edu.uprm.icom5217.wave.view;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.border.LineBorder;
import javax.swing.border.TitledBorder;

import net.miginfocom.swing.MigLayout;
import edu.uprm.icom5217.wave.WaveSphere;
import edu.uprm.icom5217.wave.xbee.Xbee;

public class RightPanel extends JPanel {
	
	private static final long serialVersionUID = -9151818196289914762L;
	private JPanel sphereInfoPanel;
	private JButton exitButton;
	private JPanel panel;
	
	public RightPanel() {
		setBorder(new TitledBorder(new LineBorder(new Color(0, 0, 0), 2), "Sphere 1", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		setLayout(new MigLayout("", "[41.00][grow][]", "[42.00][121.00,grow][35.00][][grow]"));
		add(getExitButton(), "cell 1 0,alignx right");
		add(getSphereInfoPanel(), "flowx,cell 0 1 2 1,growx,aligny top");
		add(getPanel(), "cell 1 4,grow");
	}
	private JPanel getSphereInfoPanel() {
		if (sphereInfoPanel == null) {
			sphereInfoPanel = new SphereInfoPanel();
			sphereInfoPanel.setName("sphereInfoPanel");
		}
		return sphereInfoPanel;
	}
	private JButton getExitButton() {
		if (exitButton == null) {
			exitButton = new JButton("Exit Retrieval Mode");
			exitButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					WaveSphere.serial.write(Xbee.STATUS_MODE);
					WaveSphere.serial.setFlag(Xbee.STATUS_MODE);
					MainWindow.normalMode();
				}
			});
			exitButton.setName("exitButton");
		}
		return exitButton;
	}
	private JPanel getPanel() {
		if (panel == null) {
			panel = new SamplePanel();
			panel.setName("panel");
		}
		return panel;
	}
}
