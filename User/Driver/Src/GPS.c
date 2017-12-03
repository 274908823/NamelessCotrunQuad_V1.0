#include "Headfile.h"
#include "GPS.h"
#include "USART.h"
#include <math.h>
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------/
        *               ������ֻ��������ѧϰʹ�ã���Ȩ����Ȩ���������ƴ��Ŷӣ�
        *               �����ƴ��Ŷӽ��ɿس���Դ���ṩ�������ߣ�
        *               ������ҪΪ�����ƴ��Ŷ��ṩ������
        *               δ��������ɣ����ý�Դ�����ṩ������
        *               ���ý�Դ����ŵ����Ϲ�����������أ�
        *               �������Դ�����Ĳ�����緢��������Ϊ��
        *               �����ƴ��Ŷӽ���֮�Է��ɽ��������
-----------------------------------------------------------------------------------------------------------------------/
        *               ������Ϣ���ܶ���ֹ��ǰ�����������˳���������
        *               ��Դ���ף���ѧ����ϧ��ף������Ϯ�����׳ɹ�������
-----------------------------------------------------------------------------------------------------------------------/
	*		�����ƴ���Դ�ɿ� V1.1	�人�Ƽ���ѧ  By.YuYi
	*		CSDN����: http://blog.csdn.net/u011992534
	*               �ſ�ID��NamelessCotrun����С��
	*               �����ƴ���Դ�ɿ�QQȺ��540707961
        *               https://shop348646912.taobao.com/?spm=2013.1.1000126.2.5ce78a88ht1sO2
        *               �ٶ�����:�����ƴ���Դ�ɿ�
        *               �޸�����:2017/10/30
        *               �汾��V1.1
        *               ��Ȩ���У�����ؾ���
        *               Copyright(C) �人�Ƽ���ѧ�����ƴ��Ŷ� 2017-2019
        *               All rights reserved
----------------------------------------------------------------------------------------------------------------------*/
double Last_Longitude=0,Last_Latitude=0;
int32_t Longitude_Origion=0,Latitude_Origion=0;//PVT����������ԭʼ��γ�ȣ��൱������deg*10^7
double Longitude,Latitude;
double Longitude_Deg,Latitude_Deg,Longitude_Min,Latitude_Min;
float GPS_Ground_Speed=0;//�Ե��ٶ�
float GPS_Yaw=0;//�˶�����
float GPS_Quality=0;//��λ����
uint16 GPS_Sate_Num=0;//���붨λ������
float GPS_Speed_Resolve[2]={0,0};//GPS�۲��ٶ�
u16 TimeBeijing[3];//����ʱ��
char TimeStr[8];
Vector3_Nav GPS_Vel={0};//NED���ٶ�
float GPS_Pos_DOP=0;//��λ��������//��λ����
uint8 GPS_FixType=0;//��λ����
uint8 GPS_Fix_Flag[4]={0};//��λ��־
int16 Horizontal_Acc_Est=0;//ˮƽλ�þ���
int16 Vertical_Acc_Est=0;//��ֱλ�þ���
int16 Speed_Acc_Est=0;//��ֱλ�þ���
float High_GPS=0;//���θ߶�
void GPS_PVT_Parse(void)
{
  Last_Longitude=Longitude;
  Last_Latitude=Latitude;
  //����ʱ��
  TimeBeijing[0]=Ublox_Data[9]+8;//ʱ
  TimeBeijing[1]=Ublox_Data[10];//��
  TimeBeijing[2]=Ublox_Data[11];//��

  GPS_FixType=Ublox_Data[21];//��λ����

  GPS_Fix_Flag[0]=Ublox_Data[23]&0x01;//�Ƿ���Ч��λ
  GPS_Fix_Flag[1]=(Ublox_Data[23]&0x02)>>1;//���ģʽ�Ƿ���(DGPS)
  GPS_Fix_Flag[2]=(Ublox_Data[23]&0x3A)>>2;//ʡ��ģʽ��δ��
  GPS_Fix_Flag[3]=Ublox_Data[23]&0x20;//�����Ƿ���Ч

  GPS_Sate_Num=Ublox_Data[24];//���붨λ���Ǹ���

  Longitude_Origion=Ublox_Data[25]//����*10^7
             +(Ublox_Data[26]<<8)
              +(Ublox_Data[27]<<16)
               +(Ublox_Data[28]<<24);
  Longitude=Longitude_Origion*0.0000001f;//deg


  Latitude_Origion=Ublox_Data[29]//γ��*10^7
             +(Ublox_Data[30]<<8)
              +(Ublox_Data[31]<<16)
               +(Ublox_Data[32]<<24);
  Latitude=Latitude_Origion*0.0000001f;//deg


  Longitude_Deg=(int)(Longitude);//��γ�ȴ�������OLED��ʾ
  Longitude_Min=((int)((Longitude-Longitude_Deg)*10000000));
  Latitude_Deg=(int)(Latitude);
  Latitude_Min=((int)((Latitude-Latitude_Deg)*10000000));


  High_GPS=Ublox_Data[37]//GPS��ȡ���θ߶�
             +(Ublox_Data[38]<<8)
              +(Ublox_Data[39]<<16)
               +(Ublox_Data[40]<<24);
  High_GPS/=1000;//m


  Horizontal_Acc_Est=Ublox_Data[41]//ˮƽλ�ù��ƾ���
             +(Ublox_Data[42]<<8)
              +(Ublox_Data[43]<<16)
               +(Ublox_Data[44]<<24);;
  Horizontal_Acc_Est*=0.01;//m


  Vertical_Acc_Est=Ublox_Data[45]//��ֱλ�ù��ƾ���
             +(Ublox_Data[46]<<8)
              +(Ublox_Data[47]<<16)
               +(Ublox_Data[48]<<24);;
  Vertical_Acc_Est*=0.01;//m


  GPS_Vel.N=Ublox_Data[49]//GPS��ȡ���ص���ϵ�����ٶ�
             +(Ublox_Data[50]<<8)
              +(Ublox_Data[51]<<16)
               +(Ublox_Data[52]<<24);
  GPS_Vel.N/=10;//cm/s  N


  GPS_Vel.E=Ublox_Data[53]//GPS��ȡ���ص���ϵ�������ٶ�
             +(Ublox_Data[54]<<8)
              +(Ublox_Data[55]<<16)
               +(Ublox_Data[56]<<24);//mm/s
  GPS_Vel.E/=10;//cm/s  E


  GPS_Vel.U=Ublox_Data[57]//GPS��ȡ���ص���ϵ '��' ���ٶ�
             +(Ublox_Data[58]<<8)
              +(Ublox_Data[59]<<16)
               +(Ublox_Data[60]<<24);
  GPS_Vel.U/=(-10);//cm/s  D

  GPS_Speed_Resolve[0]=GPS_Vel.N;//Y  Axis
  GPS_Speed_Resolve[1]=GPS_Vel.E;//X  Axis


  GPS_Ground_Speed=Ublox_Data[61]//����Ե��ٶ�
             +(Ublox_Data[62]<<8)
              +(Ublox_Data[63]<<16)
               +(Ublox_Data[64]<<24);
  GPS_Ground_Speed/=10;//cm/s


  GPS_Yaw=Ublox_Data[65]//�����˶������
             +(Ublox_Data[66]<<8)
              +(Ublox_Data[67]<<16)
               +(Ublox_Data[68]<<24);
  GPS_Yaw*=0.00001;//deg

  Speed_Acc_Est=Ublox_Data[69]//�ٶȹ��ƾ���
             +(Ublox_Data[70]<<8)
              +(Ublox_Data[71]<<16)
               +(Ublox_Data[72]<<24);
  Speed_Acc_Est*=0.1;//cm/s


  GPS_Pos_DOP=Ublox_Data[77]
             +(Ublox_Data[78]<<8);
  GPS_Pos_DOP*=0.01;
  GPS_Quality=GPS_Pos_DOP;//λ�þ�������
}

