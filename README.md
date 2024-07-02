# gmsv_stripper
Stripper:Source for Garry's Mod

Based on original [stripper-source][1] and [gm_stripper_lib][2] to be standalone binary module for Garry's mod with extra LUA interface exposed for easier debugging.

Awfully integrates Stripper:Source into Garry's Mod with the help of [garrysmod_common][3] for extra definitions.

BUG: Due to load order of Garry's mod binary modules, stripper will kick in only on next map load.

## Compiling

The only supported compilation platform for this project on Windows is **Visual Studio 2017** on **release** mode. However, it's possible it'll work with *Visual Studio 2015* and *Visual Studio 2019* because of the unified runtime.

On Linux, everything should work fine as is, on **release** mode.

For macOS, any **Xcode (using the GCC compiler)** version *MIGHT* work as long as the **Mac OSX 10.7 SDK** is used, on **release** mode.

These restrictions are not random; they exist because of ABI compatibility reasons.

If stuff starts erroring or fails to work, be sure to check the correct line endings (\n and such) are present in the files for each OS.

## Requirements

This project requires [garrysmod_common][3], a framework to facilitate the creation of compilations files (Visual Studio, make, XCode, etc). Simply set the environment variable '**GARRYSMOD\_COMMON**' or the premake option '**gmcommon**' to the path of your local copy of [garrysmod_common][3].

We also use [SourceSDK2013][4]. The links to [SourceSDK2013][4] point to my [Danielga's](https://github.com/danielga) fork of VALVe's repo and for good reason: Garry's Mod has lots of backwards incompatible changes to interfaces and it's much smaller.

  [1]: https://github.com/alliedmodders/stripper-source
  [2]: https://github.com/Bagellll/gm_stripper_lib
  [3]: https://github.com/danielga/garrysmod_common
  [4]: https://github.com/danielga/sourcesdk-minimal

