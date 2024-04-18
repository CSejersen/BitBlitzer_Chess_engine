stockfishFile = open("stockfish.txt", "r")
perftFile = open("perft.txt", "r")

stockfish = stockfishFile.readlines()
perft = perftFile.readlines()

newFish = []
for line in stockfish:
    newFish.append(line.replace("\n", ""))

newPerft = []
for line in perft:
    newPerft.append(line.replace("\n", ""))

newFish = [x for x in newFish if x != ' ']
newPerft = [x for x in newPerft if x != ' ']

newFish.sort()
newPerft.sort()

finalFish = newFish[1:]
finalPerft = newPerft[1:]

while len(finalFish) > len(finalPerft):
    finalPerft.append("not found.. 0")

while len(finalPerft) > len(finalFish):
    finalFish.append("only found by you 0")


for i in range(0,len(finalFish)):
    if finalFish[i] == finalPerft[i]:
        print(f'{finalFish[i]} Confirmed')
    else:
        fish = finalFish[i].split()
        fishValue = int(fish[-1])
        perft = finalPerft[i].split()
        perftValue = int(perft[-1])

        print(f'{finalFish[i]} you got: {perftValue} off by: {perftValue - fishValue}')

