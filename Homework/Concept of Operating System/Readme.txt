ʵ��2 �ӽ����м���쳲���������
��Ҫʹ�������д򿪣�����Win8/Win10ϵͳ�У�������Ͻ�ѡ�񡰴�������ʾ����������"SE2 ��쳲��������г��ȣ�"���С����磬����"SE2 10"����ʾ"0 1 1 2 3 5 8 13 21 34 55"����11��ӵ�0�ʼ����

ʵ��3 ģ��FIFO��LRUҳ���û��㷨
ֱ�����У�����ʾ"Input the total number of frames:"������֡������ĿҪ��������1-7��������ʾ"Input the count of reference (Negative Number to Quit):"�������ж��ٴ�ҳ����ʡ����������һ��ҳ���0-9�����У�Ȼ����FIFO��LRU���м��㡣����'q'����������

���磺
Input the total number of frames:5
Input the count of reference (Negative Number to Quit):15
FIFO
1               5       -1      -1      -1      -1              Now Referring:5
2               5       4       -1      -1      -1              Now Referring:4
3               5       4       1       -1      -1              Now Referring:1
4               5       4       1       3       -1              Now Referring:3
5               5       4       1       3       -1              Now Referring:5
6               5       4       1       3       -1              Now Referring:4
7               5       4       1       3       9               Now Referring:9
8               7       4       1       3       9               Now Referring:7
9               7       0       1       3       9               Now Referring:0
10              7       0       1       3       9               Now Referring:3
11              7       0       2       3       9               Now Referring:2
12              7       0       2       1       9               Now Referring:1
13              7       0       2       1       3               Now Referring:3
14              7       0       2       1       3               Now Referring:3
15              7       0       2       1       3               Now Referring:7
Total Faults: 10

LRU
1               5       -1      -1      -1      -1              Now Referring:5
2               5       4       -1      -1      -1              Now Referring:4
3               5       4       1       -1      -1              Now Referring:1
4               5       4       1       3       -1              Now Referring:3
5               5       4       1       3       -1              Now Referring:5
6               5       4       1       3       -1              Now Referring:4
7               5       4       1       3       9               Now Referring:9
8               5       4       7       3       9               Now Referring:7
9               5       4       7       0       9               Now Referring:0
10              3       4       7       0       9               Now Referring:3
11              3       2       7       0       9               Now Referring:2
12              3       2       7       0       1               Now Referring:1
13              3       2       7       0       1               Now Referring:3
14              3       2       7       0       1               Now Referring:3
15              3       2       7       0       1               Now Referring:7
Total Faults: 10
