#4700
In the Inn~
2 b 1
~
wecho A butler walks in from a back room, and begins to water the plants.
wait 1
wecho He straightens the signs and replaces the water in the washing bowl.
wait 2
wecho Giving a disapproving look, he then heads to the other rooms to tidy them up.
end
~
#4713
Birds at the Well~
2 b 1
~
wecho A few birds swoop down to the well for a quick dip and drink before flying off.
end
~
#4714
A Bird at the Well~
2 b 1
~
wecho A small red feathered bird glides toward the well as if stopping for a drink.
wecho The bird drops a shiny, glittering object into the well, and trills a song.
wecho The bird dips his head into the water, taking a drink before flying off.
end
~
#4718
Sirrion's Bonfire~
2 bg 20
~
wecho The flames within the bowl leap into the air with crackling intensity and heat.
~
#4719
Sirrion's Vision~
2 b 5
~
wecho The shimmering heat above the bowl creates a visionary figure dressed in flame.
wecho The figure spreads his hands in a gesture before disappearing within the fire.
end
~
#4724
Guardian of the Magic Shop~
0 b 1
~
wecho You hear an odd, low pitched purring growl, and the rustling of wings, coming from the shadow upon the shelf as it moves from one place to another.
~
#4725
Jonyam's Boast~
2 gi 50
~
if (%actor.vnum% == -1) then
wait 1
wsend %actor% The burly owner of the Armorer shop greets you in friendly fashion.
wsend %actor%  "Greetings, greetings to you!" he says.  "Come on in!  I have the finest in armor you should ever need!"
wsend %actor% He waves his hands around in a grand gesture.  "Come! Look! Browse!  Hear tales of my travels!"
end
~
#4727
Jonyam's Tale I~
0 b 100
~
msay Ahh, yes.  Only the best I sell.  Only the best I make as well.  After all, I traveled with the heroes of my time, and learned from master crasftmen!
wait 1
msay I saw it my duty to help this quaint little town by staying here, and setting up shop, and trade my items for a low, low price, so people here could afford such luxuries.
wait 2
msay Nothing like the heart of a lion!  And I certainly have that!  And the bravery of a lion as well!  But I have to make a living somehow, so I can't give as freely as some people think I should.
msay I may have royal blood in me, but that does not mean I can eat for free!
~
#4728
Magic Shop Entrance II~
2 g 40
~
if (%actor.vnum% == -1) then
wait 1
wsend %actor% The smoke from burning incense drifts about the room in maddening swirls from the opening door.
end
~
#4729
Magic Shop Entrance I~
2 g 60
~
if (%actor.vnum% == -1) then
wait 1
wsend %actor% The opening door causes the candles to flicker wildly, creating odd dancing shadows along the walls.
end
~
#4730
Chimes ring~
2 g 80
~
if (%actor.vnum% == -1) then
wait 1
wsend %actor% The wind from the opening door causes the multi-colored glass chimes to ring.
wsend %actor% The slim man looks up from examining a gemstone and smiles at you.
end
~
#4740
postal children~
2 b 5
~
wecho Three children run in and out of the room wildly, chasing each other.
wecho One growls fiercely in between fits of laughter as he chases the other two.
wecho A fourth child, smudged in charcoal waves her hands around shouting, 'groos!'
wait 1
wecho The children, after a stern look from the postmistress, run into the back room.
end
~
#4741
postal children II~
2 b 10
~
wecho A childish scream, a crash, and giggles can be heard from the back room.
wecho The postmistress sighs as she thinks of the damage done to her kitchen.
end
~
#4742
postal children III~
2 b 5
~
wecho A small child, smeared in jelly, runs into the room.
wecho A second child, brandishing a wooden spoon, starts to chase the first.
wecho They both run into the back room, pretending to battle a large monster.
end
~
#4743
postal children IV~
2 b 5
~
wecho A small child, unnoticed, walks in with pieces of colored charcoal.
wecho He sits down in a corner, and begins to draw pictures on the mailboxes.
wait 2
wecho A second child sneaks from the back room and tackles the first.
wecho The second child laughs as he runs away from the first, into the back room.
end
~
#4744
postal children V~
2 b 10
~
wecho The postmistress grabs a child as he runs into the back room.
wecho The child giggles, knowing he was caught, and now pretends he is in a dungeon.
wecho The child sits in a corner for a few minutes and helps sort mail.
end
~
#4745
m4779 Random Potion~
0 b 8
~
   if (%potionamt% > 0)
   eval potionamt (%potionamt% - 1)
   global potionamt
   end
~
#4746
m4779 Greet~
0 g 100
~
   wait 2s
   if (%potionamt% > 2)
   em looks up from her holy text, examining you with her cool, azure eyes.
   wait 2s
   em smiles at you warmly.
   wait 1s
   say Welcome to my home.
   wait 3s
   say If you've come for a potion, I'm sorry but I'm brewing some at the moment. If you come back later, I should have a few.
   wait 3s
   say Though if you would like to learn about my goddess Mishakal, or some of the other deities in the pantheon of light, I would be happy to teach you.
   wait 2s
   em smiles to you once more, then returns to her studies.
   else
   em looks up from her holy text, examining you with her cool, azure eyes.
   wait 2s
   em smiles at you warmly.
   wait 1s
   say Welcome to my home.
   wait 2s
   say If you've come for some potions, simply ask, and I shall see what I have. I ask in return, only that you offer your thanks to the Blue Lady, Mishakal for her aid.
   wait 4s
   say If you would like to learn about my goddess Mishakal, or some of the other deities in the pantheon of light, I would be happy to teach you.
   wait 2s
   em smiles to you once more, then returns to her studies, reaching down to pet the dog occasionally.
   end
~
#4747
m4779 Gives Potions~
0 d 1
potion vial healing heal~
wait 1s
if (%potionamt% > 1)
say I'm sorry. I've given out all the cures I have for now.
halt
end
eval dummy %random.7%
if (%dummy% == 1) 
say Ah yes, I have something here.
mload obj 4742
eval potionamt (%potionamt% + 1)
global potionamt
wait 2s
give vial %actor.alias%
wait 1s
say There you are. I hope that helps.
wait 2s
em smiles warmly and returns to her work.
elseif (%dummy% == 2) 
say Ah yes, I have something here.
mload obj 4742
eval potionamt (%potionamt% + 1)
global potionamt
wait 2s
give vial %actor.alias%
wait 1s
say There you are. I hope that helps.
wait 2s
em smiles warmly and returns to her work.
elseif (%dummy% == 3)
say Oh yes.. I believe I have something here, just a moment.
mload obj 4744
eval potionamt (%potionamt% + 1)
global potionamt
wait 2s
give bottle %actor.alias%
wait 2s
say I hope that helps.
wait 1s
emote smiles and returns to work.
elseif (%dummy% == 4)
say I'm dearly sorry.. I've run out of cures. Please wait a few hours, and I'm sure I'll have something ready.
wait 1s
eval potionamt 3
global potionamt
else
say Oh? Alright, let me have a look.
mload obj 4743
eval potionamt (%potionamt% + 1)
global potionamt
wait 2s
give vial %actor.alias%
wait 1s
say There you are. I hope that helps.
end
end
~
#4748
r4761, r4767 Spectral Man/Woman Loader~
2 b 100
~
   wait until 2100
   wecho An unnatural chill creeps into the air, giving you goosebumps.
   wait 10s
   wecho A strange mist seeps from the ground, covering the graveyard.
   wait until 2200
   wecho &WA bone-chilling wail echoes through the graveyard!&n
   wait 5s
   wecho &KThe ground all around you begins to writhe under the mist.&n
   wait 2s
   wecho &KShadowy forms begin to rise from the mist, bringing the scent of decay with them.&n
   wait 2s
   wecho &KThe undead have risen, staring at you with &rburning&K hatred!&n
   wload mob 4764
   wload mob 4765
   wait until 2300
   wload mob 4765
   wait until 0000
   wload mob 4764
   wait until 0100
   wload mob 4765
   wait until 0200
   wload mob 4764
   wait until 0300
   wload mob 4765
   wait until 0400
   wload mob 4764
   wait until 0500
   wecho &WThe undead freeze in the sunlight, letting out a dreadful wail in unison.&n
   wait 2s
   wecho &WThe undead crumble where they stand, the dust blowing away in the breeze.&n
   wpurge
   wait 2s
   wpurge
   wait 1s
   wpurge
   wait 1s
   wpurge
   wait 1s
   wpurge
   halt
~
#4749
r4763, r4769 Skeleton/Zombie Loader~
2 b 100
~
   wait until 2100
   wecho An unnatural chill creeps into the air, giving you goosebumps.
   wait 10s
   wecho A strange mist seeps from the ground, covering the graveyard.
   wait until 2200
   wecho &WA bone-chilling wail echoes through the graveyard!&n
   wait 5s
   wecho &KThe ground all around you begins to writhe under the mist.&n
   wait 2s
   wecho &KShadowy forms begin to rise from the mist, bringing the foul scent of decay.&n
   wait 2s
   wecho &KThe undead have risen, staring at you with &rburning&K hatred!&n
   wload mob 4766
   wload mob 4762
   wait until 2300
   wload mob 4766
 wait until 0000
   wload mob 4762
   wait until 0100
   wload mob 4766
 wait until 0200
   wload mob 4762
   wait until 0300
   wload mob 4766
   wait until 0400
   wload mob 4762
   wait until 0500
   wecho &WThe undead freeze in the sunlight, letting out a dreadful wail in unison.&n
   wait 2s
   wecho &WThe undead crumble where they stand, the dust blowing away in the breeze.&n
   wpurge
   wait 2s
   wpurge
   wait 1s
   wpurge
   wait 1s
   wpurge
   wait 1s
   wpurge
   halt
~
#4750
r4773 Ghast Loader~
2 b 100
~
   wait until 2100
   wecho An unnatural chill creeps into the air, giving you goosebumps.
   wait 10s
   wecho A strange mist seeps from the ground, covering the graveyard.
   wait until 2200
   wecho &WA bone-chilling wail echoes through the graveyard!&n
   wait 5s
   wecho &KThe ground all around you begins to writhe under the mist.&n
   wait 2s
   wecho &KShadowy forms begin to rise from the mist, bringing the foul scent of decay.&n
   wait 2s
   wecho &KThe undead have risen, staring at you with &rburning&K hatred!&n
   wload mob 4760
   wload mob 4761
   wait until 2300
   wload mob 4761
   wait until 0000
   wload mob 4761
   wait until 0100
   wload mob 4761
   wait until 0200
   wload mob 4761
   wait until 0300
   wload mob 4761
   wait until 0400
 wload mob 4761
   wait until 0500
   wait 2s
   wecho &WThe undead freeze in the sunlight, letting out a dreadful wail in unison.&n
   wait 2s
   wecho &WThe undead crumble where they stand, the dust blowing away in the breeze.&n
   wpurge
   wait 2s
   wpurge
   wait 1s
   wpurge
   wait 1s
   wpurge
   wait 1s
   wpurge
   halt
~
#4751
r4775 Ghoul Loader~
2 b 100
~
   wait until 2100
   wecho An unnatural chill creeps into the air, giving you goosebumps.
   wait 10s
   wecho A strange mist seeps from the ground, covering the graveyard.
   wait until 2200
   wecho &WA bone-chilling wail echoes through the graveyard!&n
   wait 5s
   wecho &KThe ground all around you begins to writhe under the mist.&n
   wait 2s
   wecho &KShadowy forms begin to rise from the mist, bringing the foul scent of decay.&n
   wait 2s
   wecho &KThe undead have risen, staring at you with &rburning&K hatred!&n
   wload mob 4760
   wload mob 4761
   wait until 2300
   wload mob 4760
   wait until 0000
   wload mob 4760
   wait until 0100
   wload mob 4760
   wait until 0200
   wload mob 4760
   wait until 0300
   wload mob 4760
   wait until 0400
   wload mob 4760
   wait until 0500
   wait 2s
   wecho &WThe undead freeze in the sunlight, letting out a dreadful wail in unison.&n
   wait 2s
   wecho &WThe undead crumble where they stand, the dust blowing away in the breeze.&n
   wpurge
   wait 2s
   wpurge
   wait 1s
   wpurge
   wait 1s
   wpurge
   wait 1s
   wpurge
   halt
~
#4752
Undead Loader/Purger~
2 b 100
~
   wait until 2100
   wecho An unnatural chill creeps into the air, giving you goosebumps.
   wait 10s
   wecho A strange mist seeps from the ground, covering the graveyard.
   wait until 2200
   wecho &WA bone-chilling wail echoes through the graveyard!&n
   wait 5s
   wecho &KThe ground all around you begins to writhe under the mist.&n
   wait 2s
   wecho &KShadowy forms begin to rise from the mist, bringing the foul scent of decay.&n
   wait 2s
   wecho &KThe undead have risen, staring at you with &rburning&K hatred!&n
   wload mob 4762
   wload mob 4764
   wait until 0500
   wait 2s
   wecho &WThe undead freeze in the sunlight, letting out a dreadful wail in unison.&n
   wait 2s
   wecho &WThe undead crumble where they stand, the dust blowing away in the breeze.&n
   wpurge
   wait 2s
   wpurge
   wait 1s
   wpurge
   wait 1s
   wpurge
   wait 1s
   wpurge
   halt
~
#4753
m4779 Mish Speech~
0 d 0
mishakal~
   wait 4s
   em looks up from her holy text, smiling warmly.
   wait 4s
   say So you'd like to know about my goddess, Mishakal?
   wait 4s
   em places the text to one side, and folds her hands in her lap.
   wait 4s
   em pauses for a moment, sinking into thought.
   wait 6s
   say Mishakal is the healing hand, revered throughout Ansalon as the "Healer" and "Light Bringer". In certain parts of Solamnia, she is known as "The Blue Lady", or "Skymistress".
   wait 6s
   say Among the elves, she is known as "Quenesti Pah", and "Quen Illumini". The Ergothians call her "Mishas".
   wait 6s
   say Mishakal's symbol is the infinity sign, or the figure eight. Her color is sky blue.
   wait 6s
   say She represents the ideals of healing, mercy, fertility, and beauty.
   wait 4s
   say She is gentle and compassionate. She seeks to heal ills and minister to all creatures through her servants on Krynn.
   wait 6s
   say Mishakal is Paladine's wife and advisor. They are the parents of the twin gods, Kiri-Jolith and Habbakuk, and of Solinari.
   wait 6s
   say The worshipers of Mishakal diametrically oppose those who follow the evil god Morgion.
   wait 4s
   em frowns deeply.
   wait 4s
   say Worshipers of Morgion seek to spread disease and decay, which are anathema to the precepts of Mishakal's healing hands.
   wait 6s
   say The Dogma of Mishakal's faithful is thus:
   wait 3s
   say Heal all who require it, even if they are your enemies. Drive out disease, cut away decay, and bring light to those who live in darkness and ignorance.
   wait 4s
   say Heal the wounded land, promote harmony and peace, and lend help where it is needed- particularly to those who cannot help themselves.
   wait 6s
   say Do not raise weapons in anger, but defend yourself and those whose lives are in danger. Honor those who have passed on to a better life, but remember that your duties lie with those who are left behind.
   wait 6s
   say Comfort their pain, bandage their wounds, show them compassion, and ease the sorrow in their hearts.
  wait 5s
   em sits back in her chair, thoughtful for a moment.
   wait 6s
   say I hope you have learned something from this.
   wait 4s
   em smiles warmly to you before retrieving her text and sinking into her thoughts.
   halt
~
#4754
m4779 Gods Speech~
0 d 1
Habbakuk Solinari Kiri-Jolith Paladine Morgion bias biased~
   if (%speech% /= Habbakuk) then
   wait 2s
   em looks up from her text, pausing a moment to gather her thoughts.
   wait 5s
   say Habbakuk, also known as "the Fisher King", or "the Blue Phoenix", created and rules all creatures of the land and sea. His domains are animals, water and passion.
   wait 7s
   say His colors are deep blue, and white. His symbol is the Blue Phoenix, representing rebirth and the cycle of life.
   wait 5s
   say Habbakuk is the middle son of Paladine and Mishakal, and the twin brother of Kiri-Jolith. Rangers and sailors revere him, as do the Knights of Solamnia, especially the Order of the Crown, of which he is a patron.
   wait 7s
   em smiles warmly to you, then returns to her text.
   elseif (%speech% == Paladine) then
   wait 2s
   em looks up from her text, pausing a moment to gather her thoughts.
   wait 5s
   say Paladine is the father of good, and the master of law. He is also known as "Draco Paladin" in Ergoth, and by the Knights of Solamnia. He is known as "E'li" by the elves, and "Bah'mut" by the Istarians.
   wait 7s
   say His constellation is the Platinum Dragon, and his colors are white and silver.
   wait 5s
   say Paladine is husband to Mishakal, and father to the Habbakuk, Kiri-Jolith, and Solinari. Paladins and warriors who embrace good revere Paladine, as do the Knights of Solamnia, especially the Order of the Rose, to which he is a patron.
   wait 7s
   say Paladine upholds the laws of Good, assisting the righteous and abjuring evil.
   wait 4s
   em smiles warmly to you, then returns to her text.
   elseif (%speech% /= Kiri) then
   wait 2s
   em looks up from her text, pausing a moment to gather her thoughts.
   wait 5s
   say Kiri-Jolith is the deity of glory, honor, obedience, justice, and righteous warfare. He is also known as the "Sword of Justice", or "Corij" in Ergoth.
   wait 7s
   say It is rumored that a small faction of the minotaur race worships Kiri-Jolith for his principles as well, and they refer to him as "Emperor", a term which is said to gain them emnity from their Sargonnas-worshipping brethren.
   wait 7s
   say Kiri-Jolith is the eldest son of Paladine and Mishakal, and the twin brother of Habbakuk. Paladins and fighters who embrace good revere Kiri-Jolith, as do the Knights of Solamnia, especially the Order of the Sword, of which he is a patron.
   wait 8s
   say Kiri-Jolith's symbol is a bison's horn, or a horned battle-axe. His colors are brown and white.
   wait 4s
   em smiles warmly to you, then returns to her text.
   elseif (%speech% == Solinari) then
   wait 2s
   em looks up from her text, pausing a moment to gather her thoughts.
   wait 5s
   say Solinari is the hand of white magic, the patron deity of all the White Robe Wizards of High Sorcery. He is also known as "the Mighty Hand", or "Solin".
   wait 5s
   say His colors are white or silver, and his symbol is the white moon which caries his name.
   wait 4s
   say Unlike the other gods, Solinari does not have a clerical order dedicated to his worship. The White Mages of High Sorcery are his dedicated.
   wait 4s
   say I'm sorry, but I don't know much more about Solinari.. you should seek a Mage, they may be able to tell you more.
   wait 3s
   em smiles warmly to you, then returns to her text.
   elseif (%speech% == Morgion) then
   wait 2s
   em looks up from her text, frowning deeply.
   wait 4s
   say The god of pestilence, decay, and disease, Morgion is widely feared by almost all humanoid races.
   wait 4s
   say He is known by many names, "Pestilence", and "H'rar" among the Ergothians and Istarians. His most common title is "The Black Wind".
   wait 5s
   say His colors are dark brown, and black. His symbol is a hood with two crimson eyes, or a rat's claw. His domains are disease, plotting, and suffering.
   wait 2s
   em frowns deeply to herself.
   wait 3s
   say His worshipers are little more than crazed cultists. They must work in secret, for if they are ever discovered, they are in peril of their lives.
   wait 2s
   em pauses for a moment, smirking.
   wait 4s
   say Above all, they fear the clerics of Mishakal, their sworn enemies. Morgion and his followers will never succeed in their vile plans for long, as long as holy Mishas and her followers are here to fight them.
   wait 7s
   em clears her throat, looking slightly flushed.
   wait 2s
   say I, uh.. I hope you learned something.
   wait 1s
   em smiles at you before returning to her holy text.
   elseif (%speech% /= bias) then
   wait 2s
   em glances towards you for just a moment.
   wait 1s
   say It's not bias, it's merely the truth.
   wait 1s
   em returns to her study.
  end
~
#4755
m4702 Greet Warning~
0 g 100
~
   wait 4s
   emote turns smartly to face you.
   wait 2s
   say Greetings. Welcome to the Talenrock Cemetary.
   wait 2s
   say Please enjoy your visit. However..
   wait 4s
   say I must warn you, the Cemetary is not safe during the night hours.
   wait 4s
   say Foul creatures and spirits roam the yard at night, and some yet persist unto the daylight hours.
   wait 5s
   say Therefore, please be cautious after you enter. If you linger past sunset, you may be set upon by foul spirits.
   wait 5s
   em nods to you solemnly, and steps aside to allow you entrance.
   halt
~
#4756
Bodak Aggro~
0 c 100
look~
   if (%actor.vnum% == -1) then
   if ("%arg%" == "tomb") then
   msend %actor% A blood-curdling wail echos from the darkness!
   wait 1s
   msend %actor% A sleek, shadowy form emerges from the darkness striding towards you.
   wait 2s
   mkill %actor%
   end
   end
~
#4757
new trigger~
1 j 100
~
   otransform 4767
   halt
~
#4763
m4763 aggro~
0 c 100
look~
   if (%actor.vnum% == -1) then
   if ("%arg%" == "tomb") then
   msend %actor% A wight springs from the shadows and lunges towards you!
   wait 2s
   mkill %actor%
   end
   end
~
#4790
r4790, Sunrise~
2 b 100
~
 wait until 0400
 wait 44s
 wecho &bThe night begins to slowly melt away, and the sky becomes a lighter shade of blue.&n
 wait 14s
 wecho &mThe sky begins to redden slowly, almost by inches. The sun can be sensed behind the horizon to the east.&n
 wait 15s
 wecho &mThe sun begins to rise in the east, lighting the sky in brilliant, warm shades of magenta, orange, and blue.&n
 wait 19s
 wecho &RThe sun peeks over the horizon, warming your bones.&n
 wait 16s
 wecho &RThe sun slowly creeps up over the rooftops of Talenrock, triumphantly dispelling darkness and shadow.&n
 wait 12s
 wecho &YThe sun slowly climbs the sky, filling the area with bright light, as the other colours fade and vanish in morning's blazing glory.&n
 wait 10s
 wecho &CMorning has dawned in Talenrock.&n
 halt
~
#4791
r4791 Sunset Trigger~
2 b 100
~
 wait until 2000
 wait 4s
 wecho &YThe hot, blazing sun begins to sink in the west, casting the area in a warm light.&n
 wait 7s
 wecho &RThe sun moves behind the horizon line, lighting the sky in vibrant reds and purples.&n
 wait 12s
 wecho &MThe sky turns beautiful shades of orange, magenta, pink; colours which are reflected upon the treetops below you.&n
 wait 10s
 wecho &mThe sky's colours become more subdued, and the mountains become more shadowy.&n
 wait 14s
 wecho &mTwilight settles onto the area, casting it in long shadows.&n
 wait 10s
 wecho &bNight slowly descends, casting the area into darkness.&n
 wait 6s
 wecho &cThe moons appear in the sky.&n
 halt
~
#4792
r4792, Breeze~
2 b 3
~
 wecho &WHundreds of colorful, fragrant wildflowers sway gently in the breeze.&n
 halt
~
#4793
r4792, Kingfisher~
2 b 2
~
 wecho &WA majestic, &Broyal blue&W kingfisher soars through the sky above you.&n
 wload mob 4793
 wait 30s
 wecho &WThe kingfisher wheels in the sky above you one last time, then flies away.&n
 wpurge
 halt
~
#4794
m4758 Ferret Chitter~
0 b 2
~
   wecho The curious little ball of fur skitters about, getting into what mischief it can.
   halt
~
$~
