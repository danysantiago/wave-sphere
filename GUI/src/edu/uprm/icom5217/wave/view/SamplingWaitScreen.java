package edu.uprm.icom5217.wave.view;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.Timer;

import edu.uprm.icom5217.wave.WaveSphere;
import edu.uprm.icom5217.wave.model.SphereList;
import edu.uprm.icom5217.wave.xbee.Xbee;

import net.miginfocom.swing.MigLayout;

public class SamplingWaitScreen extends JPanel{
	private static final long serialVersionUID = -5703286762738329221L;
	private static JProgressBar progressBar;
	private JLabel pleaseWaitLabel;
	
	private Timer t = new Timer(1000, new ActionListener() {
		int count = 0;
		@Override
		public void actionPerformed(ActionEvent arg0) {
		   getProgressBar().setValue(++count);

		   if(count > 60){
			   
			   //check which one works
			   //MainWindow.getInstance().getSplitPane().setRightComponent(LocatePanel.getInstance());

			   MainWindow.setRightPanel(LocatePanel.getInstance());

			   WaveSphere.serial.write(SphereList.getInstance().get(0).getId(), Xbee.LOCATE_MODE);
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
	
	public void done(){
		t.stop();
	}

	private static JProgressBar getProgressBar() {
		if (progressBar == null) {
			progressBar = new JProgressBar();
			progressBar.setMaximum(60);
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
