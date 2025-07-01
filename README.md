# Xenogears Decompilation
[![Build Status]][actions] [![Progress]][progress site]

[Build Status]: https://github.com/ladysilverberg/xenogears-decomp/actions/workflows/build.yaml/badge.svg
[actions]: https://github.com/ladysilverberg/xenogears-decomp/actions/workflows/build.yaml
[Progress]: https://decomp.dev/ladysilverberg/xenogears-decomp.svg?mode=shield&measure=code&category=all&label=Progress
[progress site]: https://decomp.dev/ladysilverberg/xenogears-decomp

This project is an in-progress matching decompilation of Xenogears for Playstation 1. The project currently targets the US release (SLUS 006.64), with the intention to target other releases as well down the road.

<img src="https://i.imgur.com/FfAa7QA.png" />

## Thanks
This project leans on the work of a lot of work done by others before.
-  [splat](https://github.com/ethteck/splat) - For splitting the binaries
-  [spimdiasm](https://github.com/Decompollaborate/spimdisasm) - For disassembling the binaries
-  [The silent hill decompilation](https://github.com/Vatuu/silent-hill-decomp/tree/master) - As a project structure template this project could be adapted from.
-  [MASPSX](https://github.com/mkst/maspsx) - For making parts of building less painful
-  [objdiff](https://github.com/encounter/objdiff) - For diffing
-  [decomp.dev](https://decomp.dev/) - For progress tracking
-  [decomp.me](https://decomp.me/) - For decompiling in collaboration

## Progress
Progress is tracked and visualized on [decomp.dev](https://decomp.dev/ladysilverberg/xenogears-decomp).

## Building
To build the project according to these instructions, you will need Linux. If you are on Windows, you can install and use WSL2. Specifically, the instructions assumes you are using a debian-based distro such as Ubuntu.

### Install system packages
Run the following commands on the terminal to install the packages required for building the project.
```
sudo apt update
sudo apt install git make binutils-mips-linux-gnu cpp-mips-linux-gnu python3 python3-pip python3-venv
```

### Clone the repository
Next, you need to download the project to your system. Do this by cloning the repository with the following command:
```
git clone --recursive https://github.com/ladysilverberg/xenogears-decomp.git
```

### Install Python packages
In addition to the system packages we installed, we need to install the python packages required for building as well. We will do this using a virtual environment.
```
cd xenogears-decomp
python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install -r requirements.txt
```

Note that each time you open a new terminal, you will need to active your virtual environment. You can do this by navigation to the directory of the project and running `source .venv/bin/activate` again.

### Set up the ROM
You need to bring your own ROM. The version currently targeted by the project is SLUS 006.64, with the SHA1 hash `560bbdbeb9264c935294ecad5a3d4ab230a006a9`. If you dump the CD, you should have a .BIN and .CUE file. To get the binary, you can mount the CD image and copy it to the `disc` folder in the project. If you are on Windows and are unsure of how to mount, you can download and use WinCDEmu and follow their tutorial to mount. On your Linux terminal, please run `sha1sum disc/SLUS_006.64` and verify that the hash is the same as the one above.

### Build and match
Run `make clean-check` to build and check that the built binary matches the original one. If everything goes as expected, you should see `build/out/SLUS_006.64: OK` when building is done.

Additional Make commands:
* `build`: Builds the executable and overlays.
* `check`: Builds the executable and overlays. After compilation, it compares checksums with the original files.
* `clean-build`: Clears the project configuration without deleting files.
* `clean-check`: Clears the project configuration without deleting files. After compilation, it compares checksums with the original files.
* `objdiff-config`: Generates project configuration for [Objdiff.](https://github.com/encounter/objdiff)


## Contributing
After building the project you are very welcome to start decompiling and contributing!

For a lot more information on how you can help and get started, please have a look at the [wiki](https://github.com/ladysilverberg/xenogears-decomp/wiki).
