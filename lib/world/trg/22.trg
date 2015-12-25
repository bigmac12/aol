#2200
load eq from list #1~
0 f 100
~
eval loadobj %random.16%
if (%loadobj% ==1) then
mload obj 2202
elseif (%loadobj% ==2) then
mload obj 2209
elseif (%loadobj% ==3) then
mload obj 2245
elseif (%loadobj% ==4) then
mload obj 2249
elseif (%loadobj% ==5) then
mload obj 2251
else return 1
end
~
#2201
load from eq list #2~
0 f 100
~
eval loadobj %random.8%
if (%loadobj% == 1) then
mload obj 2200
elseif (%loadobj% == 2) then
mload obj 2206
elseif (%loadobj% ==3) then
mload obj 2209
elseif (%loadobj% ==4) then
mload obj 2210
elseif (%loadobj% ==5) then
mload obj 2211
else return 1
end
~
#2202
pull statue trigger~
2 c 100
pull~
if (%actor.vnum% == -1) then
if (%arg% != statue) then
wsend %actor% You do not see that here.
else
wait 2s
wechoaround %actor% %actor.name% slowly pulls the statue away from the wall revealing the faint outline of a door.
wsend %actor% You slowly pull the statue away from the wall and discover the faint outline of a door.
wdoor 2265 south flags abcd
wdoor 2265 south key 2208
wdoor 2265 south name door
wdoor 2265 south room 2267 
wait 30s
wecho Slowly the statue slides back into place covering the door once again.
wdoor 2265 south purge
end
end
~
#2203
load key for private temple area~
0 f 100
~
mload obj 2208
end
~
#2287
m2287 Don't You Dare Cringe!!!~
0 c 100
cringe~
if (%actor.vnum% == -1) then
wait 1s
mkill %actor%
end
~
#2290
m2285 Forces the Actors to do This upon Entry~
2 c 100
xyxyxy~
if (%actor.vnum% == -1) then
wait 2s
wecho An odd chill stirs in the air.
wait 4s
wecho You are no longer alone in the darkness!
wload mob 2287
wload mob 2288
wload mob 2289
wload mob 2290
wload mob 2291
wload mob 2292
wload mob 2293
wload mob 2294
wload mob 2295
wload mob 2296
wload mob 2297
wload mob 2298
wload mob 2299
wteleport rift 2296
end
~
#2291
m2285 Presence Greet Trigger~
0 g 100
~
if (%actor.vnum% == -1) then
wait 1s
mforce %actor% xyxyxy
end
~
#2292
m2286 Old Man Farewell (Qwerty) Trigger~
0 c 100
qwerty~
if (%actor.vnum% == -1) then
wait 1s
backpat %actor%
wait 4s
say You have done the best you could, young one.
wait 4s
wave %actor%
wait 2s
say Perhaps you shall return some day?  Ah, I would like that.
wait 4s
emote returns to his position, guarding the passageway to the east.
end
~
#2293
m2286 Greet, Purge, Teleport Trigger~
0 g 100
~
   if (%direction% == west) and (%actor.vnum% == -1) then
wait 1s
   emote raises a gnarled hand in your direction.
   mpurge
   wait 4s
   emote chuckles dryly, starting to cough.
   wait 3s
   say Just makin' sure ye aren't tryin' to cheat.
   wait 3s
   mecho strange While you wonder 'Cheat at what?', it does not appear as though the strange old man will be offering you any further suggestions as to what lays beyond.
wait 2s
msend %actor% The strange old man guides you into the inner chambers.
mechoaround %actor% The strange old man guides %actor.alias% into the inner chambers.
wait 2s
mteleport %actor% 2293
mforce %actor% look
end
~
#2294
r2292 Farewell Trigger~
2 g 100
~
   if (%direction% == east) and (%actor.vnum% == -1) then
   wait 1s
wforce %actor% qwerty
end
~
#2295
m2297 Got the Right One Trigger~
2 c 100
kill~
if (%actor.vnum% == -1) then
if (%arg% != eleventh) then
wsend %actor% Your weapon passes through the air as the spectre vanishes!
wechoaround %actor% %actor.name% swings at a spectre, who blinks out of existence!
wait 3s
wecho The spectres sense your unworthiness and descend upon you to tear you apart!
wait 2s
wforce %actor% cringe
else
wait 2s
wecho Everything seems to freeze, time itself slowing to a crawl!
wait 3s
wecho The room clears, leaving you standing face-to-face with the lone spectre.
wpurge first
wpurge second
wpurge third
wpurge fourth
wpurge fifth
wpurge sixth
wpurge seventh
wpurge eighth
wpurge ninth
wpurge tenth
wpurge twelfth
wpurge thirteenth
end
end
~
$~
