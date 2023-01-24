/* 
 * 
 * Programacion avanzada 
 * 
 * Actividad colaborativa 4 
 * Señales 
 *
 * Diana Ortiz | Pedro Gonzalez
 * A01209403   | A01651517
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h> 
#include <sys/wait.h>

unsigned char *bf1;
unsigned char *bf2;
unsigned char *bf3;

// Recibe SISGUR1 y ejecuta la instruccion 1, pero el programa principal no termina 
void inst1(){
  int res;
  // Crea un proceso hijo exactamente igual al proceso padre
  res = fork();
  if(res == 0){
    char *token;
    char *argvs[10];
    char s[2] = ",";
    // Strtok permite partir una cadena en subcadenas usando en este caso como separador " , "
    token = strtok(bf1, s);
    argvs[0] = token;
    // La función recuerda la cadena y la última palabra que devolvió y continua con la siguiente
    token =  strtok(NULL, s);
    int i = 1;
    while (token != NULL){
       token =  strtok(NULL, " ");
       argvs[i] = token;
       i++;
     }
     // Proporcionan un vector de  punteros a la cadena de caracteres terminada en cero
     execvp ("/bin/ls", argvs);  
  }
}

// Recibe SIGUSR2 y ejecuta la intruccion 2, pero el programa principal no termina
void inst2(){
  int res;
  // Crea un proceso hijo exactamente igual al proceso padre
  res = fork();
  if(res == 0){
    char *token;
    char *argvs[10];
    char s[2] = ",";
    // Strtok permite partir una cadena en subcadenas usando en este caso como separador " , "
    token = strtok(bf2, s);
    argvs[0] = token;
    // La función recuerda la cadena y la última palabra que devolvió y continua con la siguiente
    token =  strtok(NULL, s);
    int i = 1;
    while (token != NULL){
     token =  strtok(NULL, " ");
     argvs[i] = token;
     i++;
    }
    argvs[i]  = 0;
    // Proporcionan un vector de  punteros a la cadena de caracteres terminada en cero
    execvp ("/bin/ps", argvs);  
  }
}

// Recibe SIGPWR y ejecuta la intruccion 3, pero el programa principal no termina
void inst3(){
  // Crea un proceso hijo exactamente igual al proceso padre
  int res = fork();
  if(res == 0){
    char *token;
    char *argvs[10];
    char s[2] = ",";
    // Strtok permite partir una cadena en subcadenas usando en este caso como separador " , "
    token = strtok(bf3, s);
    argvs[0] = token;
    // La función recuerda la cadena y la última palabra que devolvió y continua con la siguiente
    token =  strtok(NULL, s);
    int i = 1;
    while (token != NULL){
     token =  strtok(NULL, " ");
     argvs[i] = token;
     i++;
    }
    argvs[i]  = 0;
    // Proporcionan un vector de  punteros a la cadena de caracteres terminada en cero
    execvp ("/bin/uname", argvs);
  }
}

// Termina hasta recibir la señal SIGINT, elimina correctamente la memoria dinámica asignada
void inst4(){
  printf("Ending...\n");
  free(bf1);
  free(bf2);
  free(bf2);
  exit(0);
}

int main(int argc, char * argv[]){
  int file1, file2, file3;
  
  // Verifica que se reciba la cantidad correcta de parámetros 
  if(argc !=4){
    printf("usage: %s file1 file2 file3\n", argv[0]);
    return -2;
  }

  // Verifica que los archivos de entrada existan
	if ((file1=open(argv[1],O_RDONLY))<0){
    // En caso de no ser así, el programa despliega el siguiente mensaje y termina
		fprintf(stderr, "%s: the file %s does not exist\n", argv[0], argv[1]);
		return -3;
	}

  // Verifica que los archivos de entrada existan
  if ((file2=open(argv[2],O_RDONLY))<0){
    // En caso de no ser así, el programa despliega el siguiente mensaje y termina
		fprintf(stderr, "%s: the file %s does not exist\n", argv[0], argv[2]);
		close(file1);
		return -4;
	}

  // Verifica que los archivos de entrada existan
  if ((file3=open(argv[3],O_RDONLY))<0){
    // En caso de no ser así, el programa despliega el siguiente mensaje y termina
		fprintf(stderr, "%s: the file %s does not exist\n", argv[0], argv[3]);
		close(file1);
		close(file2);
		return -5;
	}
 
  // El acceso directo a cualquier posición dentro de un fichero puede lograrse con la función lseek
  unsigned long f1size = lseek(file1, 0, SEEK_END); 
  // Asignar memoria dinamica
  bf1 = (unsigned char*) malloc(sizeof(unsigned char)*f1size);
  lseek(file1,0,SEEK_SET);
  read(file1,bf1,f1size);
  
  // El acceso directo a cualquier posición dentro de un fichero puede lograrse con la función lseek
  unsigned long f2size = lseek(file2, 0, SEEK_END); 
  // Asignar memoria dinamica
  bf2 = (unsigned char*) malloc(sizeof(unsigned char)*f2size);
  lseek(file2,0,SEEK_SET);
  read(file2,bf2,f2size);

  // El acceso directo a cualquier posición dentro de un fichero puede lograrse con la función lseek
  unsigned long f3size = lseek(file3, 0, SEEK_END); 
  // Asignar memoria dinamica
  bf3 = (unsigned char*) malloc(sizeof(unsigned char)*f3size);
  lseek(file3,0,SEEK_SET);
  read(file3,bf3,f3size);

  f1size = lseek(file1, 0, SEEK_END);
  f2size = lseek(file2, 0, SEEK_END); 
  f3size = lseek(file3, 0, SEEK_END);
  
  // Despliega mensaje adecuado si los archivos no contienen informacion
  if(f1size == 0){
    printf("the file %s is empty\n",argv[1]);
    return -6;
  }
  if(f2size == 0 ){
    printf("the file %s is empty\n",argv[2]);
    return -7;
  }
  if(f3size == 0){
    printf("the file %s is empty\n",argv[3]);
    return -8;
  }

  // Ejecutar señales 
  printf("Waiting for a signal...\n");
  signal(SIGUSR1, inst1);
  signal(SIGUSR2, inst2);
  signal(SIGPWR, inst3);
  signal(SIGINT, inst4);

  while(1){
    sleep(1);
  }
  
  close(file1);
  close(file2);
  close(file3);

  return 0;
}
