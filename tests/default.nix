{ pkgs ? import <nixpkgs> {} }:

with pkgs;
let
  python-with-packages = python3.withPackages (p: with p; [
    pip
    termcolor
    pyyaml
  ]);
in
python-with-packages.env