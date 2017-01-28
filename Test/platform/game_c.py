import Queue
import sys
filepath = './input.txt'
timepath = './time.txt'

def Calc(lines,move_y, move_x, e_time_s , tlines, ai):
	N = int(lines[0])
	Mode = lines[1]
	Youplay = lines[2]
	Time = float(lines[3])
	Cell = lines[4:4+int(N)]
	State = lines[4+int(N):4+int(N)*2]
	Enemy = ''
	t1 = ''
	if Youplay == 'X': 
		Enemy = 'O'
		t1 = tlines[1]
	else:
		Enemy = 'X'
		t1 = tlines[0]
	print 'py printing: move'+ str(move_y)+' ' + str(move_x)
	print e_time_s
	e_time_f = (float(e_time_s.split('s')[0].split('m')[0])+float(e_time_s.split('s')[1].split('m')[0]))*60 \
	+float(e_time_s.split('s')[0].split('m')[1])+float(e_time_s.split('s')[1].split('m')[1])
	Time -= e_time_f
	if Youplay == 'X': 
		tlines[0] = str(Time)
	else:
		tlines[1] = str(Time) 
	# print e_time_f
	y = move_y
	x = move_x
	# print x 
	# print y
	statelist = []

	for i in range(N):
		statelist+=[[]]
		for j in range(N):
			statelist[i] += [State[i][j]]
		# ----------------
		if ai: 
			showCell = ''
			lineCell = Cell[i].split()
			for j in range(N):
				showCell += '%3s'%lineCell[j]
			print showCell
		# ----------------
	if x >= N or y >= N or x < 0 or y < 0: 
		print 'Wrong move! Invalid input.'
	elif statelist[y][x] != '.': 
		print 'Wrong move! It is already occupied.'
		f_out = open('./Final.txt', 'w')
		f_out.write('final')
		f_out.close()
	else:
		raid = False
		raid_avail = False
		up = ''
		down = ''
		if x>0:
			if statelist[y][x-1] == Youplay:
				raid_avail = True
		if x<N-1 and not raid:
			if statelist[y][x+1] == Youplay:
				raid_avail = True
		if y>0 and not raid:
			up = statelist[y-1][x]
			if up == Youplay:
				raid_avail = True
		if y<N-1 and not raid:
			down = statelist[y+1][x]
			if down == Youplay:
				raid_avail = True
		if raid_avail:
			if x>0:
				if statelist[y][x-1] == Enemy:
					statelist[y][x-1] = Youplay
					raid = True
			if x<N-1:
				if statelist[y][x+1] == Enemy:
					statelist[y][x+1] = Youplay
					raid = True
			if y>0:
				if statelist[y-1][x] == Enemy:
					statelist[y-1][x] = Youplay
					raid = True
			if y<N-1:
				if statelist[y+1][x] == Enemy:
					statelist[y+1][x] = Youplay
					raid = True
		statelist[y][x] = Youplay
		statestring = ''
		endgame = True
		onum = 0
		oscore = 0
		xnum = 0
		scale = ''
		for j in range(N):
			scale += '%2s'%j
		print scale
		for i in range(N):
			celllist = Cell[i].split()
			# statestring += chr(ord('A')+i)+' '
			for j in range(N):
				if statelist[i][j] == '.': 
					endgame = False
				if statelist[i][j] == 'O': 
					onum +=1
					oscore += int(celllist[j])
				if statelist[i][j] == 'X': 
					xnum +=1
					oscore -= int(celllist[j])
				statestring += statelist[i][j]
			statestring += '\n'
		ret_cell = ''
		for i in range(N-1):
			ret_cell += Cell[i] +'\n'
		ret_cell+= Cell[N-1]
		# ----------------
		if ai: print statestring
		# ----------------
		if endgame:	
			oscore = 0
			for i in range(N):
				celllist = Cell[i].split()
				for j in range(N):
					if statelist[i][j] == 'O': 
						oscore += int(celllist[j])
					else:
						oscore -= int(celllist[j])
			
			ret = ret = 'End\n' + str(N)+'\n'+Mode+'\n'+Enemy+'\n'+t1+'\n'+ret_cell+'\n'+statestring+ \
			'\nO scores: ' + str(oscore) + '\nX scores: ' + str(-oscore)
			print 'O scores: ' + str(oscore)
			print 'X scores: ' + str(-oscore)
			if oscore>0 : print 'O wins!'
			elif oscore<0 : print 'X wins!'
			else: print 'Tie!'
			print 'X have:' + str(xnum) + 'pieces'
			print 'O have:' + str(onum) + 'pieces'
			f_out = open('./Final.txt', 'w')
			f_out.write('final\n'+'O scores: ' + str(oscore)+ '\nX scores: ' + str(-oscore))
			f_out.close()
		else: 
			ret = str(N)+'\n'+Mode+'\n'+Enemy+'\n'+t1+'\n'+ret_cell+'\n'+statestring
			# ----------------
			if ai:
				print 'X have:' + str(xnum) + 'pieces, ' + str(-oscore)
				print 'O have:' + str(onum) + 'pieces, ' + str(oscore)
				print Enemy + '\'s move'
			# ----------------
		return ret
		
ret = ''


f = open(filepath,'r')
lines = f.read().splitlines()
f.close()
t2 = ''
t= open(timepath,'r')
tlines = t.read().splitlines()
t.close()
# print len(sys.argv)
if len(sys.argv) == 2:
	try:
		move = open('./output.txt','r')
		line = move.readline().split()
		y = int(line[0][1:])-1
		# print ord(line[0][0])
		# print ord('A')
		x = ord(line[0][0])-ord('A')
		print 'AI '+ line[1] + ' on ' + str(y+1) +' ' + str(x+1)
		ret= Calc(lines, y, x, sys.argv[1], tlines, True)	
		t2f = open(timepath,'w')
		t2lines = tlines[0]+'\n'+tlines[1]
		t2f.write(t2lines)
		t2f.close()
	except IOError:
		print 'Wrong input!'
	

if ret:
	if ret[0] == 'E': 	
		f_out = open('./Final.txt', 'w')
		f_out.write(ret)
		f_out.close()
	else:
		f_out = open(filepath, 'w')
		f_out.write(ret)
		f_out.close()
else:
	print "Wrong! Invalid output"

# if len(sys.argv)==1:
# 	f = open('input.txt','r')
# else:
# 	f = open(sys.argv[1], 'r')
