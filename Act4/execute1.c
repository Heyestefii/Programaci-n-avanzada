/*
 * Programacion avanzada 
 * 
 * Actividad 4
 * Tema : Señales
 * 
 * Diana Estefanía Ortiz Ledesma
 * A01209403
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/utsname.h>

typedef unsigned long long ull;
typedef unsigned char uchar;
typedef unsigned long ulong;

// El programa solo termina hasta recibir la señal SIGINT, elimina correctamente la memoria dinámica asignada
void end(int s) {
    printf("Ending...\n");
    exit(0);
}

// El programa recibe SISGUR1 y ejecuta la instruccion 1, pero el programa principal no termina 
void inst1(int sig) {

  int res;
  // Crea un proceso hijo exactamente igual al proceso padre
  res = fork();

  if(res == 0){
    int route;
    char *token;
    char *argvs[100];
    char sig1[2] = ",";

    if ( (route = open("file1.txt", O_RDONLY)) < 0 ) {
  		perror("./execute");
  		exit(-1);
  	}

    // Indicara número de bytes a transferir
    int bytes;
    // Apuntador al área de memoria donde se va a efectuar la transferencia
	  uchar *buffer;
    // El acceso directo a cualquier posición dentro de un fichero puede lograrse con la función lseek, en este caso es relativo al final del fichero
	  ulong size = lseek(route, 0, SEEK_END);
    // Asignación de memoria dinámica para la variable buffer
	  buffer = (uchar*) malloc(sizeof(uchar) * size);

    // El acceso directo a cualquier posición dentro de un fichero puede lograrse con la función lseek, en este caso al inicio del fichero
	  lseek(route, 0, SEEK_SET);
    // Leer información en fichero
	  read(route, buffer, size);
    // Strtok permite partir una cadena en subcadenas usando en este caso como separador " , "  
    token = strtok(buffer, sig1);
    argvs[0] = token;
    // La función recuerda la cadena y la última palabra que devolvió y continua con la siguiente
    token =  strtok(NULL, sig1);
    int i = 1;
    while (token != NULL){
      token =  strtok(NULL, " ");
      argvs[i] = token;
      i++;
    }
    argvs[i]  = 0;
    // Proporcionan un vector de  punteros a la cadena de caracteres terminada en cero
    execvp ("/bin/ls", argvs);
    free(buffer);
    close(route);
    exit(0);
  }

}

// El programa recibe SIGUSR2 y ejecuta la intruccion 2, pero el programa principal no termina
void inst2(int sig) {

  int res;
  // Crea un proceso hijo exactamente igual al proceso padre
  res = fork();
  if(res == 0){

    int route;
    char *token;
    char *argvs[100];
    char sig2[2] = ",";

    if ( (route = open("file2.txt", O_RDONLY)) < 0 ) {
  		perror("./execute");
  		exit(-1);
  	}

    // Indicara número de bytes a transferir
    int bytes;
    // Apuntador al área de memoria donde se va a efectuar la transferencia
	  uchar *buffer;
    // El acceso directo a cualquier posición dentro de un fichero puede lograrse con la función lseek, en este caso es relativo al final del fichero
	  ulong size = lseek(route, 0, SEEK_END);
    // Asignación de memoria dinámica para la variable buffer
	  buffer = (uchar*) malloc(sizeof(uchar) * size);

    // El acceso directo a cualquier posición dentro de un fichero puede lograrse con la función lseek, en este caso al inicio del fichero
	  lseek(route, 0, SEEK_SET);
    // Leer información en fichero
	  read(route, buffer, size);
    // Strtok permite partir una cadena en subcadenas usando en este caso como separador " , " 
    token = strtok(buffer, sig2);
    argvs[0] = token;
    // La función recuerda la cadena y la última palabra que devolvió y continua con la siguiente
    token =  strtok(NULL, sig2);
    int i = 1;
    while (token != NULL){
      token =  strtok(NULL, " ");
      argvs[i] = token;
      i++;
    }
    argvs[i]  = 0;
    // Proporcionan un vector de  punteros a la cadena de caracteres terminada en cero
    execvp ("/bin/ps", argvs);
    free(buffer);
    close(route);
    exit(0);
    }
}

// El programa recibe SIGPWR y ejecuta la intruccion 3, pero el programa principal no termina
void inst3(int sig) {

  int res;
  // Crea un proceso hijo exactamente igual al proceso padre
  res = fork();
  if(res == 0){

    int route;
    char *token;
    char *argvs[100];
    char sig3[2] = ",";

    if ( (route = open("file3.txt", O_RDONLY)) < 0 ) {
      perror("./execute");
      exit(-1);
    }

    int bytes;
    // Apuntador al área de memoria donde se va a efectuar la transferencia
    uchar *buffer;
    // El acceso directo a cualquier posición dentro de un fichero puede lograrse con la función lseek, en este caso es relativo al final del fichero
    ulong size = lseek(route, 0, SEEK_END);
    buffer = (uchar*) malloc(sizeof(uchar) * size);

    // El acceso directo a cualquier posición dentro de un fichero puede lograrse con la función lseek, en este caso al inicio del fichero
    lseek(route, 0, SEEK_SET);
    // Leer información en fichero
    read(route, buffer, size);

    uchar *bufferaux  = buffer;

    // Strtok permite partir una cadena en subcadenas usando en este caso como separador " , " 
    token = strtok(buffer, sig3);
    argvs[0] = token;
    // La función recuerda la cadena y la última palabra que devolvió y continua con la siguiente
    token = strtok(NULL, "\n");
    token = strtok(token, " ");
    int i = 1;
    while (token != NULL){
      token =  strtok(NULL, " ");
      argvs[i] = token;
      i++;
    }
    argvs[i] = NULL;
    // Proporcionan un vector de  punteros a la cadena de caracteres terminada en cero
    execvp(argvs[0], argvs);

    free(buffer);
    close(route);
    exit(0);
  }
}


int main(int argc, char *argv[]){
  int file1;
  int file2;
  int file3;
  ull size;

  // El programa verifica que se reciba la cantidad correcta de parámetros
	if (argc!=4){
    // En caso de no ser así, el programa despliega el siguiente mensaje y termina
		fprintf(stderr, "usage: %s file1 file2 file3\n", argv[0]);
    // Regresa -2 como resultado de su ejecución
		return -2;
	}

  // El programa verifica que los archivos de entrada existan 
	if ((file1=open(argv[1],O_RDONLY))<0){
    // En caso de que no sea así, el programa despliega el siguiente mensaje
		fprintf(stderr, "%s: the file %s does not exist\n", argv[0], argv[1]);
    // Regresa -3 para el file1
		return -3;
	}

  // El programa verifica que los archivos de entrada existan
	if ((file2=open(argv[2],O_RDONLY))<0){
    // En caso de que no sea así, el programa despliega el siguiente mensaje
		fprintf(stderr, "%s: the file %s does not exist\n", argv[0], argv[2]);
		close(file1);
    // Regresa -4 para el file2
		return -4;
	}

  // El programa verifica que los archivos de entrada existan
	if ((file3=open(argv[3],O_RDONLY))<0){
    // En caso de que no sea así, el programa despliega el siguiente mensaje
		fprintf(stderr, "%s: the file %s does not exist\n", argv[0], argv[3]);
		close(file1);
		close(file2);
    // Regresa -5 para el file3
		return -5;
	}

  // El programa verifica que los archivos de entrada contengan información (size)
	size=lseek(file1,0,SEEK_END);
	if(size==0){
    // En caso de que no sea así, el programa despliega el siguiente mensaje
		fprintf(stderr, "%s: the file %s is empty\n", argv[0], argv[1]);
		close(file1);
		close(file2);
		close(file3);
    // Regresa -6 para el file1
		return -6;
	}

  // El programa verifica que los archivos de entrada contengan información (size)
	size=lseek(file2,0, SEEK_END);
	if (size==0){
    // En caso de que no sea así, el programa despliega el siguiente mensaje
		fprintf(stderr, "%s: the file %s is empty\n", argv[0], argv[2]);
		close(file1);
		close(file2);
		close(file3);
    // Regresa -7 para el file2
		return -7;
	}

  // El programa verifica que los archivos de entrada contengan información (size)
	size=lseek(file3,0, SEEK_END);
	if (size==0){
    // En caso de que no sea así, el programa despliega el siguiente mensaje
		fprintf(stderr, "%s: the file %s is empty\n", argv[0], argv[3]);
		close(file1);
		close(file2);
		close(file3);
    // Regresa -8 para el file3
		return -8;
	}

    // Ejecutar señales 
    signal(SIGINT, end);
    signal(SIGUSR1, inst1);
    signal(SIGUSR2, inst2);
    signal(SIGPWR, inst3);

    printf("Waiting for a signal...\n");

    while(1) {
        pause();
    }

	close(file1);
	close(file2);
	close(file3);
	return 0;
}