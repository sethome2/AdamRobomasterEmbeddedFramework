//2021-7-12 sethome

//�������ṹ��
typedef struct
{
	long long count;//�ܼ���
	
	float speed;								 //�ٶ�
	unsigned long long last_time;//��һ�θ��µ�ʱ��
	
} Encode_t;

void init_Encoder(Encode_t *target);		  //��ʼ������������
void update_Encoder(Encode_t *target, int B); //���±���������
