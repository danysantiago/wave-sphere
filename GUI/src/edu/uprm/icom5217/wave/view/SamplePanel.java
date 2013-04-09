package edu.uprm.icom5217.wave.view;

import java.awt.Color;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.LineBorder;
import javax.swing.border.TitledBorder;

import net.miginfocom.swing.MigLayout;
import javax.swing.JButton;
import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;
import javax.swing.JFileChooser;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.JScrollPane;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

public class SamplePanel extends JPanel{
	public SamplePanel() {
		setBorder(new TitledBorder(new LineBorder(new Color(0, 0, 0)), "Sample 1", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		setLayout(new MigLayout("", "[grow][grow]", "[][grow][]"));
		add(getSampleStartDateLabel(), "cell 0 0,alignx trailing");
		add(getJanPmLabel(), "cell 1 0");
		add(getTabbedPane(), "cell 0 1 2 1,grow");
		add(getDownloadDataButton(), "cell 0 2,alignx center");
		add(getDeleteDataButton(), "cell 1 2,alignx center");
	}

	
	private static final long serialVersionUID = 6642732318340578520L;
	private JLabel sampleStartDateLabel;
	private JLabel janPmLabel;
	private JButton downloadDataButton;
	private JButton deleteDataButton;
	private JTabbedPane tabbedPane;
	private JPanel tableTabPanel;
	private JPanel plainTextTabPanel;
	private JTextArea textArea;
	private JTable table;
	private JScrollPane scrollPane;

	
	private JLabel getSampleStartDateLabel() {
		if (sampleStartDateLabel == null) {
			sampleStartDateLabel = new JLabel("Sample Start Date: ");
			sampleStartDateLabel.setName("sampleStartDateLabel");
		}
		return sampleStartDateLabel;
	}
	private JLabel getJanPmLabel() {
		if (janPmLabel == null) {
			janPmLabel = new JLabel("27/Jan/2013 1:00:00 PM");
			janPmLabel.setName("janPmLabel");
		}
		return janPmLabel;
	}
	private JButton getDownloadDataButton() {
		if (downloadDataButton == null) {
			downloadDataButton = new JButton("Download Sample");
			downloadDataButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					JFileChooser fc = new JFileChooser();
					fc.showSaveDialog(MainWindow.getInstance());
				}
			});
			downloadDataButton.setName("downloadDataButton");
		}
		return downloadDataButton;
	}
	private JButton getDeleteDataButton() {
		if (deleteDataButton == null) {
			deleteDataButton = new JButton("Delete Sample");
			deleteDataButton.setName("deleteDataButton");
		}
		return deleteDataButton;
	}
	private JTabbedPane getTabbedPane() {
		if (tabbedPane == null) {
			tabbedPane = new JTabbedPane(JTabbedPane.TOP);
			tabbedPane.setName("tabbedPane");
			tabbedPane.addTab("Table", null, getTableTabPanel(), null);
			tabbedPane.addTab("Text", null, getPlainTextTabPanel(), null);
		}
		return tabbedPane;
	}
	private JPanel getTableTabPanel() {
		if (tableTabPanel == null) {
			tableTabPanel = new JPanel();
			tableTabPanel.setName("tableTabPanel");
			tableTabPanel.setLayout(new MigLayout("", "[grow][grow][1px]", "[144.00,grow][-36.00px]"));
			tableTabPanel.add(getScrollPane(), "cell 0 0,grow");
		}
		return tableTabPanel;
	}
	private JPanel getPlainTextTabPanel() {
		if (plainTextTabPanel == null) {
			plainTextTabPanel = new JPanel();
			plainTextTabPanel.setName("plainTextTabPanel");
			plainTextTabPanel.setLayout(new MigLayout("", "[4px,grow]", "[22px,grow]"));
			plainTextTabPanel.add(getTextArea(), "cell 0 0,grow");
		}
		return plainTextTabPanel;
	}
	private JTextArea getTextArea() {
		if (textArea == null) {
			textArea = new JTextArea();
			textArea.setEditable(false);
			textArea.setName("textArea");
		}
		return textArea;
	}
	private JTable getTable() {
		if (table == null) {
			table = new JTable();
			table.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
			table.setFillsViewportHeight(true);
			table.setModel(new DefaultTableModel(
				new Object[][] {
					{null, null, null, null, null, null, null, null, null},
					{null, null, null, null, null, null, null, null, null},
					{null, null, null, null, null, null, null, null, null},
					{null, null, null, null, null, null, null, null, null},
					{null, null, null, null, null, null, null, null, null},
					{null, null, null, null, null, null, null, null, null},
					{null, null, null, null, null, null, null, null, null},
				},
				new String[] {
					"Mag. X", "Mag. Y", "Mag. Z", "Acc. X", "Acc. Y", "Acc. Z", "Gyro X", "Gyro Y", "Gyro Z"
				}
			) {
				Class[] columnTypes = new Class[] {
					Float.class, Float.class, Float.class, Float.class, Float.class, Float.class, Float.class, Float.class, Float.class
				};
				public Class getColumnClass(int columnIndex) {
					return columnTypes[columnIndex];
				}
			});
			table.setName("table");
		}
		return table;
	}
	private JScrollPane getScrollPane() {
		if (scrollPane == null) {
			scrollPane = new JScrollPane();
			scrollPane.setName("scrollPane");
			scrollPane.setViewportView(getTable());
		}
		return scrollPane;
	}
}
