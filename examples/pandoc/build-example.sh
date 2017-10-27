#!/bin/bash

pp -import Highlight.pp example.md > example-preprocessed.md
pandoc --self-contained -S --normalize example-preprocessed.md -o example.html
