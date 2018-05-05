BattleZone-DesertSabre
======================
CSCI 405 - Computer Graphics Assignment 3

A simple BattleZone clone.

You are the hero tank. Destroy your enemy.

Controls
========
Left Tread:  'a'-forward, 'z'-backward

Right Tread: '''-forward, '/'-backward

Fire:        Space bar\n"

Tanks move forward/backward at 3 units/s.

Individual tank treads move at 1 unit/s.

Tank turn radius is 100 units for a pivot.

Tank turn radius is 200 units for a turn.

Resulting angular velocity for pivots and turns (with some linear velocity in
the case of turns) result from w = v / r.

Features
========
- Full surface rendering of enemy and player tanks with some tasteful faux
  shading.
- Tank control inputs make use of flags within a control component, ensuring
  GLUT keyboard event handling is consistent.
- Tank movement/orientation make use of a simple physics engine which only
  handles linear velocity, angular velocity, and time.
- Rendering and animation of all shells (which begin from the appropriate 
  barrel position of the shooting tank).
- Full shell/tank collision detection (enemy tanks may accidently destroy each
  other as well).
- A basic radar/overhead map screen.
- Basic enemy AI: enemy detects player at 100 units distance, doggedly plots
  intercept vectors, and fires (deadly accurate) from 75 units. Given that all
  tanks move with a forward velocity of 3 units/s, this give a player between
  9 and 10 seconds to deal with a newly approaching enemy. When a player is not
  detected, enemy will patrol in a circular fashion. Enemies will respawn upon
  their destruction, usually about 110 units away from the player, in any
  direction.
- When a player strays to far from the map origin, his position will wrap to the
  other side of the map.
- A 5 second death screen, after which the player regains control, but is still
  in a tight spot.

Notes
=====
The deliverable for this assignment is the result of two experimental design
approaches. It began as a simple exploration of an entity component system. As
the assignment deadline approached, design shifted to a model-view-controller
framework to ensure on-time delivery.

I realize not every method should be inlined, which sadly is the case in this
deliverable. I have also kept individual class implementations within their
declarative header file (traditionally a no-no). All of this was done for the
sake of rapid prototyping and turn around times - I humbly sacrifice marks here
just to be done with this assignment and move on to other concerns.

I have also made use of freeglut.h, which provides the glutMainLoopEvent
function (fire the glutMainLoop once), to get a tighter control on the game
loop - chiefly for keeping the physics simulation and game rendering
in sync. See http://gafferongames.com/game-physics/fix-your-timestep/ . I
realize GLUT provides glutTimerFunc, but I wanted to try a more framework
agnostic approach.

Spoiler: Upon game start, that tank in front of you will kill you if you are not
         too careful.