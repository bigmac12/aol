#11502
m11502 Alarm~
0 g 100
~
if (%actor.align% < 0) then
wait 4
emo eyes widen at your approach.
wait 2
say To arms! Evil is at the gate!
wait 2
mkill %actor%
mload mob 11532
mload mob 11526
mload mob 11526
end
~
#11525
m11525 We're not worthy trigger~
0 c 100
bow~
return 0
if ((%actor.vnum% == -1) & (%actor.align% >500))
if ((%actor.level% > 25) && (%actor.class% == paladin))
wait 2s
say Your arrival was foretold.
wait 4s
remove key
wait 4s
unlock chest
wait 2s
open chest
wait 2s
mechoaround %actor% A bright, holy light spills forth from the chest.
msend %actor% A bright, holy light spills forth from the chest
wait 2s
emo motions toward the chest and says, 'It is yours.'
end
end
~
$~
