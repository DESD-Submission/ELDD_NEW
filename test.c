#include<stdio.h>
#include<math.h>
#include<sys/msg.h>
#include<pthread.h>

#include<stdlib.h>
#include<unistd.h>
#define MQ_KEY 0x2564
typedef struct msgbuf{
	long type;
	int num;
}msg_t;


void *thread1(void *param){
	int *pass = (int *)param;
	int mqid = *pass; 

	int *num,ret;
	msg_t m1;
	m1.type = 25;
	printf("Enter the number :");
	scanf("%d",&m1.num);
	ret = msgsnd(mqid,&m1,sizeof(m1) - sizeof(long),0);
	if(ret < 0){
		perror("msgsnd1 failed");
		exit(0);
	}
	printf("The number sent\n");

	msg_t m2;
	m2.type = 50;
	printf("Waiting for squareroot....\n");
	ret = msgrcv(mqid,&m2,sizeof(m2) - sizeof(long),50,0);
	if(ret < 0){
		perror("msgrcv1 failed");
		exit(0);
	}
	printf("The squareroot recieved %d \n",m2.num);
	num = (int *)malloc(sizeof (int));
	*num = m2.num;
	return num;
}

void *thread2(void *param){
	int *pass = (int *)param;
	int mqid = *pass;

	msg_t m1;
	m1.type = 25;
	printf("Waiting for Number....\n");
	int ret = msgrcv(mqid,&m1,sizeof(m1) - sizeof(long),25,0);
	if(ret < 0){
		perror("msgrcv 2 failed");
		exit(0);
	}
	printf("The number recieved : %d\n",m1.num);
	int sq = sqrt(m1.num);

	msg_t m2;
	m2.type = 50;
	m2.num = sq;
	ret = msgsnd(mqid,&m2,sizeof(m2) - sizeof(long),0);
	if(ret < 0){
		perror("msgsnd 2 failed");
		exit(0);
	}
	printf("Square Root of %d is %d is sent..\n",m1.num,sq);

	int *num = (int *)malloc(sizeof (int));
	*num = m1.num;
	return num;


}
int main(void){

	pthread_t t1,t2;
	int	mqid = msgget(MQ_KEY,IPC_CREAT | 0600);
	if(mqid < 0){
		perror("MSG Queue failed");
		exit(0);
	}
	int ret,*org;
	char ch1[5],ch2[5];
	double *sqr;
	ret = pthread_create(&t1,NULL,thread1,&mqid);
	if(ret < 0){
		perror("first create thread failed");
		exit(0);
	}
	ret = pthread_create(&t2,NULL,thread2,&mqid);
	if(ret < 0){
		perror("second create thread failed");
		exit(0);
	}
	pthread_join(t1,(void **)&sqr);
	pthread_join(t2,(void **)&org);
	sprintf(ch1, "%d",*org);
	int *sqr1 = (int*) sqr;
	sprintf(ch2, "%d",*sqr1);
	ret = execlp("expr","expr",ch1,"+",ch2,NULL);
	if(ret < 0){
		perror("exec failed");
		exit(0);
	}
	msgctl(mqid, IPC_RMID, NULL);
	return 0;
}