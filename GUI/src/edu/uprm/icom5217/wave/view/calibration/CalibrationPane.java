package edu.uprm.icom5217.wave.view.calibration;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.UIManager;
import javax.swing.border.TitledBorder;

import net.miginfocom.swing.MigLayout;

public class CalibrationPane extends JPanel {
	private static final long serialVersionUID = -7774973064203837802L;
	private JLabel xLabel;
	private JLabel scaleLabel;
	private JLabel offsetLabel;
	private JTextField textField;
	private JTextField textField_1;
	private JLabel yLabel;
	private JTextField textField_2;
	private JTextField textField_3;
	private JLabel zLabel;
	private JTextField textField_4;
	private JTextField textField_5;

	/**
	 * Create the panel.
	 */
	public CalibrationPane() {
		setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "Component", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		setLayout(new MigLayout("", "[][grow][grow]", "[][][][]"));
		add(getScaleLabel(), "cell 1 0,alignx center");
		add(getOffsetLabel(), "cell 2 0,alignx center");
		add(getXLabel(), "cell 0 1,alignx trailing");
		add(getTextField(), "cell 1 1,growx");
		add(getTextField_1(), "cell 2 1,growx");
		add(getYLabel(), "cell 0 2,alignx trailing");
		add(getTextField_2(), "cell 1 2,growx");
		add(getTextField_3(), "cell 2 2,growx");
		add(getZLabel(), "cell 0 3,alignx trailing");
		add(getTextField_4(), "cell 1 3,growx");
		add(getTextField_5(), "cell 2 3,growx");

	}
	private JLabel getXLabel() {
		if (xLabel == null) {
			xLabel = new JLabel("X :   ");
			xLabel.setName("xLabel");
		}
		return xLabel;
	}
	private JLabel getScaleLabel() {
		if (scaleLabel == null) {
			scaleLabel = new JLabel("Scale");
			scaleLabel.setName("scaleLabel");
		}
		return scaleLabel;
	}
	private JLabel getOffsetLabel() {
		if (offsetLabel == null) {
			offsetLabel = new JLabel("Offset");
			offsetLabel.setName("offsetLabel");
		}
		return offsetLabel;
	}
	private JTextField getTextField() {
		if (textField == null) {
			textField = new JTextField();
			textField.setName("textField");
			textField.setColumns(10);
		}
		return textField;
	}
	private JTextField getTextField_1() {
		if (textField_1 == null) {
			textField_1 = new JTextField();
			textField_1.setName("textField_1");
			textField_1.setColumns(10);
		}
		return textField_1;
	}
	private JLabel getYLabel() {
		if (yLabel == null) {
			yLabel = new JLabel("Y :  ");
			yLabel.setName("yLabel");
		}
		return yLabel;
	}
	private JTextField getTextField_2() {
		if (textField_2 == null) {
			textField_2 = new JTextField();
			textField_2.setName("textField_2");
			textField_2.setColumns(10);
		}
		return textField_2;
	}
	private JTextField getTextField_3() {
		if (textField_3 == null) {
			textField_3 = new JTextField();
			textField_3.setName("textField_3");
			textField_3.setColumns(10);
		}
		return textField_3;
	}
	private JLabel getZLabel() {
		if (zLabel == null) {
			zLabel = new JLabel("Z :  ");
			zLabel.setName("zLabel");
		}
		return zLabel;
	}
	private JTextField getTextField_4() {
		if (textField_4 == null) {
			textField_4 = new JTextField();
			textField_4.setName("textField_4");
			textField_4.setColumns(10);
		}
		return textField_4;
	}
	private JTextField getTextField_5() {
		if (textField_5 == null) {
			textField_5 = new JTextField();
			textField_5.setName("textField_5");
			textField_5.setColumns(10);
		}
		return textField_5;
	}
}
