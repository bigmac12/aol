#1
Animal Mob Flee~
0 l 75
~
flee
~
#2
mob abjure ability~
0 e 0
elemental hits~
if (%random.6% == 1) then
emo utters the word, 'ivyecandus'.
emote banishes the elemental back where it came from.
mpurge elemental
end
~
#3
no littering~
2 h 100
~
wsend %actor% You cannot litter here!
return 0
~
#4
Female EQ Wear Trigger~
1 j 100
~
if (%actor.sex% == male) then
wait 2s
osend %actor% You hope your friends don't see you dressed like this...
end
~
#5
call for help~
0 ek 100
hits misses~
mload mob 3060
halt
~
#6
Eric Cartman~
0 d 100
cheesy poofs~
say Yeah I want cheesy poofs!
~
#7
NO_DISARM~
2 c 100
disarm~
if (%actor.vnum% == -1) then
  wsend %actor% A powerful force prevents you from doing that.
~
#8
Antique Dealer Drop~
0 e 100
sells~
say Gimmie a minute, would ya?
wait 4
emote carries the item upstairs and adds it to their collection.
up
drop all
down
wait 6
~
#9
NO_STEAL~
2 c 100
steal~
if (%actor.vnum% == -1) then
  wsend %actor% A powerful force prevents you from doing that.
~
#10
CAST_HEAL~
0 d 100
doheal~
dg_cast 'heal' %victim%
~
#11
closed area trigger~
2 g 100
official~
say Move along.
~
#20
maggot attack!~
0 f 100
~
return 0
mecho hound A swarm of maggots appear from nowhere, ready to comsume everything!
mload mob 10202
end
~
#21
Lunitari~
0 f 100
~
return 0
mecho beetles A swarm of maggots appear from nowhere, ready to consume everything!
mload mob 10202
end
~
$~
