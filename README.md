TWAUMM
======

TribalWars Auto-Updating Map Maker

This is the work of an old TribalWars fan, beginning when Nickjer first announced he would discontinue his server for the maps. The intention was to recreate his code, to be more efficient, better organized, threaded, and open to the public at all times.

Overall improvements include an automated zoom, more maps, modular structure, and it's open source.

Currently, the maps it creates are: Top Tribes, Top Players, Top ODA Tribes, Top ODD Tribes, Top ODA Players, Top ODD Players, Tribe Conquers, Tribe Losses, Player Conquers, and Player Losses

Dependencies
=====
libgd: https://github.com/libgd/libgd

libcURL: https://curl.haxx.se/libcurl/

Instructions
=====

To properly set up, first you will want to get the dependencies listed above. These can be obtained through the individual projects themselves, or through this repository. Next, add or comment out worlds you wish to create maps for within "worlds.txt". To comment out a world, simply add a semicolon(';') before the world entry. If you are unsure how to add a specific world, consult the URL listed when viewing the "World Info" section of the TribalWars website(i.e. - viewing World 117 of the internation server will give "en117.tribalwars.net").

All maps created will be located within the "/maps/" folder, based on server and world.

To-Do
====
There are several features that still need to be added, specifically: include some multithreading, and the K Dominance Maps.
