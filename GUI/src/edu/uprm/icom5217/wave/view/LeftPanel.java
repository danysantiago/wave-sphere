package edu.uprm.icom5217.wave.view;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ListSelectionModel;
import javax.swing.UIManager;
import javax.swing.border.EmptyBorder;
import javax.swing.border.TitledBorder;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import net.miginfocom.swing.MigLayout;
import edu.uprm.icom5217.wave.WaveSphere;
import edu.uprm.icom5217.wave.model.Sphere;
import edu.uprm.icom5217.wave.model.SphereList;
import edu.uprm.icom5217.wave.xbee.Xbee;

public class LeftPanel extends JPanel {
	/**
	 * 
	 */
	private static final long serialVersionUID = -4124150540994039205L;
	private JPanel scrollListContainer;
	private JList<Sphere> spheresList;

	private JButton addSphereButton;

	public LeftPanel() {
		setAlignmentY(Component.TOP_ALIGNMENT);
		setMaximumSize(new Dimension(200, 600));
		setLayout(new MigLayout("", "[200px, center]", "[100%][][][][][]"));
		setMinimumSize(new Dimension(200, 50));

		scrollListContainer = new JPanel();
		scrollListContainer.setAlignmentY(Component.TOP_ALIGNMENT);
		scrollListContainer.setBorder(new EmptyBorder(10, 15, 10, 15));
		scrollListContainer.setLayout(new BoxLayout(scrollListContainer,
				BoxLayout.Y_AXIS));

		JScrollPane scrollPane = new JScrollPane(getSpheresList());
		scrollPane.setPreferredSize(new Dimension(50, 130));
		scrollPane.setAlignmentY(Component.TOP_ALIGNMENT);
		scrollPane.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "Local Spheres:", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		scrollListContainer.add(scrollPane);

		add(scrollListContainer, "cell 0 0 1 3, grow, shrink 0");
		add(getAddSphereButton(), "cell 0 3");
	}

	private JList<Sphere> getSpheresList() {
		if (spheresList == null) {
			spheresList = new JList<Sphere>();
			spheresList.addListSelectionListener(new ListSelectionListener() {
				public void valueChanged(ListSelectionEvent arg0) {
					MainWindow.normalMode();
					WaveSphere.serial.setFlag(Xbee.STATUS_MODE);
				}
			});
			spheresList.setAlignmentY(Component.TOP_ALIGNMENT);
			spheresList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
			spheresList.setModel(SphereList.getInstance());
		}

		return spheresList;
	}

	private JButton getAddSphereButton() {
		if (addSphereButton == null) {
			addSphereButton = new JButton("  Add New Sphere ");
			addSphereButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent arg0) {
					MainWindow.setRightPanel(new AddSpherePanel());
				}
			});
		}

		return addSphereButton;
	}
}
