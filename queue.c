#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Burst{
	int length;
	struct Burst *next;
};

struct BurstQ{
	int n;
	struct Burst *head, *tail;
};

struct PCB {
	int pid, at, numBursts, awakeTime;
	struct BurstQ *cpuBurst, *waitBurst;
	struct PCB *next;
};

struct Q {
	int n;
	struct PCB *head, *tail;
} *jobQ, *waitQ, *arrivalQ, *doneQ;

int quantum = 0;

void create(){
	jobQ = (struct Q *)malloc(sizeof(struct Q));
	waitQ = (struct Q *)malloc(sizeof(struct Q));
	arrivalQ = (struct Q *)malloc(sizeof(struct Q));
	doneQ = (struct Q *)malloc(sizeof(struct Q));
	jobQ->n = 0;
	waitQ->n = 0;
	arrivalQ->n = 0;
	doneQ->n = 0;

}

void pushPCB(struct PCB *process, struct Q *q){
	if (q->tail == NULL){
		q->tail = process;
		q->head = q->tail;
	} else {
		q->tail->next = process;
		q->tail = process;
	}
	q->n++;
}

void pushBurst(int length, struct BurstQ *q){
	struct Burst *temp = (struct Burst *)malloc(sizeof(struct Burst));
	temp->length = length;
	temp->next = NULL;
	if (q->tail == NULL){
		q->tail = temp;
		q->head = q->tail;
	} else {
		q->tail->next = temp;
		q->tail = temp;
	}
	q->n++;
}

struct PCB *popQ(struct Q *q){
	q->n--;
	if (q->head == NULL){
		return NULL;
	} else {
		struct PCB *head1 = q->head;
		q->head = q->head->next;
		return head1;
	}
}

int popBurst(struct BurstQ *q){
	q->n--;
	if (q->head == NULL){
		return 0;
	} else {
		int length = q->head->length;
		// printf("length: %d ", length);
		q->head = q->head->next;
		return length;
	}
}

void insertQ(int pid, struct Q *q){
	struct PCB *temp = (struct PCB *)malloc(sizeof(struct PCB));
	temp->pid = pid;
	struct PCB *pointer = (struct PCB *)malloc(sizeof(struct PCB));
	pointer = q->head;
	if (pid < q->head->pid){
		temp->next = q->head;
		q->head = temp;
	} else if (pid > q->tail->pid){
		q->tail->next = temp;
		temp->next = NULL;
		q->tail = temp;
	} else {
		while (pointer->next != NULL){
			if (pid < pointer->next->pid){
				temp->next = pointer->next;
				pointer->next = temp;
				break;
			} else {
				pointer = pointer->next;
			}
		}
	}

}

void getProcesses(){
	FILE *fp;
	fp = fopen("input.txt", "r");
	int pidCounter = 0;
	int at, numBursts;
	char rest[100];
	char *r[100];
	//fscanf(fp, "%d %d %c ", &at, &numBursts, r);
	while(fgets(rest, 50, fp)){
		struct PCB *newPCB = (struct PCB *)malloc(sizeof(struct PCB));
		pushPCB(newPCB, arrivalQ);
		newPCB->cpuBurst = (struct BurstQ *)malloc(sizeof(struct BurstQ));
		newPCB->waitBurst = (struct BurstQ *)malloc(sizeof(struct BurstQ));
		//printf("Read in: %s %s %s", r[0], r[1], r[2]);
		newPCB->pid = pidCounter;
		newPCB->awakeTime = 0;
		pidCounter++;
		char *token = strtok(rest, " ");
		newPCB->at = atoi(token);
		token = strtok(NULL, " ");
		newPCB->numBursts = atoi(token);

		//get first CPU Burst
		token = strtok(NULL, " ");
		pushBurst(atoi(token), newPCB->cpuBurst);
		token = strtok(NULL, " "); //get first wait burst
		while(token != NULL){
			//token = Wait Burst
			printf("Pushing Burst: %d ", atoi(token));
			pushBurst(atoi(token), newPCB->waitBurst);
			token = strtok(NULL, " ");

			//token = CPU Burst
			printf("Pushing Burst: %d ", atoi(token));
			pushBurst(atoi(token), newPCB->cpuBurst);

			token = strtok(NULL, " "); //wait burst or null
		}
	}
}

void printProcesses(){
	int i = arrivalQ->head->next->cpuBurst->n;
	printf("CPU Q: ");
	while (i-- > 0){
		int burst = popBurst(arrivalQ->head->next->cpuBurst);
		printf("%d ", burst);
	}
	i = arrivalQ->head->next->waitBurst->n;
	printf("Wait Q: ");
	while (i-- > 0){
		int burst = popBurst(arrivalQ->head->next->waitBurst);
		printf("%d ", burst);
	}
}

void arrival(int cycle){
	if (arrivalQ->n > 0){
		while(arrivalQ->head->at <= cycle){
			printf("Process %d arrived at cycle %d \n", arrivalQ->head->pid, cycle);
			pushPCB(popQ(arrivalQ), jobQ);
			if(arrivalQ->n <= 0) break;
		}
	}
}

void awake(int cycle){
	if (waitQ->n > 0){
		while(waitQ->head->awakeTime <= cycle){
			printf("Process %d awoke at cycle %d \n", waitQ->head->pid, cycle);
			pushPCB(popQ(waitQ), jobQ);
			if (waitQ->n <= 0) break;
		}
	}
}

void cpu(){
	int cycle = 0;
	struct PCB *onCPU;
	int quantumCounter = quantum;
	int contextSwitch = 0;
	while(cycle < 100){
		arrival(cycle);
		awake(cycle);
		//countdown Context switch, do nothing else
		if (contextSwitch > 0){
			contextSwitch--;
		}
		//if no context switch, handle jobs
		else {
			if(onCPU == NULL && jobQ->n > 0) {
			onCPU = popQ(jobQ);
			printf("Process %d began running at %d", onCPU->pid, cycle);
			} else if (){

			}
		}
		//preemption
		if(quantum > 0 ){
			quantumCounter++;
			if(quantumCounter > quantum){
				quantumCounter = 0;
				//quantum expired, take off CPU

			}
		}


		//if there's a quantum, increment it

		cycle++;
	}
}

int main()
{
	create();
	getProcesses();

	cpu();


	return 0;
}
