#12000
r12000 Set sail on arrival of m12000~
2 g 100
~
set num %random.2%
if ((%actor.name% == Dvarish) && (%num% == 1))
wait 2s
wforce Dvarish say It's time to cast off! Get to work you slackers!
wdoor 12000 north purge
wait 2s
wecho The crew jump to work and untie the mooring lines.
wait 10s
wecho The ship glides out of the docks
wait 30s
wecho A strong wind catches hold of the sails and the ship moves faster.
wait 150s
wecho Black clouds can be seen on the distant horizon.
wait 150s
wecho The wind picks up making the sails billow out.
wait 150s
wecho Large waves threaten to engulf the ship as it valiantly battles against the sea.
wait 90s
wecho Rain lashes down upon you.
wait 90s
wecho Thunder rumbles in the heavens.
wait 90s
wecho Lightning flashes down from the sky.
wait 90s
wecho The ship creaks and groans under the effort of fighting the storm.
wait 60s
wecho Beams of sunlight break through the cloud cover
wait 90s
wecho The sea begins to calm and the violent motion of the ship lessens.
wait 30s
wforce Dvarish say Land ahoy! Trim those sails ye fools!
wait 30s
wecho The ship slowly glides into the harbour.
wait 10s
wecho The crew tie the mooring lines.
wdoor 12000 south room 1990
wasound A ship arrives at the docks.
wdoor 12000 south purge
wforce Dvarish say Right ye landlubbers get off my ship!
wecho Dvarish climbs down a cargo hatch.
wpurge Dvarish
wteleport all 12001
wload mob 12000
wforce Dvarish get all
wait 1200s
wdoor 12000 north room 12006
wdoor 12000 north description A pier.
wasound A ship arrives at the pier.
wait 5s
wteleport Dvarish 12006
wechoaround Dvarish Dvarish climbs onto the pier.
elseif ((%actor.name% == Dvarish) && (%num% == 2))
wait 2s
wforce Dvarish say It's time to cast off! Get to work you slackers!
wdoor 12000 north purge
wait 2s
wecho The crew jump to work and untie the mooring lines.
wait 10s
wecho The ship glides out of the docks
wait 30s
wecho A strong wind catches hold of the sails and the ship moves faster.
wait 150s
wecho The boards creak gently beneath you.
wait 150s
wecho The boards creak gently beneath you.
wait 150s
wecho The boards creak gently beneath you.
wait 90s
wecho Dolphins swim playfully alongside the ship.
wait 90s
wecho The boards creak gently beneath you.
wait 90s
wecho The boards creak gently beneath you.
wait 60s
wecho The boards creak gently beneath you.
wait 90s
wecho The boards creak gently beneath you.
wait 30s
wforce Dvarish say Land ahoy! Trim those sails ye fools!
wait 30s
wecho The ship slowly glides into the harbour.
wait 10s
wecho The crew tie the mooring lines.
wdoor 12000 south room 1990
wasound A ship arrives at the docks.
wdoor 12000 south purge
wforce Dvarish say Right ye landlubbers get off my ship!
wecho Dvarish climbs down a cargo hatch.
wpurge Dvarish
wteleport all 12001
wload mob 12000
wforce Dvarish get all
wait 1200s
wdoor 12000 north room 12006
wdoor 12000 north description A pier.
wasound A ship arrives at the pier.
wait 5s
wteleport Dvarish 12006
wechoaround Dvarish Dvarish climbs onto the pier.
end
~
#12001
m12000 greets players and offers services~
0 g 100
~
if ((%direction% == north) && (%actor.vnum% == -1)) then
if (%actor.level% >= 10) then
wait 2s
say Good day to you
say I'll be travelling to Palanthas shortly, only 3 platinum pieces if you want passage on me ship
elseif (%actor.level% <= 9) then
wait 1s
say No passage for the like of ye!
msend %actor% Dvarish points at you.
mechoaround %actor% Dvarish points at %actor.name%
end
end
~
#12002
m12000 accept payment and transport player to ship~
0 m 1
~
if (%actor.level% <= 9) then
waits 1s 
say Ye look unlucky, I'll nae have ye on my ship 
elseif ((%amount% < 3000) & (%actor.level% >= 10)) then
say I told ye it's three platinum for passage
elseif ((%amount% >= 3000) & (%actor.level% >= 10)) then 
wait 1s
say Wait on the deck until we sail
mechoaround %actor% %actor.name% boards the ship. 
mteleport %actor% 12000
msend %actor% You board the ship.
mechoaround %actor% %actor.name% boards the ship.
mforce %actor% look 
end
~
#12003
r12006 3min ship departure timer~
2 d 0
wait~
if (%actor.name% == Dvarish)
wait 60s
wforce %actor% say All aboard who's coming aboard, we leave in two minutes!
wasound %actor.name% shouts, 'All aboard who's coming aboard, we leave in two minutes!'
wait 120s
wforce %actor% group all
wforce %actor% ungroup
wdoor 12006 south room 12000
wforce %actor% south
wdoor 12006 south purge
wait 5s
wecho A ship drifts away from the docks.
end
~
#12004
m12000 If ship sinks transport players to sinking boat~
0 i 10
~
wait 145s
mteleport all 12002
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
mteleport all 12003
mteleport %self% 12000
halt
~
#12005
m12002 greets players and offers services~
0 g 100
~
if ((%direction% == south) && (%actor.vnum% == -1)) then
wait 1s
say Greetings my friend!
say Should you wish to travel to Kalaman, I am prepared to take you there for 3 platinum pieces
end
~
#12006
m12002 accept payment and transport player to ship~
0 m 1
~
if (%amount% < 3000) then
say Please do not insult my intelligence
elseif (%amount% >= 3000) then 
wait 1s
say Wait on board, we will be setting sail soon
mechoaround %actor% %actor.name% boards the ship.
mteleport %actor% 12008
msend %actor% You board the ship. 
mechoaround %actor% %actor.name% boards the ship.
mforce %actor% look 
end 
~
#12007
r12007 3min ship departure timer~
2 d 100
Wait~
if (%actor.name% == Forden) 
wait 60s
wforce %actor% say All aboard who's coming aboard, we leave in two minutes!
wasound %actor.name% shouts, 'All aboard who's coming aboard, we leave in two minutes!'
wait 120s
wforce %actor% group all
wforce %actor% ungroup
wdoor 12007 east room 12008
wforce %actor% east
wdoor 12007 east purge 
wait 5s
wecho A ship drifts away from the docks. 
end
~
#12008
m12002 If ship sinks transport players to sinking boat~
0 i 10
~
wait 145s
mteleport all 12010
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
say Now would be a good time to pray to whatever Gods you follow!
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
mteleport all 12003
mteleport %self% 12008
halt
~
#12009
r12008 Set sail on arrival of m12001 ~
2 g 100
~
set num %random.2%
if ((%actor.name% == Forden & (%num% == 1))
wait 2s
wforce Forden say Right lads lets go before the tide changes
wdoor 12008 west purge
wait 2s
wecho The crew jump to work and untie the mooring lines.
wait 10s
wecho The ship glides out of the docks
wait 30s
wecho A strong wind catches hold of the sails and the ship moves faster.
wait 150s
wecho Black clouds can be seen on the distant horizon.
wait 150s
wecho The wind picks up making the sails billow out.
wait 150s
wecho Large waves threaten to engulf the ship as it valiantly battles against the sea.
wait 90s
wecho Rain lashes down upon you.
wait 90s
wecho Thunder rumbles in the heavens.
wait 90s
wecho Lightning flashes down from the sky.
wait 90s
wecho The ship creaks and groans under the effort of fighting the storm.
wait 60s
wecho Beams of sunlight break through the cloud cover
wait 90s
wecho The sea begins to calm and the violent motion of the ship lessens.
wait 30s
wforce Forden say Bosun prepare for docking
wait 30s
wecho The ship slowly glides into the harbour.
wait 10s
wecho The crew tie the mooring lines.
wdoor 12008 south room 9842
wasound A ship arrives at the docks.
wdoor 12008 south purge
wforce Forden say Farewell my friends, I hope you enjoy your stay in Kalaman
wecho Forden climbs down a cargo hatch.
wpurge Forden
wteleport all 12009
wload mob 12002
wforce Forden get all 
wait 1200s
wdoor 12008 west room 12007
wdoor 12000 west description A pier.
wasound A ship arrives at the pier.
wait 5s
wteleport Forden 12007
wechoaround Forden Forden climbs onto the pier. 
elseif ((%actor.name% == Forden) & (%num% == 2))
wait 2s
wforce Forden say Right lads lets go before the tide changes
wdoor 12008 west purge 
wait 2s
wecho The crew jump to work and untie the mooring lines.
wait 10s
wecho The ship glides out of the docks
wait 30s
wecho A strong wind catches hold of the sails and the ship moves faster.
wait 150s
wecho The boards creak gently beneath you.
wait 150s 
wecho The boards creak gently beneath you.
wait 150s 
wecho The boards creak gently beneath you.
wait 90s
wecho Dolphins swim playfully alongside the ship.
wait 90s
wecho The boards creak gently beneath you.
wait 90s
wecho The boards creak gently beneath you.
wait 60s
wecho The boards creak gently beneath you.
wait 90s
wecho The boards creak gently beneath you.
wait 30s
wforce Forden say Bosun prepare for docking
wait 30s
wecho The ship slowly glides into the harbour.
wait 10s
wecho The crew tie the mooring lines.
wdoor 12008 south room 9842
wasound A ship arrives at the docks.
wdoor 12008 south purge
wforce Forden say Farewell my friends, I hope you enjoy your stay in Kalaman
wecho Forden climbs down a cargo hatch.
wpurge Forden
wteleport all 12009
wload mob 12002
wforce Forden get all
wait 1200s
wdoor 12008 west room 12007
wdoor 12000 west description A pier.
wasound A ship arrives at the pier.
wait 5s
wteleport Forden 12007
wechoaround Forden Forden climbs onto the pier.
end
~
$~
