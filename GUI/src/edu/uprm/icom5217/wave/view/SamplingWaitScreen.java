package edu.uprm.icom5217.wave.view;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JPanel;
import net.miginfocom.swing.MigLayout;
import javax.swing.JProgressBar;
import javax.swing.JLabel;
import javax.swing.Timer;

public class SamplingWaitScreen extends JPanel{
	private JProgressBar progressBar;
	private JLabel pleaseWaitLabel;
	
	final Timer t  = new Timer(1000, new ActionListener() {
		int count = 0;
		@Override
		public void actionPerformed(ActionEvent arg0) {
		   getProgressBar().setValue(++count);
		   if(count >= 30){
			   MainWindow.normalMode();
			   new LocationModeWindow();
			   done();
		   }
			
		}
	});
	
	public SamplingWaitScreen() {
		setLayout(new MigLayout("fill", "[center]", "[center][]"));
		add(getPleaseWaitLabel(), "flowy,cell 0 0 1 2");
		add(getProgressBar(), "cell 0 0");
		t.start();
		
	}
	
	private void done(){
		t.stop();
	}

	private JProgressBar getProgressBar() {
		if (progressBar == null) {
			progressBar = new JProgressBar();
			progressBar.setMaximum(30);
			progressBar.setName("progressBar");
		}
		return progressBar;
	}
	private JLabel getPleaseWaitLabel() {
		if (pleaseWaitLabel == null) {
			pleaseWaitLabel = new JLabel("Please Wait ....");
			pleaseWaitLabel.setName("pleaseWaitLabel");
		}
		return pleaseWaitLabel;
	}
	
	
}
