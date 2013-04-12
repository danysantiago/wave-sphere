package edu.uprm.icom5217.wave.events;

import java.util.HashMap;
import java.util.Map;

public class EventBus {

	private Map<Event.Type<EventHandler>, EventHandler> map;
	private final static EventBus INSTANCE = new EventBus();
	
	private EventBus(){
		map = new HashMap<Event.Type<EventHandler>, EventHandler>();
	}
	
	public  void  addHandler( Event.Type<EventHandler> type, EventHandler handler){
		
	}
	
	public  void fireEvent(Event<?> e)
	{
		for(Map.Entry<Event.Type<EventHandler>, EventHandler> entry: map.entrySet())
		{
			if(entry.getKey() ==   e.getAssociatedType())
			{
//				e.dispatch(entry.getValue());
			}
		}
	}
}
