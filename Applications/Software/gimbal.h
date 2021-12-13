/*
 * @Author: your name
 * @Date: 2021-12-06 11:35:13
 * @LastEditTime: 2021-12-09 11:16:27
 */
#define __GIMBAL_H__
#ifdef __GIMBAL_H__

struct gimbal_status
{
    struct
    {
        float set, now, last, offset;
        float stable;
    } pitch;

    struct
    {
        float set, now, last, offset;
        float stable;
    } yaw;

    struct
    {
        float now, last;
        float stable;
    } roll;
};

struct gimbal_status gimbal;

//外部调用
void gimbal_init(void);                         //云台初始化
void gimbal_set_offset(float pitch, float yaw); //设置云台初始值
void gimbal_updata(void);                       //更新云台数据
//end of file

#endif
