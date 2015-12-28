#8000
Noble moving north and south~
0 b 30
~
if (%random.2% == 1) then
  north
else
  south
end
~
#8001
m8019 Greet Trigger~
0 g 40
~
if (%actor.vnum% == -1) then
wait 1s
Emote jiggles a cup with a few coins in it hopefully.
end
~
#8002
m8019 bribe trigger~
0 m 1
~
wait 1s
say Thank you for your kindness!
~
#8003
Bump the spectator~
0 g 5
~
if (%actor.vnum% == -1)
wait 1s
msend %actor% You accidentally bump into the spectator and he flies into a fit o
f rage.
mechoaround %actor% %actor% bumps into a spectator who growls loudly and throws 
a punch.
wait 1s
mkill %actor%
end
~
#8096
No Disassemble Head Priest!~
2 g 100
~
if (%actor.vnum%==-1)
wait 1s
wforce head emote looks up from his text in alarm, frowning at the interruption to his studies.
wait 1s
wforce head say What are you doing here?  I do not take visitors here.  Kindly be off with you.
wforce head emote waves his hand dismissively at you.
wait 2s
wforce head frown
wait 1s
wforce head say Very well....
wait 1s
wforce head say Guards! 
wait 1s
wforce head say To my chambers, now!  Remove these vagrants at once!
wait 1s
wecho The sound of a great commotion comes from the halls without this chamber, as though many mailed feet hustle to heed the Head Priest's summons.
wait 1s
wecho Several guards force their way into the chamber, lead by a holy templar in the service of the Temple, intersecting you from the Head Priest.
wload mob 8095
wload mob 8094
wload mob 8094
wload mob 8094
wload mob 8094
wait 1s
wforce head say I think you will be leaving now, yes?
wecho The guards move slowly yet deliberately and with a single purpose, easily forcing you to return to the outer halls.
wpurge head1
wpurge paladin1
wpurge guard1
wpurge guard1
wpurge guard1
wpurge guard1
wdoor 8096 east room 8095 flags a
wforce all east
wdoor 8096 east purge
wload mob 8096
end
~
$~
