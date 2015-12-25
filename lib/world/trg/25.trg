#2500
m2500 !cleric trigger (NOT USED)~
0 g 100
~
if (%actor.vnum% == -1)
if (%actor.class% != cleric)
wait 4
say This training room is for clerics only.
wait 2s
msend %actor% The trainer helps you back into the hallway.
wait 2s
mforce %actor% n
end
end
~
#2501
Down Entrance r2545~
2 c 100
pray~
if (%actor.vnum% == -1) then
wait 1s
wsend %actor% You begin to pray.
wechoaround %actor% %actor.name% begins to pray.
wait 1s
wecho A bright light fills the room.
wait 3s
wecho The altar slides into the wall, revealing a staircase.
wdoor 2545 down room 2599 flags a
wait 20s
wecho The altar slides back into place.
wdoor 2545 down purge
end
~
#2526
Monk Teacher Arsla (m2526) greet~
0 g 100
~
if (%actor.vnum% == -1) then
     wait 2
     say Hello and welcome to our monastery.  I am Arsla.  I enlighten any who seek my knowledge.
     wait 6s
     emote smiles at you before &Gspeaking&n, "&CWould you like to hear any of my lessons?&n"
     wait 5s
     say I currently can provide you with knowledge of &BMajere&C, what &Bmonks&C are, our daily &Broutines&C, or about our &Bcombat training&C.&n
else
end
~
#2527
Monk Teacher Arsla (m2526) - Teach~
0 d 100
Majere monks routines combat training~
if (%speech% /= combat training)
     wait 2s
     say Ah, you wish to know about our combat training, do you?
     wait 3s
     say We do not believe in the use of swords or arrows.  Instead, we use blunt, nonmetallic,  weapons, such as staves, or our bare hands.
     wait 9s
     say This may seem strange to some, or even down right crazy...
     wait 4s
     emote smiles to herself for a moment before continuing.
     wait 3s
     say ...But we believe in saving lives.  It is not easy to save a life while shoving a sharpened piece of steel into another's skin at the same time.
     wait 9s
     say The only time a monk is allowed to take the life of another is when they have tried all other options, and innocent lives are in danger.
     wait 8s
     say After years of training, some monks are able to use what we call "chakra".
     wait 5s
     say The monk calms their mind, using ancient breathing techniques.
     wait 4s
     say It is only when the mind is sufficiently calm that the monk is able to focus the necessary mental energy in order to heal the damage that their body has sustained.
     wait 12s
     say It leaves the monk mentally drained for at least most of the day. 
     wait 5s
     say Is there anything else you'd like to learn from me?  I can teach you about &BMajere&C, our &Broutines&C, or give you more information about &Bmonks&C.&n
elseif (%speech% /= majere)
     wait 2s
     say You wish to know about our god, Majere?  Very well, I shall help you understand.
     wait 4s
     say Majere is the god of organization, industry, and control, as well as meditation, faith, and dreams.
     wait 6s
     say Majere creates and inspires all the martial arts and all disciplines that lead to an honest confrontation of oneself.
     wait 7s
     say His worshipers seek simple lives, devoid of luxurious trappings but filled with meditation, self-discipline, and deep thought.
     wait 8s
     say According to some scholars, Majere's crowning creations are the insects of Krynn, who emulate the quiet god's unassuming grace, introspective complexity, and ceaseless industry.
     wait 14s
     say Majere's celestial symbol is the constellation of the Rose.  His colors are copper and red.
     wait 6s
     emote smiles as she motions to her red clothing, then points to the medallion around her neck.
     wait 6s
     say Majere's most commonly recognized symbol is the copper spider.  Although he is also seen as a single rose among Elven, and as a mantis to the Solamnic.
     wait 10s
     say Majere takes the form of a bald and blind man or woman who can make those that are not true to themselves feel unnerved.
     wait 8s
     say Is there anything else you'd like to learn from me?  I can teach you about our &Broutines&C, our &Bcombat training&C, or give you more information about &Bmonks&C.&n
elseif (%speech% /= monks)
     wait 2
     say You wish to know more about monks?  I will tell you what I can.
     wait 3s
     say We follow the teachings of our god and take a vow of chastity...
     wait 3s
     say We do, however, allow the cycle of life between a man and a woman to continue.
     wait 4s
     say Each day we follow our routine, without fail.  I can go over my routine for you shortly, if you'd like.  Just ask me after this lesson.
     wait 8s
     say We believe in mastering our minds, bodies, and spirits.  We do this through our daily routines and simplistic lifestyle.
     wait 8s
     say We live away from the rest of the world.  Our monastery can support us without contact from the outside for ages.  There is little reason for a monk to travel outside of the monastery.
     wait 13s
     say Monks are taught not to tell another how they should live their life unless their council on the matter is sought.
     wait 7s
     say Is there anything else you'd like to learn from me?  I can teach you about &BMajere&C, our &Bcombat training&C, or I can tell you of our &Broutines&C.&n
elseif (%speech% /= routine)
     wait 2
     say You wish to know of our routines?  I believe it would be best for me to give you an example of a monk's daily routine.
     wait 9s
     say Every day I get up one hour before dawn.  Right after waking up, I meditate and pray to Majere for one hour.
     wait 9s
     say Afterwards, I preform combat training for thirty minutes.  Then I go to the dining hall to eat my breakfast of meat, bread, and goat's milk.
     wait 11s
     say If the weather is fair, I tend to the gardens until lunch.  Otherwise, I go inside of the monastery and teach those who wish to be taught.
     wait 11s
     say I have my lunch of cheese and bread, then I come out to the courtyard to teach others, if the weather is fair.  If it isn't, then I stay near the dining hall.
     wait 12s
     chuckle
     wait 2s
     say During the winter I eat onion soup, meat, bread, and dried fruits or nuts for dinner.
     wait 7s
     say During the other seasons, I eat onion soup, meat, bread, and vegetables for dinner.
     wait 7s
     say Afterwards, I spend two hours teaching others and then I retire to bed so I may repeat the process the next day.
     wait 8s
     say A routine is very important to monks.  Breaking it should be avoided if at all possible.
     wait 6s
     say Is there anything else you'd like to learn from me?  I can teach you about &BMajere&C, our &Bcombat training&C, or give you more information about &Bmonks&C.&n 
end
~
$~
