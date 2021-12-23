/*
 * @Author: sethome
 * @Date: 2021-12-09 11:32:02
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Algorithm\small_tool.h
 */

typedef struct 
{
    float data[5];
    unsigned char index;
}sliding_variance;
void sliding_variance_init(sliding_variance *obj);
float sliding_variance_cal(sliding_variance *obj, float new);

typedef struct 
{
    float data[5];
    float sum;
    unsigned char index;
}sliding_average;
void sliding_average_init(sliding_average *obj);
float sliding_average_cal(sliding_average *obj, float new);

//end of file