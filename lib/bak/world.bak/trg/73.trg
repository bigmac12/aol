#7300
r7296 Karthaybound traffic stopper~
2 g 100
~
If (%actor.hometown% == 1) then
If (%actor.level% < 11) then
wait 1
wload mob 7305
wecho A slight breeze blows across the highway and the air is filled with a silvery light.
wait 4
wecho As the light dissipates the image of a befuddled, old mage appears before you.
wait 4
wecho &WA befuddled, old mage &Gsays&W, '&CThe road ahead is not safe for you. It is best that you head back the way you came.&W'&n
wpurge
end
end
~
#7301
r7236 Kalbound traffic stopper~
2 g 100
~
If (%actor.hometown% == 2) then
If (%actor.level% < 11) then
wait 1
wload mob 7304
wecho A warm breeze blows across the highway and the sky become increasingly dark.
wait 4
wecho The figure of an elven woman of unparalled beauty forms within the darkness.
wait 4
wecho &WAn elven woman of unparalled beauty &Gsays&W, '&CThe way ahead is not safe for you. It is best if you return the way you came.&W'&n
wpurge
end
end
~
$~
