//types.h ��������볣��


//���峣�õ�ʱ�䳣��
#define ONELAYERLEN 3  //��������һ����Ҫ��ʱ��Ƭ
#define STOPLEN 5    //����ͣ����ʱ��Ƭ
#define OPENLEN 3    //���ݿ�����Ҫ��ʱ��Ƭ
#define CLOSELEN 3   //���ݹ�����Ҫ��ʱ��Ƭ


//ö�ٳ���
enum STATE {IDLE,UP,DOWN,DOOROPENING,DOORCLOSING,STOP};
//ö�ٵ���״̬(���У����У����У����ţ����ţ�ͣ��)
enum ACTION {EMPTY=-1, INSIDE,OUTSIDEUP,OUTSIDEDOWN,OPEN,CLOSE};
//ö�ٵ��ݶ�������(���У��ڲ������������½������ţ�����) 


//����ṹ
struct ELEVATOR {
	enum STATE eleState;
	int nowF;
	int goalF;
	int stopTime;
	int stateTime;
};//���ݽṹ����¼����״̬������¥�㡢ͣ��ʱ����ʱ��Ƭ��
struct REQ{
	enum ACTION reqAct;
	int reqF;
};//�����¼�
struct LIST{
	struct REQ reqData;
	struct LIST * nextPtr;
};//�����������ڵ�

typedef struct LIST LISTNODE;
typedef LISTNODE * LISTNODEPTR;//��LIST�ṹ���м�

struct HANDLETRANS{
	LISTNODEPTR headp;//�������� 
	struct ELEVATOR * elevatorPtr; //����״̬ 
	int * serviceMethodPtr; //����ģʽ�ĵ�ַ 
	int * reqCountPtr; //�������ĵ�ַ 
	int * reqTimePtr; //����ʱ��ĵ�ַ
	struct REQ * reqTempPtr;
};//��������ݵĵ�ַ���������