#4200
Eye of Beholder Load~
0 f 100
~
if (%random.10% == 1)
mload obj 32
end
~
#4220
Keeper Warning~
0 g 100
~
if (%actor.vnum% == -1) then
wait 1s
shiver
wait 1s
say Get out while you still can!
wait 1s
say GET OUT!
end
~
#4237
Eyes Peering from the Pool~
2 g 100
~
if (%actor.vnum% == -1) then
wait 1s
wecho You see a pair of glowing eyes staring out from the pool.
end
~
#4238
m4202 aggro~
0 c 100
look~
if (%actor.vnum% == -1) then
if ("%arg%" == "pool") then
wait 1s
msend %actor% A strange figure reflected in the pool suddenly moves and lunges toward you.
wait 2s
mkill %actor%
end
end
~
#4265
Current SE~
2 g 90
~
if (%actor.vnum% == -1) then
wait 1s
wecho You are swept up in the current and dragged along with the river.
wait 1s
wforce all se
end
~
#4268
Current E~
2 g 90
~
if (%actor.vnum% == -1) then
wait 1s
wecho You are swept up in the current and dragged along with the river.
wait 1s
wforce all e
end
~
$~
