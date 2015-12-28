#3351
m3352 death load m3352~
0 f 100
~
mecho Bits Bits and pieces of flesh and bone join together to form an animated being.
mload mob 3352
~
#3352
m3351 Animate Dead Helpers~
0 k 100
~
say AHAHAHAHAHAHAHAHAHAHAH
say HELP!
~
#3353
m3351 death load of o3351/54~
0 f 100
~
if (%random.10% == 1)
mload obj 3351
elseif (%random.10% == 2)
mload obj 3354
end
~
#3365
o3365 poison~
1 c 2
eat~
if (%actor.vnum% == -1) then
eval pain %actor.level%
if (%arg% != flowers) then
return 0
elseif (%actor.level% =< 10) then
wait 5
odamage %actor% %pain%
wait 7
odamage %actor% %pain%
osend %actor% The juice that bursts from the berries as you eat them is delicously sweet.
wait 5
osend %actor% You feel a bit odd... lightheaded.
wait 2
oechoaround %actor% %actor.name% looks alittle dizzy.
wait 5
osend %actor% You feel very strange.. the room seems to spin slightly, and there are so many pretty colors.
oechoaround %actor% %actor.name% begins to sway a bit, their eyes looking alittle unfocused.
wait 10
osend %actor% You are in excruciating pain!
odamage %actor% %pain%
wait 8
osend %actor% You are in excruciating pain!
else
wait 5
odamage %actor% %pain%
wait 7
odamage %actor% %pain%
osend %actor% The juice that bursts from the berries as you eat them is delicously sweet.
wait 5
osend %actor% You feel a bit odd... lightheaded.
wait 2
oechoaround %actor% %actor.name% looks alittle dizzy.
wait 5
osend %actor% You feel very strange.. the room seems to spin slightly, and there are so many pretty colors.
oechoaround %actor% %actor.name% begins to sway a bit, their eyes looking alittle unfocused.
wait 10
osend %actor% You are in excruciating pain!
odamage %actor% 30
wait 8
osend %actor% You are in excruciating pain!
odamage %actor% 60
end
end
~
#3369
m3369 greet and look innocent~
0 g 100
~
if (%actor.vnum% == -1) then
wait 4
emo stands up sharply, turning to face you.
wait 10
emo clears her throught and tries to casually brush the dirt from her hands.
end
~
$~
