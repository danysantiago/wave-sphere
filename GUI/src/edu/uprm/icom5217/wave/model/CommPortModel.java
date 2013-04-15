package edu.uprm.icom5217.wave.model;

import gnu.io.CommPortIdentifier;

import java.util.Enumeration;
import java.util.Vector;

import javax.swing.DefaultComboBoxModel;

@SuppressWarnings("unchecked")
public class CommPortModel extends DefaultComboBoxModel<String> {

	private static final long serialVersionUID = -5725420706783496775L;

	private static final Vector<String> theList = new Vector<String>(); //shouldnt be static
	static {
		Enumeration<CommPortIdentifier> portList = CommPortIdentifier
				.getPortIdentifiers();

		CommPortIdentifier portId = null;

		while (portList.hasMoreElements()) {

			portId = portList.nextElement();

			if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {

				theList.add(portId.getName());

			}
		}
	}

	public CommPortModel() {
		super(theList);
	}
	

}
