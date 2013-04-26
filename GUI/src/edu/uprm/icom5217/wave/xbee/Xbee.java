package edu.uprm.icom5217.wave.xbee;

public enum Xbee {

	 STATUS_MODE(lang.STATUS_MODE),
	 RETRIEVAL_MODE(lang.RETRIEVAL_MODE),
	 SHUTDOWN_MODE(lang.SHUTDOWN_MODE),
	 SAMPLING_MODE(lang.SAMPLING_MODE),
	 STOP_LOCATE_MODE(lang.STOP_LOCATE_MODE),
	 DIAGNOSTIC_MODE(lang.DIAGNOSTIC_MODE),
	 STOP_DIAGNOSTIC_MODE(lang.STOP_DIAGNOSTIC_MODE);
	 
	 private final char command;
	 
	 private Xbee(char command)
	 {
		 this.command = command;
	 }
	 
	 public char getCommand()
	 {
		 return command;
	 }
	 
	 private class lang{
			private static final char STATUS_MODE = 0;
			private static final char RETRIEVAL_MODE = 1;
			private static final char SHUTDOWN_MODE = 2;
			private static final char SAMPLING_MODE = 3;
			private static final char STOP_LOCATE_MODE = 4;
			private static final char DIAGNOSTIC_MODE = 5;
			private static final char STOP_DIAGNOSTIC_MODE = 6;

	}
}
