#ifndef __HMC5883_H_
#define __HMC5883_H_	

extern float Mag_Data[3];


#define	HMC5883L_Addr   0x3C	//�ų�������������ַ 
void HMC5883L_Initial(void);
void HMC5883L_Read(void);
void HMC5883L_StateMachine(void);
extern float HMC5883L_Yaw; 
extern float MAGData[3];
#endif  


