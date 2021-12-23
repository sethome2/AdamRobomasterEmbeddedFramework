/*
 * @Author: sethome
 * @Date: 2021-12-09 11:31:38
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Algorithm\small_tools.c
 */
#include "small_tools.h"
#include "math.h"

//��������
void sliding_variance_init(sliding_variance *obj)//��һ����Ҫ��ʼ������Ҫע��indexҪΪ0
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
