//�������� yum
//�༭�� vim
//������ g++ / gcc
//������ gdb makefile git
//
//yum
//yum - y[packetname] ��װ
//yum remove ɾ��
//yum list չʾ
//
//����ģʽ����
//vim �� ɾ �� �� �� �� ��
//vim  filename  ��ͨģʽ
//i I o O
//����ģʽ
//������ģʽ
//
//�ƶ� h j k l
//ɾ�� x X  dd
//���� yy yw
//�滻 r R
//���� u ctrl
//���� cw c3w
//�� ctrl + g / 6G
//������ģʽ
//set nu / set nonu ��ʾ�к�
//s / src / tar / �滻��ǰ�еĵ�һ��
//%s / src / tar / �滻�����еĵ�һ��
//%s / src / tar / g  �滻����
//
//g++ / gcc
//gcc - E test.c - o test.i  Ԥ����
//gcc - S test.i - o test.s ����
//gcc - C test.s - o test.o ���
//* gcc  test.c - o out ��̬����
//* gcc  test.c - o out1 - static ��̬����
//gcc  test.c - o out - g ����debug�汾�Ŀ�ִ���ļ�
//md5sum out
//ldd out �鿴��ǰ�ļ�������Щ�ļ�
//file out �鿴�ļ�����
//ll - h �鿴�ļ���С
//
//gdb ����
//��ǰ
//list �鿴Դ��
//run
//breakpoint + �к�  ��ϵ�
//b �к� ��main�������ڵ��ļ���ϵ�
//b test.c:�к�
//enable ��� ʹ�ϵ���Ч
//disable ��� ʹ�ϵ㲻��Ч
//delete ��� ɾ���ϵ�
//info b  �鿴�ϵ�
//i b
//n ����� next
//s ����� step
//bt �鿴���ö�ջ backtrace / where
//c ����ִ�� continue
//�鿴������ֵ
//p ����
//i locals �鿴��ʱ������ֵ
//����
//ps aux �鿴��ǰ���н���
//ps aux | grep . / out �ܵ��鿴. / out(��ִ�г���)
//�鿴����
//gdb - p  PID �鿴����
//gdb attach PID �鿴����
//pstack �鿴���еĵ��ö�ջ
//
//�º�
//ulimit - a �鿴ϵͳ������Ϣ ulimit - c �޸Ĵ�С
//* core file size ����ת���ļ��Ĵ�С
//* gdb ��ִ�г��� coredumpfile(����ת���ļ�)
//bt �鿴���ö�ջ-- > �鿴��Ӧ��Դ������н���ȷ��-- > �����Ƿ��������ת���ļ���
//��һ�� / �����Ƿ���а汾���� / �Ƿ�Ϊdebug�汾 
//f ��ջ��
//
//make
//���� makefile �е�����
//linux makefile.gcc
//���� : makefile.inc
//	 win makefile.mvc
// ���� : makefile.inc
//  makefile :
//Ŀ����� : ��������
//	��������
//	1.�Ա����һ���޸�ʱ��, �����Ƿ�ִ�б�������
//	.PHONY : out αĿ��, ���αȽ��޸�ʱ��
//	2.�ж����������Ƿ����,
//	����, ִ�б�������
//	�����������������������ķ���, �Ҳ�������
//	3.ֻΪ���ɵ�һ��Ŀ�����
//
//	Ԥ�������
//	$^ ������������
//	$@ Ŀ�����
//	$ < �����ĵ�һ������
//clean : ����������, ִ��������
//
//		$ %.c %.a
//git clone
//git add
//git commit - m "������Ϣ"
//git push
//git status ��Ϣ
//git pull
