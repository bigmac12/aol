#2500
m2500 !cleric trigger (NOT USED)~
0 g 100
~
if (%actor.vnum% == -1)
if (%actor.class% != cleric)
wait 4
say This training room is for clerics only.
wait 2s
msend %actor% The trainer helps you back into the hallway.
wait 2s
mforce %actor% n
end
end
~
#2501
Down Entrance r2545~
2 c 100
pray~
if (%actor.vnum% == -1) then
wait 1s
wsend %actor% You begin to pray.
wechoaround %actor% %actor.name% begins to pray.
wait 1s
wecho A bright light fills the room.
wait 3s
wecho The altar slides into the wall, revealing a staircase.
wdoor 2545 down room 2599 flags a
wait 20s
wecho The altar slides back into place.
wdoor 2545 down purge
end
~
$~
