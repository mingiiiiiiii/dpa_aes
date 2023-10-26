테스트 방법

simulator :
gcc AES.c util.c main.c
./a.out

이후 생긴 simulatedPowerConsumption.txt 파일을 DPA 폴더에 옮겨주기
cp simulatedPowerConsumption.txt ../DPA/

DPA : 
gcc AES.c util.c main.c
./a.out

최종결과 : DPAresult.txt