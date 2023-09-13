#6500
thief-area-entrance-door~
2 d 0
fish soup~
   if (%actor.vnum% == -1) then
   wait 3s
   wecho From behind a wall you hear someone say, "C'mon in."
   wait 3s
   wsend %actor% You can hear someone grunting and the grinding of gears.
   wechoaround %actor% You can hear some grunting and gears grinding as %actor.name% speaks the phrase.
   wait 2 s
   wecho A hidden panel in the wall slowly rolls open.
   wasound You hear the sound of grinding gears in the distance.
   wdoor 6531 south flags a
   wdoor 6566 north flags a
   wait 15 s
   wdoor 6531 south flags abcd
   wdoor 6566 north flags abd
   end
~
#6501
leaving-thief-area-locks-door~
2 g 100
open stone~
 if ((%actor.vnum% == -1) & (%direction == south)) then
      wait 10s
      wdoor 6531 south flags abcd
      wdoor 6566 north flags abd
else
end
~
#6541
m6541 Random Actions~
0 b 15
~
set num %random.3%
if (%num% == 1)
wait 1s
mecho hok Whew, did something die in here? Oh.. it's only a goblin.
wait 4s
mecho hok The runty little goblin begins choking on some scrap, making a rather worrisome sound. "Hok... HOK."
wait 4s
mecho hok The goblin clears the item from his throat suddenly, shooting a piece of chewed up beef across the room.
elseif (%num% == 2)
wait 1s
mecho hok A runty little goblin crawls about your feet, looking for scraps.
wait 4s
mecho hok The goblin disappears under a nearby table.
wait 4s
mecho hok The goblin emerges from under the table, chewing on someones shoe.
elseif (%num% == 3)
wait 1s
mecho hok A runty little goblin crawls about the barmaids legs, who promptly sends a string of curses his way.
wait 3s
mecho hok Finished with her tirade, the barmaid turns to continue whatever task she was at before being interrupted.
end
end
~
#6542
barmaid greet~
0 g 100
~
if (%actor.vnum% == -1) then
wait 1s
say Have a seat, if you'd like. If you're interested in a bottle of the usual, it'll cost you six coppers.
end
~
#6543
barmaid bribe~
0 m 0
~
if %amount% >= 6 then
say Here you are.
wait 2
mload obj 6541
give bottle %actor.alias%
elseif %amount% < 6
wait 2
say It's six copper for a drink. No less.
end
~
#6544
barmaid receive~
0 j 100
~
if (%object.vnum% == 6541) then
wait 1s
say Just a moment.
mpurge bottle
mload obj 6541
wait 1s
emote collects a few other bottles, heading behind the bar to refill them.
wait 1s
give bottle %actor.alias%
else
wait 1s
say Sorry, we only refill our own bottles. If you'd like one, that'll be six copper.
wait 1s
end
~
#6565
m6565 deathload~
0 f 100
~
eval objload %random.10%
if (%objload% == 3) then
mload obj 6567
else
end
~
#6566
m6566 deathload~
0 f 100
~
eval objload %random.3%
if (%objload% == 1) then
mload obj 6569
else
end
~
#6567
m6567 Deathload o6572~
0 f 100
~
eval objload %random.3%
if (%objload% == 1) then
mload obj 6572
else
end
~
#6570
m6522 Snore~
0 b 15
~
wait 3s
snore
wait 3s
end
~
$~
