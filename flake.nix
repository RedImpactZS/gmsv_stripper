{
  description = "Flake to build gmsv_stripper";

  inputs = {
    nixpkgs-new.url = github:NixOS/nixpkgs/24.05;
    flake-utils.url = github:numtide/flake-utils;
  };

  outputs = { self, nixpkgs-new, flake-utils }: let

    nixpkgs = builtins.fetchGit {
      url = "https://github.com/NixOS/nixpkgs/";
      rev = "2158ec610d90359df7425e27298873a817b4c9dd";
      shallow = true;
    };


    lib = nixpkgs-new.lib;

    darwin = [ "x86_64-darwin" "i686-darwin" ];
    linux = [ "x86_64-linux" "i686-linux" ];
    allSystems = darwin ++ linux;

    forEachSystem = systems: f: lib.genAttrs systems (system: f system);
    forAllSystems = forEachSystem allSystems;
  in {
    packages = forAllSystems (system: let 
        pkgs = (import nixpkgs-new {inherit system;});
        premake5 = pkgs.premake5;
      in {
        crossPkgs = forEachSystem (lib.filter (sys: sys != system) allSystems) (targetSystem:
          let
            crossPkgs = (import nixpkgs) { localSystem = system; crossSystem = targetSystem; overlays = [ (self: super: { premake5 = premake5; }) ]; };
          in rec {
            gmsv_stripper = crossPkgs.callPackage ./default.nix {};
            default = gmsv_stripper;
          }
        );
      }
    );
  };
}
