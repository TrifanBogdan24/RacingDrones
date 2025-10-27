# 🏁 SkySprint 🚁

> Fast paced 1v1 3D drone racing game built with `OpenGL` on top of
> [gfx-framework](https://github.com/UPB-Graphics/gfx-framework).


## Game Overview

Race side-by-side with your friend through 3D tracks full of obstacles.  
Fly through forests and colorful domes inspired by [Santorini's Blue Domes](https://imgs.search.brave.com/vEynbfF19sFE7PqAhwC7OtQIzei6gxHJDDgJyYD9rB8/rs:fit:860:0:0:0/g:ce/aHR0cHM6Ly93d3cu/c3Ryb2dpbGlzYW50/b3JpbmkuY29tL2Js/b2cvdXNlci9wYWdl/cy8wMS5ob21lLzIy/LjEwLWJlc3Qtc2Fu/dG9yaW5pLWJsdWUt/ZG9tZXMtY2h1cmNo/ZXMvc2FudG9yaW5p/LWJsdWUtZG9tZXMu/anBn).  
The winner is the first to reach all checkpoints (one gate at a time).

No two races are exactly the same!
Each time you open the game, a **new map** is waiting for new:
obstacles appear in **random locations** and come in **different sizes**,
to make the view more appealing.

This keeps every race fresh and unpredictable.

> **Pro tip:** For the best experience, play in **3rd person** with a **perspective minimap**.


## Demo Controls

### View
| Key | Action |
| :--- | :--- |
| `Right Ctrl + 1` | 1st person view |
| `Right Ctrl + 3` | 3rd person view |
| `Right Ctrl + O` | Orthogonal minimap |
| `Right Ctrl + P` | Perspective minimap |

### Movement
| Key | Action |
| :--- | :--- |
| `W` | Move Forward |
| `S` | Move Backward |
| `A` | Move Left |
| `D` | Move Right |
| `←` | Rotate Left |
| `→` | Rotate Right |
| `↑` | Climb Up |
| `↓` | Climb Down |

> **Start 1v1 game:** `TAB` ends demo and splits screen.


## Player 1 Controls (Left Side)

### View
| Key | Action |
| :--- | :--- |
| `Left Ctrl + 1` | 1st person view |
| `Left Ctrl + 3` | 3rd person view |
| `Left Ctrl + O` | Orthogonal minimap |
| `Left Ctrl + P` | Perspective minimap |

### Movement
| Key | Action |
| :--- | :--- |
| `W` | Move Forward |
| `S` | Move Backward |
| `A` | Move Left |
| `D` | Move Right |
| `←` | Rotate Left |
| `→` | Rotate Right |
| `↑` | Climb Up |
| `↓` | Climb Down |


## Player 2 Controls (Right Side)

### View
| Key | Action |
| :--- | :--- |
| `Right Ctrl + 8` | 1st person view |
| `Right Ctrl + 9` | 3rd person view |
| `Right Ctrl + O` | Orthogonal minimap |
| `Right Ctrl + P` | Perspective minimap |

### Movement (Numpad)
| Key | Action |
| :--- | :--- |
| `8` | Move Forward |
| `2` | Move Backward |
| `4` | Move Left |
| `6` | Move Right |
| `1` | Rotate Left |
| `3` | Rotate Right |
| `9` | Climb Up |
| `7` | Climb Down |


## Mini-Map Controls

Each player sees their drone in a personal viewport with navigation arrows.  
Mini-map control is shared between players due to limited keys.

| Key | Action |
| :--- | :--- |
| `I` | Move Up |
| `K` | Move Down |
| `J` | Move Left |
| `L` | Move Right |
| `U` | Shrink minimap (16:9 ratio) |
| `M` | Enlarge minimap (16:9 ratio) |


## Gameplay Mechanics

- **Checkpoint Colors:**  
  - **Red:** Unvisited gates  
  - **Green:** Visited gates  
  - **Blue:** Target gate (mini-map arrow points here)  
  - **Yellow:** Next gate in sequence  

- **Race Flow:** Winner waits for opponent to finish.  
- New random checkpoints are generated after each race, starting where players left off.


## Game Info

Elapsed time and winner are not shown in-game.

```cpp
#ifdef _WIN32
    // Hide DEBUG console, if it exists
    HWND hWnd = GetConsoleWindow();
    if (hWnd)
        ShowWindow(hWnd, SW_HIDE);
#endif
```


For debugging, uncomment the following lines in  
`src/main.cpp -> main()` to see console logs of checkpoint times:


Example debug output:

```
[DEMO] Check point reached after 16.97 seconds!
[DEMO] Check point reached after 5.22 seconds!
[DEMO] Check point reached after 16.73 seconds!

[FIRST PLAYER] Check point reached after 3.00 seconds!
[SECOND PLAYER] Check point reached after 4.16 seconds!
[SECOND PLAYER] Check point reached after 2.95 seconds!
[FIRST PLAYER] Check point reached after 7.06 seconds!
[FIRST PLAYER] Check point reached after 9.13 seconds!
...
First player completed race in 260.86 seconds!

[SECOND PLAYER] Check point reached after 6.15 seconds!
[SECOND PLAYER] Check point reached after 6.83 seconds!
...
[SECOND PLAYER] Check point reached after 15.92 seconds!
Second player completed race in 309.34 seconds!


Player 1 WON! Congrats!

```
