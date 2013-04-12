package edu.uprm.icom5217.wave;

import java.awt.EventQueue;
import java.io.IOException;
import java.util.TooManyListenersException;

import javax.swing.JDialog;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

import edu.uprm.icom5217.wave.view.MainWindow;
import edu.uprm.icom5217.wave.xbee.SerialCommunication;
import gnu.io.PortInUseException;
import gnu.io.UnsupportedCommOperationException;


public class WaveSphere{
	public static SerialCommunication serial;

	private static  MainWindow window;
	/**
	 * @param args
	 * @throws IOException 
	 * @throws TooManyListenersException 
	 * @throws UnsupportedCommOperationException 
	 * @throws PortInUseException 
	 */
	public static void main(String[] args) throws PortInUseException, UnsupportedCommOperationException, TooManyListenersException, IOException {
		
		serial = new SerialCommunication();
		serial.openSerialPort("COM10", 9600); // TODO change to dropdown list
		
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());//might make it look better on Mac
		} catch (ClassNotFoundException | InstantiationException
				| IllegalAccessException | UnsupportedLookAndFeelException e) {
			System.out.println("Could not apply local look and feel. Using default.");
		}
		
		window =  MainWindow.getInstance();
		EventQueue.invokeLater(new Runnable() {
			
			@Override
			public void run() {
				window.pack();
				window.setVisible(true);
				window.setMinimumSize(window.getSize());
				window.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
			}
		});
	
	}
	
}
