# s(M)exoscope

This is Bram de Jong's classic **s(M)exoscope** plug-in with the source code cleaned up, documented, and modernized for JUCE 8.

![s(M)exoscope](Docs/oscilloscope_full.png)

The original smartelectronix site is defunct now but Bram was kind enough to upload [his plug-in source code](https://github.com/bdejong/smartelectronix/tree/master) to GitHub.

This version is based on [Armando Montanez's JUCE port](https://github.com/armandomontanez/smexoscope) from 2016 and [Jason Perez's fixes](https://github.com/jasonmarkperez/smexoscope-mac). However, I changed almost everything. :smile:

## How to use this plug-in

[Read the manual](Docs/Manual.md) (original by Kerrydan)

Note: The External trigger mode is not implemented in this version. The docs mention a "modular" version but only the standard version is available.

:warning: **Use this software at your own risk.** See the [LICENSE](LICENSE.txt) for full details. :warning:

## To-do list

- [ ] Make thread-safe. Currently the audio and UI threads are messing with each other's data.
- [ ] Improve drawing. Stuff seems to be off by a few pixels.
- [ ] Retina/HiDPI graphics. Resizable UI.
- [ ] Provide binaries so you don't have to compile this from scratch.

From the original `todo.txt`:

```text
[TODO]
* stereo (left/right/both on display, trigger=left) [Ross Bencina]
* VST programs [AFH]
* editable default settings (think bidule / save as default / registry) [AFH]
```

## Revision history

```text
revision history
----------------
20161202
* static Windows VST builds

20161106
* fixed line rendering gaps
* rebuilt for release compatibility and optimization
* included build for 64-bit Windows

20160902
* ported to JUCE framework
* refresh rate increased to 30fps
* Mouse crosshair black instead of inverted

20040403
* DC-Killer now defaults to OFF [Ross Bencina]
* changed the way the trigger works ("<" to "<=" etc...) [Ross Bencina]
* XY-cross with value readouts on left-click

20031203 :: first release
```

---

```text
[CHANGELOG]

v = bugfix
+ = addition
c = change

[17-04-2004] c   Added sean's graphic readout
[15-12-2003] c   DC-Killer now defaults to OFF [Ross Bencina]
[15-12-2003] v   changed the way the trigger works ("<" to "<=" etc...) [Ross Bencina]
[??-12-2003] +   XY-cross with value readouts

  -- version 1 released

[06-08-2003] +   optional DC-killing [Lee Stacey]
[05-08-2003] +   freeze [Ross Bencina]
[05-08-2003] +   if(trigger less than XX seconds ago) don't trigger again [Ross Belcina]
[05-08-2003] +   waveform zoom when samples/pixel < 1 [Ross Bencina]
[05-08-2003] +   channel switch [Lee Stacey / Ross Bencina]
[04-08-2003] v   stupid clipping bug [Lee Stacey]
```

## Credits

Copyright (C) 2003 Bram de Jong

```text
Bram @ Smartelectronix presents :: The S(m)exoscope
---------------------------------------------------


bram @ smartelectronix.com      :: Code & Conccept
sean @ deadskinboy.com          :: GUI Design
kerrydan                        :: Documentation
sophia @ smartelectronix.com    :: Mac port
armando                         :: Multiplatform JUCE port


Smexoscope is not an effect, nor is it a synthesiser, don't
expect it to generate noise: it's a visualisation only plugin!

Useful for looking at waveforms and very useful for developers.

For more info, see the splendid documentation by Kerrydan.

have fun,


 - bram, sean, kyle, armando
```

Cleaned up and documented by [Matthijs Hollemans](https://audiodev.blog) in December 2024.

JUCE is copyright © Raw Material Software.

VST® is a trademark of Steinberg Media Technologies GmbH, registered in Europe and other countries.
