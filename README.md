# Raven Lune

BMS Player powered by SDL.

# Build

## Dependencies

- SDL2
- SDL2_image
- SDL2_mixer
- SDL2_ttf

## Commands

Unix, \*nix:

```sh
git clone https://github.com/Rokr0k/raven-lune.git
cd raven-lune
git submodule init
git submodule update
mkdir build
cd build
cmake ..
make
```

# How to Play

1. Copy `res` directory to the same location with the executable.
2. Edit `res/list.txt`, write directories where bms files are stored at each line.
   - `~` or other shell options are not available.
   - Relative path may be working, but relative to the executable. (not tested)
3. Run executable.

# Controls

## Default Key Map

<table>
   <tr>
      <td colspan="8">Player 1</td>
      <td rowspan="8"></td>
      <td colspan="8">Player 2</td>
   </tr>
   <tr>
      <td style="background: red;"></td>
      <td style="background: white;"></td>
      <td style="background: cyan;"></td>
      <td style="background: white;"></td>
      <td style="background: cyan;"></td>
      <td style="background: white;"></td>
      <td style="background: cyan;"></td>
      <td style="background: white;"></td>
      <td style="background: white;"></td>
      <td style="background: cyan;"></td>
      <td style="background: white;"></td>
      <td style="background: cyan;"></td>
      <td style="background: white;"></td>
      <td style="background: cyan;"></td>
      <td style="background: white;"></td>
      <td style="background: red;"></td>
   </tr>
   <tr>
      <td>LShift, LCtrl</td>
      <td>Z</td>
      <td>S</td>
      <td>X</td>
      <td>D</td>
      <td>C</td>
      <td>F</td>
      <td>V</td>
      <td>M</td>
      <td>K</td>
      <td>,</td>
      <td>L</td>
      <td>.</td>
      <td>;</td>
      <td>/</td>
      <td>RShift, RCtrl</td>
   </tr>
</table>

## Start Screen

`Space`: Start

## List Screen

<table>
   <tr>
      <td colspan="8">Player 1</td>
   </tr>
   <tr>
      <td style="background: red;"></td>
      <td style="background: white;"></td>
      <td style="background: cyan;"></td>
      <td style="background: white;"></td>
      <td style="background: cyan;"></td>
      <td style="background: white;"></td>
      <td style="background: cyan;"></td>
      <td style="background: white;"></td>
   </tr>
   <tr>
      <td></td>
      <td>List down</td>
      <td>List up</td>
      <td>Play</td>
      <td></td>
      <td>Auto play</td>
      <td>Key Settings</td>
      <td>Back to title</td>
   </tr>
</table>

## Key Settings Screen

`Up`, `Down`: Go around keys

`Enter`: Set key (press another key to set)

`Backspace`: Toggle between SP and DP

`Escape`: Back to list

# Todo

- Play video
- Improve design
