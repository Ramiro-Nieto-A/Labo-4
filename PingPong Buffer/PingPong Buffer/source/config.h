/*
 * config.h
 *
 *  Created on: Feb 28, 2025
 *      Author: jacoby
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define  ENABLED 1
#define  DISABLED 0

// In this file 3 things can be configured
// 1-Test points ON / OFF
// 2-Sampling period
// 3-System function (pipe , function generator calibration and filter type)


//=====================1-Test points======================================
// The Test points helps to measure system timings
// TP0 measures how much time takes the filter to perform calculations
// TP0 is High while the filter code is running.
// TP1 measures the actual sampling period (Ts).
// TP1 toggles every time a new sample is ready
// i.e. TP1 period / 2 = Ts ; Fs=1/Ts.
// TP0 and TP1 can be disabled to avoid too much noise in the DAC output
// due their fast transitions.
// It is recommended to disable them when making filter measurements
// so that output signal is cleaner (no spikes).

#define TEST_POINT ENABLED //ENABLED/ DISABLED TPs outputs


//================2-Sampling period (TS)================
// TS defines the sampling period in microseconds.

#define TS 20    // TS in us (Note 20us => 50KHz)


//================ 3-Filter Type =================================
// FILTER_TYPE defines what  kind of filter / Mode will be used.
// It defines what code will be run.


#define	PIP_T 0     // PIPE (z**(-1)) (caño)
#define	CAL_T 1     // Calibrate signal generator Amplitude / Offset
#define	SOS_T 3		// FILTER_TYPE =IIR_T  (SOS Infinite Impulse Response).
#define	KPS_T 4		// KP Strong =KPS_T  ().
#define	IIR_T 5		// FILTER_TYPE =IIR_T  (SOS Infinite Impulse Response).
#define	FIR_T 6	    // FILTER_TYPE =FIR_T  (Finite impulse Response filter).


#define FILTER_TYPE IIR_T     // Change this to suit your needs


#endif /* CONFIG_H_ */
