@echo off
SET inPath=in
SET resultPath=result
SET outPath=out
SET params= --time --mode ocl --device gpu
GOL.exe --load %inPath%/random250_in.gol --generations 250 --save %resultPath%/random250_result.gol  %params%
GOL.exe --load %inPath%/random500_in.gol --generations 250 --save %resultPath%/random500_result.gol  %params%
GOL.exe --load %inPath%/random750_in.gol --generations 250 --save %resultPath%/random750_result.gol  %params%
GOL.exe --load %inPath%/random1000_in.gol --generations 250 --save %resultPath%/random1000_result.gol  %params%
GOL.exe --load %inPath%/random1250_in.gol --generations 250 --save %resultPath%/random1250_result.gol  %params%
GOL.exe --load %inPath%/random1500_in.gol --generations 250 --save %resultPath%/random1500_result.gol  %params%
GOL.exe --load %inPath%/random1750_in.gol --generations 250 --save %resultPath%/random1750_result.gol  %params%
GOL.exe --load %inPath%/random2000_in.gol --generations 250 --save %resultPath%/random2000_result.gol  %params%
GOL.exe --load %inPath%/random3000_in.gol --generations 250 --save %resultPath%/random3000_result.gol  %params%
GOL.exe --load %inPath%/random4000_in.gol --generations 250 --save %resultPath%/random4000_result.gol  %params%
GOL.exe --load %inPath%/random5000_in.gol --generations 250 --save %resultPath%/random5000_result.gol  %params%
GOL.exe --load %inPath%/random6000_in.gol --generations 250 --save %resultPath%/random6000_result.gol  %params%
GOL.exe --load %inPath%/random7000_in.gol --generations 250 --save %resultPath%/random7000_result.gol  %params%
GOL.exe --load %inPath%/random8000_in.gol --generations 250 --save %resultPath%/random8000_result.gol  %params%
GOL.exe --load %inPath%/random9000_in.gol --generations 250 --save %resultPath%/random9000_result.gol  %params%
GOL.exe --load %inPath%/random10000_in.gol --generations 250 --save %resultPath%/random10000_result.gol  %params%