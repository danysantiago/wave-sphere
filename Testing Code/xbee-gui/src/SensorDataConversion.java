public class SensorDataConversion {
	public static final double VDD = 3.3;

	public static final double[] ACC_OFFSET = {1.74754941, 1.764655605, 1.770736235};
	public static final double ACC_GAIN = 0.00715;
	
	public static final double GYR_GAIN = 8.75/1000;
	
	public static final double[] MAG_GAIN = {330,330,330};
	
	public static final String[] AXIS_LABEL = {"x", "y", "z"};
	
	public static double[] convertAccData(String[] strData) {
		double[] data = new double[3];
		
		for(int i = 0; i < 3; i++) {
			data[i] = Double.parseDouble(strData[i].trim());
			data[i] = ((data[i])*(VDD/4096)-ACC_OFFSET[i])/ACC_GAIN;
		}
		
		return data;
	}
	
	public static double[] convertGyrData(String[] strData) {
		double[] data = new double[3];
		
		for(int i = 0; i < 3; i++) {
			data[i] = Double.parseDouble(strData[i].trim());
			data[i] *= GYR_GAIN;
		}
		
		return data;
		
	}
	
	public static double[] convertMagData(String[] strData) {
		double[] data = new double[3];
		
		for(int i = 0; i < 3; i++) {
			data[i] = Double.parseDouble(strData[i].trim());
			data[i] = data[i] / MAG_GAIN[i];
		}
		
		return data;
		
	}

}