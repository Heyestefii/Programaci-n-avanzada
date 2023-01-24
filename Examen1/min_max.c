/*-------------------------------------------------------------*

* Programación avanzada: Examen 1

* Fecha: 10 de marzo de 2020

* Autor: 1209403 Diana Estefania Ortiz Ledesma 

*--------------------------------------------------------------*/

/* MIN_MAX
 * Librerias
 * */ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>

/* Escribe un programa llamado min_max que recibe
 * de línea de comando un directorio, directory.
 *
 * Buscar de manera recursiva, el mínimo y máximo,
 * entero almacenado en cada uno de los archivos binarios
 * que se encuentran en el directorio.
 * */

int dir_file(char * name);
void min_max(char *file_name, char *program);
void list(char *nombre_dir, char *program);

int main(int argc, char *argv[]){
  char nombre_dir[PATH_MAX+1];
  char *directory;

  int res[2];
  res[0] = 0;
  res[1] = 0;
  
  /* Verificar que reciba la cantidad correcta de parámetros : 
   * El nombre del programa se guarda en argv[0], por lo tanto la numeración de parametros empieza en argv[1]
   * Argc, es utilizado para conocer el número de parámetros ingresados y realizar la evaluación de estos
   * En caso de que no se cumpla, el programa regresa -1 y termina
   */
  if(argc > 2){
    fprintf(stderr, "usage: %s directory\n", argv[0]);
    return -1;
  }

  /* Verificar que directory sea un directorio existente : 
   * Obtener el directorio actual de trabajo
   * Checar si es un directorio 
   * En caso de que no se cumpla, el programa despliega un mensaje adecuado, regresa -2 y termina
   */
  getcwd(nombre_dir, PATH_MAX);
  directory = nombre_dir;
  if(argc == 2){
    directory = argv[1];
  }

  if(dir_file(directory) == -1){ 
    fprintf(stderr, "%s: No such file or directory\n", argv[0]);
    return -2;
  }

  else if(dir_file(directory) != 0){
    fprintf(stderr, "%s: Not a directory\n", argv[0]);
    return -2;
  }

  list(directory,argv[0]);
  return 0;
}

 /* Checar si existe 
  * Struct stat, devuelve información ordenada del archivo 
  * lstat, obtiene información del enlace del archivo 
  * S_ISDIR, comprueba si es un directorio
  * */
int dir_file(char *name){
  struct stat info;
  
  if(lstat(name, &info) < 0){
    return -1;
  }
  
  if(S_ISDIR(info.st_mode)){
    return 0;
  }

  else{
    return -2;
  }
}

/* Buscar solo dentro del directorio actual y desplegar el resultado de los archivos ordinarios 
 * Se ignoran los archivos no ordinarios ".." y "." , para evitar ciclar el programa 
 * */
void list(char *nombre_dir, char *program){
  char path[PATH_MAX + NAME_MAX + 1];
  DIR *dir;
  struct dirent *direntry;
  struct stat info;
  
  dir = opendir(nombre_dir);

  printf("directory: %s\n", nombre_dir);

  while((direntry = readdir(dir)) != NULL){
    if(strcmp(direntry->d_name, "..") != 0 && strcmp(direntry->d_name, ".") != 0){ 
      sprintf(path,"%s/%s",nombre_dir, direntry->d_name);
      lstat(path, &info);
      
      if((info.st_mode & S_IFMT)!=S_IFDIR){ 
        min_max(path, program);
      }
    }
  }

/* Realizar la busqueda de manera recursiva
 * Después de desplegar la información del directorio actual
 * Buscar en archivos, en caso de ser directorio, seguir buscando
 * Rewinddir, resetear posición 
 * */
  rewinddir(dir);
  while((direntry = readdir(dir)) != NULL){
    if(strcmp(direntry->d_name, "..") != 0 && strcmp(direntry->d_name, ".") != 0){ 
      sprintf(path,"%s/%s",nombre_dir, direntry->d_name);
      lstat(path, &info);
     
      if((info.st_mode & S_IFMT)==S_IFDIR){ 
        list(path, program);
      }
    }
  }
}

/* Buscar mínimo y máximo valor entero almacenado 
 * */

void min_max(char *file_name, char *program){
  int filedes, num, min, max;
  if((filedes = open(file_name, O_RDONLY)) < 0){
    fprintf(stderr, "%s: Could not open file\n", program);
  }
  
  if(read(filedes, &num, sizeof(uint)) != 0){
    min = num;
    max = num;
    while(read(filedes, &num, sizeof(uint)) != 0){
      if(num > max){ 
        max = num; 
      }else if(num < min){ 
        min = num;
      }
    }
  }
  printf("%s - min = %i - max = %i\n", file_name, min, max);
  close(filedes);
}
