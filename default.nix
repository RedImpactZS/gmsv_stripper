{ lib, fetchFromGitHub, gcc9Stdenv, premake5, pkgsStatic }:

let
  garrysmod_common = fetchFromGitHub {
    owner = "danielga";
    repo = "garrysmod_common";
    rev = "cc6a291817f5a6ea037c553c0ff48d307015f685";
    sha256 = "sha256-A/2je2s/NuhrJS71c2nNmJISA9CcFOCHHpuM07IlRss=";
    fetchSubmodules = true;
  };

in gcc9Stdenv.mkDerivation rec {
  pname = "gmsv_stripper";
  version = "0.0.1";

  src = ./.;

  nativeBuildInputs = [ premake5 ];

  buildInputs = [ pkgsStatic.pcre ];

  GARRYSMOD_COMMON = "/build/thirdparty/garrysmod_common";

  patchPhase = ''
    cp -r $src/. /build/
    mkdir -p /build/thirdparty
    cp -r --no-preserve=mode,ownership ${garrysmod_common} /build/thirdparty/garrysmod_common
    cd /build/
  '';

  buildPhase = ''
    cd projects/*/*
    make
  '';

  installPhase = ''
    mkdir -p $out/garrysmod/lua/bin
    cp /build/projects/*/*/*/*/*.dll $out/garrysmod/lua/bin
  '';

  enableParallelBuilding = true;

  meta = with lib; {
    description = "Stripper:Source for Garry's Mod";
    homepage = "https://github.com/redImpactZS/gmsv_stripper";
    license = licenses.gpl2;
    platforms = platforms.unix;
    maintainers = with maintainers; [ deinferno ];
  };

}
