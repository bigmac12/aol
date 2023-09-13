#8959
Kender Random Load~
0 f 100
~
eval objload %random.22%
if (%objload% == 1) then
mload obj 2019
elseif (%objload% == 2) then
mload obj 13084
elseif (%objload% == 3) then
mload obj 2450
elseif (%objload% == 4) then
mload obj 1326
elseif (%objload% == 5) then
mload obj 11809
elseif (%objload% == 6) then
mload obj 4784
elseif (%objload% == 7) then
mload obj 8514
elseif (%objload% == 8) then
mload obj 55
elseif (%objload% == 9) then
mload obj 10108
elseif (%objload% == 10) then
mload obj 2000
elseif (%objload% == 11) then
mload obj 9701
elseif (%objload% == 12) then
mload obj 8955
elseif (%objload% == 13) then
mload obj 8956
elseif (%objload% == 14) then
mload obj 8957
elseif (%objload% == 15) then
mload obj 8958
elseif (%objload% == 16) then
mload obj 8963
elseif (%objload% == 17) then
mload obj 8964
elseif (%objload% == 18) then
mload obj 8965
elseif (%objload% == 19) then
mload obj 8966
elseif (%objload% == 20) then
mload obj 8967
elseif (%objload% == 21) then
mload obj 8968
else
return 0
end
~
#8975
m8976 bribe trigger~
0 m 0
~
if %amount% >= 100 then
wait 3
say Thank ye.  That'll do nicely.
wait 1s
emo slides down into the pit, pushes aside a pile of brush and leads you through a dark tunnel.
wait 1s
say Don't say I never did anything for ye.
wait 1s
emo disappears back into the shadows.
wait 4
mforce all south
elseif %amount% < 100
wait 2
say The fee is 10 gold coins. No less.
wait 1s
say If you would rather stay there. I've better things ta do wit my time.
end
~
#8976
r8976/78 pit trap~
2 g 100
~
wait 2
set num %random.3%
if (%num% == 1)
wecho The branches snap underfoot, causing you to nearly lose your footing. 
elseif (%num% == 2)
wecho The leaves underfoot rustle as the branches shift precariously.
elseif (%num% == 3)
wecho The branches underfoot snap loudly as they give way and reveal an open pit below.
wait 2
wecho You begin to slip into the pit, unable to stop your descent.
wload mob 8976
wload mob 8975
wteleport all 8975
wait 1s
wechoaround %actor% A sneaky little man leans over the side of the pit.
wsend %actor% A sneaky little man leans over the side of the pit.
wait 2s
wforce %actor% look
wait 2s
wforce redherring say What 'ave we 'ere... my my, looks like ye've got yerself inta a spot, eh?
wait 3s
wechoaround redherring The jaguar stirs, emitting a low growl.
wait 2s
wforce redherring say Oh-ho, she sounds 'ungry don't she? I 'magine ye'll be wantin outta there...
wait 2s
wforce redherring emote grins slyly.
wait 1s
wforce redherring say Well, I just so 'appen ta know how ta get ye out, but it'll cost ye 10 shiny gold coins. No less.
wait 8
wforce redherring emo extends his greedy hand.
end
~
#8979
r8975 creation of door to r8989~
2 d 0
anything for ye.~
wait 3s
wdoor 8975 south room 8989 flags a
wait 2
wpurge redherring
wpurge jaguar
wforce all south
wforce all look
wait 2s
wdoor 8975 south purge
~
#8986
m8987 aggro on dig~
0 e 0
starts to dig industriously~
if (%actor.vnum% == -1) then
wait 1
msend %actor% A short, broad man emerges from the shadows and lunges toward you.
mechoaround %actor% A short, broad man emerges from the shadows and lunges toward %actor.name%!
wait 1
mkill %actor%
end
~
#8987
m8986 deathload of o8987~
0 f 100
~
mload obj 8987
~
#8988
m8988 aggro on chest open~
0 c 100
open~
if (%actor.vnum% == -1) then
if ("%arg%" == "chest")
wait 1
msend %actor% A woman dressed in black emerges from the shadows and lunges toward you.
mechoaround %actor% A woman dressed in black emerges from the shadows and lunges toward %actor.name%!
wait 1
mkill %actor%
end
end
~
#8989
m8988 aggro on dig~
0 e 0
starts to dig industriously~
if (%actor.vnum% == -1) then
wait 1
msend %actor% A woman dressed in black emerges from the shadows and lunges toward you.
mechoaround %actor% A woman dressed in black emerges from the shadows and lunges toward %actor.name%!
wait 1
mkill %actor%
end
~
$~
