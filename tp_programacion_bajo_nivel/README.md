# Sistema de Gestión de Estudiantes y Materias

## Integrantes
- Agustin Andres Scazzino
- Brenda Tosini
- Gonzalo Giuffre
- Ivan Garay
- Martin Moreno

## Descripción
Este proyecto es un sistema de gestión de estudiantes y materias desarrollado en C. Permite dar de alta, modificar, eliminar y listar estudiantes y materias, así como inscribir estudiantes en materias y registrar cuando rinden materias.

## Configuración del entorno local en Windows
1. Descargar MSys2 https://www.msys2.org/, dirigirse hasta titulo Instalación y descargar el archivo **msys2-x86_64-20240507.exe**
2. Abrir consola MSys2 y correr los siguientes comandos: 
```sh
$ pacman -Syu
$ pacman -Su  
```
3. Instalar librerías externas:
```sh
$ pacman -S mingw-w64-x86_64-cunit
$ pacman -S mingw-w64-x86_64-gtk3 
$ pacman -S mingw-w64-x86_64-gtk3 mingw-w64-x86_64-glib2
```
Aclaración : Se utilizo versión gtk 3.


## Configuración del entorno local en Linux
1. En la consola ejecutar los siguientes comandos para instalar GTK3:
```sh
sudo apt update
sudo apt install build-essential
sudo apt install libgtk-3-dev
```
2. En la consola ejecutar los siguientes comandos para instalar CUNIT:
```sh
sudo apt update
sudo apt install libcunit1 libcunit1-doc libcunit1-dev
```
## Configuración del entorno local en MacOS
1. En la consola ejecutar los siguientes comandos para instalar GTK3:
```sh
brew install gtk+3
```
2. En la consola ejecutar los siguientes comandos para instalar CUNIT:
```sh
brew install cunit
```
## Funcionalidades Implementadas
- Alta, modificación, eliminación y listado de estudiantes.
- Búsqueda de estudiantes por nombre y por rango de edad.
- Alta, modificación, eliminación y listado de materias.
- Inscripción de estudiantes en materias.
- Rendir materias.

## Funcionalidades Extra Implementadas
- Paginado de las tablas estudiantes y materias
- poder elegir el estudiante de un listado reducido (Búsqueda por rango de edad)
- generar estudiantes y materias de prueba
- estadísticas de los estudiantes y materias (Promedio de edad y cantidad de materias)
- Validación si se puede anotar a una materia
- Calculo de promedio
- archivo de configuración general donde se especifican las variables del sistema (Sistema modular con archivos de encabezado y código separado carpeta include)
- mejorar la interfaz de usuario (CSS y gtk3)
- Test unitarios
- Persistencia en csv

## Observaciones
Cantidad de información donde el sistema sigue siendo manejable es de 2.500.000 registros por estudiante y materias.
Video: [Sistema de Gestión de Estudiantes y Materias/ TP Bajo Nivel UNTREF](https://youtu.be/3HTksu8qvio)

## Compilación y Ejecución
Para compilar el proyecto, dirigirse al directorio del proyecto y ejecutar el siguiente comando:
```sh
$ make
$ /bin/main
```
Aclaracion : Si estas en Windows este paso se debe de ejecutar desde la consola de MSys2.
```sh
$ make
$ /bin/main.exe
```
