package edu.uprm.icom5217.wave.view;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

import net.miginfocom.swing.MigLayout;
import edu.uprm.icom5217.wave.model.Sphere;
import edu.uprm.icom5217.wave.model.SphereList;

public class AddSpherePanel extends JPanel {

	private static final long serialVersionUID = 8178676106627626883L;
	private JLabel idLabel;
	private JTextField idField;
	private JLabel nameLabel;
	private JTextField nameField;
	private JButton addButton;
	private JButton cancelButton;


	
	public AddSpherePanel() {
		setLayout(new MigLayout("", "[grow,center, pref!]", "[]10[]10[]"));
		add(getIdLabel(), "flowx,cell 0 0");
		add(getIdField(), "cell 0 0, growx");
		add(getNameLabel(), "flowx,cell 0 1");
		add(getNameField(), "cell 0 1,growx");
		add(getAddButton(), "flowx,cell 0 2");
		add(getCancelButton(), "cell 0 2");
	}

	private JLabel getIdLabel() {
		if (idLabel == null) {
			idLabel = new JLabel("ID:      ");
			idLabel.setName("idLabel");
		}
		return idLabel;
	}
	private JTextField getIdField() {
		if (idField == null) {
			idField = new JTextField();
			idField.setName("idField");
			idField.setColumns(10);
		}
		return idField;
	}
	private JLabel getNameLabel() {
		if (nameLabel == null) {
			nameLabel = new JLabel("Name: ");
			nameLabel.setName("nameLabel");
		}
		return nameLabel;
	}
	private JTextField getNameField() {
		if (nameField == null) {
			nameField = new JTextField();
			nameField.setName("nameField");
			nameField.setColumns(10);
		}
		return nameField;
	}
	
	public String getId(){
		return getIdField().getText();
	}
	
	public String getName(){
		return getNameField().getText();
	}
	
	private JButton getAddButton() {
		if (addButton == null) {
			addButton = new JButton(" Add Sphere ");
			addButton.addActionListener(new AddSphereController(this));
			addButton.setName("addButton");
		}
		return addButton;
	}
	
	public final class AddSphereController implements ActionListener {
		private final Component parent;
		
		public AddSphereController(Component parent)
		{
			this.parent = parent;
		}
		
		public void actionPerformed(ActionEvent arg0) {
			String id = getId();
			String name = (getName().trim().isEmpty()? id: getName().trim());

			if(id.matches("[0-9]{3}-[0-9]{4}"))
			{
				Sphere sphere = new Sphere(id,name);
				if(!SphereList.getInstance().contains(sphere)){
					SphereList.getInstance().addElement(sphere);
				}
				else{
					JOptionPane.showMessageDialog(parent, "Given Id already exists in list", "Error", JOptionPane.ERROR_MESSAGE);
				}
			}
			else{
				JOptionPane.showMessageDialog(parent, "Invalid Id provided", "Error", JOptionPane.ERROR_MESSAGE);
			}
		}
	}
	private JButton getCancelButton() {
		if (cancelButton == null) {
			cancelButton = new JButton("Cancel");
			cancelButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					MainWindow.normalMode();
				}
			});
			cancelButton.setName("cancelButton");
		}
		return cancelButton;
	}
}
