DATA32 @@ITERATOR@@
MOVE32_32(@@INITIAL_VALUE@@, @@ITERATOR@@)

@@Block1@@:
JR_EQ32(@@BOUND@@, @@ITERATOR@@, @@Block2@@)

@@BODY@@

ADD32(1, @@ITERATOR@@, @@ITERATOR@@)
JR(@@Block1@@)

@@Block2@@:
