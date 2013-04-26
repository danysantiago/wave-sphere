package edu.uprm.icom5217.wave.view;

import java.awt.Toolkit;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSplitPane;

import net.miginfocom.swing.MigLayout;
import edu.uprm.icom5217.wave.WaveSphere;

public class MainWindow extends JFrame{
	
	
	/**
	 * The generated serial ID
	 */
	private static final long serialVersionUID = 6647353545887103818L;
	public static final String LOGO_PATH = "/img/wavespherelogo4.png";
	
	private static MainWindow INSTANCE;
	private static final RightPanel RETRIEVAL_PANEL = new RightPanel();
	private static final RightPanel2 BOTONES = RightPanel2.getInstance();

	private final static JPanel defaultRightPanel = new JPanel();
	static{
		defaultRightPanel.setLayout(new MigLayout("fill","grow,c","grow,c"));
		defaultRightPanel.add(new JLabel("Please Select a sphere"));
	}
	
	private JSplitPane splitPane;
	public static volatile boolean isConnected;
	
	public MainWindow() {
		setIconImage(Toolkit.getDefaultToolkit().getImage(MainWindow.class.getResource(LOGO_PATH)));
		setTitle("Wave Sphere");
		getContentPane().setLayout(new MigLayout("fill", "[]", "[]"));
//		getContentPane().add(getSplitPane(), "cell 0 0,grow,aligny top");
		getContentPane().add(new ConnectionPane());
	}

	public JSplitPane getSplitPane() {
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
		if(isConnected){
			getInstance().getSplitPane().setRightComponent(BOTONES);
		}
		else{
			isConnected = true;
			getInstance().getContentPane().removeAll();
			getInstance().getContentPane().add(getInstance().getSplitPane(), "cell 0 0,grow,aligny top");
			getInstance().getSplitPane().setRightComponent(BOTONES);
			getInstance().pack();
		}
		
		getInstance().revalidate();
		getInstance().repaint();
	}
	 
	public static void connectMode(){
		isConnected = false;
		getInstance().getContentPane().removeAll();
		getInstance().getContentPane().add(new ConnectionPane());
		getInstance().pack();
		getInstance().revalidate();
		getInstance().repaint();
	}
	public static void samplingMode(){
		getInstance().getSplitPane().setRightComponent(new SamplingWaitScreen());
	}
	
	@Override
	public void dispose() {
		super.dispose();
		WaveSphere.serial.close();
	}
}
