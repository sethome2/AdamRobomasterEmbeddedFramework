
#ifndef KALMAN_FILTER_IMU_KALMAN_FILTER_C_H
#define KALMAN_FILTER_IMU_KALMAN_FILTER_C_H

#endif //KALMAN_FILTER_IMU_KALMAN_FILTER_C_H

#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Attitude_3D_t
{
    float yaw;
    float pitch;
    float roll;
    float unbiased_gyro_x;
    float unbiased_gyro_y;
    float unbiased_gyro_z;
} Attitude_3D_t;

typedef struct Attitude_3D_Kalman {
    //Abtastzeit des Filters für Integration [Sekunden]#
    float Abtastzeit_s;

    //Varianzen der Zustände
    float Qyaw, Qpitch_roll, Qgyrobias;

    //Varianzen der Messungen
    float Ryaw, Rpitch_roll;

    //Zustände
    float yaw;
    float pitch;
    float roll;
    float xbias;
    float ybias;
    float zbias;

    //error covariance matrix, nur elemente ungleich 0, außerdem symmetrische Matrix
    float P1_1, P2_2, P3_3, P4_4, P5_5, P6_6;
    float P3_4, P3_5, P3_6, P2_5, P2_6, P1_6;
} Attitude_3D_Kalman;

void Kalman_init(Attitude_3D_Kalman* filter,float Abtastzeit_s, float Qyaw, float Qpitch_roll, float Qgyrobias, float Ryaw, float Rpitch_roll);

void Kalman_update(Attitude_3D_t * result, Attitude_3D_Kalman * filter, float Acc_X, float Acc_Y, float Acc_Z, float Gyro_X, float Gyro_Y, float Gyro_Z, float Mag_X, float Mag_Y, float Mag_Z);

#ifdef __cplusplus
}
#endif

