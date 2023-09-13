#13610
o13610 open random load of o13611~
1 c 2
open~
if (%arg% != oyster) then
return 0
else
if (%random.10% == 7) then
return 0
wait 3
osend %actor% When you open the oyster you discover a pearl!
wait 4
osend %actor% The pearl rolls out of the oyster and onto the ground.
oechoaround %actor% %actor.name% drops a pink pearl the size of a pea.
oload obj 13611
else
return 0
end
end
~
#13611
o13611 purge of o13610~
1 g 100
~
wait 3
opurge liveoyster
oload obj 13612
osend %actor% Oops.  You accidentally drop the oyster as you pick up the pearl.
oechoaround %actor% %actor.name% drops a shucked oyster.
~
$~
