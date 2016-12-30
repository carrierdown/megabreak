# MegaBreak

To mark its impending 20 year anniversary, I'm doing a little revisit of my first big programming project. This repo collects all the source code for the game, as well as discussing some aspects of its creation.

I started this project way back in 1995, at the age of 15. Having to first create a lot of low level graphics and I/O functions, this project took quite a while to finish, and was finally released in 1997 as free software. It gained some recognition after it's release, including a feature in scandinavian amateur computer magazine "Komputer for alle" where I was interviewed briefly about the game. It was subsequently distributed on their cover CD for the next couple of years or so.

![MegaBreak gameplay](https://github.com/carrierdown/megabreak/raw/master/megabreak-first-two-levels.gif "Megabreak gameplay: first two levels")

Note: This GIF does not accurately depict the actual frame rate of the game.

## Graphics

The graphics for the game was all created using Deluxe Paint II, which originally hails from the Amiga 500, and which I'd previously used extensively with my Atari ST. The PC version was not up to the same standards as the Atari and Amiga versions, but was nonetheless one of the few PC applications that made pixel based graphics creation easy. All art assets were saved as PCX-files, which I mainly chose because it was relatively easy to parse. However, I did not like the idea of having lots of PCX-files distributed alongside my game, so I wrote some tools to enable me to essentially create a data file containing stripped versions of the PCX data, where only the picture data was stored and all other header info as well as palette data was discarded.

![MegaBreak sprite sheets](https://github.com/carrierdown/megabreak/raw/master/megabreak-asset-sheet-x2.png "Megabreak sprite sheets")


