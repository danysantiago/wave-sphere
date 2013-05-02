package edu.uprm.icom5217.wave.model;

import javax.swing.DefaultListModel;

public class SphereList extends DefaultListModel<Sphere> {

	private static final long serialVersionUID = -7444939952063249674L;
	private static SphereList INSTANCE = new SphereList();
	
	private int selectedIndex = -1;
	
	public SphereList(){
		super();
	}
	
	public static SphereList getInstance()
	{
		return INSTANCE;
	}
	
	public synchronized void setSelectedIndex(int i){
		selectedIndex = i;
	}
	
	public synchronized int getSelectedIndex(){
		
		return selectedIndex;
	}
	
	
	@Override
	public boolean contains(Object elem) {
		Sphere sphere;
		if(elem instanceof Sphere)
		{
			sphere= (Sphere) elem;
		}
		else return false;
		     
		for(int i =0; i< getSize(); ++i){
			if(getElementAt(i).getId().equals(sphere.getId()))
				return true;
		}
      
		return false;
	}
	
	public synchronized Sphere getSelected(){
		if(selectedIndex != -1){
			return get(selectedIndex);
		}
		else return null;
	}
}
