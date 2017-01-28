run:
	rm -f output.txt
	./JasonBourne
	
agent:
	g++ -std=c++11 -Ofast JasonBourne3.cpp -o JasonBourne
	rm -f timer.txt
	# rm input.txt
	# rm output.txt

newgame:
	rm -f timer.txt
	rm -f input.txt
	rm -f output.txt
	rm -f log.txt
	
calibrate:
	g++ -std=c++11 -Ofast main1_acc.cpp -o Jason_acc
	cp cal1.txt.bk cal1.txt
	python calibrate.py cal1.txt
	# python calibrate.py cal2.txt rd
	

clean:
	rm -f Jason_acc
	rm -f JasonBourne
	rm -f *.txt
	rm -f *.o
