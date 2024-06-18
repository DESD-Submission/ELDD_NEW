// fork 
// concurrent using child 

#include <unistd.h>
#include <stdio.h>

int main() {
	int ret;
	printf("program started..\n");
	ret = fork();
	if(ret == 0) {
		printf("child: fork() returned: %d\n", ret);
		printf("child: pid=%d\n", getpid());
		printf("child: parent pid=%d\n", getppid());
	}
	else {
		printf("parent: fork() returned: %d\n", ret);
		printf("parent: pid=%d\n", getpid());
		printf("parent: parent pid=%d\n", getppid());
	}
	printf("program completed..\n");
	getchar();
	return 0;
}


// exex load program from perticukar path

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int ret, err, s;
	printf("parent started!\n");
	ret = fork();
	if(ret == 0) {
		// child process
		//err = execl("/home/nilesh/mar-24/desd/eos/Day09/demo17_child.out", "demo17.out", "one", "two", "three", "four", "five", NULL);
		char *args[]={"demo17.out", "one", "two", "three", "four", "five", NULL}; 
		err = execv("/home/nilesh/mar-24/desd/eos/Day09/demo17_child.out", args);
		if(err < 0) {
			perror("exec() failed");
			_exit(1);
		}
	}
	else {
		// parent process
		wait(&s);
		printf("child exit status: %d\n", WEXITSTATUS(s));
	}
	printf("parent completed!\n");
	return 0;
}


		char *args[]={ "firefox", "http://sunbeaminfo.in", NULL };
		err = execvp("firefox", args);



// signal               (kill-l)


#include <stdio.h>
#include <signal.h>
#include <unistd.h>

//step1: implement SIGINT handler
void sigint_handler(int sig) {
	printf("SIGINT caught: %d\n", sig);
}

//step1: implement SIGTERM handler
void sigterm_handler(int sig) {
	printf("SIGTERM caught: %d\n", sig);
}

int main() {
	int i=1;
	//step2: register signal handler
	signal(SIGINT, sigint_handler);
	//step2: register signal handler
	signal(SIGTERM, sigterm_handler);
	while(1) {
		printf("running: %d\n", i++);
		sleep(1);
	}
	return 0;
}
------------------------------------------------------------------------------------------

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>

void sigchld_handler(int sig) {
	int s;
	wait(&s);
	printf("child exit status: %d\n", WEXITSTATUS(s));
}

int main() {
	int ret, i;
	signal(SIGCHLD, sigchld_handler);
	ret = fork();
	if(ret == 0) {
		// child process
		for(i=1; i<=5; i++) {
			printf("child: %d\n", i);
			sleep(1);
		}
		_exit(2);
	}
	else {
		// parent process
		for(i=1; i<=10; i++) {
			printf("parent: %d\n", i);
			sleep(1);
		}
	}
	return 0;
}
------------------------------------------------------------------------------------------------

// message q

#ifndef __DEMO36_H
#define __DEMO36_H

#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>

#define MQ_KEY	0x4321

typedef struct {
	long type;
	char str[32];
} msg_t;

#endif
-------------------------------

#include "demo36.h"

int main() {
	int mqid, ret;
	msg_t m1;
	// create message queue
	mqid = msgget(MQ_KEY, IPC_CREAT | 0600);
	if(mqid < 0) {
		perror("msgget() failed");
		_exit(1);
	}

	// send message
	printf("p1: enter a string: ");
	scanf("%s", m1.str);

	m1.type = 11;

	ret = msgsnd(mqid, &m1, sizeof(m1.str), 0);
	printf("p1: message sent: %d\n", ret);
	return 0;
}

----------------------------------

#include "demo36.h"

int main() {
	int mqid, ret;
	msg_t m2;
	// access message queue
	mqid = msgget(MQ_KEY, 0);
	if(mqid < 0) {
		perror("msgget() failed");
		_exit(1);
	}

	// receive message
	printf("p2: waiting for message...\n");
	ret = msgrcv(mqid, &m2, sizeof(m2.str), 11, 0);
	printf("p2: message received: %s\n", m2.str);
	// destroy message queue
	msgctl(mqid, IPC_RMID, NULL);
	return 0;
}

---------------------------------------------------------------------------------------------------


//sigaction

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

// common signal handler for multiple signals
void sig_handler(int sig) {
	printf("signal caught: %d\n", sig);
	switch(sig) {
	case SIGINT:
		printf("SIGINT handled\n");
		break;
	case SIGTERM:
		printf("SIGTERM handled\n");
		break;
	case SIGALRM:
		printf("SIGALRM handled\n");
		alarm(3); // send SIGALRM after 3 more seconds
		break;
	}
}

int main() {
	int i=1, ret;
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sig_handler;
	
	ret = sigaction(SIGINT, &sa, NULL);
	if(ret < 0) {
		perror("sigaction() failed");
		_exit(1);
	}

	ret = sigaction(SIGTERM, &sa, NULL);
	if(ret < 0) {
		perror("sigaction() failed");
		_exit(1);
	}
	
	ret = sigaction(SIGALRM, &sa, NULL);
	if(ret < 0) {
		perror("sigaction() failed");
		_exit(1);
	}
	
	alarm(3); // will send SIGALRM to current process after 3 seconds

	while(1) {
		printf("running: %d\n", i++);
		sleep(1);
	}
	return 0;
}

---------------------------------------------------------------------------------------------------


//PIPE

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
	int arr[2], ret, s;
	char datawr[32], datard[32];
	ret = pipe(arr);
	if(ret < 0) {
		perror("pipe() failed");
		_exit(1);
	}

	ret = fork();
	if(ret == 0) {
		// child -- writer process
		close(arr[0]); // close read fd
		printf("child: enter a string: ");
		scanf("%s", datawr);
		ret = write(arr[1], datawr, strlen(datawr)+1); // write data in pipe
		printf("child: data written in pipe: %d bytes\n", ret);
		close(arr[1]); // close write fd
	}
	else {
		// parent -- reader process
		close(arr[1]); // close write fd
		printf("parent: waiting for data...\n");
		ret = read(arr[0], datard, sizeof(datard)); // read data from pipe
		printf("parent: data read from pipe: %d bytes -- %s\n", ret, datard);
		close(arr[0]);	// close read fd
		waitpid(-1, &s, 0);
	}

	return 0;
}
--------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int ret1, ret2, err1, err2, s1, s2;
	int arr[2], ret;
	printf("parent started!\n");

	// create pipe
	ret = pipe(arr);
	if(ret < 0) {
		perror("pipe() failed");
		_exit(3);
	}

	// run "who" command
	ret1 = fork();
	if(ret1 == 0) {
		// dup pipe write fd to stdout
		close(arr[0]);
		dup2(arr[1], 1); 
		close(arr[1]);

		err1 = execlp("who", "who", NULL);
		if(err1 < 0) {
			perror("exec() failed to execute who cmd");
			_exit(1);
		}
	}

	// run "wc" command
	ret2 = fork();
	if(ret2 == 0) {
		// dup pipe read fd to stdin
		close(arr[1]);
		dup2(arr[0], 0);
		close(arr[0]);

		err2 = execlp("wc", "wc", NULL);
		if(err2 < 0) {
			perror("exec() failed to execute wc cmd");
			_exit(2);
		}
	}

	// close pipe both ends in parent (no write no read)
	close(arr[1]);
	close(arr[0]);
	waitpid(ret1, &s1, 0);
	waitpid(ret2, &s2, 0);
	printf("parent completed!\n");
	return 0;
}

------------------------------------------------------------------------------------------------

// Soket

// client
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
// client
#define SERV_PORT	2809
#define SERV_IP		"127.0.0.1"
int main() {
	int cli_fd, ret;
	struct sockaddr_in serv_addr;
	char str[64];
	cli_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(cli_fd < 0) {
		perror("socket() failed");
		_exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	inet_aton(SERV_IP, &serv_addr.sin_addr);
	ret = connect(cli_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(ret < 0) {
		perror("connect() failed");
		_exit(2);
	}

	do {
		printf("client: ");
		gets(str);
		write(cli_fd, str, strlen(str)+1);

		read(cli_fd, str, sizeof(str));
		printf("server: %s\n", str);
	}while(strcmp(str, "bye")!=0);

	close(cli_fd);
	return 0;
}

------------------------------------------

// server

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
// server
#define SERV_PORT	2809
#define SERV_IP		"127.0.0.1"
int main() {
	int serv_fd, cli_fd, ret;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t socklen;
	char str[64];
	serv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(serv_fd < 0) {
		perror("socket() failed");
		_exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	inet_aton(SERV_IP, &serv_addr.sin_addr);
	ret = bind(serv_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(ret < 0) {
		perror("bind() failed");
		_exit(2);
	}

	listen(serv_fd, 5);

	socklen = sizeof(cli_addr);
	cli_fd = accept(serv_fd, (struct sockaddr*)&cli_addr, &socklen);
	if(cli_fd < 0) {
		perror("accept() failed");
		_exit(3);
	}

	do {
		read(cli_fd, str, sizeof(str));
		printf("client: %s\n", str);

		printf("server: ");
		gets(str);
		write(cli_fd, str, strlen(str)+1);
	}while(strcmp(str, "bye")!=0);
	
	close(cli_fd);

	shutdown(serv_fd, SHUT_RDWR);
	return 0;
}


------------------------------------------------------------------------------------------------

// SHARED MEMEROY

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define SHM_KEY	0x4231

typedef struct shm {
	int count;
} shm_t;

int main() {
	int i, ret, s, shmid;
	shm_t *ptr;

	shmid = shmget(SHM_KEY, sizeof(shm_t), IPC_CREAT|0600);
	if(shmid < 0) {
		perror("shmget() failed");
		_exit(1);
	}

	ptr = (shm_t*)shmat(shmid, NULL, 0);
	if(ptr == (void*)-1) {
		perror("shmat() failed");
		_exit(2);
	}

	// mark shm for destruction -- it will destroyed when attach count become 0.
	shmctl(shmid, IPC_RMID, NULL);
	
	ptr->count = 0;
	ret = fork();
	if(ret == 0) {
		// child
		for(i=1; i<=30; i++) {
			ptr->count++;
			printf("incr: %d\n", ptr->count);
			sleep(1);
		}
		shmdt(ptr);
	}
	else {
		// parent
		for(i=1; i<=30; i++) {
			ptr->count--;
			printf("decr: %d\n", ptr->count);
			sleep(1);
		}
		waitpid(ret, &s, 0);
		printf("final count: %d\n", ptr->count);
		shmdt(ptr);
	}
	return 0;
}



===================================================================================================

// SHARED MEMEROY EMAPHORE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHM_KEY	0x4231
#define SEM_KEY	0x1432

typedef struct shm {
	int count;
} shm_t;

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};

int main() {
	int i, ret, s, shmid, semid;
	shm_t *ptr;
	union semun su;
	struct sembuf ops[1];

	shmid = shmget(SHM_KEY, sizeof(shm_t), IPC_CREAT|0600);
	if(shmid < 0) {
		perror("shmget() failed");
		_exit(1);
	}

	ptr = (shm_t*)shmat(shmid, NULL, 0);
	if(ptr == (void*)-1) {
		perror("shmat() failed");
		_exit(2);
	}
	printf("shared memory attached at address: %p\n", ptr);

	// mark shm for destruction -- it will destroyed when attach count become 0.
	shmctl(shmid, IPC_RMID, NULL);

	// create semaphore with 1 counter
	semid = semget(SEM_KEY, 1, IPC_CREAT|0600);
	if(semid < 0) {
		perror("semget() failed");
		_exit(3);
	}

	// initialize semaphore counter to 1
	su.val = 1;
	ret = semctl(semid, 0, SETVAL, su);
	if(ret < 0) {
		perror("semctl() failed");
		_exit(3);
	}

	ptr->count = 0;
	ret = fork();
	if(ret == 0) {
		// child
		for(i=1; i<=300; i++) {
			// P(sem);
			ops[0].sem_num = 0;
			ops[0].sem_op = -1;
			ops[0].sem_flg = 0;
			ret = semop(semid, ops, 1);

			ptr->count++;
			printf("incr: %d\n", ptr->count);
			// V(sem);
			ops[0].sem_num = 0;
			ops[0].sem_op = +1;
			ops[0].sem_flg = 0;
			ret = semop(semid, ops, 1);
			
		//	sleep(1);
		}
		shmdt(ptr);
	}
	else {
		// parent
		for(i=1; i<=300; i++) {
			// P(sem);
			ops[0].sem_num = 0;
			ops[0].sem_op = -1;
			ops[0].sem_flg = 0;
			ret = semop(semid, ops, 1);
			
			ptr->count--;
			printf("decr: %d\n", ptr->count);
			
			// V(sem);
			ops[0].sem_num = 0;
			ops[0].sem_op = +1;
			ops[0].sem_flg = 0;
			ret = semop(semid, ops, 1);
			
		//	sleep(1);
		}
		waitpid(ret, &s, 0);
		printf("final count: %d\n", ptr->count);
		shmdt(ptr);

		// destroy the semaphore
		semctl(semid, 0, IPC_RMID);
	}
	return 0;
}
