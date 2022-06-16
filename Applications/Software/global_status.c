#include "global_status.h"

struct GlobalStatus_t Global_status;

void change_gimbal_visual_status(enum gimbal_visual_e in)
{
    Global_status.gimbal_with_visual = in;
}

// �����������ݲ�������
void change_cap_status(enum cap_e in)
{
    Global_status.cap = in;
}

enum gimbal_visual_e read_gimbal_visual_status()
{
    return Global_status.gimbal_with_visual;
}

enum cap_e read_cap_status()
{
    return Global_status.cap;
}

//end of file