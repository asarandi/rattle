# rattle

simple rtttl player i wrote while learning about sound wave generation

### building:

`brew install sdl2`

`make`

### usage:

`usage: rattle [--sine | --square | --sawtooth | --triangle] 'ring_tone_text_string'`

```
e1z3r3p20% ./rattle --square "NokiaTun:d=4,o=5,b=225:8e6,8d6,f#,g#,8c#6,8b,d,e,8b,8a,c#,e,2a";
    name: NokiaTun
defaults: duration = 4, octave = 5, beats = 225
    data: 8e6,8d6,f#,g#,8c#6,8b,d,e,8b,8a,c#,e,2a
  note name: e6,	frequency: 1318.51 Hz,	duration 0.1333 s
  note name: d6,	frequency: 1174.66 Hz,	duration 0.1333 s
  note name: f#5,	frequency:  739.99 Hz,	duration 0.2667 s
  note name: g#5,	frequency:  830.61 Hz,	duration 0.2667 s
  note name: c#6,	frequency: 1108.73 Hz,	duration 0.1333 s
  note name: b5,	frequency:  987.77 Hz,	duration 0.1333 s
  note name: d5,	frequency:  587.33 Hz,	duration 0.2667 s
  note name: e5,	frequency:  659.25 Hz,	duration 0.2667 s
  note name: b5,	frequency:  987.77 Hz,	duration 0.1333 s
  note name: a5,	frequency:  880.00 Hz,	duration 0.1333 s
  note name: c#5,	frequency:  554.37 Hz,	duration 0.2667 s
  note name: e5,	frequency:  659.25 Hz,	duration 0.2667 s
  note name: a5,	frequency:  880.00 Hz,	duration 0.5333 s
```

#

# keyb

simple keyboard synthesizer

### building:

`brew install sdl2`

`make keyb`

### usage:

`usage: keyb [--sine | --square | --sawtooth | --triangle]`

### key mappings:

```
 _______________________________________________________
|  | | | |  |  | | | | | |  |  | | | |  |  | | | | | |  |
|  | | | |  |  | | | | | |  |  | | | |  |  | | | | | |  |
|  | | | |  |  | | | | | |  |  | | | |  |  | | | | | |  |
|  |_| |_|  |  |_| |_| |_|  |  |_| |_|  |  |_| |_| |_|  |
|   |   |   |   |   |   |   |   |   |   |   |   |   |   |
|   |   |   |   |   |   |   |   |   |   |   |   |   |   |
|___|___|___|___|___|___|___|___|___|___|___|___|___|___|

    2   3       5   6   7       9   0       =   BS              <<-- black keys
  q   w   e   r   t   y   u   i   o   p   [   ]   \             <<-- white keys

 _______________________________________
|  | | | |  |  | | | | | |  |  | | | |  |
|  | | | |  |  | | | | | |  |  | | | |  |
|  | | | |  |  | | | | | |  |  | | | |  |
|  |_| |_|  |  |_| |_| |_|  |  |_| |_|  |
|   |   |   |   |   |   |   |   |   |   |
|   |   |   |   |   |   |   |   |   |   |
|___|___|___|___|___|___|___|___|___|___|

    s   d       g   h   j       l   ;                           <<-- black keys
  z   x   c   v   b   n   m   ,   .   /                         <<-- white keys
```
