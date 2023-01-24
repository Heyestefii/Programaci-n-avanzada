/*
 * Programación avanzada
 *
 * 26 de mayo de 2020
 *
 * Tercer examen 
 *
 * Diana Estefanía Ortiz Ledesma
 * A01209403
 */

#ifndef SEMAFOROS_H
#define SEMAFOROS_H
#endif

/* Librerias */
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define COMPARTIDO 1

/* Threads */ 
void *oso(void *);  
void *abejas(void *);

/* Semáforos globales */
sem_t vacio;
sem_t lleno;
sem_t olla;

/* Número de porciones necesarias para llenar la olla */
int porciones;

/* Buffer */
int miel;  

/* Número de abejas que estarán llenando la olla */
int numabejas;
long i;

int main(int argc, char *argv[]) {
  /* Threads y atributos 
     Apuntador (identificador) del oso */
  pthread_t pid;  
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  srand (time(NULL));

  /* Guardar valores ingresados por el usuario en variables correspondientes */
  numabejas = atoi(argv[1]);
  porciones = atoi(argv[2]);

  /* Inicializar buffer */
  miel = 0;

  /* Apuntador (identificador) de abejas */
  pthread_t abejaId[numabejas];

  /* Inicializar semáforos en 0 para que sólo puedan ser utilizados por el thread del mismo proceso */
  sem_init(&vacio, COMPARTIDO, 0);  
  sem_init(&lleno, COMPARTIDO, 0);   

  /* Recurso compartido utilizado con exclusión mutua, se inicializa en 1 */
  sem_init(&olla, COMPARTIDO, 1);  

  /* Crear hilo correpondiente al oso */
  pthread_create(&pid, &attr, oso, NULL);

  /* Crear el número de abejas ingresadas por el usuario*/
  for (i = 0; i < numabejas; ++i){
    /* Crear hilo correpondiente a las abejas */
    pthread_create(&abejaId[i], &attr, abejas, (void * ) i);
  }
  for (int i = 0; i < numabejas; i++) {
    /* Mecanismo de sincronización para esperar la terminación del hilo de las abejas */
    pthread_join(abejaId[i], NULL);  
  }
}

/* El oso debera esperar a que una abeja lo despierte para poder consumir la olla de miel completa */
void *oso(void *arg) {
  /* Imprimir en caso que la olla no este completa */
  printf("Bear sleeping...\n");
  sleep(1);
  while(1){
    /* Uso del recurso compartido */ 
    sem_wait(&lleno);
    /* Inicilizar buffer */
    miel = 0;
    /* Imprimir en caso que la olla este completa */
    printf("Bear : What beautiful bees! The jar is full! Now, I'm going to eat! \n");
    sem_post(&vacio);
  }
}

void *abejas(void *arg) {
  long Id = (long) arg;
  printf("Bee ready to work.\n");
  while(1){
    /* Cada abeja debera esperar para dejar su porción en el recurso compartido */
    sem_wait(&olla);

    if (miel == porciones){
      printf("Bee %ld : Oops! It is full, I will notify the bear. \n", Id);
      /* Avisarle al oso que la olla esta llena de miel */
      sem_post(&lleno);  
      /* Abejas esperan a que la olla este vacía de nuevo */
      sem_wait(&vacio); 
      printf("Bear : The jar ir empty. I will wait for honey ... while I sleep again. \n");
      /* Abejas continuan llenando la olla con porciones */
      sem_post(&olla); 
    }

    else{
      /* Contador de la miel */
      miel++; 
      int tiempo = rand()%3;
      printf("Bee %ld : I'm going to put one portion.\n", Id);
      sem_post(&olla);
      sleep(tiempo);
    }
  }
}
