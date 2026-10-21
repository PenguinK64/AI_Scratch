# Sheep Herding AI

A 2D sheep-herding game built from scratch in C using Raylib, focused on artificial intelligence, steering behaviours, and flocking mechanics.

Originally developed as a university project for *Games Programming: AI and Physics*, the project involved implementing AI behaviours and a finite state machine to create autonomous sheep that react to the player and their surroundings.


https://github.com/user-attachments/assets/9cd8c889-15ad-43df-bcb1-3c62bb01ab89

## About

Sheep Herding AI is a short 2D game where the player controls a sheepdog and must herd five sheep into a pen.

The project was primarily an exploration of implementing game AI from scratch, including autonomous movement, steering behaviours, flocking mechanics, and finite state machines.

Each sheep can wander independently, flee when the player approaches, and interact with nearby sheep using flocking behaviours.

## Features

- Autonomous sheep AI
- Finite state machine with Calm and Fleeing states
- Random wandering behaviour
- Flee steering behaviour in response to the player
- Separation, Alignment and Cohesion flocking behaviours
- Adjustable steering weights and detection radii
- AABB collision detection and resolution
- Sheep pen detection and herding counter
- Win condition for herding all five sheep
- Start, pause and win screens
- Game restart system
- 2D pixel-art sprites

## Controls

| Input | Action |
|---|---|
| W / A / S / D or Arrow Keys | Move sheepdog |
| Enter | Start / Continue / Restart |
| P | Pause |
| E | Return to menu (while paused) |

## Technical Highlights

The game was developed without a traditional game engine, with AI and gameplay systems implemented directly in C using Raylib.

The sheep use a finite state machine to switch between Calm and Fleeing behaviours depending on their distance from the player. While calm, sheep wander randomly and respond to nearby sheep through Separation, Alignment and Cohesion steering behaviours.

Each flocking behaviour calculates a steering direction based on neighbouring sheep. Separation prevents overcrowding, Alignment encourages movement in similar directions, and Cohesion encourages sheep to stay together. Detection radii and weights were adjusted to balance these behaviours.

I also implemented AABB collision detection for the fences and pen, with horizontal and vertical collision resolution handled separately to prevent sheep from getting stuck against obstacles.

A herding system tracks how many sheep are currently inside the pen, triggering the win condition once all five sheep have been successfully herded.

## Built With

- C
- Raylib
- Visual Studio

 
