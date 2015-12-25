#2600
Shifting sands~
2 g 100
~
if (%actor.vnum% == -1) then
    wait 1s
    wecho The sands shift precariously under your feet.
end
~
#2601
Hermits greets Players~
0 g 100
~
if ((%direction% == east) || (%direction% == north)) and (%actor.vnum% == -1) then
wait 4s
mecho hermit The old hermit stops his work and looks down at you as you approach. 
wait 4s
mecho hermit The hermit says, Well now ... another band of would be adventurers! 
wait 4s
mecho hermit The hermit says, 'I suppose ye be wantin' ta get down into the mine?
wait 4s
mecho hermit The hermit says, 'Only I know the secret of unlockin' that grate, so if ye be wantin in, it will cost ya!
wait 4s
mecho hermit The hermit looks you over, judging your wealth.
wait 10s
mecho hermit The hermit grins mischeviously and says, ' For you ... It will cost ya ten shiny platinum coins!   
end
~
#2602
Strange forms~
2 g 100
~
if (%actor.vnum% == -1) then
   wait 1s
   wecho Strange forms skitter across the ground into the shadows.
end
~
#2603
hermit bribe~
0 m 1
~
if %amount% < 10000
mpurge
mload mob 2699
wait 2s
mecho hermit The hermit snickers slightly and stands there looking at you.
wait 2s
mecho hermit The hermit says, 'I asked for ten platinum. Are you deaf?'
elseif %amount% >= 10000
mpurge
mload mob 2699
wait 5s
mecho hermit The hermit coughs uncontrollably.
wait 5s
mecho hermit The hermit says, 'Oh, you're still here. I guess you be wantin' me to open the grate huh?
mecho hermit The hermit shoves the coins into his robes and moves to unlock the grate.
wait 3s
remove staff
wait 1s
unlock grate
wait 2s
open grate
wait 20s
close grate
wait 2s
lock grate
wait 2s
hold staff
wait 2s
mecho hermit The hermit laughs under his breath.
end
~
#2604
Western Lever Pull~
2 c 100
pull~
if (%actor.vnum% == -1) then
if ("%arg%" != "lever")
wsend %actor% Pull what?
   else
      wsend %actor% You pull down on the lever and it clicks into place.
      wechoaround %actor% %actor.name% pulls on a lever, and it clicks into place.
wait 3s
   wecho The cogs and pullies slowly and ponderously begin to move.
wait 3s
   wecho You hear a deep rumbling from the depths of the elevator shaft.
wait 5s
wecho The elevator slowly comes into view. When it stops, the grate swings open and allows entrance to the elevator.
wdoor 2624 east flags a
wdoor 2673 west flags a
wait 70s
wecho A low rumble can be heard in the distance.
end
~
#2605
Going down door close~
2 g 100
~
if (%actor.vnum% == -1) then
if (%direction% == west)
wait 1
wecho The grate swings shut behind you.
wasound The grate slides shut.
wdoor 2673 west flags abcd
wdoor 2624 east flags abcd
wait 6s
wecho The elevator lurches into motion and begins its slow descent into the depths.
wait 20s
wecho The air begins to get more humid and fetid as you approach the bottom of the shaft.
wait 15s
wecho The elevator slows down slowly as it nears the bottom of the mine.
wait 5s
wecho With a soft thump, the elevator comes to a halt at the bottom of the shaft.
wait 3s
wecho The grate on the eastern side of the elevator slides open with a loud screech.
wdoor 2673 east flags a
wdoor 2625 west flags a
wait 1s
wecho The elevator jolts suddenly and you tumble out!
wteleport all 2625
wait 1s
wasound The elevator door slides shut.
wdoor 2673 east flags abcd
wdoor 2625 west flags abcd
end
~
#2606
Going down final exit~
2 g 100
~
if (%direction% == west) and (%actor.vnum% == -1) then
wait 1
wecho The grate to the elevator closes.
wdoor 2673 east flags abcd
wdoor 2625 west flags abcd
end
~
#2607
Eastern door going up lever pull~
2 c 100
pull~
if (%actor.vnum% == -1) then
if ("%arg%" != "lever")
wsend %actor% Pull what?
else
wsend %actor% You pull down on the lever and it clicks into place.
wechoaround %actor% %actor.name% pulls on a lever, and it clicks into place.
wait 3s
wecho The cogs and pullies slowly and ponderously begin to move.
wait 3s
wecho You hear a deep rumbling from the heights of the elevator shaft.
wait 5s
wecho The elevator slowly comes into view. When it stops, the grate swings open and allows entrance to the elevator.
wdoor 2625 west flags a
wdoor 2679 east flags a
wait 70s
wecho A low rumble can be heard in the distance.
end
~
#2608
elevator going up~
2 g 100
~
if (%actor.vnum% == -1) then
if (%direction% == east)
wait 2s
wecho The grate swings shut behind you.
wasound The grate slides shut.
wdoor 2679 west flags abcd
wdoor 2679 east flags abcd
wait 6s
wecho The elevator lurches into motion and begins its slow ascent to the surface.
wait 20s
wecho The air begins to get less humid and fetid as you approach the top of the shaft.
wait 15s
wecho The elevator slows down slowly as it nears the top of the mine.
wait 5s
wecho With a soft thump, the elevator comes to a halt at the top of the shaft.
wait 3s
wecho The grate on the western side of the elevator slides open with a loud screech.
wdoor 2679 west flags a
wdoor 2624 east flags a
wait 1s
wecho The elevator jolts suddenly and you tumble out!
wteleport all 2624
wait 1s
wasound The elevator door slides shut.
wdoor 2624 east flags abcd
wdoor 2625 west flags abcd
end
~
#2609
Going Up final exit~
2 g 100
~
if (%direction% == east) and (%actor.vnum% == -1) then
wait 10s
wecho The grate to the elevator closes.
wdoor 2624 east flags abcd
wdoor 2679 west flags abcd
end
~
#2610
Creaking Beams~
2 g 100
~
if (%actor.vnum% == -1) then
wait 1s
wecho The beams holding up the roof of the tunnel creak ominously.
end
~
#2612
Secret lever~
2 cg 100
pull~
if (%actorvnum% == -1) then
wait 2s
wsend %actor% You pull on the lever and a deep rumbling can be heard in the distance.
wechoaround %actor% %actor.name% pulls on the lever and a deep rumbling can be heard in the distance.
wait 4s
wecho The ground shudders under your feet.
wdoor 2677 northeast room 2678 flags a
wait 15s
wecho A louding grinding sound can be faintly heard.
wdoor 2677 northeast purge
~
#2613
Chain Pull~
2 c 100
pull~
if (%actor.vnum% == -1) then
if ("%arg%" != "chain")
   wsend %actor% Pull what?
else
wsend %actor% You pull on the chain forcefully.
wechoaround %actor% %actor.name% pulls on the chain forcefully.
wait 2s
wecho A deep rumbling can be heard in the distance, and the ground beneath your feet trembles slightly.
wdoor 2677 east flags a
wdoor 2678 west flags a
wait 17s
wecho The ground shudders again, then all is still.
wdoor 2677 east flags abcd
wdoor 2678 west flags abcd
~
#2614
Drow Load~
2 g 100
~
if (%actor.vnum% == -1)
wait 1
wecho The darkness coalesces into the spirit of a dark elf.
~
#2615
Drow Aggro~
0 g 100
~
if (%actor.vnum% == -1)
wait 4
mkill %actor%
~
#2616
Slug Aggro~
0 g 100
~
if (%actor.vnum% == -1)
msend %actor% You feel compelled to attack this hideous creature!
wait 2
mforce %actor% kill slug
~
#2617
Goo Pulse~
2 g 100
~
if (%actor.vnum% == -1)
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!'
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!'
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!'
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!'
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!'
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!'
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!'
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!'
wait 6s
wecho Acidic slime drips from the ceiling, stinging you!'
end
~
#2618
Dragon Aggro Open chest~
0 c 100
open~
if (%actor.vnum% == -1) then
if ("%arg%" == "chest")
wait 1
msend %actor% The dragon awakens suddenly, and bellows loudly as it sees you attempting to take its treasure.
mechoaround %actor% The dragon awakens and lunges at %actor.name%!
wait 1
mkill %actor%
end
end
~
#2619
Chest Close~
1 c 4
open~
if (%actor.vnum% == -1) then
oat %chest% %close chest%
oat %chest% %lock chest%
oecho This is a test.
~
#2620
Swirling Eddie Greet ATTACK~
0 g 100
~
if (%actor.vnum% == -1)
wait 1s
mkill %actor%
end
~
#2621
Hermit coming up close~
2 g 100
~
if (%direction% == down) then
wait 2
wechoaround hermit The old hermit cackles and says, 'So you survived! Good for you!
wdoor 2626 down flags abcd
wdoor 2621 up flags ab
wait 2
wechoaround hermit The hermit closes and locks the grate.
end
~
#2622
o2608 random load on m2608~
0 f 100
~
 if (%random.5% == 1)
mload obj 2608
end
~
#2623
o2603 random load on m2608~
0 f 100
~
 if (%random.5% == 1)
mload obj 2603
end
~
#2699
Ring Invis~
1 j 100
~
if (%actor.vnum% == -1)
ogive 2800 %actor%
~
$~
