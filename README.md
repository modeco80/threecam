# 3cam

A free camera mod for SSX 3.

# Controls

| Keybind         | Action                         |
|-----------------|--------------------------------|
| **L3 + R2**     | Toggle the free camera         | 
| **Left Stick**  | Aim camera                     |
| **Right Stick** | Move camera                    |
| **L2**          | Slow down movement and aiming  |
| **R2**          | Speed up movement and aiming   |

## Extended Controls

These controls are provided by the 3Cam mod and control its QoL features.

| Keybind         | Action                             |
|-----------------|------------------------------------|
| **R3**          | Snap camera back to rider position | 
| **SELECT**      | Snap rider to camera position      |
| **Right Stick** | Move camera                        |
| **L1**          | Toggle 3Cam HUD                    |

# How does this mod work?

In the [July 17, 2003 prototype build](https://hiddenpalace.org/SSX_3_\(Jul_17,_2003_prototype\)) of SSX 3, a free camera was included
which is activated by pressing **L3 + R2**. This camera is a debug feature which was seemingly included in this build to help magazine editors create glamor shots. There are actually some features which are broken due to some debug-only code not being included like whatever "Statcam" is.

When the free camera is activated, the camera controller is set to one that calls itself "manual", which happens to be the freecam. 

This manual controller **still exists in the retail NTSC-U build**. Through some experimenting, preserved in this repository, I was able to switch the controller, and the rest is history. I've added some quality of life features that I think the camera needed, but other than that, most of this mod just re-enables code that was sitting in the game waiting for us to activate it.

# Building

## Dependencies
- Python (3.13 & 3.14 are known to work but I'm sure it will work on others.)
- Pipenv
- GNU Make

You'll additionally need to procure a copy of ee-gcc `2.96-ee-001003-1` somehow. 
Place it inside of `tools/cc/2.96-ee-001003-1`.

Run the following commands (install only needs to be run once):
```
pipenv install
pipenv shell
```

Running `make` inside of the pipenv shell will build the mod and the pnach for you. The pnach is in `src/freecam`.
