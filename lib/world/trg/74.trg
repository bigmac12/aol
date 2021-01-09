#7400
r7406 nw exit~
2 c 100
bleed~
if (%actor.vnum% == -1) then
wait 1s
wechoaround %actor% %actor.name% begins to bleed profusely.
wait 1s
wecho An intense, fiery heat fills the room as the smell of sulfur swirls in the air.
wait 2s
wecho The stone wall slides open quietly, revealing a dark passageway.
wdoor 7406 west room 7670 flags a
wait 20s
wecho The wall slides back into place.
wdoor 7406 down purge
~
#7404
m7404 death load o7481, o7484, o7488, o7489~
0 f 100
~
eval chance %random.70%
if (%chance% == 1) then  
    mload obj 7477
elseif (%chance% == 2) then  
    mload obj 7478
elseif (%chance% == 3) then  
    mload obj 7479
elseif (%chance% == 4) then  
    mload obj 7480
elseif (%chance% == 4) then  
    mload obj 7481
elseif (%chance% == 5) then  
    mload obj 7482
elseif (%chance% == 6) then  
    mload obj 7483
elseif (%chance% == 7) then  
    mload obj 7484
elseif (%chance% == 8) then  
    mload obj 7485
elseif (%chance% == 9) then  
    mload obj 7486
elseif (%chance% == 10) then  
    mload obj 7487
elseif (%chance% == 11) then  
    mload obj 7488
elseif (%chance% == 12) then  
    mload obj 7489
else  
    return 0
end
~
$~
