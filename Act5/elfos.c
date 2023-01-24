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

void crear_elfos(char * program){
  int semid;
  int i;
  key_t key;

  if((key = ftok("/dev/null",65)) == (key_t) -1){
    perror(program);
    exit(-1);
  }

  if((semid = semget(key, 6, 0666)) < 0){
    perror(program);
    exit(-1);
  }

  while(1){
    sleep(1);
    wait(semid,elfo);
    wait(semid,mt);

    ssignal(semid, elfos, 1);
    printf("el elfo %i pide ayuda...\n", getpid());
    int numElfos = semctl(semid,elfos,GETVAL,0);

    if(numElfos == 3){
      signal(semid,santa);
    }else{
      signal(semid,elfo);
    }
    signal(semid,mt);
    wait(semid,mt);

    swait(semid, elfos, 1);
    numElfos = semctl(semid, elfos, GETVAL, 0);

    if(numElfos == 0){
      signal(semid,elfo);
    }
    signal(semid,mt);
  }

  exit(0);
}

int main(int argc, char *argv[]) {
  int elf; 
  int i;
  int pid;

  elf = atoi(argv[1]);

  for(i = 0; i < elf; i++){
    if((pid = fork()) < 0){
      perror("fork");
      return -1;
    }else if(pid == 0){
      crear_elfos(argv[0]);
    }else{

    }
  }

  return 0;
}
