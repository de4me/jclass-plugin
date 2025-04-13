# JClass plugin for Total Commander

[![Downloads](https://img.shields.io/github/downloads/de4me/jclass-plugin/total.svg)](https://github.com/de4me/jclass-plugin/releases)

This plugin allows you to view the contents of java class files.

## Requirements

1. [Windows 7](https://en.wikipedia.org/wiki/Windows_7) or later;
2. Architectures [x86_64](https://en.wikipedia.org/wiki/X86-64) or [x86](https://en.wikipedia.org/wiki/X86);
3. [Total Commander](https://www.ghisler.com);
4. [MS Visual Studio 2019](https://visualstudio.microsoft.com/vs/older-downloads/) or later to build from source;
5. [7zip](https://www.7-zip.org/) to create plugin archive;


## How to BUILD

1. Open **jclass.sln** in MS Visual Studio;
2. Choose **Build** -> **Batch Build** and place a check mark next to **jclass Release Win32** and **jclass Release x64**;
3. Click on **Build** and wait until the project compilation is finished;
4. Run ```jclass.bat [VERSION]``` to create plugin archive **jclass-[VERSION].zip**;
5. Ready to install;

## How to INSTALL

If you using Total Commander 6.5 or later:

1. Run [Total Commander](https://www.ghisler.com);
2. Navigate to **jclass-[VERSION].zip** archive;
3. Press **ENTER** to initiate auto-installation for the plugin archive;

Or for manual installation:

1. Extract **jclass.wlx64** and **jclass.wlx** files from archive **jclass-[VERSION].zip**;
2. Run [Total Commander](https://www.ghisler.com);
3. Choose **Options** -> **Configure** -> **LS-Plugins**;
4. Click on **Add**;
5. Choose **jclass.wlx64** (x86_64) or **jclass.wlx** (x86) file and click **OK**;

## How to USE

* Run [Total Commander](https://www.ghisler.com);
* Navigate to file with extension **.class**;
* Press **F3**;

<p align="center">
<img width="554" src="https://github.com/de4me/jclass-plugin/blob/main/jclass.png">
</p>
