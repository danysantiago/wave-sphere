import gnu.io.PortInUseException;
import gnu.io.UnsupportedCommOperationException;

import java.awt.EventQueue;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.TooManyListenersException;

import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.text.DefaultCaret;


public class serialWindow {
	
	static SerialCommunication serial;
	static String comName;

	private JFrame frame;
	private JComboBox comboBox;
	private JButton btnOpenPort;
	private static JTextArea textArea;
	private JScrollPane scrollPane;

	/**
	 * Launch the application.
	 * @throws IOException 
	 * @throws TooManyListenersException 
	 * @throws UnsupportedCommOperationException 
	 * @throws PortInUseException 
	 */
	public static void main(String[] args) {
		
		serial = new SerialCommunication();
		
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					serialWindow window = new serialWindow();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public serialWindow() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		frame = new JFrame();
		frame.setBounds(100, 100, 450, 300);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		JButton btnLed = new JButton("LED ON");
		btnLed.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				serial.write(XBee.lang.LEDON);
			}
		});
		
		JButton btnLed_1 = new JButton("LED OFF");
		btnLed_1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				serial.write(XBee.lang.LEDOFF);
			}
		});
		
		btnOpenPort = new JButton("OPEN PORT");
		btnOpenPort.addActionListener(new ActionListener() {
			private boolean portOpen;

			public void actionPerformed(ActionEvent e) {
				try {
					if(!portOpen){
						serial.openSerialPort((String) comboBox.getSelectedItem(), 9600);
						btnOpenPort.setText("CLOSE PORT");
						comboBox.setEnabled(false);
						portOpen = true;
					} else {
						serial.close();
						btnOpenPort.setText("OPEN PORT");
						comboBox.setEnabled(true);
						portOpen = false;
					}
				} catch (PortInUseException e1) {
					new msgDialog("Error Opening Serial Port:\nPort is being used by another process...");
					e1.printStackTrace();
				} catch (UnsupportedCommOperationException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				} catch (TooManyListenersException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		
		comboBox = new JComboBox(serial.getSerialPorts().toArray());
		
		JLabel lblGps = new JLabel("GPS:");
		
		scrollPane = new JScrollPane();
		
		GroupLayout groupLayout = new GroupLayout(frame.getContentPane());
		groupLayout.setHorizontalGroup(
			groupLayout.createParallelGroup(Alignment.LEADING)
				.addGroup(groupLayout.createSequentialGroup()
					.addContainerGap()
					.addGroup(groupLayout.createParallelGroup(Alignment.LEADING)
						.addComponent(scrollPane, GroupLayout.DEFAULT_SIZE, 414, Short.MAX_VALUE)
						.addGroup(groupLayout.createSequentialGroup()
							.addGroup(groupLayout.createParallelGroup(Alignment.LEADING)
								.addComponent(btnLed)
								.addComponent(btnLed_1))
							.addPreferredGap(ComponentPlacement.RELATED, 252, Short.MAX_VALUE)
							.addGroup(groupLayout.createParallelGroup(Alignment.LEADING, false)
								.addComponent(comboBox, 0, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
								.addComponent(btnOpenPort, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
						.addComponent(lblGps))
					.addContainerGap())
		);
		groupLayout.setVerticalGroup(
			groupLayout.createParallelGroup(Alignment.LEADING)
				.addGroup(groupLayout.createSequentialGroup()
					.addContainerGap()
					.addGroup(groupLayout.createParallelGroup(Alignment.BASELINE)
						.addComponent(btnLed)
						.addComponent(btnOpenPort))
					.addPreferredGap(ComponentPlacement.RELATED)
					.addGroup(groupLayout.createParallelGroup(Alignment.BASELINE)
						.addComponent(btnLed_1)
						.addComponent(comboBox, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
					.addGap(11)
					.addComponent(lblGps)
					.addPreferredGap(ComponentPlacement.RELATED)
					.addComponent(scrollPane, GroupLayout.PREFERRED_SIZE, 150, GroupLayout.PREFERRED_SIZE)
					.addContainerGap())
		);
		
		textArea = new JTextArea();
		DefaultCaret caret = (DefaultCaret)textArea.getCaret();
		caret.setUpdatePolicy(DefaultCaret.ALWAYS_UPDATE);
		
		scrollPane.setViewportView(textArea);
		textArea.setEditable(false);
		frame.getContentPane().setLayout(groupLayout);
	}
	
	public static void printToTextArea(char c){
		textArea.append(""+c);
	}

	public static void printToTextArea(String s) {
		// TODO Auto-generated method stub
		textArea.append(s);
	}
}
