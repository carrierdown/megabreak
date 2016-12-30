# MegaBreak

To mark its imminent 20 year anniversary, I'm doing a little revisit of my first big programming project. This repo collects all the source code for the game, as well as discussing some aspects of its creation.

I started this project way back in 1995, at the age of 15. My goal was to create a fast, "arcadey" type of game. Since this was my first attempt at creating a full game, I also figured it would be smart to keep things relatively simple. I eventually decided to create a Breakout/Arkanoid type of game, since I felt this wouldn't be too complicated to implement, while at the same time presenting plenty of technical challenges. In addition, the amount of graphical assets needed were low enough that I could do it all by myself. Since I was just starting out with PC programming at this point, taking on this project also meant creating lots of low level graphics and I/O functions, as well as some basic tools to help me realise my vision. I went with C++ (though I mostly used pure C) for the main implementation, which I had been learning from a couple of books for a year or so. I also had been learning some basic assembler programming and thus decided to do the low level graphics functions in assembly. The project took quite a while to finish, and was finally released in 1997 as free software. It gained some recognition after it's release, including a feature in scandinavian amateur computer magazine "Komputer for alle" where I was interviewed briefly about the game. It was subsequently distributed on their cover CD for the next couple of years or so.

![MegaBreak gameplay](https://github.com/carrierdown/megabreak/raw/master/megabreak-first-two-levels.gif "Megabreak gameplay: first two levels")

Note: This GIF does not accurately depict the actual frame rate of the game, which is a lot more fluid ;)

## Graphics

The graphics for the game were all created using Deluxe Paint II, which originally hails from the Amiga 500, and which I'd previously used extensively with my Atari ST. The PC version was not up to the same standards as the Atari and Amiga versions, but was nonetheless one of the few PC applications that made pixel based graphics creation easy. All art assets were saved as PCX-files, which I mainly chose because it was relatively easy to parse. However, I did not like the idea of having lots of PCX-files distributed alongside my game, so I wrote some tools to enable me to store the raw picture data from each PCX-file sequentially to a data file. Since all graphics were made with the same palette, this information was also removed from each image and only stored in the data file once.

![MegaBreak sprite sheets](https://github.com/carrierdown/megabreak/raw/master/megabreak-asset-sheet-x2.png "Megabreak sprite sheets")


