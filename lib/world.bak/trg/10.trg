#1090
r1998 start town trigger~
2 c 1
begin~
if (%actor.level% == 0) then
return 0
else
wechoaround %actor% %actor.alias% goes off to enter the realm.
wait 1
if (%actor.hometown% == 2) then
wsend %actor% You will start in Karthay.
wait 1s
wteleport %actor% 5113
wforce %actor% l
wechoaround %actor% Baeran'ayl welcomes a new visitor to town.
elseif (%actor.hometown% == 1) then
wsend %actor% You will start in Kalaman.
wait 1s
wteleport %actor% 10172
wforce %actor% l
wechoaround %actor% Stanley welcomes a new visitor to town.
end
end
~
#1091
elemental abjure trigger~
0 e 0
elemental hits~
mechoaround elemental Your elemental has been dispelled!
mpurge elemental
~
#1092
Return to Newbie Area from OOC Rooms~
2 g 100
~
if (%actor.level% <= 5) then
wait 3
wsend %actor% You may return to the world by typing 'begin'.
elseif %actor.level% > 30) then
halt
else
return 0
wsend %actor% This area is for newbies only.
end
~
$~
