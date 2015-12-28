#11420
m11423 death load of o11422~
0 f 100
~
 if (%random.10% == 1)
   mload obj 11422
   end
~
#11432
r11432 guard greet~
0 g 100
~
if (%actor.vnum% == -1) then
say you should not be here,I saw you from a ways off.
wait 5
say Best be gone before I have to report you here.
wait 12
emote screams "Intruders Intruders"
wait 12
say I warned you.
end
~
#11435
r11435 wolf growl~
0 g 100
~
if (%actor.vnum% == -1) then
emote growls evilly at you.
end
~
#11437
r11437 commander~
0 g 100
~
if (%actor.vnum% == -1) then
emote sighs.
wait 10
say Well I suppose you want to kill me too.
wait 10
say Better people then you have tried and my hunters are a shout away.
wait 10
say Leave now before I grow angry.
end
~
#11440
farmer~
0 b 70
~
If (%actor.vnum% == -1) then
emote leans his rake against his shoulder.
em whipes the swear off his brow.
wait 1
say Boy this is hard work.
end
~
#11442
scarecrow~
2 c 100
approach~
wsend %actor% As you approach the scarecrow it comes to life!
wait 1
mload mob 11441
~
#11453
m11455 tell about seer~
0 e 0
steps in front of~
wait 2
say If you would like to go in there, you will need a few items from me first.
~
#11454
m11459 Receive Parchment~
0 j 100
~
   wait 2s
   emo studies the parchment then nods to herself as she closes her eyes.
      wait 1s
      emo mutters some words that you cannot understand then raises her head to look up at you.
      wait 1s
      emo says, 'You have your answer,' as she gestures toward the pool.
wait 1s
drop parchment
   mpurge parchment
~
#11455
m11455 ready~
0 d 1
ready~
wait 4
nod
wait 5
say Very well.
wait 1s
emo gestures for the guard to allow you to pass.
wait 1s
wecho The guard steps aside as you pass.
mpurge guard
mforce %actor% north
mload mob 11458
~
#11456
m11450 how~
0 d 100
how help explain~
wait 1
say You must use the blood from a fresh sacrifice to write your question on our parchment.
wait 1s
say Simply write a yes or no question.
wait 1s
say When you enter, say nothing.  Give the parchment to the seer only when asked.
wait 2s
say Let me know when you have written your question and are ready to go in.
wait 1s
emo bows slightly, then returns to her work.
~
#11457
r11459 Load new pool~
2 h 100
~
if (%actor.name% == Hapaloseus) then
if (%object.vnum% == 11450) then
   wpurge pool
   eval answer %random.6%
   eval npool  11452
   if (%answer% == 1) then
      eval npool 11453
   elseif (%answer% == 2) then
      eval npool 11454
   elseif (%answer% == 3) then
      eval npool 11455
   elseif (%answer% == 4) then
      eval npool 11456
   elseif (%answer% == 5) then
      eval npool 11457
   end
   wload obj %npool%
   wait 7s
   wpurge pool
wecho The pool reverts to its normal state.
   wload obj 11459
end
end
~
#11458
NOT USED~
1 j 0
~
return 0
oechoaround %actor% %actor.alias% lights a stick of incense.
osend %actor% You light a stick of incense.
wait 1s
oecho The scent of incense swirls in the air.
wait 3s
oecho The incense burns out.
~
#11459
m11459 Greet~
0 g 100
answer~
wait 5
emo turns slowly toward you without saying a word.
wait 1s
emo moves to sit upon her tripod.
wait 1s
emo keeps her eyes focused upon the still waters of the pond as she extends a hand in your direction.
wait 5
say The parchment, please.
~
$~
