endgame=$false
# p2=Elephant
p1=linjiach
# p1=JasonBorne
mp2="make run"
mp1="make run"
# p2m="python ab.py 1"
p2=JB3
rm -f ./Final.txt
rm -rf playset1 playset2
rm -f ../$p1/input.txt
rm -f ../$p2/input.txt
mkdir playset1
mkdir playset2
outputnum=0
python ./generator.py 10 200 20
cp input.txt init.txt
# cp init.txt input.txt
cd ../$p1
make newgame
cd ../$p2
make agent
cd ../platform
while [[ !$endgame ]]; do
    echo -------------------
    cp input.txt ../$p1
    cd ../$p1
    elapse=$(time ($mp1 >log.txt) 2>&1)
    # echo here
    elapse_s=$(echo $elapse | awk ' { print $4$6 }')
    # echo $elapse_s
    cd ../platform
    cp ../$p1/output.txt ./output.txt
    python game_c.py $elapse_s
    if [[ -e "./Final.txt" ]]; then
        echo 'End of the match'
        endgame=$true
        break
    fi
    mv ../$p1/output.txt playset1/$outputnum.txt
    
    cp ./input.txt ../$p2
    cd ../$p2
    elapse2=$(time ($mp2 >log.txt) 2>&1)
    elapse_s2=$(echo $elapse2 | awk ' { print $4$6 }')
    # echo $elapse2
    echo $elapse_s2
    cd ../platform
    cp ../$p2/output.txt ./output.txt
    python game_c.py $elapse_s2
    if [[ -e "./Final.txt" ]]; then
        echo 'End of the match'
        endgame=$true
        break
    fi
    let outputnum=outputnum+1
    mv ../$p2/output.txt playset2/$outputnum.txt
    # rm ./Elephant/output.txt
done
# cat ./playset/*.txt >> all.txt
echo "Game Finished!!"
echo $elapse