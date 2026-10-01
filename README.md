# Logic Simulator

An interactive digital logic circuit designer and simulator written in C++. Build circuits visually from logic gates, switches, and LEDs, wire them together, simulate their behavior in real time, and automatically generate truth tables.

<img width="1665" height="966" alt="Sample 1 - Design" src="https://github.com/user-attachments/assets/c9e9a498-82e1-4ffd-abf1-18e15bec0bf2" />
<img width="1665" height="966" alt="Sample 1 - Simulation" src="https://github.com/user-attachments/assets/85c2acf1-338e-4a03-9534-f7fb4f06d5b6" />
<img width="1665" height="966" alt="Sample 2 - Design" src="https://github.com/user-attachments/assets/2d727bc9-0094-498d-a176-8823e44ee316" />
<img width="1665" height="966" alt="Sample 2 - Simulation" src="https://github.com/user-attachments/assets/92a912e5-6090-42ed-9fcb-9d8d55e43c40" />
<img width="1665" height="966" alt="Sample 3 - Design" src="https://github.com/user-attachments/assets/5337598e-2fea-4210-98bc-7fd94734536c" />
<img width="1665" height="966" alt="Sample 3 - Simulation" src="https://github.com/user-attachments/assets/6ca39bf1-0f01-499d-a5b5-53b1da14feb4" />

## Overview

Logic Simulator provides a two-mode workspace that mirrors how circuits are designed and tested in practice:

- **Design Mode:** place components, draw connections, label, move, copy, and organize the circuit.
- **Simulation Mode:** toggle switches and watch signals propagate through the circuit live, or generate a full truth table with one click.

The project is built around an object-oriented architecture that applies the Command pattern for user actions and polymorphism for circuit components, on top of the CMU Graphics library for the GUI.

## Features

**Components**
- 11 logic gates: Buffer, NOT, AND (2/3-input), OR, NAND, NOR (2/3-input), XOR (2/3-input), XNOR
- Switches (inputs) and LEDs (outputs)
- Wire connections between output and input pins, with visual on/off state

**Design tools**
- Click-to-place components with automatic boundary and overlap validation
- Select, move, delete, cut, copy, and paste single components or groups, with connections preserved
- Custom labels for any component
- Undo for recent actions (up to 8)
- New project, save, and load

**Simulation**
- Live signal propagation: toggling a switch updates every downstream gate and LED
- Detection of incomplete circuits (gates with unconnected inputs or outputs are flagged)
- Automatic truth table generation: the simulator enumerates all 2^n switch combinations, records every LED output, and renders the table in the window

**Persistence**
- Circuits are saved as human-readable text files, and the `Saved Circuits/` folder includes ready-to-load examples such as a half adder and a full adder

## Architecture

```
Project/
├── main.cpp                  Entry point: read action -> execute -> redraw
├── ApplicationManager.*      Central controller: component list, undo stack, action dispatch
├── Actions/                  One class per user action (Command pattern)
│   ├── Action.h              Abstract base: Execute() / Undo()
│   ├── AddGate, AddConnection, Select, SelectionTools
│   ├── Save, Load, NewProject, Paste
│   └── Operate, CreateTruthTable
├── Components/               Circuit model
│   ├── Component             Abstract base class
│   ├── Gate, Connection      Concrete components
│   └── Pin, InputPin, OutputPin
├── GUI/                      Input handling, drawing, and UI constants
├── CMUgraphicsLib/           Third-party graphics library
├── Images/                   Gate, toolbar, and state icons
└── Saved Circuits/           Example and user-saved circuits
```

**Key design decisions**
- **Command pattern:** every user operation is an `Action` object with `Execute()` and `Undo()`, which keeps the controller simple and makes undo systematic.
- **Polymorphic components:** `Gate` and `Connection` derive from a common `Component` base, so the controller can store, draw, and operate on them uniformly.
- **Staged propagation:** simulation starts at the switches and advances stage by stage (connections, then gates), re-evaluating each gate once its inputs update until the signal has reached every output.
- **Simple file format:** saved circuits list gates (type, ID, label, position) followed by connections (source ID, destination ID, input pin), terminated by a sentinel value.

## Getting Started

### Requirements
- Windows
- Visual Studio 2022 (MSVC v143 toolset)

### Build and run
1. Clone the repository.
2. Open `Project/graphics_prj.sln` in Visual Studio.
3. Select the **Debug** configuration and build the solution.
4. Run the application with `Project/` as the working directory so the `Images/` and `Saved Circuits/` folders resolve correctly.

## Usage

1. **Build:** pick a component from the left toolbar and click the canvas to place it.
2. **Wire:** choose the connection tool, then click a source and a destination gate.
3. **Edit:** click a component to open its toolbar for label, move, delete, copy, and cut. Use **Paste** from the top bar.
4. **Simulate:** switch to Simulation Mode, click switches to change inputs, and observe the LEDs.
5. **Analyze:** in Simulation Mode, use the truth table button to generate the table for the current circuit.
6. **Save / Load:** use the top bar to save a circuit by name or load an existing one (try `Full adder`).

## Example Circuits

| File | Description |
|------|-------------|
| `Half adder.txt` | Sum and carry from two input bits |
| `Full adder.txt` | Sum and carry-out from A, B, and carry-in |

## Skills Demonstrated

- Object-oriented design in C++ (inheritance, polymorphism, abstract interfaces)
- Design patterns: Command (undo-able actions)
- Event-driven GUI programming
- Graph traversal for signal propagation
- File I/O and a custom serialization format
- Manual memory management and resource handling

## Author

**[Your Name]**
[LinkedIn](https://www.linkedin.com/in/eyad-hossni-nada/) · [GitHub](https://github.com/EyadHossni)

## Acknowledgments

Graphics powered by the CMU Graphics Library, which is bundled in `CMUgraphicsLib/` under its original terms.
