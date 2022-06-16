#ifndef __ARM_H__
#define __ARM_H__

struct Arm_t
{
  struct
  {
    float setAngle;
		float setGNDangle;
		
		float vector[2];
		float GNDangle;
		
		float offset;
  } a_d1; // Arm first d1

  struct
  {
    float setAngle;
		float setGNDangle;
		
		float vector[2];
		float GNDangle;
		
		float offset;
  } a_d2; // Arm second

  struct
  {
    /* data */
    float cal_speed;
		float now_speed;
		
    int sendCurrrent;
		
		float nowAngle;
    float setAngle;
		
		float vector[2];
  } a_s; //Ä©¶ËÖ´ÐÐ»ú¹¹
};

extern struct Arm_t Arm_status;

void ARM_init(void);
void ARM_update(void);
void ARM_set_GND_angle(float d1,float d2);
void ARM_set_MAN_angle(float d1,float d2);
void ARM_set_angle(float d1,float d2);
void ARM_set_s_offset(float set);

#endif
