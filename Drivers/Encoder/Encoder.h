//2021-7-12 sethome

//编码电机结构体
typedef struct
{
	long long count;//总计数
	
	float speed;								 //速度
	unsigned long long last_time;//上一次更新的时间
	
} Encode_t;

void init_Encoder(Encode_t *target);		  //初始化编码器变量
void update_Encoder(Encode_t *target, int B); //更新编码器变量
