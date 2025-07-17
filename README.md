**EcoSim**

**Description**
EcoSim is a multithreaded C++ application modeling a dynamic, emergent ecosystem. From simple rules governing organisms’ behaviors (plants, herbivores, carnivores), complex interactions and emergent patterns arise on a 2D grid world.

---

## Table of Contents

1. [Features](#features)
2. [Getting Started](#getting-started)

   - [Prerequisites](#prerequisites)
   - [Building](#building)
   - [Running the Simulation](#running-the-simulation)

3. [Core Simulation Mechanics](#core-simulation-mechanics)

   - [The World](#the-world)
   - [Ticks & Update Cycle](#ticks--update-cycle)
   - [Energy Model](#energy-model)

4. [Entities & Behaviors](#entities--behaviors)

   - [Organism (Base Class)](#organism-base-class)
   - [Plant](#plant)
   - [Animal (Base Class)](#animal-base-class)

     - [Herbivore](#herbivore-eg-sheep-🐑)
     - [Carnivore](#carnivore-eg-wolf-🐺)

5. [Configuration](#configuration)
6. [Testing](#testing)
7. [Roadmap & Future Work](#roadmap--future-work)
8. [Contributing](#contributing)
9. [License](#license)

---

## Features

- Discrete 2D grid world (configurable size, e.g., 80×40)
- Emergent behaviors from simple rules
- Multithreaded update loop for performance
- Energy-based lifecycle: movement, eating, reproduction, death
- Expandable: easily add new organism types or behaviors

---

## Getting Started

### Prerequisites

- C++17-compatible compiler (e.g., GCC ≥ 9, Clang ≥ 10, MSVC 2019)
- CMake ≥ 3.15
- Google Test for unit testing

### Building

```bash
# Clone repository
git clone https://github.com/your-org/ecosim.git
cd ecosim

# Create build directory
mkdir build && cd build

# Generate build files
cmake ..

# Compile
cmake --build . -- -j 4
```

### Running the Simulation

```bash
./EcoSim [--width 80] [--height 40] [--threads 4]
```

Options:

- `--width`, `-w`: world grid width (default: 80)
- `--height`, `-h`: world grid height (default: 40)
- `--threads`, `-t`: number of worker threads (default: hardware concurrency)

---

## Core Simulation Mechanics

### The World

A 2D grid where each tile may be empty or host multiple organisms. Coordinates are zero-based `(x, y)`.

### Ticks & Update Cycle

Each tick (turn) consists of:

1. **Animal Actions**: each animal runs `update()` (move, hunt, eat).
2. **Energy Management**: passive energy costs deducted.
3. **Lifecycle Events**: handle deaths (energy ≤ 0) and reproductions.
4. **Environmental Changes**: new plants grow per reproduction chance.

### Energy Model

- Passive cost per tick (`energy_cost_per_tick`).
- Gain from eating (plants or herbivores).
- Death at energy ≤ 0.
- Energy spent on reproduction.

---

## Entities & Behaviors

### Organism (Base Class)

```cpp
class Organism {
public:
    int x, y;          // grid position
    char symbol;       // console display symbol
    virtual void update(World& world) = 0;
};
```

### Plant

- **Symbol**: `*` (green)
- **Property**: `double reproduction_chance`
- **Behavior**:

  1. Roll random \[0.0,1.0).
  2. If < `reproduction_chance`, pick a random empty neighbor and spawn a new Plant.

### Animal (Base Class)

```cpp
class Animal : public Organism {
public:
    int energy, max_energy;
    int vision_range;
    int energy_cost_per_tick;

protected:
    void moveTowards(int tx, int ty);
    void moveAwayFrom(int tx, int ty);
    void moveRandomly();
};
```

#### Herbivore (e.g., Sheep 🐑)

- **Symbol**: `H` (white)
- **Starting Energy**: 100 / max 200
- **Vision**: 4 tiles
- **Cost/Tick**: 1 energy
- **Gain**: 25 per Plant
- **Reproduction**: threshold 150, cost 75
- **Logic** (_update()_):

  1. **Flee** if a Carnivore within vision: `moveAwayFrom(nearest Carnivore)`.
  2. **Seek** Plant: if on plant, eat; else `moveTowards(nearest Plant)`.
  3. **Wander**: `moveRandomly()`.

#### Carnivore (e.g., Wolf 🐺)

- **Symbol**: `C` (red)
- **Starting Energy**: 150 / max 300
- **Vision**: 6 tiles
- **Cost/Tick**: 2 energy
- **Gain**: 100 per Herbivore
- **Reproduction**: threshold 250, cost 125
- **Logic** (_update()_):

  1. **Hunt** Herbivore: if on prey, eat; else `moveTowards(nearest Herbivore)`.
  2. **Patrol**: `moveRandomly()`.

---

## Configuration

Parameters (grid size, reproduction chances, initial populations) can be adjusted via `config.json` or command-line flags.

---

## Testing

Unit tests in `tests/` use Google Test. Run:

```bash
cd build && ctest --output-on-failure
```

---

## Roadmap & Future Work

- **MVP**: core ecosystem mechanics, single-threaded stable release
- **Phase 2**: multithreading, performance tuning
- **Phase 3**: new species, environmental effects (weather, seasons)
- **Phase 4**: GUI visualization
- **Phase 5**: plugin system for custom behaviors

---

## Contributing

Contributions welcome! Please open issues for bugs or feature requests, and submit pull requests with tests.

---

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.
