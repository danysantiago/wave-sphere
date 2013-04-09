package edu.uprm.icom5217.wave.view;

import java.awt.Toolkit;

import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSplitPane;

import net.miginfocom.swing.MigLayout;
import java.awt.Dimension;

public class MainWindow extends JFrame{
	
	
	/**
	 * The generated serial ID
	 */
	private static final long serialVersionUID = 6647353545887103818L;
	private static final String LOGO_PATH = "/img/wavespherelogo.png";
	
	private static MainWindow INSTANCE;
	private static final RightPanel RETRIEVAL_PANEL = new RightPanel();
	private static final RightPanel2 BOTONES = new RightPanel2();

	private final static JPanel defaultRightPanel = new JPanel();
	static{
		defaultRightPanel.setLayout(new MigLayout("fill","grow,c","grow,c"));
		defaultRightPanel.add(new JLabel("Please Select a sphere"));
	}
	
	private JSplitPane splitPane;
	
	public MainWindow() {
		setMinimumSize(new Dimension(500, 400));
		setIconImage(Toolkit.getDefaultToolkit().getImage(MainWindow.class.getResource(LOGO_PATH)));
		setTitle("Wave Sphere");
		getContentPane().setLayout(new MigLayout("fill", "[]", "[]"));
		getContentPane().add(getSplitPane(), "cell 0 0,grow,aligny top");
	}

	private JSplitPane getSplitPane() {
		if (splitPane == null) {
			splitPane = new JSplitPane();
			splitPane.setEnabled(false);
			splitPane.setAutoscrolls(true);
			splitPane.setName("splitPane");
			splitPane.setLeftComponent(new LeftPanel());
			splitPane.setRightComponent(defaultRightPanel);
		}
		return splitPane;
	}
	
	public static MainWindow getInstance(){
		if(INSTANCE == null){
			INSTANCE = new MainWindow();
		}
		
		return INSTANCE;
	}
	
	public static void retrievalMode(){
		getInstance().getSplitPane().setRightComponent(RETRIEVAL_PANEL);
		getInstance().pack();
		getInstance().revalidate();
		getInstance().repaint();
	}
	
	public static void normalMode(){
		getInstance().getSplitPane().setRightComponent(BOTONES);
		getInstance().revalidate();
		getInstance().repaint();
	}
	 
	public static void samplingMode(){
		getInstance().getSplitPane().setRightComponent(new SamplingWaitScreen());
	}
}
