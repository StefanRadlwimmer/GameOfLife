@echo off
SET inPath=in
SET resultPath=result
SET outPath=out
SET params=--mode ocl --device gpu
echo.--
GOL.exe --load %inPath%/random10000_in.gol --generations 250 --save %resultPath%/random10000_result.gol --measure %params%
echo.
echo.n|Comp "%resultPath%/random10000_result.gol" "%outPath%/random10000_out.gol" /L
echo.
pause