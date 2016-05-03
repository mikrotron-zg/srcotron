import pygame
from pygame.locals import *
import os
import serial

# prepisivanje vrijednosti amplitude
def agrid(canvas, ascale):
	for i in range(-5,5):
		label=sfnt.render(str(round(40*i/ascale,2)),1,fpen,bg)
		canvas.blit(label,(5,195-40*i))

pygame.font.init()

# inicijalizacija
h,w=400,800										#dimenzije povrsine za crtanje
ascale=1										#faktor amplitude
fs=250											#frekvencija uzoraka
fnt=pygame.font.SysFont("arial",15)				#font za detekciju RR
sfnt=pygame.font.SysFont("arial",11)			#font za osi
bg=pygame.Color(255,255,255)					#boja pozadine
gpen=pygame.Color(150,150,150)					#boja linije resetke
spen=pygame.Color(0,0,255)						#boja signala
wpen=pygame.Color(255,0,0)						#pulse marker
fpen=pygame.Color(0,0,0)						#tekst

# resetka
grid=pygame.Surface((w,h))
grid.fill(bg,((0,0),(w,h)))

# vrijeme
taxis=pygame.Surface((w,20))
taxis.fill(bg,((0,0),(w,20)))

# amplituda
aaxis=pygame.Surface((50,h))
aaxis.fill(bg,((0,0),(50,h)))
agrid(aaxis,ascale)

# crtanje resetke
hor=0
hinc=0.2*fs
ctr=0

for i in range(-5,5):
	if not(i):														#zastava za 0
		sflag=1
	else:
		sflag=0
	pygame.draw.line(grid,gpen,(0,200-40*i),(w,200-40*i),1+sflag)	#horizontalne

while hor<w:
	if not(ctr%5):													#zastava sekunde svakih 5 inkremenata(hinc) po 0.2s
		sflag=1
	else:
		sflag=0
	pygame.draw.line(grid,gpen,(hor,0),(hor,h),1+sflag)				#vertikalne
	num=sfnt.render(str(round(hor/fs,2))+"s",1,fpen,bg)				#os vremena
	taxis.blit(num,(hor,0))
	hor+=hinc
	ctr+=1

# pokretanje prozora
pygame.init()
screen=pygame.display.set_mode((w+50,h+20))						#povrsina prozora
pygame.display.set_caption("Srcotron EKG")
screen.fill(bg,((0,0),(w+50,h+20)))
screen.blit(taxis,(50,0))
screen.blit(aaxis,(0,20))
canvas=pygame.Surface((w,h))									#crtaca povrsina
canvas.fill(bg,((0,0),(w,h)))
canvas.blit(grid, (0,0))
pygame.display.flip()

# serial init
s=serial.Serial()
s.baudrate=57600
s.port="COM3"
s.open()

# podaci (trenutno se ne koriste u programu)
time=[]
signal=[]
pulse=[]

# log file
log = open("log.csv","w")
log.write("time, signal, pulse_flag\n")

# inicijalizacija za petlju iscrtavanja
running=True

ctr=0
reads=0
ppoint=(0,0)
lastR=0
while running:
	
	if pygame.event.peek(QUIT):																		#prekid petlje
		running = False
	for event in pygame.event.get():																#event handle - mora se prazniti queue inace lockup
		print event
	keys=pygame.key.get_pressed()																	#obrada pritisnutih tipki
	if keys[pygame.K_UP]!=0:
		ascale+=0.04
		aaxis.fill(bg,((0,0),(50,h)))
		agrid(aaxis,ascale)
		screen.blit(aaxis,(0,20))
	if keys[pygame.K_DOWN]!=0:
		ascale-=0.04
		aaxis.fill(bg,((0,0),(50,h)))
		agrid(aaxis,ascale)
		screen.blit(aaxis,(0,20))

	try:																							#sve vezano za serial comm je u try bloku radi gresaka u citanju
		if ctr==w:																					#kraj crtace plohe
			ctr=0
			canvas.blit(grid, (0,0))
			ppoint=(0,ppoint[1])
		a=s.readline()																				#serial read
		data=a.split()																				#python string split, snimanje korisnog u varijable
		time.append(int(data[0]))
		signal.append(float(data[1]))
		pulse.append(int(data[4]))
		loc=200-int(ascale*float(signal[reads]))													#lokacija za plotanje na crtacu povrsinu
		pygame.draw.line(canvas,spen,ppoint,(ctr,loc),1)
		ppoint=(ctr,loc)																			#spremam lokaciju za sljedeci korak radi crtanja linije
		if data[4]=='1':																			#pulse flag handling - oznacavam otkucaj srca
			pygame.draw.line(canvas,wpen,(ctr,0),(ctr,h),1)
			label=fnt.render("T="+str(round((reads-lastR)*1000/fs,2))+"ms",1,fpen,bg)
			canvas.blit(label,(ctr,h-20))
			lastR=reads
		screen.blit(canvas,(50,20))
		pygame.display.flip()																		#prikaz svih promjena na ekran
		
		ctr+=1
		reads+=1
		log.write(data[0]+", "+data[1]+", "+data[4]+"\n")											#zapis reda u log file
	except:
		print("step failed")
os.system("pause")
log.close()