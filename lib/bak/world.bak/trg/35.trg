#3500
Lev 23 or greater only~
2 g 100
~
if (%actor.vnum% == -1)
  if (%actor.level% < 23)
    wait 1
    wsend %actor% You sense that you are not worthy, and so you retreat down the stairs.
    wechoaround %actor% You sense that one among you is not worthy, and so you leave.
    wteleport all 3578
  end
end
~
$~
