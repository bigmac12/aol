#8102
Ardain's glance~
0 g 20
~
wait 1s
emote looks up as you enter, studying you with a quick glance.
wait 3
emote returns to his work, with an inquiry of, 'May I help you?'
~
#8119
m8120 Bribe Trigger~
0 m 10
~
wait 1s
say Please take a seat and the performance will be starting shortly.
wait 1s
mechoaround %actor% %actor.name% enters the theater. 
mteleport %actor% 8123
msend %actor% You enter the theater.
mechoaround %actor% %actor.name% enters the theater.
mforce %actor% look
~
#8120
r8120 wait for play to start~
2 d 0
shortly~
if (%actor.name% == Boyd)
wait 20s
wforce %actor% say Last call before the curtain goes up!
wasound %actor.name% shouts, 'Last call before the curtain goes up!'
wait 40s
wforce %actor% group all
wforce %actor% ungroup
wdoor 8120 west room 8123
wforce %actor% west
wdoor 8120 west purge
wait 1s
wecho The doors to the theater swing closed. 
end
~
#8121
r8121 Random usher load~
2 b 100
~
wait until 5
wait 5t
wecho The trigger works! It works!
~
#8123
r8123 play~
2 g 100
~
if (%actor.name% == Boyd)
wload mob 8123
wait 2s
wforce pnar say Today, The Istar Play House is proud to present...
wait 2s
wforce pnar say The Great Battle of Huma Against Takhisis!
wait 2s
wforce pnar say This is the final battle of the mighty forces of light against the desperate armies of darkness.
wait 3s
wforce pnar say The battle takes place in the Vingaard Mountains, far west from here.
wait 3s
wforce pnar say On the ground noble knights and soldiers from Ergoth fight against an army of ogres and humans.
wait 3s
wforce pnar say In the air, all dragons of Paladine are fighting against the Dark Queen's evil dragons. The knights are armed with hundreds of Dragonlances.
wait 2s
wload mob 8122
wload obj 8120
wecho Huma arrives on stage, his legs around the base of the neck of the silver dragon following behind him.
wait 3s
wecho Huma stands still, the dragon stopping just behind him.
wait 3s
wecho Huma's polished armor reflects the light on the stage.
wait 3s
if (%random.10% == 1) then
wecho Huma raises his Dragonlance into the air, but loses grip on it.
wait 3s
wecho The Dragonlance falls to the stage, causing the wooden shaft to vibrate loudly.
wait 3s
wecho Huma bends over to pick it up, but when he grabs the Dragonlance, his helmet falls of and lands on the stage loudly.
wait 3s
wecho A soft groan comes from inside the silver dragon.
wait 3s
wecho A look of fear and shock is on the young actor's face.
wait 3s
wecho Huma drops the Dragonlance again and picks up his helmet.
wait 3s
wecho Huma quickly places the helmet back over his dirty blonde hair.
wait 3s
wecho Huma puts his left hand on his helmet and reaches for the Dragonlance with his right hand.
wait 3s
wecho Huma quickly straightens himself and starts again.
wait 2s
end
wecho Huma holds his Dragonlance high into the air.
wait 2s
wforce phuma say I destroyed the evil mage that brought the evil Queen back, but not quick enough. Now I ride towards Takhisis to face her. My fellow knights and their Dragonlances will help me fight Her.
wait 3s
wload mob 8121
wecho Takhisis arrives on the stage, her five heads each have an angry look on them.
wait 3s
wecho A couple of screams can be heard in the crowd, most likely coming from children.
wait 3s
wecho The crowd boos, neither Huma nor Takhisis talk, giving the crowd some time to get it out of their system.
wait 2s
wforce ptak say I have destroyed your allies, you will fight me alone, Mortal.
wait 3s
wforce phuma say I am giving you a chance to leave this world now. I suggest that you take my offer.
wait 3s
wecho Takhisis lets out a loud and sinister laugh.
wait 2s
wforce ptak say You can't harm me! You are but a mortal!
wait 3s
if (%random.10% == 1) then
wforce phuma say This Dragonlance is created by Paladine. Th-...ummm...
wait 2s
wecho Huma says softly, '&CWhat was it that I was going to say?&n'
wait 2s
wecho A voice coming from inside the silver dragon speaks, a bit of anger is obvious in the voice, '&C"The Knights of Solamnia are the hand of Paladine. Paladine can harm you!" You twit!&n'
wait 2s
wforce phuma say Did you really have to call me a twit?
wait 2s
wecho The voice inside of the body of the silver dragon says, '&CNo, but it did feel good.&n'
wait 2s
wecho Huma sighs and shakes his head, then continues.
wait 2s
end
wforce phuma say This Dragonlance is created by Paladine. The Knights of Solamnia are the hand of Paladine. Paladine can harm you!
wait 3s
wecho Huma points his Dragonlance at Takhisis.
wait 3s
wecho Takhisis takes a step back.
wait 3s
wecho Huma and the silver dragon move forward at the same speed, his Dragonlance still pointed at her.
wait 3s
wecho Takhisis turns around and runs from Huma.
wait 3s
wecho Huma begins chasing her around the stage, while still on the silver dragon.
wait 3s
if (%random.10% == 1) then
wecho Takhisis swiftly turns left, Huma not far behind. Huma turns too quick, causing the man inside the silver dragon to fall out.
wait 3s
wecho Huma drags the body of the silver dragon several feet before realizing that something is wrong.
wait 3s
wecho The man that was inside the silver dragon gets on to his feet.
wait 3s
wecho The man says, '&CI cannot believe how bad you are at this! Don't turn so fast! I can't make this huge prop turn like that!&n'
wait 3s
wecho Huma fidgets a little, adjusting his armor.
wait 3s
wecho Huma murmers, '&CSorry.&n'
wait 3s
wecho The man gets back under the silver dragon costume, grumbling the whole time.
wait 3s
wecho Takhisis tries to get things rolling again and says, '&CThe breath from my five heads is painful! You saw how much that hurt your poor silver dragon!&n'
wait 3s
wecho Takhisis lets out a loud laugh.
wait 3s
wecho There is a moment of silence.
wait 3s
wecho Huma improvises, '&CErr...Paladine protected the silver dragon! See? You barely caused a scratch!&n'
wait 3s
wecho Takhisis snarls.
wait 2s
end
wforce phuma say You can't run forever!
wait 3s
wforce ptak say Yes, I can, but toying with you bores me. Die mortal!
wait 3s
wecho Takhisis stops running and faces Huma.
wait 3s
wforce Huma stops as well and points the Dragonlance right at her chest.
wait 3s
if (%random.10% == 1) then
wecho Huma inhales deeply, getting ready to shout a battle cry, but before he can, he starts coughing.
wait 3s
wecho Huma grabs his throat.
wait 3s
wecho Takhisis says with a hint of concern in her voice, '&CAre you okay?&n'
wait 3s
wecho Huma coughs some more, his hands still around his neck.
wait 3s
wecho Takhisis takes a few steps forward.
wait 3s
wecho Huma cries out, '&CI think I swallowed a bug!!&n'
wait 3s
wecho Takhisis pauses for a moment, then sighs and takes a few steps back.
wait 3s
wecho One of the silver dragon's hind legs kicks Huma in the back of his knee, almost causing him to fall over.
wait 3s
wforce phuma say OW! It's not my fault that a bug flew in my mouth and almost made me get sick!
wait 2s
wecho Takhisis sighs again.
wait 2s
wforce ptak say Try it again, just this time don't swallow whatever flies in your helmet.
wait 2s
end
wecho Huma lets out a battle cry and then charges at Takhisis.
wait 2s
wecho Takhisis lets out a loud roar and runs at Huma.
wait 3s
wforce phuma say Est Sularus Oth Mithas
end
~
#8124
r8123 Speech Trigger for ending~
2 d 0
Mithas~
if (%actor.name% == Huma)
wait 2s
if (%random.10% == 1) then
wecho Huma runs towards Takhisis, but trips.
wait 2s
wecho The Dragonlance hits the ground first, the tip first. Huma doesn't let go and his weight shatters the wooden shaft in half.
wait 2s
wecho Huma hits the ground a moment later.
wait 2s
wecho None of the actors move for a few seconds. The first one to move is Huma, who gets back on to his feet, still holding half of the broken Dragonlance.
wait 2s
wecho Huma raises the visor on his helmet, staring at the shaft he is holding in his hand.
wait 2s
wecho The man inside the silver dragon gets out and tackles Huma. Being much larger than Huma, he doesn't have any difficulty in knocking him down.
wpurge fake
wait 2s
wecho The muscular man is furious, and he yells to Huma, '&CDo you know how much one of those cost?! We're not made of money you know!&n'
wait 3s
wecho The man shakes Huma until his helmet falls off.
wait 2s
wecho The young actor who plays Huma has a stunned look on his face. He doesn't even fight back.
wait 2s
wecho Takhisis runs up and puts her hands on the man's shoulders.
wait 2s
wecho The man looks at her for a second, then gets up and walks off stage, dragging the silver dragon costume with him.
wait 2s
wecho Takhisis bends over, keeping her right hand on the heads of her costume to prevent it from falling off.
wait 3s
wecho Takhisis softly says to the now crying Huma, '&CHey, don't worry about it. Now just play along okay?&n'
wait 2s
wecho Huma nods slightly and makes a sniffling sound.
wait 2s
wecho Takhisis stands straight and puts one foot on Huma's chest.
wait 2s
wecho Takhisis mades a triumphant roar.
wait 2s
wforce ptak say Paladine couldn't protect you forever! Your lance shattered against my magic! I knocked you off your dragon and let you fall to the ground! I killed the dragon, burned it into nothing but ashes!
wait 2s
wecho Huma cries out.
wait 2s
wforce ptak say It is only through my will that you are alive right now. I will torture you until you die a slow and painful death. After that I will torture you for the rest of eternity!
wait 3s
wecho Huma says weakly, '&CNo...&n'
wait 2s
wecho Huma cries, this time an acted cry.
wait 2s
wforce ptak say Yes! The world is mine!
wait 3s
wforce pnar say This concludes our play of how Huma saved...
wait 2s
wecho The narrator clears his throat.
wait 2s
wforce pnar say Err...
wait 2s
wforce pnar say This concludes our presenation of how the Dark Queen defeated the brave Huma!
wait 3s
wforce pnar say Don't dispair though! While Takhisis was distracted with torturing Huma-
wait 2s
wecho Huma cries out again, as if he was feeling great pain.
wait 2s
wforce pnar say -a rider on a gold dragon stabbed Takhisis through the heart and-
wait 2s
wecho &yTakhisis shouts, 'HEY! Let me win this time!'&n
wait 2s
wforce pnar say But I can't...
wait 2s
wecho Takhisis runs off stage.
wait 2s
wforce pnar say Huh?
wait 2s
wecho There is a shuffling sound made off stage.
wait 2s
wforce pnar say Ack!
wait 2s
wecho A thin middle aged man with glasses runs on stage, being chased by Takhisis, who has removed the five heads of her costume for easier movement.
wait 2s
wforce ptak say Let me win or I'll throw you to the crowd! They want to see me win once in a while too you know!
wait 3s
wecho The crowd joins in and cheers at Takhisis.
wait 3s
wforce pnar say You have to catch me first!
wait 3s
wforce ptak say Death Knight Huma! Get him!
wait 2s
wecho Huma, who just got on his feet, looks at Takhisis and blinks, then after a moment, he grins and grabs the narrator as he runs by.
wait 3s
wforce phuma say We captured you! Now give up!
wait 3s
wecho The narrator struggles against Huma, but can't free himself.
wait 3s
wecho Takhisis and Huma let out evil cackles.
wait 3s
wforce pnar say Okay okay! Takhisis-
wait 2s
wecho Takhisis clears her throat.
wait 2s
wforce pnar say -Queen- Takhisis rules over the world and life as we know it stops.
wait 2s
wecho Takhisis breathes heavily, tired after chasing the narrator around, but enjoying the moment.
wait 3s
wecho The actors take a bow and a small hatch on the stage is opened from the inside.
wait 3s
wecho The actors gather the props and go down the hatch.
wait 20s
wecho The hatch is closed from the inside.
wdoor 8123 down purge
wait 2s
else
wecho The Dragonlance hits Takhisis near her armpit, she starts snarling in pain.
wait 3s
wforce ptak say How? You're just a mortal!
wait 2s
wecho Takhisis screams, '&CHOW?!&n'
wait 3s
wforce phuma say Good always wins over evil.
wait 3s
wecho Takhisis screams in pain.
wait 3s
wecho Takhisis says in a snarl, '&CI'll kill you!&n'
wait 3s
wecho Takhisis moves forward, the Dragonlance going under her arm.
wait 3s
wecho With her free arm, Takhisis starts scratching at the silver dragon's head and Huma.
wait 3s
wecho Both Huma and Takhisis scream in pain.
wait 3s
wecho Everybody lies down on the ground. The Dragonlance sticks up out of Takhisis who is laying on her back.
wait 2s
wecho Huma slowly gets to his feet.
wait 2s
wecho Takhisis moans in pain, barely moving. The silver dragon doesn't move at all.
wait 2s
wforce ptak say AGH!
wait 2s
wecho Takhisis lets out a long scream of pain.
wait 2s
wforce ptak say Mortal...I beg of you...let me go.
wait 3s
wecho Huma slowly walks to Takhisis.
wait 3s
wforce phuma say I will let you go if you promise never to return again.
wait 2s
wforce ptak say But...
wait 2s
wforce phuma say Never! Either accept my offer, or my allies will stab you with their Dragonlances. Imagine, hundreds of those piercing your skin...
wait 8
wforce ptak say Alright! ...I will leave.
wait 2s
wecho Huma removes the Dragonlance from Takhisis.
wait 2s
wecho Takhisis gets up right after he removes it and releases a mighty roar just before leaving.
wait 3s
if (%random.5% == 1) then
wecho Somebody nearby in the crowd loudly says, '&CHey! I know this part!&n'
wait 3s
wecho A few people mutter something but continue watching.
wait 3s
wecho The loud viewer says, '&CYeah! This is where Huma dies!&n'
wait 2s
wecho Many of the people near him turn to face him and glare at him. One person throws a half eaten apple at him, hitting him in the shoulder.
wait 2s
wecho The loud viewer mutters something and looks at his feet.
wait 2s
end
wecho Once Takhisis leaves, Huma falls to the ground, clutching his chest.
wait 3s
wecho After a few moments, Huma stops moving.
wait 3s
if (%random.10% == 1) then
wforce pnar say Huma the brave knight, di-
wait 2s
wecho Huma sits up and looks around.
wait 2s
wecho A voice inside of the body of the silver dragon says, '&CHey! You're still dead remember?&n'
wait 2s
wecho Huma turns to the silver dragon then says, '&COh! Right!&n'
wait 2s
wecho Huma lies down, pretending to be dead again.
wait 2s
wecho The narrator clears his throat.
wait 2s
end
wforce pnar say Huma, the brave knight, died from his wounds that were caused by Takhisis.
wait 2s
wforce pnar say His death was a great loss to the knighthood. Even today young squires look up to him. He was an example of the perfect knight.
wait 2s
wforce pnar say Paladine has grown stronger since then. Takhisis won't be able to enter Krynn again as long as the honor-bound knights and followers of light from Istar have power!
wait 2s
wforce pnar say This concludes our presentation.
wait 2s
wecho The man under the body of the silver dragon gets out while Huma gets up and stands beside him.
wait 3s
wecho Takhisis arrives back on stage.
wait 3s
wecho They all take a bow to the crowd, who gives a round of applause.
wait 3s
wecho Huma waves to the crowd.
wait 3s
wecho A hatch on stage is opened up behind the actors.
wdoor 8123 down room 8124 flags a
wait 3s
wecho The actors gather the props and leave down the open hatch.
wait 20s
wecho The hatch is closed from the inside.
wdoor 8123 down purge
wait 3s
wforce pnar say If you enjoyed the show, please tell a friend and come back again! Thank you all!
end
wforce Boyd east
wait 1
wpurge
end
~
#8125
m8118 Greet Trigger~
0 g 100
~
if (%actor.vnum% == -1) then
wait 1s
say The Istar Play House is proud to present:
wait 1s
say The Battle of Huma against Takhisis!
wait 1s
say Only one gold!  Come on and see the performance!
wait 1s
say The usher will accept your money and show you in.
~
#8170
r8195, 96, 97 Forget Trigger for Grove~
2 g 100
~
If (%actor.vnum% == -1) then
if (%actor.class% != mage) then
  return 0
    if (%random.10% == 1) then
      wsend %actor% You feel lost and disoriented and cannot remember your destination.
      wechoaround %actor% %actor.alias% appears lost and disoriented.
      wteleport %actor% 8195
      wait 1s
      wforce %actor% look
      wechoaround %actor% %actor.alias% wanders in looking quite a bit confused.
    elseif (%random.10% == 2) then
      wechoaround %actor% %actor.alias% seems unable to remember which direction they were going.
      wsend %actor% You are unable to remember which direction you were going.
      wteleport %actor% 8196
      wait 1s
      wechoaround %actor% %actor.alias% wanders in looking quite a bit confused.
      wforce %actor% look
    elseif (%random.10% == 3) then
      wechoaround %actor% %actor.alias% seems unable to remember which direction they were going.
      wsend %actor% You are unable to remember which direction you were going.
      wteleport %actor% 8197
      wait 1s
      wechoaround %actor% %actor.alias% wanders in looking quite a bit confused.
      wforce %actor% look
    else
    wsend %actor% You have forgotten where you were going.
    wteleport %actor% 8199
    wforce %actor% look
    wait 1s
    wechoaround %actor% %actor.alias% wanders in looking quite a bit confused.
  end
end
end
~
#8171
Messages for r8198~
2 g 100
~
if (%direction% == west) then
wait 1s
wechoaround %actor% %actor.alias% appears lost and disoriented.
wsend %actor% You feel lost and disoriented and cannot remember your destination.
end
~
#8191
m8191 Greet~
0 g 100
~
wait 2
say Welcome to the Laughing Spirits.
wait 4
emo offers a warm smile.
~
#8193
m8193 request for help~
0 e 1
sits~
wait 2
emo nods, looking quite relieved.
wait 1s
say Then perhaps you can help me. My family, in fact the whole village, has taken ill.
wait 1s
say No one has been able to cure their malady, and all who have tried have fallen ill themselves.
wait 12
say I dare not take anyone else to them.
wait 12
say I wish I knew what I have been spared.
wait 12
sigh
wait 12
say I am weakened, yes, but not like the rest.
wait 10
say My only guess is that my strength comes from the waters of Taragral Springs.
wait 10
say I returned from my stay there to find them all so sick.  I began to feel weak shortly thereafter.
wait 1s
headrub me
wait 10
say Now I am too weak to make it back to the Springs.  Could you? Would you?
wait 1s
emo watches you, eagerly awaiting a response.
~
#8195
m8193 Greet~
0 g 100
~
wait 2
say Hello there, friend.
wait 1s
say Have you come to the Laughing Spirits to offer your services?  If so, please sit a moment so I may explain my situation.
wait 3s
~
#8196
m8193 nod~
0 e 1
nods~
wait 2
say Many thanks, friend.  I will return here each day to await your return.
~
#8197
m8193 shake~
0 e 1
shakes~
wait 2
say I understand.  The journey is long.
wait 10
say If you know of any willing to help, please let them know we are in need.
~
#8198
Raven's trig~
0 g 100
~
wait 3
say i was forsaken by paladine...
wait 8
say so i'm wondering...
wait 12
say would it be too much trouble to change me to evil?
wait 8
say thanks!  this is great!
wait 16
say okay, evil sucks, I see the light.  can I be good?
wait 5
say thanks!
wait 12
say arrghh!
wait 3
sigh
wait 15
say forsaken again!  can I be changed back to...
wait 1
say evil?
wait 2
say good?
wait 2
say evil?
wait 2
say good?
wait 2
say evil?
wait 1
say good?
wait 1
say evil?
say good?
say evil?
say good?
say evil?
say good?
emote collapses in a heap, having worn himself out.
~
$~
