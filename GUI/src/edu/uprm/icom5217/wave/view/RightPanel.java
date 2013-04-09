package edu.uprm.icom5217.wave.view;

import java.awt.Color;

import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JToggleButton;
import javax.swing.border.LineBorder;
import javax.swing.border.TitledBorder;

import edu.uprm.icom5217.wave.view.diagnostic.DiagnosticWindow;

import net.miginfocom.swing.MigLayout;
import java.awt.Component;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

public class RightPanel extends JPanel {
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
