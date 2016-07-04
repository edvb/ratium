math.randomseed(os.time())
math.random(); math.random(); math.random()

-- TODO: add weighting depending on number of available tiles in chunk
calc_rarity = function (rarity)
	return math.random(1,100) <= rarity*100
end

place_block = function(b)
	local tries = 0
	repeat
		x = math.random(0, MAX_X-1)
		y = math.random(0, MAX_Y-1)
		if tries > 100000 then return end
		tries = tries + 1
	until is_spawn(x, y, b)
	set_map(x, y, b)
end

place_blocks = function(b)
	local tries = 0
	repeat
		x = math.random(0, MAX_X-1)
		y = math.random(0, MAX_Y-1)
		if tries > 100000 then return end
		tries = tries + 1
	until is_spawn_range(x-1, y-1, 3, 3, b)
	set_map(x, y, b)
	for i = 0,math.random(0,8) do
		pos = math.random(0,8)
		if     pos == 0 then set_map(x+1, y,   b)
		elseif pos == 1 then set_map(x,   y+1, b)
		elseif pos == 2 then set_map(x+1, y+1, b)
		elseif pos == 3 then set_map(x-1, y,   b)
		elseif pos == 4 then set_map(x,   y-1, b)
		elseif pos == 5 then set_map(x-1, y-1, b)
		elseif pos == 6 then set_map(x+1, y-1, b)
		elseif pos == 7 then set_map(x-1, y+1, b)
		end
	end
end

place_room = function(x_0, y_0, wall, floor, doorqty, door, type)
	local len    = math.random(4,10)
	local height = math.random(4,10)
	local tries  = 0
	repeat
		x = math.random(x_0, x_0+16)
		y = math.random(y_0, y_0+16)
		if tries > 100000 then return end
		tries = tries + 1
	until is_spawn_range(x-1, y-1, len+2, height+2, type)

	for i=0,len do -- place walls
		for j=0,height do
			set_map(i+x, j+y, wall)
		end
	end
	for i=1,len-1 do -- place floor
		for j=1,height-1 do
			set_map(i+x, j+y, floor)
		end
	end
	for i=1,doorqty do -- place doors
		pos = math.random(0,3)
		if     pos == 0 then set_map(x+math.random(1,len-1), y,                         door)
		elseif pos == 1 then set_map(x,                      y+math.random(1,height-1), door)
		elseif pos == 2 then set_map(x+math.random(1,len-1), y+height,                  door)
		elseif pos == 3 then set_map(x+len,                  y+math.random(1,height-1), door)
		end
	end
end

place_bld = function(x_0, y_0, bld) -- TODO separate to include x and y args in back end?
	local tries = 0
	repeat
		x = math.random(x_0, x_0+16)
		y = math.random(y_0, y_0+16)
		if tries > 100000 then return end
		tries = tries + 1
	until is_spawn_range(x-1, y-1, #bld.map[1]+2, #bld.map+2, bld.spawntype)
	for i in pairs(bld.map) do
		for j in pairs(bld.map[i]) do
			if bld.map[i][j] ~= 0 then
				set_map(j+x, i+y, bld.map[i][j])
			end
		end
	end
end

for x_0=0,MAX_X-1,16 do
	for y_0=0,MAX_Y-1,16 do
		for i,v in pairs(blds) do
			if calc_rarity(v.rarity) then
				place_bld(x_0, y_0, v)
			end
		end
		if calc_rarity(.5) then place_room(x_0, y_0, 3, 4, math.random(1,3), 8, 14) end -- create dungeons
		if calc_rarity(.2) then place_room(x_0, y_0, 5, 6, math.random(1,2), 8, 14) end -- create houses
	end
end
for i=1,math.random(1, 3) do
	place_blocks(12)
	place_blocks(15)
end
for i=1,math.random(4, 8) do
	place_block(13)
	place_blocks(14)
end
