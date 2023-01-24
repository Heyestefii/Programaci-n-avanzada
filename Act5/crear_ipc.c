/*
 * Programación Avanzada 
 *
 * Actividad : 
 * Manejo de semaforos
 * 
 * 12 de Mayo de 2020
 *
 * Diana Estefanía Ortiz Ledesma
 * A01209403
 *
 */

#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>

#define mt 0
#define elfos 1
#define renos 2
#define santa 3
#define reno 4
#define elfo 5

int wait(int semid, int sem_num) {
	struct sembuf op;
	op.sem_num = sem_num;
	op.sem_op = -1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int signal(int semid, int sem_num) {
	struct sembuf op;
	op.sem_num = sem_num;
	op.sem_op = 1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int swait(int semid, int sem_num, int val) {
	struct sembuf op;
	op.sem_num = sem_num;
	op.sem_op = -val;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int ssignal(int semid, int sem_num, int val) {
	struct sembuf op;
	op.sem_num = sem_num;
	op.sem_op = val;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int main(int argc, char *argv[]) {
  int semid;
  int i;
  unsigned short values[6];
  key_t key;

  if ((key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}

  if ((semid = semget(key,6,0666 | IPC_CREAT))  < 0 ) {
		perror(argv[0]);
		return -1;
	}

  semctl(semid,elfos,SETVAL,0);
  semctl(semid,mt,SETVAL,1);
  semctl(semid,renos,SETVAL,0);
  semctl(semid,santa,SETVAL,0);
  semctl(semid,reno,SETVAL,0);
  semctl(semid,elfo,SETVAL,1);
  semctl(semid,0,GETALL,values);

  return 0;
}
