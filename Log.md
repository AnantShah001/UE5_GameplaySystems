# UE5_GameplaySystems

## 📅 5\1\26
## 📘 Unreal Engine 5 C++ – Development Log

This file tracks my daily work, experiments, and observations
while building gameplay systems in **Unreal Engine 5 using C++**.

The purpose of this log is:
- Long-term reference
- Clear documentation of system-level understanding
- Tracking how gameplay code evolves over time

---

### Project Information
- **Project Name:** UE5-Gameplay-Systems
- **Engine:** Unreal Engine 5
- **Template:** Third Person (C++)
- **Language:** C++
- **Version Control:** GitHub

---

### Work Done
- Created a new Unreal Engine 5 project using the **Third Person C++ template**
- Set up GitHub repository for continuous version control
- Added this development log for documentation and iteration tracking

---

### Technical Notes
- The Third Person template provides a solid baseline for gameplay system development
- Movement, jumping, and camera logic are already implemented and can be extended later
- This setup allows focus on **system design and C++ architecture**.

---

### Repository :- **UE5_GameplaySystems**

🔗 GitHub :  [Anant Shah](https://github.com/AnantShah001)

[UE5_GameplaySystems](https://github.com/AnantShah001/UE5_GameplaySystems)

---

### Screenshots

**Unreal Editor**
![UE Editor](Screenshots/UE_Editor.png)

**IDE / Source Code**
![IDE](Screenshots/IDE.png)

**Gameplay View**
![Gameplay](GamePlayScreenshots/GamePlay.png)

---

🔗 [GitHub Previous Commit](https://github.com/AnantShah001/UE5_GameplaySystems/commit/b9555e0559055a49e76a37f8694d85c73b611429)

## 📅 Day 1 – Project Setup & Create DebugActor Class
- Created a new class called DebugActor derived from AActor.
- Blueprint child class derived from `ADebugActor`
- Used for level placement and iteration
- Demonstrates proper C++ → Blueprint inheritance workflow
- Implemented functionality to print debug messages using UE_LOG.
- Used GEngine->AddOnScreenDebugMessage to display messages on the game screen.

 > Blueprint Editor Terminology:
* `Viewport` = `This is where we can see our Actor in 3D Space`
* `Event Graph` = `This is Execute After Game Starts`
* `Construction_Script` = `This is Execute Before Game Starts`

**Showing Debugging**
![Showing Debugging](GamePlayScreenshots/ShowingDebugging.png)

---

🔗 [GitHub Previous Commit](https://github.com/AnantShah001/UE5_GameplaySystems/commit/16a8a9c80c88720d81a8ce84de6a9478d2b71a2b)


## Day 2 – Unreal Engine C++ Project Structure & Gameplay Framework

### Objective
Understand Unreal Engine’s C++ project structure and the responsibility of core gameplay framework classes.

---

### Project Structure Observations

- The `Source/` folder contains the main game module.
- `.Build.cs` defines module dependencies and build rules.
- `Target.cs` separates Editor and Game build configurations.
- The main module files (`UE5_GameplaySystems.cpp/.h`) handle module startup and shutdown.

This helped me understand how Unreal loads and manages C++ code at runtime.

---

### Gameplay Framework Understanding

I studied the responsibility of Unreal Engine gameplay classes:

- **GameMode**  
  Controls game rules and default classes. Exists only on the server.

- **Character**  
  Represents a controllable pawn with movement and collision.
  Handles physical representation in the world.

- **PlayerController**  
  Handles player input and high-level control logic.
  It is NOT the player character itself.

- **Actor**  
  Base class for all gameplay objects.
  Should be used only when world presence is required.

- **Component**  
  Used to add reusable behavior to Actors.
  Preferred over Actors when possible for performance and design clarity.

---

### Actor Lifecycle Review

I reviewed the Actor lifecycle execution order:

* Constructor  
	* Runs when the Actor is created
	* Used to set **default values** and create components
	* Runs **even in the editor**
* OnConstruction  
    * Called after spawning or when a property changes in the editor
	* Great for **editor-time logic**
	* Runs multiple times
* BeginPlay  
	* Called once when the game starts or actor is spawned at runtime
	* Best place for **gameplay logic initialization**
* Tick  
	* Runs every frame (if enabled)
	* Used for continuous updates
* EndPlay  
	* Called when the actor is removed from the level or game ends
	* Used for **cleanup**
* Destroyed  

This clarified when initialization, gameplay logic, and cleanup should occur.

---

### Key Learning

Not all gameplay logic belongs in Actors.
Understanding ownership and responsibility is critical for scalable gameplay systems.

---

## Day 3 – Actor vs Component Design

Focus: Understanding when to use Actors versus Actor Components in Unreal Engine.

What I did:
- Created a reusable C++ Actor Component (DebugInfoComponent)
- Attached the component to an existing DebugActor
- Used the component to access and log its owning Actor
- Observed execution order between Actor and Component lifecycle

Key Learnings:
- Actors represent entities in the world
- Components represent reusable behavior
- Components help reduce duplication and improve scalability
- Not all logic should live inside Actors

This approach will be used for future systems such as health, interaction, and abilities.

---

## Day 4 – Enhanced Input (UE5)

Focus:
Learning and implementing Unreal Engine 5’s Enhanced Input system using C++.

What I did:
- Created Input Actions and Mapping Context
- Added Enhanced Input Mapping Context at runtime
- Bound input actions using UEnhancedInputComponent
- Implemented movement and camera input using FInputActionValue

Key Learnings:
- Input belongs in Character / PlayerController, not Actors
- Enhanced Input is context-based and scalable
- Clean input architecture is critical for future multiplayer support

| Code                                               | Purpose                                                                |
| -------------------------------------------------- | ---------------------------------------------------------------------- |
| `Super::BeginPlay()`                               | Calls the parent class’s BeginPlay to ensure proper initialization     |
| `Cast<APlayerController>(Controller)`              | Ensures the character has a PlayerController controlling it            |
| `GetSubsystem<UEnhancedInputLocalPlayerSubsystem>` | Accesses the Enhanced Input system for this player                     |
| `AddMappingContext(DefaultMappingContext, 0)`      | Activates this character’s input mapping so Jump, Move, Look will work |

---

## Day 5 – Gameplay Framework Deep Dive

### Objective
Understand Unreal Engine’s Gameplay Framework by analyzing
and documenting existing template code without adding new features.

#### GameMode
- GameMode defines game rules and default gameplay classes.
- Exists only on the server.
- Not responsible for player input or per-frame gameplay logic.

#### Character
- Character is a specialized Pawn with built-in movement logic.
- Movement is handled by CharacterMovementComponent.
- Camera behavior is managed using SpringArm + Camera components.

### Why This Matters
Understanding framework responsibilities prevents bad architecture,
such as placing input logic in GameMode or game rules in Actors.

### Next Step
Use this understanding to safely extend gameplay systems
starting Day 6 with input-driven behavior.

---
## Day 6 – Input Handling & Gameplay Flow

Focus:
Understanding how player input flows through Unreal Engine
from input mapping to C++ execution.

What I worked on:
- Reviewed PlayerController vs Character input responsibilities
- Created a custom Enhanced Input Action for testing
- Bound the input action in the Character C++ class
- Verified runtime execution using UE_LOG and on-screen debug messages

Key Learnings:
- Input is owned by the PlayerController but executed by the possessed Pawn
- Input actions should trigger intent, not direct gameplay complexity
- Clean input flow is critical for scalable systems (abilities, interaction, UI)

Outcome:
We now clearly understand where and how gameplay input should be handled
before implementing real systems like interactions or abilities.

**Debug Input**
![Showing Debugging](GamePlayScreenshots/DebugInput.png)

---
## Day 7 – Moving Platform (Actor System)
### What I built
- Created a reusable Moving Platform Actor in C++
- Platform moves between two locations using Tick and DeltaTime
- Exposed movement values to the editor using UPROPERTY

### Key Concepts Learned
- Tick-based movement and frame-rate independence
- FVector position math
- Actor lifecycle in a real gameplay use-case
- How to design simple but reusable gameplay systems

### Why this matters
Moving platforms are a core level-design mechanic.
This system can be reused, extended, or networked later.

### Notes
- Logic kept in C++, values exposed to designers
- Avoided hardcoding positions