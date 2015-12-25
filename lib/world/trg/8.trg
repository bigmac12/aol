#800
m811 - m813 Army travelling north~
0 ai 100
~
wait 30s
north
eval count 0
while count != 10
wait 30s
north
wait 30s
north
wait 30s
north
wait 30s
north
wait 30s
north
wait 30s
north
wait 30s
north
wait 30s
east
wait 30s
east
wait 30s
east
wait 30s
east
wait 30s
north
wait 30s
mteleport self 10171
wait 30s
north
set count (%count% + 1)
done
end
~
#801
r10018 loop army north~
2 ag 100
~
if ((%actor.name% == Knights) & (%num% == 1))
wait 30s
wforce Knights north
end
~
#802
o817 not sure what it does yet~
1 j 100
~
wait 2s
osend %actor.name% OOC: You are wearing a Girdle of Masculinity/Feminity.
osend %actor.name% OOC: Please inform an imm to have your sex and descriptions changed.
end
~
#803
m810 spreading ember~
0 ac 100
look~
mecho ember A tiny ember causes another fire.
mload mob 807
mpurge %self%
end
~
#804
m807 water/fire out~
0 c 100
pour water~
if ("%arg%" != "water")
msend %actor% Throw what?
else
mforce %actor% pour water out
mecho fire A small fire is extinguished.
mpurge %self%
end
~
#807
m807 death~
0 f 100
~
mecho fire A small fire is extinguished.
return 0
end
~
#808
m810 death~
0 f 100
None~
mecho tiny A tiny burning ember ceases to exist.
return 0
end
~
#809
m808 death load m807~
0 f 100
~
mecho blaze A large blaze lessens.
mload mob 807
return 0
end
~
#810
m809 death load m808~
0 f 100
~
mecho roaring A roaring conflagration grows less severe.
mload mob 808
return 0
end
~
#811
m807 fire expands~
0 ab 3
~
mecho The small fire spreads.
Mload mob 808
mpurge %self%
end
~
#812
m808 blaze expands~
0 ab 3
~
mecho blaze A large blaze expands.
mload mob 809
mpurge %self%
end
~
#813
m810 room check~
0 ab 10
~
look fire
end
~
#814
m807 808 809 ember creation~
0 ab 10
~
mecho fire A shower of sparks fly from the fire.
mload mob 810
end
~
#815
m810 random death~
0 ab 5
~
mecho ember A tiny burning ember winks out of existence.
mpurge %self%
end
~
#816
warrant test~
1 g 100
~
if KOS /= %actor.alias%
oecho It works
else
oecho It does not work
end
~
#817
m811 greet char and offer services~
0 g 100
~
if (%direction% == north) and (%actor.vnum% == -1) then
if (%actor.level% >= 10) then
wait 2s
say Good day to you
say I'll be travelling to Palanthas shortly, only 10 platinum pieces if you want passage on me ship
elseif (%actor.level% <= 9) then
wait 2s
say No passage for the like of ye!
msend %actor% Jolek points at you.
mechoaround %actor% Jolek points at %actor.name%
end
end
~
#818
m811 Accept payment and move chat to ship~
0 m 1
~
if (%actor.level% <= 9) then
waits 1s
say Ye look unlucky, I'll nae have ye on my ship
elseif (%amount% < 10000) & (%actor.level% >= 10) then
say I told ye it's ten platinum for passage
elseif (%amount% >= 10000) & (%actor.level% >= 10) then
wait 1s
say Wait on the deck until we sail
mechoaround %actor% %actor.name% boards the ship.
mteleport %actor% 802
msend %actor% You board the ship.
mechoaround %actor% %actor.name% boards the ship.
mforce %actor% look
end
~
#819
r800 final boarding and setting sail~
2 d 0
wait~
if (%actor.name% == Jolek, Captain of the Kalaman Drifter) then
wait 60s
wforce jolek say All aboard who's coming aboard, we leave in two minutes!
wasound Jolek shouts, 'All aboard who's coming aboard, we leave in two minutes!'
wait 120s
wforce jolek group all
wforce jolek ungroup
wdoor 800 south room 802
wforce jolek south
wdoor 800 south purge
wait 5s
wecho A ship drifts away from the docks.
end
~
#820
m811 ship sinks~
0 i 1
~
   wait 145s
mteleport all 808
wait 2s
mecho %self% Black clouds can be seen on the distant horizon.
wait 200s
mecho %self% The wind picks up making the sails billow out.
wait 60s
mecho %self% Large waves threaten to engulf the ship as it valiantly battles against the sea.
wait 60s
mecho %self% Rain lashes down upon you.
wait 30s
mecho %self% Thunder rumbles in the heavens.
wait 10s
   mecho %self% Lightning flashes down from the sky.
wait 30s
mecho %self% The ship creaks and groans under the effort of fighting the storm.
wait 30s
mecho %self% A crewmember shouts, 'ROCKS!'
wait 5s
mecho %self% The sound of splintering wood rings in your ears as the ship lurches sideways.
wait 5s
mecho %self% Cries of, 'Abandon ship!', ring out around you.
wait 5s
mecho %self% As the ship slides beneath the waves you are thrown clear into the icy water.
mteleport all 805
mteleport %self% 802
end
~
#821
m812 write on board~
0 m 1
~
wait 1s
say Thankyou %actor.name%, I will inform the clerks of your deposit.
idea Today %actor.name% deposited %amount%
end
~
#890
o890 send prisoner to jail and force rent~
1 i 100
~
if %actor.alias% == KOS
oforce say It works
else
oforce say It does not work
end
~
#899
TRIGGER VNUM BROKEN DO NOT USE~
1 i 100
~
if ((%actor.vnum% == -1) && (%victim.vnum% ==-1)) then
if ((%actor.class% == knight) && (%actor.align% > 0) then
osend %victim.name% You are under arrest and %actor.name% drags you off to prison.
oteleport %victim.name% 899
oechoaround %actor.name% %victim.name% has been arrested by %actor.name% and taken to prison.
osend %actor.name% You have arrested %victim.name% please report this arrest, with details of the offences, on the crime board.
oforce %victim.name% rent
~
$~
