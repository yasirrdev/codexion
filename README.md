*Este proyecto ha sido creado como parte del currículo de 42 por ybel-maa.*

# Codexion

## Descripción

Codexion es un proyecto de concurrencia en C que reimagina el clásico problema de los filósofos comensales. Varios programadores (coders) comparten dongles USB en una mesa circular: cada coder necesita dos dongles adyacentes (izquierdo y derecho) para compilar. Si un coder pasa demasiado tiempo sin compilar, sufre burnout y la simulación termina.

Cada coder es un hilo POSIX que cicla entre compilar, depurar y refactorizar. Un hilo monitor independiente detecta el burnout con precisión de milisegundos. El acceso a los dongles se arbitra mediante dos políticas de planificación implementadas sobre una cola de prioridad (heap): FIFO y EDF.

## Instrucciones

### Compilación

```
make        # compila el binario codexion
make clean  # elimina los objetos
make fclean # elimina objetos y binario
make re     # recompila desde cero
```

### Ejecución

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Todos los argumentos son obligatorios:

- `number_of_coders`: número de coders (y de dongles)
- `time_to_burnout`: ms máximos sin compilar antes del burnout
- `time_to_compile`: duración de la compilación en ms
- `time_to_debug`: duración de la depuración en ms
- `time_to_refactor`: duración de la refactorización en ms
- `number_of_compiles_required`: compilaciones necesarias por coder para terminar
- `dongle_cooldown`: ms de espera de un dongle tras ser liberado
- `scheduler`: `fifo` o `edf`

### Ejemplos

```
./codexion 5 800 200 200 200 3 100 fifo
./codexion 5 800 200 200 200 3 100 edf
./codexion 5 300 400 200 200 3 100 fifo   # burnout inevitable
```

### Formato del log

```
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```

## Recursos

### Referencias

- Dijkstra, E.W. — Hierarchical ordering of sequential processes (origen del problema de los filósofos comensales)
- POSIX Threads Programming — Lawrence Livermore National Laboratory: https://hpc-tutorials.llnl.gov/posix/
- The Little Book of Semaphores — Allen B. Downey: https://greenteapress.com/wp/semaphores/
- Dining Philosophers Problem — Wikipedia: https://en.wikipedia.org/wiki/Dining_philosophers_problem
- Earliest Deadline First Scheduling — Wikipedia: https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling
- Páginas de manual: man pthread_create, man pthread_mutex_init, man pthread_cond_init, man gettimeofday

### Uso de IA

Se utilizó Claude (Anthropic) como apoyo durante el desarrollo del proyecto, en las siguientes tareas y partes:

- Conceptos de concurrencia: explicación de hilos, mutexes, condiciones de Coffman y race conditions antes de comenzar la implementación.
- Depuración: identificación de bugs concretos durante el desarrollo, c*Este proyecto ha sido creado como parte del currículo de 42 por ybel-maa.*

# Codexion

## Descripción

Codexion es un proyecto de concurrencia en C que reimagina el clásico problema de los filósofos comensales. Varios programadores (coders) comparten dongles USB en una mesa circular: cada coder necesita dos dongles adyacentes (izquierdo y derecho) para compilar. Si un coder pasa demasiado tiempo sin compilar, sufre burnout y la simulación termina.

Cada coder es un hilo POSIX que cicla entre compilar, depurar y refactorizar. Un hilo monitor independiente detecta el burnout con precisión de milisegundos. El acceso a los dongles se arbitra mediante dos políticas de planificación implementadas sobre una cola de prioridad (heap): FIFO y EDF.

## Instrucciones

### Compilación

```
make        # compila el binario codexion
make clean  # elimina los objetos
make fclean # elimina objetos y binario
make re     # recompila desde cero
```

### Ejecución

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Todos los argumentos son obligatorios:

- `number_of_coders`: número de coders (y de dongles)
- `time_to_burnout`: ms máximos sin compilar antes del burnout
- `time_to_compile`: duración de la compilación en ms
- `time_to_debug`: duración de la depuración en ms
- `time_to_refactor`: duración de la refactorización en ms
- `number_of_compiles_required`: compilaciones necesarias por coder para terminar
- `dongle_cooldown`: ms de espera de un dongle tras ser liberado
- `scheduler`: `fifo` o `edf`

### Ejemplos

```
./codexion 5 800 200 200 200 3 100 fifo
./codexion 5 800 200 200 200 3 100 edf
./codexion 5 300 400 200 200 3 100 fifo   # burnout inevitable
```

### Formato del log

```
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```

## Recursos

### Referencias

- Dijkstra, E.W. — Hierarchical ordering of sequential processes (origen del problema de los filósofos comensales)
- POSIX Threads Programming — Lawrence Livermore National Laboratory: https://hpc-tutorials.llnl.gov/posix/
- The Little Book of Semaphores — Allen B. Downey: https://greenteapress.com/wp/semaphores/
- Dining Philosophers Problem — Wikipedia: https://en.wikipedia.org/wiki/Dining_philosophers_problem
- Earliest Deadline First Scheduling — Wikipedia: https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling
- Páginas de manual: man pthread_create, man pthread_mutex_init, man pthread_cond_init, man gettimeofday

### Uso de IA

Se utilizó Claude (Anthropic) como apoyo durante el desarrollo del proyecto, en las siguientes tareas y partes:

- Conceptos de concurrencia: explicación de hilos, mutexes, condiciones de Coffman y race conditions antes de comenzar la implementación.
- Depuración: identificación de bugs concretos durante el desarrollo, como race conditions de visibilidad en `last_compile`, la llamada a `release_dongles` sin dongles adquiridos, y errores en la implementación del heap (comparación de puntero con entero, doble decremento del tamaño).
- Diseño: validación del enfoque de paridad para la prevención de deadlocks y del diseño del scheduler basado en heap de prioridad (archivos `dongles.c`, `heap.c` y `scheduler.c`).
- Revisión: verificación del cumplimiento del subject y de la Norma antes de la entrega, y generación de casos de prueba.

Todo el código fue revisado, probado y comprendido antes de ser incluido en el proyecto.

## Blocking cases handled

- **Deadlock (espera circular)**: si todos los coders toman su dongle izquierdo a la vez, se produce una espera circular (condiciones de Coffman). Se rompe con orden de adquisición por paridad: los coders de ID par toman izquierda→derecha y los de ID impar derecha→izquierda, eliminando el ciclo de dependencias.

- **Prevención de inanición (starvation)**: el scheduler basado en heap garantiza que ningún coder espere indefinidamente. En FIFO se respeta el orden de llegada; en EDF siempre avanza el coder más cercano al burnout, evitando que la mala suerte en la contención cause agotamientos evitables.

- **Gestión del cooldown**: tras liberarse, cada dongle registra su timestamp (`last_used`) y no puede tomarse de nuevo hasta que transcurra `dongle_cooldown`. La espera comprueba la señal de parada cada 1 ms para no bloquear el cierre de la simulación.

- **Detección precisa del agotamiento**: un hilo monitor comprueba cada ~1 ms si algún coder ha superado `time_to_burnout` desde el inicio de su última compilación, garantizando el log de burnout dentro del margen de 10 ms exigido. El monitor omite a los coders que ya completaron sus compilaciones para evitar falsos positivos.

- **Serialización del log**: todos los mensajes pasan por una única función protegida con mutex, de modo que nunca se mezclan dos mensajes en una misma línea.

- **Parada limpia**: los sleeps largos se sustituyen por una espera interrumpible que comprueba la bandera de parada cada 1 ms, y se verifica dicha bandera antes de cada bloqueo de mutex, evitando que hilos queden colgados tras el fin de la simulación.

- **Caso de un solo coder**: con un coder, los dongles izquierdo y derecho son el mismo. Se detecta y se toma un único dongle, evitando el doble lock del mismo mutex.

- **Liberación segura de dongles**: `take_dongles` devuelve un entero indicando si los dongles fueron realmente adquiridos; solo en ese caso se llama a `release_dongles`, evitando desbloquear mutexes no bloqueados (comportamiento indefinido).

## Thread synchronization mechanisms

Primitivas utilizadas:

- **`pthread_mutex_t` por dongle**: cada dongle tiene su propio mutex. Un coder debe adquirir el lock antes de usar el dongle, garantizando acceso exclusivo e impidiendo la duplicación del recurso.

- **`pthread_mutex_t print_mutex`**: toda escritura al terminal pasa por `print_action`, que bloquea este mutex antes de `printf` y lo libera después. Así se evita que dos hilos mezclen sus líneas.

- **`pthread_mutex_t heap_mutex`**: protege la cola de prioridad compartida. Las operaciones de inserción, extracción y eliminación se realizan íntegramente bajo este lock, evitando condiciones de carrera en el scheduler.

- **`volatile` en campos compartidos**: `stop`, `last_compile`, `compiles_done` y `wait_start` se declaran `volatile` para impedir que el compilador cachee sus valores en registros, garantizando que todos los hilos lean siempre el valor actual de memoria.

Cómo se evitan las condiciones de carrera:

- Cada campo `volatile` de un coder tiene un único hilo escritor (el propio coder), mientras que el monitor solo lee. Este patrón de un-escritor/múltiples-lectores junto con `volatile` garantiza visibilidad sin necesidad de mutex en la ruta crítica de detección de burnout.
- La bandera `stop` solo la escribe el monitor (transición única de 0 a 1) y la leen todos los demás hilos, funcionando como señal de parada thread-safe.
- El heap es la única estructura con múltiples escritores, por lo que todas sus operaciones están serializadas con `heap_mutex`. La comprobación de "estoy en el tope" y la extracción se hacen bajo el mismo lock, evitando que dos coders crean simultáneamente tener prioridad.

Comunicación entre coders y monitor:

- Los coders actualizan `last_compile` al inicio de cada compilación y `compiles_done` al terminarla.
- El monitor lee estos campos cada ~1 ms: si detecta un burnout imprime el log y activa `stop`; si todos los coders alcanzaron el número requerido de compilaciones, declara el fin de la simulación.
- Los coders comprueban `stop` en cada punto de espera (scheduler, cooldown, sleeps y antes de cada lock), garantizando una terminación ordenada de todos los hilos mediante `pthread_join`.omo race conditions de visibilidad en `last_compile`, la llamada a `release_dongles` sin dongles adquiridos, y errores en la implementación del heap (comparación de puntero con entero, doble decremento del tamaño).
- Diseño: validación del enfoque de paridad para la prevención de deadlocks y del diseño del scheduler basado en heap de prioridad (archivos `dongles.c`, `heap.c` y `scheduler.c`).
- Revisión: verificación del cumplimiento del subject y de la Norma antes de la entrega, y generación de casos de prueba.

Todo el código fue revisado, probado y comprendido antes de ser incluido en el proyecto.

## Blocking cases handled

- **Deadlock (espera circular)**: si todos los coders toman su dongle izquierdo a la vez, se produce una espera circular (condiciones de Coffman). Se rompe con orden de adquisición por paridad: los coders de ID par toman izquierda→derecha y los de ID impar derecha→izquierda, eliminando el ciclo de dependencias.

- **Prevención de inanición (starvation)**: el scheduler basado en heap garantiza que ningún coder espere indefinidamente. En FIFO se respeta el orden de llegada; en EDF siempre avanza el coder más cercano al burnout, evitando que la mala suerte en la contención cause agotamientos evitables.

- **Gestión del cooldown**: tras liberarse, cada dongle registra su timestamp (`last_used`) y no puede tomarse de nuevo hasta que transcurra `dongle_cooldown`. La espera comprueba la señal de parada cada 1 ms para no bloquear el cierre de la simulación.

- **Detección precisa del agotamiento**: un hilo monitor comprueba cada ~1 ms si algún coder ha superado `time_to_burnout` desde el inicio de su última compilación, garantizando el log de burnout dentro del margen de 10 ms exigido. El monitor omite a los coders que ya completaron sus compilaciones para evitar falsos positivos.

- **Serialización del log**: todos los mensajes pasan por una única función protegida con mutex, de modo que nunca se mezclan dos mensajes en una misma línea.

- **Parada limpia**: los sleeps largos se sustituyen por una espera interrumpible que comprueba la bandera de parada cada 1 ms, y se verifica dicha bandera antes de cada bloqueo de mutex, evitando que hilos queden colgados tras el fin de la simulación.

- **Caso de un solo coder**: con un coder, los dongles izquierdo y derecho son el mismo. Se detecta y se toma un único dongle, evitando el doble lock del mismo mutex.

- **Liberación segura de dongles**: `take_dongles` devuelve un entero indicando si los dongles fueron realmente adquiridos; solo en ese caso se llama a `release_dongles`, evitando desbloquear mutexes no bloqueados (comportamiento indefinido).

## Thread synchronization mechanisms

Primitivas utilizadas:

- **`pthread_mutex_t` por dongle**: cada dongle tiene su propio mutex. Un coder debe adquirir el lock antes de usar el dongle, garantizando acceso exclusivo e impidiendo la duplicación del recurso.

- **`pthread_mutex_t print_mutex`**: toda escritura al terminal pasa por `print_action`, que bloquea este mutex antes de `printf` y lo libera después. Así se evita que dos hilos mezclen sus líneas.

- **`pthread_mutex_t heap_mutex`**: protege la cola de prioridad compartida. Las operaciones de inserción, extracción y eliminación se realizan íntegramente bajo este lock, evitando condiciones de carrera en el scheduler.

- **`volatile` en campos compartidos**: `stop`, `last_compile`, `compiles_done` y `wait_start` se declaran `volatile` para impedir que el compilador cachee sus valores en registros, garantizando que todos los hilos lean siempre el valor actual de memoria.

Cómo se evitan las condiciones de carrera:

- Cada campo `volatile` de un coder tiene un único hilo escritor (el propio coder), mientras que el monitor solo lee. Este patrón de un-escritor/múltiples-lectores junto con `volatile` garantiza visibilidad sin necesidad de mutex en la ruta crítica de detección de burnout.
- La bandera `stop` solo la escribe el monitor (transición única de 0 a 1) y la leen todos los demás hilos, funcionando como señal de parada thread-safe.
- El heap es la única estructura con múltiples escritores, por lo que todas sus operaciones están serializadas con `heap_mutex`. La comprobación de "estoy en el tope" y la extracción se hacen bajo el mismo lock, evitando que dos coders crean simultáneamente tener prioridad.

Comunicación entre coders y monitor:

- Los coders actualizan `last_compile` al inicio de cada compilación y `compiles_done` al terminarla.
- El monitor lee estos campos cada ~1 ms: si detecta un burnout imprime el log y activa `stop`; si todos los coders alcanzaron el número requerido de compilaciones, declara el fin de la simulación.
- Los coders comprueban `stop` en cada punto de espera (scheduler, cooldown, sleeps y antes de cada lock), garantizando una terminación ordenada de todos los hilos mediante `pthread_join`.
