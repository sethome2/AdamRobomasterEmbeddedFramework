#include "Kalman_Filter_c.h"

void Kalman_init(Attitude_3D_Kalman* filter,float Abtastzeit_s, float Qyaw, float Qpitch_roll, float Qgyrobias, float Ryaw, float Rpitch_roll) 
{
    filter->Abtastzeit_s = Abtastzeit_s;
    filter->Qyaw = Qyaw;
    filter->Qpitch_roll = Qpitch_roll;
    filter->Qgyrobias = Qgyrobias;
    filter->Ryaw = Ryaw;
    filter->Rpitch_roll = Rpitch_roll;
    filter->P1_1 = filter->P2_2 = filter->P3_3 = filter->P4_4 = filter->P5_5 = filter->P6_6 = 1.;
    filter->P3_4 = filter->P3_5 = filter->P3_6 = filter->P2_5 = filter->P2_6 = filter->P1_6 = 0.;
    filter->yaw = filter->pitch = filter->roll = 0.;
    filter->xbias = 0.;
    filter->ybias = 0.;
    filter->zbias = 0.;
}

void Kalman_update(Attitude_3D_t* result, Attitude_3D_Kalman* filter, float Acc_X, float Acc_Y, float Acc_Z, float Gyro_X, float Gyro_Y, float Gyro_Z, float Mag_X, float Mag_Y, float Mag_Z) 
	{
    /*********  Variablen  **********/
    //yaw pitch roll
    float measurement[3];

    //Innovation, yaw pitch roll
    float y[3];

    //x状态值
    float K1_1, K6_1, K2_2, K5_2, K6_2, K3_3, K4_3, K5_3, K6_3;

    /*********  测量值计算  **********/
    //Winkel in RAD für Winkelfunktionen
    float pitch_rad = (double)filter->pitch * DEG_TO_RAD;
    float roll_rad = (double)filter->roll * DEG_TO_RAD;

    //Pitch und Roll aus Accelerometer
    measurement[2] = (double)atanf(Acc_Y / sqrtf(Acc_X * Acc_X + Acc_Z * Acc_Z)) * RAD_TO_DEG;
    measurement[1] = (double)atanf(-1 * Acc_X / sqrtf(Acc_Y * Acc_Y + Acc_Z * Acc_Z)) * RAD_TO_DEG;

    //Magnetometer Messung in horizontale Ebene transformieren
    float XH = Mag_X * cosf(pitch_rad) + Mag_Z * cosf(roll_rad) * sinf(pitch_rad) + Mag_Y * sinf(pitch_rad) * sinf(roll_rad);
    float YH = Mag_Y * cosf(roll_rad) - Mag_Z * sinf(roll_rad);

    //Yaw mit Magnetometer berechnen
    measurement[0] = (double)atan2f(XH, YH) * RAD_TO_DEG;

    //prevent 360deg jump
    if ((measurement[0] - filter->yaw) > 180)
    {
        measurement[0] -= 360;
    }
    else if ((measurement[0] - filter->yaw) < -180)
    {
        measurement[0] += 360;
    }

    /*********  Kalman Algorithmus  **********/
    //new state estimates mit Vorwärtstransitions Matrix
    filter->yaw += ((cosf(roll_rad) * (Gyro_Z - filter->zbias)) / cosf(pitch_rad) + (sinf(roll_rad) * (Gyro_Y - filter->ybias)) / cosf(pitch_rad)) * filter->Abtastzeit_s;
    filter->pitch += (cosf(roll_rad) * (Gyro_Y - filter->ybias) - sinf(roll_rad) * (Gyro_Z - filter->zbias)) * filter->Abtastzeit_s;
    filter->roll += (Gyro_X - filter->xbias + cosf(roll_rad) * tanf(pitch_rad) * (Gyro_Z - filter->zbias) + tanf(pitch_rad) * sinf(roll_rad) * (Gyro_Y - filter->ybias)) * filter->Abtastzeit_s;

    //new error covariance estimate
    float P4_4_temp = filter->P4_4;
    float P5_5_temp = filter->P5_5;
    float P6_6_temp = filter->P6_6;

    filter->P1_1 = filter->P1_1 + filter->Qyaw - (filter->P1_6 * filter->Abtastzeit_s * cosf(roll_rad)) / cosf(pitch_rad) - (filter->Abtastzeit_s * cosf(roll_rad) * (filter->P1_6 - (filter->P6_6 * filter->Abtastzeit_s * cosf(roll_rad)) / cosf(pitch_rad))) / cosf(pitch_rad);

    filter->P2_2 = filter->P2_2 + filter->Qpitch_roll - filter->Abtastzeit_s * cosf(roll_rad) * (filter->P2_5 - filter->P5_5 * filter->Abtastzeit_s * cosf(roll_rad)) + filter->Abtastzeit_s * sinf(roll_rad) * (filter->P2_6 + filter->P6_6 * filter->Abtastzeit_s * sinf(roll_rad)) - filter->P2_5 * filter->Abtastzeit_s * cosf(roll_rad) + filter->P2_6 * filter->Abtastzeit_s * sinf(roll_rad);

    filter->P3_3 = filter->P3_3 + filter->Qpitch_roll - filter->P3_4 * filter->Abtastzeit_s - filter->Abtastzeit_s * (filter->P3_4 - filter->P4_4 * filter->Abtastzeit_s) - filter->Abtastzeit_s * tanf(pitch_rad) * sinf(roll_rad) * (filter->P3_5 - filter->P5_5 * filter->Abtastzeit_s * tanf(pitch_rad) * sinf(roll_rad)) - filter->P3_6 * filter->Abtastzeit_s * cosf(roll_rad) * tanf(pitch_rad) - filter->P3_5 * filter->Abtastzeit_s * tanf(pitch_rad) * sinf(roll_rad) - filter->Abtastzeit_s * cosf(roll_rad) * tanf(pitch_rad) * (filter->P3_6 - filter->P6_6 * filter->Abtastzeit_s * cosf(roll_rad) * tanf(pitch_rad));

    filter->P4_4 += filter->Qgyrobias;
    filter->P5_5 += filter->Qgyrobias;
    filter->P6_6 += filter->Qgyrobias;

    filter->P1_6 -= (P6_6_temp * filter->Abtastzeit_s * cosf(roll_rad)) / cosf(pitch_rad);
    filter->P2_5 -= P5_5_temp * filter->Abtastzeit_s * cosf(roll_rad);
    filter->P2_6 += P6_6_temp * filter->Abtastzeit_s * sinf(roll_rad);
    filter->P3_4 -= P4_4_temp * filter->Abtastzeit_s;
    filter->P3_5 -= P5_5_temp * filter->Abtastzeit_s * tanf(pitch_rad) * sinf(roll_rad);
    filter->P3_6 -= P6_6_temp * filter->Abtastzeit_s * cosf(roll_rad) * tanf(pitch_rad);

    //Innovation berechnen
    y[0] = measurement[0] - filter->yaw;
    y[1] = measurement[1] - filter->pitch;
    y[2] = measurement[2] - filter->roll;

    //Kalman Gain berechnen
    K1_1 = filter->P1_1 / (filter->P1_1 + filter->Ryaw);
    K2_2 = filter->P2_2 / (filter->P2_2 + filter->Rpitch_roll);
    K3_3 = filter->P3_3 / (filter->P3_3 + filter->Rpitch_roll);
    K4_3 = filter->P3_4 / (filter->P3_3 + filter->Rpitch_roll);
    K5_2 = filter->P2_5 / (filter->P2_2 + filter->Rpitch_roll);
    K5_3 = filter->P3_5 / (filter->P3_3 + filter->Rpitch_roll);
    K6_1 = filter->P1_6 / (filter->P1_1 + filter->Ryaw);
    K6_2 = filter->P2_6 / (filter->P2_2 + filter->Rpitch_roll);
    K6_3 = filter->P3_6 / (filter->P3_3 + filter->Rpitch_roll);

    //corrected state estimate
    filter->yaw += K1_1 * y[0];
    filter->pitch += K2_2 * y[1];
    filter->roll += K3_3 * y[2];

    filter->xbias += K4_3 * y[2];
    filter->ybias += K5_2 * y[1] + K5_3 * y[2];
    filter->zbias += K6_1 * y[0] + K6_2 * y[1] + K6_3 * y[2];

    //corrected error covariance matrix
    filter->P1_1 -= filter->P1_1 * K1_1;
    filter->P2_2 -= filter->P2_2 * K2_2;
    filter->P3_3 -= filter->P3_3 * K3_3;
    filter->P4_4 -= filter->P3_4 * K4_3;
    filter->P5_5 -= (K5_2 * filter->P2_5 + K5_3 * filter->P3_5);
    filter->P6_6 -= (K6_1 * filter->P1_6 + K6_2 * filter->P2_6 + K6_3 * filter->P3_6);

    filter->P1_6 -= filter->P1_6 * K1_1;
    filter->P2_5 -= filter->P2_5 * K2_2;
    filter->P2_6 -= filter->P2_6 * K2_2;
    filter->P3_4 -= filter->P3_4 * K3_3;
    filter->P3_5 -= filter->P3_5 * K3_3;
    filter->P3_6 -= filter->P3_6 * K3_3;

    //contrain yaw to +-180deg
    if (filter->yaw > 180)
    {
        filter->yaw -= 360;
    }
    else if (filter->yaw < -180)
    {
        filter->yaw += 360;
    }

    Attitude_3D_t return_Data = {filter->yaw, filter->pitch, filter->roll, Gyro_X-filter->xbias, Gyro_Y-filter->ybias, Gyro_Z-filter->zbias};
    *result = return_Data;
}
