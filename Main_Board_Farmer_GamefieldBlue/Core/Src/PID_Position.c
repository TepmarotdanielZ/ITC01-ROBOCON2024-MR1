#include "PID_Position.h"

void PID_Position_Init(PIDPosition *pid, int N_input) {

	for(int i = 0; i< N_input; i++)
	{
		pid->integrator[i] = 0.0f;
		pid->prevError[i] = 0.0f;
		
		pid->differentiator[i]  = 0.0f;
		pid->prevMeasurement[i] = 0.0f;
		pid->error_est[i] = 0.0f;
		pid->derror[i] = 0.0f;

		pid->out[i] = 0.0f;
	}
}

float PID_Position(PIDPosition *pid, float setpoint, float measurement,float Kp,float Ki,float Kd, int i)
{
	float error = setpoint - measurement;
	float propotional = Kp*error;
	pid->integrator[i] = pid->integrator[i] + 0.5f * Ki *(error + pid->prevError[i])*pid->T;
	/* Anti-wind-up via integrator clamping */
    	if (pid->integrator[i] > pid->limMaxInt) {
        	pid->integrator[i] = pid->limMaxInt;
    	}
    	else if (pid->integrator[i] < pid->limMinInt) {
        	pid->integrator[i] = pid->limMinInt;
    	}
    	else{
    		pid->integrator[i] = pid->integrator[i];
    	}
    	
    	/* Lowpass filter */

	pid->error_est[i] = error - pid->prevError[i];
	pid->derror[i] = (1 - pid->alpha) * pid->derror[i] + pid->alpha * pid->error_est[i];
	pid->differentiator[i] = Kd*(pid->derror[i]/pid->T);
//	pid->differentiator[i] = pid->alpha*Kd*(error - pid->prevError[i])/pid->T + (1-pid->alpha)*pid->differentiator[i];

	pid->out[i] = propotional + pid->integrator[i] + pid->differentiator[i];
	//satuaration in  pid->out[i];
    	if (pid->out[i] > pid->limMax) {
        	pid->out[i] = pid->limMax;
    	}
    	else if (pid->out[i] < pid->limMin) {
        	pid->out[i] = pid->limMin;
    	}
    	else{
    		pid->out[i] = pid->out[i];
    	}
    pid->prevError[i] = error;
	return pid->out[i];
}
