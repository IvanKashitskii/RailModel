# RailModel

Discrete-event simulation of wagon operations on a rail network.  
Independent research project (2021) written in C++.  
The model explores how operational conditions, dispatching policies, and information completeness affect wagon turnaround and fleet utilization.

---

## Overview

**RailModel** simulates daily wagon circulation within a simplified railway network.  
It measures how network distances, travel times, empty-run routing, and control policies influence efficiency indicators such as:
- empty-run ratio,  
- wagon turnaround,  
- fleet utilization.

The model compares alternative operating regimes — for example, unified vs. split fleets, and systems with or without redistribution.

---

## Methodology

The simulation combines deterministic and stochastic elements:

- **Shortest paths** between stations are precomputed by the **Dijkstra algorithm** (by distance and by time).  
- **Daily operations** follow a discrete cycle:
  1. cancel a share of unserved requests,  
  2. generate new requests with uneven intensity,  
  3. move wagons along active routes,  
  4. assign available wagons to open requests.  
- **Assignment problem** is solved using the **Hungarian algorithm** on a bipartite graph,  
  with weights defined by the selected control objective:  
  - *L* — minimize empty-run kilometers,  
  - *T* — minimize delivery time,  
  - *C* — minimize empty-run cost,  
  - *P* — maximize profit.

---

## Input Data

Input files in `.xlsx` format:
- `distances.xlsx` — inter-station distances,  
- `runtimes.xlsx` — average travel times,  
- `emptycosts.xlsx` — empty-run penalties.

---

## Visualization & Analysis

Network topology and routing paths were visualized using **Grin (Graph Interface)**.  
Statistical analysis and result plots were produced in **Python 3** with **NumPy** and **Matplotlib** libraries.  
Typical outputs include wagon turnaround distributions, empty-run ratios, and utilization trends under different scenarios.

---

## Key Findings

- Dispatching regimes **with redistribution** significantly reduce empty-run ratios compared to static allocation.  
- Differences between **unified and split fleets** are minor under otherwise identical settings.  
- Incomplete information (limited market visibility) decreases fleet efficiency.

---

## Implementation

- **Language:** C++17  
- **Environment:** Visual Studio Community 2019  
- **Model:** discrete-event simulation with precomputed routes and daily iterative loop  
- **Algorithms:** Dijkstra (routing), Hungarian (assignment)

---

## Related Publication

Kashitskii I. (2021). *О влиянии различных условий управления вагонными парками на качество использования подвижного состава.*  
Conference on Transport Systems, October 2021.  
[📄 Read paper (PDF)](docs/Конференция 20 октября 2021 г. - О влиянии различных условий на показатели.pdf)

---

## Author

**Ivan Kashitskii**  
Rail operations & process analytics • Tallinn, Estonia
