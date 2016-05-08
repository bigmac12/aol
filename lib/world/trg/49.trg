#4900
m4902 Greet~
0 g 100
~
if (%actor.vnum% == -1) then
wait 1s
say Have a seat, if you'd like.  If you're interested in a glass of our specialty, muddy ale, it'll cost you one gold piece.
end
~
#4901
m4902 Drink Service~
0 m 0
~
if %amount% >= 10 then
say Here you are.
wait 2
mload obj 4900
give boot %actor.alias%
elseif %amount% < 10
wait 2
say It's one gold piece for a drink. No less.
end
~
#4902
m4902 Call for Refills~
0 b 10
~
say Refills, anyone?
~
#4903
m4902 Refill~
0 j 100
~
if (%object.vnum% == 4900) then
wait 1s
say Just a moment.
mpurge boot
mload obj 4900
wait 1s
emote collects a few more mugs then goes behind the bar to refill them.
wait 1s
give boot %actor.alias%
else
wait 1s
say Sorry, we only refill our own mugs.  If you'd like one, that'll be one gold piece.
wait 1s
emote begins to walk away but stops a second.
wait 1s
say Oh, and thanks for the gift.
wait 1s
wink %actor.alias%
end
~
#4904
m4921 Vis trigger~
0 h 100
~
wait 3
vis
wait 2s
emote chants some strange, spidery words.
wait 1s
mechoaround dracolich The shadows swirl, spawning an evil presence.
wait 2
mforce hellcat vis
wait 3
mforce hellhound vis
~
#4905
r4978 Flee~
2 g 50
~
If (%actor.vnum% == -1) then
wait 4s
wsend %actor% A feeling of intense panic and fear fills your bones, causing you to flee.
wait 2
wforce %actor% west
~
#4906
m4921 Death Trigger to load o4924~
0 f 100
~
if (%random.10% == 1)
mload obj 4924
end
~
#4907
r4922 Pit Trap~
2 b 100
~
set num %random.3%
if (%num% == 1)
wecho The branches snap underfoot, causing you to nearly lose your footing. 
elseif (%num% == 2)
wecho The leaves underfoot rustle as the branches shift precariously.
elseif (%num% == 3)
wecho The branches underfoot snap loudly as they give way and reveal an open pit below.
wait 2
wecho You begin to slip into the pit, unable to stop your descent.
wteleport all 4977
wforce all look
end
~
#4908
m4921 Dragonfear~
0 l 20
~
msend %actor% You are overcome with dragonfear.
mechoaround %actor% %actor.alias% gets a panicked look.
mforce %actor% flee
~
#4909
m4920 death trigger for o4920, 21, 22, 27, or785~
0 f 100
~
   eval objload %random.30%
   if (%objload% == 1) then
   mload obj 4920
   elseif (%objload% == 2) then
   mload obj 4921
   elseif (%objload% == 3) then
   mload obj 4922
   elseif (%objload% == 4) then
   mload obj 785
   elseif (%objload% == 5) then
   mload obj 4927
   else return 1
   end
~
#4912
m4912 death load o4907~
0 f 100
~
if (%random.100% == 1)
mload obj 4907
end
~
#4955
m4904 attack on crate open~
0 c 100
open~
   if (%actor.vnum% == -1) then
if ("%arg%" == "crate")
wait 1
msend %actor% A well-dressed man stands up from behind the table and lunges toward you!
mechoaround %actor% A well-dressed man stands up from behind the table and lunges toward you %actor.name%!
wait 1
mkill %actor%
end
end
~
#4991
wolfhunt~
2 g 60
~
if (%actor.vnum%==-1)
wait 1s
wecho An emaciated looking deer streaks out of the tall grasses to the south, running hard to the north.
wload mob 4991
wait 1s
wecho A pair of sleek, grey and black wolves burst from the grasses to the south, hot on the trial of the deer.
wload mob 4993
wload mob 4992
wforce timberwolf2 kill deer
wforce timberwolf kill deer
wait 11s
wecho Suddenly aware of your presence, the wolves turn and bolt back into the tall grass, more wary than hungry.
wpurge timberwolf
wpurge timberwolf
end
~
#4992
killdeer~
0 g 100
~
wforce timberwolf kill deer
~
$~
