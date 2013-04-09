package edu.uprm.icom5217.wave.events;

import edu.uprm.icom5217.wave.events.Event.Type;

public abstract class Event<T> {
	
	
	
	protected Event()
	{
		
	}
	
	
	public abstract void dispatch(T handler);
	
	public abstract Type<T> getAssociatedType();
	
	public static class Type<T>{
		
	}


}
