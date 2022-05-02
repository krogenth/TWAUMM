TWAUMM
======

TribalWars Auto-Updating Map Maker

This is the work of an old TribalWars fan, beginning when Nickjer first announced he would discontinue his server for the maps. The intention was to recreate his code, to be more efficient, better organized, and open to the public at all times.

Overall improvements include: an automated zoom, more maps, modular structure, and it's open source.

Supported maps
=====

Top Tribes, Top Players, Top ODA Tribes, Top ODD Tribes, Top ODA Players, Top ODD Players, Tribe Conquers, Tribe Losses, Player Conquers, and Player Losses

How to build
=====

Ensure you have [Visual Studio 2022](https://docs.microsoft.com/en-us/visualstudio/ide/whats-new-visual-studio-2022?view=vs-2022).

Clone this repo and and its linked libraries:

```
git clone https://github.com/krogenth/TWAUMM.git
cd TWAUMM
git submodule update --init --recursive
```

For Windows, change libgd to version 2.2 to avoid libheif:

```
cd libs/libgd
git checkout GD-2.2
```

Follow the listed instructions required for each library to be built for whatever platform you are running on.

Once all libraries have been built, you can create the project through VS2022. All builds can be found within the `./build/` directory.