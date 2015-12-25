#1420
z14 pull blanket r1420~
2 c 100
pull~
if (%actor.vnum% == -1) then
if (%arg% != blankets) then
wsend %actor% You do not see that here.
else
wait 1s
wechoaround %actor% %actor.name% pulls the rancid blankets back from the bed.
wsend %actor% You pull the rancid covers back from the bed.
wload mob 1407
wecho The leader of the guards rises up from the bed with a scowl on his face as the blankets hit the ground.
wait 1s
wforce 212 scowl
wait 1s
wforce 212 say 'HOW DARE YOU DISTURB MY SLEEP!!!"
wait 2s
wforce 212 kill %actor.alias%
wait 200s
end
~
#1440
m1441 Acceptance of Offerings~
0 c 100
offer~
if (%actor.vnum% == -1) then
wait 1s
say Thank you for your offering.
wait 1s
mecho angelina Angelina gathers the donations.
wait 1s
mpurge
~
#1450
m1450 Trade Trigger~
0 j 100
~
mjunk all
wait 3
emo studies the %object.shortdesc%.
wait 3
say Wow! Is that ever neat.
wait 2
say Hmm.  Which map should I give you in exchange for that?
wait 1s
ponder
say Oh! I have the perfect one!
wait 2
emo shuffles through his many pouches and produces a crumbled piece of paper.
wait 2
say Here you go!
wait 2
set num %random.3%
if (%num% == 1)
mload obj 1450
elseif (%num% == 2)
mload obj 1451
elseif (%num% == 3)
mload obj 11837
endif
give map %actor.alias%
~
#1451
m1450 Greet Trigger~
0 g 100
~
say Hi! I'm so glad you came!
wait 4
say I just plopped down here and started drawing maps!
wait 4
say There are so many interesting things around here to see!
wait 4
say Do you have any neat items that you want to trade for one of my maps?
wait 4
~
#1494
load twisted doorframe~
0 f 100
~
mload obj 28
end
~
$~
