```
 ██████╗ ██████╗ ██████╗ ███████╗██╗  ██╗██╗ ██████╗ ███╗   ██╗
██╔════╝██╔═══██╗██╔══██╗██╔════╝╚██╗██╔╝██║██╔═══██╗████╗  ██║
██║     ██║   ██║██║  ██║█████╗   ╚███╔╝ ██║██║   ██║██╔██╗ ██║
██║     ██║   ██║██║  ██║██╔══╝   ██╔██╗ ██║██║   ██║██║╚██╗██║
╚██████╗╚██████╔╝██████╔╝███████╗██╔╝ ██╗██║╚██████╔╝██║ ╚████║
 ╚═════╝ ╚═════╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝
```

<div align="center">

![42 Badge](https://img.shields.io/badge/42-Málaga-000000?style=for-the-badge&logo=42&logoColor=white)
![Language](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Threads](https://img.shields.io/badge/Threads-pthreads-FF6B35?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Complete-00D4AA?style=for-the-badge)

*N coders. N dongles. One deadline. No burnout.*

</div>

---

## 📖 About

**Codexion** is a concurrency project in C that reimagines the classic **Dining Philosophers** problem with a developer twist.

> N coders sit around a table. Between each pair of coders lies a shared USB dongle. To compile, a coder needs **two dongles** — the one to their left and the one to their right. If a coder goes too long without compiling, they **burn out**.

The challenge: manage synchronization, prevent deadlocks, avoid race conditions, and ensure no coder burns out — using two different scheduling strategies.

---

## ⚙️ Compilation

```bash
make        # build
make clean  # remove objects
make fclean # remove objects + binary
make re     # rebuild from scratch
```

---

## 🚀 Usage

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> \
           <time_to_debug> <time_to_refactor> \
           <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coder threads |
| `time_to_burnout` | Max ms a coder can go without compiling |
| `time_to_compile` | Duration of compile phase (ms) |
| `time_to_debug` | Duration of debug phase (ms) |
| `time_to_refactor` | Duration of refactor phase (ms) |
| `number_of_compiles_required` | Compiles needed per coder to finish |
| `dongle_cooldown` | Ms a dongle must rest after being released |
| `scheduler` | `fifo` or `edf` |

### Examples

```bash
# Normal run — all coders finish
./codexion 5 800 200 200 200 3 100 fifo

# Burnout scenario — compile takes longer than burnout window
./codexion 5 300 400 200 200 3 100 fifo

# EDF prevents burnout where FIFO might fail
./codexion 5 700 200 200 200 3 100 edf
./codexion 5 700 200 200 200 3 100 fifo
```

---

## 📋 Output Format

Each event is logged as:

```
<timestamp_ms> <coder_id> <action>
```

```
1700000000000 3 has taken a dongle
1700000000001 3 has taken a dongle
1700000000001 3 is compiling
1700000000201 3 is debugging
1700000000401 3 is refactoring
1700000000601 0 all coders finished successfully
```

A coder ID of `0` is reserved for simulation-level events (victory / burnout detection).

---

## 🧠 Core Concepts

### The Deadlock Problem

If every coder picks up their **left dongle simultaneously**, all are blocked waiting for their right — classic circular wait. 

**Solution:** Parity-based lock ordering.
- Even-ID coders: lock **left → right**
- Odd-ID coders: lock **right → left**

This breaks the circular dependency.

### Dongle Cooldown

After a dongle is released, it must rest for `dongle_cooldown` ms before being reacquired. This simulates hardware constraints and forces fairer resource distribution.

### Burnout Detection

A dedicated **monitor thread** runs in parallel, checking every ~1ms whether any coder has exceeded `time_to_burnout` ms since their last compile. Detection margin: ≤10ms.

---

## 📅 Schedulers

### FIFO — First In, First Out

Coders waiting for dongles are served in arrival order. The coder that has been waiting the longest gets priority. Fair by definition.

### EDF — Earliest Deadline First

Coders are prioritized by how close they are to burning out. The coder with the least time remaining before burnout gets to go first. Optimal for preventing burnout under tight timing constraints.

> EDF outperforms FIFO when `time_to_burnout` is close to `time_to_compile + time_to_debug + time_to_refactor`.

---

## 🔧 Thread Architecture

| Thread | Role |
|---|---|
| `coder[i]` | Runs the compile → debug → refactor cycle |
| `monitor` | Watches for burnout; declares victory when all finish |

### Synchronization Mechanisms

- **`pthread_mutex_t`** per dongle — prevents simultaneous access
- **`pthread_mutex_t print_mutex`** — serializes all console output
- **`volatile`** on shared coder fields (`stop`, `last_compile`, `compiles_done`) — prevents compiler caching across threads
- **`interruptible_sleep`** — replaces raw `usleep` with stop-aware 1ms polling loop
- **`wait_cooldown`** — enforces dongle rest period with stop-awareness

---

## ⚠️ Edge Cases Handled

| Case | Behavior |
|---|---|
| 1 coder | Left and right dongle are the same — burnout is immediate and clean |
| Burnout during compile | `interruptible_sleep` catches `stop=1` and exits early |
| Coder blocked in mutex after burnout | Stop checks before/after each lock + parity ordering prevent deadlock |
| All coders finish | Monitor detects victory and sets `stop=1` |
| EDF with multiple critical coders | `wait_start` is cleared only after both dongles are acquired, preserving scheduling visibility |

---

## 📁 Project Structure

```
codexion/
├── Makefile
├── README.md
└── coders/
    ├── include/
    │   └── codexion.h       # structs, defines, prototypes
    └── src/
        ├── main.c           # entry point
        ├── init.c           # parsing, allocation, cleanup
        ├── threads.c        # coder routine, monitor, schedulers
        └── utils.c          # get_time, print_action
```

---

## 🧪 Test Suite

```bash
# Base cases
./codexion 5 800 200 200 200 3 100 fifo
./codexion 5 800 200 200 200 3 100 edf

# Edge: single coder
./codexion 1 800 200 200 200 3 100 fifo

# Edge: two coders
./codexion 2 800 200 200 200 3 100 fifo

# Burnout inevitable
./codexion 5 300 400 200 200 3 100 fifo

# Zero cooldown
./codexion 5 800 200 200 200 3 0 fifo

# High cooldown
./codexion 4 1500 200 200 200 3 500 fifo

# High contention
./codexion 10 1500 200 200 200 3 100 fifo

# EDF vs FIFO — tight margin (EDF should survive, FIFO may not)
./codexion 5 700 200 200 200 3 100 edf
./codexion 5 700 200 200 200 3 100 fifo
```

---

## 👤 Author

**ybel-maa** — 42 Málaga

---

<div align="center">
<sub>Built with threads, mutexes, and an unhealthy amount of usleep(1000).</sub>
</div>
