*Este proyecto ha sido creado como parte del currículo de 42 por ybel-maa.*

# 🧠 Codexion

## 📖 Descripción

Codexion es un proyecto de concurrencia en C que simula múltiples programadores compitiendo por recursos limitados (dongles USB). El objetivo es gestionar sincronización, evitar deadlocks, race conditions y burnout.

## ⚙️ Instrucciones

### Compilación
make

### Ejecución
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler

### Ejemplo
./codexion 5 800 200 200 200 3 100 fifo

## 🧠 Conceptos clave
- pthread
- mutex
- sincronización
- planificación FIFO / EDF

## ⚠️ Blocking cases handled
(To be completed)

## 🔧 Thread synchronization mechanisms
(To be completed)
