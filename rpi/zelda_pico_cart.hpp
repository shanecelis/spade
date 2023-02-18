#ifndef ZELDA_PICO_CART_H_
#define ZELDA_PICO_CART_H_

#include <string>

static std::string zeldaPicoCart = R"~(pico-8 cartridge // http://www.pico-8.com
version 39
__lua__
--pico zelda
--by thesebonesalone
function savegame()
	dset(1,hassword)
	dset(2,hasroc)
	dset(3,haspow)
	dset(4,hasflips)
	dset(41,mapnum)
	for i=5,37 do
		dset(i,dungeonsave[i-4])
	end
	for i=1,3 do
		dset(i + 37,dungeonkeys[i])
	end
	dset(42,maxhp)
end
function newcut()
	entarr = {}
	newtext("this land is beset by beasts.")
	t = entarr[1]
	timer = 10
	triforce()
	ti = entarr[2]
	while count(entarr) > 1 do
		cls()
		t.update(t)
		t.draw(t)
		ti.draw(ti)
		flip()
		if count(entarr)<2 then
			timer -= 1
			if (timer == 9) newtext("you must find the magic ore\nand slay the evil...")
			if (timer == 8) newtext("now go forth brave one. and\nbring peace...")
			t = entarr[2]
		end
	end
	fancycls()
end



function newgame()
	dset(0,1)
	dset(1,0)
	dset(2,0)
	dset(3,0)
	dset(4,0)
	dset(41,0)
	for i=5,37 do
		dset(i,1)
	end
	for i=1,3 do
		dset(i + 37,0)
	end
	dset(42,3)
end

function loadgame()
	hassword = dget(1)
	hasroc = dget(2)
	haspow = dget(3)
	hasflips = dget(4)
	mapnum = dget(41)
	for i=5,37 do
			dungeonsave[i-4] = dget(i)
	end
	for i=1,3 do
		dungeonkeys[i] = dget(i + 37)
	end
	--dungeonsave[19] = 1
	maxhp = dget(42)
	if (mapnum == 0) then
		mapx = 0
		mapy = 24
		px = 32
		py = 32
		music(9)
	elseif mapnum == 1 then
		mapx = 72
		mapy = 24
		px = 32
		py = 48
		music(0)
	elseif mapnum == 2 then
		mapx = 112
		mapy = 24
		px = 32
		py = 48
		music(0)
	elseif mapnum == 3 then
		mapx = 40
		mapy = 56
		px = 32
		py = 48
		music(0)
	end
end
function startgame()
	if (dget(0) == 0) newgame()
	--newgame()
	sfx(16,3)
	cls()
	--mapdrawex(16,48)
	mapdrawex(16,48)
	print("the legend of",49,16,12)
	readvectorst("020,016,047,016,019,017,016,024,017,025,018,025,019,024,024,019,025,019,042,019,042,020,016,057,016,058,016,063,017,063,043,063,044,062,047,055,045,054,040,059,039,059,020,059,021,058,047,020,047,019,047,017,047,023,071,023,047,024,049,026,049,026,049,051,048,052,046,054,047,054,087,054,065,026,065,051,069,026,069,051,081,026,081,051,085,028,085,049,052,026,052,037,052,041,052,051,053,051,058,051,059,050,061,048,062,053,062,048,063,053,062,054,053,026,058,026,053,037,057,037,053,041,057,041,061,034,061,044,070,051,074,051,093,034,093,043,098,044,098,033,101,023,109,023,109,024,115,050,097,050,104,028,103,042,109,042,094,054,102,054,110,054,118,054,079,023,087,023,086,028,089,029,086,049,089,048,078,048,078,053,062,029,062,024,100,051,103,043,110,043,112,051,101,024,104,027,106,032,104,039,107,031,109,039,105,039,108,039,110,053,112,051,116,051,118,053,100,051,102,053,063,024,064,025,070,025,071,024,079,024,080,025,059,027,061,029,058,036,060,034,058,042,060,044,075,050,077,048,079,053,080,052,102,053,101,052,094,053,096,051,095,050,097,045,095,027,097,032,092,052,094,051,092,025,094,026,088,024,091,024,088,053,091,053,090,047,092,044,090,030,092,033,",8)
	waitframe()

	print("pico!",49,56,12)
	waitframe()
	print("2021 thesebonesalone\n❎ continue\n🅾️ new game",16,65,7)

	local timer = 4
	while (timer > 3) do
		if (btn(❎)) timer = 1
		if (btn(🅾️)) timer = 2
		flip()
	end
	fancycls()
	if (timer == 2) then
		newcut()
		newgame()
	end
	loadgame()
	bigm = 0
	mapeffect = 0
	tmapx = mapx
	tmapy = mapy
	lmapx = mapx
	lmapy = mapy
	--restart object
	start = {mapx=mapx,mapy=mapy,x=px,y=py}
	entarr = {}
	addplayer(px,py)
	entarr[1].hpmax = maxhp
	entarr[1].hp = maxhp
	anim = false
	anim_time = 0
	pause = false
	lastmap = {}
	last_add()
	dark = true
	loadin()
end

function parseint(ch)
	chkst = "0123456789"
	for i=1,11 do
		if (ch == sub(chkst,i,i)) return i-1
	end
end
function build3dig(st)
	local n1 = parseint(sub(st,1,1)) * 100
	local n2 = parseint(sub(st,2,2)) * 10
	local n3 = parseint(sub(st,3,3))
	return n1 + n2 + n3
end

function readvectorst(st,col)
	local pc = 1
	while (sub(st,0,pc) != st) do
		local vec = {}
		for i=0,3 do
			add(vec,build3dig(sub(st,pc+i*4,pc*3+i*4)))
		end
		for x=-0.5,0.5 do
			for y=-0.5,0.5 do
			 line(vec[1] + x,vec[2] + y,vec[3] + x,vec[4] + y,col)
				--flip()
			end
		end
		pc += 16
	end
end
function waitframe()
	for i=0,30 do
		flip()
	end
end

function _init()
	dungeonsave = {}
	dungeonkeys = {}
	cartdata("pico_zelda")
	startgame()
end

function _draw()
	cls()
	if mapnum != 0 then
		dark = true
	else
		dark = false
	end

	anim_time += 1
	if anim_time % 8 == 0 then
		anim = not anim
		anim_time = 0
	end
	mapdrawex(mapx,mapy)
	for obj in all(entarr) do
		if (obj != entarr[1])	obj.draw(obj)
	end
	entarr[1].draw(entarr[1])
	if dark then
		darken()
	else
		pal()
	end
	local ex = flr(mapx/16)
	local ey = flr(mapy/16)
end

function _update()
	--check for textboxes
	for obj in all(entarr) do
			if (obj.name == "text") pause = true
	end

	if (pause == false) then
		for obj in all(entarr) do
			obj.update(obj)
		end
	else
		for obj in all(entarr) do
			if (obj.name == "text") obj.update(obj)
		end
	end
	if tmapx != mapx or tmapy != mapy then
		pause = true
		local xdif = 0
		local ydif = 0
		if (tmapx > mapx) xdif = 0.5
		if (tmapx < mapx) xdif = -0.5
		if (tmapy > mapy) ydif = 0.5
		if (tmapy < mapy) ydif = -0.5
		dumploop()
		local obj = entarr[1]
		obj.x -= xdif * 8
		obj.y -= ydif * 8
		mapx += xdif
		mapy += ydif
		if (tmapx == mapx and tmapy == mapy)	loadin()
	else
		pause = false
	end
	if (entarr[1].hp <= 0) gameover()
	savegame()
end
function fancycls()
	music(-1)
	sfx(17)
	for y=0,3 do
		for x=0,16 do
			local ch = x*8 + y*2
			rectfill(0,ch,128, ch+1,0)
			flip()
		end
	end
end


function gameover()
	fancycls()
	last_put()
	music(15)
	local press = false
	local timer = 0
	pal()
	flip()
	while (btn(❎) == false) do
		print("game over",49,56,7)
		print("press ❎ to retry",32,64,7)
		flip()
	end
	entarr = {}
	startgame()
end

function wingame()
	fancycls()
	last_put()
	music(24)
	pal()
	flip()
	while (btn(❎) == false) do
		mapdrawex(64,0)
		print("the land is at peace!",22,56,7)
		print("press ❎ to play again!",18,64,7)
		flip()
	end
	fancycls()
	entarr = {}
	startgame()
end





function loadin()
	local p = entarr[1]
	p.safex = p.x
	p.safey = p.y
	last_put()
	last_add()
	enemyloc()
	loaddoorsnpcs()
end

function dumploop()
	local player = entarr[1]
	if (player.state == 3) player.state = 0
	entarr = {}
	add(entarr,player)
end

function last_add()
	lastmap = {}
	for i=0, 63 do
		ix = i%8
		iy = (i - ix)/8
		add(lastmap, mget(ix + mapx,iy + mapy))
	end
end
function last_put()
	for i=0, 63 do
		ix = i%8
		iy = (i - ix)/8
		val = lastmap[i + 1]
		mset(ix + lmapx,iy + lmapy,val)
	end
	lmapx = mapx
	lmapy = mapy
end
-->8
--map code

function mapdrawex(x,y)
	for i=0,7 do
		for j=0,7 do
		local sp = mget(i + x, j + y)
		if (sp > 145 and sp < 153) sp = mget(i + x + 1, j + y)


		local hflip = false
		if (fget(sp,7) and anim) hflip = true
		local vec = getsco(sp)
		sspr(vec.x,vec.y,8,8,i*16,j*16,16,16,hflip,false)
		end
	end
end

function colcheck(x,y,z,this)
	local colv = spcheck(x,y)
	local collide = false
	if (this.name == "enemy") return (fget(colv,0) or fget(colv,3))
	if (fget(colv,3) and z > 0) return false
	return fget(colv,0)
end

function spcheck(x,y)
	x = flr(x/8) + mapx
	y = flr(y/8) + mapy
	return mget(x,y)
end

function setat(x,y,val)
	x = flr(x/8) + mapx
	y = flr(y/8) + mapy
	mset(x,y,val)
end

function cutat(x,y)
	if x < 64 and x > 0 and y < 64 and y > 0 and fget(spcheck(x,y),1) then
		setat(x,y,3)
		newdebris(x,y,126)
		if (rnd(100)<20) heartpup(x,y)
	end
end
-->8
--extra functions
function ckforenemies()
	for obj in all(entarr) do
		if (obj.name == "enemy") return false
	end
	return true
end


function hurtpal(this)
 vec = getsco(169)
 sspr(vec.x,vec.y,8,8,this.x * 2,this.y * 2,16,16,this.hflip,this.vflip)
	pal()
end



function outline_sprite(n,col_outline,x,y,flip_x,flip_y)
  local vec = getsco(n)
  for c=1,15 do
    pal(c,col_outline)
  end
  for xx=-1,1 do
    for yy=-1,1 do
      sspr(vec.x,vec.y,8,8,(x + xx) * 2,(y + yy) * 2,16,16,flip_x,flip_y)
    end
  end
  pal()
  sspr(vec.x,vec.y,8,8,x * 2,y * 2,16,16,flip_x,flip_y)
end

function checkcolent(this, name)
	for obj in all(entarr) do
		if (obj.name == name and boxcol(getbbox(this),getbbox(obj))) return obj
	end
	return nil
end

function getsco(sp)
	local sx = sp%16*8
	local sy = flr(sp/16) * 8
	--vec.x = sx
	--vec.y = sy
	return {x=sx,y=sy}
end

function get_4way_spr(x,y,spos,direction, hsp, vsp)
	hflip = false
	if direction == 0 or direction == 2 then
	 if (anim and (hsp != 0 or vsp !=0)) spos += 1
	 if (direction == 2) hflip = true
	else
		if (anim and (hsp != 0 or vsp != 0)) hflip = true
		spos += 2
		if (direction == 3) spos += 1
	end
	outline_sprite(spos,0,x,y,hflip,false)
end

function getbbox(this)
	return {left=this.x + this.bl,right=this.x + this.br,bottom=this.y + this.bb,top=this.y + this.bt}
end

function applycol(this)
	--horizontal
	local bbox = getbbox(this)
	local side = bbox.left
	if (this.hsp > 0) side = bbox.right
	if (colcheck(side + this.hsp,bbox.bottom,this.z,this) or colcheck(side + this.hsp, bbox.top,this.z,this)) this.hsp = 0
	this.x += this.hsp
	--vertical
	bbox = getbbox(this)
	side = bbox.top
	if (this.vsp > 0) side = bbox.bottom
	if (colcheck(bbox.left, side + this.vsp,this.z,this) or colcheck(bbox.right, side + this.vsp,this.z,this)) this.vsp = 0
	this.y += this.vsp
end

function darken()
	for i=1, 15 do
		pal(i,i+128,1)
	end
end
-->8
--player code

function addplayer(x,y)
	player = {}
	player.x = x
	player.y = y
	player.safex = x
	player.safey = y
	player.hp = 3
	player.hpmax = 3
	player.coin = 0
	player.startx = x
	player.starty = y
	player.direction = 0
	player.hsp = 0
	player.vsp = 0
	player.bl = 1
	player.bt = 4
	player.br = 7
	player.bb = 7
	player.z = 0
	player.sprite = 112
	player.state = 0
	player.timer = 0
	player.jsp = 0
	player.gx = x
	player.gy = y
	player.interact = false
	player.name = "player"
	player.update = function(this)
		--handle fall safety
		if this.state != 4 then
			this.hsp = 0
			this.vsp = 0
		end
		if this.state == 0 then
			movestate(this)
		elseif this.state == 1 then
			attackstate(this)
		elseif this.state == 5 then
			swimstate(this)
		elseif this.state == 2 then
			jumpstate(this)
		elseif this.state == 3 then
			carrystate(this)
		elseif this.state == 4 then
			damagestate(this)
		elseif this.state == 6 then
			fallstate(this)
		end
	end

	player.draw = function(this)
		if (this.z != 0) circfill((this.x + 4) * 2, (this.y + 4) * 2, 4,0)
		if this.state != 6 then
			get_4way_spr(this.x, this.y - this.z, this.sprite, this.direction, this.hsp, this.vsp)
			if (this.state == 4 and this.timer % 2 == 0) hurtpal(this)
		else
			local off = 8 - this.timer
			sspr(16,55,8,8,this.x * 2 + off,this.y * 2 + off,16 - (off * 2),16-(off * 2),false,false)
		end

		if (this.interact) spr(153,(this.gx - this.gx%8) * 2,(this.gy - this.gy%8) * 2)
	--hud
	for i=1, this.hpmax do
		if this.hp >= i then
			spr(176,(i-1)*8,0)
		else
			spr(177,(i-1)*8,0)
		end
	end
	if mapnum != 0 then
		for x=-1,1 do
			for y = -1,1 do
				print("x"..dungeonkeys[mapnum] ,118 + x,0 + y,0)
			end
		end
		spr(161,110,0)
		print("x"..dungeonkeys[mapnum],118,0,7)
	end
	end


	add(entarr,player)
end

function check_bounds(this)
	if this.x == 60 then
		this.x += 4
		tmapx += 8
	end
	if this.x == -4 then
		this.x -= 4
		tmapx -= 8
	end
	if this.y == 60 then
		this.y += 4
		tmapy += 8
	end
	if this.y == -4 then
		this.y -= 4
		tmapy -= 8
	end

end

--states
--move is 0
function movestate(this)
	--handle fall safety
	local curs = spcheck(this.x+4,this.y+4)
	if fget(curs,3) then
		if (curs == 21 and hasflips == 1) == false then
			this.hp -= 1
			sfx(10,3)
			this.state = 6
		 this.timer = 8
		end
	end
	this.sprite = 112
	getinput(this)
	getuse(this)
	applycol(this)
	checkinteract(this)
	check_bounds(this)
	if fget(spcheck(this.x + 4,this.y + 4),2) and hasflips == 1 then
		this.state = 5
		newdebris(this.x,this.y,177)
	end
end
--attack is 1
function attackstate(this)
	this.sprite = 112
	applycol(this)
	checktimer(this)
	check_bounds(this)
end
--jump is 2
function jumpstate(this)
	this.z += this.jsp
	getinput(this)
	applycol(this)
	check_bounds(this)
	this.jsp -= 0.25
	if this.z <=0 then
		this.z = 0
		this.state = 0
		sfx(1,3)
	end
end

--carry is 3
function carrystate(this)
	this.sprite = 120
	if (btnp(🅾️)) this.state = 0
	getinput(this)
	applycol(this)
	check_bounds(this)
end

--damage is 4
function damagestate(this)
	if (this.timer == 0) this.state = 0
	this.timer -= 1
	applycol(this)
	check_bounds(this)
end

--swim is 5
function swimstate(this)
	this.sprite = 116
	getinput(this)
	applycol(this)
	check_bounds(this)
	if (fget(spcheck(this.x + 4,this.y + 4),2) == false) this.state = 0
end
--fall is 6
function fallstate(this)
	if this.timer == 0 then
		this.state = 0
		this.x = this.safex
		this.y = this.safey
	end
	this.timer -= 1
end


function getinput(this)
	if btn(➡️) then
		this.hsp = 1
		this.direction = 0
		this.gx = this.x + 9
		this.gy = this.y + 4
	end
	if btn(⬅️) then
		this.hsp = -1
		this.direction = 2
		this.gx = this.x - 1
		this.gy = this.y + 4
	end
	if btn(⬇️) then
		this.vsp = 1
		this.direction = 1
		this.gx = this.x + 4
		this.gy = this.y + 9
	end
	if btn(⬆️) then
		this.vsp = -1
		this.direction = 3
		this.gx = this.x + 4
		this.gy = this.y - 1
	end
end

function getuse(this)
	if btnp(❎) and hassword == 1 then
			this.timer = 6
			this.state = 1
			newsword(this.x,this.y,this.direction,this)
	end
	if this.interact then
		if (btnp(🅾️)) interaction(this)

	else
		if btnp(🅾️) and hasroc == 1 then
				sfx(0,3)
				this.jsp = 2
				this.state = 2
		end
	end
end

function interaction(this)
	local val = spcheck(this.gx,this.gy)
	if haspow == 1 and (val == 30 or val == 31) then
		setat(this.gx,this.gy,13)
		this.state = 3
		newheld(135 + val,this)
	end
	if val == 169 then
		for obj in all(entarr) do
			if obj.name == "npc" then
				newtext(obj.text)
				obj.action(obj)
			end
		end
	end
	if val == 101 then
		if dungeonkeys[mapnum] > 0 then
			setat(this.gx,this.gy,12)
			dungeonkeys[mapnum] -= 1
			dungeonsave[mapeffect] = 0
		else
			newtext("you need a small key")
		end
	end
	if val == 28 then
		local tx = this.x + 4
		local ty = this.y + 4
		if (this.direction == 0) tx = this.x + 17
		if (this.direction == 2) tx = this.x - 9
		if (this.direction == 1) ty = this.y + 17
		if (this.direction == 3) ty = this.y - 9
		local fl = spcheck(tx,ty)
		if (fget(fl,0) == false and fget(fl,2) == false and fget(fl,3) == false) then
			setat(this.gx,this.gy,15)
			newblock(flr(this.gx/8)*8,flr(this.gy/8)*8,this)
		end
	end



end


function checktimer(this)
	if (this.timer == 0) this.state = 0
	this.timer -= 1
end

function checkinteract(this)
	this.interact = false
	if (fget(spcheck(this.gx,this.gy),6)) this.interact = true
end


-->8
--sword code

function ckenemydmg(this,dam)
	local colen = checkcolent(this,"enemy")
		if (colen != nil and colen.state != 2) then
			colen.state = 2
			colen.timer = 10
			colen.hsp = 0
			colen.vsp = 0
			colen.hp -= dam
			local p = entarr[1]
			sfx(6,3)
			if (p.x > this.x) colen.hsp = -1
			if (p.x < this.x) colen.hsp = 1
			if (p.y > this.y) colen.vsp = -1
			if (p.y < this.y) colen.vsp = 1
		end
end




function newsword(x,y,direction,player)
	local sword = {}
	sword.x = x
	sword.y = y
	sword.direction = direction
	sword.player = player
	sword.sp = 0
	sword.hflip = 0
	sword.vflip = 0
	sword.xoff = 0
	sword.yoff = 0
	sword.bl = 1
	sword.bt = 1
	sword.br = 7
	sword.bb = 7
	sword.name = "sword"

	sword.update = function(this)
	ckenemydmg(this,1)
	local timer = 6 - this.player.timer
	timer -= timer % 2
	timer = timer/2
	if (this.player.timer == 0 or this.player.state != 1) then
		del(entarr,this)
		return false
	end
	local loc = this.direction * 2 + timer + 1
	if (loc >= 9) loc = 1
	local temp = {163,164,162,164,163,164,162,164}
	this.sp = temp[loc]
	temp = {false,false,false,true,true,true,false,false}
	this.vflip = temp[loc]
	temp = {false,false,false,false,false,true,true,true}
	this.hflip = temp[loc]
	temp = {0,1,1,1,0,-1,-1,-1}
	this.xoff = temp[loc] * 8
	temp = {-1,-1,0,1,1,1,0,-1}
	this.yoff = temp[loc] * 8
	this.x = this.player.x + this.xoff
	this.y = this.player.y + this.yoff
	local bbox = getbbox(this)
	cutat(bbox.left,bbox.top)
	cutat(bbox.right,bbox.top)
	cutat(bbox.left,bbox.bottom)
	cutat(bbox.right,bbox.bottom)
	end



	sword.draw = function(this)
		outline_sprite(this.sp,0,this.x,this.y,this.hflip,this.vflip)
	end
	sfx(4,3)
	add(entarr,sword)
end
-->8
--hold item

function newheld(sp,player)
	local held = {}
	held.x = player.x
	held.y = player.y - 8
	held.sp = sp
	held.player = player
	held.name = "generic"

	held.update = function(this)
		this.x = this.player.x
		this.y = this.player.y - 8
		if (this.player.state != 3) then
			newproj(this.sp,this.player)
			del(entarr,this)
		end
	end

	held.draw = function(this)
		outline_sprite(this.sp,0,this.x,this.y,false,false)
	end
	sfx(11,3)
	add(entarr,held)


end
-->8
--thrown item

function newproj(sp,player)
	local held = {}
	held.x = player.x
	held.y = player.y + 4
	held.z = 12
	held.name = "generic"
	local val = player.direction
	--set hsp and vsp
	held.hsp = 0
	held.vsp = 0
	if (val == 0) held.hsp = 1
	if (val == 1) held.vsp = 1
	if (val == 2) held.hsp = -1
	if (val == 3) held.vsp = -1
	held.sp = sp
	held.player = player

	held.update = function(this)
		if (this.z == 0 or colcheck(this.x,this.y,this.z,this)) then
			newdebris(this.x,this.y,177)
			del(entarr,this)
		end
		this.z -= 1
		this.x += this.hsp * 2
		this.y += this.vsp * 2
	end

	held.draw = function(this)
		circfill((this.x + 4) * 2, (this.y + 4) * 2, 4,0)
		outline_sprite(this.sp,0,this.x,this.y - this.z,false,false)
	end
	sfx(12,3)
	add(entarr,held)


end
-->8
--debris

function newdebris(x,y,sp)
	local debris = {}
	debris.timer = 0
	debris.sp = sp
	debris.name = "generic"
	debris.update = function(this)
		this.timer += 1
		if (this.timer == 8) del(entarr,this)
	end

	debris.draw = function(this)
		if (this.timer % 2 == 0) then
			outline_sprite(this.sp,0,x-this.timer,y-this.timer,false,false)
			outline_sprite(this.sp,0,x-this.timer,y+this.timer,false,false)
			outline_sprite(this.sp,0,x+this.timer,y+this.timer,false,false)
			outline_sprite(this.sp,0,x+this.timer,y-this.timer,false,false)
		end
	end
	sfx(3,3)
	add(entarr,debris)


end
-->8
--check map locations
function spikes()
	spike(8,8)
	spike(48,48)
end

function keyblock(place)
	mapeffect = place
	local bx = 0
	local by = 0
	for x=0,8 do
		for y=0,8 do
			if spcheck(x * 8,y * 8) == 101
			then
				bx = x*8
				by = y*8
			end
		end
	end
	if (dungeonsave[place] == 0) setat(bx,by,12)
end



function enemyloc()
	if dungeonsave[4] == 0 then
		mset(66,16,188)
		mset(69,16,188)
	end
	if dungeonsave[16] == 0 then
		mset(106,1,15)
		mset(109,1,15)
	end
	if dungeonsave[21] == 0 then
		mset(66,48,188)
		mset(69,48,188)
		mset(67,49,15)
	end


	local m = 0
	for x = 0,7 do
		for y = 0,7 do
			local ex = x*8
			local ey = y*8
			m = spcheck(ex,ey)
			if m > 145 and m < 153 then
				setat(ex,ey,spcheck(ex+8,ey))
				if (m == 146) octorock(ex,ey)
				if (m == 147) moblin(ex,ey)
				if (m == 148)	keese(ex,ey)
				if (m == 149)	boss1(ex,ey)
				if (m == 150)	boss2(ex,ey)
				if (m == 151)	then
					spike(ex,ey)
					setat(ex,ey,15)
				end

			end
		end
	end
end

function movepboss()
	music(-1)
	for i=0,15 do
		entarr[1].y -= 0.5
		_draw()
		darken()
		flip()
	end
	music(20)
end

function addnpc(sp,st)
	for x=0,8 do
		for y=0,8 do
			if spcheck(x * 8,y * 8) == 169
			then
				local p = newnpc(x * 8,y  * 8,sp)
				p.text = st
				return p
			end
		end
	end
end

function loaddoorsnpcs()
	mloc = flr(mapx/8) + 16 * flr(mapy/8)
	--dungeon 1
	if mloc == 40 then
		if dungeonsave[4] == 1 then
			local t = noenemies()
				t.action = function()
				bigheart(28,28,4)
				sfx(13,3)
				music(0)
			end
			movepboss()
		end
	end


	if mloc == 41 and hasroc == 0 then
		local t = noenemies()
		t.action = function()
			rocpup(32,24)
			newdebris(32,24,177)
			sfx(13,3)
		end
	end
	if mloc == 56 then
		keyblock(3)
	end
	if mloc == 59 then
		smallkey(24,24,1)
	end

	--dungeon 2
	if mloc == 12 then
		smallkey(32,16,14)
	end
	if mloc == 29 then
		keyblock(13)
	end

	if mloc == 44 then
		if (haspow == 0) powpup(28,16)
	end
	if mloc == 60 then
		if dungeonsave[12] == 1 then
			local t = noenemies()
			t.action = function()
				setat(24,8,15)
				setat(32,8,15)
				dungeonsave[12] = 0
			end
		else
			setat(24,8,15)
			setat(32,8,15)
		end
	end

	if (mloc == 61) keyblock(11)
	if mloc == 62 then
		if (mapnum != 2) mapnum = 2
		newdoor(32,56,2,40,48)
	end

	if mloc == 46 then
		if dungeonsave[9] == 1 then
			local b = blocktrigger(24,32)
			b.action = function(this)
				dungeonsave[9] = 0
				setat(8,24,15)
			end
		else
			setat(8,24,15)
		end
	end

	if mloc == 45 then
		if dungeonsave[10] == 1 then
			local t = noenemies()
			t.action = function(this)
				smallkey(8,24,10)
			end
		end
	end

	-- dungeon 3

	if mloc == 117 then
		newdoor(32,56,7,32,40)
		if (mapnum != 3) then
			mapnum = 3
			music(0)
		end
	end
	if mloc == 116 then
		flippup(28,28)
	end
	if mloc == 99 then
		local b = blocktrigger(8,48)
		b.action = function()
		setat(24,48,15)
		setat(32,48,15)
		end
	end
	if mloc == 102 then
		if dungeonsave[19] == 1 then
			local n = noenemies()
			n.action = function()
				smallkey(28,28,19)
			end
		end
	end
	if mloc == 119 then
		keyblock(20)
	end
	if mloc == 105 then
		triforce()
	end
	if mloc == 104 then
		if dungeonsave[21] == 1 then
			setat(24,56,100)
			movepboss()
			local n = noenemies()
			n.action = function()
				bigheart(28,28,21)
				music(0)
				setat(24,56,15)
				setat(8,24,15)
				setat(8,32,15)
			end
		else
			setat(8,24,15)
			setat(8,32,15)
		end
	end
	if mloc == 121 then
		local n = false
		for i=29,32 do
			if (dungeonsave[i] == 1) n = true
		end
		if (n or dungeonsave[4] == 1 or dungeonsave[16] == 1 or dungeonsave[21] == 1) then
			newtext("something is calling out.\n'bring me ore...'")
		else
			setat(24,8,15)
			setat(32,8,15)
			newtext("something is calling out.\n'you are worthy...'")
		end
	end


	if mloc == 115 then
		if dungeonsave[18] == 1 then
			local n = noenemies()
			n.action = function()
				dungeonsave[18] = 0
				setat(48,24,15)
				setat(48,32,15)
			end
		else
			setat(48,24,15)
			setat(48,32,15)
		end
	end
	--overworld
	if (mloc == 49) bigheart(16,24,25)
	if mloc == 7 then
		newdoor(32,32,117,32,48)
		if (mapnum != 0) then
			mapnum = 0
			music(8)
		end
	end
	if mloc == 54 then
		bigheart(48,24,26)
	end
	if mloc == 38 then
		bigheart(48,8,27)
	end
	if mloc == 21 then
		bigheart(48,32,28)
	end
	if mloc == 4 then
		orepup(16,24,29)
	end
	if mloc == 55 then
		orepup(32,40,30)
	end
	if mloc == 13 then

		if dungeonsave[16] == 1 then
			local e = entarr[1]
			movepboss()
			setat(32,56,100)
			local t = noenemies()
			t.action = function(this)
				setat(32,56,15)
				bigheart(28,28,16)
				music(0)
			end
		end

	end

	if mloc == 16 then
		newdoor(16,24,96,16,40)
	end
	if mloc == 17 then
		newdoor(40,24,112,32,40)
	end
	if mloc == 112 then
		addnpc(167,"find four ore and something\ngood will happen!")
		newdoor(32,48,17,40,32)
	end
	if mloc == 16 then
		addnpc(168,"welcome to pico village! make\nyourself at home!")
	end
	if mloc == 17 then
		local text = ""
		if hassword == 1 then
			text = "do be careful with that\nsword!"
		else
			text = "i think the man on the beach\nfound a sword..."
		end
		addnpc(184,text)
	end
	if mloc == 50 then
		newdoor(16,40,113,32,40)
	end
	if mloc == 113 then
		newdoor(32,48,50,16,48)
		local np = newnpc(32,24,182)
		if hassword == 1 then
			np.text = "press 🅾️ to slice stuff!\nhave fun!"
		else
			np.text = "i found this sword washed up.\nyou can have it!"
			np.action = function()
				hassword = 1
			end
		end
	end
	if mloc == 0 then
		newdoor(32,32,127,32,48)
	end
	if mloc == 127 then
		newdoor(32,56,0,32,40)
	end
	if mloc == 124 then
		newdoor(32,56,35,48,32)
	end
	if mloc == 126 or mloc == 125 then
		orepup(28,28,mloc - 94)
	end
	if mloc == 35 then
		newdoor(48,24,124,32,48)
	end
	if mloc == 1 then
		newdoor(24,40,97,40,40)
	end
	if mloc == 2 then
		if (mapnum != 0)	mapnum = 0
		newdoor(40,40,62,32,48)
	end
	if mloc == 96 then
		newdoor(16,48,16,16,32)
	end
	if mloc == 97 then
		newdoor(40,48,1,24,48)
	end
	if mloc == 38 then
		if (mapnum != 0)	mapnum = 0
		newdoor(24,32,57,32,48)
	end

	if mloc == 57 then
		if mapnum != 1 then
			mapnum = 1
			music(0,600)
		end
		newdoor(32,54,38,24,40)
	end
end





--
-->8
--generic enemy functions



function buildenemy(sp,hp)
	local enemy = {}
	enemy.sp = sp
	enemy.hp = hp
	enemy.bl = 1
	enemy.br = 7
	enemy.bb = 7
	enemy.bt = 1
	enemy.z = 0
	enemy.state = 0
	enemy.direction = 0
	enemy.timer = 12
	enemy.hflip = false
	enemy.vflip = false
	enemy.name = "enemy"
	return enemy
end
function damageenemy(this)
	applycol(this)
	keepbounds(this)
	this.timer -= 1
	if this.timer == 0 then
		this.state = 0
		this.timer = 12
	end
end
function keepbounds(this)
	if (this.x < 0) this.x = 0
	if (this.y < 0) this.y = 0
	if (this.x > 56) this.x = 56
	if (this.y > 56) this.y = 56
end

function projstate(this,sp)
	project(this.x,this.y,this.direction,sp)
end

function moveenemy(this,sp)
	if this.timer == 0 then
		this.state = 0
		this.timer = 12
	end
	this.hsp = 0
	this.vsp = 0
	if (this.direction == 0) this.hsp = sp
	if (this.direction == 2) this.hsp = -sp
	if (this.direction == 3) this.vsp = sp
	if (this.direction == 1) this.vsp = -sp
	applycol(this)
	keepbounds(this)
	this.timer -= 1
end
function movekeese(this)
	if this.timer == 0 then
		this.direction = flr(rnd(4))
		this.timer = 12
	end
	local hs = {1,1,-1,-1}
	local vs = {1,-1,-1,1}
	this.hsp = hs[this.direction + 1]
	this.vsp = vs[this.direction + 1]
	this.x += this.hsp/2
	this.y += this.vsp/2
	keepbounds(this)
	this.timer -= 1
end

function getclose(this)
	local h1 = {}
	h1.left = this.x-8
	h1.right = this.x+16
	h1.bottom = this.y+16
	h1.top = this.y-8
	local h2 = getbbox(entarr[1])
	if (boxcol(h1,h2)) this.state = 1
end

function destroyenemy(this)
	newdebris(this.x,this.y,177)
	if rnd(100) < 20 then
		heartpup(this.x,this.y)
	end
	sfx(5,3)
	del(entarr,this)
end

function waitran(this)
	this.hsp = 0
	this.vsp = 0
	if this.timer == 0 then
		this.state = 1
		this.timer = 12
		this.direction = flr(rnd(4))
	end
	this.timer -= 1

end

function checkdamage(this)
	if (entarr[1].z > 0) return
	local b1 = getbbox(this)
	local b2 = getbbox(entarr[1])
	if boxcol(b1,b2) and entarr[1].state != 4 then
		local val = entarr[1]
		val.state = 4
		val.timer = 8
		val.hp -= 1
		sfx(2,3)
		if (val.x < this.x) val.hsp = -1
		if (val.x > this.x) val.hsp = 1
		if (val.y < this.y) val.vsp = -1
		if (val.y > this.y) val.vsp = 1
		return true
	end
	return false
end

function boxcol(b1,b2)
	if (b1.left > b2.right or b1.right < b2.left or b1.top > b2.bottom or b1.bottom < b2.top) return false
	return true
end
--spike functions
function spike(x,y)
	local enemy = buildenemy(144,50)
	enemy.x = x
	enemy.y = y
	enemy.sx = x
	enemy.sy = y
	enemy.name = "spike"
	enemy.update = function(this)
		checkdamage(this)
		if this.state == 0 then
			local bb = {}
			local pb = getbbox(entarr[1])
			--check horizontal movement
			bb.left = this.x-64
			bb.right = this.x+64
			bb.top = this.y
			bb.bottom = this.y+8
			if boxcol(bb,pb) then
				this.state = 1
				this.vsp = 0
				if this.x > entarr[1].x then
					this.hsp = -2
				else
					this.hsp = 2
				end
			end
			bb.left = this.x
			bb.right = this.x+8
			bb.top = this.y - 64
			bb.bottom = this.y + 64
			if boxcol(bb,pb) then
				this.state = 1
				this.hsp = 0
				if this.y > entarr[1].y then
					this.vsp = -2
				else
					this.vsp = 2
				end
			end
		elseif this.state == 1 then
			applycol(this)
			if (this.hsp == 0 and this.vsp ==0)	this.state = 2
		elseif this.state == 2 then
			if (this.sx > this.x) this.hsp = 1
			if (this.sy > this.y) this.vsp = 1
			if (this.sx < this.x) this.hsp = -1
			if (this.sy <	this.y) this.vsp = -1
			applycol(this)
			if ((this.hsp == 0 and this.vsp == 0) or (this.sx == this.x and this.sy == this.y)) then
				this.state = 0
				this.sx = this.x
				this.sy = this.y
			end
		end
	end
	enemy.draw = function(this)
		outline_sprite(this.sp,0,this.x,this.y,false,false)
	end
	add(entarr,enemy)
	return enemy
end

function boss2(x,y)
	local enemy = buildenemy(155,5)
	coord = {}
	enemy.x = x
	enemy.y = y
	enemy.name = "enemy"
	enemy.hflip = false
	enemy.vflip = false
	enemy.lastx = {}
	enemy.lasty = {}
	for i=1,6 do
		add(enemy.lastx,x)
		add(enemy.lasty,y)
	end
	enemy.direction = flr(rnd(4))
	enemy.update = function(this)
		add(this.lastx,this.x)
		add(this.lasty,this.y)
		deli(this.lastx,1)
		deli(this.lasty,1)
		if (this.hp <=0) destroyenemy(this)
		checkdamage(this)
		if this.state == 0 then
			local vec = {1,1,-1,-1}
			local d = this.direction + 1
			this.hsp = vec[d]
			vec = {-1.5,1.5,1.5,-1.5}
			this.vsp = vec[d]
			vec = {false,false,true,true}
			this.hflip = vec[d]
			vec = {false,true,true,false}
			this.vflip = vec[d]
			applycol(this)
			if (this.vsp == 0 or this.hsp == 0) then
				this.direction = flr(rnd(4))
			end
		elseif this.state == 2 then
			this.timer -= 1
			this.direction = flr(rnd(4))
			if (this.timer == 0) this.state = 0
		end
	end
	enemy.draw = function(this)
		outline_sprite(157,3,this.lastx[1],this.lasty[1],false,false)
		outline_sprite(156,3,this.lastx[4],this.lasty[4],false,false)
		outline_sprite(this.sp,3,this.x,this.y,this.hflip,this.vflip)
		if (this.state == 2 and this.timer % 2 == 0) hurtpal(this)
	end
	add(entarr,enemy)
end


function boss1(x,y)
	local enemy = buildenemy(141,4)
	laser(enemy)
	enemy.x = x
	enemy.y = y
	enemy.soff = 0
	enemy.update = function(this)
		this.timer -= 0.5
		local t = flr(this.timer)
		if (this.state == 0) then
			if (this.hp <=0) destroyenemy(this)
			local s = 0
			if (t%8 == 0  or t% 10 == 0)	s = 1
			if (t%9 == 0)	s = 2
			this.soff = s
		if (t == 0) this.timer = 11
		else
			this.soff = 0
			if (t == 0) this.state = 0
		end
	end
	enemy.draw = function(this)
		outline_sprite(this.sp + this.soff,0,this.x,this.y,false,false)
		if (this.state == 2 and this.timer % 2 == 0) hurtpal(this)
	end
	add(entarr,enemy)
end

function laser(other)
	local laser = buildenemy(0,1000)
	laser.y = flr(rnd(8)) + 16
	laser.x = 0
	laser.br = 64
	laser.bt = 4
	laser.bb = 5
	laser.other = other
	laser.name = "name"
	laser.vsp = rnd(1) + 0.5
	laser.update = function(this)
		checkdamage(this)
		if (other.hp <= 0) del(entarr,this)
		this.y += this.vsp
		if (this.y > 57 or this.y<7) this.vsp = this.vsp * -1
	end
	laser.draw = function(this)
	for i=3,5 do
		line(0,this.y * 2 + i,128,this.y * 2 + i,flr(rnd(3) + 8))
	end
	end
	add(entarr,laser)
end

--octorock functions
function octorock(x,y)
	local enemy = buildenemy(132,2)
	enemy.x = x
	enemy.y = y
	enemy.update = function(this)
		if (this.hp <=0) destroyenemy(this)
			checkdamage(this)
		if this.state == 0 then
			waitran(this)
			if (rnd(100) < 2) this.state = 3
		elseif this.state == 1 then
			moveenemy(this,1)
		elseif this.state == 2 then
			damageenemy(this)
		elseif this.state == 3 then
			projstate(this,160)
			this.state = 0
		end

	end
	enemy.draw = function(this)
		octodraw(this)
		outline_sprite(this.sp,0,this.x,this.y,this.hflip,this.vflip)
		if (this.state == 2 and anim_time % 2 == 0) hurtpal(this)
	end
	add(entarr,enemy)
end

-- keese functions
function keese(x,y)
	local enemy = buildenemy(136,1)
	enemy.x = x
	enemy.y = y
	enemy.update = function(this)
		if (this.hp <=0) destroyenemy(this)
			checkdamage(this)
		if this.state == 0 then
			getclose(this)
		elseif this.state == 1 then
			movekeese(this)
		end

	end
	enemy.draw = function(this)
		if this.state == 1 then
			twospenemy(this)
		else
			outline_sprite(this.sp,0,this.x,this.y,false,false)
		end
	end
	add(entarr,enemy)
end

function twospenemy(this)
	local aon = 0
	if (anim) aon += 1
	outline_sprite(this.sp + aon,0,this.x,this.y,false,false)
end

function octodraw(this)
	this.sp = 132
	this.hflip = false
	this.vflip = false
	if (this.direction == 0) this.hflip = true
	if this.direction == 3 then
		this.vflip = true
		this.sp = 134
	end
	if this.direction == 1 then
		this.sp = 134
	end
	if (anim) this.sp += 1
end

--moblin functions
function moblin(x,y)
	local enemy = buildenemy(128,4)
	enemy.x = x
	enemy.y = y
	enemy.update = function(this)
		if (this.hp <=0) destroyenemy(this)
		checkdamage(this)
		if this.state == 0 then
			waitran(this)
		elseif this.state == 1 then
			moveenemy(this,1)
		elseif this.state == 2 then
			damageenemy(this)
		end

	end
	enemy.draw = function(this)
		get_4way_spr(this.x,this.y,this.sp,this.direction,this.hsp,this.vsp)
		if (this.state == 2 and anim_time % 2 == 0) hurtpal(this)
	end
	add(entarr,enemy)
end

function project(x,y,direction,sp)
	local proj = buildenemy(sp,10)
	proj.name = "generic"
	proj.x = x
	proj.y = y
	proj.direction = direction
	proj.hsp = 0
	proj.vsp = 0
	proj.update = function(this)
		local hd = {1,0,-1,0}
		local vd = {0,-1,0,1}
		this.x += hd[this.direction + 1] * 1.5
		this.y += vd[this.direction + 1] * 1.5
		if fget(spcheck(this.x + 4,this.y + 4),0) or checkdamage(this,1) then
			del(entarr,this)
			newdebris(this.x,this.y,177)
		end
	end

	proj.draw = function(this)
		outline_sprite(this.sp,0,this.x,this.y,false,false)
	end
	add(entarr,proj)
end
-->8
--door stuff






function newdoor(x,y,mgo,pgox,pgoy)
	local door = {}
	door.name = "generic"
	door.x = x
	door.y = y
	door.bl = 0
	door.br = 8
	door.bb = 8
	door.bt = 0
	door.mgo = mgo

	door.pgox = pgox
	door.pgoy = pgoy
	door.draw = function(this)
	end
	door.update = function(this)
		local b1 = getbbox(this)
		local b2 = {}
		local p = entarr[1]
		b2.left = p.x + 4
		b2.right = b2.left
		b2.top = p.y + 4
		b2.bottom = b2.top

		if boxcol(b1,b2) then
			dumploop()
			fancycls()
		 tmapx = (mgo % 16) * 8
		 tmapy = flr(this.mgo / 16) * 8
		 mapx = tmapx
		 mapy = tmapy
		 p.x = this.pgox
		 p.y = this.pgoy
		 loadin()
		 if mapnum != 0 then
		 	music(0)
		 else
		 	music(8)
		 end
		end

	end
	add(entarr,door)
end
-->8
--block code





function newblock(x,y,player)
	local dir = player.direction
	local block = {}
	block.hsp = 0
	block.vsp = 0
	block.x = x
	block.y = y
	block.timer = 8
	if (dir == 0) block.hsp = 1
	if (dir == 1) block.vsp = 1
	if (dir == 2) block.hsp = -1
	if (dir == 3) block.vsp = -1

	block.update = function(this)
		sfx(8,3)
		sfx(9,1)
		this.timer -= 1
		this.x += this.hsp
		this.y += this.vsp
		if this.timer == 0 then
			del(entarr,this)
			setat(this.x,this.y,27)
		end
	end

	block.draw = function(this)
		outline_sprite(28,0,this.x,this.y,false,false)
	end

	add(entarr,block)

end


-->8
--pickups

function pickup(x,y,sp)
	local pup = {}
	pup.x = x
	pup.y = y
	pup.bl = 0
	pup.br = 8
	pup.bt = 0
	pup.bb = 8
	pup.z = 0
	pup.sp = sp
	pup.update = function(this)
		if checkcolent(this,"player") then
			this.action(this)
			del(entarr,this)
			sfx(this.snd,3)
		end
		if (this.z > 0) this.z -= 0.5
	end
	pup.draw = function(this)
		outline_sprite(this.sp,0,this.x,this.y - this.z,false,false)
	end
	return pup
end
function smallkey(x,y,place)
	if (dungeonsave[place] == 0) return
	newdebris(x,y,177)
	local this = pickup(x,y,161)
	this.z = 0
	this.place = place
	this.snd = 14
	this.action = function(this)
		dungeonsave[place] = 0
		dungeonkeys[mapnum] += 1
	end
	add(entarr,this)
end

function triforce()
	local this = pickup(28,28,154)
	this.timer = 0
	this.snd = 26
	this.action = function(this)
		wingame()
	end
	this.draw = function(this)
		this.timer += 0.02
		circfill(64,64,32 - sin(this.timer),10)
		circfill(64,64,25 - sin(this.timer),7)
		outline_sprite(this.sp,0,this.x,this.y - sin(this.timer),false,false)
	end
	add(entarr,this)
end

function bigheart(x,y,place)
	if (dungeonsave[place] == 0) return
	local this = pickup(x,y,176)
	newdebris(x,y,177)
	this.z = 4
	this.place = place
	this.snd = 14
	this.action = function(this)
		local p = entarr[1]
		dungeonsave[place] = 0
		p.hpmax += 1
		p.hp = p.hpmax
		maxhp = p.hpmax
		newtext("you got a heart container!\nyour life increased!")
	end
	add(entarr,this)
end



function heartpup(x,y)
	x = x
	y = y
	local heart = pickup(x,y,178)
	heart.z = 4
	heart.snd = 7
	heart.action = function(this)
		local p = entarr[1]
		p.hp += 1
		if (p.hp > p.hpmax) p.hp = p.hpmax
	end
	add(entarr,heart)
end

function rocpup(x,y)
	local roc = pickup(x,y,180)
	roc.z = 4
	roc.snd = 14
	roc.action = function(this)
		hasroc = 1
		newtext("you got roc's feather! press\n🅾️ to hop around!")
	end
	add(entarr,roc)
end

function powpup(x,y)
	local roc = pickup(x,y,181)
	roc.z = 4
	roc.snd = 14
	roc.action = function(this)
		haspow = 1
		newtext("you got the power bracelet!\npress 🅾️ to lift stuff!")
	end
	add(entarr,roc)
end

function flippup(x,y)
	if (hasflips == 1) return
	local roc = pickup(x,y,185)
	roc.z = 4
	roc.snd = 14
	roc.action = function(this)
		hasflips = 1
		newtext("you got the flippers! you\ncan swim safely now!")
	end
	add(entarr,roc)
end

function orepup(x,y,place)
	if (dungeonsave[place] == 0) return
	local roc = pickup(x,y,139)
	roc.z = 4
	roc.snd = 14
	roc.action = function(this)
		dungeonsave[place] = 0
		newtext("you got magic ore! there\nmay be more hidden!")
	end
	add(entarr,roc)
end
-->8
--npc code

function newnpc(x,y,sp)
	this = {}
	this.sp = sp
	this.x = x
	this.y = y
	this.name = "npc"
	this.action = function()
	end
	this.draw = function(this)
		outline_sprite(this.sp,0,this.x,this.y,anim,false)
	end

	this.update = function(this)
	end
	add(entarr,this)
	return this
end
-->8
--textbox

function newtext(st)

	for obj in all(entarr) do
		if (obj.name == "text") del(entarr,obj)
	end
	this = {}
	this.st = st
	this.x = 1
	this.y = 0
	this.name = "text"

	this.update = function(this)
		pause = true
		this.x += 1
		if (btnp(🅾️) and this.x > 4) del(entarr,this)
		if (btnp(❎)) this.x = 100
	end
	this.draw = function(this)
		rectfill(4,6,124,26,10)
		rect(4,6,124,26,0)
		local s = sub(this.st,1,this.x)
		for x=-1,1 do
			for y=-1,1 do
				print(s,7 + x, 11 + y,0)
			end
		end
		if s != this.st then
			sfx(15,1)
		end
		print(s,7, 11,7)
	end
	add(entarr,this)
end

-->8
--behavior triggers

function noenemies()
	this = {}
	this.update = function(this)
		if ckforenemies() == true	then
			this.action()
			del(entarr,this)
		end
	end

	this.draw = function()
	end
	add(entarr,this)
	return this
end

function blocktrigger(x,y)
	this = {}
	this.update = function(this)
		if spcheck(x,y) == 27 then
			this.action(this)
			del(entarr,this)
		end
	end
	this.draw = function()
	end
	add(entarr,this)
	return this
end
__gfx__
00000000bbbbbbbbbbbbbbbaaaaaaaaaabbbbbbbbbbbbbbbaaaaaaaabbbbbbbbaaaaaaaaaaaaaaaaabbbbbbbbbbbbbba77797779aaaa4a9afaaaaaafa4444aaa
00000000bbbbbbbbbbbbbbaaababaaaaaabbbbbbbbbbbbbbbaabbaabb8bbbb3baabbbbbbbbbbbbaaabbbbbbbbbbbbbba79987998a4aaaaaaaaaaaaff4aaa4a8a
00700700bbbbbbbbbbbbbbaaaabaaaaaaabbbbbbbbbbbbbbbbbbbbbb8a8bb3bbabbbbbbbbbbbbbbaabbbbbbbbbbbbbba79987998aa9aaa4aaaaaaffa4a8a4aaa
00077000bbbbbbbbbbbbbbbaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbb8bbbbbbabb3bbbbbbbb3bbaabbb3bbbbbb3bbba98889888aaaa4aaaaaaafffa4aaaa444
00077000bbbbbbbbbbbbbbbaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbb8bbabbb3bbbbbb3bbbaabb3bbbbbbbb3bba77797779a4aaaaa9aaaffffa444aaaa4
00700700bbbbbbbbbbbbbbaaaaaababaaabbbbbbbbbbbbbbbbbbbbbbb3bb8a8babbbbbbbbbbbbbbaabbbbbbbbbbbbbba799879989aaa9a4aaafffffaaaa4a8a4
00000000bbbbbbbbbbbbbbaaaaaaabaaaabbbbbbbaabbaabbbbbbbbbbb3bb8bbabbbbbbbbbbbbbbaaabbbbbbbbbbbbaa79987998aa4aaaaaafffffaaa8a4aaa4
00000000bbbbbbbbbbbbbbbaaaaaaaaaabbbbbbbaaaaaaaabbbbbbbbbbbbbbbbabbbbbbbbbbbbbbaaaaaaaaaaaaaaaaa98889888a9aaaaa4ffaaaaafaaa4444a
9994999499949994999499949994999411111111ccccccccb333b33399aa944966666666ffffffff500000050000000000000000aaaaaaaaaaa00aaaaa0000aa
99499949994999499a4aaa4999499949c111c111cc77cc7cb3b333a39999999977777777ff77ff7f00666600079999400799994000a0a00aaa0940aaa099990a
9449944994499449a99aa99a944994491111111177cc77c7333b3ab3944999aa66666666771177170677776009799440097994400303030aa099440a09900990
49944994499449999aa99aa99994499411c111c1cccccccca33bab33444444447777777711cc11c106700760099aa440099aa440a0b3b0aa09977440a099990a
99949994999499a9aa7777a9aa94999411111111ccccccccba33ab3399aa944966666666cccccccc06700760099aa440099aa44003bbb30a0974474009000090
994999499949999aa777777aaa499949c111c111c7cc77cc3ba333339999999977777777c7cc77cc067777600944494009444940a03030aa0974474004999940
944994499449999a77777777a9499449111111117c77cc773ba33b33944999aa66666666717711770066660004444490044444900303030a0974474009444490
4994499449944aa9777777779aa4499411c111c1cccccccc3333b33b44444444777777771c11cc1105000060000000000000000000a0a00aa000000aa000000a
aaaa30000003aaaa000bbbbbbbb33000aaa000000000000000000aaadddddddddddddddd07777770a00aa00aa00000000000000aa00aa00a0400404000000000
aa300333333003aa333003bbbb300333aa01777777777777777710aad71110111011117d077777700c100c1006666666666666600ff00ff04000000400000000
a30bb377333bb03a333bb03bb30bb377a0117cccccccccccccc7110adc7000000000071d077777700c100c100dddddddddddddd00ff00ff00000000000000000
a03b3377333bbb0a333bbb0b303b337701117cccccccccccccc71110dc1710111101701d077777700c100c10a00000000000000a044004404000000000000000
03bb3333333bbb30333bbb3003bb333301117cccccccccccccc71110d11c70111107100d077777700c100c100449999999999440044004400000000400000000
03bbb333333bbb30333bbb3003bbb33301117cccccccccccccc71110dc1117000070001d077777700c100c100444444444444440044004400000000000000000
03bbb33333bbbb3033bbbb3003bbb33301117cccccccccccccc71110dc1cc1711701101d077777700c100c10a00000444400000a044004404000000400000000
03bbbbbbbbbbb330bbbbb33003bbbbbb01117cccccccccccccc71110dc1cc1c77101101d0000000000000000aaa0000000000aaaa00aa00a0404004000000000
030bbbbbbbb33330044444404444444401117cccccccccccccc71110d11cc1c000000000dc1cc1c77101101d04490d6006d09440aa000000000000aaa000a00a
40033bbbbb330304000000000000000001117cccccccccccccc71110dc1cc1c0ccc1ccc1dc1cc17cc701101d04990dd66dd09940a04444444444440a066a9a60
0400303bb3330040090000904000000401117cccccccccccccc71110dc1111c011111111d11117111170000d049990dddd099940044444444444444006a98a60
04400033333004400a0000a040000004011177777777777777771110dc1cc1c01cccc1ccdc1c71cccc17101d0499990000999940044449999994444006988960
40094000000490040a0000a040000004011c1111111111111111c110d11cc1101cccc1ccdc17c1cccc1c701d0449999999999440044499000099444006666660
90940977944049090a8888a04000000401c111111111111111111c10dc1cc1c011111111dc7111111111171d0444444444444440044990dddd09944006766760
a00040999404000a0aaaaaa0400000040c11111111111111111111c0dc1cc1c0ccc1ccc1d7ccc1cccc1ccc7da00000444400000a04490dd66dd0944006600660
aaa9990000999aaa0000000000000000000000000000000000000000dc1111c0ddddddddddddddddddddddddaaa0000000000aaa04490d6006d09440a004400a
aa00000000000000000000aa06d0944004490d6006d094000000000000490d60044999999999999999999440044999999999944004490d6006d0944000000000
a0666666666666666666660a06d0944004490d6006d094004400004400490d60004444499444444994444400004444499444440004990dd66dd099400aaaaaa0
0666dddddddddddddddd666006d0940000490d6006d090444444444444090d600900004444000044440000900900004444000090049990dddd0999400a9999a0
066d0000000000000000d66006d0940000490d6006d099999999999999990d60049000000000000000000940049000000000094004999900009999400a9999a0
06d099999999999999990d6006d0940000490d60066d0000000000000000d660044999999999999999999440044999999999944004499999999994400aaaaaa0
06d090444444444444090d6006d0940000490d600666dddddddddddddddd66600044444994444449944444000444444444444440004444499444440000000000
06d094004400004400490d6006d0944004490d60a0666666666666666666660a090000444400004444000090a00000444400000a090444004400009009044090
06d094000000000000490d6006d0944004490d60aa00000000000000000000aa049000000000000000000940aaa0000000000aaa049000000000094040444404
52228868886888688868225555228868886822550566666005666660500000000000000500000000076666700766667007666670000000000000000099000049
552288688868886888682255552288688868225505566660055666600077777777777700777777770766667007666677776666700aaaaaaaaaaaaaa090aaaa09
525288688868886888682525525288688868252500556600005566000777666666667770666666660766667007666666666666700a999999999999a00a9999a0
5255666666666666666655255255666666665525050000600500006007766d666666677066666d6607666d7007666d66666d66700a999999999999a00a9999a0
525268886888688868882525525268886888252505666660056666600766666666666670666666660766667007766666666667700aaaaaaaaaaaaaa00a9999a0
255268886888688868882552a55268886888255a0556666005566660076666666666667066666666076666705077666666667702000000000000000004aaaa40
255268886888688868882552aa526888688825aa00556600a055660a076d6677776d667077777777076d6670520777777777705209044444444440909044440a
252566666666666666665252aaa5666666665aaa05000060aa0000aa076666700766667000000000076666705220000000000252404444444444440444000044
dddddddddddddddd08b2c8b00288888000000000000000000007aaaa00000000cccccccccccccccccccccccccccccccc1111111771111111cc7ccc1711111111
11101110111011100999999002228880076666d0076666d00007aaaa01111110cccccc000000000000cccccc777c7777111111777711111171717c1711111111
000000000000000002c8b2c00288888006766dd0067007d0007aaaaa01111110ccc0004444444444440000cccccccccc111177777777111171117c1c11111111
110000110000000002c8b2c00222228006677dd0067007d0007aaaaa00000000cc044999999999999994440ccccccccc1117777777777111c171c17c11111111
1099990109999990099999900299992006677dd0067707d007aaaaaa094aa490004499000000000000994440cccccccc1117777777777111c177c17111111111
099999900999999008b2c8b00999999006ddd6d0067007d007aaaaaa09444490044990dddddddddddd09944077cc77cc1777777777777771c1c717c111111111
097777900999999008b2c8b0090000900ddddd600ddddd607aaaaaaa0999999004490dd6666666666dd09440cccccccc777777777777777717cc17cc11111111
0788887008b2c8b000000000004444000000000000000000999999990000000004490d600000000006d09440cccccccc777777777777777717cc1ccc11111111
0033330a0000000000333300003333a0000000000000000000000000000000000033330a0000000000333300003333a0003333000000000000000000cccccccc
0333aaa00033330a03333a300333333a000000000000000000333300003333a0033faaa00033330af3333a3ff333333ff3aaaa3f0000000000000000cccccccc
33fafcff0333aaa0f3aaaa3f03bb33300033330a0000000003333a300333333a33f3fcff033faaa033aaaa3333bb33333fcffcf30000000000000000cccccccc
03ffffff33fafcff0fcffcf00b3333b00333aaa00033330af3aaaa3f03bb333003f3ffff33f3fcff3fcffcf33b3333b33ffffff300000000000b3000cccccccc
0033330003ffffff03ffff30f4bbbb4033fafcff0333aaa07fcffcf77b3333b70033330003f3ffff33ffff3304bbbb4033ffff33000000000003b000cccccccc
0044f4a000444fa00f4aa44f0344443073ffffff33fafcff07ffff7007bbbb70004444a0004444a0034aa44003444430034aa4400000000000000000cccccccc
003333300433333003333330003333000777777777777777007777000077770000333330043333300333333000333300033333300000000000000000cccccccc
000444000440044400004400000044000000000000000000000000000000000000044400044004440000440000004400044004400000000000000000cccccccc
777777007700000077077707770777070220002200020200008888000088880000000000000000000077770000bb700000000007000000000000000000000000
66770077677777000777777007777770002078200020722020077002000770000000000010000010070000700b3bb70000000007000770000000000000000000
0167707601770077077117700700700080078880800788802270702202707020001010001000001070777707b333bb700000007a077777700000000000000000
1116666011677076071111700670707087788888877888880807870022078702011111001100011007777077b3333b660000007a778888770000000000000000
c1111100c116666071cccc10716777008707888887078888087888700878887001717100111111107707777063333dd6000007aa788118877800008700000000
17777cc017777cc07c1111c07116677080707880807078800888888022888822111111101171711070777707555dddd6000007aa781111877811118770000007
17777110c77771100111cc1000111770002888200022822022888822028888201101011001111100070000700555dd6000007aaa081111800811118008100180
00ccc000ccc00ccc000cccc0000cccc00220002200020200200880020008800010000010001110000077770000ddd00000007aaa000110000001100000011000
0aa00aa0999999990000000000000000000000000000000000000000000000000007aaaa00bbb00000079000009997800000000000000000a4444aa794444aaa
aa8888aa990000990000000000000000000000000000000000000000000000000007aaaa0b777b00007a90000999a77000099000000000004aaa4a7999aa4a8a
a808808a400000040aa00000a000a0000a00a000aa000a00aa000a00000aaa00007aaaaab7bbb7b0007aa9009999aa990099aa00000990004a8a4799999a4aaa
0808808000000000a00a0000aa0aa0000a0a0000a0a00a00a0a0a0a0000a0000007aaaaab7bbb7b007aaa9009999997709999a90009aa9004aaa79999999a444
0188881090000009a00a0000a0a0a0000aa00000aa000a00aa0000a0000aaa0007aaaaaab7bbb7b0079007909999997809999990009aa9004a47999999999aa4
a111111a90000009a00a0000a000a0000a0a0000a0a00a00a0a00a0000000a0007aaaaaa0b777b007aa97a90999999990099990000099000aa799999999999a4
aa1111aa400000040aa00000a000a0000a00a000aa000a00aa00aaa0000aaa007aaaaaaa00bbb0007aa97aa9099999900009900000000000a799999999999994
0aa00aa00000000000000000000000000000000000000000000000000000000099999999000000007aa7aaa90099990000000000000000007999999999999999
00000000009aaa000000000000066000066000000000000000000000044444400aaaaaa0aaaaaaaaaaaa90000001101ddddddddd00000000000000000d0d0d0d
00aaa000009a0a00000000000006600000060660000940000099990044444444aaaaaaaaaaaaaaaaaaaa90000101101d1110111007ccc1ccc1cccc70d0d0d0d0
0a889a00009aaa0000c0000000066000000066600099440009900990f4ffff4faaffffaaaaaaaaaaaaaaa9000100001d0000000001711111111117c00d0d0d0d
0a898a0000009a00ac666666000660000006660009977440009999004f0ff0f4afcffcfaaaaaaaaaaaaaa9000101101d011110110107c1cccc1c71c0d0d0d0d0
0a988a000009aa00ac666666000660000c666060097447400900009009ffff90a8ffff8aaaaaaaaaaaaaaa900001100d01111011000171cccc17c1100d0d0d0d
00aaa00000009a0000c0000000c66c000c66000609744740049999400f99999f0f88888faaaaaaaaaaaaaa900101101d0000000001000711117111c0d0d0d0d0
000000000009aa0000000000000cc000accc000609744740094444900099990000888800aaaaaaaaaaaaaaa90101101d111011100101107cc71cc1c00d0d0d0d
000000000000000000000000000aa0000a00000000000000000000000000110000004400aaaaaaaa999999990100001d00000000010110177c1cc1c0d0d0d0d0
00707000000000000000000000065000000000000000000006666660044444400666666000001110999999990101101cdddddddd7c1cc1c00000000090000000
0787870000000000000000000006500000098990009990000666666004444440666666660000101097777779010110c111101110171cc1c00700000a90000000
728888700000000000000000000650000098990009900900f6ffff6ff4ffff4ff6ffff6f00aa11909700007900000c00000000000071111000000a00a9000000
7288887000066000008080000006500000899900089000000f0ff0f00f0ff0f06f0ff0f6aa99aa90977777790101c011011110111107c1c000000000a9000000
0728870000066000008880000c0650c0008990000890000002f66f2001f44f106effffe69aaaa90099999999010c101101111011110171c0c0000070aa900000
00727000000000000008000000c65c0000099000099009000f22222f0f11111f0feeeeef09aa90000044440001c0000000000000000007c000a00000aa900000
000700000000000000000000000cc0000000440000999000002222000011110000eeee0000990000009999000c1110111110111011011170700000a0aaa90000
000000000000000000000000000aa00000000000000000000000440000004400000044000000000000099000000000000000000000000000000c0000aaa90000
72cacacaca16068272060616cacaca82f2f2f2c8fbf2f2f272cacacacacaca8272cacacacacaca8272cacacacacaca8272cacacacacacacbcbcacacacacaca82
72ca59caca59ca8272cacacacacaca8200000000000000000000000000000000fa72cbcbcbcb82fafa72cbcbcbcb82fafa72cbcbcbcb82fafa72cbcbcbcb82fa
73f17171c02636ba73363626c0f1f1baf2f2f266aaf2f2f273b1f0f0f0f0b1ba73794141414141ba73494141414141ba735151515151515151515151515151ba
73f0f069f0f0f0ba73f0f0f0f0f0f0ba0000000000000000000000000000000072dbf0f0f0f0bb8272db79b179b1bb8272dbf0f0f0f0bb8272dbb1f0f0b1bb82
73f17171c0c0c0ba73c0c0c0c071f1baf2f2c8fbc8fbf2f273f0f029f0f0f0bbdb4141f0f04141bbdb414151515141ba735139f039f051daea515151515151bb
dbf0c0c0c0c0f0ba73f0f0f0f0f0f0ba0000000000000000000000000000000073f0f0b179f0f0bbdbf0f2f0f2f0f0ba73f0f0c1c1c1f0bbdbb1f0b1b1f0b1ba
73d5e571717171ba73c0c0c0c07171baf2f266aa66aaf2f273f0f0f0f0f0f0f0f041f3f0f0f3414141415151515141ba7351f0c0c0f051ba735151515151f1f0
f046c0f0f0c0f0ba73f0f0f0f0f0f0ba0000000000000000000000000000000073f0f2b1b1b1f0f0f0f0f2f0f2f0f0ba73f0f0c1f0f0c1f0f0f0c1f0f0b1f0ba
73f5f571717171ba73d5e571010101baf2f2f2f2f2f2f2f273f0f029f029f0f0f041f3f0f0f3414141515151515141ba7351f0c0c0f051ba735151515151f1f0
f046c0f0f0c0f0ba73f0f0f0f0f0f0ba0000000000000000000000000000000073f0f2f279b1f0f0f0f0b1b1b1b1f0ba73f0f0f0c1f0c1f0f0f0c1f0f0b1f0ba
73112131717171ba73f5f571112131baf2f2f2f2f2f2f2f273c1f0f0f0f0f0daea4141f0f04141daea515151514941ba7351f029f0f051ba73515151515151da
eaf0c0c0c0c0f0ba73f0f0f0f0f0f0ba0000000000000000000000000000000073f0f2f2b1da83838383eaf0f0f0f0ba73f0c1c1c1c1f0daeab1f0c1c1f0b1ba
93839283838383a393838383839283a3f2f2f2f2f2f2f2f273f0f04646f0b1ba73414141414179ba73515151f04141ba7351f0f0f0f051ba73f05151515151ba
73f0f0f0f0f0f0ba73f0f0f0f0f0f0ba0000000000000000000000000000000093eaf0f0f2bafafafafa73f0f0f0daa393eaf0f0f0f0daa393eab1f0f0b1daa3
fafafafafafafafafafafafafafafafaf2f2f2f2f2f2f2f29383eaf0f0da83a393838383838383a3938383eaf0da83a373515151515151ba73f05151515151ba
9383eaf0da8383a39383eaf0f0da83a300000000000000000000000000000000fa93eaf0f0bafafafafa73f0f0daa3fafa93eaf0f0daa3fafa93eaf0f0daa3fa
fafafafafafafafafafafafafafafafafafafafafafafafafafa73f0f0bafafa72cacacacacaca8272cacadbf0bbca8273515151515151ba73f051515151f0ba
72cadbf0bbcbca8272cadbf0f0bbca8200000000000000000000000000000000fa72dbf0f0bb82fa72cadbf0f0bbca8272cbdbf0f0bbcb82fa72dbf0f0bb82fa
fa720616caca82fafa720616caca82fafa72cacacaca82fa72cadbf0f0bbca827379f0f0f0f0f0ba735151f0f0f0f3ba7351515151f1f1bbdbf051515151f0ba
73d1d1f0f0d1d1ba73f3f04646f0f3ba0000000000000000000000000000000072dbf0f0f0f0bb8273f0f0f0f0f0f0ba73f0f0f0f0f0f0ba72dbf0f0f0f0bb82
fa733626719abafafa7336264141bafafa73f1f1f166bafa73f0f0f0f0f0f0bbdbf0da8383eaf0bbdb5151f0f0f0f0ba7351515151f1f0f0f0f051515151f0bb
dbd1d1f0f0d1d1bbdbf0f0f0f0f0f0ba0000000000000000000000000000000073f2f0f0f0f0f2ba73f0da8383eaf0ba73f0da8383eaf0ba73f3f0f0f0f0f3ba
fa7371717171bafafa73d0d09ad1bafafa73f1f1f1f1bafa73f049f0f0f046f0f0f0bac0c073f051515151f0f0f0f0bbdbf0f05151f1f0f0f0f05151515156f0
f0f0f0f0f0f0f0f0f0f0f0e9f9f0f0ba0000000000000000000000000000000073f2f2f2f2f2f2ba73f0bac0c073f0ba73f0bac0c073f0ba73f0f0f0f0f0f0ba
fa73f4f57171bafafa73d0d0d0d0bafafa73d0d0d0d0bafa73f0f039f0f046f0f0f0bac0c073f051515151c0c0c0c0f0f0f0f05151f1f1daeaf051515151f0da
eaf0f0f0f0f0f0f0f0f0e9f9e9f9f0ba0000000000000000000000000000000073f0f0f0f0f0f0ba73f0bac0c073f0ba73f0bac0c073f0ba73f0f0f0f0f0f0ba
fa73f5112131bafafa7341112131bafafa73d0112131bafa73f0f0f0f049f0daeaf0bb8181dbf0daea5151010101c0daeaf0f051515151ba73f051515151f0ba
73d1d1f0f0d1d1daeaf0f0f0f0f0f0ba0000000000000000000000000000000073010101010101ba73f0bb8181dbf0ba73f0bb8181dbf0ba73010101010101ba
fa9383839283a3fafa9383839283a3fafa9383839283a3fa7339f0f0f0f0f0ba73f0f0f0f0f079ba735151112131c0ba73515151515151ba73f0f0f0f0f0f0ba
73d1d1f0f0d1d1ba73f0f0f0f0f0f0ba0000000000000000000000000000000093ea01112131daa373f0f0f0f0f0f0ba73f0f0f0f0f0f0ba93ea01112131daa3
fafafafafafafafafafafafafafafafafafafafafafafafa93838383838383a393838383838383a3938383ea92da83a393838383838383a393838383838383a3
93838383838383a393838383838383a300000000000000000000000000000000fa9383839283a3fa93838383838383a393838383838383a3fa9383839283a3fa
__label__
00000000000000000000000000000000000000000000000000000000000000779900000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000779900000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000779900000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000779900000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000077aaaa99000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000077aaaa99000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000077aaaa99000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000077aaaa99000000000000000000000000000000000000000000000000000000000000
000000000000000000000000000000000000000000000000000000000077aaaaaaaa990000000000000000000000000000000000000000000000000000000000
000000000000000000000000000000000000000000000000000000000077aaaaaaaa990000000000000000000000000000000000000000000000000000000000
000000000000000000000000000000000000000000000000000000000077aaaaaaaa990000000000000000000000000000000000000000000000000000000000
000000000000000000000000000000000000000000000000000000000077aaaaaaaa990000000000000000000000000000000000000000000000000000000000
0000000000000000000000000000000000000000000000000000000077aaaaaaaaaaaa9900000000000000000000000000000000000000000000000000000000
0000000000000000000000000000000000000000000000000000000077aaaaaaaaaaaa9900000000000000000000000000000000000000000000000000000000
0000000000000000000000000000000000000000000000000000000077aaaaaaaaaaaa9900000000000000000000000000000000000000000000000000000000
0000000000000000000888888888888888888888888888880000000077aaaaaaaaaaaa9900000000000000000000000000000000000000000000000000000000
0000000000000000008888888888888888888888888888880ccc0c7cacccaaaaacaaaccc99cc0ccc0cc00cc0000000cc0ccc0000000000000000000000000000
00000000000000000088000000000000000000000000008800c00c7cacaaaaaaacaaacaa9c000c000c0c0c0c00000c0c0c000000000000000000000000000000
00000000000000000888000888888888888888888880008800c00cccaccaaaaaacaaacca9c000cc00c0c0c0c00000c0c0cc00000000000000000000000000000
00000000000000000880008888888888888888888880008800c00c7cacaaaaaaacaaacaa9c0c0c000c0c0c0c00000c0c0c000000000000000000000000000000
00000000000000008880088800000000000000008880088800c07cacacccaaaaacccacccaccc0ccc0c0c0ccc00000cc00c000000000000000000000000000000
000000000000000088008880000000000000000088000880000077aaaaaaaaaaaaaaaaaaaa990000000000000000000000000000000000000000000000000000
000000000000000888088800000000000000000888008888888888888888888888888888aa990088888888880000000000008888888888000000000000000000
000000000000000880888000000000000000008880088888888888888888888888888888aa990088888888888888000000008888888888000000000000000000
0000000000000008888800000000000000000888000880888077aaaaaaaaa8888aaaa888aaaa9988800000088888800000008880000088000000000000000000
00000000000000008880000000000000000008800088800888788888888aa88888aa888aaaaa9908880000000008888000000888000088000000000000000000
000000000000000000000000000000000000888008880000887888888888a88a88aa88aaaaaa9900880000000000888800000088800088800000000000000000
00000000000000000000000000000000000888000880000088788aaaaa88888a88aa88aaaaaa9900880088880000008800000008800008800000000000000000
00000000000000000000000000000000000880008880000088a88aaaaaa8888a88aa88aaaaaaaa99880088888800008880000008800008800000000000000000
00000000000000000000000000000000008880088800000088a88aaaaaaa888a88aa88aaaaaaaa99880088088880000880000088800008800000000000000000
00000000000000000000000000000000088800888000000088988999999999998899889999999999880088000888000888000088008808880000000000000000
00000000000000000000000000000000088000880000000088988999999999998899889999999999880088000088000088000088088800880000000000000000
00000000000000000000000000000000888008880000007788088000000000008800880000000077880088000088800088800888088880880000000000000000
00000000000000000000000000000008880088800000007788088000000888008800880000000077880088000008880008800880888880880000000000000000
00000000000000000000000000000088800088000000007788088000008888008800880000000077880088000000880008800880880880888000000000000000
00000000000000000000000000000088000888000000007788088000088888008800880000000077880088000000880008808880880880088000000000000000
0000000000000000000000000000088800888000000077aa889888888880880088008800000077aa889988000000880008808800880888088000000000000000
0000000000000000000000000000888000880000000077aa889888888800880088008800000077aa889988000000880008808808880088088000000000000000
0000000000000000000000000000880008880000000077aa889900000000880088008800000077aa889988000000880008808808888888088000000000000000
0000000000000000000000000008880088800000000077aa889900000000880088008800000077aa889988000000880008888808888888088800000000000000
00000000000000000000000000888000880000000077aaaa88a8888888008800880088000077aaaa88aa88000000880008888000000000008800000000000000
00000000000000000000000000880008880000000077aaaa88a8888888808800880088000077aaaa88aa88000000880008888088888888008800000000000000
00000000000000000000000008880088800000000077aaaa88a8890008888800880088000077aaaa88aa88000000880008888088888888808800000000000000
00000000000000000000000088800088000000000077aaaa88a8890000888800880088000077aaaa88aa88000008880008880088000008808880000000000000
000000000000000000000008880008880000000077aaaaaa88a88a99000888008800880077aaaaaa88aa88990088800088880888000008880880000000000000
000000000000000000000008800088800000000077aaaaaa88a88a99000000008800880077aaaaaa88aa88990088000088880880000000880880000000000000
000000000000000000000088800088000000000077aaaaaa88a88a99000000008800880077aaaaaa88aa88990888000888800880000000880880000000000000
000000000000000000000888000888000000000077aaaaaa88a88a99000088808800880077aa888a88aa88988880000888808880000000880888000000000000
0000000000000000000008800088800000000077aaaaaaaa88a88aaa9908888088008877aaa8888a88aa88888800008888808800000000888088000000000000
0000000000000000000088800888000000000077aaaaaaaa88a88aaa9988888088008877aa88888a88aa88889900008888088800000000088088000000000000
0000000000000000000888000880000000000077aaaaaaaa88a8888888880880880088888888a88a88aaaaaa9900088888088000000000088088800000000000
0000000000000000000880008880000000000077aaaaaaa888a888888880088088008888888aa88888aaaaaa9908888880088800000000888008880000000000
00000000000000000088800888000000000077aaaaaaaa888aaaaaaaaa990888000077aaaaaaa8888aaaaaa88888888800008880000008880000888000000000
00000000000000000888000880000000000077aaaaaa888888888888888888888888888888888888888888888888088888888880000008888888888000000000
00000000000000008880008880000000000077aaaaa888888888888888888888888888888888888888888888aa99088888888880000008888888888000000000
00000000000000008800088800000000000077aaaa888a88aaaaaaaaaa990000000077aaaaaaaaaaaaaaaaaaaa99000000000000000000000000000000000000
000000000000000888000880000000000077aaaaa888a888acccacccaacc99cc00c7aaaaaaaaaaaaaaaaaaaaaaaa990000000000000000000000000000000000
000000000000000880008880000000000077aaaa888aa88aacacaacaacaa9c0c00c7aaaaaaaaaaaaaaaaaaaaaaaa990000000000000000000000000000000000
000000000000000880088888888888888888888888aa888aacccaacaacaa9c0c00c7aaaaaaaaaaaaaaaaaaaaaaaa990000000000000000000000000000000000
00000000000000088008888888888888888888888aaa88aaacaaaacaacaa9c0c0077aaaaaaaaaaaaaaaaaaaaaaaa990000000000000000000000000000000000
0000000000000008800000000000000077aaaaaaaaa888aaacaaacccaaccacc977caaaaaaaaaaaaaaaaaaaaaaaaaaa9900000000000000000000000000000000
0000000000000008800000000000000077aaaaaaaaa88aaaaaaaaaaaaaaaaa9977aaaaaaaaaaaaaaaaaaaaaaaaaaaa9900000000000000000000000000000000
00000000000000088888888888888888888888888888899999999999999999999999999999999999999999999999999900000000000000000000000000000000
00000000000000088888888888888888888888888888999999999999999999999999999999999999999999999999999900000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000

__gff__
0000000000000000000000000000000000000000808c02000081010141034141010101010101010101000101010108080101012001010101010101010101018101010101010101010101010101010101010101010101010101010101010101010101010101410101010101810101810100000000000000000000000000010001
000000000000000000000000000000000020010101010101010000000000000000000000000000c0c041000101010100000000000000c0c0c00000010101000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
__map__
6c6d6f6f6c6d6f6f6f6f6f6c6d6f6c6d6f6f6c57595959586f6f6f6c6d6f6f6f6f6f6c6d6f6c6d6f6f6f6c6d6f6f6f6f6c6d6f6f6f6f6f6c6d6f5a5351545a6d2f2f2f2f2f2f2f2faf27bcbcbcbcbcbcbcbcbcbcbcbc28af000000000000000027acacacacacac2827acacacacacac2827acacacacacac2827acacacacacac28
6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b5a1a1a1a5a6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b5b5959595c6b2f2f2f2f2f2f2f2f27bd0f940f0f0f0f0f0f0f0f0f0fbb280000000000000000372f2f2f2f2f2fab370f960f0f960fab37970f0f0f0f0fab372f2f2f2f0f0fab
686969575958696969696969696969696969695b5959595c69696969696969696969696a7f6869696969696969696969696969696969696a686950515151526a2f2f2f2f2f2f2f2f37940f2f2f2f2f2f2f2f2f2f2f940fab0000000000000000372f2f2f0c2f2fab370f0c0c0c0c0fab370f2f2f2f2f0fbbbd0f0f930f0f0fab
4420215b595c20212021242526032021202104505151515220210806092d0806060102437f44030a010101070993030304010107022e2e43440d5051515152432f2f2f2f2f2f2f2f370f2f2f2f2f2f2f2f2f2f2f2f2f0fab0000000000000000371b1b1b1f1b1bab370f0c0f0f0c0fab370f2f2f2f2f0f0f0f0f0f0f940f2fab
44303153335430232231343536202223223104505151515230310407022d040107050b437f4403030a05930101090303040105050b032e43440d5351335154432f2f2f2f2f2f2f2f370f2f2f2f2f0f0f0f2f940f0f2f0fab000000000000000037970f0f0f0f0fab370f0c0f0f0c0fab370f2f2f2f2f0f0f0f0f2f2f0f0f2fab
4408091e031e20222321323332302322310801535133515420219201022e0401022021437f44080903030401070109030a0b2e2e93032e43440d0d0d0d0d0d436b6b6b6b6b6b6b6b370f2f2f2f2f0fadae2f0f2f0f0f0fab0000000000000000371b1f1f1b1b1bab370f0c0c0c0c0fab370f2f2f2f2f0fadae2f2f2f930f2fab
440401091e0830232231080c080930232104010606010b2022310401022d0a050b303145464d4141414141411841414141420d0d2e2e0d434d4141411841414e7f68696969696a7f370fad3838ae0fab370f0f2f2f2f2fab0000000000000000372f0f0f2f2f2fab370f0f0f0f0f0fab370f0f0f0f0f97ab372f2f0f0f0f2fab
44040101060120222321040c04010130310401070102202231080101022021202120212021484949494949491849494949440d0d0d0d0d43484949491849494a684720212021456a370fabafaf370fab370f2f2f2f2f2fab000000000000000039ae0fad3838383a393838ae0fad383a3938ae0f0fad383a3938ae0f0fad383a
44041616161630313031040c0a050720210a0101010b302321040101023031303130313031484949494949491849494949440d0d0d0d0d43484949491849494a0000000000000000370fbbbcbcbd0fab370f2f2f2f2f2fab000000000000000027bd0fbbacacac2827acacbd0fbbac28af27bd0f0fbb28af27acbd0f0fbbac28
440a0501162021160106050c24252630312e0a050b2e083031041616162e161616161692164b4949494949491849494949440d0d0d0d0d43484949491849494c0000000000000000370f0f1d1d0f0fab370f2f2f2f2f2fab0000000000000000370f0f1f2f0f0fab370f0f0f650f0fab27bd0f0f0f0fbb28370f0f0f0f0f0fab
442a2a2a163031160702a90c34353620210620212e08011616161616162e16161616161616162e16010192010b20212021440d0d039303454708091e0d1e03400000000000000000370f0f1d1d0f0fab370f2f2f2f2f2fab0000000000000000370f0f1f2f2f0fab370f940f0f940fab37920fadae920fab370f0f0f0f0f0fab
44323332070101010101090c32333230310130232193011616162e92162e161616162e2e2e162e161607010b2022313031440d930d030303080107091e9203430000000000000000370f3f0c0c3f0fab370f0f0f0f2f2fab0000000000000000371f0f942f2f2fbbbd0f0f0f0f0f0fab370fad3a39ae0fab371b0f0f1c1c1cab
441d0ca90a050501010c0c0c0c0c2e08060109303104161616162e161692161616162e16169216161616022022310806074d414118414141420a0101060606430000000000000000370f0c0c0c0c0fab372f2f0f0f2f2fab0000000000000000372f2f2f942f0f0c0c0f0f1f1f0f0fab370fbb2827bd0fab37970f0f0f0f0fab
440a0c0c0c0c0c0c0c0c080706092e0a050501069216161616162e161616161616162e921616162021040230310807010b4b4949184949494d429201050501430000000000000000371d0f0f0f0f1dab372f2f0f0f2f2fab0000000000000000372f0f2f2f0f0fadae0f1f0f0f1f0fab37920fbbbd920fab371b0f0f1c1c1cab
4420210c20210a0505050505050b202120210520212021202120212021202120212021161616202231202120210a050b0303032e032021034b440a0b03030a430000000000000000371d1d0f0f1d1dab372f0f0f0f0f2fab0000000000000000372f0f0f2f0f0fab370f1f0f0f1f0fab39ae0f0f0f0fad3a370f0f0f0f0f0fab
4430310c3023212021202120212022232223213031303130313031303130313031302321202130311d302322232120212021202120222321034403030303034300000000000000003938ae0f0fad383a3938ae0f0fad383a0000000000000000393838383838383a3938ae0f0fad383aaf39ae0f0fad3aaf39383838ae0f0fab
4420210c20222322232231303130313031303140414141414141414141414141422022232231202120213031303130232231302322313023214403030303034327ac95acac95ac2827acbd0f0fbbac2827acbd0f0fbbac2827acacacacacac2827acacacacacac2827acbd0f0fbbac2827acbd0f0fbbac2827acacacbd0f0fab
4430310c3031302322314041414141414141413c1515151515151515153d463e44302322310330313031930308091d3031575959595803303144414141184143370f0f0f0f0f0fab373f0f0f0f0f3fab370f0f0f0f0f0fab370f2f940f940fab370fad3838ae0fab370f0f1f1f920fab37970f0f0f0f0fab37940f150f940fab
440a010c060609303108431515151515151515151515151515151515154d414e4420223193030303030303030a010920215b581a575c1e202144151515141543370f0f0f0f0f0fab370f0f0c0c0c0fab370f940f2f2f2fbbbd2fad38ae0f0fab370fab0c0c370fab370c0c0f0f0f0fbbbd0f0f0f0f0f0fbbbd0f0f150f0f0fab
44030a92010701060102453e15151515151515151515151515151515154b914c443023210303202120212021030a0b30311a5b595c1a03303144141414141543370f0f0c0c0f0fab37930f0c0f0c0fab370f0f0f2f0c0c0f0f0fabaf372f2fab370fbb1818bd0fab37920c930f0f0f0c0c640f1b1b0f1f0f0f0f0f150f0f0fab
4403030a05050101010109454646464646464646463e1515151515151515153d4720223108093031303130232193030303555333545503202144151515151543370f0f0c0c0f0fab370f0f0c0c0c0fab370f940f2f0c0c0f0f0fabaf37940fab370f0f0f0f0f0fab370c0c0f930f0fadae0f1c0c1b0f0fadae151515151515ab
4403920303030a01070101060606060606010706094546464646464618464647202223210a930109030809303103030303560303035603303144151514141443370f0f0f0f0f0fab37930f0f0f930fab370f0f0f2f2f2fad38383a27bd2f2fab370f0f0f0f0f0fab370f0f0f0f0f0fab370f0f0f0f0f0fab37151515150f0fab
440d0d0d0d0d0d0401010101010701010101019205060920212021030303032022232223211d0a0b030402202103030303030303030303202144151514151543370f0f0f0f0f0fab373f0f0f930f3fab370f0f0f0f0f0fabaf27acbd0f0f0fab370f0f0f0f0f0fab370f0f0f0f0f0fab370f0f0f0f0f97ab37151515150f0fab
440d0d0d0d0d0d3d4646463e04023d463e0102202120213023223108090303302322232223210320210a0b303103032021202120212021303144141515151543393838ae0cad383a393838383838383a393838ae0fad383aaf370f0f0fad383a3938ae0f0fad383a3938ae0f0fad383a393838383838383a37151515150f0fab
440e0e0e0e0e0e442e40414e0402442e43010730313031202223210401060920223130313031033031030320210303303130232223222321034414151515154327acacbd0fbbac2827acacacacacac2827acacbd0fbbac2827bd0f0f0fbbac2827acbd0f0fbbac2827acbd0f0fbbac2827acacacacacac2837151515150f0fab
470e0e402c0e0e44404e494a04024d424301010101091d303130310407010b30310993030809030303080930312e2e0303033031303130232144141414141443372f2f0f652f2fab370f940f0f940fab372f1b0c0c0c1bab370f0f0f0f0f0fab37970f64640f0fab370f2f1f1f2f0fab37149414149414ab37151515150f0fab
4041414e0e0e0e44434a494c0a0b4b4443414207010b1d080505069201022e0801020308010203030304021d1d1d0809030801092d1d1d303144141414141443372f2f0f0f2f2fbbbd3f0f0f0f0f3fbbbd2f2f1c1c1c2fab370fad38ae0f0fab370f930f0f0f0fab370f2f2f2f2f0fab37143f14143f14ab37151515150f0fab
4349494c0e0e0e4d4e4c0e0e2e2e0e44431e3b414118414220210a01010b2e0401010601050b0320210a93011d08010b030a93011e0109202144151515151543370f0f940f0f0f0f0f0f0f0f0f0f0f0f0f0f2f0c0c0c2fab370fab0c370f0fab370f0f0f940f0fbbbd0f0f0f0f0f0fbbbd141414141414bbbd151594150f0fab
430e0e0e0e0e0e484a0e0e0e0e0e0e4d4e2a2a2a1e0e1e4430232104022e2e040105050b0393033031030a0106050b2021030a022d0402303144151515151543370f0f0f940f0f0f0f0f0c0c0c0c0c0f0f0f2f0f0f0f2fab370fab0f370f0fab370f0f940f0f0f0f0f650f0f0f0f0f0f0f0c0c0c0c0c0c0f0f0f0f15150f0fab
430e0e0e0e0e0e484a0e0e0e0e0e0e484a3233320e0e1e4403303104019206010203039303030320211d1d0a0b031d30311d1d0a2d050b202144151515151543372f2f0f0f2f2fadae0f0c1010100cadae0f2f2f0f2f2fab370fbb18bd0f0fab370f0f0f0f930fadae0f0f0f0f0f0f0f0f0c101010100c0f0f0f0f0f0f0f0fab
3c0e0e0e0e0e0e4b4c0e0e0e0e0e0e4b4c0e0e0e0e0e0e3b4141414141414141420320212021202223211d93031d1d20211d202120211d30314d416e6e6e414e372f2f0f0f2f2fab370f0c1112130cab370f0f0f0f0f0fab370f0f0f0f0f0fab370f0f0f0f0f97ab370f0f0f0f0f0fadae0c101112130cadae0f0f0f0f0f0fab
19191919191919191919191919191919191919191919191919191919191919194420222322232223222321202120213031202223222321202148496e6e6e494a393838383838383a393838382938383a393838383838383a393838383838383a393838383838383a393838383838383a393838382938383a393838383838383a
__sfx__
010300001135114351233511a3511e35123351283512d3512f3510220018200102000520000a0000a0000a0000a00000000000000000000000000000000000000000000000000000000000000000000000000000
000100003a640336402c640256201d6201561024650266502e6503065000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
0001000017570125700d5700857004570005700057000570027000070001500005000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
000200001b6301c6303f6303f6303f6303f6003f6003f6303f6303f6303f6003f6003f6303f6303f6303e6003f60002600066000d600156001a600216002c600366003f600000000000000000000000000000000
01020000056500c650216502b650386503f6502d60033600386003d6003f600000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
000100003b650346502e6502a650236501d650146500965003650076500b65010650166501e65027650336503f6503f6503f6503f6503f6503f6503f6503f6503e6503e6503e6503e6503e650000000000000000
000100000a45008450054500245000450004500245005450094500b45000450014500345006450084500d45013450204501f4501c450174501445000000000000000000000000000000000000000000000000000
000300002455124551245533855238552385533810038100381003810000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
190300000265002650026500265002650026500265000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00010000113500e3500d3500c3500b3500a3500935007350053500235000350003500035000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
4509000037031261312b1311a1311f1310e1311313102131071310e100131000e100131000e100131000e100131000e1000310000100000000000000000000000000000000000000000000000000000000000000
0002000013350103500c3500835003350003500035000350003501c35024350273502b35000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
000200003b350393503635033350303502c350293502635024350213501f3501c350193501735014350113000e3000b3000830006300043000130000300000000000000000000000000000000000000000000000
730e00002a35229352273522135220352283522b35230352303523030000300002000020000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
211e0000290522a0522b0522c0522c0522c0520000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
270402001b25500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
350d00001b6511965116651126510f6550b6531165119651226553025130251302550000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
b70700003567533675306752e67533675306752e6752c675306752e6752c6752a6752e6752c6752a67529675276752467522675276752467522675206751e67522675206751e6751d6751b67518675166751b675
011400001e0521e0521e0521e05222052220522205222052220522205222052220522205222052220522205200000000000000000000000000000000000000000000000000000000000000000000000000000000
011400000a0520a0520a0520a0520d0520d0520d0520d0520d0520d0520d0520d0520d0520d0520d0520d05200000000000000000000000000000000000000000000000000000000000000000000000000000000
011400001605217352183521935219052190521905219052190521905219052190521905219052190521905200000000000000000000000000000000000000000000000000000000000000000000000000000000
2d14000015075090750c075160750907516075090750c07515075090750c075140750907514075090750c07515075090750c075160750907516075090750c07515075090750c075140750907514075090750c075
2d1400001a0750e075110751b0750e0751b0750e075110751a0750e07511075190750e075190750e075110751a0750e075110751b0750e0751b0750e075110751a0750e07511075190750e075190750e07511075
ad1400000e6533f6000e6533f6000e6533f6000e6533f6000e6533f6000e6533f6000e6533f6000e6533f6000e6533f6000e6533f6000e6533f6000e6533f6000e6533f6000e6533f6000e6533f6000e6533f600
3c1400001537215305090051637209005163720900009371153721530509005143720900514372090000937115372153050900516372090051637209000093711537215305090051437209005143720900009371
3d1400001a27215305090051b272090051b272090000e2711a2721530509005192720900519272090000e2711a27215305090051b272090051b272090000e2711a2721530509005192720900519272090000e271
410500001a5551c5551d5551f5551c5551d5551f555215551d5551f55521555225551f5552155522555245552155522555245552655522555245552655528555245552655528555295552655528555295552b555
001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
251400000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
7f1400002b0352e03532035330352b0352e03532035330352b0352e03532035330352b0352e03532035330352a0352d03532035330352a0352d03532035330352a0352d03532035330352a0352d0353203533035
7f140000290352c0353203533035290352c0353203533035290352c0353203533035290352c0353203533035280352b0353203533035280352b0353203533035280352b0353203533035280352b0353203533035
7f140000270352b0353003532035270352b0353003532035270352b0353003532035270352b0353003532035260352b0353003532035260352b0353003532035260352b0353003532035260352b0353003532035
7f141400180351e03521035240351e03521035240352703521035240352703524035270352a035270352a0352d0352a0352d03530035000050000500000000000000000000000000000000000000000000000000
0114000007331073320733207332073320733207335073350a3310a3320a3320a3320e3310e3320e3320e3320d3310d3320d3320d332063310633206332063320633206332063320633206332063320633506333
01140000053310533205332053320533205332053320533205335053350a3310a3320a3320d3310d3320d3320c3310c3320c3320c332043310433204332043320433204332043320433204332043320433104333
0114000003331023310333103332033350333503335033350733107332073320f3310f3320f3320e3310e33202331013310233102332023350233502335023350733107332073320e3310e3320e3320d3310d332
0114140002335063350933506335093350c335093350c3350f3350c3350f3351233515335123350f3350c3350f3350c3350933506335000000000000000000000000000000000000000000000000000000000000
49140000006653f6053f6053f605336433f6053f6053f6053f6053f605006653f605336433f6053f6053f605006653f6053f6053f605336433f6053f6053f6053f6053f6050066500665336433f6053363333633
491400000066500600000000000032643000000000000000000000000000665000003264300000006650066532643000000066500000000000000000000000000000000000000000000000000000000000000000
011000000a0550a0000a0550a0550a055090000a0550a0550a055000000a0550a0550a0550a0550a0550a0550a055000000a0550a0550a055000000a0550a0550a055000000a0550a0550a0550a0550a0550a055
011000000a0550a0000a0550a0550a055090000a0550a0550a055000000a0550a0550a0550a0550a0550a05508055000000805508055080550000008055080550805500000080550805508055080550805508055
01100000060550a000060550605506055090000605506055060550000006055060550605506055060550605505055000000505505055050550000005055050550505500000050550505505055050550505505055
01100000060550a00006055060550605509000060550605506055000000605506055060550605506055060550d055000000d0550d0550d055000000d0550d0550d055000000d0550d0550d0550d0550d0550d055
011000000b0550a0000b0550b0550b055090000b0550b0550b055000000b0550b0550b0550b0550b0550b0550a055000000a0550a0550a055000000a0550a0550a055000000a0550a0550a0550a0550a0550a055
011000000c0550a0000c0550c0550c055150000c0550c0550c0550c0000c0550c0550c0550c0550c0550c05506055000000605506055060550000006055060550605500000060550605506055060550605506055
49100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004653046533c633000000465300000
49100000046530000000000000003c633000000000000000000000000004653000003c633000000000000000046530000000000000003c633000000000000000046030000004653000003c633000003c6333c633
1910000016432164321643216432114311143211432114320000000000164300040016432184321a4321b4321d4311d4321d4321d4320a4210c4220e4220f42211421114221142211422114220f4220e4220c422
1910000016432164321643216432114311143211432114320000000000164300000016432184321a4321b4321d4311d4321d4321d4321d4321d4321d4321d43200000000001d432060001e4321e4001e43220432
191000002243222432224322243222432224322243222432224002240022432004002243222432204321e43220431204321e4001e4321d4311d4321d4321d432004000040000400004001d4311d4321d4321d432
191000001b432004001b4321d4321e4321e4321e4321e432004000040012425124251d4321d4321b4311b4321943200400194321b4321d4311d4321d4321d432004000040011425114251b4321b4321943119432
19100000184320c431184321a4321c4311c4321c4321c432004000040010435104351f4321f4321f4321f4321d431114311d4351d4351d435004001d4351d4351d435114011d4351d4351b4351b4351b4351b435
011000001143500005114350a4351143500005114350a4351143500005114350a435114350a435114350a4351143500005114350a4351143500005114350a4351143500005114350a435114350a435114350a435
411400002d55526555225551f5552b55526555225551f5552a55526555225551f5552b55526555225551f5552b55524555215551d5552955524555215551d5552855524555215551d5552955524555215551d555
4114000029555225551f5551c55528555225551f5551c55527555225551f5551c55528555225551f5551c55528555215551d5551a55526555215551d5551a55525555215551d5551a55526555215551d5551a555
411400002d55526555225551f5552b55526555225551f5552a55526555225551f5552b55526555225551f5552e5552755524555215552d5552755524555215552c5552755524555215552d555275552455521555
411400003055526555225551f5552e55526555225551f5552d55526555225551f5552e55526555225551f5552d555225551f5551c5552b555225551f5551c55529555225551f5551c55528555225551f5551c555
010a00201505015050150002105026050260502600028050280502805000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
010a00200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
611400000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
011400000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
__music__
00 20644044
00 21654344
00 22664344
00 23674344
01 20242844
00 21252844
00 22262844
02 23272944
00 372a3044
01 322b3144
00 322c3144
00 332b3144
00 342d3144
00 352e3144
02 362f3144
01 38427c7e
00 397a7c7f
00 3a7b7d7f
02 3b7a7c7f
00 1213147f
01 15177c7f
00 16177c7f
01 1517187f
02 1617197f
00 1a6c7144
01 386b7144
00 396d7144
00 3a6e7144
02 3b6f7144
)~";

#endif // ZELDA_PICO_CART_H_
