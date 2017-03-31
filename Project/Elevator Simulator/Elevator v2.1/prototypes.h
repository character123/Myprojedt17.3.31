//output.c
int openFileToWrite(char inFileName[] ,FILE * * fpout);
//�����ļ�������������ļ� ,�ļ��ṹ������fpoutָ�򡣴򿪳ɹ�������1�����򷵻�0

void set_fileOutput(FILE *fpout, int timeCount, struct ELEVATOR elevator, LISTNODEPTR headp, int detail);
//������״̬����Ϣ������ļ���

//state.c
void runService(struct ELEVATOR * elePtr, LISTNODEPTR headp, struct REQ * reqTempPtr);
//������������������״̬��������ǰ��Ӧ����reqTemp��
void deleteListNode (LISTNODEPTR listPtr);
//ɾ���ڵ�

void initElevator(struct ELEVATOR * elePtr);
//�Ե���״̬���г�ʼ��

void doorReqFunc(int req, struct ELEVATOR * elevatorPtr);

//graphic.c
DWORD WINAPI graphicOutProc(LPVOID lpParameter);
//�����������

//strategy.c
void addServList(int reqMode, LISTNODEPTR headPtr, struct REQ reqs[], int reqsSize,struct ELEVATOR * elevatorPtr, struct REQ * reqTempPtr); 
//��������ݲ��Բ�ͬ��������������ĺ���λ��

DWORD WINAPI addServListProc(LPVOID lpParameter);
//���Խ��̣�Ϊ˳�����������

void sortServList(LISTNODEPTR headPtr, struct ELEVATOR * elevatorPtr);
//˳�����Ĳ��Ժ�������������˳�������Խ���ʵʱ���

void deleteInsideReq(LISTNODEPTR headPtr, struct ELEVATOR * elevatorPtr);

//input.c
DWORD WINAPI keyboardInputProc(LPVOID lpParameter);
//����������̣�����keyboardinput���������addServList������reqs[]�����������������

void keyboardInput(struct REQ reqs[], int * serviceMethodPtr);
//�������뺯���������������洢��req[]������

void mouseInput(struct REQ reqs[], int * serviceMethodPtr);
//������뺯�������������ת������Ӧ������

DWORD WINAPI mouseInputProc(LPVOID lpParameter);
//���������̣�����mouseInput������addServList����
