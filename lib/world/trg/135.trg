#13500
black mage component guy greeting m13500~
0 g 100
~
Eval pcclass %actor.class%
Eval pcalign %actor.align%
If (%pcclass% != Mage) then
Wait 3s 
Emote glances up from the book he was reading, as you enter the room.
Wait 3s
Emote sets the book down quietly and reaches into his robes for something.
Wait 3s
Emote pulls out a slender wand made of yew wood and points it at you.
Wait 6s
Emote places the wand back into his robes and retrieves the book, opening it to the page he was previously studying.
Elseif (%pcalign% >= 0) then
Wait 3s
Emote raises his head, irritated at being disturbed.
Wait 3s
Emote marks the page in the book on his lap, closing it.
Wait 4s
Emote examines you for a moment, nodding in acknowledgement.
Wait 5s
Say I am in the middle of my studies and wish to be left alone.
Wait 3s
Emote looks over at the door and then glances at you meaningfully.
Wait 5s
Say Please, leave my chambers.
Wait 5s
Frown
Wait 3s 
Say Very well, have it your way.
Wait 3s 
Emote reaches into his robes and pulls out a slender wand made of yew wood.
Wait 3s
Say This is my final warning, leave me be.
Wait 5s
Emote points the wand at you.
Wait 9s
Emote places the wand back into his robes and retrieves his book, returning to his studies.
Else
Wait 3s
Emote raises his head, irritated at being disturbed.
Wait 3s
Emote marks the page in the book at his lap, closing it.
Wait 4s
Emote examines you for a moment, nodding in acknowledgement.
Wait 5s
Say Greetings. I am studying just now, a matter of importance as you well know.
Wait 3s
Emote ponders over you a moment before continuing.
Wait 4s
Say Currently I am studying somatic components, most important to our work.
Wait 4s
Say I have little time to spare, but as a colleague in the Order of Nuitari, I suppose I can spare a moment if you have questions concerning that topic.
Wait 6s
Say If you'd like to know something about somatic components, just ask.
Wait 3s
Say Otherwise I bid you leave me to my studies, please.
Wait 5s
Emote retrieves the book, opening it to the page where he left off.
Wait 3s
End
End
End
~
#13501
magical blast r13501~
2 g 100
~
If (%actor.vnum% == -1) then
Wait 11s
Eval pcclass %actor.class%
Eval pcalign %actor.align%
If (%pcclass% != Mage) then
Wechoaround %actor% A black mage utters a single word causing a blast of magic to shoot out from the tip of his wand.
Wait 1s
Wdamage %actor% 15
Wsend %actor% The magical blast strikes you in the chest, hurling you out the room!
Wechoaround %actor% %actor.alias% is hurled out of the room by the magical blast!
Wforce %actor% west
Elseif (%pcalign% >=0) then
Wait 33s
Wechoaround %actor% A black mage utters a single word causing a blast of magic to shoot out from the tip of his wand.
Wechoaround %actor% A black mage utters a single word causing a blast of magic to shoot out from the tip of his wand.
Wait 1s
Wdamage %actor% 15
Wsend %actor% The magical blast strikes you in the chest, hurling you out the room!
Wechoaround %actor% %actor.alias% is hurled out of the room by the magical blast!
Wforce %actor% west
end
end
~
#13502
part 1 black component m13500~
0 d 100
sleep recharge item prismatic spray dancing sword~
eval pcclass %actor.class%
   eval pcalign %actor.align%
   if (%speech% /= sleep) then
   if (%pcclass% == Mage) and
   (%pcalign% < 0) then
   wait 3s
   emote pauses a moment to gather his thoughts.
   wait 4s
   say Ahh, sleep. I think most mages learn the component of this spell at a young age.
   wait 2s
   chuckle
   wait 2s
   say Nevertheless, the recital of the sleep spell requires a handful of rose petals, or sand as it's focus.
   wait 4s
   say They are to be sprinkled over the victim, or at least sprinkled in their direction as the words of the spell are spoken.
   wait 4s
   say I believe the tower shop carries a supply of them, and of course you can always get them yourself.
   End
   elseif (%speech% /= recharge item) then
   if (%pcclass% == Mage) and
(%pcalign% < 0) then
   wait 3s
   emote pauses a moment while he gathers his thoughts.
   Wait 4s
   Say Recharge item is a risky spell if you do not know what you're doing. You'll need a thick hoop of adamantite which can be found in the Tower shops. Place the hoop over the item you wish to recharge and then speak the words.
   end
   elseif (%speech% /= prismatic spray) then
   if (%pcclass% == Mage) and
(%pcalign% < 0) then
   wait 3s
   emote pauses a moment to gather his thoughts.
   wait 4s
   say The somatic component for the prismatic spray spell is a small prism. These can be found at the tower shop, among other places.
   wait 4s
   end
   elseif (%speech% /= dancing sword) then
   if (%pcclass% == Mage) and
(%pcalign% < 0) then
   wait 3s
   emote pauses a moment to gather his thoughts.
   Wait 4s
   Say The component required for the dancing sword spell is quite obvious. It requires a figurine of a sword. These can be found in the Tower shops as well as near the keeps of those damned knight.
   Wait 3s
   Emote curses the knights under his breath.
   end
   else
   end
   end
   end
~
#13503
part 2 black component m13500~
0 d 100
enchant weapon energy drain charm blackmantle vampiric aura mass charm somatic~
eval pcclass %actor.class%
   eval pcalign %actor.align%
   if (%speech% /= enchant weapon) then
   if (%pcclass% == Mage) and
   (%pcalign% < 0) then
   wait 3s
   emote stops a moment to gather his thoughts.
   wait 3s
   say The incantation to enchant weaponry requires a pinch of powdered lime, which luckily can be found at most mageware shops. The darkgnomes make use of it as well, for some nefarious purpose.
   wait 6s
   say It is to be sprinkled over the weapon being enchanted, while speaking the words of the spell.
   end
   elseif (%speech% /= energy drain) then
   if (%pcclass% == Mage) and
   (%pcalign% < 0) then
   wait 3s
   emote pauses a moment to gather his thoughts.
   Wait 4s
   Say Ah, one of my favorite spells. You'll enjoy seeing the terror in the eyes of your foes as you use one of our mightier spells. It requires the finger bone of a lich though and it isn't an easy component to get a hold of.
   Wait 2s
   Emote stops to think for a moment.
   Wait 2s
   Say Look for them in crypts and tombs. I'm sure you'll come across one, though whether it will be still attached to the lich is something you'll have to be wary of.
   Wait 2s
   Emo chuckles eerily.
   end
   elseif (%speech% /= charm) then
   if (%pcclass% == Mage) and
   (%pcalign% < 0) then
   wait 3s
   emote pauses a moment to gather his thoughts.
   wait 4s
   say The incantation for the charming of others requires a pendulum of crystal as it's focus. It is to be rocked before the eyes of victim, while reciting the words of the spell.
   wait 5s
   say These can be difficult to find, however I have seen them in mageware shops, as well as the tower shop from time to time.
   End
   Elseif (%speech% /= blackmantle) then
   If (%pcclass% == Mage) and
(%pcalign% < 0) then
   wait 3s
   emote pauses a moment as he gathers is thoughts.
   Wait 4s
   Say Ah, blackmantle. A wonderful curse. The component used for this spell is a small black onyx, which can be found in the Tower shop. Onyx, being a preciou
   Wait 3s
   Emote offers a broad smile.
   End
   elseif (%speech% /= vampiric aura) then
   if (%pcclass% == Mage) and
   (%pcalign% < 0) then
   wait 3s
   emote pauses a moment while he gathers his thoughts.
   Wait 4s
   Say A frightful spell, vampiric aura.
   Wait 1s
    Say For the victim of the spell that is.
   Wait 1s
    Chuckle
    Wait 3s
    Say The somatic component for this is dust from a sunburned vampire. Sprinkle the dust upon yourself while reciting the words to the spell. 
    end
   elseif (%speech% /= mass charm) then
   if (%pcclass% == Mage) and
   (%pcalign% < 0) then
   wait 3s
   emo pauses a moment while he gathers his thoughts.
   Wait 4s
   Say The spell to charm a number of people requires a bit more attention, both to the actual spellcasting and the component used. In this case, a vial of pixie dust. A rare item indeed, however the Tower shops keep a good stuck of the stuff.
   end
   elseif (%speech% /= somatic) then
   if (%pcclass% == Mage) and
   (%pcalign% < 0) then
   wait 3s
say Very well then. I can tell you alittle about somatic components, though I can only tell you about the ones I have studied so far.
   wait 7s
   say Currently, I can tell you about the components for Enchant Weapon, Energy Drain, Charm, Sleep, Recharge Item, Vampiric Aura, Dancing Sword, Black Mantle, and Mass Charm. Just ask about the spell by name, and then I'll tell you about it's components.
   end
   else
   end
   end
   end
   end
~
#13504
part 1 red component m13501~
0 d 100
deceptive imagery cripple greater elemental prismatic spray dancing sword~
eval pcclass %actor.class%
   eval pcalign %actor.align%
   if (%speech% /= deceptive imagery) then
   if (%pcclass% == Mage) and
   (%pcalign% == 0) then
   wait 3s
   emote pauses a moment while he gathers his thoughts.
   Wait 4s
   say Deceptive imagery is a combination of two spells really. As such it requires a mirrored rod as a component. Some are available in the Tower shops. Hold the rod in your hand before speaking the words of the spell
   End
   elseif (%speech% /= cripple) then
   if (%pcclass% == Mage) and
   (%pcalign% == 0) then
   wait 3s
   emote pauses a moment while he gathers his thoughts.
   Wait 4s
   Say Cripple, a very effective spell. It can disable an opponent quite well. It requires a broken marionette. They can be found in the Tower shops as well. Hold the puppet in your hand as you utter the incantation.
   end
   elseif (%speech% /= greater elemental) then
   if (%pcclass% == Mage) and
   (%pcalign% == 0) then
   wait 3s
   emote pauses a moment while he gathers his thoughts.
   Wait 4s
   Say A very fine spell indeed, though also very dangerous. Like any spells that summon beings from another plane, a risk is involved that you may summon something you hadn't originally intended. 
   Wait 2s
   Say You'll need a shard of elemental matter, which may or may not be found in Tower shops. If you find an elemental roaming about, chances are that it may hold a small piece of its home.
   end
   elseif (%speech% /= prismatic spray) then
   if (%pcclass% == Mage) and
(%pcalign% == 0) then
   wait 3s
   emote pauses a moment to gather his thoughts.
   wait 4s
   say The somatic component for the prismatic spray spell is a small prism. These can be found at the tower shop, among other places.
   wait 4s
   end
   elseif (%speech% /= dancing sword) then
   if (%pcclass% == Mage) and
(%pcalign% == 0) then
   wait 3s
   emote pauses a moment to gather his thoughts.
   Wait 4s
   Say The component required for the dancing sword spell is quite obvious. It requires a figurine of a sword. These can be found in the Tower shops as well as near the keeps of the knights.
   end
   else
   end
   end
   end
~
#13505
part 2 red component m13501~
0 d 100
haste magical suscept phantasmal snake recollection somatic~
eval pcclass %actor.class%
   eval pcalign %actor.align%
   if (%speech% /= haste) then
   if (%pcclass% == Mage) and
   (%pcalign% == 0) then
   wait 3s
   emo pauses a moment while he gathers his thoughts.
   Wait 4s
   Say The haste spell requires the feather of a hummingbird. These can be found in shops or during your travels. Hold the feather firmly between your thumb and your index finger while speaking the words of the spell.
   end
   elseif (%speech% /= magical suscept) then
   if (%pcclass% == Mage) and
   (%pcalign% == 0) then
   wait 3s
   emote pauses a moment while he gathers his thoughts.
   Wait 4s
   Say One of the few curses in our arsenal. I never found much use for the spell, however I will tell you what I know of it. You'll need a sliver of solid ether, I believe there's a city in Solamnia that is rumored to be a home of one.
   Wait 2s
    Emote seems to be lost in his thoughts.
    Wait 4s
   Say Hmm? Oh! Yes, of course. Hold the ether out towards the target of the spell and utter the incantation. 
   Wait 1s
   Nod
   end
   elseif (%speech% /= phantasmal snake) then
   if (%pcclass% == Mage) and
   (%pcalign% == 0) then
   wait 3s
   emote pauses a moment while he gathers his thoughts.
   Wait 4s
   Say The somatic component for the phantasmal snake is an ivory-carved snake fang. They can be found in the Tower shops along with many other components. Hold the fang in your hand while speaking the spell.
   end
   elseif (%speech% /= recollection) then
   if (%pcclass% == Mage) and
   (%pcalign% == 0) then
   wait 3s
   emote pauses a moment while he gathers his thoughts.
   Wait 4s
   Say Recollection, a useful spell though at times a bit situational. It requires a tiny gold-leaf book. Hold the book in your hand while speaking the words to the spell.
   end
   elseif (%speech% /= somatic) then
   if (%pcclass% == Mage) and
   (%pcalign% == 0) then
   wait 3s
say Very well then. I can tell you alittle about somatic components, though I can only tell you about the ones I have studied so far.
   wait 7s
   say Currently, I can tell you about the components for Prismatic Spray, Dancing Sword, Haste, Magical Suscept, Deceptive Imagery, Cripple, Greater Elemental, Phantasmal Snake, and Recollection. Just ask about the spell by name, and then I'll tell you about its components.
   end
   else
   end
   end
   end
   end
~
#13506
part 1 white component m13502~
0 d 100
sleep recharge item deceptive imagery phantasmal snake~
eval pcclass %actor.class%
   eval pcalign %actor.align%
   if (%speech% /= sleep) then
   if (%pcclass% == Mage) and
   (%pcalign% > 0) then
   wait 3s
   emote pauses a moment to gather his thoughts.
   wait 4s
   say Ahh, sleep. I think most mages learn the component of this spell at a young age.
   wait 2s
   chuckle
   wait 2s
   say Nevertheless, the recital of the sleep spell requires a handful of rose petals, or sand as it's focus.
   wait 4s
   say They are to be sprinkled over the victim, or at least sprinkled in their direction as the words of the spell are spoken.
   wait 4s
   say I believe the tower shop carries a supply of them, and of course you can always get them yourself.
   End
   elseif (%speech% /= recharge item) then
   if (%pcclass% == Mage) and
(%pcalign% > 0) then
   wait 3s
   emote pauses a moment while he gathers his thoughts.
   Wait 4s
   Say Recharge item is a risky spell if you do not know what you're doing. You'll need a thick hoop of adamantite which can be found in the Tower shops. Place the hoop over the item you wish to recharge and then speak the words.
   end
   elseif (%speech% /= deceptive imagery) then
   if (%pcclass% == Mage) and
   (%pcalign% > 0) then
   wait 3s
   emote pauses a moment while he gathers his thoughts.
   Wait 4s
   say Deceptive imagery is a combination of two spells really. As such it requires a mirrored rod as a component. Some are available in the Tower shops. Hold the rod in your hand before speaking the words of the spell
   elseif (%speech% /= phantasmal snake) then
   if (%pcclass% == Mage) and
   (%pcalign% > 0) then
   wait 3s
   emote pauses a moment while he gathers his thoughts.
   Wait 4s
   Say The somatic component for the phantasmal snake is an ivory-carved snake fang. They can be found in the Tower shops along with many other components. Hold the fang in your hand while speaking the spell.
   end
   else
   end
   end
   end
~
#13507
part 2 white component m13502~
0 d 100
enchant weapon antimagic aura spell turning charm mass charm somatic~
eval pcclass %actor.class%
   eval pcalign %actor.align%
   if (%speech% /= enchant weapon) then
   if (%pcclass% == Mage) and
   (%pcalign% > 0) then
   wait 3s
   emote stops a moment to gather his thoughts.
   wait 3s
   say The incantation to enchant weaponry requires a pinch of powdered lime, which luckily can be found at most mageware shops. The darkgnomes make use of it as well, for some nefarious purpose.
   wait 6s
   say It is to be sprinkled over the weapon being enchanted, while speaking the words of the spell.
   End
   elseif (%speech% /= antimagic aura) then
   if (%pcclass% == Mage) and
   (%pcalign% > 0) then
   wait 3s
   emo pauses for a moment while he gathers his thoughts.
   Wait 4s
   Say A potent spell, much like silence. However, instead of rendering the person unable to speak it renders them unable to cast any sort of spell. It requires an eye of the beholder, which are found in the Tower shops.
   End
   elseif (%speech% /= spell turning) then
   if (%pcclass% == Mage) and
   (%pcalign% > 0) then
   wait 3s
   emote pauses a moment to gather his thoughts.
   wait 4s
   say One of the most potent spells in our arsenal. The somatic component for this spell is a rod of antipolarized metal. You may be able to find one in the Tower shops or hidden away somewhere protected by fearsome guardians.
   end
   elseif (%speech% /= charm) then
   if (%pcclass% == Mage) and
   (%pcalign% > 0) then
   wait 3s
   emote pauses a moment to gather his thoughts.
   wait 4s
   say The incantation for the charming of others requires a pendulum of crystal as it's focus. It is to be rocked before the eyes of victim, while reciting the words of the spell.
   wait 5s
   say These can be difficult to find, however I have seen them in mageware shops, as well as the tower shop from time to time.
   End
   elseif (%speech% /= mass charm) then
   if (%pcclass% == Mage) and
   (%pcalign% > 0) then
   wait 3s
   emo pauses a moment while he gathers his thoughts.
   Wait 4s
   Say The spell to charm a number of people requires a bit more attention, both to the actual spellcasting and the component used. In this case, a vial of pixie dust. A rare item indeed, however the Tower shops keep a good stuck of the stuff.
   end
   elseif (%speech% /= somatic) then
   if (%pcclass% == Mage) and
   (%pcalign% > 0) then
   wait 3s
say Very well then. I can tell you alittle about somatic components, though I can only tell you about the ones I have studied so far.
   wait 7s
   say Currently, I can tell you about the components for Enchant Weapon, Antimagic Aura, Charm, Sleep, Recharge Item, Deceptive Imagery, Phantasmal Snake, Spell Turning, and Mass Charm. Just ask about the spell by name, and then I'll tell you about its components.
   end
   else
   end
   end
   end
   end
~
#13508
general mage comps~
0 d 100
locate object fire shield identify scry legend lore portal~
eval pcclass %actor.class%
   if (%speech% /= scry) then
   if (%pcclass% == Mage) then
   wait 3s
   emote pauses a moment to gather his thoughts.
   wait 4s
   say Scry requires a fragment of map as it's focus, though not just any map will do. It must be an aged or even ancient map, making the component for this spell a bit of a pain.
   wait 5s
   emote muses to himself for a moment.
   wait 3s
   say I once saw a pirate with a suitable map..
   wait 4s
   chuckle
   wait 2s
   say Nevermind.
   End
   elseif (%speech% /= identify) then
   if (%pcclass% == Mage) then
   wait 3s
   emote pauses a moment to gather his thoughts.
   wait 7s
   say The spell to identify an object requires a perfectly cut diamond as it's focus. These can be found in the tower shops, as well as some mageware shops. Occasionally, you can find a diamond of suitable quality at the jewelers as well.
   End
   Elseif (%speech% /= portal) then
   If (%pcclass% == Mage) then
   Wait 3s 
   Emote pauses a moment while he gathers his thoughts.
   Wait 4s
   Say The portal spell requires a special gem called the nexus gem. They can be found in the Tower shops, though not in huge quantities. Interplanar travel is dangerous, however and I advise caution.
   End
   Elseif (%speech% /= locate object) then
   If (%pcclass% == Mage) then
   Wait 3s
   Emote pauses a moment while he gathers his thoughts.
   Wait 4s
   Say The locate object spell requires a fragment of a map. The map must be age for some time for the spell to work successfully. Maps such as these can be found in the Tower shops.
   End
   Elseif (%speech% /= fire shield)
   If (%pcclass% == Mage) then
   Wait 3s
   Emote pauses a moment while he gathers his thoughts.
   Wait 4s
   Say The fire shield spell utilizes a shard of dragon bone. They are sold in the Tower shops along with a number of other components. 
   End
  Elseif (%speech% /= legend lore)
   If (%pcclass% == Mage) then
   Wait 3s
   Emote pauses a moment while he gathers his thoughts.
   Wait 4s
   Say Legend lore requires a pure mithril diamond as a focus. I hear a jeweler in Istar is selling one. 
   End
   Else 
   End
   End
~
#13509
mage trainer greets leading into components ~
0 g 100
~
eval pcclass %actor.class%
if (%pcclass% != Mage) then
wait 3s
emote glances up at you with an appraising eye.
wait 4s
say You are no student of the Art. I have nothing for you.
else
wait 3s
emote glances up at you with an appraising eye.
wait 4s
nod
wait 2s
say Greetings. If you've any questions pertaining to spells that require components, I am more than willing to assist in that endeavor. However, my studies have only encompasses the spells of Locate Object, Fire Shield, Identify, Scry, Legend Lore, and Portal. Ask for the spells by name and I will tell you what I know.
wait 6s
say If you wish to acquire knowledge of other spells, I suggest traveling to the Tower of High Sorcery in Istar. I hear that some of the mages will share knowledge of spells to those of their own order.
wait 4s
emote nods once more and waits patiently for any questions.
end
end
~
$~
