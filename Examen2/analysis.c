/*
 * Programación Avanzada 
 * Examen 2
 * 
 * 21 de Abril de 2020
 *
 * Diana Estefanía Ortiz Ledesma
 * A01209403
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <math.h>

const char *prefix_input = NULL;
const char *prefix_output = NULL;

int iteracion = 1;
int time = 2;
int pids[2];

/*
 * El proceso ONE crea el archivo con los datos
 * Usa el nombre definido en el problema  
 * Despliega la información en pantalla 
 */
void ONE(){
  printf("ONE: I received SIGUSR1 from ANALYSIS, I will start to work\n\n");

  /* 
   * Strcpy : Asignar un valor a una cadena de texto
   * Copia prefix_input a file
   */
  char file[100];
  strcpy(file, prefix_input);

  /* 
   * Strcat : Concatenar dor cadenas
   * Añade itr y la extensión .dat a file
   */
  char itr[2];
  sprintf(itr, "%i", iteracion);
  strcat(file, itr);
  strcat(file, ".dat");

  /* 
   * Abrir archivo, modo escritura 
   */
  int fd;
  if ((fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0){
    perror("open");
    exit(-1);
  }

  /*
   * El programa genera los números usando la semilla 12345
   */
  srand(12345);
  int numero;
  printf("file: %s -> ", file);

  for (int i = 0; i < 100; i++){
    numero = rand() % 100 + 1;
    write(fd, &numero, sizeof(int));
    printf("%i ", numero);
  }

  close(fd);
  printf("\n\nONE: I send SIGUSR1 to ANALYSIS\n");
  iteracion++;
  kill(getppid(), SIGUSR1);
}

/*
 * El proceso TWO crea el archivo con los datos
 * Usa el nombre definido en el problema 
 * Despliega la información en pantalla 
 */
void TWO(){
  printf("TWO: I received SIGUSR2 from ANALYSIS, I will start to work\n\n");

  /*
   * Variables para asignar valores a las cadenas de texto 
   * Copiar prefix_input a rf
   * Copiar prefix_output a wf 
   */
  char rf[100], wf[100];
  strcpy(rf, prefix_input);
  strcpy(wf, prefix_output);

  /*
   * Variables para concatenar cadenas     
   */
  int fd;
  char itr[5];

  /*
   * Strcat : Concatenar dor cadenas
   * Añade itr y la extensión .dat a rf    
   */
  sprintf(itr, "%i", iteracion);
  strcat(rf, itr);
  strcat(rf, ".dat");
  
  /*
   * Strcat : Concatenar dor cadenas
   * Añade itr y la extensión .dat a wf    
   */
  strcat(wf, itr);
  strcat(wf, ".dat");


  printf("reading file: %s\n", rf);
  if ((fd = open(rf, O_RDONLY)) < 0){
    exit(-1);
  }

  printf("calculating avg and stddev\n");

  /* 
   * Calcular media
   */
  int numero, re;
  int n[100];
  int posicion = 0;
  float media = 0;
  while ((re = read(fd, &numero, sizeof(int))) != 0){
    media += (numero / 100.0);
    n[posicion] = numero;
    posicion++;
  }

  /* 
   * Calcular varianza
   */
  float varianza = 0;
  for (int i = 0; i < 100; i++){
    varianza += (n[i] - media) * (n[i] - media);
  }
  varianza /= 100;

  /*
   * Mostrar resultados del calculo media y varianza
   */
  printf("results... avg = %f stddev = %f\n", media, varianza);
  printf("writing to file %s\n\n", wf);

  /* 
   * Abrir archivo, modo escritura
   */
  int fd1;
  if ((fd1 = open(wf, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0){
    exit(-1);
  }

  /*
   * Escribir valores de tipo float
   * en un archivo binario
   */
  write(fd1, &media, sizeof(float));
  write(fd1, &varianza, sizeof(float));

  /* 
   * Cerrar archivos
   */
  close(fd);
  close(fd1);

  iteracion++;
  printf("TWO: I send SIGUSR2 to ANALYSIS.\n");
  kill(getppid(), SIGUSR2);
}

/* 
 * Correr proceso ONE
 * Desde de su señal correspondiente SIGUSR1
 */
void PROCESS_ONE(){
  signal(SIGUSR1, ONE);
  while (1){
    pause();
  }
}

/* 
 * Correr proceso TWO
 * Desde de su señal correspondiente SIGUSR1
 */
void PROCESS_TWO(){
  signal(SIGUSR2, TWO);
  while (1){
    pause();
  }
}

/*
 * El programa crea dos hijos 
 */
void PROCESS_CHILD(int i){
  switch (i){
  case 0:
    printf("I am ANALYSIS and I create ONE\n");
    PROCESS_ONE();
    break;
  case 1:
    printf("I am ANALYSIS and I create TWO\n");
    PROCESS_TWO();
    break;
  }
}

/*
 * El proceso completo se realiza sólo tres veces 
 */
void HANDLER(int signal){
  switch (signal){
  /*
   * Señal SIGUSR1 enviada al proceso ONE
   */
  case SIGUSR1:
    printf("ANALYSIS: I received SIGUSR1 from ONE, sending SIGUSR2 to TWO\n");
    kill(pids[1], SIGUSR2);
    break;
  /*
   * Señal SIGUSR2 enviada al proceso TWO
   */
  case SIGUSR2:
    printf("ANALYSIS: I received SIGUSR2 from TWO\n");  
    if (iteracion < 3){
      iteracion++;
      kill(pids[0], SIGUSR1);
    }
    else{
      kill(pids[0], SIGINT);
      kill(pids[1], SIGINT);
      exit(0);
    }
    break;
  }
}

/*
 * Correr programa 3 veces 
 */
void PROGRAM(){
  int pid;
  for (int i = 0; i < 2; i++){
    /* 
     * Fork: si se ejecuta con éxito devuelve al padre
     * El PID del proceso hijo creado
     */
    if ((pid = fork()) < 0){
      perror("Fork");
      exit(-1);
    }
    else if (pid == 0){
      PROCESS_CHILD(i);
    }
    else{
      pids[i] = pid;
    }
  }
  signal(SIGUSR1, HANDLER);
  signal(SIGUSR2, HANDLER);
  sleep(time);
  printf("\nANALYSIS: I send SIGUSR1 to ONE\n");
  kill(pids[0], SIGUSR1);
  wait(NULL);
}

/*
 * El programa verifica que la cantidad    
 * y tipo de parámetros sea correcto
 */
int main(int argc, char const *argv[]){
  /*
   * En caso de no recibir la cantidad de parámetros correcta
   * despliega el siguiente mensaje      
   */
  if (argc != 4){
    fprintf(stderr, "usage: %s timeout prefix_input prefix_output\n", argv[0]);
    return -2;
  }

  char *pos;
  long check;
  check = strtol(argv[1], &pos, 10);
  /*
     * En caso de no recibir el tipo de parámetro correcto 
     * despliega el siguiente mensaje 
     */
  if ((pos == argv[1]) || (*pos != '\0') || 0 > check){
    fprintf(stderr, "the timeout must be an interger number greater than that\n");
    return -3;
  }
    
  prefix_input = argv[2];
  prefix_output = argv[3];
  PROGRAM();
  return 0;
}
