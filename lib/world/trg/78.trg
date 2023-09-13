#7800
time test~
2 ab 100
~
wait until 2
wecho the trigger activates
wload mob 7914
end
~
#7801
OakDeath(m7800,15,16,17)~
0 f 100
~
mecho oak The tall oak crashes over in splinters onto the forest floor.
purge %self%
mload obj 7800
return 0
end
~
#7802
BranchDeath(m7802,19,20,21)~
0 f 100
~
    mecho branch The branch falls dead off the nearby tree and thuds to the ground.
   mload obj 7802
   return 0
end
~
#7803
AntDeath(m7813)~
0 f 100
~
   set num %random.3%
      if (%num% == 1) then
      mecho ant The ant emits a high pitched wail as it curls up its legs, summoning another ant.
      mload mob 7813
      return 0
   else
   end
~
#7804
WolfPackRandom(m7808)~
0 b 70
~
   set num %random.3%
      if (%num% == 1)
         wait 2s
         mecho pack The wolves cautiously circle around, eyeing you up and down.
      elseif (%num% == 2)
         wait 2s
         mecho pack The alpha male leans down over his front legs and lets out a quiet snarl.
      else
         wait 2s
         mecho pack The wolves nip and bite at each other, running through the nearby trees.
      end
   end
~
$~
