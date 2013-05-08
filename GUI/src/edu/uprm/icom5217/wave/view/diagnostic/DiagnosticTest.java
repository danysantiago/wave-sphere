package edu.uprm.icom5217.wave.view.diagnostic;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Random;

import javax.swing.JButton;
import javax.swing.JFrame;

public class DiagnosticTest extends JFrame{

	
	public DiagnosticTest(){
		
		getContentPane().add(DiagnosticPanel.getInstance());
		getContentPane().add(new JButton("Test"){
			private static final long serialVersionUID = 1L;
		{
			addActionListener(new ActionListener() {
				
				@Override
				public void actionPerformed(ActionEvent arg0) {
					Random r = new Random();
					
					DiagnosticPanel.getInstance().setAccelerationValueLabel(" acc " + r.nextInt(100));
					
				}
			});
		}}, BorderLayout.SOUTH);
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {

		JFrame test = new DiagnosticTest();
	    test.pack();
		test.setVisible(true);
		test.setDefaultCloseOperation(EXIT_ON_CLOSE);
	
		
		
		
	}

}
