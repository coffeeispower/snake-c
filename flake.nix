{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system;  };
      in with pkgs; {
        devShells = {
          ghactions = mkShell {
            buildInputs = [
              pkgsCross.mingwW64.buildPackages.gcc
              gcc
              gnumake
            ];
          };
          default = mkShell {
            buildInputs = [
              bear
              gcc
              clang-tools
              gnumake
              pkgsCross.mingwW64.buildPackages.gcc
            ];
          };
        };
      }
    );
}
