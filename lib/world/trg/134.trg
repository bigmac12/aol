#13400
Galiard (M13416) welcome bar trigger~
0 g 100
~
if (%actor.vnum% == -1) then
wait 1s
mechoaround Galiard Galiard looks up as you enter the tavern and scowls slightly.
wait 1s
say Welcome to my tavern!
wait 1s
say You can stay as long as you want too just so long as ye don't be causing any ruckus!
wait 3s
mechoaround Galiard Galiard mutters under his breath 'and ye got enough coin to pay yer way.'
end
~
#13401
Dripping sound trigger of o13400 in r13401~
2 b 5
~
wecho A gentle splash can be heard as a drop of water falls into the bottom of the well.
end
~
#13482
Ioun: Food and drink~
1 j 100
~
osend %actor% %actor.str%
osend %actor% %actor.player.hunger%
~
#13489
m13489 random death load o13485-89~
0 f 100
~
eval objload %random.6%
if (%objload% == 1) then
mload obj 13485
elseif (%objload% == 2) then
mload obj 13486
elseif (%objload% == 3) then
mload obj 13487
elseif (%objload% == 4) then
mload obj 13488
elseif (%objload% == 5) then
mload obj 13489
else
return 0
end
~
#13490
m13490 Go North~
0 e 0
leaves north~
north
~
$~
