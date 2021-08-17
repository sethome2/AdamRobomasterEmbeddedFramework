#include "math.h"
#include "pid.h"

void pid_set(pid_t *PidSet,float p_set,float i_set,float d_set,float lim_out_set,float lim_i_outset)//PID设置
{
  PidSet->p = p_set;   PidSet->i = i_set;   PidSet->d = d_set;
	PidSet->p_out = PidSet->i_out = PidSet->d_out = 0;
  PidSet->lim_out = lim_out_set;   PidSet->lim_i_out = lim_i_outset;//将设置赋值
  return;
}

//PID计算
float pid_cal(pid_t *PidGoal,float Now,float Set)//PID??
{
	PidGoal->set = Set;
  PidGoal->err_last = PidGoal->err;
  PidGoal->err = Set - Now;//计算误差
 
  PidGoal->p_out = PidGoal->p * PidGoal->err;
  PidGoal->i_out += PidGoal->i * PidGoal->err;
  PidGoal->d_out = PidGoal->d * (PidGoal->err - PidGoal->err_last);//pid运算
  
  if(fabs(PidGoal->i_out) > PidGoal->lim_i_out)//防止积分过大
  {
    if(PidGoal->i_out < 0)
      PidGoal->i_out = -PidGoal->lim_i_out;
    else
      PidGoal->i_out = PidGoal->lim_i_out;
  }
	
  PidGoal->total_out = PidGoal->p_out + PidGoal->i_out + PidGoal->d_out;//计算总和输出  

	if(fabs(PidGoal->total_out) > PidGoal->lim_out)//防止总和过大
  {
    if(PidGoal->total_out < 0)
      PidGoal->total_out = -PidGoal->lim_out;
    else
      PidGoal->total_out = PidGoal->lim_out;
  }
	
  return PidGoal->total_out;
}
