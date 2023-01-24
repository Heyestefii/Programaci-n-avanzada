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

void crear_renos(char * program){
  int semid;
  int i;
  key_t key;

  if((key = ftok("/dev/null",65)) == (key_t) -1){
    perror(program);
    exit(-1);
  }

  if((semid = semget(key,6,0666)) < 0){
    perror(program);
    exit(-1);
  }

  while(1){
    sleep(1);
    wait(semid,mt);
    ssignal(semid,renos,1);
    printf("reno %d ha llegado\n", getpid());
    int numRen = semctl(semid, renos, GETVAL, 0);

    if(numRen == 9){
      printf("reno %d ya esta listo para salir\n", getpid());
      signal(semid,santa);
    }
    signal(semid, mt);
    wait(semid,reno);
  }
  exit(0);
}

int main(int argc, char *argv[]) {
  int i;
  int pid;

  for(i = 0; i < 9; i++){
    if((pid = fork()) < 0){
      perror("fork");
      return -1;
    }else if(pid == 0){
      crear_renos(argv[0]);
    }else{
    }
  }
  return 0;
}
