package edu.uprm.icom5217.wave.model;

import java.io.Serializable;

public class Sphere implements Serializable {

	private static final long serialVersionUID = 4906525776681688805L;
	
	private final String id;
	private final String name;
	
	public Sphere(String id, String name)
	{
		this.id = id;
		this.name = name;
	}

	public String getId() {
		return id;
	}

	public String getName() {
		return name;
	}

	@Override
	public String toString() {
		return name;
	}
}
