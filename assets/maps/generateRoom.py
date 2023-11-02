import sys
import json

if (len(sys.argv) < 5):
    print("Usage: python generateTilemap.py <inFilename> <outFilename>")

jsonFile = {}
objects = []

colliders = set([""])

with open(sys.argv[1], "r") as f:
    srcTileSize = int(f.readline())
    dstTileSize = int(f.readline())

    





