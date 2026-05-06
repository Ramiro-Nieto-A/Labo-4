/*
 * main.c
 *
 *  Created on: Jun 23, 2025
 *      Author: mpaurabe
 */

#include "fsl_device_registers.h"
#include "hardware.h"
#include "PingPongBuffer.h"
#include <ports.h>
#include "config.h"
#include <math.h>

#define BUFFER_SIZE 1000
#define SAMPLE_RATE_HZ 10000U


volatile bool nextsample;

volatile uint16_t *adc_in;
volatile uint16_t *dac_out;
static int alternate = -1;

void my_app(void);

int main(void)
{
	hw_Init();
	Init_Ports();
	PingPongBuffer_Init(SAMPLE_RATE_HZ,BUFFER_SIZE);

    for(;;)
    {
        if(SW2_read())
        	alternate = -1;

        if(SW3_read())
        	alternate = 1;


    	if(nextsample == 1)
        {
        	my_app();
        	nextsample = 0;
        }
    }
}



#define FIR_TAP1    281
#define FIR_TAP2    281
#define COMB_DELAY  200

const float taps_FIR1[281] = {
    0.00054709f,     0.00085821f,     0.00108468f,     0.00119782f,     0.00118063f,
    0.00103018f,     0.00075842f,     0.00039142f,     -0.00003305f,     -0.00046940f,
    -0.00086900f,     -0.00118560f,     -0.00138062f,     -0.00142775f,     -0.00131636f,
    -0.00105329f,     -0.00066264f,     -0.00018373f,     0.00033281f,     0.00083032f,
    0.00125230f,     0.00154871f,     0.00168189f,     0.00163116f,     0.00139577f,
    0.00099569f,     0.00047004f,     -0.00012659f,     -0.00072977f,     -0.00127208f,
    -0.00169062f,     -0.00193417f,     -0.00196939f,     -0.00178513f,     -0.00139448f,
    -0.00083409f,     -0.00016091f,     0.00055352f,     0.00123057f,     0.00179310f,
    0.00217413f,     0.00232473f,     0.00222008f,     0.00186308f,     0.00128499f,
    0.00054292f,     -0.00028571f,     -0.00111112f,     -0.00184076f,     -0.00238960f,
    -0.00268987f,     -0.00269927f,     -0.00240656f,     -0.00183390f,     -0.00103553f,
    -0.00009287f,     0.00089368f,     0.00181523f,     0.00256631f,     0.00305672f,
    0.00322221f,     0.00303250f,     0.00249594f,     0.00165996f,     0.00060715f,
    -0.00055263f,     -0.00169352f,     -0.00268711f,     -0.00341668f,     -0.00379060f,
    -0.00375353f,     -0.00329390f,     -0.00244691f,     -0.00129234f,     0.00005256f,
    0.00144476f,     0.00273027f,     0.00376092f,     0.00441103f,     0.00459229f,
    0.00426509f,     0.00344492f,     0.00220288f,     0.00066011f,     -0.00102357f,
    -0.00266574f,     -0.00408069f,     -0.00509985f,     -0.00559122f,     -0.00547564f,
    -0.00473792f,     -0.00343144f,     -0.00167537f,     0.00035534f,     0.00244675f,
    0.00436761f,     0.00589403f,     0.00683457f,     0.00705295f,     0.00648588f,
    0.00515384f,     0.00316327f,     0.00069960f,     -0.00198875f,     -0.00461399f,
    -0.00687811f,     -0.00850482f,     -0.00927093f,     -0.00903384f,     -0.00775210f,
    -0.00549635f,     -0.00244920f,     0.00110695f,     0.00481311f,     0.00826659f,
    0.01106056f,     0.01282667f,     0.01327681f,     0.01223947f,     0.00968721f,
    0.00575162f,     0.00072398f,     -0.00495946f,     -0.01074696f,     -0.01601344f,
    -0.02011209f,     -0.02243114f,     -0.02245072f,     -0.01979480f,     -0.01427310f,
    -0.00590914f,     0.00504896f,     0.01813613f,     0.03269670f,     0.04792745f,
    0.06293426f,     0.07679791f,     0.08864337f,     0.09770707f,     0.10339625f,
    0.10533549f,     0.10339625f,     0.09770707f,     0.08864337f,     0.07679791f,
    0.06293426f,     0.04792745f,     0.03269670f,     0.01813613f,     0.00504896f,
    -0.00590914f,     -0.01427310f,     -0.01979480f,     -0.02245072f,     -0.02243114f,
    -0.02011209f,     -0.01601344f,     -0.01074696f,     -0.00495946f,     0.00072398f,
    0.00575162f,     0.00968721f,     0.01223947f,     0.01327681f,     0.01282667f,
    0.01106056f,     0.00826659f,     0.00481311f,     0.00110695f,     -0.00244920f,
    -0.00549635f,     -0.00775210f,     -0.00903384f,     -0.00927093f,     -0.00850482f,
    -0.00687811f,     -0.00461399f,     -0.00198875f,     0.00069960f,     0.00316327f,
    0.00515384f,     0.00648588f,     0.00705295f,     0.00683457f,     0.00589403f,
    0.00436761f,     0.00244675f,     0.00035534f,     -0.00167537f,     -0.00343144f,
    -0.00473792f,     -0.00547564f,     -0.00559122f,     -0.00509985f,     -0.00408069f,
    -0.00266574f,     -0.00102357f,     0.00066011f,     0.00220288f,     0.00344492f,
    0.00426509f,     0.00459229f,     0.00441103f,     0.00376092f,     0.00273027f,
    0.00144476f,     0.00005256f,     -0.00129234f,     -0.00244691f,     -0.00329390f,
    -0.00375353f,     -0.00379060f,     -0.00341668f,     -0.00268711f,     -0.00169352f,
    -0.00055263f,     0.00060715f,     0.00165996f,     0.00249594f,     0.00303250f,
    0.00322221f,     0.00305672f,     0.00256631f,     0.00181523f,     0.00089368f,
    -0.00009287f,     -0.00103553f,     -0.00183390f,     -0.00240656f,     -0.00269927f,
    -0.00268987f,     -0.00238960f,     -0.00184076f,     -0.00111112f,     -0.00028571f,
    0.00054292f,     0.00128499f,     0.00186308f,     0.00222008f,     0.00232473f,
    0.00217413f,     0.00179310f,     0.00123057f,     0.00055352f,     -0.00016091f,
    -0.00083409f,     -0.00139448f,     -0.00178513f,     -0.00196939f,     -0.00193417f,
    -0.00169062f,     -0.00127208f,     -0.00072977f,     -0.00012659f,     0.00047004f,
    0.00099569f,     0.00139577f,     0.00163116f,     0.00168189f,     0.00154871f,
    0.00125230f,     0.00083032f,     0.00033281f,     -0.00018373f,     -0.00066264f,
    -0.00105329f,     -0.00131636f,     -0.00142775f,     -0.00138062f,     -0.00118560f,
    -0.00086900f,     -0.00046940f,     -0.00003305f,     0.00039142f,     0.00075842f,
    0.00103018f,     0.00118063f,     0.00119782f,     0.00108468f,     0.00085821f,
    0.00054709f,
};
const float taps_FIR2[281] = {
    0.00032125f,     0.00028024f,     0.00023664f,     0.00019048f,     0.00014179f,
    0.00009063f,     0.00003704f,     -0.00001891f,     -0.00007714f,     -0.00013757f,
    -0.00020011f,     -0.00026466f,     -0.00033111f,     -0.00039934f,     -0.00046923f,
    -0.00054063f,     -0.00061340f,     -0.00068739f,     -0.00076242f,     -0.00083834f,
    -0.00091496f,     -0.00099210f,     -0.00106955f,     -0.00114712f,     -0.00122460f,
    -0.00130177f,     -0.00137842f,     -0.00145430f,     -0.00152920f,     -0.00160287f,
    -0.00167506f,     -0.00174554f,     -0.00181405f,     -0.00188034f,     -0.00194415f,
    -0.00200523f,     -0.00206331f,     -0.00211814f,     -0.00216945f,     -0.00221699f,
    -0.00226049f,     -0.00229971f,     -0.00233438f,     -0.00236425f,     -0.00238907f,
    -0.00240861f,     -0.00242261f,     -0.00243085f,     -0.00243309f,     -0.00242912f,
    -0.00241871f,     -0.00240168f,     -0.00237780f,     -0.00234690f,     -0.00230880f,
    -0.00226333f,     -0.00221033f,     -0.00214965f,     -0.00208115f,     -0.00200473f,
    -0.00192026f,     -0.00182764f,     -0.00172681f,     -0.00161768f,     -0.00150020f,
    -0.00137434f,     -0.00124006f,     -0.00109737f,     -0.00094627f,     -0.00078678f,
    -0.00061894f,     -0.00044280f,     -0.00025844f,     -0.00006595f,     0.00013457f,
    0.00034299f,     0.00055919f,     0.00078299f,     0.00101425f,     0.00125275f,
    0.00149831f,     0.00175070f,     0.00200968f,     0.00227501f,     0.00254642f,
    0.00282363f,     0.00310634f,     0.00339425f,     0.00368703f,     0.00398435f,
    0.00428585f,     0.00459119f,     0.00489998f,     0.00521185f,     0.00552641f,
    0.00584325f,     0.00616196f,     0.00648212f,     0.00680332f,     0.00712511f,
    0.00744706f,     0.00776873f,     0.00808967f,     0.00840943f,     0.00872756f,
    0.00904360f,     0.00935710f,     0.00966761f,     0.00997467f,     0.01027783f,
    0.01057664f,     0.01087065f,     0.01115943f,     0.01144254f,     0.01171955f,
    0.01199004f,     0.01225359f,     0.01250980f,     0.01275828f,     0.01299863f,
    0.01323049f,     0.01345349f,     0.01366728f,     0.01387153f,     0.01406591f,
    0.01425012f,     0.01442387f,     0.01458687f,     0.01473887f,     0.01487962f,
    0.01500891f,     0.01512652f,     0.01523226f,     0.01532597f,     0.01540749f,
    0.01547669f,     0.01553347f,     0.01557772f,     0.01560938f,     0.01562840f,
    0.01563474f,     0.01562840f,     0.01560938f,     0.01557772f,     0.01553347f,
    0.01547669f,     0.01540749f,     0.01532597f,     0.01523226f,     0.01512652f,
    0.01500891f,     0.01487962f,     0.01473887f,     0.01458687f,     0.01442387f,
    0.01425012f,     0.01406591f,     0.01387153f,     0.01366728f,     0.01345349f,
    0.01323049f,     0.01299863f,     0.01275828f,     0.01250980f,     0.01225359f,
    0.01199004f,     0.01171955f,     0.01144254f,     0.01115943f,     0.01087065f,
    0.01057664f,     0.01027783f,     0.00997467f,     0.00966761f,     0.00935710f,
    0.00904360f,     0.00872756f,     0.00840943f,     0.00808967f,     0.00776873f,
    0.00744706f,     0.00712511f,     0.00680332f,     0.00648212f,     0.00616196f,
    0.00584325f,     0.00552641f,     0.00521185f,     0.00489998f,     0.00459119f,
    0.00428585f,     0.00398435f,     0.00368703f,     0.00339425f,     0.00310634f,
    0.00282363f,     0.00254642f,     0.00227501f,     0.00200968f,     0.00175070f,
    0.00149831f,     0.00125275f,     0.00101425f,     0.00078299f,     0.00055919f,
    0.00034299f,     0.00013457f,     -0.00006595f,     -0.00025844f,     -0.00044280f,
    -0.00061894f,     -0.00078678f,     -0.00094627f,     -0.00109737f,     -0.00124006f,
    -0.00137434f,     -0.00150020f,     -0.00161768f,     -0.00172681f,     -0.00182764f,
    -0.00192026f,     -0.00200473f,     -0.00208115f,     -0.00214965f,     -0.00221033f,
    -0.00226333f,     -0.00230880f,     -0.00234690f,     -0.00237780f,     -0.00240168f,
    -0.00241871f,     -0.00242912f,     -0.00243309f,     -0.00243085f,     -0.00242261f,
    -0.00240861f,     -0.00238907f,     -0.00236425f,     -0.00233438f,     -0.00229971f,
    -0.00226049f,     -0.00221699f,     -0.00216945f,     -0.00211814f,     -0.00206331f,
    -0.00200523f,     -0.00194415f,     -0.00188034f,     -0.00181405f,     -0.00174554f,
    -0.00167506f,     -0.00160287f,     -0.00152920f,     -0.00145430f,     -0.00137842f,
    -0.00130177f,     -0.00122460f,     -0.00114712f,     -0.00106955f,     -0.00099210f,
    -0.00091496f,     -0.00083834f,     -0.00076242f,     -0.00068739f,     -0.00061340f,
    -0.00054063f,     -0.00046923f,     -0.00039934f,     -0.00033111f,     -0.00026466f,
    -0.00020011f,     -0.00013757f,     -0.00007714f,     -0.00001891f,     0.00003704f,
    0.00009063f,     0.00014179f,     0.00019048f,     0.00023664f,     0.00028024f,
    0.00032125f,
};

void FIR_Process_Buffer(float *in, float *out, int numElem, float *state, int *pIdx, const float *taps, int numTaps);
float RMS_calculator(float *s, int numElem);

// Estados para los filtros (Lo que mantiene la memoria)
static float state_comb[COMB_DELAY] = {0};
static int idx_comb = 0;

static float state_y2[FIR_TAP1] = {0};
static int idx_y2 = 0;

static float state_y3[FIR_TAP2] = {0};
static int idx_y3 = 0;

// Parámetros del Comb
const float alpha = 0.18f; // 2/11
const float beta  = 0.81f; // 9/11

void my_app(void)
{
	static float modul = 0.5;
	volatile uint16_t *adc_in = GetBufferIn();
	volatile uint16_t *dac_out = SetBufferOut();

	TurnON(TP0);

	// Señales a implementar
	static float y1[BUFFER_SIZE];
	static float y2[BUFFER_SIZE];
	static float y3[BUFFER_SIZE];

	int i,j;

	for (int i = 0; i < BUFFER_SIZE; i++)
	{
	    float x_n = (float)adc_in[i] - 2048.0f;

	    // Leemos el valor que tiene el delay (la salida de hace D muestras)
	    volatile float delayed_y = state_comb[idx_comb];

	    // Calculamos la salida actual siguiendo y[n] = a*x[n] + b*y[n-D]
	    y1[i] = (alpha * x_n) + (beta * delayed_y);

	    // Guardamos la salida actual en el buffer de estado para que sea
	    //    el "delayed_y" dentro de D muestras
	    state_comb[idx_comb] = y1[i];

	    // Avanzamos el puntero circular
	    idx_comb++;
	    if (idx_comb >= COMB_DELAY) idx_comb = 0;
	}

	FIR_Process_Buffer(y1, y2, BUFFER_SIZE, state_y2, &idx_y2, taps_FIR1, FIR_TAP1);
	FIR_Process_Buffer(y1, y3, BUFFER_SIZE, state_y3, &idx_y3, taps_FIR2, FIR_TAP2);

	float V0 = RMS_calculator(y3, BUFFER_SIZE);
	float Vt = RMS_calculator(y2, BUFFER_SIZE);

	float THD = sqrtf(Vt * Vt - V0 * V0)/V0;

	for (int i = 0; i < BUFFER_SIZE; i++)
		dac_out[i]= (uint16_t)(THD * 4095.0f);

	modul *=alternate;
	TurnOFF(TP0);

}

/**
 * @brief Procesa un buffer completo de entrada y genera uno de salida.
 * @param in       Puntero al buffer de entrada (ej: y1_buffer)
 * @param out      Puntero al buffer donde se guardará la salida (ej: y2_buffer)
 * @param numElem  Tamaño del buffer (BUFFER_SIZE, ej: 1024)
 * @param state    Array de historial (debe ser estático y de tamaño numTaps)
 * @param pIdx     Puntero al índice actual del buffer circular (mantiene el estado entre bloques)
 * @param taps     Array de coeficientes
 * @param numTaps  Cantidad de coeficientes (281)
 */
void FIR_Process_Buffer(float *in, float *out, int numElem, float *state, int *pIdx, const float *taps, int numTaps)
{
    // Bucle que recorre cada muestra del buffer de entrada
    for (int i = 0; i < numElem; i++)
    {
        // Guardar la muestra actual en el buffer circular de estado
        state[*pIdx] = in[i];

        // Convolución: Multiplicar y Acumular (MAC)
        float acc = 0.0f;
        int currIdx = *pIdx;

        for (int j = 0; j < numTaps; j++)
        {
            acc += taps[j] * state[currIdx];

            // Retroceder en el buffer circular
            currIdx--;
            if (currIdx < 0) {
                currIdx = numTaps - 1;
            }
        }

        // Escribir el resultado en el buffer de salida
        out[i] = acc;

        // Avanzar el puntero del buffer circular para la PRÓXIMA muestra
        *pIdx = (*pIdx + 1) % numTaps;
    }
}


float RMS_calculator(float *s, int numElem)
{
    float acc = 0.0f;
    for (int i = 0; i < numElem; i++)
    {
        float val = s[i];
        acc += val * val;
    }

    // Solo una división y una raíz al final
    return sqrtf(acc / (float)numElem);
}

