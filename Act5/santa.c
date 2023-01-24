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

  if(argc != 1){
    printf("usage: %s\n",argv[0]);
    return -1;
  }

  int semid;
  int i;
  key_t key;

  if((key = ftok("/dev/null",65)) == (key_t) -1){
    perror(argv[0]);
    return -1;
  }

  if((semid = semget(key, 6, 0666)) < 0){
    perror(argv[0]);
    return -1;
  }

  while(1){
    wait(semid,santa);
    wait(semid,mt);

    int numRen = semctl(semid, renos, GETVAL, 0);
    int numElf = semctl(semid, elfos, GETVAL, 0);

    printf("Cuantos renos hay? %d\n", numRen);
    printf("Cuantos elfos hay? %d\n", numElf);

    if(numRen >= 9){
      ssignal(semid, reno, 9);
      swait(semid, reno, 9);
    }else if(numElf == 3){
      printf("Bueno... hay que ayudar a estos elfos\n");
    }
    signal(semid, mt);
    sleep(1);
  }
  return 0;
}
