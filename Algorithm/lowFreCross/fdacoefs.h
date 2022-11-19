/*
 * 离散时间 FIR 滤波器(实数)
 * ----------------
 * 滤波器结构  : 直接型 FIR
 * 滤波器长度  : 26
 * 稳定     : 是
 * 线性相位   : 是 (Type 2)
 */
/*
 * Warning - Filter coefficients were truncated to fit specified data type.  
 *   The resulting response may not match generated theoretical response.
 *   Use the Filter Design & Analysis Tool to design accurate
 *   single-precision filter coefficients.
 */
#define IMU_FIR_FILTER_LENGTH 26
const float IMU_FIR_COFFES[IMU_FIR_FILTER_LENGTH] = {
  0.0001979996014,-0.0006330574397, 0.001568097738,-0.003304710612, 0.006254237611,
   -0.01095288899,  0.01811391674, -0.02876197733,  0.04459231347, -0.06900795549,
     0.1107223779,   -0.201850757,   0.6330996752,   0.6330996752,   -0.201850757,
     0.1107223779, -0.06900795549,  0.04459231347, -0.02876197733,  0.01811391674,
   -0.01095288899, 0.006254237611,-0.003304710612, 0.001568097738,-0.0006330574397,
  0.0001979996014
};

/**
 * @brief 
 * 
 * @param state FIR滤波器状态 长度为FIR_FILTER_LENGTH + 1
 * @param x 
 * @param y 
 * @param N 
 */
void FIR_process(float state[], float *x,float *y,int N, const float FIR_coffes[], int FIR_len)
{
	int i = 0,j = 0,k=0;
	float temp; 
	for (k = 0; k < N; k++) 
	{ 
		state[0] = x[k];
		for (i = 0, temp = 0; i < FIR_len; i++)
			temp += FIR_coffes[i] * state[i];
		y[k] = temp;
		for (j = FIR_len - 1; j > -1 ; j--)
		state[j+1] = state[j];
	}
}
