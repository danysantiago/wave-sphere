package edu.uprm.icom5217.wave.model;

import javax.swing.DefaultListModel;

public class SphereList extends DefaultListModel<Sphere> {

	private static final long serialVersionUID = -7444939952063249674L;
	private static SphereList INSTANCE = new SphereList();
	
	public SphereList(){
		super();
	}
	
	public static SphereList getInstance()
	{
		return INSTANCE;
	}
}
