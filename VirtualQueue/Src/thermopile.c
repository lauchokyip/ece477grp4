#include "thermopile.h"

void thermopile_init(ADC_HandleTypeDef *h1, ADC_HandleTypeDef *h2) {
	beta = 3955; //Kelvin
	R25 = 118500;   //ohms
	T25 = 25;    //celsius
	R1 = 100000;
	s = 9;      //sensitivity conversion factor (x 10)
	ee = 99;   //emissivity of object (x 100)
	//dirac = 2;   //correction for filter transmission
	factor = 560; //factor for temperature scaling
	hadc1 = *h1;
	hadc2 = *h2;
	dirac_scale = 0;
}

int getTemp() {
	//Thermopile
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	raw = HAL_ADC_GetValue(&hadc1);

	//Thermistor
	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
	raw2 = HAL_ADC_GetValue(&hadc2);

	//Ambient temperature and resistance
	thermistorV = (raw2 * 3300) / 4095; // mv
	Rsen = (thermistorV * R1) / (3300 - thermistorV); // ohms; equation is voltage division
	Tsen = ((beta * (T25 + 273)) / (log(Rsen) - log(R25))) / ((273 + T25) + (beta / (log(Rsen) - log(R25)))) - 273;
	// this works same as line above:  Tsen = 1 / (float)((log(Rsen / (float)R25) / (float)beta) + (1 / (float)(273 + T25))) - 273;

	//Thermopile Voltage and Object Temperature
	thermopileV_raw = ((raw * 3300 / 4095)); // mv; ~580 mV offset when should be 0.0 mV
	if(thermopileV_raw < factor){  	//if - else logic prevents overflow and negative voltages which are unrealistic for our system
		thermopileV = 0;
	}
	else {
		thermopileV = (thermopileV_raw - factor) / 2.9f;  //mV * some scaling, 3.0 can change up or down to scale Tobj
	}

	//test value logic: ignore for final
	//Tsen = 30;

	//thermopile tuned logic
	if(Tsen >= 21){
		dirac_scale = 2460 - (Tsen - 22) * 20; // works for Tsen = 20, 25, 30, 32, 34
	}
	else if(Tsen < 21 && Tsen > 15){
		dirac_scale = 2460 - (Tsen - 22) * 9;//
	}
	else if(Tsen <= 15 && Tsen >= 5) {
		dirac_scale = 2460 - (Tsen - 22) * 7; //
	}
	else {
		dirac_scale = 2460 - (Tsen - 22) * 8;
	}
	Tobj = pow(      ((thermopileV * 1000) / (s * ee)) + pow(Tsen, 4 - dirac_scale / 1000.0f)       ,        1.0f / (4 - dirac_scale / 1000.0f)     );
	//

	//old version
	//dirac_scale = (Tsen - 22) * 2;
	//Tobj = pow(      ((thermopileV * 1000) / (s * ee)) + pow(Tsen, 4 - 2.4 - dirac_scale / 100.0f)       ,        1.0f / (4 - 2.4 - dirac_scale / 100.0f)     );   //dirac constant (subtracted from 4) to be changed accordingly; bigger dirac = higher temperature
	return Tobj;
}

int getTsen() {
	//Thermopile
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	raw = HAL_ADC_GetValue(&hadc1);

	//Thermistor
	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
	raw2 = HAL_ADC_GetValue(&hadc2);

	//Ambient temperature and resistance
	thermistorV = (raw2 * 3300) / 4095; // mv
	Rsen = (thermistorV * R1) / (3300 - thermistorV); // ohms; equation is voltage division
	Tsen = ((beta * (T25 + 273)) / (log(Rsen) - log(R25))) / ((273 + T25) + (beta / (log(Rsen) - log(R25)))) - 273;
	// this works same as line above:  Tsen = 1 / (float)((log(Rsen / (float)R25) / (float)beta) + (1 / (float)(273 + T25))) - 273;

	//Thermopile Voltage and Object Temperature
	thermopileV_raw = ((raw * 3300 / 4095)); // mv; ~580 mV offset when should be 0.0 mV
	if(thermopileV_raw < factor){  	//if - else logic prevents overflow and negative voltages which are unrealistic for our system
		thermopileV = 0;
	}
	else {
		thermopileV = (thermopileV_raw - factor) / 2.9;  //mV * some scaling, 3.0 can change up or down to scale Tobj
	}
	Tobj = pow(((thermopileV * 1000) / (s * ee)) + pow(Tsen, 4 - 2.49), 1.0f / (4 - 2.49));   //dirac constant (subtracted from 4) to be changed accordingly; bigger dirac = higher temperature
	return Tsen;
}

int getV() {
	//Thermopile
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	raw = HAL_ADC_GetValue(&hadc1);

	//Thermistor
	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
	raw2 = HAL_ADC_GetValue(&hadc2);

	//Ambient temperature and resistance
	thermistorV = (raw2 * 3300) / 4095; // mv
	Rsen = (thermistorV * R1) / (3300 - thermistorV); // ohms; equation is voltage division
	Tsen = ((beta * (T25 + 273)) / (log(Rsen) - log(R25))) / ((273 + T25) + (beta / (log(Rsen) - log(R25)))) - 273;
	// this works same as line above:  Tsen = 1 / (float)((log(Rsen / (float)R25) / (float)beta) + (1 / (float)(273 + T25))) - 273;

	//Thermopile Voltage and Object Temperature
	thermopileV_raw = ((raw * 3300 / 4095)); // mv; ~580 mV offset when should be 0.0 mV
	if(thermopileV_raw < factor){  	//if - else logic prevents overflow and negative voltages which are unrealistic for our system
		thermopileV = 0;
	}
	else {
		thermopileV = (thermopileV_raw - factor) / 2.9;  //mV * some scaling, 3.0 can change up or down to scale Tobj
	}
	Tobj = pow(((thermopileV * 1000) / (s * ee)) + pow(Tsen, 4 - 2.49), 1.0f / (4 - 2.49));   //dirac constant (subtracted from 4) to be changed accordingly; bigger dirac = higher temperature
	return thermopileV;
}

int getR() {
	//Thermopile
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	raw = HAL_ADC_GetValue(&hadc1);

	//Thermistor
	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
	raw2 = HAL_ADC_GetValue(&hadc2);

	//Ambient temperature and resistance
	thermistorV = (raw2 * 3300) / 4095; // mv
	Rsen = (thermistorV * R1) / (3300 - thermistorV); // ohms; equation is voltage division
	Tsen = ((beta * (T25 + 273)) / (log(Rsen) - log(R25))) / ((273 + T25) + (beta / (log(Rsen) - log(R25)))) - 273;
	// this works same as line above:  Tsen = 1 / (float)((log(Rsen / (float)R25) / (float)beta) + (1 / (float)(273 + T25))) - 273;

	//Thermopile Voltage and Object Temperature
	thermopileV_raw = ((raw * 3300 / 4095)); // mv; ~580 mV offset when should be 0.0 mV
	if(thermopileV_raw < factor){  	//if - else logic prevents overflow and negative voltages which are unrealistic for our system
		thermopileV = 0;
	}
	else {
		thermopileV = (thermopileV_raw - factor) / 2.9;  //mV * some scaling, 3.0 can change up or down to scale Tobj
	}
	Tobj = pow(((thermopileV * 1000) / (s * ee)) + pow(Tsen, 4 - 2.49), 1.0f / (4 - 2.49));   //dirac constant (subtracted from 4) to be changed accordingly; bigger dirac = higher temperature
	return Rsen;
}
