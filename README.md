# Raven Lune

BMS Player powered by SDL.

# Build

## Dependencies

- SDL2
- SDL2_image
- SDL2_mixer
- SDL2_ttf

## Commands

Unix, *nix:
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

## Start Screen

`Space`: Start

## List Screen

`Up`: Up

`Down`: Down

`Enter`: Play

`Space`: Auto Play

## Play Screen

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

# Todo

- [ ] Key settings
- [ ] Play video
- [ ] Improve design
