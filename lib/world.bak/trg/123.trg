#12322
m12320 m12307 Load Key o12384~
0 f 100
~
if (%random.50% == 1)
mload obj 12384
end
~
#12324
testingringinvis~
1 j 100
~
dg_cast 'invisibility' %actor%
done
~
#12327
dwarven gateguard greet trigger (by race!) r12327~
0 g 100
~
wait 5
mecho %self% A dwarven gate guard looks in your direction.
eval pcrace %actor.race%
if (%pcrace% == Neidar) then
   msend %actor% The guard nods respectfully to you.
   wait 1
   msend %actor% The gateguard says, 'Greetins friend. Enjoy yer visit.'
elseif (%pcrace% == Hylar) then
   msend %actor% The guard nods respectfully, though he stares at you with hard eyes.
elseif (%pcrace% == Daewar) then
   msend %actor% The guard keeps a close eye on you, staring at you suspiciously.
   wait 1
   msend %actor% The gateguard says, 'Don't you go causin no trouble.'
elseif (%pcrace% == Qualinesti) then
   msend %actor% The gateguard nods to you stiffly.
elseif (%pcrace% == Silvanesti) then
   msend %actor% The gateguard nods stiffly, keeping an eye on you.
elseif (%pcrace% == Kagonesti) then
   msend %actor% The gateguard nods to you stiffly.
elseif (%pcrace% == Kender) then
   msend %actor% The gateguard frowns at you disapprovingly.
   wait 1
   msend %actor% The gateguard says, 'You keep yer hands in yer own pockets while yer here! Ye hear me?'
   wait 1
   msend %actor% The gateguard grumbles about you under his breath.
elseif (%pcrace% == Minotaur) then
   msend %actor% The gateguard frowns deeply, staring at you with cold eyes.
   wait 1
   msend %actor% The gateguard says, 'Ye cause any trouble here beast, and ye won't leave.'
   wait 1
   msend %actor% The gateguard continues to glare at you coldly.
else
   msend %actor% The guard nods at you indifferently.
end
~
#12399
new trigger~
1 j 100
~
Nothing.
~
$~
