/*
 *
 * Programación avanzada 
 *
 * Proyecto final 
 *
 * Diana Estefanía Ortiz Ledesma  |  A01209403
 */

#define  _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 4096

typedef unsigned char uchar;

void shell(char *linea){
 /*
  * Verificar que se ingrese un comando 
  */   
  if (strcmp(linea, " ") == 0){
    puts("No se ingreso ningun comando");
    exit(0);
  }

  char *args[100];
  int i = 0;
  char *token = strtok(linea, " ");

  while(token != NULL){
    args[i] = token;
    i++;
    token = strtok(NULL, " ");
  }

  args[i] = NULL;
  execvp(args[0], args);
  char *echo[2] = {"echo", "$?"};
  if (execvp(echo[1], echo) != 0){
   /*
    * Verificar que sea un comando valido 
    */ 
    printf("Introducir comando valido\n");
  }

  exit(0);
}

void proceso(char *linea){
  pid_t wpid;
  int pid;
  int status = 0;
  char *comando = strtok(linea, ";");

 /*
  * Verificar que se ingrese un comando 
  */  
  if(strcmp(comando, " ") ==  0){
    puts("No se ingreso ningun comando");
  }

 /*
  * Verificar que comando se recibe y ejecutarlo  
  */  
  while(comando != NULL){
    /*
     * Salir del "shell"
     */
    if(strcmp(comando, "quit") == 0){
      exit(0);
    }

    /*
     * Salir del "shell"
     */
    if(strcmp(comando, "CTRL-D") == 0){
      exit(0);
    }

    if((pid = fork()) < 0){
      perror("fork");
      exit(-1);
    }
    else if (pid == 0){
      shell(comando);
    }
    else{
      comando = strtok(NULL, ";");
    }
  }

  while ((wpid = wait(&status)) > 0){
  ;
  }
}

/*
 * Se mostrará un “prompt” 
 * El usuario del “shell” escribirá un comando en la solicitud
 */
void interactiva(){
  char linea[512];

  while(1){
    /*
     * Imprimir “prompt"    
     */
    printf("mini-shell> ");
    fgets(linea, sizeof(linea), stdin);
    linea[strcspn(linea, "\n")] = '\0';
    proceso(linea);
  }
}

/*
 * El “shell” recibe como parámetro de línea de comando un archivo 
 * Ejecuta todos los comandos dentro del archivo
 */
void lotes(const char *file){
  char linea[512];
  FILE *fp;
  char *renglon = NULL;
  size_t longitud = 0;
  ssize_t leer;
  fp = fopen(file, "r");

  if(fp == NULL){
    printf("El archivo no existe o no se puede leer\n");
    exit(EXIT_FAILURE);
  }

  while((leer = getline(&renglon, &longitud, fp)) != -1){
    renglon[strcspn(renglon, "\n")] = '\0'; 
    printf("\n");
    puts(renglon);
    printf("\n");  
    proceso(renglon);
  }
  fclose(fp);
  
  if (renglon){
  free(renglon);
  }
  exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[]){
  /* 
   * Verificar que reciba la cantidad correcta de parámetros 
   * Realizar la acción requerida 
   */
  if (argc == 1){
    interactiva();  
  }
  else if (argc == 2){     
    lotes(argv[1]);  
  }
  else {
    fprintf(stderr, "Invocar programa interactivo : %s \n", argv[0]);
    fprintf(stderr, "Invocar programa por lotes : %s [BatchFile] \n", argv[0]); 
  } 
  return 0;
}
