# Raven Lune

BMS Player powered by SDL.

# Build

## Dependencies

- SDL2
- SDL2_image
- SDL2_mixer
- SDL2_ttf
- libVLC

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
./raven-lune
```

# How to Play

1. Copy `res` directory to the same location with the executable.
2. Edit `res/data/list.txt`, write directories where bms files are stored at each line.
   - Relative path works based on the executable location.
3. Run executable.

# Controls

## Default Key Map

<table>
   <tr>
      <td colspan="8">Player 1</td>
      <td rowspan="3"></td>
      <td colspan="8">Player 2</td>
   </tr>
   <tr>
      <td>LShift</td>
      <td rowspan="2">Z</td>
      <td rowspan="2">S</td>
      <td rowspan="2">X</td>
      <td rowspan="2">D</td>
      <td rowspan="2">C</td>
      <td rowspan="2">F</td>
      <td rowspan="2">V</td>
      <td rowspan="2">M</td>
      <td rowspan="2">K</td>
      <td rowspan="2">,</td>
      <td rowspan="2">L</td>
      <td rowspan="2">.</td>
      <td rowspan="2">;</td>
      <td rowspan="2">/</td>
      <td>RShift</td>
   </tr>
   <tr>
      <td>LCtrl</td>
      <td>RCtrl</td>
   </tr>
</table>

## Start Screen

`Space`: Start

`Escape`: Quit

## List Screen

`Left Arrow`: Next chart

`Right Arrow`: Previous chart

`Enter`: Start playing

`Shift` + `Enter`: Autoplay

`R`: Refresh

`K`: Key setting

`Escape`: Quit

## Result Screen

`Enter`: Go to list

## Key Settings Screen

`Up`, `Down`: Go around keys

`Enter`: Set key (press another key to set)

`Backspace`: Toggle between SP and DP

`Escape`: Save and Go to list

# Todo

- Improve design
