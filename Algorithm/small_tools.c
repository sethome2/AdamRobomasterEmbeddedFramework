/*
 * @Author: sethome
 * @Date: 2021-12-09 11:31:38
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Algorithm\small_tools.c
 */
#include "small_tools.h"
#include "math.h"

//滑动方差
void sliding_variance_init(sliding_variance *obj) //不一定需要初始化，但要注意index要为0
{
  int i = 0;
  for (i = 0; i < 5; i++)
    obj->data[i] = 0;

  obj->index = 4;
}
float sliding_variance_cal(sliding_variance *obj, float new)
{
  int i = 0;
  float average = 0;
  float variance = 0;

  if (obj->index == 4)
    obj->index = 0;
  else
    obj->index++;
  obj->data[obj->index] = new;

  for (i = 0; i < 5; i++)
    average += obj->data[i] / 5.0f;

  for (i = 0; i < 5; i++)
    variance += fabs(obj->data[i] - average) / 5.0f;

  return variance;
}

void sliding_average_init(sliding_average *obj)
{
  for (int i = 0; i < 5; i++)
    obj->data[i] = 0;
  obj->sum;
  obj->index = 4;
}

float sliding_average_cal(sliding_average *obj, float new)
{
  obj->sum += new - obj->data[obj->index];
  obj->data[obj->index] = new;
  if (obj->index == 4)
    obj->index = 0;
  else
    obj->index++;

  return obj->sum / 5.0f;
}

//end of file
