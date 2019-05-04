# rattle

simple rtttl player i wrote while learning about sound wave generation

### building:

`brew install sdl2`
`make`

### usage:

`./rattle [--sine] [--square] [--sawtooth] [--triangle] <rtttl_string>`


```
e1z3r3p20% ./rattle Zelda64:d=4,o=6,b=200:2a#5,2f5,2p,8a#5,8c,8d,8d#,2f,2p,f,f,8f#,8g#,2a#,2p,a#,8a#,8p,8g#,8f#,g#,8f#,2f,2p,2f,d#,8d#,8f,2f#,2p,f,d#,c#,8c#,8d#,2f,2p,d#,c#,c,8c,8d,2e,2p,2g,1f
    name: Zelda64
defaults: duration = 4, octave = 6, beats = 200
    data: 2a#5,2f5,2p,8a#5,8c,8d,8d#,2f,2p,f,f,8f#,8g#,2a#,2p,a#,8a#,8p,8g#,8f#,g#,8f#,2f,2p,2f,d#,8d#,8f,2f#,2p,f,d#,c#,8c#,8d#,2f,2p,d#,c#,c,8c,8d,2e,2p,2g,1f
  note name: a#5,	frequency:  932.33 Hz,	duration 0.6000 s
  note name: f5,	frequency:  698.46 Hz,	duration 0.6000 s
  note name: pause,	frequency:    0.00 Hz,	duration 0.6000 s
  note name: a#5,	frequency:  932.33 Hz,	duration 0.1500 s
  note name: c6,	frequency: 1046.50 Hz,	duration 0.1500 s
  note name: d6,	frequency: 1174.66 Hz,	duration 0.1500 s
  note name: d#6,	frequency: 1244.51 Hz,	duration 0.1500 s
  note name: f6,	frequency: 1396.91 Hz,	duration 0.6000 s
  note name: pause,	frequency:    0.00 Hz,	duration 0.6000 s
  note name: f6,	frequency: 1396.91 Hz,	duration 0.3000 s
  note name: f6,	frequency: 1396.91 Hz,	duration 0.3000 s
  note name: f#6,	frequency: 1479.98 Hz,	duration 0.1500 s
  note name: g#6,	frequency: 1661.22 Hz,	duration 0.1500 s
  note name: a#6,	frequency: 1864.66 Hz,	duration 0.6000 s
  note name: pause,	frequency:    0.00 Hz,	duration 0.6000 s
  note name: a#6,	frequency: 1864.66 Hz,	duration 0.3000 s
  note name: a#6,	frequency: 1864.66 Hz,	duration 0.1500 s
  note name: pause,	frequency:    0.00 Hz,	duration 0.1500 s
  note name: g#6,	frequency: 1661.22 Hz,	duration 0.1500 s
  note name: f#6,	frequency: 1479.98 Hz,	duration 0.1500 s
  note name: g#6,	frequency: 1661.22 Hz,	duration 0.3000 s
  note name: f#6,	frequency: 1479.98 Hz,	duration 0.1500 s
  note name: f6,	frequency: 1396.91 Hz,	duration 0.6000 s
  note name: pause,	frequency:    0.00 Hz,	duration 0.6000 s
  note name: f6,	frequency: 1396.91 Hz,	duration 0.6000 s
  note name: d#6,	frequency: 1244.51 Hz,	duration 0.3000 s
  note name: d#6,	frequency: 1244.51 Hz,	duration 0.1500 s
  note name: f6,	frequency: 1396.91 Hz,	duration 0.1500 s
  note name: f#6,	frequency: 1479.98 Hz,	duration 0.6000 s
  note name: pause,	frequency:    0.00 Hz,	duration 0.6000 s
  note name: f6,	frequency: 1396.91 Hz,	duration 0.3000 s
  note name: d#6,	frequency: 1244.51 Hz,	duration 0.3000 s
  note name: c#6,	frequency: 1108.73 Hz,	duration 0.3000 s
  note name: c#6,	frequency: 1108.73 Hz,	duration 0.1500 s
  note name: d#6,	frequency: 1244.51 Hz,	duration 0.1500 s
  note name: f6,	frequency: 1396.91 Hz,	duration 0.6000 s
  note name: pause,	frequency:    0.00 Hz,	duration 0.6000 s
  note name: d#6,	frequency: 1244.51 Hz,	duration 0.3000 s
  note name: c#6,	frequency: 1108.73 Hz,	duration 0.3000 s
  note name: c6,	frequency: 1046.50 Hz,	duration 0.3000 s
  note name: c6,	frequency: 1046.50 Hz,	duration 0.1500 s
  note name: d6,	frequency: 1174.66 Hz,	duration 0.1500 s
  note name: e6,	frequency: 1318.51 Hz,	duration 0.6000 s
  note name: pause,	frequency:    0.00 Hz,	duration 0.6000 s
  note name: g6,	frequency: 1567.98 Hz,	duration 0.6000 s
  note name: f6,	frequency: 1396.91 Hz,	duration 1.2000 s
```
